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

/******************************************************************************
*  Filename:
*  ---------
*  spim.c
*
*  Project:
*  --------
*  cc6801
*
*  Description:
*  ------------
*  This is SPI master driver
*
*  Author:
*  -------
*  PATTAYA
*
*===========================================================================/
*  20170203 PAT initial version
******************************************************************************/

#include "spim.h"

volatile uint32_t SPI0_M_INTR = 0;
volatile uint32_t SPI1_M_INTR = 0;
volatile uint32_t SPI2_M_INTR = 0;

void SPI0_M_IRQHandler(void)
{
    wr(SPI0_ADDR_BASE+0x00, 0x00010001);
    SPI0_M_INTR = 1;
}

void SPI1_M_IRQHandler(void)
{
    wr(SPI1_ADDR_BASE+0x00, 0x00010001);
    SPI1_M_INTR = 1;
}

void SPI2_M_IRQHandler(void)
{
    wr(SPI2_ADDR_BASE+0x00, 0x00010001);
    SPI2_M_INTR = 1;
}

#define SPIM_CONFIG_CPOL_RISE_EDGE (0UL) /*!< Active high. */
#define SPIM_CONFIG_CPOL_FALL_EDGE (1UL) /*!< Active low. */

#define SPIM_CONFIG_CPHA_NO_PHASE_SHIFT (0UL) /*!< Sample on leading edge of the clock. Shift serial data on trailing edge. */
#define SPIM_CONFIG_CPHA_PHASE_SHIFT (1UL) /*!< Sample on trailing edge of the clock. Shift serial data on leading edge. */

__STATIC_INLINE void cc_spim_tx_buffer_set(U_regSPI * p_spim,
                                            uint8_t const * p_buffer,
                                            uint8_t         length)
{
    p_spim->bf.dma_str_raddr = (uint32_t)p_buffer;
    p_spim->bf.total_rbyte = length-1;
}

__STATIC_INLINE void cc_spim_rx_buffer_set(U_regSPI * p_spim,
                                            uint8_t const * p_buffer,
                                            uint8_t   length)
{
    p_spim->bf.dma_str_waddr = (uint32_t)p_buffer;
    p_spim->bf.total_wbyte = length-1;
}

__STATIC_INLINE void cc_spim_event_clear(U_regSPI * p_spim)
{
    p_spim->bf.error_int_status = 1;
    p_spim->bf.event_int_status = 1;
}

__STATIC_INLINE void cc_spim_configure(U_regSPI * p_spim,
                                        cc_spim_mode_t spi_mode,
                                        cc_spim_bit_order_t spi_bit_order)
{
    p_spim->bf.wbyte_swap = spi_bit_order;
    p_spim->bf.rbyte_swap = spi_bit_order;

    switch (spi_mode)
    {
        case CC_SPIM_MODE_0:
            p_spim->bf.cpol = SPIM_CONFIG_CPOL_RISE_EDGE;
            p_spim->bf.cpha = SPIM_CONFIG_CPHA_NO_PHASE_SHIFT;
            break;
        case CC_SPIM_MODE_1:
            p_spim->bf.cpol = SPIM_CONFIG_CPOL_RISE_EDGE;
            p_spim->bf.cpha = SPIM_CONFIG_CPHA_PHASE_SHIFT;
            break;
        case CC_SPIM_MODE_2:
            p_spim->bf.cpol = SPIM_CONFIG_CPOL_FALL_EDGE;
            p_spim->bf.cpha = SPIM_CONFIG_CPHA_NO_PHASE_SHIFT;
            break;
        case CC_SPIM_MODE_3:
            p_spim->bf.cpol = SPIM_CONFIG_CPOL_FALL_EDGE;
            p_spim->bf.cpha = SPIM_CONFIG_CPHA_PHASE_SHIFT;
            break;
        default:
            break;
    }

}

int cc_drv_spi_init(cc_drv_spi_t const * const p_instance,
                            cc_drv_spi_config_t const * p_config,
                            cc_drv_spi_handler_t handler)
{
    U_regSPI * p_spim = p_instance->p_registers;

    regCKGEN->bf.spi1ClkDiv = 2;

    cc_spim_configure(p_spim,
        (cc_spim_mode_t)p_config->mode,
        (cc_spim_bit_order_t)p_config->bit_order);

    p_spim->bf.cs_polarity = 1;
    p_spim->bf.spi_m_en = 1;

    p_spim->bf.error_int_en = 1;
    p_spim->bf.event_int_en = 1;

    return CC_SUCCESS;
}

void cc_drv_spi_uninit(cc_drv_spi_t const * const p_instance)
{
}

int cc_drv_spi_transfer(cc_drv_spi_t const * const p_instance,
                                uint8_t const * p_tx_buffer,
                                uint8_t         tx_buffer_length,
                                uint8_t       * p_rx_buffer,
                                uint8_t         rx_buffer_length)
{
    cc_drv_spi_xfer_desc_t xfer_desc;
    xfer_desc.p_tx_buffer = p_tx_buffer;
    xfer_desc.p_rx_buffer = p_rx_buffer;
    xfer_desc.tx_length   = tx_buffer_length;
    xfer_desc.rx_length   = rx_buffer_length;

    return cc_drv_spi_xfer(p_instance, &xfer_desc, 0);
}

static int spim_xfer(U_regSPI                * p_spim,
                           cc_drv_spi_xfer_desc_t const * p_xfer_desc,
                           uint32_t                        flags)
{
    cc_spim_tx_buffer_set(p_spim, p_xfer_desc->p_tx_buffer, p_xfer_desc->tx_length);
    cc_spim_rx_buffer_set(p_spim, p_xfer_desc->p_rx_buffer, p_xfer_desc->rx_length);

    p_spim->bf.op_mode = 2;

    cc_spim_event_clear(p_spim);
    NVIC_EnableIRQ(SPI1_M_IRQn);

    p_spim->bf.spi_m_dma_en = 1;

    while(!SPI1_M_INTR);
    SPI1_M_INTR = 0;

    NVIC_DisableIRQ(SPI1_M_IRQn);
    return CC_SUCCESS;
}

int cc_drv_spi_xfer(cc_drv_spi_t     const * const p_instance,
                            cc_drv_spi_xfer_desc_t const * p_xfer_desc,
                            uint32_t                        flags)
{
    return spim_xfer(p_instance->p_registers,  p_xfer_desc, flags);
}
