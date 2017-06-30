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

#define drvi_uart_init(a,b,c)                  cc_drv_uart_init(a,b,c)
#define drvi_uart_tx(a,b,c)                    cc_drv_uart_tx(a,b,c)
#define drvi_uart_rx(a,b,c)                    cc_drv_uart_rx(a,b,c)

typedef enum
{
    DRVI_UART_BAUDRATE_1200,
    DRVI_UART_BAUDRATE_2400,
    DRVI_UART_BAUDRATE_4800,
    DRVI_UART_BAUDRATE_9600,
    DRVI_UART_BAUDRATE_19200,
    DRVI_UART_BAUDRATE_38400,
    DRVI_UART_BAUDRATE_57600,
    DRVI_UART_BAUDRATE_115200,
    DRVI_UART_BAUDRATE_230400,
    DRVI_UART_BAUDRATE_460800,
    DRVI_UART_BAUDRATE_921600,
} drvi_uart_baudrate_t;

typedef enum
{
    DRVI_UART_PARITY_ODD,
    DRVI_UART_PARITY_EVEN,
    DRVI_UART_PARITY_MARK,
    DRVI_UART_PARITY_SPACE,
    DRVI_UART_PARITY_DISABLE,
} drvi_uart_parity_t;

typedef enum
{
    DRVI_UART_8_DATA_BITS,
    DRVI_UART_7_DATA_BITS,
} drvi_uart_data_bits;

typedef enum
{
    DRVI_UART_1_STOP_BITS,
    DRVI_UART_2_STOP_BITS,
} drvi_uart_stop_bits;


typedef enum
{
    DRVI_UART_HWFC_DISABLE,
    DRVI_UART_HWFC_ENABLE,
} drvi_uart_hwfc_t;


typedef struct
{
    drvi_uart_baudrate_t baudrate;
    drvi_uart_parity_t parity;
    drvi_uart_data_bits data_bits;
    drvi_uart_stop_bits stop_bits;
    drvi_uart_hwfc_t hw_flow;
} drvi_uart_params_t;


/**@brief UART default configuration. */
#define DRVI_UART_DEFAULT_CONFIG                                               \
{                                                                              \
    .baudrate           = DRVI_UART_BAUDRATE_115200,                             \
    .parity             = DRVI_UART_PARITY_DISABLE,                            \
    .data_bits          = DRVI_UART_8_DATA_BITS,                               \
    .stop_bits          = DRVI_UART_1_STOP_BITS,                               \
    .hw_flow            = DRVI_UART_HWFC_DISABLE,                              \
}










#endif //_DRVI_UART_H_


