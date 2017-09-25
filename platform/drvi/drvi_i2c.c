/**
 ****************************************************************************************
 *
 * @file drvi_i2c.c
 *
 * @brief HW I2C interface for all SOC.
 *
 * Copyright (C) CloudChip 2017-2019
 *
 *
 ****************************************************************************************
 */
#include "drvi_i2c.h"
#include "tracer.h"

#if defined I2C_INUSE && (I2C_INUSE)
int drvi_I2cInit(void)
{
    uint8_t bBusNum = 0;
    uint32_t dwClock = 0;
    int iResult = 0;

    #if defined I2C0_INUSE && (I2C0_INUSE)
    bBusNum = 0;
    iResult = cc6801_I2cInit(bBusNum);
    if (!iResult)
        TracerErr("I2C0 initial error\n");

    dwClock = I2C0_CLOCK;
    cc6801_I2cFreqSet(bBusNum, dwClock);
    #endif
    #if defined I2C1_INUSE && (I2C1_INUSE)
    bBusNum = 1;
    iResult = cc6801_I2cInit(bBusNum);
    if (!iResult)
        TracerErr("I2C1 initial error\n");

    dwClock = I2C1_CLOCK;
    cc6801_I2cFreqSet(bBusNum, dwClock);
    #endif

    return iResult;
}

int drvi_I2cWriteThenRead(uint8_t bBusNum,
                          uint8_t const *pTxData, uint16_t wTxLen,
                          uint8_t *pRxData, uint16_t wRxLen)
{
    int iResult = 0;

    do
    {
        iResult = cc6801_I2cWrite(bBusNum, pTxData, wTxLen);
        if (CC_SUCCESS != iResult)
            break;

        iResult = cc6801_I2cRead(bBusNum, pRxData, wRxLen);
    } while(0);

    return iResult;
}
#endif
