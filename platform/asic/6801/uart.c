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

#include "uart.h"

#define UART_DMA_ENABLE

static uint8_t *p_rx_buffer[3];
static uint32_t rx_counter[3] = {0};
static uint32_t rx_buffer_length[3]  = {0};

static cc_uart_event_handler_t cb_handler[3] = {0};

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
    if (rx_buffer_length[0])
    {
        NVIC_DisableIRQ(UART0_RXDMA_IRQn);
        rx_done_event(UART0_INSTANCE_INDEX, rx_counter[0]);
    }
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
    if (rx_buffer_length[1])
    {
        NVIC_DisableIRQ(UART1_RXDMA_IRQn);
        rx_done_event(UART1_INSTANCE_INDEX, rx_counter[1]);
    }
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
    if (rx_buffer_length[2])
    {
        NVIC_DisableIRQ(UART2_RXDMA_IRQn);
        rx_done_event(UART2_INSTANCE_INDEX, rx_counter[2]);
    }
}

void UART2_TXDMA_IRQHandler(void)
{
    wr(UART2_ADDR_BASE+0x00, 0x00020003);   // clear interrupt
    UART2_TXDM_INTR = 1;
}

__STATIC_INLINE void cc_uart_tx_buffer_set(U_regUARTDMA * p_uart_dma,
                                            uint8_t const * tx_buffer,
                                            uint8_t         length)
{
    p_uart_dma->bf.dma_txbyte_num = length-1;
    //DMA tx start address
    p_uart_dma->bf.dma_txstrart_addr = (uint32_t)tx_buffer;
    //DMA tx end address
    p_uart_dma->bf.dma_txend_addr = (uint32_t)tx_buffer+(((length-1)>>2) << 2);
}

__STATIC_INLINE void cc_uart_rx_buffer_set(U_regUARTDMA * p_uart_dma,
                                            uint8_t const * rx_buffer,
                                            uint8_t   length)
{
    p_uart_dma->bf.dma_rxbyte_num = length-1;
    //DMA tx start address
    p_uart_dma->bf.dma_rxstrart_addr = (uint32_t)rx_buffer;
    //DMA tx end address
    p_uart_dma->bf.dma_rxend_addr = (uint32_t)rx_buffer+(((length-1)>>2) << 2);
}
__STATIC_INLINE void cc_uart_config(U_regUARTCTRL   * p_reg,
                                        drvi_uart_params_t const * config)
{
    //7: Request to send, 1: urts_n=0
    //5: Enable receive DMA
    //4: Enable transmit DMA
    p_reg->bf.unrts = 1;
    p_reg->bf.unfce = config->hw_flow;
    p_reg->bf.unerd = 1;
    p_reg->bf.unetd = 1;

    //7: Enable receive error interrupt
    //6: Enable receiver interrupt
    //5: Enable transmitter interrupt
    //4: Enable flow control interrupt
    p_reg->bf.uneei = 1;
    p_reg->bf.uneri = 1;
    p_reg->bf.uneti = 1;
    p_reg->bf.unefci = 1;

    //8bits data, 1 stop bit, 0 as 9th data bit, parity disable
    if (DRVI_UART_PARITY_DISABLE != config->parity)
    {
        p_reg->bf.unpen = 1;
        p_reg->bf.unpsel = config->parity;
    }
    p_reg->bf.unxb9 = 0;
    p_reg->bf.unstp = config->stop_bits;
    p_reg->bf.unchar = config->data_bits;
}

