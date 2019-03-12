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
#include "test.h"

#if (TEST_UART_LOOPBACK)

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

/******************************************************************************
Declaration of static Global Variables & Functions
******************************************************************************/
#ifdef TEST_UART0_BUS
static uint16_t g_iuart0Done = 0;
static uint16_t g_iUart0Error = 0;
static uint32_t g_iUart0Count = 0;

__align(4) static INT8 g_caUart0TxBuffer[UART_BUFFER_SIZE] = {0};
__align(4) static INT8 g_caUart0RxBuffer[UART_BUFFER_SIZE] = {0};

static T_UartPort TestUart0 =
{
    .bPortNum = TEST_UART0_BUS,
    .dwConfig = UART_BAUDRATE_115200 | UART_DATA_BITS_8,
};

static void test_Uart0EventHandler(T_UartEvent * p_event)
{
    if (strcmp(g_caUart0TxBuffer, g_caUart0RxBuffer))
        g_iUart0Error++;

    g_iuart0Done = 1;
}
#endif

#ifdef TEST_UART1_BUS
static uint16_t g_iuart1Done = 0;
static uint16_t g_iUart1Error = 0;
static uint32_t g_iUart1Count = 0;

__align(4) static INT8 g_caUart1TxBuffer[UART_BUFFER_SIZE] = {0};
__align(4) static INT8 g_caUart1RxBuffer[UART_BUFFER_SIZE] = {0};

static T_UartPort TestUart1 =
{
    .bPortNum = TEST_UART1_BUS,
    .dwConfig = UART_BAUDRATE_115200 | UART_DATA_BITS_8,
};

static void test_Uart1EventHandler(T_UartEvent * p_event)
{
    if (strcmp(g_caUart1TxBuffer, g_caUart1RxBuffer))
        g_iUart1Error++;

    g_iuart1Done = 1;
}
#endif

#ifdef TEST_UART2_BUS
static uint16_t g_iuart2Done = 0;
static uint16_t g_iUart2Error = 0;
static uint32_t g_iUart2Count = 0;

__align(4) static INT8 g_caUart2TxBuffer[UART_BUFFER_SIZE] = {0};
__align(4) static INT8 g_caUart2RxBuffer[UART_BUFFER_SIZE] = {0};

static T_UartPort TestUart2 =
{
    .bPortNum = TEST_UART2_BUS,
    .dwConfig = UART_BAUDRATE_115200 | UART_DATA_BITS_8,
};

static void test_Uart2EventHandler(T_UartEvent * p_event)
{
    if (strcmp(g_caUart2TxBuffer, g_caUart2RxBuffer))
        g_iUart2Error++;

    g_iuart2Done = 1;
}
#endif

uint32_t g_dwaUartConfig[720];

static uint16_t test_UartConigInit(void)
{
    uint16_t iIndex = 0;

    uint32_t dwaTestBaud[] =
    {
        UART_BAUDRATE_1200,
        UART_BAUDRATE_2400,
        UART_BAUDRATE_4800,
        UART_BAUDRATE_9600,
        UART_BAUDRATE_14400,
        UART_BAUDRATE_19200,
        UART_BAUDRATE_38400,
        UART_BAUDRATE_57600,
        UART_BAUDRATE_115200,
        UART_BAUDRATE_230400,
        UART_BAUDRATE_460800,
        UART_BAUDRATE_921600
    };

    uint32_t dwaTestParity[] =
    {
        0,
        UART_PARITY_ENABLE,
        UART_PARITY_ENABLE | UART_PARITY_EVEN,
        UART_PARITY_ENABLE | UART_PARITY_MARK,
        UART_PARITY_ENABLE | UART_PARITY_SPACE,
    };

    uint32_t dwaTestSize[] =
    {
        UART_DATA_BITS_7,
        UART_DATA_BITS_8,
        UART_DATA_BITS_9,
    };

    uint32_t dwaTestStopBit[] =
    {
        0,
        UART_STOP_BITS_2,
    };

    uint32_t dwaTestHwFlowCtrl[] =
    {
        0,
        UART_RTSCTS,
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
                                                  dwaTestHwFlowCtrl[bHIdx];
                        iIndex++;
                    }
    return iIndex;
}

void TEST_UartInit(void)
{
#ifdef TEST_UART0_BUS
    drvi_UartRxDoneRegister(TEST_UART0_BUS, test_Uart0EventHandler);
    g_iuart0Done = 1;
#endif
#ifdef TEST_UART1_BUS
    drvi_UartRxDoneRegister(TEST_UART1_BUS, test_Uart1EventHandler);
    g_iuart1Done = 1;
#endif
#ifdef TEST_UART2_BUS
    drvi_UartRxDoneRegister(TEST_UART2_BUS, test_Uart2EventHandler);
    g_iuart2Done = 1;
#endif
}

