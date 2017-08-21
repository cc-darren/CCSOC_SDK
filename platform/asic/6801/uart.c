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

static void (*cb_handler[3])(T_UartEvent *p_event) = {0};

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
        rx_done_event(UART_0, rx_counter[0]);
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
        rx_done_event(UART_1, rx_counter[1]);
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
        rx_done_event(UART_2, rx_counter[2]);
    }
}

void UART2_TXDMA_IRQHandler(void)
{
    wr(UART2_ADDR_BASE+0x00, 0x00020003);   // clear interrupt
    UART2_TXDM_INTR = 1;
}

static void cc6801_UartRxDmaEnable(U_regUARTDMA *pUartBase, uint8_t bEnable)
{
    pUartBase->bf.dma_rxen = bEnable;
}


static void cc6801_UartTxDmaEnable(U_regUARTDMA *pUartBase, uint8_t bEnable)
{
    pUartBase->bf.dma_txen = bEnable;
}

#if 0
static void cc6801_UartTxDmaFlush(U_regUARTDMA *pUartBase, uint8_t bFlush)
{
    pUartBase->bf.tx_flush = bFlush;

}
#endif

static void cc6801_UartRxIntEnable(U_regUARTDMA *pUartBase, uint8_t bEnable)
{
    pUartBase->bf.rx_dma_done_intr_en = bEnable;
}

static void cc6801_UartTxIntEnable(U_regUARTDMA *pUartBase, uint8_t bEnable)
{
    pUartBase->bf.tx_dma_done_intr_en = bEnable;
}

static int cc6801_Uart0Xfer(void const * const pReg)
{
    U_regUARTDMA *pUart0DmaBase = (U_regUARTDMA *)pReg;

    NVIC_EnableIRQ(UART0_TXDMA_IRQn);
    //DMA tx enable
    cc6801_UartTxDmaEnable(pUart0DmaBase, 1);

    //UART0 busy waiting until transfer done
    while(!UART0_TXDM_INTR);
    UART0_TXDM_INTR = 0;
    NVIC_DisableIRQ(UART0_TXDMA_IRQn);

    return CC_SUCCESS;
}

static int cc6801_Uart1Xfer(void const * const pReg)
{
    U_regUARTDMA *pUart1DmaBase = (U_regUARTDMA *)pReg;

    NVIC_EnableIRQ(UART1_TXDMA_IRQn);
    //DMA tx enable
    cc6801_UartTxDmaEnable(pUart1DmaBase, 1);

    //UART1 busy waiting until transfer done
    while(!UART1_TXDM_INTR);
    UART1_TXDM_INTR = 0;
    NVIC_DisableIRQ(UART1_TXDMA_IRQn);

    return CC_SUCCESS;
}

static int cc6801_Uart2Xfer(void const * const pReg)
{
    U_regUARTDMA *pUart2DmaBase = (U_regUARTDMA *)pReg;

    NVIC_EnableIRQ(UART2_TXDMA_IRQn);
    //DMA tx enable
    cc6801_UartTxDmaEnable(pUart2DmaBase, 1);

    //UART2 busy waiting until transfer done
    while(!UART2_TXDM_INTR);
    UART2_TXDM_INTR = 0;
    NVIC_DisableIRQ(UART2_TXDMA_IRQn);

    return CC_SUCCESS;
}

static int cc6801_Uart0Rcvr(void const * const pReg)
{
    U_regUARTDMA *pUart0DmaBase = (U_regUARTDMA *)pReg;

    NVIC_EnableIRQ(UART0_RXDMA_IRQn);

    cc6801_UartRxDmaEnable(pUart0DmaBase, 1);
    if (cb_handler[0])
        return CC_SUCCESS;

    while(!UART0_RXDM_INTR);
    UART0_RXDM_INTR = 0;
    //NVIC_DisableIRQ(UART0_RXDMA_IRQn);

    return CC_SUCCESS;
}

static int cc6801_Uart1Rcvr(void const * const pReg)
{
    U_regUARTDMA *pUart1DmaBase = (U_regUARTDMA *)pReg;

    NVIC_EnableIRQ(UART1_RXDMA_IRQn);

    cc6801_UartRxDmaEnable(pUart1DmaBase, 1);
    if (cb_handler[1])
        return CC_SUCCESS;

    while(!UART1_RXDM_INTR);
    UART1_RXDM_INTR = 0;
    //NVIC_DisableIRQ(UART1_RXDMA_IRQn);

    return CC_SUCCESS;
}

