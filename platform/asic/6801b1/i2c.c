/* Copyright (c) 2019 Cloudchip, Inc. All Rights Reserved.
 *
 * The information contained herein is property of Cloudchip, Inc.
 * Terms and conditions of usage are described in detail in CLOUDCHIP
 * STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information.
 * NO WARRANTY of ANY KIND is provided. This heading must NOT be removed
 * from the file.
 */

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

volatile uint32_t I2C0_M_INTR = 0;
volatile uint32_t I2C1_M_INTR = 0;

static int cc6801_I2c0Xfer(S_regI2C *pI2cBase);
static int cc6801_I2c1Xfer(S_regI2C *pI2cBase);

static T_cc6801_I2cMaster g_tI2cBus[I2C_TOTAL_SUPPORTED] =
{
    {regI2C0, 0, cc6801_I2c0Xfer},
    {regI2C1, 0, cc6801_I2c1Xfer},
};

void I2C0_M_IRQHandler(void)
{
    uint32_t dwInt = g_tI2cBus[I2C_0].pReg->interrupt;

    if (dwInt & I2C_INT_ERROR_CD_CLEAR_MASK)
        g_iError |= I2C_ERR_COLLIDED;

    if (dwInt & I2C_INT_ERROR_ACK_CLEAR_MASK)
        g_iError |= I2C_ERR_NO_ACK;

    dwInt |= (I2C_INT_ERROR_CD_CLEAR_MASK |
              I2C_INT_ERROR_ACK_CLEAR_MASK |
              I2C_INT_DMA_DONE_CLEAR_MASK);

    g_tI2cBus[I2C_0].pReg->interrupt |= dwInt;

    I2C0_M_INTR = 1;
}

void I2C1_M_IRQHandler(void)
{
    uint32_t dwInt = g_tI2cBus[I2C_1].pReg->interrupt;

    if (dwInt & I2C_INT_ERROR_CD_CLEAR_MASK)
        g_iError |= I2C_ERR_COLLIDED;

    if (dwInt & I2C_INT_ERROR_ACK_CLEAR_MASK)
        g_iError |= I2C_ERR_NO_ACK;

    dwInt |= (I2C_INT_ERROR_CD_CLEAR_MASK |
              I2C_INT_ERROR_ACK_CLEAR_MASK |
              I2C_INT_DMA_DONE_CLEAR_MASK);

    g_tI2cBus[I2C_1].pReg->interrupt |= dwInt;

    I2C1_M_INTR = 1;
}

void cc6801_I2cIntDisable(S_regI2C *pI2cBase)
{
    pI2cBase->interrupt |= (I2C_INT_DMA_DONE_DISABLE_MASK |
                               I2C_INT_ERROR_ACK_DISABLE_MASK |
                               I2C_INT_ERROR_CD_DISABLE_MASK |
                               I2C_INT_RESYNC_DISABLE_MASK);
}

void cc6801_I2cIntEnable(S_regI2C *pI2cBase)
{
    pI2cBase->interrupt |= (I2C_INT_DMA_DONE_CLEAR_MASK |
                               I2C_INT_ERROR_ACK_CLEAR_MASK |
                               I2C_INT_ERROR_CD_CLEAR_MASK |
                               I2C_INT_RESYNC_CLEAR_MASK);

    pI2cBase->interrupt |= (I2C_INT_DMA_DONE_ENABLE_MASK |
                               I2C_INT_ERROR_ACK_ENABLE_MASK |
                               I2C_INT_ERROR_CD_ENABLE_MASK |
                               I2C_INT_RESYNC_ENABLE_MASK);
}

