/******************************************************************************
*  Copyright 2017, Cloudchip, Inc.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Cloudchip, Inc. (C) 2017
******************************************************************************/

/******************************************************************************
*  Filename:
*  ---------
*  cc6801_options.h
*
*  Project:
*  --------
*  All
*
*  Description:
*  ------------
*  project options of cc6801
*
*  Author:
*  -------
*  PPP   (CC-pattaya)
*
*===========================================================================
*
******************************************************************************/
#ifndef _CC6801_OPTIONS_H_
#define _CC6801_OPTIONS_H_



// Interface List

#define _IF_SPI_    0x10
#define _IF_I2C_    0x20
#define _IF_UART_   0x30
#define _IF_WKT_    0x40
#define _IF_PWM_    0x50
#define _IF_NULL_   0xFF

#define _Interface_SPI0_    0x10
#define _Interface_SPI1_    0x11
#define _Interface_SPI2_    0x12
#define _Interface_I2C0_    0x20
#define _Interface_I2C1_    0x21
#define _Interface_UART0_   0x30
#define _Interface_UART1_   0x31
#define _Interface_UART2_   0x32
#define _Interface_WKT0_    0x40
#define _Interface_WKT1_    0x41
#define _Interface_PWM0_    0x50
#define _Interface_PWM1_    0x51

// GPIO Configuration List

#define GPIO_MODE_GPIO          0
#define GPIO_MODE_SPI_4WIRE     1
#define GPIO_MODE_I2S_MASTER    2
#define GPIO_MODE_I2S_SLAVE     3
#define GPIO_MODE_UART          4
#define GPIO_MODE_DMIC          5
//#define GPIO_MODE_DMIC        6        //Not Support
#define GPIO_MODE_I2C_GPIO      7
#define GPIO_MODE_GPIO_I2C      8
#define GPIO_MODE_PWM_GPIO      9
#define GPIO_MODE_GPIO_PWM      10
#define GPIO_MODE_SPI_3WIRE     11

#define GPIO_PULL_DOWN          0x0001
#define GPIO_PULL_UP            0x0002

#define GPIO_PINMUX_DISABLE     0x0004
#define GPIO_PINMUX_ENABLE      0x0008

#define GPIO_DIR_INPUT          0x0010
#define GPIO_DIR_OUTPUT         0x0020

#define GPIO_INPUT_NOPULL       0x0100
#define GPIO_OUTPUT_LOW         0x0200
#define GPIO_OUTPUT_HIGH        0x0400


// SPI Configuration List

#define SPI_CPHA                 0x01            /* clock phase */
#define SPI_CPOL                 0x02            /* clock polarity */
#define SPI_MODE_0               (0|0)
#define SPI_MODE_1               (0|SPI_CPHA)
#define SPI_MODE_2               (SPI_CPOL|0)
#define SPI_MODE_3               (SPI_CPOL|SPI_CPHA)
#define SPI_CS_HIGH              0x04            /* chipselect active high? */
#define SPI_LSB_FIRST            0x08
#define SPI_3WIRE                0x10            /* SI/SO signals shared */


// UART Configuration List

#define UART_BAUDRATE            0x0000000F
#define  UART_BAUDRATE_1200       0x00000001
#define  UART_BAUDRATE_1800       0x00000002
#define  UART_BAUDRATE_2400       0x00000003
#define  UART_BAUDRATE_4800       0x00000004
#define  UART_BAUDRATE_9600       0x00000005
#define  UART_BAUDRATE_14400      0x00000006
#define  UART_BAUDRATE_19200      0x00000007
#define  UART_BAUDRATE_38400      0x00000008
#define  UART_BAUDRATE_57600      0x00000009
#define  UART_BAUDRATE_115200     0x0000000A
#define  UART_BAUDRATE_230400     0x0000000B
#define  UART_BAUDRATE_460800     0x0000000C
#define  UART_BAUDRATE_500000     0x0000000D
#define  UART_BAUDRATE_576000     0x0000000E
#define  UART_BAUDRATE_921600     0x0000000F

#define UART_DATA_BITS           0x00000030
#define  UART_DATA_BITS_7         0x00000010
#define  UART_DATA_BITS_8         0x00000020
#define  UART_DATA_BITS_9         0x00000030

#define UART_STOP_BITS_2         0x00080000

#define UART_PARITY_ENABLE       0x00100000
#define  UART_PARITY_ODD         0x00200000
#define  UART_PARITY_EVEN        0x00400000
#define  UART_PARITY_MARK        0x00800000  /* mark parity */
#define  UART_PARITY_SPACE       0x01000000  /* space parity */

#define UART_RTSCTS              0x02000000  /* flow control */


// I2C Configuration List

#define I2C_FREQ_100K           100000
#define I2C_FREQ_200K           200000
#define I2C_FREQ_400K           400000

#endif //_CC6801_OPTIONS_H_