void TEST_UartLoopBackSingleConfig(uint32_t iCount)
{
    uint32_t iIndex = 0;
    uint32_t dwTemp = 0;

    TEST_UartInit();

    while(iIndex<iCount)
    {
        //Generate a complicated number
        dwTemp = iIndex*iIndex*3957;
#ifdef TEST_UART0_BUS
        if (g_iuart0Done)
        {
            g_iuart0Done = 0;
            memset(g_caUart0TxBuffer, 0, UART_BUFFER_SIZE);
            memset(g_caUart0RxBuffer, 0, UART_BUFFER_SIZE);
            sprintf(g_caUart0TxBuffer, "%lu%c", dwTemp, '\0');
            drvi_UartReceive(TEST_UART0_BUS, (UINT8 *)g_caUart0RxBuffer, UART_BUFFER_SIZE);
            drvi_UartTx(TEST_UART0_BUS, (const UINT8 *)g_caUart0TxBuffer, UART_BUFFER_SIZE);
            iIndex++;
            g_iUart0Count++;
        }
#endif
#ifdef TEST_UART1_BUS
        if (g_iuart1Done)
        {
            g_iuart1Done = 0;
            memset(g_caUart1TxBuffer, 0, UART_BUFFER_SIZE);
            memset(g_caUart1RxBuffer, 0, UART_BUFFER_SIZE);
            sprintf(g_caUart1TxBuffer, "%lu%c", dwTemp, '\0');
            drvi_UartReceive(TEST_UART1_BUS, (UINT8 *)g_caUart1RxBuffer, UART_BUFFER_SIZE);
            drvi_UartTx(TEST_UART1_BUS, (const UINT8 *)g_caUart1TxBuffer, UART_BUFFER_SIZE);
            //while(!g_iuart1Done);
            //g_iuart1Done = 0;
            iIndex++;
            g_iUart1Count++;
        }
#endif
#ifdef TEST_UART2_BUS
        if (g_iuart2Done)
        {
            g_iuart2Done = 0;
            memset(g_caUart2TxBuffer, 0, UART_BUFFER_SIZE);
            memset(g_caUart2RxBuffer, 0, UART_BUFFER_SIZE);
            sprintf(g_caUart2TxBuffer, "%lu%c", dwTemp, '\0');
            drvi_UartReceive(TEST_UART2_BUS, (UINT8 *)g_caUart2RxBuffer, UART_BUFFER_SIZE);
            drvi_UartTx(TEST_UART2_BUS, (const UINT8 *)g_caUart2TxBuffer, UART_BUFFER_SIZE);
            //while(!g_iuart2Done);
            //g_iuart2Done = 0;
            iIndex++;
            g_iUart2Count++;
        }
#endif
    }

    TracerInfo("UART Loopback Single Config Done!\r\n");
}

