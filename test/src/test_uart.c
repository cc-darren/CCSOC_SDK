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

#include "tracer.h"
#include "drvi_uart.h"

#define UART_BUFFER_SIZE 16
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

/******************************************************************************
Declaration of External Variables & Functions
******************************************************************************/

/******************************************************************************
Declaration of data structure
******************************************************************************/

/******************************************************************************
Declaration of Global Variables & Functions
******************************************************************************/
//static void test_Uart0EventHandler(T_UartEvent * p_event);
static void test_Uart1EventHandler(T_UartEvent * p_event);
static void test_Uart2EventHandler(T_UartEvent * p_event);

/******************************************************************************
Declaration of static Global Variables & Functions
******************************************************************************/
//static T_UartPort TestUart0 =
//{
//    .bPortNum = 0,
//    .dwConfig = DRVI_UART_B1152000 | DRVI_UART_S8,
//    .fpComplete = test_Uart0EventHandler,
//};

static T_UartPort TestUart1 =
{
    .bPortNum = 1,
    .dwConfig = DRVI_UART_B1152000 | DRVI_UART_S8 | DRVI_UART_READ,
    .fpComplete = test_Uart1EventHandler,
};

static T_UartPort TestUart2 =
{
    .bPortNum = 2,
    .dwConfig = DRVI_UART_B1152000 | DRVI_UART_S8 | DRVI_UART_READ,
    .fpComplete = test_Uart2EventHandler,
};

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

uint32_t g_dwaUartConfig[512];

//static void test_Uart0EventHandler(T_UartEvent * p_event)
//{
//    if (strcmp(g_caUart0TxBuffer, g_caUart0RxBuffer))
//        g_iUart0Error++;

//    g_iuart0Done = 1;
//}
static void test_Uart1EventHandler(T_UartEvent * p_event)
{
    if (strcmp(g_caUart1TxBuffer, g_caUart1RxBuffer))
        g_iUart1Error++;

    g_iuart1Done = 1;
}
static void test_Uart2EventHandler(T_UartEvent * p_event)
{
    if (strcmp(g_caUart2TxBuffer, g_caUart2RxBuffer))
        g_iUart2Error++;

    g_iuart2Done = 1;
}

static INT16 test_UartConigInit(void)
{
    INT16 iIndex = 0;

    uint32_t dwaTestBaud[] =
    {
        DRVI_UART_B1200,
        DRVI_UART_B2400,
        DRVI_UART_B4800,
        DRVI_UART_B9600,
        DRVI_UART_B19200,
        DRVI_UART_B38400,
        DRVI_UART_B57600,
        DRVI_UART_B115200,
        DRVI_UART_B230400,
        DRVI_UART_B460800,
        DRVI_UART_B576000,
        DRVI_UART_B921600
    };

    uint32_t dwaTestParity[] =
    {
        0,
        DRVI_UART_PARENB,
        DRVI_UART_PARENB | DRVI_UART_PAREVEN,
        DRVI_UART_PARENB | DRVI_UART_MPAR,
        DRVI_UART_PARENB | DRVI_UART_SPAR,
    };

    uint32_t dwaTestSize[] =
    {
        DRVI_UART_S7,
        DRVI_UART_S8,
        DRVI_UART_S9,
    };

    uint32_t dwaTestStopBit[] =
    {
        0,
        DRVI_UART_STOPB,
    };

    uint32_t dwaTestHwFlowCtrl[] =
    {
        0,
        DRVI_UART_RTSCTS,
    };

    uint8_t bBIdx, bDIdx, bPIdx, bSIdx, bHIdx;

    memset(g_dwaUartConfig, 0, 512);

    for (bBIdx = 0; bBIdx < ARRAY_SIZE(dwaTestBaud); bBIdx++)
        for (bPIdx = 0; bPIdx < ARRAY_SIZE(dwaTestParity); bPIdx++)
            for (bDIdx = 0; bDIdx < ARRAY_SIZE(dwaTestSize); bDIdx++)
                for (bSIdx = 0; bSIdx < ARRAY_SIZE(dwaTestStopBit); bSIdx++)
                    for (bHIdx = 0; bHIdx < ARRAY_SIZE(dwaTestHwFlowCtrl); bHIdx++)
                    {
                        g_dwaUartConfig[iIndex] = dwaTestBaud[bBIdx] | dwaTestParity[bPIdx] |
                                                  dwaTestSize[bDIdx] | dwaTestStopBit[bSIdx] |
                                                  dwaTestHwFlowCtrl[bHIdx] | DRVI_UART_READ; 
                        iIndex++;
                    }
    return iIndex;
}

