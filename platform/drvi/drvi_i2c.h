/* Copyright (c) 2018 Cloudchip, Inc. All Rights Reserved.
 *
 * The information contained herein is property of Cloudchip, Inc.
 * Terms and conditions of usage are described in detail in CLOUDCHIP
 * STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information.
 * NO WARRANTY of ANY KIND is provided. This heading must NOT be removed 
 * from the file.
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
} T_I2cDevice;

typedef void (*fpComplete)(T_I2cEvent *pEvent);

int drvi_I2cInit(void);

__forceinline int drvi_I2cWrite(T_I2cDevice *tpDev, uint8_t const *pData, uint16_t wLen)
{
    return cc6801_I2cWrite(tpDev, pData, wLen);
}

__forceinline int drvi_I2cRead(T_I2cDevice *tpDev, uint8_t *pData, uint16_t wLen)
{
    return cc6801_I2cRead(tpDev, pData, wLen);
}

#if (defined(CC6801B0) || defined(CC6801C0))
__forceinline int drvi_I2cWriteThenRead(T_I2cDevice *tpDev,
                          uint8_t const *pTxData, uint16_t wTxLen,
                          uint8_t *pRxData, uint16_t wRxLen)
{
    return cc6801_I2cWriteThenRead(tpDev, pTxData, wTxLen, pRxData, wRxLen);
}
#else
int drvi_I2cWriteThenRead(T_I2cDevice *tpDev,
                          uint8_t const *pTxData, uint16_t wTxLen,
                          uint8_t *pRxData, uint16_t wRxLen);
#endif

#endif //_DRVI_I2C_H_
