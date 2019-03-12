/* Copyright (c) 2019 Cloudchip, Inc. All Rights Reserved.
 *
 * The information contained herein is property of Cloudchip, Inc.
 * Terms and conditions of usage are described in detail in CLOUDCHIP
 * STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information.
 * NO WARRANTY of ANY KIND is provided. This heading must NOT be removed
 * from the file.
 */
#include <string.h>
#include "drvi_uart.h"
#include "utility.h"

#if defined UART_INUSE && (UART_INUSE)

T_UartPort g_tDrviUartPort[UART_TOTAL_SUPPORTED];

#if (defined(CC6801B0) || defined(CC6801C0))

#ifndef UART_BUFFER_SIZE
#define UART_BUFFER_SIZE 64
#endif

#if defined UART0_INUSE && (UART0_INUSE)
#if !(defined(UART_IP_MODE) && (UART_IP_MODE))
static uint8_t g_baUart0RxBuffer[UART_BUFFER_SIZE] = {0};
#endif
#endif
#if defined UART1_INUSE && (UART1_INUSE)
static uint8_t g_baUart1RxBuffer[UART_BUFFER_SIZE] = {0};
#endif
#if defined UART2_INUSE && (UART2_INUSE)
static uint8_t g_baUart2RxBuffer[UART_BUFFER_SIZE] = {0};
#endif

#define HEAD(x)                  (*(volatile uint32_t *) &(x)->dwHead)
#define TAIL(x)                  (*(volatile uint32_t *) &(x)->dwTail)
#define LAST_POS(x)              (*(volatile uint32_t *) &(x)->dwWrLastPos)

#define MASKT(x)                 ((x)->dwTail & ((x)->iRxBufSz - 1))

static __inline int DrviUringIsEmpty(T_UartRing *pUring)
{
	return HEAD(pUring) == TAIL(pUring);
}

static __inline int DrviUringGetCount(T_UartRing *pUring)
{
	return HEAD(pUring) - TAIL(pUring);
}

int DrviUringPop(T_UartRing *pUring, void *pData, int iNum)
{
	int iCnt1, iCnt2;

	if (DrviUringIsEmpty(pUring))
		return 0;

	iCnt1 = iCnt2 = DrviUringGetCount(pUring);
	if (MASKT(pUring) + iCnt1 >= pUring->iRxBufSz)
		iCnt1 = pUring->iRxBufSz - MASKT(pUring);
	iCnt2 -= iCnt1;

	iCnt1 = MIN(iCnt1, iNum);
	iNum -= iCnt1;

	/* Write segment 1 */
	memcpy(pData, (uint8_t *)(pUring->pbRxBuffer)+MASKT(pUring), iCnt1);
	pUring->dwTail += iCnt1;

    if (iNum == 0)
        return iCnt1;

	iCnt2 = MIN(iCnt2, iNum);
	iNum -= iCnt2;

	/* Write segment 2 */
	memcpy((uint8_t *)pData+iCnt1, (uint8_t *)(pUring->pbRxBuffer)+MASKT(pUring), iCnt2);
	pUring->dwTail += iCnt2;

	return iCnt1 + iCnt2;
}


int drvi_UartRx(uint8_t bPortNum, uint8_t bLen)
{
    return cc6801_UartRxDMA(bPortNum, bLen);
}

static void drvi_UartRingHandle(T_UartPort *pPort, T_cc6801UartEvent *pEvent)
{
    T_UartRing *RingBuff = &pPort->tUring;
    T_UartEvent *ptEvent;
    uint8_t bPort;

    int iSize = 0;
    int iRingCount = 0;
    uint32_t dwWrPos = 0;

    bPort = pEvent->bPort;
    ptEvent = &g_tDrviUartPort[bPort].tUevent;
    dwWrPos = pEvent->dwDbusRxAddr;

    if(LAST_POS(RingBuff) != dwWrPos)
    {
        if (dwWrPos > LAST_POS(RingBuff))
        {
            iSize = dwWrPos - LAST_POS(RingBuff);
        }
        else
        {
            iSize = (uint32_t)(pPort->tUring.pbRxBuffer+pPort->tUring.iRxBufSz-1) - LAST_POS(RingBuff);
            iSize += (dwWrPos - (uint32_t)pPort->tUring.pbRxBuffer + 1);
        }
        pPort->tUring.dwHead += iSize;
        LAST_POS(RingBuff) = dwWrPos;
    }

    if (ptEvent->bRxBytes)
    {
        iRingCount = DrviUringGetCount(&pPort->tUring);
        if (iRingCount >= ptEvent->bRxBytes)
        {
            DrviUringPop(&pPort->tUring, ptEvent->pRxBuf, ptEvent->bRxBytes);
            ptEvent->type = DRVI_UART_EVENT_RX_DONE;

            if (g_tDrviUartPort[bPort].fpDrviUartCb)
                g_tDrviUartPort[bPort].fpDrviUartCb(ptEvent);

            //ptEvent->bRxBytes = 0;
            //g_CurrRxByteNum = 4; //a default value
            //drvi_UartRx(TEST_UART0_BUS, g_CurrRxByteNum); //reconfig uart
        }
    }
}

