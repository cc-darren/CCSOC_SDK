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

#if defined _I2C_INUSE_ && (_I2C_INUSE_)
int drvi_I2cInit(void)
{
    uint8_t bBusNum = 0;
    uint32_t dwClock = 0;
    int iResult = 0;

    #if defined _I2C0_INUSE_ && (_I2C0_INUSE_)
    bBusNum = 0;
    iResult = cc6801_I2cInit(bBusNum);
    if (!iResult)
        TracerErr("I2C0 initial error\n");

    dwClock = I2C0_CLOCK;
    cc6801_I2cFreqSet(bBusNum, dwClock);
    #endif
    #if defined _I2C1_INUSE_ && (_I2C1_INUSE_)
    bBusNum = 1;
    iResult = cc6801_I2cInit(bBusNum);
    if (!iResult)
        TracerErr("I2C1 initial error\n");

    dwClock = I2C1_CLOCK;
    cc6801_I2cFreqSet(bBusNum, dwClock);
    #endif

    return iResult;
}
#endif
