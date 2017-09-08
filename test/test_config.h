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
*
*
*  Project:
*  --------
*  CC6801
*
*  Description:
*  ------------
*  the description
*
*  Author:
*  -------
*  Blake   (cc-blake)
*
*===========================================================================
*
******************************************************************************/

#ifndef _TEST_CONFIG_H_
#define _TEST_CONFIG_H_

/******************************************************************************
Head Block of The File
******************************************************************************/
// Sec 0: Comment block of the file

// Sec 1: Include File

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous
#define TEST_MEM                    0
#define TEST_SPI                    1
#define TEST_I2C                    1
#define TEST_UART0_TXDMA            1
#define TEST_UART0_RXDMA            1
#define TEST_AES                    1
#define TEST_WDT                    1
#define TEST_EFLASH                 0
#define TEST_WKTM0                  1
#define TEST_PWM0                   1
#define TEST_GPI                    1
#define TEST_RTC                    1

#if (TEST_SPI)
#define TEST_SPIM ACC_IF_ID
#endif

#if defined(TEST_I2C) && TEST_I2C
#define TEST_I2C_BUS        MAG_IF_ID
#define TEST_I2C_ADDRESS    0x50
#define TEST_I2C_CLKRATE    CC_I2C_FREQ_100K
#endif

/******************************************************************************
Declaration of External Variables & Functions
******************************************************************************/
// Sec 3: declaration of external variable

// Sec 4: declaration of external function prototype

/******************************************************************************
Declaration of data structure
******************************************************************************/
// Sec 5: structure, uniou, enum, linked list

/******************************************************************************
Declaration of Global Variables & Functions
******************************************************************************/
// Sec 6: declaration of global variable

// Sec 7: declaration of global function prototype

/******************************************************************************
Declaration of static Global Variables & Functions
******************************************************************************/
// Sec 8: declaration of static global variable

// Sec 9: declaration of static function prototype

/******************************************************************************
// Sec 10: C Functions
******************************************************************************/

#endif // _TEST_CONFIG_H_
