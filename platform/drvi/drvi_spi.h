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

//#define drvi_SpiInit(dev)                                    cc6801_SpimInit(dev)
#define drvi_SpiWrite(bus,buf,len)                           cc6801_SpimWrite(bus,buf,len)
#define drvi_SpiRead(bus,buf,len)                            cc6801_SpimRead(bus,buf,len)
#define drvi_SpiWriteThenRead(bus,tbuf,tlen,rbuf,rlen)       cc6801_SpimWriteThenRead(bus,tbuf,tlen,rbuf,rlen)

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


#endif //_DRVI_SPI_H_


