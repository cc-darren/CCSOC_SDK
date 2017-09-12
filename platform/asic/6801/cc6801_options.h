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
#define _IF_NULL_   0xFF

#define _Interface_SPI0_    0x10
#define _Interface_SPI1_    0x11
#define _Interface_SPI2_    0x12
#define _Interface_I2C0_    0x20
#define _Interface_I2C1_    0x21
#define _Interface_UART0_   0x30
#define _Interface_UART1_   0x31
#define _Interface_UART2_   0x32


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

#define _SPI_CPHA_              0x01            /* clock phase */
#define _SPI_CPOL_              0x02            /* clock polarity */
#define _SPI_MODE_0_            (0|0)
#define _SPI_MODE_1_            (0|_SPI_CPHA_)
#define _SPI_MODE_2_            (_SPI_CPOL_|0)
#define _SPI_MODE_3_            (_SPI_CPOL_|_SPI_CPHA_)
#define _SPI_CS_HIGH_           0x04            /* chipselect active high? */
#define _SPI_LSB_FIRST_         0x08
#define _SPI_3WIRE_             0x10            /* SI/SO signals shared */


// I2C Configuration List

#define _I2C_FREQ_100K_         100000
#define _I2C_FREQ_200K_         200000
#define _I2C_FREQ_400K_         400000

#endif //_CC6801_OPTIONS_H_










