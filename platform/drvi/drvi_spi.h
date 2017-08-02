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
#include "spim.h"

#define drvi_SpiInit(dev)                                    cc6801_SpimInit(dev)
#define drvi_SpiWrite(dev,buf,len)                           cc6801_SpimWrite(dev,buf,len)
#define drvi_SpiRead(dev,buf,len)                            cc6801_SpimRead(dev,buf,len)
#define drvi_SpiWriteThenRead(dev,tbuf,tlen,rbuf,rlen)       cc6801_SpimWriteThenRead(dev,tbuf,tlen,rbuf,rlen)

#define DRVI_SPI_BUS_NUM           SPIM_TOTAL_SUPPORTED

#define DRVI_SPI_CPHA              0x01            /* clock phase */
#define DRVI_SPI_CPOL              0x02            /* clock polarity */
#define DRVI_SPI_MODE_0            (0|0)           /* (original MicroWire) */
#define DRVI_SPI_MODE_1            (0|DRVI_SPI_CPHA)
#define DRVI_SPI_MODE_2            (DRVI_SPI_CPOL|0)
#define DRVI_SPI_MODE_3            (DRVI_SPI_CPOL|DRVI_SPI_CPHA)
#define DRVI_SPI_CS_HIGH           0x04            /* chipselect active high? */
#define DRVI_SPI_LSB_FIRST         0x08            /* per-word bits-on-wire */
#define DRVI_SPI_3WIRE             0x10            /* SI/SO signals shared */


typedef enum
{
    DRVI_SPI_EVENT_DONE,
    DRVI_SPI_EVENT_ERROR,
} E_DrviSpiEvent;

typedef struct S_SpiDevice {
    UINT8        bBusNum;
    UINT8        chip_select;
    UINT16       wMode;
    UINT32       dwMaxSpeedHz;

    void (*fpComplete)(E_DrviSpiEvent *event);
}T_SpiDevice;








#endif //_DRVI_SPI_H_


