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
#include <stdlib.h>
#include <string.h>
#include "test.h"
#include "tracer.h"

#if (TEST_UARTRX)
/******************************************************************************
Declaration of External Variables & Functions
******************************************************************************/

/******************************************************************************
Declaration of data structure
******************************************************************************/
static uint8_t g_bStringArray[] =
{
    'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z',
    'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z',
};

/******************************************************************************
Declaration of Global Variables & Functions
******************************************************************************/
static volatile uint16_t g_iuart0Done = 0;

/******************************************************************************
Declaration of static Global Variables & Functions
******************************************************************************/
static void test_Uart0EventHandler(T_UartEvent * p_event)
{
    g_iuart0Done = 1;
}

static void test_UartInit(void)
{
    drvi_UartRxDoneRegister(TRACER_IF_ID, test_Uart0EventHandler);
    g_iuart0Done = 1;
}

void test_UARTRX(void)
{
    __align(4) uint8_t baCharBuf[16];
    int iRxByte = 0;
    int iLastByte = 0;
    int iaResult[3] = {0};
    uint8_t baDebug[16];
    uint8_t *pSour, *pDest;

    test_UartInit();
    pDest = &g_bStringArray[0];
    iLastByte = sizeof(g_bStringArray);

    while(1)
    {
        iRxByte = (rand()%8)+1;
        g_iuart0Done = 0;
        drvi_UartReceive(TRACER_IF_ID, baCharBuf, iRxByte);
        while(!g_iuart0Done);

        pSour = baCharBuf;

        if (iRxByte < iLastByte)
        {
            iaResult[0] = memcmp(pSour, pDest, iRxByte);
            pDest += iRxByte;
            iLastByte -= iRxByte;
        }
        else
        {
            int i2ndSize = iRxByte - iLastByte;
            iaResult[1] = memcmp(pSour, pDest, iLastByte);
            pSour += iLastByte;
            pDest = &g_bStringArray[0];
            iaResult[2] = memcmp(pSour, pDest, i2ndSize);
            pDest += i2ndSize;
            iLastByte = sizeof(g_bStringArray) - i2ndSize;
        }

        if (iaResult[0]|iaResult[1]|iaResult[2])
        {
            memcpy(baDebug, baCharBuf, sizeof(baCharBuf));
            TracerInfo("ERROR\r\n");
            while(1);
        }
    }
}

#endif //TEST_UARTRX



