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

#if (TEST_WDT)
#include "drvi_wdt.h"
#include "tracer.h"

/******************************************************************************
Declaration of External Variables & Functions
******************************************************************************/

/******************************************************************************
Declaration of data structure
******************************************************************************/


/******************************************************************************
Declaration of Global Variables & Functions
******************************************************************************/
volatile static int g_iWdtFeed = 0;

/******************************************************************************
Declaration of static Global Variables & Functions
******************************************************************************/

static void test_WdtHandler(void *event)
{
    g_iWdtFeed = 1;
}

void TEST_Watchdog(uint32_t iCount)
{
    TracerInfo("Watchdog test...\r\n");

    //drvi_WdtRegisterCallback(2000, test_WdtHandler);
    drvi_WdtRstSet(10000);
    drvi_WdtEnable();
    while(iCount)
    {
        if (g_iWdtFeed)
        {
            g_iWdtFeed = 0;
            drvi_WdtFeed();
            TracerInfo("Feed dog...\r\n");
            iCount--;
        }
    }

    TracerInfo("Watchdog test done, Reset after 10sec...\r\n");
    while(1)
    {
        if (g_iWdtFeed)
        {
            g_iWdtFeed = 0;
            TracerInfo("Warning!! reset count down...\r\n");
        }
    }
}

#endif //TEST_WDT