__STATIC_INLINE int cc_uart_config_baudrate(U_regUARTCTRL  * p_reg, drvi_uart_baudrate_t baudrate)
{
    switch (baudrate)
    {
      case DRVI_UART_BAUDRATE_1200:
        p_reg->bf.undiv_lower = 0x40;
        p_reg->bf.undiv_upper = 0x3;
        p_reg->bf.unpsc = 0x3;
        p_reg->bf.unovr = 0x0;
        break;
      case DRVI_UART_BAUDRATE_2400:
        p_reg->bf.undiv_lower = 0x40;
        p_reg->bf.undiv_upper = 0x3;
        p_reg->bf.unpsc = 0x1;
        p_reg->bf.unovr = 0x0;
        break;
      case DRVI_UART_BAUDRATE_4800:
        p_reg->bf.undiv_lower = 0xA0;
        p_reg->bf.undiv_upper = 0x1;
        p_reg->bf.unpsc = 0x1;
        p_reg->bf.unovr = 0x0;
        break;
      case DRVI_UART_BAUDRATE_9600:
        //FPGA DEMO setting, clk=16M
        p_reg->bf.undiv_lower = 0x53;
        //p_reg->bf.undiv_lower = 0x7c;
        p_reg->bf.undiv_upper = 0x0;
        p_reg->bf.unpsc = 0x4;
        p_reg->bf.unovr = 0x0;
        break;
//      case CC_UART_BAUDRATE_14400:
//        break;
      case DRVI_UART_BAUDRATE_19200:
        p_reg->bf.undiv_lower = 0xA7;
        p_reg->bf.undiv_upper = 0x0;
        p_reg->bf.unpsc = 0x1;
        p_reg->bf.unovr = 0x0A;
        break;
//      case CC_UART_BAUDRATE_28800:
//        break;
      case DRVI_UART_BAUDRATE_38400:
        p_reg->bf.undiv_lower = 0x53;
        p_reg->bf.undiv_upper = 0x0;
        p_reg->bf.unpsc = 0x1;
        p_reg->bf.unovr = 0x0A;
        break;
      case DRVI_UART_BAUDRATE_115200:
        //FPGA DEMO setting, clk=16M
        p_reg->bf.undiv_lower = 0x05;
        //p_reg->bf.undiv_lower = 0x10;
        p_reg->bf.undiv_upper = 0x0;
        p_reg->bf.unpsc = 0xC;
        p_reg->bf.unovr = 0x7;
        break;
      case DRVI_UART_BAUDRATE_230400:
        p_reg->bf.undiv_lower = 0x0A;
        p_reg->bf.undiv_upper = 0x0;
        p_reg->bf.unpsc = 0xC;
        p_reg->bf.unovr = 0x7;
        break;
//      case CC_UART_BAUDRATE_345600:
//        break;
      case DRVI_UART_BAUDRATE_460800:
        p_reg->bf.undiv_lower = 0x14;
        p_reg->bf.undiv_upper = 0x0;
        p_reg->bf.unpsc = 0xC;
        p_reg->bf.unovr = 0x7;
        break;
      case DRVI_UART_BAUDRATE_921600:
        p_reg->bf.undiv_lower = 0x28;
        p_reg->bf.undiv_upper = 0x0;
        p_reg->bf.unpsc = 0xC;
        p_reg->bf.unovr = 0x7;
        break;
//      case CC_UART_BAUDRATE_1105920:
//        break;
      case DRVI_UART_BAUDRATE_57600:
      default:
        return CC_ERROR_INVALID_DATA;
    }

    return CC_SUCCESS;
}

__STATIC_INLINE void cc_uart_int_enable_rx(U_regUARTDMA * p_reg, uint8_t enable)
{
    p_reg->bf.rx_dma_done_intr_en = enable;
}

__STATIC_INLINE void cc_uart_int_enable_tx(U_regUARTDMA * p_reg, uint8_t enable)
{
    p_reg->bf.tx_dma_done_intr_en = enable;
}

#ifdef UART_DMA_ENABLE
__STATIC_INLINE void cc_uart_dma_enable_rx(U_regUARTDMA * p_reg, uint8_t enable)
{
    p_reg->bf.dma_rxen = enable;
}


__STATIC_INLINE void cc_uart_dma_enable_tx(U_regUARTDMA * p_reg, uint8_t enable)
{
    p_reg->bf.dma_txen = enable;
}
#endif

#if 0
__STATIC_INLINE void cc_uart_dma_flush_tx(U_regUARTDMA * p_reg, uint8_t flush)
{
    p_reg->bf.tx_flush = flush;

}
#endif

