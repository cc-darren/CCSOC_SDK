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
#include "test_config.h"

#if (TEST_UART0_TXDMA) || (TEST_UART0_RXDMA)

#include <stdio.h>
#include <string.h>

#include "app_trace.h"
#include "drvi_uart.h"
#include "uart.h"

#define UART_BUFFER_SIZE 16

/******************************************************************************
Declaration of External Variables & Functions
******************************************************************************/

/******************************************************************************
Declaration of data structure
******************************************************************************/

/******************************************************************************
Declaration of Global Variables & Functions
******************************************************************************/

/******************************************************************************
Declaration of static Global Variables & Functions
******************************************************************************/
//static const cc_drv_uart_t uart0 = CC_DRV_UART_INSTANCE(0);
static const cc_drv_uart_t uart1 = CC_DRV_UART_INSTANCE(1);
static const cc_drv_uart_t uart2 = CC_DRV_UART_INSTANCE(2);

//__align(4) static INT8 g_caUart0TxBuffer[UART_BUFFER_SIZE] = {0};
//__align(4) static INT8 g_caUart0RxBuffer[UART_BUFFER_SIZE] = {0};
__align(4) static INT8 g_caUart1TxBuffer[UART_BUFFER_SIZE] = {0};
__align(4) static INT8 g_caUart1RxBuffer[UART_BUFFER_SIZE] = {0};
__align(4) static INT8 g_caUart2TxBuffer[UART_BUFFER_SIZE] = {0};
__align(4) static INT8 g_caUart2RxBuffer[UART_BUFFER_SIZE] = {0};

//static INT16 g_iuart0Done = 0;
static INT16 g_iuart1Done = 0;
static INT16 g_iuart2Done = 0;

//static INT16 g_iUart0Error = 0;
static INT16 g_iUart1Error = 0;
static INT16 g_iUart2Error = 0;

//static UINT32 g_iUart0Count = 0;
static UINT32 g_iUart1Count = 0;
static UINT32 g_iUart2Count = 0;

drvi_uart_params_t g_EUartConfig[512];

//static void test_Uart0EventHandler(cc_drv_uart_event_t * p_event, void* p_context)
//{
//    if (strcmp(g_caUart0TxBuffer, g_caUart0RxBuffer))
//        g_iUart0Error++;
//
//    g_iuart0Done = 1;
//}
static void test_Uart1EventHandler(cc_drv_uart_event_t * p_event, void* p_context)
{
    if (strcmp(g_caUart1TxBuffer, g_caUart1RxBuffer))
        g_iUart1Error++;

    g_iuart1Done = 1;
}
static void test_Uart2EventHandler(cc_drv_uart_event_t * p_event, void* p_context)
{
    if (strcmp(g_caUart2TxBuffer, g_caUart2RxBuffer))
        g_iUart2Error++;

    g_iuart2Done = 1;
}

static INT16 test_UartConigInit(void)
{
    INT16 iIndex = 0;
    drvi_uart_baudrate_t baud;
    drvi_uart_parity_t parity;
    drvi_uart_data_bits databit;
    drvi_uart_stop_bits stopbit;
    drvi_uart_hwfc_t hwfc;

    memset(g_EUartConfig, 0, 512);

    for (baud=DRVI_UART_BAUDRATE_1200; baud<=DRVI_UART_BAUDRATE_921600; baud++)
        for (parity=DRVI_UART_PARITY_ODD; parity <=DRVI_UART_PARITY_DISABLE; parity++)
            for (databit=DRVI_UART_8_DATA_BITS; databit <=DRVI_UART_7_DATA_BITS; databit++)
                for (stopbit=DRVI_UART_1_STOP_BITS; stopbit <=DRVI_UART_2_STOP_BITS; stopbit++)
                    for (hwfc=DRVI_UART_HWFC_DISABLE; hwfc <=DRVI_UART_HWFC_ENABLE; hwfc++)
                    {
                        g_EUartConfig[iIndex].baudrate = baud;
                        g_EUartConfig[iIndex].data_bits = databit;
                        g_EUartConfig[iIndex].hw_flow = hwfc;
                        g_EUartConfig[iIndex].parity = parity;
                        g_EUartConfig[iIndex].stop_bits = stopbit;
                        iIndex++;
                    }
    return iIndex;
}

