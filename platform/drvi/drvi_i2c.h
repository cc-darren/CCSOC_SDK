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

#define drvi_I2cDeviceRegister(dev)                          cc6801_I2cDeviceRegister(dev)
#define drvi_I2cWrite(bus,buf,len)                           cc6801_I2cWrite(bus,buf,len)
#define drvi_I2cRead(bus,buf,len)                            cc6801_I2cRead(bus,buf,len)

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

    void (*fpComplete)(T_I2cEvent *pEvent);
} T_I2cDevice;

int drvi_I2cInit(void);

#endif //_DRVI_I2C_H_


