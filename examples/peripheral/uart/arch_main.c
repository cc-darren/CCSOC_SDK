/* Copyright (c) 2019 Cloudchip, Inc. All Rights Reserved.
 *
 * The information contained herein is property of Cloudchip, Inc.
 * Terms and conditions of usage are described in detail in CLOUDCHIP
 * STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information.
 * NO WARRANTY of ANY KIND is provided. This heading must NOT be removed 
 * from the file.
 */

/*
 * INCLUDES
 ****************************************************************************************
 */
#include <stdio.h>
#include <string.h>

#include "ll.h"
#include "drvi_init.h"
#include "tracer.h"
#include "project.h"

extern void sys_InitMain(void);

static volatile uint16_t g_iuart0Done = 0;

static void test_Uart0EventHandler(T_UartEvent * p_event)
{
    g_iuart0Done = 1;
}

#ifdef UART_LOOPBACK_TEST
#define UART_BUFFER_SIZE 16
__align(4) static INT8 g_caUart0TxBuffer[UART_BUFFER_SIZE] = {0};
__align(4) static INT8 g_caUart0RxBuffer[UART_BUFFER_SIZE] = {0};

static void UartLoopBackTest(uint32_t iCount)
{
    uint32_t iIndex = 0;
    uint32_t dwTemp = 0;

    while(iIndex<iCount)
    {
        //Generate a complicated number
        dwTemp = iIndex*iIndex*3957;

        if (g_iuart0Done)
        {
            g_iuart0Done = 0;
            if (strcmp(g_caUart0TxBuffer, g_caUart0RxBuffer))
                TracerInfo("Uart Loopback Error!!\r\n");

            memset(g_caUart0TxBuffer, 0, UART_BUFFER_SIZE);
            memset(g_caUart0RxBuffer, 0, UART_BUFFER_SIZE);
            sprintf(g_caUart0TxBuffer, "%lu%c", dwTemp, '\0');
            drvi_UartReceive(TRACER_IF_ID, (UINT8 *)g_caUart0RxBuffer, UART_BUFFER_SIZE);
            drvi_UartTx(TRACER_IF_ID, (const UINT8 *)g_caUart0TxBuffer, UART_BUFFER_SIZE);
            iIndex++;
        }
    }

    return;
}
#endif

static void UartInit(void)
{
    drvi_UartRxDoneRegister(TRACER_IF_ID, test_Uart0EventHandler);
    g_iuart0Done = 1;
}

/*
 * MAIN FUNCTION
 ****************************************************************************************
 */

int main(void)
{
    __align(4) uint8_t cr;

    //Must be first in main()
    sys_InitMain();

    //start interrupt handling
    GLOBAL_INT_START();

    drvi_initialize();

    TracerInfo("== CC6801 Start ==\r\n");

    /*******************************/
    /****** Application Start ******/
    /*******************************/

    UartInit();

    while(1)
    {
#ifndef UART_LOOPBACK_TEST
        g_iuart0Done = 0;
        drvi_UartReceive(TRACER_IF_ID, &cr, 1);
        while(!g_iuart0Done);

        drvi_UartTx(TRACER_IF_ID, &cr, 1);

//        if (cr == 'q' || cr == 'Q')
//        {
//            TracerInfo(" \r\nExit!\r\n");

//            while (1)
//            {
//                // Stop here
//            }
//        }
#else
        UartLoopBackTest(1000);
        while (1)
        {
            // Stop here
        }
#endif
    }
}