static int cc6801_I2c0Xfer(S_regI2C *pI2cBase)
{
    g_iError = I2C_ERR_NONE;

    NVIC_EnableIRQ(I2C0_M_IRQn);

    pI2cBase->dmaCtrl |= I2C_DMA_ENABLE_MASK;

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

static int cc6801_I2c1Xfer(S_regI2C *pI2cBase)
{
    g_iError = I2C_ERR_NONE;

    NVIC_EnableIRQ(I2C1_M_IRQn);

    pI2cBase->dmaCtrl |= I2C_DMA_ENABLE_MASK;

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

int cc6801_I2cClk16MFreqSet(uint8_t bBusId, uint32_t dwFreq)
{
    uint32_t dwPrescaler;
    uint32_t dwCfgMask;
    uint32_t dwI2cConfig;

    dwI2cConfig = g_tI2cBus[bBusId].pReg->coreCfg;
    dwI2cConfig &= ~I2C_CONFIG_PRESCALER_MASK;
    dwI2cConfig &= ~I2C_CONFIG_CFGMASK_MASK;

    switch (dwFreq)
    {
        case CC6801_I2C_100K:
          dwPrescaler = 70;
          dwCfgMask = 0;
          break;
        case CC6801_I2C_200K:
          dwPrescaler = 30;
          dwCfgMask = 0;
          break;
        case CC6801_I2C_400K:
          dwPrescaler = 10;
          dwCfgMask = 0;
          break;
        default:
          //TracerWarn("Clock rate %dKHz not support\r\n", dwFreq/1000);
          return CC_ERROR_INVALID_PARAM;
    }

    dwI2cConfig |= ((dwPrescaler << I2C_CONFIG_PRESCALER_SHIFT) |  (dwCfgMask << I2C_CONFIG_CFGMASK_SHIFT));
    g_tI2cBus[bBusId].pReg->coreCfg = dwI2cConfig;

    //Todo: When RTOS is enabled, interrupt must be disabled during initialization.
    //      So, tracer function can't be enabled.
    //      We need to implement UART direct output function
    //TracerInfo("I2C%d clock rate %dHz\r\n", bBusId, dwFreq);

    return CC_SUCCESS;
}

int cc6801_I2cClk24MFreqSet(uint8_t bBusId, uint32_t dwFreq)
{
    uint32_t dwPrescaler;
    uint32_t dwCfgMask;
    uint32_t dwI2cConfig;

    dwI2cConfig = g_tI2cBus[bBusId].pReg->coreCfg;
    dwI2cConfig &= ~I2C_CONFIG_PRESCALER_MASK;
    dwI2cConfig &= ~I2C_CONFIG_CFGMASK_MASK;

    switch (dwFreq)
    {
        case CC6801_I2C_100K:
          dwPrescaler = 108;
          dwCfgMask = 0;
          break;
        case CC6801_I2C_200K:
          dwPrescaler = 48;
          dwCfgMask = 0;
          break;
        case CC6801_I2C_400K:
          dwPrescaler = 18;
          dwCfgMask = 0;
          break;
        default:
          //TracerWarn("Clock rate %dKHz not support\r\n", dwFreq/1000);
          break;
    }

    dwI2cConfig |= ((dwPrescaler << I2C_CONFIG_PRESCALER_SHIFT) |  (dwCfgMask << I2C_CONFIG_CFGMASK_SHIFT));
    g_tI2cBus[bBusId].pReg->coreCfg = dwI2cConfig;

    //Todo: When RTOS is enabled, interrupt must be disabled during initialization.
    //      So, tracer function can't be enabled.
    //      We need to implement UART direct output function
    //TracerInfo("I2C%d clock rate %dHz\r\n", bBusId, dwFreq);

    return CC_SUCCESS;
}

static void cc6801_I2cClkDivSet(uint8_t bBusId)
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
        case CLOCK_96:
            bClkDiv = 4;
            g_tI2cBus[bBusId].bClkSrc = CLOCK_24;
            break;
        default:
            TracerWarn("Can't recognize system clock %d\r\n", g_dwCurrentClock);
            bClkDiv = 1;
            g_tI2cBus[bBusId].bClkSrc = CLOCK_16;
            break;
    }

    dwClkCfg = regCKGEN->dw.clkCfg2;
    dwClkCfg &= ~((0x1F) << 8);
    dwClkCfg |= (bClkDiv << 8);

    regCKGEN->dw.clkCfg2 = dwClkCfg;
}

