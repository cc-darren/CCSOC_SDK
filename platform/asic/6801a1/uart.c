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

#include "clock.h"
#include "uart.h"
#include "drvi_uart.h"

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

static int cc6801_Uart0Xfer(void const * const pReg);
static int cc6801_Uart1Xfer(void const * const pReg);
static int cc6801_Uart2Xfer(void const * const pReg);
static int cc6801_Uart0Rcvr(void const * const pReg);
static int cc6801_Uart1Rcvr(void const * const pReg);
static int cc6801_Uart2Rcvr(void const * const pReg);

volatile uint32_t UART0_RXDM_INTR = 0;
volatile uint32_t UART0_TXDM_INTR = 0;
volatile uint32_t UART1_RXDM_INTR = 0;
volatile uint32_t UART1_TXDM_INTR = 0;
volatile uint32_t UART2_RXDM_INTR = 0;
volatile uint32_t UART2_TXDM_INTR = 0;

extern E_ClockSupported g_dwCurrentClock;
E_ClockSupported g_dwUartClock;

T_cc6801UartPort g_tUartPort[UART_TOTAL_SUPPORTED] =
{
    {(void *)regUART0DMA, (void *)regUART0CTRL, cc6801_Uart0Xfer, cc6801_Uart0Rcvr},
    {(void *)regUART1DMA, (void *)regUART1CTRL, cc6801_Uart1Xfer, cc6801_Uart1Rcvr},
    {(void *)regUART2DMA, (void *)regUART2CTRL, cc6801_Uart2Xfer, cc6801_Uart2Rcvr},
};

struct S_BaudrateTable
{
    uint8_t  bBaudRate;
    uint8_t  bOVR;
    uint8_t  bDIV;
    uint8_t  bPSR;
};

struct S_BaudrateTable sBrTable16M[] =
{
    {UART_BAUDRATE_1200,   0x0C, 0xC9, 0x0A},
    {UART_BAUDRATE_2400,   0x0B, 0x64, 0x0B},
    {UART_BAUDRATE_4800,   0x0B, 0xC9, 0x02},
    {UART_BAUDRATE_9600,   0x0E, 0x21, 0x06},
    {UART_BAUDRATE_14400,  0x0F, 0x24, 0x03},
    {UART_BAUDRATE_19200,  0x07, 0x21, 0x06},
    {UART_BAUDRATE_38400,  0x00, 0x19, 0x01},
    {UART_BAUDRATE_57600,  0x0A, 0x1B, 0x01},
    {UART_BAUDRATE_115200, 0x0A, 0x0D, 0x01},
    {UART_BAUDRATE_230400, 0x0A, 0x01, 0x06},
    {UART_BAUDRATE_460800, 0x07, 0x01, 0x04},
    {UART_BAUDRATE_921600, 0x09, 0x01, 0x01},
};

struct S_BaudrateTable sBrTable24M[] =
{
    {UART_BAUDRATE_1200,   0x00, 0x7C, 0x13},
    {UART_BAUDRATE_2400,   0x00, 0x7C, 0x09},
    {UART_BAUDRATE_4800,   0x00, 0x7D, 0x04},
    {UART_BAUDRATE_9600,   0x0A, 0xF9, 0x01},
    {UART_BAUDRATE_14400,  0x0B, 0x64, 0x02},
    {UART_BAUDRATE_19200,  0x0A, 0x7D, 0x01},
    {UART_BAUDRATE_38400,  0x0A, 0x18, 0x04},
    {UART_BAUDRATE_57600,  0x07, 0x10, 0x06},
    {UART_BAUDRATE_115200, 0x0D, 0x0F, 0x01},
    {UART_BAUDRATE_230400, 0x0D, 0x07, 0x01},
    {UART_BAUDRATE_460800, 0x0D, 0x03, 0x01},
    {UART_BAUDRATE_921600, 0x0D, 0x01, 0x01},
};

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
    uint32_t dwDmaWrEn = pUartBase->dw.dmaRxEn;

    pUartBase->dw.dmaRxEn = ((dwDmaWrEn & ~UART_DMA_ENABLE_BIT) |
                                 (bEnable & UART_DMA_ENABLE_BIT));
}


static void cc6801_UartTxDmaEnable(U_regUARTDMA *pUartBase, uint8_t bEnable)
{
    uint32_t dwDmaRdEn = pUartBase->dw.dmaTxEn;

    pUartBase->dw.dmaTxEn = ((dwDmaRdEn & ~UART_DMA_ENABLE_BIT) |
                                 (bEnable & UART_DMA_ENABLE_BIT));
}

static void cc6801_UartRxIntEnable(U_regUARTDMA *pUartBase, uint8_t bEnable)
{
    uint32_t dwInt = pUartBase->dw.interrupt;

    pUartBase->dw.interrupt = ((dwInt & ~UART_INT_RX_DONE_ENABLE_MASK) |
                                (bEnable & UART_INT_RX_DONE_ENABLE_BIT));
}

