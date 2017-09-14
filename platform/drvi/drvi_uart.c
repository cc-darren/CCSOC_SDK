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

#if defined _UART_INUSE_ && (_UART_INUSE_)
int drvi_UartInit(void)
{
    T_UartPort tUartPort;
    int iResult = 0;

    #if defined _UART0_INUSE_ && (_UART0_INUSE_)
    tUartPort.bPortNum = 0;
    tUartPort.dwConfig = UART0_CONFIG;
    cc6801_UartInit(&tUartPort);
    #endif
    #if defined _UART1_INUSE_ && (_UART1_INUSE_)
    tUartPort.bPortNum = 1;
    tUartPort.dwConfig = UART1_CONFIG;
    cc6801_UartInit(&tUartPort);
    #endif
    #if defined _UART2_INUSE_ && (_UART2_INUSE_)
    tUartPort.bPortNum = 2;
    tUartPort.dwConfig = UART2_CONFIG;
    cc6801_UartInit(&tUartPort);
    #endif

    return iResult;
}
#endif
