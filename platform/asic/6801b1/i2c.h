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

#ifndef _I2C_H_
#define _I2C_H_

#include "global.h"
#include "cc6801_reg.h"
#include <stddef.h>
#include <stdbool.h>

#define I2C_INT_DMA_DONE_ENABLE_BIT               (1)
#define I2C_INT_DMA_DONE_ENABLE_MASK              (I2C_INT_DMA_DONE_ENABLE_BIT << 0)
#define I2C_INT_DMA_DONE_DISABLE_BIT              (0)
#define I2C_INT_DMA_DONE_DISABLE_MASK             (I2C_INT_DMA_DONE_DISABLE_BIT << 0)

#define I2C_INT_ERROR_ACK_ENABLE_BIT              (1)
#define I2C_INT_ERROR_ACK_ENABLE_MASK             (I2C_INT_ERROR_ACK_ENABLE_BIT << 1)
#define I2C_INT_ERROR_ACK_DISABLE_BIT             (0)
#define I2C_INT_ERROR_ACK_DISABLE_MASK            (I2C_INT_ERROR_ACK_DISABLE_BIT << 1)

#define I2C_INT_ERROR_CD_ENABLE_BIT               (1)
#define I2C_INT_ERROR_CD_ENABLE_MASK              (I2C_INT_ERROR_CD_ENABLE_BIT << 2)
#define I2C_INT_ERROR_CD_DISABLE_BIT              (0)
#define I2C_INT_ERROR_CD_DISABLE_MASK             (I2C_INT_ERROR_CD_DISABLE_BIT << 2)

#define I2C_INT_RESYNC_ENABLE_BIT                 (1)
#define I2C_INT_RESYNC_ENABLE_MASK                (I2C_INT_RESYNC_ENABLE_BIT << 3)
#define I2C_INT_RESYNC_DISABLE_BIT                (0)
#define I2C_INT_RESYNC_DISABLE_MASK               (I2C_INT_RESYNC_DISABLE_BIT << 3)

#define I2C_INT_DMA_DONE_CLEAR_BIT                (1)
#define I2C_INT_DMA_DONE_CLEAR_MASK               (I2C_INT_DMA_DONE_CLEAR_BIT << 16)

#define I2C_INT_ERROR_ACK_CLEAR_BIT               (1)
#define I2C_INT_ERROR_ACK_CLEAR_MASK              (I2C_INT_ERROR_ACK_CLEAR_BIT << 17)

#define I2C_INT_ERROR_CD_CLEAR_BIT                (1)
#define I2C_INT_ERROR_CD_CLEAR_MASK               (I2C_INT_ERROR_CD_CLEAR_BIT << 18)

#define I2C_INT_RESYNC_CLEAR_BIT                  (1)
#define I2C_INT_RESYNC_CLEAR_MASK                 (I2C_INT_RESYNC_CLEAR_BIT << 19)

#define I2C_DMA_WBYTE_SHIFT                       (0)
#define I2C_DMA_WBYTE_BIT                         (0x0FFF)
#define I2C_DMA_WBYTE_MASK                        (I2C_DMA_WBYTE_BIT << I2C_DMA_WBYTE_SHIFT)

#define I2C_DMA_RBYTE_SHIFT                       (16)
#define I2C_DMA_RBYTE_BIT                         (0x0FFF)
#define I2C_DMA_RBYTE_MASK                        (I2C_DMA_RBYTE_BIT << I2C_DMA_RBYTE_SHIFT)

#define I2C_DMA_OPMODE_SHIFT                      (28)
#define I2C_DMA_OPMODE_BIT                        (0x3)
#define I2C_DMA_OPMODE_MASK                       (I2C_DMA_OPMODE_BIT << I2C_DMA_OPMODE_SHIFT)

#define I2C_DMA_OPMODE_WR_BIT                     (0)
#define I2C_DMA_OPMODE_WR_MASK                    (I2C_DMA_OPMODE_WR_BIT << I2C_DMA_OPMODE_SHIFT)

#define I2C_DMA_OPMODE_RD_BIT                     (1)
#define I2C_DMA_OPMODE_RD_MASK                    (I2C_DMA_OPMODE_RD_BIT << I2C_DMA_OPMODE_SHIFT)

#define I2C_DMA_OPMODE_WBR_BIT                    (2)
#define I2C_DMA_OPMODE_WBR_MASK                   (I2C_DMA_OPMODE_WBR_BIT << I2C_DMA_OPMODE_SHIFT)

