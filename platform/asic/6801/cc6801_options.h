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


// SPI Configuration List

#define _SPI_CPHA_              0x01            /* clock phase */
#define _SPI_CPOL_              0x02            /* clock polarity */
#define _SPI_MODE_0_            (0|0)
#define _SPI_MODE_1_            (0|_SPI_CPHA_)
#define _SPI_MODE_2_            (_SPI_CPOL_|0)
#define _SPI_MODE_3_            (_SPI_CPOL_|_SPI_CPHA_)
#define _SPI_CS_HIGH_           0x04            /* chipselect active high? */
#define _SPI_LSB_FIRST_         0x08            /* per-word bits-on-wire */
#define _SPI_3WIRE_             0x10            /* SI/SO signals shared */




#endif //_CC6801_OPTIONS_H_










