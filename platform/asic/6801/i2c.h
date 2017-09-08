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

#ifndef _I2C_H_
#define _I2C_H_

#include "global.h"
#include "cc6801_reg.h"
#include <stddef.h>
#include <stdbool.h>

#define I2C_INT_DMA_DONE_ENABLE_BIT (1)
#define I2C_INT_DMA_DONE_ENABLE_MASK (I2C_INT_DMA_DONE_ENABLE_BIT << 0)
#define I2C_INT_DMA_DONE_DISABLE_BIT (0)
#define I2C_INT_DMA_DONE_DISABLE_MASK (I2C_INT_DMA_DONE_DISABLE_BIT << 0)

#define I2C_INT_ERROR_ACK_ENABLE_BIT (1)
#define I2C_INT_ERROR_ACK_ENABLE_MASK (I2C_INT_ERROR_ACK_ENABLE_BIT << 1)
#define I2C_INT_ERROR_ACK_DISABLE_BIT (0)
#define I2C_INT_ERROR_ACK_DISABLE_MASK (I2C_INT_ERROR_ACK_DISABLE_BIT << 1)

#define I2C_INT_ERROR_CD_ENABLE_BIT (1)
#define I2C_INT_ERROR_CD_ENABLE_MASK (I2C_INT_ERROR_CD_ENABLE_BIT << 2)
#define I2C_INT_ERROR_CD_DISABLE_BIT (0)
#define I2C_INT_ERROR_CD_DISABLE_MASK (I2C_INT_ERROR_CD_DISABLE_BIT << 2)

#define I2C_INT_RESYNC_ENABLE_BIT (1)
#define I2C_INT_RESYNC_ENABLE_MASK (I2C_INT_RESYNC_ENABLE_BIT << 3)
#define I2C_INT_RESYNC_DISABLE_BIT (0)
#define I2C_INT_RESYNC_DISABLE_MASK (I2C_INT_RESYNC_DISABLE_BIT << 3)

#define I2C_INT_DMA_DONE_CLEAR_BIT (1)
#define I2C_INT_DMA_DONE_CLEAR_MASK (I2C_INT_DMA_DONE_CLEAR_BIT << 16)

#define I2C_INT_ERROR_ACK_CLEAR_BIT (1)
#define I2C_INT_ERROR_ACK_CLEAR_MASK (I2C_INT_ERROR_ACK_CLEAR_BIT << 17)

#define I2C_INT_ERROR_CD_CLEAR_BIT (1)
#define I2C_INT_ERROR_CD_CLEAR_MASK (I2C_INT_ERROR_CD_CLEAR_BIT << 18)

#define I2C_INT_RESYNC_CLEAR_BIT (1)
#define I2C_INT_RESYNC_CLEAR_MASK (I2C_INT_RESYNC_CLEAR_BIT << 19)

#define I2C_DMA_DBUS_SINGLE_BIT (0)
#define I2C_DMA_DBUS_SINGLE_MASK (I2C_DMA_DBUS_SINGLE_BIT << 15)
#define I2C_DMA_DBUS_BURST_BIT (1)
#define I2C_DMA_DBUS_BURST_MASK (I2C_DMA_DBUS_BURST_BIT << 15)

#define I2C_DMA_ENABLE_BIT (1)
#define I2C_DMA_ENABLE_MASK (I2C_DMA_ENABLE_BIT << 31)


#define I2C_ERR_NONE        0x00
#define I2C_ERR_NO_ACK      0x01
#define I2C_ERR_COLLIDED    0x02

typedef struct S_I2cDevice T_I2cDevice;

typedef enum
{
    I2C_0 = 0,
    I2C_1,
    I2C_TOTAL_SUPPORTED
}E_I2cSupported;

typedef struct
{
    U_regI2C *pReg;
    int (*fpI2cXfer)(U_regI2C * p_i2c);
} cc6801_I2cMaster;

int cc6801_I2cInit(uint8_t bBusNum);

int cc6801_I2cFreqSet(uint8_t bBusNum, uint32_t dwFreq);

void cc6801_I2cDeviceRegister(T_I2cDevice *tpDev);

int cc6801_I2cWrite(uint8_t bBusNum,
                    uint8_t const *pData,
                    uint16_t wLen);

int cc6801_I2cRead(uint8_t bBusNum,
                   uint8_t *pData,
                   uint16_t wLen);

#endif // _I2C_H_
