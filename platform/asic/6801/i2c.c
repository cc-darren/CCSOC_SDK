/******************************************************************************
*  Copyright 2017, CloudChip, Inc.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of CloudChip, Inc. (C) 2017
******************************************************************************/

/******************************************************************************
*  Filename:
*  ---------
*  i2c.c
*
*  Project:
*  --------
*  cc6801
*
*  Description:
*  ------------
*  This is I2C driver
*
*  Author:
*  -------
*  CloudChip
*
*===========================================================================/
*  20170206 PAT initial version
******************************************************************************/
#define TracerFormat(fmt) "I2C: " fmt

#include "drvi_clock.h"
#include "drvi_i2c.h"
#include "i2c.h"
#include "tracer.h"

typedef enum
{
    CC6801_I2C_100K = I2C_FREQ_100K,
    CC6801_I2C_200K = I2C_FREQ_200K,
    CC6801_I2C_400K = I2C_FREQ_400K,
} E_I2cFreq;

extern E_ClockSupported g_dwCurrentClock;
extern const T_ClockConfig g_aClockTable[CLOCK_TOTAL_SUPPORTED];

static int g_iError = 0;
static cc6801_I2cMaster g_tI2cBus[I2C_TOTAL_SUPPORTED];

volatile uint32_t I2C0_M_INTR = 0;
volatile uint32_t I2C1_M_INTR = 0;

void I2C0_M_IRQHandler(void)
{
    regI2C0->bf.dma_done_intr = 1;

    //regI2C0->ms_resync_done_intr = 1;
    if (regI2C0->bf.i2c_cd_err_intr)
    {
        g_iError |= I2C_ERR_COLLIDED;
        regI2C0->bf.i2c_cd_err_intr = 1;
    }

    if (regI2C0->bf.i2c_err_ack_intr)
    {
        g_iError |= I2C_ERR_NO_ACK;
        regI2C0->bf.i2c_err_ack_intr = 1;
    }

    if (regI2C0->bf.dma_done_intr)
        regI2C0->bf.dma_done_intr = 1;

    I2C0_M_INTR = 1;
}

void I2C1_M_IRQHandler(void)
{
    regI2C1->bf.dma_done_intr = 1;

    //regI2C1->ms_resync_done_intr = 1;
    if (regI2C1->bf.i2c_cd_err_intr)
    {
        g_iError |= I2C_ERR_COLLIDED;
        regI2C1->bf.i2c_cd_err_intr = 1;
    }

    if (regI2C1->bf.i2c_err_ack_intr)
    {
        g_iError |= I2C_ERR_NO_ACK;
        regI2C1->bf.i2c_err_ack_intr = 1;
    }

    if (regI2C1->bf.dma_done_intr)
        regI2C1->bf.dma_done_intr = 1;

    I2C1_M_INTR = 1;
}

void cc6801_I2cIntDisable(U_regI2C *pI2cBase)
{
    pI2cBase->dw.interrupt |= (I2C_INT_DMA_DONE_DISABLE_MASK |
                               I2C_INT_ERROR_ACK_DISABLE_MASK |
                               I2C_INT_ERROR_CD_DISABLE_MASK |
                               I2C_INT_RESYNC_DISABLE_MASK);
}

void cc6801_I2cIntEnable(U_regI2C *pI2cBase)
{
    pI2cBase->dw.interrupt |= (I2C_INT_DMA_DONE_CLEAR_MASK |
                               I2C_INT_ERROR_ACK_CLEAR_MASK |
                               I2C_INT_ERROR_CD_CLEAR_MASK |
                               I2C_INT_RESYNC_CLEAR_MASK);

    pI2cBase->dw.interrupt |= (I2C_INT_DMA_DONE_ENABLE_MASK |
                               I2C_INT_ERROR_ACK_ENABLE_MASK |
                               I2C_INT_ERROR_CD_ENABLE_MASK |
                               I2C_INT_RESYNC_ENABLE_MASK);
}

static int cc6801_I2c0Xfer(U_regI2C *pI2cBase)
{
    g_iError = I2C_ERR_NONE;

    NVIC_EnableIRQ(I2C0_M_IRQn);

    pI2cBase->bf.dma_enable = I2C_DMA_ENABLE_BIT;

    while(!I2C0_M_INTR);
    I2C0_M_INTR = 0;

    NVIC_DisableIRQ(I2C0_M_IRQn);

    if (g_iError == I2C_ERR_NONE)
        return CC_SUCCESS;

    //Error occurs, reset I2C bus
    if ((g_iError == I2C_ERR_COLLIDED) || (g_iError == I2C_ERR_NO_ACK))
        return CC_ERROR_INTERNAL;

    return CC_ERROR_BUSY;
}

