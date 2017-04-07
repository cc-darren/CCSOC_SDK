/******************************************************************************
*  Copyright 2017, Cloud Chip, Inc.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Cloud Chip, Inc. (C) 2017
******************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>

//#ifdef ENABLE_DEBUG_LOG_SUPPORT
#include "global.h"
#include "app_uart.h"
#include "app_trace.h"

#ifndef UART_TX_BUF_SIZE
    #define UART_TX_BUF_SIZE 1024                         /**< UART TX buffer size. */
#endif
#ifndef UART_RX_BUF_SIZE
    #define UART_RX_BUF_SIZE 1                           /**< UART RX buffer size. */
#endif

void app_trace_init(void)
{
    uint32_t err_code = SUCCESS;

    const app_uart_comm_params_t comm_params =
    {
        APP_UART_FLOW_CONTROL_DISABLED,
        false,
        APP_UART_BAUDRATE_115200
    };

    APP_UART_FIFO_INIT(&comm_params,
                       UART_RX_BUF_SIZE,
                       UART_TX_BUF_SIZE,
                       NULL,
                       err_code);
    UNUSED_VARIABLE(err_code);
}

void app_trace_dump(uint8_t * p_buffer, uint32_t len)
{
	  uint32_t index = 0;

    app_trace_log("\r\n");
    for (index = 0; index <  len; index++)
    {
        app_trace_log("0x%02X ", p_buffer[index]);
    }
    app_trace_log("\r\n");
}

//#endif // ENABLE_DEBUG_LOG_SUPPORT