static void cc6801_UartTxIntEnable(U_regUARTDMA *pUartBase, uint8_t bEnable)
{
    uint32_t dwInt = pUartBase->dw.interrupt;

    pUartBase->dw.interrupt = ((dwInt & ~UART_INT_TX_DONE_ENABLE_MASK) |
                             ((bEnable & UART_INT_TX_DONE_ENABLE_BIT) << 1));
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
    uint32_t dwTxEndAddr = 0;
    uint32_t dwByteNum = pUartBase->dw.dmaByteNum;

    dwByteNum &= UART_DMA_RX_BYTENUM_MASK;
    dwByteNum |= ((bLen - 1) << UART_DMA_TX_BYTENUM_SHIFT);
    pUartBase->dw.dmaByteNum = dwByteNum;
    //DMA tx start address
    pUartBase->dw.dmaTxStart = (dwTxBufAddr & UART_DMA_START_ADDR_MASK);
    //DMA tx end address
    dwTxEndAddr = dwTxBufAddr + (((bLen - 1) >> 2) << 2);
    pUartBase->dw.dmaTxEnd = (dwTxEndAddr & UART_DMA_END_ADDR_MASK);
}

static void cc6801_UartRxBufferSet(U_regUARTDMA *pUartBase,
                                    uint32_t dwRxBufAddr, uint8_t bLen)
{
    uint32_t dwRxEndAddr = 0;
    uint32_t dwByteNum = pUartBase->dw.dmaByteNum;

    dwByteNum &= UART_DMA_TX_BYTENUM_MASK;
    dwByteNum |= bLen - 1;
    pUartBase->dw.dmaByteNum = dwByteNum;
    //DMA rx start address
    pUartBase->dw.dmaRxStart = (dwRxBufAddr & UART_DMA_START_ADDR_MASK);
    //DMA rx end address
    dwRxEndAddr = dwRxBufAddr + (((bLen - 1) >> 2) << 2);
    pUartBase->dw.dmaRxEnd = (dwRxEndAddr & UART_DMA_END_ADDR_MASK);
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
    U_regUARTCTRL *pUartCtrlBase;
    uint32_t dwBaud;

    struct S_BaudrateTable *psBrTable;
    int iTableSize = 0;
    int iIdx = 0;

    pUartCtrlBase = (U_regUARTCTRL *)g_tUartPort[pUartPort->bPortNum].pCtrlReg;

    if (g_dwUartClock == CLOCK_16)
    {
        psBrTable = sBrTable16M;
        iTableSize = ARRAY_SIZE(sBrTable16M);
    }
    else if(g_dwUartClock == CLOCK_24)
    {
        psBrTable = sBrTable24M;
        iTableSize = ARRAY_SIZE(sBrTable24M);
    }
    else
        return CC_ERROR_INVALID_PARAM;

    dwBaud = pUartPort->dwConfig & UART_BAUDRATE;

    for (iIdx = 0; iIdx < iTableSize; iIdx++)
    {
        if (psBrTable[iIdx].bBaudRate == dwBaud)
        {
            pUartCtrlBase->dw.ovr  = psBrTable[iIdx].bOVR;
            pUartCtrlBase->dw.baud = psBrTable[iIdx].bDIV;
            pUartCtrlBase->dw.psr  = (psBrTable[iIdx].bPSR << 3);
            return CC_SUCCESS;
        }
    }

    pUartCtrlBase->dw.ovr  = 0;
    pUartCtrlBase->dw.baud = 0;
    pUartCtrlBase->dw.psr  = 0;

    return CC_ERROR_INVALID_PARAM;
}

int cc6801_UartConfigSet(T_UartPort *pUartPort)
{
    T_cc6801UartPort port = g_tUartPort[pUartPort->bPortNum];
    U_regUARTCTRL *pUartCtrlBase = port.pCtrlReg;
    uint8_t bFrs = 0, bMdsl = 0, bIntCtrl = 0;
    uint8_t bIdx = 0;
    int result = 0;

    result = cc6801_UartBaudrateSet(pUartPort);
    if (CC_SUCCESS != result)
        return result;

    bFrs = cc6801_ComputeFRS(pUartPort->dwConfig);

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

    bIdx = pUartPort->bPortNum;
    cc6801_UartTxIntEnable(g_tUartPort[bIdx].pDmaReg, 1);
    cc6801_UartRxIntEnable(g_tUartPort[bIdx].pDmaReg, 1);

    return CC_SUCCESS;
}

void cc6801_UartRxDoneRegister(uint8_t bIdx, fpUartRxDone RxCallBack)
{
    if (RxCallBack)
        g_tUartPort[bIdx].fbRxDoneHandler = RxCallBack;
}

static void cc6801_UartClkDivSet(void)
{
    uint32_t dwClkCfg = 0;
    uint8_t bClkDiv = 0;

    switch (g_dwCurrentClock)
    {
        case CLOCK_16:
            bClkDiv = 1;
            g_dwUartClock = CLOCK_16;
            break;
        case CLOCK_24:
            bClkDiv = 1;
            g_dwUartClock = CLOCK_24;
            break;
        case CLOCK_32:
            bClkDiv = 2;
            g_dwUartClock = CLOCK_16;
            break;
        case CLOCK_48:
            bClkDiv = 2;
            g_dwUartClock = CLOCK_24;
            break;
        case CLOCK_64:
            bClkDiv = 4;
            g_dwUartClock = CLOCK_16;
            break;
        default:
            bClkDiv = 1;
            g_dwUartClock = CLOCK_24;
            break;
    }

    dwClkCfg = regCKGEN->dw.clkCfg3;
    dwClkCfg &= ~(0x1F);
    dwClkCfg |= bClkDiv;
    regCKGEN->dw.clkCfg3 = dwClkCfg;
}

int cc6801_UartInit(void)
{
    int error = CC_SUCCESS;

    cc6801_UartClkDivSet();

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
        while(!(pUartCtrlBase->dw.ictrl & 0x01));
    }

    return CC_SUCCESS;
}
