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

#include "spim.h"
#include "drvi_spi.h"
#include "spi_sensor.h"

//#define SINGLE_WR

static const cc_drv_spi_t spi0 = CC_DRV_SPI_INSTANCE(0);  /**< SPI instance. */
static const cc_drv_spi_t spi1 = CC_DRV_SPI_INSTANCE(1);  /**< SPI instance. */
static const cc_drv_spi_t spi2 = CC_DRV_SPI_INSTANCE(2);  /**< SPI instance. */

static volatile bool spi_xfer_done;  /**< Flag used to indicate that SPI instance completed the transfer. */
static uint8_t m_rx_buf[32];
static uint8_t m_tx_buf[32];


void spi_event_handler(cc_drv_spi_evt_t const * p_event)
{
    spi_xfer_done = true;
    //NRF_LOG_PRINTF(" Transfer completed.\r\n");
    //if (m_rx_buf[0] != 0)
    //{
    //    NRF_LOG_PRINTF(" Received: %s\r\n",m_rx_buf);
    //}
}


void spi_init (uint8_t spi_id)
{
    cc_drv_spi_t spi;
    cc_drv_spi_config_t spi_config = CC_DRV_SPI_DEFAULT_CONFIG();

    switch (spi_id)
    {
        case 0:
            spi = spi0;
            break;
        case 1:
            spi = spi1;
            break;
        case 2:
            spi = spi2;
            break;
        default:
            break;
    }

    drvi_spi_init(&spi, &spi_config, spi_event_handler);
}

void spi_data_write_then_read(uint8_t reg_addr, uint8_t *reg_data, uint8_t cnt, uint8_t spi_id)
{
    int i;
    uint8_t addr;

    addr = reg_addr | 0x80;
    m_tx_buf[0] = addr;
    for(i=1;i<cnt+1;i++)
    {
        m_tx_buf[i] = 0x00;
    }

    if(spi_id == 0)
        drvi_spi_write_then_read(&spi0, m_tx_buf, 1, m_rx_buf, cnt);
    else if(spi_id == 1)
        drvi_spi_write_then_read(&spi1, m_tx_buf, 1, m_rx_buf, cnt);
    else if(spi_id == 2)
        drvi_spi_write_then_read(&spi2, m_tx_buf, 1, m_rx_buf, cnt);
    else
      return;

#if 0
    while (!spi_xfer_done)
    {
        __WFE();
    }
    spi_xfer_done = false;
#endif

    for(i=0;i<cnt;i++)
    {
        *reg_data = m_rx_buf[i];
        reg_data++;
    }
    //spi_xfer_done = false;

}

void spi_data_write(uint8_t reg_addr, uint8_t *reg_data, uint8_t cnt, uint8_t spi_id)
{
    int i;
    uint8_t addr;
    addr = reg_addr & 0xFF;

#ifdef SINGLE_WR
    int k;
    for(i=0;i<cnt;i++)
    {
        k = 2*i;
        m_tx_buf[k] = addr;
        m_tx_buf[k+1] = *reg_data;
        addr++;
        addr = addr & 0x7F;
        reg_data++;
    }

    if(spi_id == 0)
        drvi_spi_write(&spi0, m_tx_buf, 2*cnt);
    else if(spi_id == 1)
        drvi_spi_write(&spi1, m_tx_buf, 2*cnt);
    else if(spi_id == 2)
        drvi_spi_write(&spi2, m_tx_buf, 2*cnt);
    else
      return;
#else
    m_tx_buf[0] = addr;
    for(i=1;i<cnt+1;i++)
    {
        m_tx_buf[i] = *reg_data;
        reg_data++;
    }
    if(spi_id == 0)
        drvi_spi_write(&spi0, m_tx_buf, cnt+1);
    else if(spi_id == 1)
        drvi_spi_write(&spi1, m_tx_buf, cnt+1);
    else if(spi_id == 2)
        drvi_spi_write(&spi2, m_tx_buf, cnt+1);
    else
      return;

#endif
#if 0
    while (!spi_xfer_done)
    {
        __WFE();
    }
    spi_xfer_done = false;
#endif
}


void spi_data_read(uint8_t reg_addr, uint8_t *reg_data, uint8_t cnt, uint8_t spi_id)
{
    int i;

    if(spi_id == 0)
        drvi_spi_read(&spi0, m_rx_buf, cnt);
    else if(spi_id == 1)
        drvi_spi_read(&spi1, m_rx_buf, cnt);
    else if(spi_id == 2)
        drvi_spi_read(&spi2, m_rx_buf, cnt);
    else
      return;

#if 0
    while (!spi_xfer_done)
    {
        __WFE();
    }
    spi_xfer_done = false;
#endif

    for(i=0;i<cnt;i++)
    {
        *reg_data = m_rx_buf[i];
        reg_data++;
}
    //spi_xfer_done = false;

}


