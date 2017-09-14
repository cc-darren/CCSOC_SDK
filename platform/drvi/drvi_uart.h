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

#define drvi_UartConfigSet(port)                     cc6801_UartConfigSet(port)
#define drvi_UartRxDoneRegister(port,func)           cc6801_UartRxDoneRegister(port,func)
#if (UART_USE_DMA) && UART_USE_DMA
#define drvi_UartTx(port,buf,len)                    cc6801_UartTxDMA(port,buf,len)
#define drvi_UartRx(port,buf,len)                    cc6801_UartRxDMA(port,buf,len)
#else
#define drvi_UartTx(port,buf,len)                    cc6801_UartTx(port,buf,len)
#define drvi_UartRx(port,buf,len)                    cc6801_UartRx(port,buf,len)
#endif

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






#endif //_DRVI_UART_H_


