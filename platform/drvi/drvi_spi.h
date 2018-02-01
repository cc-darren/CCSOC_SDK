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
 
#ifndef _DRVI_SPI_H_
#define _DRVI_SPI_H_
#include "global.h"
#include "project.h"
#include "spim.h"

typedef enum
{
    DRVI_SPI_EVENT_DONE,
    DRVI_SPI_EVENT_ERROR,
} E_DrviSpiEvent;

typedef struct S_SpiDevice
{
    uint8_t        bBusNum;
    uint8_t        chip_select;
    uint16_t       wMode;
    uint32_t       dwMaxSpeedHz;

    void (*fpComplete)(E_DrviSpiEvent *event);
} T_SpiDevice;


int drvi_SpiInit(void);

__forceinline int drvi_SpiWrite(uint8_t         bBusNum,
                                uint8_t const * pTxBuf,
                                uint8_t         bTxBufLen)
{
    return cc6801_SpimWrite(bBusNum, pTxBuf, bTxBufLen);
}

__forceinline int drvi_SpiRead(uint8_t   bBusNum,
                               uint8_t * pRxBuf,
                               uint8_t   bRxBufLen)
{
    return cc6801_SpimRead(bBusNum, pRxBuf, bRxBufLen);
}

__forceinline int drvi_SpiWriteThenRead(uint8_t         bBusNum,
                                        uint8_t const * pTxBuf,
                                        uint8_t         bTxBufLen,
                                        uint8_t       * pRxBuf,
                                        uint8_t         bRxBufLen)
{
    return cc6801_SpimWriteThenRead(bBusNum, pTxBuf, bTxBufLen, pRxBuf, bRxBufLen);
}

#endif //_DRVI_SPI_H_


