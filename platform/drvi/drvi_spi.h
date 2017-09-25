/**
 ****************************************************************************************
 *
 * @file drvi_spi.h
 *
 * @brief Head file of drvi_spi.c
 *
 * Copyright (C) CloudChip 2017-2019
 *
 *
 ****************************************************************************************
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


