/**
 ****************************************************************************************
 *
 * @file drvi_uart.c
 *
 * @brief HW UART interface for all SOC.
 *
 * Copyright (C) CloudChip 2017-2019
 *
 *
 ****************************************************************************************
 */
#include "drvi_uart.h"

#if defined UART_INUSE && (UART_INUSE)
int drvi_UartInit(void)
{
    T_UartPort tUartPort;
    int iResult = 0;

    #if defined UART0_INUSE && (UART0_INUSE)
    tUartPort.bPortNum = 0;
    tUartPort.dwConfig = UART0_CONFIG;
    cc6801_UartInit(&tUartPort);
    #endif
    #if defined UART1_INUSE && (UART1_INUSE)
    tUartPort.bPortNum = 1;
    tUartPort.dwConfig = UART1_CONFIG;
    cc6801_UartInit(&tUartPort);
    #endif
    #if defined UART2_INUSE && (UART2_INUSE)
    tUartPort.bPortNum = 2;
    tUartPort.dwConfig = UART2_CONFIG;
    cc6801_UartInit(&tUartPort);
    #endif

    return iResult;
}
#endif
