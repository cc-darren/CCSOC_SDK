/**
 ****************************************************************************************
 *
 * @file drvi_uart.h
 *
 * @brief Head file of drvi_uart.c
 *
 * Copyright (C) CloudChip 2017-2019
 *
 *
 ****************************************************************************************
 */
#ifndef _DRVI_UART_H_
#define _DRVI_UART_H_
#include "global.h"
#include "project.h"
#include "uart.h"

typedef enum
{
    DRVI_UART_EVENT_TX_DONE,
    DRVI_UART_EVENT_RX_DONE,
    DRVI_UART_EVENT_ERROR,
} E_DrviUartEvent;

typedef struct S_UartEvent
{
    E_DrviUartEvent type;
    uint8_t *pRxBuf;
    uint8_t bRxBytes;
} T_UartEvent;

typedef struct S_UartPort
{
    uint8_t bPortNum;
    uint32_t dwConfig;
} T_UartPort;

typedef void (*fpUartRxDone)(T_UartEvent *p_event);

int drvi_UartInit(void);

__forceinline void drvi_UartConfigSet(T_UartPort *pUartPort)
{
    cc6801_UartConfigSet(pUartPort);
}

__forceinline void drvi_UartRxDoneRegister(uint8_t bIdx, fpUartRxDone RxCallBack)
{
    cc6801_UartRxDoneRegister(bIdx, RxCallBack);
}

#if (UART_USE_DMA) && UART_USE_DMA
__forceinline int drvi_UartTx(uint8_t bPortNum,
                              uint8_t const * const pData,
                              uint8_t bLen)
{
    return cc6801_UartTxDMA(bPortNum, pData, bLen);
}

__forceinline int drvi_UartRx(uint8_t bPortNum, uint8_t *pData, uint8_t bLen)
{
    return cc6801_UartRxDMA(bPortNum, pData, bLen);
}
#else
__forceinline int drvi_UartTx(uint8_t bPortNum,
                              uint8_t const * const pData,
                              uint8_t bLen)
{
    return cc6801_UartTx(bPortNum, pData, bLen);
}

__forceinline int drvi_UartRx(uint8_t bPortNum, uint8_t *pData, uint8_t bLen)
{
    return cc6801_UartRx(bPortNum, pData, bLen);
}
#endif


#endif //_DRVI_UART_H_


