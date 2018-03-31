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
#include <string.h>
#include "drvi_uart.h"

#if defined UART_INUSE && (UART_INUSE)

T_UartPort g_tDrviUartPort[UART_TOTAL_SUPPORTED];

#ifdef CC6801A1
#include "uart_ring.h"

#ifndef UART_BUFFER_SIZE
#define UART_BUFFER_SIZE 64
#endif

#if defined UART0_INUSE && (UART0_INUSE)
RINGBUFF_T sUart0Ring;
static volatile uint32_t dwWrLastPos;

__align(4) static uint8_t g_baUart0RxBuffer[UART_BUFFER_SIZE] = {0};
#endif

static uint16_t g_CurrRxByteNum = 0;

int drvi_UartRx(uint8_t bPortNum, uint8_t bLen)
{
    return cc6801_UartRxDMA(bPortNum, bLen);
}

static void drvi_UartRxCallback(T_cc6801UartEvent *pEvent)
{
    T_UartEvent *ptEvent;
    uint8_t bPort;

    int iSize = 0;
    int iRingCount = 0;
    uint32_t dwWrPos = 0;

    bPort = pEvent->bPort;
    ptEvent = &g_tDrviUartPort[bPort].tUevent;
    dwWrPos = pEvent->dwDbusRxAddr;

    if(dwWrLastPos != dwWrPos)
    {
        if (dwWrPos > dwWrLastPos)
        {
            iSize = dwWrPos - dwWrLastPos;
        }
        else
        {
            iSize = ((uint32_t)&g_baUart0RxBuffer[UART_BUFFER_SIZE-1] - dwWrLastPos) + (dwWrPos - (uint32_t)&g_baUart0RxBuffer[0] + 1);
        }
        RingBuffer_InsertMult(&sUart0Ring, NULL, iSize);
        dwWrLastPos = dwWrPos;
    }

    if (ptEvent->bRxBytes)
    {
        iRingCount = RingBuffer_GetCount(&sUart0Ring);
        if (iRingCount >= ptEvent->bRxBytes)
        {
            RingBuffer_PopMult(&sUart0Ring, ptEvent->pRxBuf, ptEvent->bRxBytes);

            if (g_tDrviUartPort[bPort].fpDrviUartCb)
                g_tDrviUartPort[bPort].fpDrviUartCb(ptEvent);

            ptEvent->bRxBytes = 0;
            //g_CurrRxByteNum = 4; //a default value
            //drvi_UartRx(TEST_UART0_BUS, g_CurrRxByteNum); //reconfig uart
        }
    }
}

int drvi_UartReceive(uint8_t bPort, uint8_t *pbBuf, int iSize)
{
    int iRingCount = 0;
    T_UartPort *ptPort;
    T_UartEvent *ptEvent;

    ptPort = &g_tDrviUartPort[bPort];
    ptEvent = &g_tDrviUartPort[bPort].tUevent;

    NVIC_DisableIRQ(UART0_RXDMA_IRQn);

    do
    {
        ptEvent->pRxBuf = pbBuf;
        ptEvent->bRxBytes = iSize;

        iRingCount = RingBuffer_GetCount(&sUart0Ring);
        if (iRingCount >= iSize)
        {
            RingBuffer_PopMult(&sUart0Ring, pbBuf, iSize);

            if (ptPort->fpDrviUartCb)
                ptPort->fpDrviUartCb(ptEvent);
            //g_CurrRxByteNum = 4; //a default value
            //drvi_UartRx(bPort, g_CurrRxByteNum); //reconfig uart
            break;
        }

        if (g_CurrRxByteNum > iSize)
        {
            drvi_UartRx(bPort, iSize); //reconfig uart
            g_CurrRxByteNum = iSize;
        }
    } while(0);

    NVIC_EnableIRQ(UART0_RXDMA_IRQn);

    return iRingCount;
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
#ifdef CC6801A1
    RingBuffer_Init(&sUart0Ring, g_baUart0RxBuffer, sizeof(uint8_t), UART_BUFFER_SIZE);
    dwWrLastPos = (uint32_t)g_baUart0RxBuffer;

    cc6801_UartRxCallbackRegister(g_tDrviUartPort[0].bPortNum, (uint8_t *)g_baUart0RxBuffer, UART_BUFFER_SIZE, drvi_UartRxCallback);
    g_CurrRxByteNum = 4;
    drvi_UartRx(g_tDrviUartPort[0].bPortNum, g_CurrRxByteNum);
#endif
    #endif
    #if defined UART1_INUSE && (UART1_INUSE)
    g_tDrviUartPort[1].bPortNum = 1;
    g_tDrviUartPort[1].dwConfig = UART1_CONFIG;
    cc6801_UartConfigSet(&g_tDrviUartPort[1]);
    #endif
    #if defined UART2_INUSE && (UART2_INUSE)
    g_tDrviUartPort[2].bPortNum = 2;
    g_tDrviUartPort[2].dwConfig = UART2_CONFIG;
    cc6801_UartConfigSet(&g_tDrviUartPort[2]);
    #endif

    return iResult;
}
#endif
