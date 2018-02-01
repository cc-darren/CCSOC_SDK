/* Copyright (c) 2018 Cloudchip, Inc. All Rights Reserved.
 *
 * The information contained herein is property of Cloudchip, Inc.
 * Terms and conditions of usage are described in detail in CLOUDCHIP
 * STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information.
 * NO WARRANTY of ANY KIND is provided. This heading must NOT be removed 
 * from the file.
 */

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
#include "drvi_uart.h"

volatile uint32_t UART0_RXDM_INTR = 0;
volatile uint32_t UART0_TXDM_INTR = 0;
volatile uint32_t UART1_RXDM_INTR = 0;
volatile uint32_t UART1_TXDM_INTR = 0;
volatile uint32_t UART2_RXDM_INTR = 0;
volatile uint32_t UART2_TXDM_INTR = 0;

T_cc6801UartPort g_tUartPort[UART_TOTAL_SUPPORTED] = {0};

static void cc6801_RxDone(uint8_t bIdx, uint8_t bBytes)
{
    T_UartEvent tEvent;

    tEvent.type      = DRVI_UART_EVENT_RX_DONE;
    tEvent.bRxBytes  = bBytes;
    tEvent.pRxBuf    = g_tUartPort[bIdx].pRxBuffer;

    g_tUartPort[bIdx].dwRxBufferLen = 0;

    g_tUartPort[bIdx].fbRxDoneHandler(&tEvent);
}