int drvi_UartGet(T_UartPort *ptPort, uint8_t *pbBuf, int iSize)
{
    int iRingCount = 0;
    T_UartEvent *ptEvent;

    ptEvent = &g_tDrviUartPort[ptPort->bPortNum].tUevent;

    do
    {
        ptEvent->pRxBuf = pbBuf;
        ptEvent->bRxBytes = iSize;

        iRingCount = DrviUringGetCount(&ptPort->tUring);
        if (iRingCount >= iSize)
        {
            DrviUringPop(&ptPort->tUring, pbBuf, iSize);

            if (ptPort->fpDrviUartCb)
                ptPort->fpDrviUartCb(ptEvent);
            //g_CurrRxByteNum = 4; //a default value
            //drvi_UartRx(bPort, g_CurrRxByteNum); //reconfig uart
            break;
        }

        if (ptPort->tUring.wCurrRxByteNum > iSize)
        {
            drvi_UartRx(UART_0, iSize); //reconfig uart
            ptPort->tUring.wCurrRxByteNum = iSize;
        }
    } while(0);

    return iRingCount;
}

#if defined UART0_INUSE && (UART0_INUSE)
#if !(defined(UART_IP_MODE) && (UART_IP_MODE))
static void drvi_Uart0RxCallback(T_cc6801UartEvent *pEvent)
{
    drvi_UartRingHandle(&g_tDrviUartPort[0], pEvent);
}
#endif

int drvi_Uart0Receive(uint8_t *pbBuf, int iSize)
{
    int iRingCount = 0;

    NVIC_DisableIRQ(UART0_RXDMA_IRQn);

    iRingCount = drvi_UartGet(&g_tDrviUartPort[0], pbBuf, iSize);

    NVIC_EnableIRQ(UART0_RXDMA_IRQn);

    return iRingCount;
}
#endif

#if defined UART1_INUSE && (UART1_INUSE)
static void drvi_Uart1RxCallback(T_cc6801UartEvent *pEvent)
{
    drvi_UartRingHandle(&g_tDrviUartPort[1], pEvent);
}

int drvi_Uart1Receive(uint8_t *pbBuf, int iSize)
{
    int iRingCount = 0;

    NVIC_DisableIRQ(UART1_RXDMA_IRQn);

    iRingCount = drvi_UartGet(&g_tDrviUartPort[1], pbBuf, iSize);

    NVIC_EnableIRQ(UART1_RXDMA_IRQn);

    return iRingCount;
}
#endif

#if defined UART2_INUSE && (UART2_INUSE)
static void drvi_Uart2RxCallback(T_cc6801UartEvent *pEvent)
{
    drvi_UartRingHandle(&g_tDrviUartPort[2], pEvent);
}

int drvi_Uart2Receive(uint8_t *pbBuf, int iSize)
{
    int iRingCount = 0;

    NVIC_DisableIRQ(UART2_RXDMA_IRQn);

    iRingCount = drvi_UartGet(&g_tDrviUartPort[2], pbBuf, iSize);

    NVIC_EnableIRQ(UART2_RXDMA_IRQn);

    return iRingCount;
}
#endif

int drvi_UartReceive(uint8_t bPort, uint8_t *pbBuf, int iSize)
{
    if (bPort >= UART_TOTAL_SUPPORTED)
        return -1;

    return g_tDrviUartPort[bPort].fpDrviUartReceive(pbBuf, iSize);
}

void drvi_UartRxDoneRegister(uint8_t bIdx,  fpDrviUartCallback fpCb)
{
    if (fpCb)
        g_tDrviUartPort[bIdx].fpDrviUartCb = fpCb;
}
#endif

