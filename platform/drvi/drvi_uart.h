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
#include "uart.h"

#define drvi_UartInit(port)                          cc6801_UartInit(port)
#define drvi_UartTx(port,buf,len)                    cc6801_UartTx(port,buf,len)
#define drvi_UartRx(port,buf,len)                    cc6801_UartRx(port,buf,len)

#define DRVI_UART_BAUD         0x0001000F
#define  DRVI_UART_B0          0x00000000
#define  DRVI_UART_B50         0x00000001
#define  DRVI_UART_B75         0x00000002
#define  DRVI_UART_B110        0x00000003
#define  DRVI_UART_B134        0x00000004
#define  DRVI_UART_B150        0x00000005
#define  DRVI_UART_B200        0x00000006
#define  DRVI_UART_B300        0x00000007
#define  DRVI_UART_B600        0x00000008
#define  DRVI_UART_B1200       0x00000009
#define  DRVI_UART_B1800       0x0000000A
#define  DRVI_UART_B2400       0x0000000B
#define  DRVI_UART_B4800       0x0000000C
#define  DRVI_UART_B9600       0x0000000D
#define  DRVI_UART_B19200      0x0000000E
#define  DRVI_UART_B38400      0x0000000F
#define  DRVI_UART_B57600      0x00010000
#define  DRVI_UART_B115200     0x00010001
#define  DRVI_UART_B230400     0x00010002
#define  DRVI_UART_B460800     0x00010003
#define  DRVI_UART_B500000     0x00010004
#define  DRVI_UART_B576000     0x00010005
#define  DRVI_UART_B921600     0x00010006
#define  DRVI_UART_B1000000    0x00010007
#define  DRVI_UART_B1152000    0x00010008
#define  DRVI_UART_B1500000    0x00010009
#define  DRVI_UART_B2000000    0x0001000A
#define  DRVI_UART_B2500000    0x0001000B
#define  DRVI_UART_B3000000    0x0001000C
#define  DRVI_UART_B3500000    0x0001000D
#define  DRVI_UART_B4000000    0x0001000E
#define DRVI_UART_SIZE         0x00000070
#define   DRVI_UART_S5         0x00000010
#define   DRVI_UART_S6         0x00000020
#define   DRVI_UART_S7         0x00000030
#define   DRVI_UART_S8         0x00000040
#define   DRVI_UART_S9         0x00000050
#define DRVI_UART_STOPB        0x00000100
#define DRVI_UART_READ         0x00000200
#define DRVI_UART_XB9          0x00000400
#define DRVI_UART_PARENB       0x00001000
#define DRVI_UART_PAREVEN      0x00002000
#define DRVI_UART_MPAR         0x00004000  /* mark parity */
#define DRVI_UART_SPAR         0x00008000  /* space parity */
#define DRVI_UART_RTSCTS       0x01000000  /* flow control */

typedef enum
{
    DRVI_UART_EVENT_TX_DONE,
    DRVI_UART_EVENT_RX_DONE,
    DRVI_UART_EVENT_ERROR,
} E_DrviUartEvent;

typedef struct
{
    E_DrviUartEvent type;
    uint8_t *pRxBuf;
    uint8_t bRxBytes;
} T_UartEvent;

typedef struct S_UartPort
{
    uint8_t bPortNum;
    uint32_t dwConfig;
    void (*fpComplete)(T_UartEvent *p_event);
} T_UartPort;









#endif //_DRVI_UART_H_