static int cc6801_I2c1Xfer(U_regI2C *pI2cBase)
{
    g_iError = I2C_ERR_NONE;

    NVIC_EnableIRQ(I2C1_M_IRQn);

    pI2cBase->bf.dma_enable = I2C_DMA_ENABLE_BIT;

    while(!I2C1_M_INTR);
    I2C1_M_INTR = 0;

    NVIC_DisableIRQ(I2C1_M_IRQn);

    if (g_iError == I2C_ERR_NONE)
        return CC_SUCCESS;

    //Error occurs, reset I2C bus
    if ((g_iError == I2C_ERR_COLLIDED) || (g_iError == I2C_ERR_NO_ACK))
        return CC_ERROR_INTERNAL;

    return CC_ERROR_BUSY;
}

int cc6801_I2cClk16MFreqSet(T_I2cDevice *tpDev)
{
    uint8_t bBusId = tpDev->bBusNum;
    uint32_t dwFreq = tpDev->dwFreq;
    uint32_t dwPrescaler;
    uint32_t dwCfgMask;
    uint32_t dwI2cConfig;

    dwI2cConfig = g_tI2cBus[bBusId].pReg->dw.cfg;
    dwI2cConfig &= ~I2C_CONFIG_PRESCALER_MASK;
    dwI2cConfig &= ~I2C_CONFIG_CFGMASK_MASK;

    switch (dwFreq)
    {
        case CC6801_I2C_100K:
          dwPrescaler = 150;
          dwCfgMask = 3;
          break;
        case CC6801_I2C_200K:
          dwPrescaler = 72;
          dwCfgMask = 1;
          break;
        case CC6801_I2C_400K:
          dwPrescaler = 30;
          dwCfgMask = 5;
          break;
        default:
          TracerWarn("Clock rate %dKHz not support\n", dwFreq/1000);
          break;
    }

    dwI2cConfig |= ((dwPrescaler << I2C_CONFIG_PRESCALER_SHIFT) |  (dwCfgMask << I2C_CONFIG_CFGMASK_SHIFT));
    g_tI2cBus[bBusId].pReg->dw.cfg = dwI2cConfig;

    TracerInfo("I2C%d clock rate %dHz\n", bBusId, dwFreq);

    return CC_SUCCESS;
}

int cc6801_I2cClk24MFreqSet(T_I2cDevice *tpDev)
{
    uint8_t bBusId = tpDev->bBusNum;
    uint32_t dwFreq = tpDev->dwFreq;
    uint32_t dwPrescaler;
    uint32_t dwCfgMask;
    uint32_t dwI2cConfig;

    dwI2cConfig = g_tI2cBus[bBusId].pReg->dw.cfg;
    dwI2cConfig &= ~I2C_CONFIG_PRESCALER_MASK;
    dwI2cConfig &= ~I2C_CONFIG_CFGMASK_MASK;

    switch (dwFreq)
    {
        case CC6801_I2C_100K:
          dwPrescaler = 230;
          dwCfgMask = 1;
          break;
        case CC6801_I2C_200K:
          dwPrescaler = 112;
          dwCfgMask = 0;
          break;
        case CC6801_I2C_400K:
          dwPrescaler = 52;
          dwCfgMask = 0;
          break;
        default:
          TracerWarn("Clock rate %dKHz not support\n", dwFreq/1000);
          break;
    }

    dwI2cConfig |= ((dwPrescaler << I2C_CONFIG_PRESCALER_SHIFT) |  (dwCfgMask << I2C_CONFIG_CFGMASK_SHIFT));
    g_tI2cBus[bBusId].pReg->dw.cfg = dwI2cConfig;

    TracerInfo("I2C%d clock rate %dHz\n", bBusId, dwFreq);

    return CC_SUCCESS;
}