static int cc6801_Uart2Rcvr(void const * const pReg)
{
    U_regUARTDMA *pUart2DmaBase = (U_regUARTDMA *)pReg;

    NVIC_EnableIRQ(UART2_RXDMA_IRQn);

    cc6801_UartRxDmaEnable(pUart2DmaBase, 1);
    if (cb_handler[2])
        return CC_SUCCESS;

    while(!UART2_RXDM_INTR);
    UART2_RXDM_INTR = 0;
    //NVIC_DisableIRQ(UART2_RXDMA_IRQn);

    return CC_SUCCESS;
}


static void cc6801_UartTxBufferSet(U_regUARTDMA * pUartBase,
                                            uint8_t const *pTxBuf,
                                            uint8_t bLen)
{
    pUartBase->bf.dma_txbyte_num = bLen - 1;
    //DMA tx start address
    pUartBase->bf.dma_txstrart_addr = (uint32_t)pTxBuf;
    //DMA tx end address
    pUartBase->bf.dma_txend_addr = (uint32_t)pTxBuf + (((bLen - 1) >> 2) << 2);
}

static void cc6801_UartRxBufferSet(U_regUARTDMA *pUartBase,
                                            uint8_t const *pRxBuf,
                                            uint8_t bLen)
{
    pUartBase->bf.dma_rxbyte_num = bLen - 1;
    //DMA rx start address
    pUartBase->bf.dma_rxstrart_addr = (uint32_t)pRxBuf;
    //DMA rx end address
    pUartBase->bf.dma_rxend_addr = (uint32_t)pRxBuf + (((bLen - 1) >> 2) << 2);
}

static T_cc6801UartPort cc6801_UartPortGet(uint8_t bPortNum)
{
    T_cc6801UartPort port;

    switch (bPortNum)
    {
        case UART_0:
            port.pDmaReg = (void *)regUART0DMA,
            port.pCtrlReg = (void *)regUART0CTRL,
            port.index = UART_0,
            port.fpUartXfer = cc6801_Uart0Xfer;
            port.fpUartRcvr = cc6801_Uart0Rcvr;
            break;
        case UART_1:
            port.pDmaReg = (void *)regUART1DMA,
            port.pCtrlReg = (void *)regUART1CTRL,
            port.index = UART_1,
            port.fpUartXfer = cc6801_Uart1Xfer;
            port.fpUartRcvr = cc6801_Uart1Rcvr;
            break;
        case UART_2:
            port.pDmaReg = (void *)regUART2DMA,
            port.pCtrlReg = (void *)regUART2CTRL,
            port.index = UART_2,
            port.fpUartXfer = cc6801_Uart2Xfer;
            port.fpUartRcvr = cc6801_Uart2Rcvr;
            break;
        default:
            break;
    }

    return port;
}

static uint8_t cc6801_ComputeFRS(uint32_t dwConfig)
{
    uint8_t bFrs;

    switch (dwConfig & DRVI_UART_SIZE)
    {
        case DRVI_UART_S7:
            bFrs = UART_FRS_CHAR_WLEN7_BIT;
            break;
        case DRVI_UART_S9:
            bFrs = UART_FRS_CHAR_WLEN9_BIT;
            break;
        default:
        case DRVI_UART_S8:
            bFrs = UART_FRS_CHAR_WLEN8_BIT;
            break;
    }

    if (dwConfig & DRVI_UART_STOPB)
        bFrs |= UART_FRS_STP_2BIT_BIT;

    if (dwConfig & DRVI_UART_XB9)
        bFrs |= UART_FRS_XB9_1_MASK;

    if (bFrs & DRVI_UART_PARENB)
        bFrs |= UART_FRS_PEN_ENABLE_MASK;

    if (dwConfig & DRVI_UART_SPAR)
        bFrs |= UART_FRS_PSEL_SPAR_MASK;
    else if (dwConfig & DRVI_UART_MPAR)
        bFrs |= UART_FRS_PSEL_MPAR_MASK;
    else if (dwConfig & DRVI_UART_PAREVEN)
        bFrs |= UART_FRS_PSEL_EPAR_MASK;
    else
        bFrs |= UART_FRS_PSEL_OPAR_MASK;

    return bFrs;
}

