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
#include "spi_sensor.h"

//#define SINGLE_WR
#define SPI1_ENABLED 1

static const cc_drv_spi_t spi0 = CC_DRV_SPI_INSTANCE(0);  /**< SPI instance. */
static const cc_drv_spi_t spi1 = CC_DRV_SPI_INSTANCE(1);  /**< SPI instance. */

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

    if(spi_id == 0)
    {
        cc_drv_spi_config_t spi0_config = CC_DRV_SPI_DEFAULT_CONFIG(0);
        cc_drv_spi_init(&spi0, &spi0_config, spi_event_handler);
    }
#if SPI1_ENABLED
    else
    {
        cc_drv_spi_config_t spi1_config = CC_DRV_SPI_DEFAULT_CONFIG(1);
        cc_drv_spi_init(&spi1, &spi1_config, spi_event_handler);
    }
#endif

}

void spi_data_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data, uint8_t cnt, uint8_t spi_id)
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
        cc_drv_spi_transfer(&spi0, m_tx_buf, 1, m_rx_buf, cnt+1);
#if SPI1_ENABLED
    else
        cc_drv_spi_transfer(&spi1, m_tx_buf, 1, m_rx_buf, cnt);
#endif

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

void spi_data_write(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data, uint8_t cnt, uint8_t spi_id)
{
    int i;
    uint8_t addr;
    addr = reg_addr & 0x7F;

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
        cc_drv_spi_transfer(&spi0, m_tx_buf, 2*cnt, m_rx_buf, 1);
#if SPI1_ENABLED
    else
        cc_drv_spi_transfer(&spi1, m_tx_buf, 2*cnt, m_rx_buf, 1);
#endif

#else
    m_tx_buf[0] = addr;
    for(i=1;i<cnt+1;i++)
    {
        m_tx_buf[i] = *reg_data;
        reg_data++;
    }
    if(spi_id == 0)
        cc_drv_spi_transfer(&spi0, m_tx_buf, cnt+1, m_rx_buf, 1);
#if SPI1_ENABLED
    else
        cc_drv_spi_transfer(&spi1, m_tx_buf, cnt+1, m_rx_buf, 1);
#endif

#endif
#if 0
    while (!spi_xfer_done)
    {
        __WFE();
    }
    spi_xfer_done = false;
#endif
}