void cc6801_I2cDeviceRegister(T_I2cDevice *tpDev)
{
    uint8_t bBusId = tpDev->bBusNum;

    g_tI2cBus[bBusId].pReg->bf.cfg_core_select = 1;

    g_tI2cBus[bBusId].pReg->bf.ms_slave_addr = tpDev->bAddr;
    g_tI2cBus[bBusId].pReg->bf.ms_no_stop = 0;
    g_tI2cBus[bBusId].pReg->bf.ms_addr_en = 0;
    g_tI2cBus[bBusId].pReg->bf.ms_addr_16bit = 0;

    cc6801_I2cIntEnable(g_tI2cBus[bBusId].pReg);
}

static int cc6801_I2cClkDivSet(uint8_t bBusId)
{
    uint32_t dwClkCfg = 0;
    uint8_t bClkDiv = 0;

    switch (g_dwCurrentClock)
    {
        case CLOCK_16:
            bClkDiv = 1;
            g_tI2cBus[bBusId].bClkSrc = CLOCK_16;
            break;
        case CLOCK_24:
            bClkDiv = 1;
            g_tI2cBus[bBusId].bClkSrc = CLOCK_24;
            break;
        case CLOCK_32:
            bClkDiv = 2;
            g_tI2cBus[bBusId].bClkSrc = CLOCK_16;
            break;
        case CLOCK_48:
            bClkDiv = 2;
            g_tI2cBus[bBusId].bClkSrc = CLOCK_24;
            break;
        case CLOCK_64:
            bClkDiv = 4;
            g_tI2cBus[bBusId].bClkSrc = CLOCK_16;
            break;
        default:
            TracerWarn("Can't recognize system clock %d\n", g_dwCurrentClock);
            bClkDiv = 1;
            g_tI2cBus[bBusId].bClkSrc = CLOCK_16;
            break;
    }

    dwClkCfg = regCKGEN->dw.clkCfg2;
    dwClkCfg &= ~((0x1F) << 8);
    dwClkCfg |= (bClkDiv << 8);
#ifdef EVB
    regCKGEN->dw.clkCfg2 = dwClkCfg;
#endif

    return 0;
}

int cc6801_I2cInit(T_I2cDevice *tpDev)
{
    uint8_t bBusId = tpDev->bBusNum;

    if (I2C_0 == bBusId)
    {
        g_tI2cBus[bBusId].pReg = regI2C0;
        g_tI2cBus[bBusId].fpI2cXfer = cc6801_I2c0Xfer;
    }
    else if (I2C_1 == bBusId)
    {
        g_tI2cBus[bBusId].pReg = regI2C1;
        g_tI2cBus[bBusId].fpI2cXfer = cc6801_I2c1Xfer;
    }
    else
    {
         TracerWarn("I2C%d not support\n", bBusId);
         return CC_ERROR_INVALID_PARAM;
    }

    cc6801_I2cClkDivSet(bBusId);
    if (CLOCK_16 == g_tI2cBus[bBusId].bClkSrc)
    {
        cc6801_I2cClk16MFreqSet(tpDev);
    }
    else if (CLOCK_24 == g_tI2cBus[bBusId].bClkSrc)
    {
        cc6801_I2cClk24MFreqSet(tpDev);
    }
    else
    {
        TracerWarn("Default clock setting 16M\n");
    }

    return CC_SUCCESS;
}

int cc6801_I2cWrite(uint8_t bBusNum,
                    uint8_t const *pData,
                    uint16_t wLen)
{
    U_regI2C *pI2cBase = g_tI2cBus[bBusNum].pReg;

    pI2cBase->bf.dma_str_raddr = (uint32_t)pData;
    pI2cBase->bf.wdata_byte_num = wLen - 1;
    pI2cBase->bf.rdata_byte_num = wLen - 1;
    pI2cBase->bf.op_mode = 0;

    pI2cBase->bf.dbus_burst = 0;

    return g_tI2cBus[bBusNum].fpI2cXfer(pI2cBase);
}


int cc6801_I2cRead(uint8_t bBusNum,
                   uint8_t *pData,
                   uint16_t wLen)
{
    U_regI2C *pI2cBase = g_tI2cBus[bBusNum].pReg;

    pI2cBase->bf.dma_str_waddr = (uint32_t)pData;
    pI2cBase->bf.wdata_byte_num = wLen - 1;
    pI2cBase->bf.rdata_byte_num = wLen - 1;
    pI2cBase->bf.op_mode = 1;

    pI2cBase->bf.dbus_burst = 0;

    return g_tI2cBus[bBusNum].fpI2cXfer(pI2cBase);
}
