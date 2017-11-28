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

/******************************************************************************
Head Block of The File
******************************************************************************/
#include <stdio.h>
#include <string.h>
#include "ll.h"
#include "drvi_init.h"
#include "drvi_gpio.h"
#include "test.h"
#if (TEST_AES)
#include "test_aes.h"
#endif
#if (TEST_I2C)
#include "test_i2c.h"
#endif
#if (TEST_SPI)
#include "test_spi.h"
#endif
#if (TEST_UART_LOOPBACK)
#include "test_uart.h"
#endif
#if (TEST_CLOCK)
#include "test_clock.h"
#endif
#if (TEST_MEMRW)
#include "test_mem.h"
#endif

/******************************************************************************
Declaration of External Variables & Functions
******************************************************************************/
//extern void mem_rw(void);

/******************************************************************************
Declaration of data structure
******************************************************************************/

/******************************************************************************
Declaration of Global Variables & Functions
******************************************************************************/

/******************************************************************************
Declaration of static Global Variables & Functions
******************************************************************************/

/******************************************************************************
FUNCTION
  cc6801_Init

DESCRIPTION
  1. The function implements all the cc6801 modules initialization.

PARAMETERS

RETURNS

******************************************************************************/
static int cc6801_Init(void)
{
#if (TEST_AES)
    TEST_AesInit();
#endif

#if (TEST_UART_LOOPBACK)
    TEST_UartInit();
#endif
    return 0;
}

void TEST_Main(void)
{
    cc6801_Init();

#if (TEST_CLOCK)
    TEST_DynamicClock(1000);
#endif
#if (TEST_MEMRW)
    TEST_MemRW(10);
#endif
#if (TEST_AES)
    TEST_AesLoop(1000);
#endif
#if (TEST_SPI)
    TEST_SpiRW(1000);
#endif
#if (TEST_I2C)
    TEST_I2cRW(1000);
#endif
#if (TEST_UART_LOOPBACK)
    TEST_UartLoopBack(1000);
#endif

}
