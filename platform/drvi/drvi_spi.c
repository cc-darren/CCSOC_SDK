/**
 ****************************************************************************************
 *
 * @file drvi_spi.c
 *
 * @brief HW SPI interface for all SOC.
 *
 * Copyright (C) CloudChip 2017-2019
 *
 *
 ****************************************************************************************
 */
#include "global.h"
#include "drvi_spi.h"

T_SpiDevice g_tSpiDev[3] = {0};

int drvi_SpiInit(uint8_t bBusNum)
{
    g_tSpiDev[bBusNum].bBusNum = bBusNum;

    if (SPIM0 == bBusNum)
        g_tSpiDev[bBusNum].wMode = SPIM0_CONFIG_MODE;
    else if (SPIM1 == bBusNum)
        g_tSpiDev[bBusNum].wMode = SPIM1_CONFIG_MODE;
    else if (SPIM2 == bBusNum)
        g_tSpiDev[bBusNum].wMode = SPIM2_CONFIG_MODE;
    else
        TracerWarn("Not Support SPI%d\r\n", bBusNum);

    return cc6801_SpimInit(&g_tSpiDev[bBusNum]);
}

int drvi_SpiWrite(uint8_t bBusNum,
                  uint8_t const * pTxBuf,
                  uint8_t         bTxBufLen)
{
    return cc6801_SpimWrite(&g_tSpiDev[bBusNum], pTxBuf, bTxBufLen);
}

int drvi_SpiRead(uint8_t bBusNum,
                 uint8_t       * pRxBuf,
                 uint8_t         bRxBufLen)
{
    return cc6801_SpimRead(&g_tSpiDev[bBusNum], pRxBuf, bRxBufLen);
}

int drvi_SpiWriteThenRead(uint8_t bBusNum,
                          uint8_t const * pTxBuf,
                          uint8_t         bTxBufLen,
                          uint8_t       * pRxBuf,
                          uint8_t         bRxBufLen)
{
    return cc6801_SpimWriteThenRead(&g_tSpiDev[bBusNum], pTxBuf, bTxBufLen, pRxBuf, bRxBufLen);
}