int cc6801_I2cInit(uint8_t bBusId)
{
    uint32_t dwCoreCfg = 0;
    uint32_t dwMsCfg = 0;

    if (bBusId >= I2C_TOTAL_SUPPORTED)
    {
         TracerWarn("I2C%d not support\r\n", bBusId);
         return CC_ERROR_INVALID_PARAM;
    }

    g_tI2cBus[bBusId].pReg->interrupt = 0;
    g_tI2cBus[bBusId].pReg->dmaCtrl = 0;
    g_tI2cBus[bBusId].pReg->wAddr = 0;
    g_tI2cBus[bBusId].pReg->rAddr = 0;
    g_tI2cBus[bBusId].pReg->coreCfg = 0;
    g_tI2cBus[bBusId].pReg->masterCfg = 0;

    dwCoreCfg = g_tI2cBus[bBusId].pReg->coreCfg;

    dwCoreCfg &= ~I2C_CONFIG_CORESEL_MASK;
    dwCoreCfg |= I2C_CONFIG_MASTER_MASK;

    dwMsCfg = g_tI2cBus[bBusId].pReg->masterCfg;
    dwMsCfg &= ~(I2C_MSCONFIG_NOSTOP_MASK |
                 I2C_MSCONFIG_ADDREN_MASK |
                 I2C_MSCONFIG_ADDR16_MASK);

    g_tI2cBus[bBusId].pReg->masterCfg = dwMsCfg;
    g_tI2cBus[bBusId].pReg->coreCfg = dwCoreCfg;

    cc6801_I2cIntEnable(g_tI2cBus[bBusId].pReg);

    return CC_SUCCESS;
}

int cc6801_I2cFreqSet(uint8_t bBusId, uint32_t dwFreq)
{
    int iRet = 0;

    cc6801_I2cClkDivSet(bBusId);

    if (CLOCK_16 == g_tI2cBus[bBusId].bClkSrc)
    {
        iRet = cc6801_I2cClk16MFreqSet(bBusId, dwFreq);
    }
    else if (CLOCK_24 == g_tI2cBus[bBusId].bClkSrc)
    {
        iRet = cc6801_I2cClk24MFreqSet(bBusId, dwFreq);
    }
    else
    {

    //Todo: When RTOS is enabled, interrupt must be disabled during initialization.
    //      So, tracer function can't be enabled.
    //      We need to implement UART direct output function
        //TracerWarn("Default clock setting 16M\r\n");
        iRet = cc6801_I2cClk16MFreqSet(bBusId, dwFreq);
    }

    return iRet;
}

int cc6801_I2cWrite(T_I2cDevice *tpDev,
                    uint8_t const *pData,
                    uint16_t wLen)
{
    uint32_t dwDmaCtrl = 0;
    uint32_t dwMsCfg = 0;
    S_regI2C *pI2cBase = g_tI2cBus[tpDev->bBusNum].pReg;
    uint8_t bSlaveAddr = 0;

    bSlaveAddr = ((pI2cBase->masterCfg & I2C_MSCONFIG_SLAVEADDR_MASK) >> I2C_MSCONFIG_SLAVEADDR_SHIFT);
    if (bSlaveAddr != tpDev->bAddr)
    {
        dwMsCfg &= ~I2C_MSCONFIG_SLAVEADDR_MASK;
        dwMsCfg |= (tpDev->bAddr << I2C_MSCONFIG_SLAVEADDR_SHIFT);
        pI2cBase->masterCfg = dwMsCfg;
    }

    dwDmaCtrl = pI2cBase->dmaCtrl;
    dwDmaCtrl &= ~(I2C_DMA_OPMODE_MASK | I2C_DMA_RBYTE_MASK | I2C_DMA_WBYTE_MASK);
    dwDmaCtrl |= (I2C_DMA_OPMODE_WR_MASK |
                 (((wLen-1)&I2C_DMA_WBYTE_BIT << I2C_DMA_WBYTE_SHIFT)));

    pI2cBase->rAddr = ((uint32_t)pData & I2C_DMA_RWADDR_MASK);
    pI2cBase->dmaCtrl = dwDmaCtrl;

    return g_tI2cBus[tpDev->bBusNum].fpI2cXfer(pI2cBase);
}