int cc_drv_uart_init(cc_drv_uart_t const * const p_instance,
                             drvi_uart_params_t const * p_config,
                             cc_uart_event_handler_t      event_handler)
{
    int error = CC_SUCCESS;

    cc_uart_config(p_instance->p_ctrl_reg, p_config);
    error = cc_uart_config_baudrate(p_instance->p_ctrl_reg, p_config->baudrate);
    cc_uart_int_enable_tx(p_instance->p_dma_reg, 1);
    cc_uart_int_enable_rx(p_instance->p_dma_reg, 1);

    cb_handler[p_instance->drv_inst_idx] = event_handler;

    return error;
}

void cc_drv_uart_uninit(void)
{
}

__STATIC_INLINE void rx_done_event(uint8_t idx, uint8_t bytes)
{
    cc_drv_uart_event_t event;

    event.type             = CC_DRV_UART_EVT_RX_DONE;
    event.data.rxtx.bytes  = bytes;
    event.data.rxtx.p_data = p_rx_buffer[idx];

    rx_buffer_length[idx] = 0;

    cb_handler[idx](&event, 0);
}

static int uart0_rcvr(cc_drv_uart_t const * const p_instance)
{
#ifdef UART_DMA_ENABLE
    U_regUARTDMA * p_reg = p_instance->p_dma_reg;

    NVIC_EnableIRQ(UART0_RXDMA_IRQn);

    cc_uart_dma_enable_rx(p_reg, 1);
    if (cb_handler[0])
        return CC_SUCCESS;

    while(!UART0_RXDM_INTR);
    UART0_RXDM_INTR = 0;
    //NVIC_DisableIRQ(UART0_RXDMA_IRQn);
#else
    U_regUARTCTRL * p_reg = p_instance->p_ctrl_reg;

    do {
        *(p_rx_buffer[0]+rx_counter[0]) = (uint8_t)p_reg->dw.bufRx;
        rx_counter[0]++;
    } while (rx_buffer_length[0] > rx_counter[0]);

    rx_done_event(UART0_INSTANCE_INDEX, rx_counter[0]);
#endif

    return CC_SUCCESS;
}

static int uart1_rcvr(cc_drv_uart_t const * const p_instance)
{
#ifdef UART_DMA_ENABLE
    U_regUARTDMA * p_reg = p_instance->p_dma_reg;

    NVIC_EnableIRQ(UART1_RXDMA_IRQn);

    cc_uart_dma_enable_rx(p_reg, 1);
    if (cb_handler[1])
        return CC_SUCCESS;

    while(!UART1_RXDM_INTR);
    UART1_RXDM_INTR = 0;
    //NVIC_DisableIRQ(UART1_RXDMA_IRQn);
#else
    U_regUARTCTRL * p_reg = p_instance->p_ctrl_reg;

    do {
        *(p_rx_buffer[1]+rx_counter[1]) = (uint8_t)p_reg->dw.bufRx;
        rx_counter[1]++;
    } while (rx_buffer_length[1] > rx_counter[1]);

    rx_done_event(UART1_INSTANCE_INDEX, rx_counter[1]);
#endif

    return CC_SUCCESS;
}

static int uart2_rcvr(cc_drv_uart_t const * const p_instance)
{
#ifdef UART_DMA_ENABLE
    U_regUARTDMA * p_reg = p_instance->p_dma_reg;

    NVIC_EnableIRQ(UART2_RXDMA_IRQn);

    cc_uart_dma_enable_rx(p_reg, 1);
    if (cb_handler[2])
        return CC_SUCCESS;

    while(!UART2_RXDM_INTR);
    UART2_RXDM_INTR = 0;
    //NVIC_DisableIRQ(UART2_RXDMA_IRQn);
#else
    U_regUARTCTRL * p_reg = p_instance->p_ctrl_reg;

    do {
        *(p_rx_buffer[2]+rx_counter[2]) = (uint8_t)p_reg->dw.bufRx;
        rx_counter[2]++;
    } while (rx_buffer_length[2] > rx_counter[2]);

    rx_done_event(UART2_INSTANCE_INDEX, rx_counter[2]);
#endif

    return CC_SUCCESS;
}

