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
#include "test_config.h"
#if (TEST_AES)
#include "test_aes.h"
#endif
#if (TEST_EFLASH)
#include "eflash.h"
#endif
#if (TEST_I2C0)
#include "ak09912.h"
#include "i2c_sensor.h"
#endif
#if (TEST_I2C1)
#include "pah8002_comm.h"
#endif
#if (TEST_PWM0)
#include "drvi_pwm.h"
#endif
#if (TEST_RTC)
#include "rtc.h"
#endif
#if (TEST_SPI)
#include "test_spi.h"
#endif
#if (TEST_UART0_TXDMA) || (TEST_UART0_RXDMA)
#include "test_uart.h"
#include "uart.h"
#endif
#if (TEST_WDT)
#include "drvi_wdt.h"
#endif
#if (TEST_WKTM0)
#include "drvi_timer.h"
#endif


/******************************************************************************
Declaration of External Variables & Functions
******************************************************************************/
//extern void mem_rw(void);

/******************************************************************************
Declaration of data structure
******************************************************************************/
struct S_ModuleExecCount
{
    UINT32 dwAes;
    UINT32 dwEflash;
    UINT32 dwGpi;
    UINT32 dwI2c0;
    UINT32 dwI2c1;
    UINT32 dwMem;
    UINT32 dwPwm0;
    UINT32 dwSpi1;
    UINT32 dwSpi2;
    UINT32 dwUart0Tx;
    UINT32 dwUart0Rx;
    UINT32 dwWdt;
    UINT32 dwWktm0;
};

/******************************************************************************
Declaration of Global Variables & Functions
******************************************************************************/
UINT32 g_MemTestStart = 1;
UINT32 g_Spi1TestStart = 1;
UINT32 g_Spi2TestStart = 1;
UINT32 g_I2c0TestStart = 1;
UINT32 g_I2c1TestStart = 1;
UINT32 g_Uart0TxDmaTestStart = 1;
UINT32 g_Uart0RxDmaTestStart = 1;
UINT32 g_AesTestStart = 1;
UINT32 g_WdtTestStart = 1;
UINT32 g_EflashTestStart = 1;
UINT32 g_Wktm0TestStart = 1;
UINT32 g_Pwm0TestStart = 1;
UINT32 g_GpiTestStart = 1;
UINT32 g_RtcTestStart = 1;

struct S_ModuleExecCount S_Count;

/******************************************************************************
Declaration of static Global Variables & Functions
******************************************************************************/
static const cc_drv_uart_t uart0 = CC_DRV_UART_INSTANCE(0);

// Exported function
void Timer_Callback(void *para)
{
    drvi_timer0_counterClear();
    S_Count.dwWktm0++;
}

void Pwm_Callback(void *para)
{
    if (S_Count.dwPwm0 < 5000)
        drvi_pwm0_stop();
    else if ((S_Count.dwPwm0 < 7000))
        drvi_pwm0_start();
    else
        S_Count.dwPwm0 = 0;

    S_Count.dwPwm0++;
}

void Gpi_Callback(void)
{
    S_Count.dwGpi++;
}

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
    memset(&S_Count, 0, sizeof(struct S_ModuleExecCount));

    drvi_gpio_pinmux_init();
#if (TEST_AES)
    TEST_AesInit();
#endif
    if (g_GpiTestStart)
    {
        drvi_request_irq(1, Gpi_Callback, IRQ_TYPE_EDGE_FALLING);
        drvi_enable_irq(1);
    }
    if (g_I2c0TestStart)
    {
        i2c_init();
    }
    if (g_I2c1TestStart)
    {
        //pah8002_twi_init();
    }
#if (TEST_UART0_TXDMA) || (TEST_UART0_RXDMA)
    TEST_UartInit();
#endif
    if (g_EflashTestStart)
    {
        //for (n=0; n<64; n++)
        //    bEflashBuf[n] = n;
        //cc6801_EFinit();
    }
    if (g_Wktm0TestStart)
    {
        //timers_init();
        //timers_start_test();
        drvi_timer0_init(Timer_Callback);
        drvi_timer0_start();
        drvi_timer0_counterLoad(30000);
    }
    if (g_Pwm0TestStart)
    {
        //pwm_start_test();
        drvi_pwm0_init(Pwm_Callback);
        drvi_pwm0_loadPrescaler(0);
        drvi_pwm0_duty(50);
    }
    if (g_WdtTestStart)
    {
        drvi_wdt_init(5000);
        drvi_wdt_enable();
    }
    if (g_RtcTestStart)
    {
        cc6801_rtcInit();
    }

    return 0;
}