static int cc6801_UartBaudrateSet(T_UartPort *pUartPort)
{
    T_cc6801UartPort port = cc6801_UartPortGet(pUartPort->bPortNum);
    U_regUARTCTRL *pUartCtrlBase = port.pCtrlReg;
	uint32_t dwBaud;

	dwBaud = pUartPort->dwConfig & DRVI_UART_BAUD;

    switch (dwBaud)
    {
      case DRVI_UART_B1200:
        pUartCtrlBase->dw.baud = 0x40;
        pUartCtrlBase->dw.psr = ((0x3 << 3) | 0x3);
        pUartCtrlBase->dw.ovr = 0x0;
        break;
      case DRVI_UART_B2400:
        pUartCtrlBase->dw.baud = 0x40;
        pUartCtrlBase->dw.psr = ((0x1 << 3) | 0x3);
        pUartCtrlBase->dw.ovr = 0x0;
        break;
      case DRVI_UART_B4800:
        pUartCtrlBase->dw.baud = 0xA0;
        pUartCtrlBase->dw.psr = ((0x1 << 3) | 0x1);
        pUartCtrlBase->dw.ovr = 0x0;
        break;
      case DRVI_UART_B9600:
        //FPGA DEMO setting, clk=16M
        pUartCtrlBase->dw.baud = 0x53;
        pUartCtrlBase->dw.psr = ((0x4 << 3) | 0x0);
        pUartCtrlBase->dw.ovr = 0x0;
        break;
//      case DRVI_UART_B14400:
//        break;
      case DRVI_UART_B19200:
        pUartCtrlBase->dw.baud = 0xA7;
        pUartCtrlBase->dw.psr = ((0x1 << 3) | 0x0);
        pUartCtrlBase->dw.ovr = 0x0A;
        break;
//      case DRVI_UART_B28800:
//        break;
      case DRVI_UART_B38400:
        pUartCtrlBase->dw.baud = 0x53;
        pUartCtrlBase->dw.psr = ((0x1 << 3) | 0x0);
        pUartCtrlBase->dw.ovr = 0x0A;
        break;
      case DRVI_UART_B230400:
        pUartCtrlBase->dw.baud = 0x0A;
        pUartCtrlBase->dw.psr = ((0xC << 3) | 0x0);
        pUartCtrlBase->dw.ovr = 0x07;
        break;
//      case DRVI_UART_B345600:
//        break;
      case DRVI_UART_B460800:
        pUartCtrlBase->dw.baud = 0x14;
        pUartCtrlBase->dw.psr = ((0xC << 3) | 0x0);
        pUartCtrlBase->dw.ovr = 0x07;
        break;
      case DRVI_UART_B921600:
        pUartCtrlBase->dw.baud = 0x28;
        pUartCtrlBase->dw.psr = ((0xC << 3) | 0x0);
        pUartCtrlBase->dw.ovr = 0x07;
        break;
//      case DRVI_UART_B1105920:
//        break;
      case DRVI_UART_B57600:
      default:
      case DRVI_UART_B115200:
        //FPGA DEMO setting, clk=16M
        pUartCtrlBase->dw.baud = 0x05;
        pUartCtrlBase->dw.psr = ((0xC << 3) | 0x0);
        pUartCtrlBase->dw.ovr = 0x07;
        break;
    }

    return CC_SUCCESS;
}