#define I2C_DMA_DBUS_SINGLE_BIT                   (0)
#define I2C_DMA_DBUS_SINGLE_MASK                  (I2C_DMA_DBUS_SINGLE_BIT << 15)
#define I2C_DMA_DBUS_BURST_BIT                    (1)
#define I2C_DMA_DBUS_BURST_MASK                   (I2C_DMA_DBUS_BURST_BIT << 15)

#define I2C_DMA_ENABLE_BIT                        (1UL)
#define I2C_DMA_ENABLE_MASK                       (I2C_DMA_ENABLE_BIT << 31)

#define I2C_CONFIG_PRESCALER_SHIFT                (16)
#define I2C_CONFIG_PRESCALER_BIT                  (0xFFF)
#define I2C_CONFIG_PRESCALER_MASK                 (I2C_CONFIG_PRESCALER_BIT << I2C_CONFIG_PRESCALER_SHIFT)

#define I2C_CONFIG_CFGMASK_SHIFT                  (2)
#define I2C_CONFIG_CFGMASK_BIT                    (0x3F)
#define I2C_CONFIG_CFGMASK_MASK                   (I2C_CONFIG_CFGMASK_BIT << I2C_CONFIG_CFGMASK_SHIFT)

#define I2C_CONFIG_CORESEL_SHIFT                  (0)
#define I2C_CONFIG_CORESEL_BIT                    (0x03)
#define I2C_CONFIG_CORESEL_MASK                   (I2C_CONFIG_CORESEL_BIT << I2C_CONFIG_CORESEL_SHIFT)

#define I2C_CONFIG_MASTER_BIT                     (0x01)
#define I2C_CONFIG_MASTER_MASK                    (I2C_CONFIG_MASTER_BIT << I2C_CONFIG_CORESEL_SHIFT)

#define I2C_CONFIG_SLAVE_BIT                      (0x02)
#define I2C_CONFIG_SLAVE_MASK                     (I2C_CONFIG_SLAVE_BIT << I2C_CONFIG_CORESEL_SHIFT)

#define I2C_MSCONFIG_WORDADDR_SHIFT               (16)
#define I2C_MSCONFIG_WORDADDR_BIT                 (0xFFFF)
#define I2C_MSCONFIG_WORDADDR_MASK                (I2C_MSCONFIG_WORDADDR_BIT << I2C_MSCONFIG_WORDADDR_SHIFT)

#define I2C_MSCONFIG_SLAVEADDR_SHIFT              (8)
#define I2C_MSCONFIG_SLAVEADDR_BIT                (0x7F)
#define I2C_MSCONFIG_SLAVEADDR_MASK               (I2C_MSCONFIG_SLAVEADDR_BIT << I2C_MSCONFIG_SLAVEADDR_SHIFT)

#define I2C_MSCONFIG_NOSTOP_SHIFT                 (2)
#define I2C_MSCONFIG_NOSTOP_BIT                   (0x01)
#define I2C_MSCONFIG_NOSTOP_MASK                  (I2C_MSCONFIG_NOSTOP_BIT << I2C_MSCONFIG_NOSTOP_SHIFT)

#define I2C_MSCONFIG_ADDREN_SHIFT                 (1)
#define I2C_MSCONFIG_ADDREN_BIT                   (0x01)
#define I2C_MSCONFIG_ADDREN_MASK                  (I2C_MSCONFIG_ADDREN_BIT << I2C_MSCONFIG_ADDREN_SHIFT)

#define I2C_MSCONFIG_ADDR16_SHIFT                 (0)
#define I2C_MSCONFIG_ADDR16_BIT                   (0x01)
#define I2C_MSCONFIG_ADDR16_MASK                  (I2C_MSCONFIG_ADDR16_BIT << I2C_MSCONFIG_ADDR16_SHIFT)

#define I2C_DMA_RWADDR_BIT                        (0x3FFFF)
#define I2C_DMA_RWADDR_MASK                       (I2C_DMA_RWADDR_BIT << 0)

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
    S_regI2C *pReg;
    uint8_t bClkSrc;

    int (*fpI2cXfer)(S_regI2C * p_i2c);
} T_cc6801_I2cMaster;

int cc6801_I2cInit(uint8_t bBusId);

int cc6801_I2cFreqSet(uint8_t bBusNum, uint32_t dwFreq);

int cc6801_I2cWrite(T_I2cDevice *tpDev,
                    uint8_t const *pData,
                    uint16_t wLen);

int cc6801_I2cRead(T_I2cDevice *tpDev,
                   uint8_t *pData,
                   uint16_t wLen);

int cc6801_I2cWriteThenRead(T_I2cDevice *tpDev,
                            uint8_t const * pWrData,
                            uint16_t        wWrLen,
                            uint8_t       * pRdData,
                            uint16_t        wRdLen);

#endif // _I2C_H_