INT16 TEST_UartInit(void)
{
    INT16 iError = CC_SUCCESS;
    drvi_uart_params_t uart1_config = DRVI_UART_DEFAULT_CONFIG;
    drvi_uart_params_t uart2_config = DRVI_UART_DEFAULT_CONFIG;

    app_trace_init();

    //iError = drvi_uart_init(&uart0, &uart0_config, test_Uart0EventHandler);
    //if (iError)
    //    return iError;
    iError = drvi_uart_init(&uart1, &uart1_config, test_Uart1EventHandler);
    if (iError)
    {
        printf("UART1 Config[b:%d,p:%d,d:%d,s:%d,f:%d] Not Support\r\n", \
                                                   uart1_config.baudrate, \
                                                   uart1_config.parity, \
                                                   uart1_config.data_bits, \
                                                   uart1_config.stop_bits,
                                                   uart1_config.hw_flow);
        return iError;
    }

    iError = drvi_uart_init(&uart2, &uart2_config, test_Uart2EventHandler);
    if (iError)
    {
        printf("UART2 Config[b:%d,p:%d,d:%d,s:%d,f:%d] Not Support\r\n", \
                                                   uart2_config.baudrate, \
                                                   uart2_config.parity, \
                                                   uart2_config.data_bits, \
                                                   uart2_config.stop_bits,
                                                   uart2_config.hw_flow);
        return iError;
    }

    //g_iuart0Done = 1;
    g_iuart1Done = 1;
    g_iuart2Done = 1;

    return CC_SUCCESS;
}

void TEST_UartLoopBackSingleConfig(UINT32 iCount)
{
    INT16 iError = CC_SUCCESS;
    UINT32 iIndex = 0;
    UINT32 dwTemp = 0;

    iError = TEST_UartInit();
    if (iError)
        return;

    while(iIndex<iCount)
    {
        //Generate a complicated number
        dwTemp = iIndex*iIndex*3957;
//        if (g_iuart0Done)
//        {
//            g_iuart0Done = 0;
//            memset(g_caUart0TxBuffer, 0, UART_BUFFER_SIZE);
//            memset(g_caUart0RxBuffer, 0, UART_BUFFER_SIZE);
//            sprintf(g_caUart0TxBuffer, "%lu%c", dwTemp, '\0');
//            drvi_uart_tx(&uart0, (const UINT8 *)g_caUart0TxBuffer, UART_BUFFER_SIZE);
//        }

        if (g_iuart1Done)
        {
            g_iuart1Done = 0;
            memset(g_caUart1TxBuffer, 0, UART_BUFFER_SIZE);
            memset(g_caUart1RxBuffer, 0, UART_BUFFER_SIZE);
            sprintf(g_caUart1TxBuffer, "%lu%c", dwTemp, '\0');
            drvi_uart_rx(&uart1, (UINT8 *)g_caUart1RxBuffer, UART_BUFFER_SIZE);
            drvi_uart_tx(&uart1, (const UINT8 *)g_caUart1TxBuffer, UART_BUFFER_SIZE);
            //while(!g_iuart1Done);
            //g_iuart1Done = 0;
            iIndex++;
            g_iUart1Count++;
        }

        if (g_iuart2Done)
        {
            g_iuart2Done = 0;
            memset(g_caUart2TxBuffer, 0, UART_BUFFER_SIZE);
            memset(g_caUart2RxBuffer, 0, UART_BUFFER_SIZE);
            sprintf(g_caUart2TxBuffer, "%lu%c", dwTemp, '\0');
            drvi_uart_rx(&uart2, (UINT8 *)g_caUart2RxBuffer, UART_BUFFER_SIZE);
            drvi_uart_tx(&uart2, (const UINT8 *)g_caUart2TxBuffer, UART_BUFFER_SIZE);
            //while(!g_iuart2Done);
            //g_iuart2Done = 0;
            iIndex++;
            g_iUart2Count++;
        }
    }

    printf("UART Loopback Single Config Done!\r\n");
}