void UART0_RXDMA_IRQHandler(void)
{
    wr(UART0_ADDR_BASE+0x00, 0x00010003);   // clear interrupt
    UART0_RXDM_INTR = 1;
    if (g_tUartPort[UART_0].dwRxBufferLen)
    {
        NVIC_DisableIRQ(UART0_RXDMA_IRQn);
        cc6801_RxDone(UART_0, g_tUartPort[UART_0].dwRxCount);
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
    if (g_tUartPort[UART_1].dwRxBufferLen)
    {
        NVIC_DisableIRQ(UART1_RXDMA_IRQn);
        cc6801_RxDone(UART_1, g_tUartPort[UART_1].dwRxCount);
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
    if (g_tUartPort[UART_2].dwRxBufferLen)
    {
        NVIC_DisableIRQ(UART2_RXDMA_IRQn);
        cc6801_RxDone(UART_2, g_tUartPort[UART_2].dwRxCount);
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
    if (g_tUartPort[UART_0].fbRxDoneHandler)
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
    if (g_tUartPort[UART_1].fbRxDoneHandler)
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
    if (g_tUartPort[UART_2].fbRxDoneHandler)
        return CC_SUCCESS;

    while(!UART2_RXDM_INTR);
    UART2_RXDM_INTR = 0;
    //NVIC_DisableIRQ(UART2_RXDMA_IRQn);

    return CC_SUCCESS;
}


static void cc6801_UartTxBufferSet(U_regUARTDMA * pUartBase,
                                    uint32_t dwTxBufAddr, uint8_t bLen)
{
    pUartBase->bf.dma_txbyte_num = bLen - 1;
    //DMA tx start address
    pUartBase->bf.dma_txstrart_addr = dwTxBufAddr;
    //DMA tx end address
    pUartBase->bf.dma_txend_addr = dwTxBufAddr + (((bLen - 1) >> 2) << 2);
}

static void cc6801_UartRxBufferSet(U_regUARTDMA *pUartBase,
                                    uint32_t dwRxBufAddr, uint8_t bLen)
{
    pUartBase->bf.dma_rxbyte_num = bLen - 1;
    //DMA rx start address
    pUartBase->bf.dma_rxstrart_addr = dwRxBufAddr;
    //DMA rx end address
    pUartBase->bf.dma_rxend_addr = dwRxBufAddr + (((bLen - 1) >> 2) << 2);
}

static uint8_t cc6801_ComputeFRS(uint32_t dwConfig)
{
    uint8_t bFrs;

    switch (dwConfig & UART_DATA_BITS)
    {
        case UART_DATA_BITS_7:
            bFrs = UART_FRS_CHAR_WLEN7_BIT;
            break;
        case UART_DATA_BITS_9:
            bFrs = UART_FRS_CHAR_WLEN9_BIT;
            break;
        default:
        case UART_DATA_BITS_8:
            bFrs = UART_FRS_CHAR_WLEN8_BIT;
            break;
    }

    if (dwConfig & UART_STOP_BITS_2)
        bFrs |= UART_FRS_STP_2BIT_BIT;

    //if (dwConfig & UART_XFER_BIT9)
    //    bFrs |= UART_FRS_XB9_1_MASK;

    if (bFrs & UART_PARITY_ENABLE)
        bFrs |= UART_FRS_PEN_ENABLE_MASK;

    if (dwConfig & UART_PARITY_SPACE)
        bFrs |= UART_FRS_PSEL_SPAR_MASK;
    else if (dwConfig & UART_PARITY_MARK)
        bFrs |= UART_FRS_PSEL_MPAR_MASK;
    else if (dwConfig & UART_PARITY_EVEN)
        bFrs |= UART_FRS_PSEL_EPAR_MASK;
    else
        bFrs |= UART_FRS_PSEL_OPAR_MASK;

    return bFrs;
}

static int cc6801_UartBaudrateSet(T_UartPort *pUartPort)
{
    T_cc6801UartPort port = g_tUartPort[pUartPort->bPortNum];
    U_regUARTCTRL *pUartCtrlBase = port.pCtrlReg;
    uint32_t dwBaud;

    dwBaud = pUartPort->dwConfig & UART_BAUDRATE;

    switch (dwBaud)
    {
      case UART_BAUDRATE_1200:
        pUartCtrlBase->dw.baud = 0x40;
        pUartCtrlBase->dw.psr = ((0x3 << 3) | 0x3);
        pUartCtrlBase->dw.ovr = 0x0;
        break;
      case UART_BAUDRATE_1800:
        pUartCtrlBase->dw.baud = 0x40;
        pUartCtrlBase->dw.psr = ((0x1 << 3) | 0x3);
        pUartCtrlBase->dw.ovr = 0x0;
        break;
      case UART_BAUDRATE_4800:
        pUartCtrlBase->dw.baud = 0xA0;
        pUartCtrlBase->dw.psr = ((0x1 << 3) | 0x1);
        pUartCtrlBase->dw.ovr = 0x0;
        break;
      case UART_BAUDRATE_9600:
        //FPGA DEMO setting, clk=16M
        pUartCtrlBase->dw.baud = 0x53;
        pUartCtrlBase->dw.psr = ((0x4 << 3) | 0x0);
        pUartCtrlBase->dw.ovr = 0x0;
        break;
//      case UART_BAUDRATE_14400:
//        break;
      case UART_BAUDRATE_19200:
        pUartCtrlBase->dw.baud = 0xA7;
        pUartCtrlBase->dw.psr = ((0x1 << 3) | 0x0);
        pUartCtrlBase->dw.ovr = 0x0A;
        break;
      case UART_BAUDRATE_38400:
        pUartCtrlBase->dw.baud = 0x53;
        pUartCtrlBase->dw.psr = ((0x1 << 3) | 0x0);
        pUartCtrlBase->dw.ovr = 0x0A;
        break;
      case UART_BAUDRATE_230400:
        pUartCtrlBase->dw.baud = 0x0A;
        pUartCtrlBase->dw.psr = ((0xC << 3) | 0x0);
        pUartCtrlBase->dw.ovr = 0x07;
        break;
      case UART_BAUDRATE_460800:
        pUartCtrlBase->dw.baud = 0x14;
        pUartCtrlBase->dw.psr = ((0xC << 3) | 0x0);
        pUartCtrlBase->dw.ovr = 0x07;
        break;
      case UART_BAUDRATE_921600:
        pUartCtrlBase->dw.baud = 0x28;
        pUartCtrlBase->dw.psr = ((0xC << 3) | 0x0);
        pUartCtrlBase->dw.ovr = 0x07;
        break;
      case UART_BAUDRATE_57600:
        pUartCtrlBase->dw.baud = 0x2;
        pUartCtrlBase->dw.psr = ((0xC << 3) | 0x0);
        pUartCtrlBase->dw.ovr = 0x07;
        break;
      default:
      case UART_BAUDRATE_115200:
        //FPGA DEMO setting, clk=16M
        pUartCtrlBase->dw.baud = 0x05;    //based on 32MHz source
        //pUartCtrlBase->dw.baud = 0x02;  //16MHz   //remove it after dynamically change clock enabled
        //pUartCtrlBase->dw.baud = 0x0B;  //64MHz   //remove it after dynamically change clock enabled
        //pUartCtrlBase->dw.baud = 0x08;  //48MHz   //remove it after dynamically change clock enabled

        pUartCtrlBase->dw.psr = ((0xC << 3) | 0x0);
        pUartCtrlBase->dw.ovr = 0x07;
        break;
    }

    return CC_SUCCESS;
}

void cc6801_UartConfigSet(T_UartPort *pUartPort)
{
    T_cc6801UartPort port = g_tUartPort[pUartPort->bPortNum];
    U_regUARTCTRL *pUartCtrlBase = port.pCtrlReg;
    uint8_t bFrs = 0, bMdsl = 0, bIntCtrl = 0;

    bFrs = cc6801_ComputeFRS(pUartPort->dwConfig);
    cc6801_UartBaudrateSet(pUartPort);

    if (pUartPort->dwConfig & UART_RTSCTS)
    {
        bIntCtrl |= UART_CTRL_INT_EFCI_ENABLE_MASK;
        bMdsl |= UART_MDSL_FCE_ENABLE_MASK;
    }

    bIntCtrl |= (UART_CTRL_INT_ETI_ENABLE_MASK |
                 UART_CTRL_INT_EEI_ENABLE_MASK |
                 UART_CTRL_INT_ERI_ENABLE_MASK);

    bMdsl |= (UART_MDSL_RTS_ENABLE_MASK |
              UART_MDSL_ETD_ENABLE_MASK |
              UART_MDSL_ERD_ENABLE_MASK);

    pUartCtrlBase->dw.ictrl = bIntCtrl;
    pUartCtrlBase->dw.frs = bFrs;
    pUartCtrlBase->dw.mdsl = bMdsl;

}

void cc6801_UartRxDoneRegister(uint8_t bIdx, fpUartRxDone RxCallBack)
{
    if (RxCallBack)
        g_tUartPort[bIdx].fbRxDoneHandler = RxCallBack;
}

int cc6801_UartInit(T_UartPort *pUartPort)
{
    uint8_t bIdx = 0;
    int error = CC_SUCCESS;

    bIdx = pUartPort->bPortNum;

    if (UART_0 == bIdx)
    {
        g_tUartPort[bIdx].pDmaReg = (void *)regUART0DMA,
        g_tUartPort[bIdx].pCtrlReg = (void *)regUART0CTRL,
        g_tUartPort[bIdx].fpUartXfer = cc6801_Uart0Xfer;
        g_tUartPort[bIdx].fpUartRcvr = cc6801_Uart0Rcvr;
    }
    else if(UART_1 == bIdx)
    {
        g_tUartPort[bIdx].pDmaReg = (void *)regUART1DMA,
        g_tUartPort[bIdx].pCtrlReg = (void *)regUART1CTRL,
        g_tUartPort[bIdx].fpUartXfer = cc6801_Uart1Xfer;
        g_tUartPort[bIdx].fpUartRcvr = cc6801_Uart1Rcvr;
    }
    else if(UART_2 == bIdx)
    {
        g_tUartPort[bIdx].pDmaReg = (void *)regUART2DMA,
        g_tUartPort[bIdx].pCtrlReg = (void *)regUART2CTRL,
        g_tUartPort[bIdx].fpUartXfer = cc6801_Uart2Xfer;
        g_tUartPort[bIdx].fpUartRcvr = cc6801_Uart2Rcvr;
    }
    else
    {
    }

    cc6801_UartConfigSet(pUartPort);

    cc6801_UartTxIntEnable(g_tUartPort[bIdx].pDmaReg, 1);
    cc6801_UartRxIntEnable(g_tUartPort[bIdx].pDmaReg, 1);

    return error;
}

int cc6801_UartRxDMA(uint8_t bPortNum, uint8_t *pData, uint8_t bLen)
{
    T_cc6801UartPort *ptPort = &g_tUartPort[bPortNum];
    U_regUARTDMA *pUartDmaBase = (U_regUARTDMA *)g_tUartPort[bPortNum].pDmaReg;
    uint32_t dwRxBufAddr = (uint32_t)pData;

    ptPort->pRxBuffer     = pData;
    ptPort->dwRxBufferLen = bLen;
    ptPort->dwRxCount     = 0;

    if (dwRxBufAddr & 0x3UL)
      return CC_ERROR_INVALID_ADDR;

    cc6801_UartRxBufferSet(pUartDmaBase, dwRxBufAddr, bLen);
    ptPort->dwRxCount = bLen;

    return ptPort->fpUartRcvr(pUartDmaBase);
}

int cc6801_UartTxDMA(uint8_t bPortNum,
                      uint8_t const * const pData, uint8_t bLen)
{
    T_cc6801UartPort *ptPort = &g_tUartPort[bPortNum];
    U_regUARTDMA *pUartDmaBase = (U_regUARTDMA *)g_tUartPort[bPortNum].pDmaReg;
    uint32_t dwTxBufAddr = (uint32_t)pData;

    if (dwTxBufAddr & 0x3UL)
      return CC_ERROR_INVALID_ADDR;

    cc6801_UartTxBufferSet(pUartDmaBase, dwTxBufAddr, bLen);

    return ptPort->fpUartXfer(pUartDmaBase);
}

int cc6801_UartRx(uint8_t bPortNum, uint8_t *pData, uint8_t bLen)
{
    T_cc6801UartPort *ptPort = &g_tUartPort[bPortNum];
    U_regUARTCTRL *pUartCtrlBase = (U_regUARTCTRL *)g_tUartPort[bPortNum].pCtrlReg;

    do {
        *(ptPort->pRxBuffer + ptPort->dwRxCount) = (uint8_t)pUartCtrlBase->dw.bufRx;
        ptPort->dwRxCount++;
    } while (ptPort->dwRxBufferLen > ptPort->dwRxCount);

    cc6801_RxDone(bPortNum, ptPort->dwRxCount);

    return CC_SUCCESS;
}

int cc6801_UartTx(uint8_t bPortNum,
                   uint8_t const * const pData, uint8_t bLen)
{
    uint8_t const *pTxBuf = pData;
    U_regUARTCTRL *pUartCtrlBase = (U_regUARTCTRL *)g_tUartPort[bPortNum].pCtrlReg;

    while (*pTxBuf)
    {
        pUartCtrlBase->dw.bufTx = *pTxBuf++;
        while(!pUartCtrlBase->bf.untbe);
    }

    return CC_SUCCESS;
}
