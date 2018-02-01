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