int cc6801_I2cRead(T_I2cDevice *tpDev,
                   uint8_t *pData,
                   uint16_t wLen)
{
    uint32_t dwDmaCtrl = 0;
    uint32_t dwMsCfg = 0;
    S_regI2C *pI2cBase = g_tI2cBus[tpDev->bBusNum].pReg;
    uint8_t bSlaveAddr = 0;

    bSlaveAddr = ((pI2cBase->masterCfg & I2C_MSCONFIG_SLAVEADDR_MASK) >> I2C_MSCONFIG_SLAVEADDR_SHIFT);
    if (bSlaveAddr != tpDev->bAddr)
    {
        dwMsCfg &= ~I2C_MSCONFIG_SLAVEADDR_MASK;
        dwMsCfg |= (tpDev->bAddr << I2C_MSCONFIG_SLAVEADDR_SHIFT);
        pI2cBase->masterCfg = dwMsCfg;
    }

    dwDmaCtrl = pI2cBase->dmaCtrl;
    dwDmaCtrl &= ~(I2C_DMA_OPMODE_MASK | I2C_DMA_RBYTE_MASK | I2C_DMA_WBYTE_MASK);
    dwDmaCtrl |= (I2C_DMA_OPMODE_RD_MASK |
                 (((wLen-1)&I2C_DMA_RBYTE_BIT) << I2C_DMA_RBYTE_SHIFT));

    pI2cBase->wAddr = ((uint32_t)pData & I2C_DMA_RWADDR_MASK);
    pI2cBase->dmaCtrl = dwDmaCtrl;

    return g_tI2cBus[tpDev->bBusNum].fpI2cXfer(pI2cBase);
}

int cc6801_I2cWriteThenRead(T_I2cDevice *tpDev,
                            uint8_t const * pWrData,
                            uint16_t        wWrLen,
                            uint8_t       * pRdData,
                            uint16_t        wRdLen)
{
    uint32_t dwDmaCtrl = 0;
    uint32_t dwMsCfg = 0;
    S_regI2C *pI2cBase = g_tI2cBus[tpDev->bBusNum].pReg;
    uint8_t bSlaveAddr = 0;

    bSlaveAddr = ((pI2cBase->masterCfg & I2C_MSCONFIG_SLAVEADDR_MASK) >> I2C_MSCONFIG_SLAVEADDR_SHIFT);
    if (bSlaveAddr != tpDev->bAddr)
    {
        dwMsCfg &= ~I2C_MSCONFIG_SLAVEADDR_MASK;
        dwMsCfg |= (tpDev->bAddr << I2C_MSCONFIG_SLAVEADDR_SHIFT);
        pI2cBase->masterCfg = dwMsCfg;
    }

    dwDmaCtrl &= ~(I2C_DMA_OPMODE_MASK | I2C_DMA_RBYTE_MASK | I2C_DMA_WBYTE_MASK);
    dwDmaCtrl |= (I2C_DMA_OPMODE_WBR_MASK |
                 (((wRdLen-1)&I2C_DMA_RBYTE_BIT) << I2C_DMA_RBYTE_SHIFT) |
                 (((wWrLen-1)&I2C_DMA_WBYTE_BIT << I2C_DMA_WBYTE_SHIFT)));

    pI2cBase->wAddr = ((uint32_t)pRdData & I2C_DMA_RWADDR_MASK);
    pI2cBase->rAddr = ((uint32_t)pWrData & I2C_DMA_RWADDR_MASK);

    pI2cBase->dmaCtrl = dwDmaCtrl;

    return g_tI2cBus[tpDev->bBusNum].fpI2cXfer(pI2cBase);
 }

