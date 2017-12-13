/**
 ****************************************************************************************
 *
 * @file drvi_i2c.h
 *
 * @brief Head file of drvi_i2c.c
 *
 * Copyright (C) CloudChip 2017-2019
 *
 *
 ****************************************************************************************
 */
#ifndef _DRVI_I2C_H_
#define _DRVI_I2C_H_
#include "project.h"
#include "i2c.h"

typedef enum
{
    DRVI_I2C_READ_DONE,
    DRVI_I2C_WRITE_DONE,
    DRVI_I2C_TRANSFER_ERROR
} E_DrviI2cEventType;

typedef struct
{
    E_DrviI2cEventType eType;
    uint8_t *pData;
    uint32_t  dwLen;
} T_I2cEvent;

typedef struct S_I2cDevice
{
    uint8_t bBusNum;
    uint8_t bAddr;
    uint32_t dwFreq;

    void (*fpComplete)(T_I2cEvent *pEvent);
} T_I2cDevice;

int drvi_I2cInit(void);

__forceinline void drvi_I2cDeviceRegister(T_I2cDevice *tpDev)
{
    cc6801_I2cDeviceRegister(tpDev);
}

__forceinline int drvi_I2cWrite(uint8_t bBusNum, uint8_t const *pData, uint16_t wLen)
{
    return cc6801_I2cWrite(bBusNum, pData, wLen);
}

__forceinline int drvi_I2cRead(uint8_t bBusNum, uint8_t *pData, uint16_t wLen)
{
    return cc6801_I2cRead(bBusNum, pData, wLen);
}

int drvi_I2cWriteThenRead(uint8_t bBusNum,
                          uint8_t const *pTxData, uint16_t wTxLen,
                          uint8_t *pRxData, uint16_t wRxLen);
#endif //_DRVI_I2C_H_