static void cc6801_UartConfigSet(T_UartPort *pUartPort)
{
    T_cc6801UartPort port = cc6801_UartPortGet(pUartPort->bPortNum);
    U_regUARTCTRL *pUartCtrlBase = port.pCtrlReg;
    uint8_t bFrs, bMdsl, bIntCtrl;

    bFrs = cc6801_ComputeFRS(pUartPort->dwConfig);
    cc6801_UartBaudrateSet(pUartPort);

    bIntCtrl |= UART_CTRL_INT_ETI_ENABLE_MASK;
    bIntCtrl |= UART_CTRL_INT_EEI_ENABLE_MASK;

    if (pUartPort->dwConfig & DRVI_UART_READ)
    {
        bIntCtrl |= UART_CTRL_INT_ERI_ENABLE_MASK;
    }

    bMdsl |= UART_MDSL_RTS_ENABLE_MASK;

    if (pUartPort->dwConfig & DRVI_UART_RTSCTS)
    {
        bIntCtrl |= UART_CTRL_INT_EFCI_ENABLE_MASK;
        bMdsl |= UART_MDSL_FCE_ENABLE_MASK;
    }

#ifdef UART_DMA_ENABLE
    bMdsl |= UART_MDSL_ETD_ENABLE_MASK;

    if (pUartPort->dwConfig & DRVI_UART_READ)
        bMdsl |= UART_MDSL_ERD_ENABLE_MASK;
#endif

    pUartCtrlBase->dw.ictrl = bIntCtrl;
    pUartCtrlBase->dw.frs = bFrs;
    pUartCtrlBase->dw.mdsl = bMdsl;

}

__STATIC_INLINE void rx_done_event(uint8_t idx, uint8_t bytes)
{
    T_UartEvent event;

    event.type             = DRVI_UART_EVENT_RX_DONE;
    event.bRxBytes  = bytes;
    event.pRxBuf = p_rx_buffer[idx];

    rx_buffer_length[idx] = 0;

    cb_handler[idx](&event);
}

int cc6801_UartInit(T_UartPort *pUartPort)
{
    T_cc6801UartPort port;
    int error = CC_SUCCESS;

    port = cc6801_UartPortGet(pUartPort->bPortNum);

    cc6801_UartConfigSet(pUartPort);

    cc6801_UartTxIntEnable(port.pDmaReg, 1);
    cc6801_UartRxIntEnable(port.pDmaReg, 1);

    if (pUartPort->fpComplete)
        cb_handler[port.index] = pUartPort->fpComplete;

    if ((pUartPort->dwConfig & DRVI_UART_READ) == 0)
    {
        NVIC_DisableIRQ(UART0_RXDMA_IRQn);
        cc6801_UartRxIntEnable(port.pDmaReg, 0);
    }

    return error;
}

int cc6801_UartRx(T_UartPort *pUartPort,
                    uint8_t * pData, uint8_t bLen)
{
    T_cc6801UartPort port;

    port = cc6801_UartPortGet(pUartPort->bPortNum);

    p_rx_buffer[port.index]      = pData;
    rx_buffer_length[port.index] = bLen;
    rx_counter[port.index]       = 0;

#ifdef UART_DMA_ENABLE
    if ((uint32_t)pData & 0x3UL)
      return CC_ERROR_INVALID_ADDR;

    cc6801_UartRxBufferSet(port.pDmaReg, pData, bLen);
    rx_counter[port.index] = bLen;

    return port.fpUartRcvr(port.pDmaReg);
#else
    U_regUARTCTRL *pUart0CtrlBase = (U_regUARTCTRL *)port.pCtrlReg;

    do {
        *(p_rx_buffer[port.index]+rx_counter[port.index]) = (uint8_t)pUart0CtrlBase->dw.bufRx;
        rx_counter[port.index]++;
    } while (rx_buffer_length[port.index] > rx_counter[port.index]);

    rx_done_event(port.index, rx_counter[port.index]);

    return CC_SUCCESS;
#endif
}

int cc6801_UartTx(T_UartPort *pUartPort,
                    uint8_t const * const pData, uint8_t bLen)
{
    T_cc6801UartPort port;

    port = cc6801_UartPortGet(pUartPort->bPortNum);

#ifdef UART_DMA_ENABLE
    if ((uint32_t)pData & 0x3UL)
      return CC_ERROR_INVALID_ADDR;

    cc6801_UartTxBufferSet(port.pDmaReg, pData, bLen);

    return port.fpUartXfer(port.pDmaReg);
#else
    U_regUARTCTRL *pUart0CtrlBase = (U_regUARTCTRL *)port.pCtrlReg;
    uint8_t const *pTxBuf = pData;

    while (*pTxBuf)
    {
        pUart0CtrlBase->dw.bufTx = *pTxBuf++;
        while(!pUart0CtrlBase->bf.untbe);
    }

    return CC_SUCCESS;
#endif
}

