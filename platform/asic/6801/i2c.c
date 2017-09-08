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
#include "cc6801_reg.h"
#include "drvi_i2c.h"
#include "i2c.h"
#include "tracer.h"

typedef enum
{
    CC6801_I2C_100K = _I2C_FREQ_100K_,
    CC6801_I2C_200K = _I2C_FREQ_200K_,
    CC6801_I2C_400K = _I2C_FREQ_400K_,
} E_I2cFreq;

static int g_iError = 0;
static cc6801_I2cMaster tI2cBus[I2C_TOTAL_SUPPORTED];

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

int cc6801_I2cFreqSet(uint8_t bBusNum, uint32_t dwFreq)
{
    if (!tI2cBus[bBusNum].pReg)
    {
        TracerWarn("I2C%d not initialized\n", bBusNum);
        return CC_ERROR_INVALID_PARAM;
    }

    switch (dwFreq)
    {
        case CC6801_I2C_100K:
          tI2cBus[bBusNum].pReg->bf.ms_prescaler = 154;
          tI2cBus[bBusNum].pReg->bf.cfg_i2c_mask = 3;
          break;
        case CC6801_I2C_400K:
          tI2cBus[bBusNum].pReg->bf.ms_prescaler = 30;
          tI2cBus[bBusNum].pReg->bf.cfg_i2c_mask = 3;
          break;
        case CC6801_I2C_200K:
        default:
          TracerWarn("Clock rate %dKHz not support\n", dwFreq/1000);
          break;
    }

    return CC_SUCCESS;
}

void cc6801_I2cDeviceRegister(T_I2cDevice *tpDev)
{
    uint8_t bIdx = tpDev->bBusNum;

    tI2cBus[bIdx].pReg->bf.cfg_core_select = 1;

    tI2cBus[bIdx].pReg->bf.ms_slave_addr = tpDev->bAddr;
    tI2cBus[bIdx].pReg->bf.ms_word_addr = 0;
    tI2cBus[bIdx].pReg->bf.ms_no_stop = 0;
    tI2cBus[bIdx].pReg->bf.ms_addr_en = 0;
    tI2cBus[bIdx].pReg->bf.ms_addr_16bit = 0;

    cc6801_I2cIntEnable(tI2cBus[bIdx].pReg);
}

int cc6801_I2cInit(uint8_t bBusNum)
{
    if (bBusNum == I2C_0)
    {
        tI2cBus[bBusNum].pReg = regI2C0;
        tI2cBus[bBusNum].fpI2cXfer = cc6801_I2c0Xfer;
    }
    else if (bBusNum == I2C_1)
    {
        tI2cBus[bBusNum].pReg = regI2C1;
        tI2cBus[bBusNum].fpI2cXfer = cc6801_I2c1Xfer;
    }
    else
    {
         TracerWarn("I2C%d not support\n", bBusNum);
         return CC_ERROR_INVALID_PARAM;
    }

    return CC_SUCCESS;
}

int cc6801_I2cWrite(uint8_t bBusNum,
                    uint8_t const *pData,
                    uint16_t wLen)
{
    U_regI2C *pI2cBase = tI2cBus[bBusNum].pReg;

    pI2cBase->bf.dma_str_raddr = (uint32_t)pData;
    pI2cBase->bf.wdata_byte_num = wLen - 1;
    pI2cBase->bf.rdata_byte_num = wLen - 1;
    pI2cBase->bf.op_mode = 0;

    pI2cBase->bf.dbus_burst = 0;

    return tI2cBus[bBusNum].fpI2cXfer(pI2cBase);
}


int cc6801_I2cRead(uint8_t bBusNum,
                   uint8_t *pData,
                   uint16_t wLen)
{
    U_regI2C *pI2cBase = tI2cBus[bBusNum].pReg;

    pI2cBase->bf.dma_str_raddr = (uint32_t)pData;
    pI2cBase->bf.wdata_byte_num = wLen - 1;
    pI2cBase->bf.rdata_byte_num = wLen - 1;
    pI2cBase->bf.op_mode = 1;

    pI2cBase->bf.dbus_burst = 0;

    return tI2cBus[bBusNum].fpI2cXfer(pI2cBase);
}