void TEST_UartLoopBack(uint32_t iCount)
{
    uint16_t iConfigIdx = 0, iConfigNum = 0;
    uint16_t iRetry = 0;
    uint32_t iIndex = 0;
    uint32_t dwTemp = 0;

    TEST_UartInit();
    iConfigNum = test_UartConigInit();

    TracerInfo("UART loopback test...\n");

    for (iConfigIdx=0; iConfigIdx<iConfigNum; iConfigIdx++)
    {
        iIndex = 0;
        iRetry = 100;

        do
        {
#ifdef TEST_UART0_BUS
            if(g_iuart0Done)
            {
                TestUart0.dwConfig = g_dwaUartConfig[iConfigIdx];
                drvi_UartConfigSet(&TestUart0);
            }
            else
                TracerInfo("UART Initial Waiting for UART0 Done!\r\n");
#endif
#ifdef TEST_UART1_BUS
            if(g_iuart1Done)
            {
                TestUart1.dwConfig = g_dwaUartConfig[iConfigIdx];
                drvi_UartConfigSet(&TestUart1);
            }
            else
                TracerInfo("UART Initial Waiting for UART1 Done!\r\n");
#endif
#ifdef TEST_UART2_BUS
            if(g_iuart2Done)
            {
                TestUart2.dwConfig = g_dwaUartConfig[iConfigIdx];
                drvi_UartConfigSet(&TestUart2);
            }
            else
                TracerInfo("UART Initial Waiting for UART2 Done!\r\n");
#endif
        } while(--iRetry);

#ifdef TEST_UART0_BUS
        if ((!iRetry) && (!g_iuart0Done))
        {
            TracerInfo("UART0 Died, Re-init and Re-active!\r\n");
            TestUart0.dwConfig = g_dwaUartConfig[iConfigIdx];
            drvi_UartConfigSet(&TestUart0);
            g_iuart0Done = 1;
        }
#endif
#ifdef TEST_UART1_BUS
        if ((!iRetry) && (!g_iuart1Done))
        {
            TracerInfo("UART1 Died, Re-init and Re-active!\r\n");
            TestUart1.dwConfig = g_dwaUartConfig[iConfigIdx];
            drvi_UartConfigSet(&TestUart1);
            g_iuart1Done = 1;
        }
#endif
#ifdef TEST_UART2_BUS
        if ((!iRetry) && (!g_iuart2Done))
        {
            TracerInfo("UART2 Died, Re-init and Re-active!\r\n");
            TestUart2.dwConfig = g_dwaUartConfig[iConfigIdx];
            drvi_UartConfigSet(&TestUart2);
            g_iuart2Done = 1;
        }
#endif

        while(iIndex<iCount)
        {
            //Generate a complicated number
            dwTemp = iIndex*iIndex*3957;

#ifdef TEST_UART0_BUS
            if (g_iuart0Done)
            {
                g_iuart0Done = 0;
                memset(g_caUart0TxBuffer, 0, UART_BUFFER_SIZE);
                memset(g_caUart0RxBuffer, 0, UART_BUFFER_SIZE);
                sprintf(g_caUart0TxBuffer, "%lu%c", dwTemp, '\0');
                drvi_UartReceive(TEST_UART0_BUS, (UINT8 *)g_caUart0RxBuffer, UART_BUFFER_SIZE);
                drvi_UartTx(TEST_UART0_BUS, (const UINT8 *)g_caUart0TxBuffer, UART_BUFFER_SIZE);
                iIndex++;
                g_iUart0Count++;
            }
#endif
#ifdef TEST_UART1_BUS
            if (g_iuart1Done)
            {
                g_iuart1Done = 0;
                memset(g_caUart1TxBuffer, 0, UART_BUFFER_SIZE);
                memset(g_caUart1RxBuffer, 0, UART_BUFFER_SIZE);
                sprintf(g_caUart1TxBuffer, "%lu%c", dwTemp, '\0');
                drvi_UartReceive(TEST_UART1_BUS, (UINT8 *)g_caUart1RxBuffer, UART_BUFFER_SIZE);
                drvi_UartTx(TEST_UART1_BUS, (const UINT8 *)g_caUart1TxBuffer, UART_BUFFER_SIZE);
                iIndex++;
                g_iUart1Count++;
            }
#endif
#ifdef TEST_UART2_BUS
            if (g_iuart2Done)
            {
                g_iuart2Done = 0;
                memset(g_caUart2TxBuffer, 0, UART_BUFFER_SIZE);
                memset(g_caUart2RxBuffer, 0, UART_BUFFER_SIZE);
                sprintf(g_caUart2TxBuffer, "%lu%c", dwTemp, '\0');
                drvi_UartReceive(TEST_UART2_BUS, (UINT8 *)g_caUart2RxBuffer, UART_BUFFER_SIZE);
                drvi_UartTx(TEST_UART2_BUS, (const UINT8 *)g_caUart2TxBuffer, UART_BUFFER_SIZE);
                iIndex++;
                g_iUart2Count++;
            }
#endif
        }
#ifdef TEST_UART0_BUS
        TracerInfo("UART Config[0x%x] Error[u0:%d] Count[u0:%lu]\n", \
                                          g_dwaUartConfig[iConfigIdx], \
                                          g_iUart0Error, g_iUart0Count);
#endif
#ifdef TEST_UART1_BUS
        TracerInfo("UART Config[0x%x] Error[u1:%d] Count[u1:%lu]\n", \
                                          g_dwaUartConfig[iConfigIdx], \
                                          g_iUart1Error, g_iUart1Count);
#endif
#ifdef TEST_UART2_BUS
        TracerInfo("UART Config[0x%x] Error[u2:%d] Count[u2:%lu]\n", \
                                          g_dwaUartConfig[iConfigIdx], \
                                          g_iUart2Error, g_iUart2Count);
#endif
    }

    TracerInfo("UART loopback test Done...\n");
}
#endif //TEST_AES