int TEST_Main(void)
{
    INT8 cUartTxBuf[128];
    INT8 cStrBuf[128];
    UINT8 bUartRxBuf;

    //UINT8 bEflashBuf[64];

    //UINT8 bAccBuf[12];
    UINT8 bMagBuf[6];

    S_rtcInfo S_Time;

    UINT32 dwLoop = 0;

    cc6801_Init();

    //start interrupt handling
    GLOBAL_INT_START();

#if (TEST_AES)
    TEST_AesLoop(1000);
#endif
#if (TEST_SPI)
    TEST_SpiRW(1000);
#endif
#if (TEST_UART0_TXDMA) || (TEST_UART0_RXDMA)
    TEST_UartLoopBack(1000);
#endif

    while(1)
    {

        if (g_MemTestStart)
        {
            g_MemTestStart = 0;
            //mem_rw();
            S_Count.dwMem++;
        }
        if (g_I2c0TestStart)
        {
            g_I2c0TestStart = 0;
            MAG_GetMagRawBurst(bMagBuf, 6);
            S_Count.dwI2c0++;
        }
        if (g_I2c1TestStart)
        {
            g_I2c1TestStart = 0;
            //pah8002_write_reg(0x7f, 0x00);
            S_Count.dwI2c1++;
        }
        if (g_Uart0RxDmaTestStart)
        {
            g_Uart0RxDmaTestStart = 0;
            cc_drv_uart_rx(&uart0, &bUartRxBuf, 1);
            S_Count.dwUart0Rx++;
        }
        if (g_EflashTestStart)
        {
            g_EflashTestStart = 0;
            //cc6801_EFFlush();
            //cc6801_EFDMAProgram(0x10020000, 64, bEflashBuf);
            S_Count.dwEflash++;
        }
        if (g_WdtTestStart)
        {
            g_WdtTestStart = 0;
            drvi_wdt_feed();
            S_Count.dwWdt++;
        }
        if (g_Wktm0TestStart)
        {
            g_Wktm0TestStart = 0;
        }
        if (g_Pwm0TestStart)
        {
            g_Pwm0TestStart = 0;
        }
        if (g_GpiTestStart)
        {
            g_GpiTestStart = 0;
        }
        if (g_RtcTestStart)
        {
            g_RtcTestStart = 0;
            S_Time = cc6801_rtcGetTime();
        }
        if (g_Uart0TxDmaTestStart)
        {
            g_Uart0TxDmaTestStart = 0;

            UINT32 *p_mod = (UINT32 *)&S_Count;
            int i = 0;
            //n = sprintf(cUartTxBuf, "%d\r\n", aes);
            sprintf(cUartTxBuf, "[%02d:%02d:%02d]", S_Time.hour, S_Time.min, S_Time.sec);
            strcat(cUartTxBuf, "[");
            for (i=0; i<(sizeof(struct S_ModuleExecCount)/sizeof(UINT32)); i++)
            {
                sprintf(cStrBuf, "%lu", *(p_mod+i));
                strcat(cUartTxBuf, cStrBuf);
            }
            strcat(cUartTxBuf, "]\r\n");

            //sprintf(cStrBuf, "[%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d]\r\n",\
            //                         S_Count.mem, S_Count.spi1, S_Count.spi2,\
            //                         S_Count.i2c0, S_Count.i2c1, S_Count.eflash,\
            //                         S_Count.aes, S_Count.gpi, S_Count.uart0_rx, S_Count.wdt, S_Count.wktm0);
            //strcat(cUartTxBuf, cStrBuf);
            cc_drv_uart_tx(&uart0, (const UINT8 *)cUartTxBuf, strlen(cUartTxBuf));
            S_Count.dwUart0Tx++;
        }
        if (((dwLoop >> 3) << 3) == dwLoop)
        {
            drvi_gpio_write(3, 0);
            drvi_gpio_write(3, 1);
        }
        dwLoop++;
    }
}