int drvi_UartInit(void)
{
    int iResult = 0;

    cc6801_UartInit();

#if defined UART0_INUSE && (UART0_INUSE)
    g_tDrviUartPort[0].bPortNum = 0;
    g_tDrviUartPort[0].dwConfig = UART0_CONFIG;
    cc6801_UartConfigSet(&g_tDrviUartPort[0]);
	#if defined(UART_IP_MODE) && (UART_IP_MODE)
	NVIC_EnableIRQ(UART0_IP_IRQn);
	#else
#if (defined(CC6801B0) || defined(CC6801C0))
    g_tDrviUartPort[0].fpDrviUartReceive = drvi_Uart0Receive;
    g_tDrviUartPort[0].tUring.pbRxBuffer = g_baUart0RxBuffer;
    g_tDrviUartPort[0].tUring.iRxBufSz = UART_BUFFER_SIZE;
    g_tDrviUartPort[0].tUring.dwHead = g_tDrviUartPort[0].tUring.dwTail = 0;
    g_tDrviUartPort[0].tUring.dwWrLastPos = (uint32_t)&g_baUart0RxBuffer[UART_BUFFER_SIZE];

    cc6801_UartRxBufferSet(g_tDrviUartPort[0].bPortNum, (uint8_t *)g_baUart0RxBuffer, UART_BUFFER_SIZE);
    cc6801_UartRxCallbackRegister(g_tDrviUartPort[0].bPortNum, drvi_Uart0RxCallback);
    drvi_UartRx(g_tDrviUartPort[0].bPortNum, g_tDrviUartPort[0].tUring.wCurrRxByteNum);
#endif
	#endif

#endif

#if defined UART1_INUSE && (UART1_INUSE)
    g_tDrviUartPort[1].bPortNum = 1;
    g_tDrviUartPort[1].dwConfig = UART1_CONFIG;
    cc6801_UartConfigSet(&g_tDrviUartPort[1]);
#if (defined(CC6801B0) || defined(CC6801C0))
    g_tDrviUartPort[1].fpDrviUartReceive = drvi_Uart1Receive;
    g_tDrviUartPort[1].tUring.pbRxBuffer = g_baUart1RxBuffer;
    g_tDrviUartPort[1].tUring.iRxBufSz = UART_BUFFER_SIZE;
    g_tDrviUartPort[1].tUring.dwHead = g_tDrviUartPort[1].tUring.dwTail = 0;
    g_tDrviUartPort[1].tUring.dwWrLastPos = (uint32_t)&g_baUart1RxBuffer[UART_BUFFER_SIZE];

    cc6801_UartRxBufferSet(g_tDrviUartPort[1].bPortNum, (uint8_t *)g_baUart1RxBuffer, UART_BUFFER_SIZE);
    cc6801_UartRxCallbackRegister(g_tDrviUartPort[1].bPortNum, drvi_Uart1RxCallback);
    drvi_UartRx(g_tDrviUartPort[1].bPortNum, g_tDrviUartPort[1].tUring.wCurrRxByteNum);
#endif
#endif

#if defined UART2_INUSE && (UART2_INUSE)
    g_tDrviUartPort[2].bPortNum = 2;
    g_tDrviUartPort[2].dwConfig = UART2_CONFIG;
    cc6801_UartConfigSet(&g_tDrviUartPort[2]);
#if (defined(CC6801B0) || defined(CC6801C0))
    g_tDrviUartPort[2].fpDrviUartReceive = drvi_Uart2Receive;
    g_tDrviUartPort[2].tUring.pbRxBuffer = g_baUart2RxBuffer;
    g_tDrviUartPort[2].tUring.iRxBufSz = UART_BUFFER_SIZE;
    g_tDrviUartPort[2].tUring.dwHead = g_tDrviUartPort[2].tUring.dwTail = 0;
    g_tDrviUartPort[2].tUring.dwWrLastPos = (uint32_t)&g_baUart2RxBuffer[UART_BUFFER_SIZE];

    cc6801_UartRxBufferSet(g_tDrviUartPort[2].bPortNum, (uint8_t *)g_baUart2RxBuffer, UART_BUFFER_SIZE);
    cc6801_UartRxCallbackRegister(g_tDrviUartPort[2].bPortNum, drvi_Uart2RxCallback);
    drvi_UartRx(g_tDrviUartPort[2].bPortNum, g_tDrviUartPort[2].tUring.wCurrRxByteNum);
#endif
#endif

    return iResult;
}
#endif
