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
#include "tracer.h"

/*
#define drvi_SpiInit(dev)                                    cc6801_SpimInit(dev)
#define drvi_SpiWrite(dev,buf,len)                           cc6801_SpimWrite(dev,buf,len)
#define drvi_SpiRead(dev,buf,len)                            cc6801_SpimRead(dev,buf,len)
#define drvi_SpiWriteThenRead(dev,tbuf,tlen,rbuf,rlen)       cc6801_SpimWriteThenRead(dev,tbuf,tlen,rbuf,rlen)
*/

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

/* ============== Move to project.h ============== */
#define SPIM0_SUPPORT 1
#define SPIM1_SUPPORT 1
#define SPIM2_SUPPORT 1

#if (SPIM0_SUPPORT)
#define SPIM0                       0
#define SPIM0_CONFIG_MODE           DRVI_SPI_MODE_0
#endif

#if (SPIM1_SUPPORT)
#define SPIM1                       1
#define SPIM1_CONFIG_MODE           DRVI_SPI_MODE_3
#endif

#if (SPIM2_SUPPORT)
#define SPIM2                       2
#define SPIM2_CONFIG_MODE           DRVI_SPI_MODE_3
#endif
/* =============================================== */

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

int drvi_SpiInit(uint8_t bBusNum);

int drvi_SpiWrite(uint8_t bBusNum,
                  uint8_t const * pTxBuf,
                  uint8_t         bTxBufLen);

int drvi_SpiRead(uint8_t bBusNum,
                 uint8_t       * pRxBuf,
                 uint8_t         bRxBufLen);

int drvi_SpiWriteThenRead(uint8_t bBusNum,
                          uint8_t const * pTxBuf,
                          uint8_t         bTxBufLen,
                          uint8_t       * pRxBuf,
                          uint8_t         bRxBufLen);



#endif //_DRVI_SPI_H_


