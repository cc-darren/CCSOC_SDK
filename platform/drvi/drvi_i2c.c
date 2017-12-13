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
    T_I2cDevice tI2cDev;
    int iResult = 0;

    #if defined I2C0_INUSE && (I2C0_INUSE)
    tI2cDev.bBusNum = 0;
    tI2cDev.dwFreq = I2C0_CLOCK;
    iResult = cc6801_I2cInit(&tI2cDev);
    if (iResult)
        TracerErr("I2C0 initial error\n");
    #endif
    #if defined I2C1_INUSE && (I2C1_INUSE)
    tI2cDev.bBusNum = 1;
    tI2cDev.dwFreq = I2C1_CLOCK;
    iResult = cc6801_I2cInit(&tI2cDev);
    if (iResult)
        TracerErr("I2C1 initial error\n");
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