INT16 TEST_UartInit(void)
{
    INT16 iError = CC_SUCCESS;

    Tracer_Init();

    //iError = drvi_UartInit(&TestUart0);
    //if (iError)
    //    return iError;

    iError = drvi_UartInit(&TestUart1);
    if (iError)
    {
        printf("UART1 Config[0x%x] Not Support\r\n", TestUart2.dwConfig);
        return iError;
    }

    iError = drvi_UartInit(&TestUart2);
    if (iError)
    {
        printf("UART2 Config[0x%x] Not Support\r\n", TestUart2.dwConfig);
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
//            drvi_UartTx(&uart0, (const UINT8 *)g_caUart0TxBuffer, UART_BUFFER_SIZE);
//        }

        if (g_iuart1Done)
        {
            g_iuart1Done = 0;
            memset(g_caUart1TxBuffer, 0, UART_BUFFER_SIZE);
            memset(g_caUart1RxBuffer, 0, UART_BUFFER_SIZE);
            sprintf(g_caUart1TxBuffer, "%lu%c", dwTemp, '\0');
            drvi_UartRx(&TestUart1, (UINT8 *)g_caUart1RxBuffer, UART_BUFFER_SIZE);
            drvi_UartTx(&TestUart1, (const UINT8 *)g_caUart1TxBuffer, UART_BUFFER_SIZE);
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
            drvi_UartRx(&TestUart2, (UINT8 *)g_caUart2RxBuffer, UART_BUFFER_SIZE);
            drvi_UartTx(&TestUart2, (const UINT8 *)g_caUart2TxBuffer, UART_BUFFER_SIZE);
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
            {
                TestUart1.dwConfig = g_dwaUartConfig[iConfigIdx];
                iError = drvi_UartInit(&TestUart1);
            }
            else
                printf("UART Initial Waiting for UART1 Done!\r\n");

            if(g_iuart2Done)
            {
                TestUart2.dwConfig = g_dwaUartConfig[iConfigIdx];
                iError = drvi_UartInit(&TestUart2);
            }
            else
                printf("UART Initial Waiting for UART2 Done!\r\n");
        }while(--iRetry);

        if ((!iRetry) && (!g_iuart1Done))
        {
            printf("UART1 Died, Re-init and Re-active!\r\n");
            TestUart1.dwConfig = g_dwaUartConfig[iConfigIdx];
            iError = drvi_UartInit(&TestUart1);
            g_iuart1Done = 1;
        }
        else if ((!iRetry) && (!g_iuart2Done))
        {
            printf("UART2 Died, Re-init and Re-active!\r\n");
            iError = drvi_UartInit(&TestUart2);
            g_iuart2Done = 1;
        }

        if (iError)
        {
            printf("UART Config[0x%x] Not Support\r\n", g_dwaUartConfig[iConfigIdx]);
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
//                drvi_UartTx(&uart0, (const UINT8 *)g_caUart0TxBuffer, UART_BUFFER_SIZE);
//            }

            if (g_iuart1Done)
            {
                g_iuart1Done = 0;
                memset(g_caUart1TxBuffer, 0, UART_BUFFER_SIZE);
                memset(g_caUart1RxBuffer, 0, UART_BUFFER_SIZE);
                sprintf(g_caUart1TxBuffer, "%lu%c", dwTemp, '\0');
                drvi_UartRx(&TestUart1, (UINT8 *)g_caUart1RxBuffer, UART_BUFFER_SIZE);
                drvi_UartTx(&TestUart1, (const UINT8 *)g_caUart1TxBuffer, UART_BUFFER_SIZE);
                iIndex++;
                g_iUart1Count++;
            }

            if (g_iuart2Done)
            {
                g_iuart2Done = 0;
                memset(g_caUart2TxBuffer, 0, UART_BUFFER_SIZE);
                memset(g_caUart2RxBuffer, 0, UART_BUFFER_SIZE);
                sprintf(g_caUart2TxBuffer, "%lu%c", dwTemp, '\0');
                drvi_UartRx(&TestUart2, (UINT8 *)g_caUart2RxBuffer, UART_BUFFER_SIZE);
                drvi_UartTx(&TestUart2, (const UINT8 *)g_caUart2TxBuffer, UART_BUFFER_SIZE);
                iIndex++;
                g_iUart2Count++;
            }
        }
        printf("UART Config[0x%x] Error[u1:%d,u2:%d] Count[u1:%lu,u2:%lu]\r\n", \
                                                   g_dwaUartConfig[iConfigIdx], \
                                                   g_iUart1Error, g_iUart2Error, \
                                                   g_iUart1Count, g_iUart2Count);
    }

    printf("UART Loopback Done!\r\n");
}
#endif //TEST_AES
