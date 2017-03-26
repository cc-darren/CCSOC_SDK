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
*  uart.c
*
*  Project:
*  --------
*  cc6801
*
*  Description:
*  ------------
*  This is UART driver
*
*  Author:
*  -------
*  CloudChip
*
*===========================================================================/
*  20170206 PAT initial version
******************************************************************************/
#include "global.h"
#include "cc6801_reg.h"
#include "uart.h"

#define CC6801_UART0_DMA (S_regUART *)(UART0_INTR_REG)
#define CC6801_UART0_CONFIG (S_regUART *)(UART0_UnTBUF_REG)

volatile uint32_t UART0_RXDM_INTR = 0;
volatile uint32_t UART0_TXDM_INTR = 0;
volatile uint32_t UART1_RXDM_INTR = 0;
volatile uint32_t UART1_TXDM_INTR = 0;
volatile uint32_t UART2_RXDM_INTR = 0;
volatile uint32_t UART2_TXDM_INTR = 0;

void UART0_RXDMA_IRQHandler(void)
{
    wr(UART0_ADDR_BASE+0x00, 0x00010003);   // clear interrupt
    UART0_RXDM_INTR = 1;
}

void UART0_TXDMA_IRQHandler(void)
{
    wr(UART0_ADDR_BASE+0x00, 0x00020003);   // clear interrupt
    UART0_TXDM_INTR = 1;
}

void UART1_RXDMA_IRQHandler(void)
{
    wr(UART1_ADDR_BASE+0x00, 0x00010003);   // clear interrupt
    UART1_RXDM_INTR = 1;
}

void UART1_TXDMA_IRQHandler(void)
{
    wr(UART1_ADDR_BASE+0x00, 0x00020003);   // clear interrupt
    UART1_TXDM_INTR = 1;
}

void UART2_RXDMA_IRQHandler(void)
{
    wr(UART2_ADDR_BASE+0x00, 0x00010003);   // clear interrupt
    UART2_RXDM_INTR = 1;
}

void UART2_TXDMA_IRQHandler(void)
{
    wr(UART2_ADDR_BASE+0x00, 0x00020003);   // clear interrupt
    UART2_TXDM_INTR = 1;
}

__STATIC_INLINE void cc_uart_config(S_regUART   * p_reg,
                                            cc_uart_parity_t parity,
                                            cc_uart_hwfc_t   hwfc)
{
    //7: Request to send, 1: urts_n=0
    //5: Enable receive DMA
    //4: Enable transmit DMA
    wr(p_reg->unrts, 1);
    wr(p_reg->unfce, hwfc);
    wr(p_reg->unerd, 1);
    wr(p_reg->unetd, 1);

    //7: Enable receive error interrupt
    //6: Enable receiver interrupt
    //5: Enable transmitter interrupt
    //4: Enable flow control interrupt
    wr(p_reg->uneei, 1);
    wr(p_reg->uneri, 1);
    wr(p_reg->uneti, 1);
    wr(p_reg->unefci, 1);

    //8bits data, 1 stop bit, 0 as 9th data bit, parity disable
    wr(p_reg->unpen, parity);
    wr(p_reg->unpsel, 0);
    wr(p_reg->unxb9, 0);
    wr(p_reg->unstp, 0);
    wr(p_reg->unchar, 0);
}

__STATIC_INLINE void cc_uart_config_baudrate(S_regUART  * p_reg, cc_uart_baudrate_t baudrate)
{
    switch (baudrate)
    {
      case CC_UART_BAUDRATE_1200:
        break;
      case CC_UART_BAUDRATE_2400:
        break;
      case CC_UART_BAUDRATE_4800:
        break;
      case CC_UART_BAUDRATE_9600:
        wr(p_reg->undiv_lower, 0x7c);
        wr(p_reg->undiv_upper, 0x0);
        wr(p_reg->unpsc, 0x4);
        wr(p_reg->unovr, 0x10);
        break;
      case CC_UART_BAUDRATE_14400:
        break;
      case CC_UART_BAUDRATE_19200:
        break;
      case CC_UART_BAUDRATE_28800:
        break;
      case CC_UART_BAUDRATE_38400:
        break;
      case CC_UART_BAUDRATE_56000:
        break;
      case CC_UART_BAUDRATE_115200:
        wr(p_reg->undiv_lower, 0x10);
        wr(p_reg->undiv_upper, 0x0);
        wr(p_reg->unpsc, 0x6);
        wr(p_reg->unovr, 0x7);
        break;
      case CC_UART_BAUDRATE_230400:
        break;
      case CC_UART_BAUDRATE_345600:
        break;
      case CC_UART_BAUDRATE_460800:
        break;
      case CC_UART_BAUDRATE_921600:
        break;
      case CC_UART_BAUDRATE_1105920:
        break;
      default:
        break;
    }

}

__STATIC_INLINE void cc_uart_dma_flush_tx(S_regUART * p_reg, uint8_t flush)
{
    wr(p_reg->tx_flush, flush);

}

__STATIC_INLINE void cc_uart_dma_enable_tx(S_regUART * p_reg, uint8_t enable)
{
    wr(p_reg->dma_txen, enable);
}

__STATIC_INLINE void cc_uart_int_enable_tx(S_regUART * p_reg, uint8_t enable)
{
    wr(p_reg->tx_dma_done_intr_en, enable);
}

#if 0
__STATIC_INLINE void cc_uart_int_enable_rx(S_regUART * p_reg, uint8_t enable)
{
    wr(p_reg->rx_dma_done_intr_en, enable);
}
#endif

void apply_config(cc_drv_uart_config_t const * p_config)
{
//    TODO: UART GPIO configuration
//    nrf_gpio_cfg_output(p_config->pseltxd);
//    nrf_gpio_pin_set(p_config->pseltxd);
//    nrf_gpio_cfg_input(p_config->pselrxd, NRF_GPIO_PIN_PULLUP);
//    nrf_uart_txrx_pins_set(NRF_UART0, p_config->pseltxd, p_config->pselrxd);

    cc_uart_config(CC6801_UART0_CONFIG, p_config->parity, p_config->hwfc);
    cc_uart_config_baudrate(CC6801_UART0_CONFIG, p_config->baudrate);
}

int cc_drv_uart_init(cc_drv_uart_config_t const * p_config,
                             cc_uart_event_handler_t      event_handler)
{

    apply_config(p_config);
    cc_uart_int_enable_tx(CC6801_UART0_DMA, 1);

    return CC_SUCCESS;
}

void cc_drv_uart_uninit(void)
{
}

int cc_drv_uart_rx(uint8_t * p_data, uint8_t length)
{
    return 0;
}

void cc_drv_uart_rx_enable(void)
{
}

void tx_byte(S_regUART * p_reg, uint8_t const * const tx_buffer, uint8_t length)
{
    //DMA tx start address
    wr(p_reg->dma_txstrart_addr, (unsigned int)tx_buffer);

    //DMA tx end address
    wr(p_reg->dma_txend_addr, (unsigned int)tx_buffer+length);

	  //UART0 busy waiting until transfer done
	  while(!UART0_RXDM_INTR);
    UART0_RXDM_INTR = 0;
}

int cc_drv_uart_tx(uint8_t const * const p_data, uint8_t length)
{
    bool err_code = CC_SUCCESS;

    //Flush tx dma buffer
    cc_uart_dma_flush_tx(CC6801_UART0_DMA, 1);
    //Write data
    tx_byte(CC6801_UART0_DMA, p_data, length);
    //DMA tx enable
    cc_uart_dma_enable_tx(CC6801_UART0_DMA, 1);

    return err_code;
}