void TEST_UartLoopBack(UINT32 iCount)
{
    INT16 iConfigIdx = 0, iConfigNum = 0;
    INT16 iError = CC_SUCCESS;
    INT16 iRetry = 0;
    UINT32 iIndex = 0;
    UINT32 dwTemp = 0;

    iError = TEST_UartInit();
    iConfigNum = test_UartConigInit();

    for (iConfigIdx=0; iConfigIdx<iConfigNum; iConfigIdx++)
    {
        iIndex = 0;
        iRetry = 100;

        do
        {
            if(g_iuart1Done)
                iError = drvi_uart_init(&uart1, &g_EUartConfig[iConfigIdx], test_Uart1EventHandler);
            else
                printf("UART Initial Waiting for UART1 Done!\r\n");

            if(g_iuart2Done)
                iError = drvi_uart_init(&uart2, &g_EUartConfig[iConfigIdx], test_Uart2EventHandler);
            else
                printf("UART Initial Waiting for UART2 Done!\r\n");
        }while(--iRetry);

        if ((!iRetry) && (!g_iuart1Done))
        {
            printf("UART1 Died, Re-init and Re-active!\r\n");
            iError = drvi_uart_init(&uart1, &g_EUartConfig[iConfigIdx], test_Uart1EventHandler);
            g_iuart1Done = 1;
        }
        else if ((!iRetry) && (!g_iuart2Done))
        {
            printf("UART2 Died, Re-init and Re-active!\r\n");
            iError = drvi_uart_init(&uart2, &g_EUartConfig[iConfigIdx], test_Uart2EventHandler);
            g_iuart2Done = 1;
        }

        if (iError)
        {
            printf("UART Config[b:%d,p:%d,d:%d,s:%d,f:%d] Not Support\r\n", \
                                                       g_EUartConfig[iConfigIdx].baudrate, \
                                                       g_EUartConfig[iConfigIdx].parity, \
                                                       g_EUartConfig[iConfigIdx].data_bits, \
                                                       g_EUartConfig[iConfigIdx].stop_bits,
                                                       g_EUartConfig[iConfigIdx].hw_flow);
            continue;
        }

        while(iIndex<iCount)
        {
            //Generate a complicated number
            dwTemp = iIndex*iIndex*3957;

//            if (g_iuart0Done)
//            {
//                g_iuart0Done = 0;
//                memset(g_caUart0TxBuffer, 0, UART_BUFFER_SIZE);
//                memset(g_caUart0RxBuffer, 0, UART_BUFFER_SIZE);
//                sprintf(g_caUart0TxBuffer, "%lu%c", dwTemp, '\0');
//                drvi_uart_tx(&uart0, (const UINT8 *)g_caUart0TxBuffer, UART_BUFFER_SIZE);
//            }

            if (g_iuart1Done)
            {
                g_iuart1Done = 0;
                memset(g_caUart1TxBuffer, 0, UART_BUFFER_SIZE);
                memset(g_caUart1RxBuffer, 0, UART_BUFFER_SIZE);
                sprintf(g_caUart1TxBuffer, "%lu%c", dwTemp, '\0');
                drvi_uart_rx(&uart1, (UINT8 *)g_caUart1RxBuffer, UART_BUFFER_SIZE);
                drvi_uart_tx(&uart1, (const UINT8 *)g_caUart1TxBuffer, UART_BUFFER_SIZE);
                iIndex++;
                g_iUart1Count++;
            }

            if (g_iuart2Done)
            {
                g_iuart2Done = 0;
                memset(g_caUart2TxBuffer, 0, UART_BUFFER_SIZE);
                memset(g_caUart2RxBuffer, 0, UART_BUFFER_SIZE);
                sprintf(g_caUart2TxBuffer, "%lu%c", dwTemp, '\0');
                drvi_uart_rx(&uart2, (UINT8 *)g_caUart2RxBuffer, UART_BUFFER_SIZE);
                drvi_uart_tx(&uart2, (const UINT8 *)g_caUart2TxBuffer, UART_BUFFER_SIZE);
                iIndex++;
                g_iUart2Count++;
            }
        }
        printf("UART Config[b:%d,p:%d,d:%d,s:%d,f:%d] Error[u1:%d,u2:%d] Count[u1:%lu,u2:%lu]\r\n", \
                                                   g_EUartConfig[iConfigIdx].baudrate, \
                                                   g_EUartConfig[iConfigIdx].parity, \
                                                   g_EUartConfig[iConfigIdx].data_bits, \
                                                   g_EUartConfig[iConfigIdx].stop_bits,
                                                   g_EUartConfig[iConfigIdx].hw_flow, \
                                                   g_iUart1Error, g_iUart2Error,
                                                   g_iUart1Count, g_iUart2Count);
    }

    printf("UART Loopback Done!\r\n");
}
#endif //TEST_AES