int cc_drv_uart_rx(cc_drv_uart_t const * const p_instance,
                                  uint8_t * p_data, uint8_t length)
{
    int  intance = p_instance->drv_inst_idx;
    int  (*uart_rcvr)(cc_drv_uart_t const * const p_instance);

    p_rx_buffer[intance]      = p_data;
    rx_buffer_length[intance] = length;
    rx_counter[intance]       = 0;

#ifdef UART_DMA_ENABLE
    if ((uint32_t)p_data & 0x3UL)
      return CC_ERROR_INVALID_ADDR;

    cc_uart_rx_buffer_set(p_instance->p_dma_reg, p_data, length);
    rx_counter[intance]       = length;
#endif

    switch (intance)
    {
        case 0:
            uart_rcvr = uart0_rcvr;
            break;
        case 1:
            uart_rcvr = uart1_rcvr;
            break;
        case 2:
            uart_rcvr = uart2_rcvr;
            break;
        default:
            break;
    }

    return uart_rcvr(p_instance);
}

static int uart0_xfer(cc_drv_uart_t const * const p_instance)
{
#ifdef UART_DMA_ENABLE
    U_regUARTDMA * p_reg = p_instance->p_dma_reg;

    NVIC_EnableIRQ(UART0_TXDMA_IRQn);
    //DMA tx enable
    cc_uart_dma_enable_tx(p_reg, 1);

    //UART0 busy waiting until transfer done
    while(!UART0_TXDM_INTR);
    UART0_TXDM_INTR = 0;
    NVIC_DisableIRQ(UART0_TXDMA_IRQn);
#else
    U_regUARTCTRL * p_reg = p_instance->p_ctrl_reg;

    while (*tx_buffer)
    {
        p_reg->dw.bufTx = *tx_buffer++;
        while(!p_reg->bf.untbe);
    }
#endif

    return CC_SUCCESS;
}

static int uart1_xfer(cc_drv_uart_t const * const p_instance)
{
#ifdef UART_DMA_ENABLE
    U_regUARTDMA * p_reg = p_instance->p_dma_reg;

    NVIC_EnableIRQ(UART1_TXDMA_IRQn);
    //DMA tx enable
    cc_uart_dma_enable_tx(p_reg, 1);

    //UART1 busy waiting until transfer done
    while(!UART1_TXDM_INTR);
    UART1_TXDM_INTR = 0;
    NVIC_DisableIRQ(UART1_TXDMA_IRQn);
#else
    U_regUARTCTRL * p_reg = p_instance->p_ctrl_reg;

    while (*tx_buffer)
    {
        p_reg->dw.bufTx = *tx_buffer++;
        while(!p_reg->bf.untbe);
    }
#endif

    return CC_SUCCESS;
}

static int uart2_xfer(cc_drv_uart_t const * const p_instance)
{
#ifdef UART_DMA_ENABLE
    U_regUARTDMA * p_reg = p_instance->p_dma_reg;

    NVIC_EnableIRQ(UART2_TXDMA_IRQn);
    //DMA tx enable
    cc_uart_dma_enable_tx(p_reg, 1);

    //UART2 busy waiting until transfer done
    while(!UART2_TXDM_INTR);
    UART2_TXDM_INTR = 0;
    NVIC_DisableIRQ(UART2_TXDMA_IRQn);
#else
    U_regUARTCTRL * p_reg = p_instance->p_ctrl_reg;

    while (*tx_buffer)
    {
        p_reg->dw.bufTx = *tx_buffer++;
        while(!p_reg->bf.untbe);
    }
#endif

    return CC_SUCCESS;
}

int cc_drv_uart_tx(cc_drv_uart_t const * const p_instance,
                            uint8_t const * const p_data, uint8_t length)
{
    int  intance = p_instance->drv_inst_idx;
    int  (*uart_xfer)(cc_drv_uart_t const * const p_instance);

#ifdef UART_DMA_ENABLE
    if ((uint32_t)p_data & 0x3UL)
      return CC_ERROR_INVALID_ADDR;

    cc_uart_tx_buffer_set(p_instance->p_dma_reg, p_data, length);
#endif

    switch (intance)
    {
        case 0:
            uart_xfer = uart0_xfer;
            break;
        case 1:
            uart_xfer = uart1_xfer;
            break;
        case 2:
            uart_xfer = uart2_xfer;
            break;
        default:
            break;
    }

    return uart_xfer(p_instance);
}

