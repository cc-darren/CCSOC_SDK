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

#if (TEST_CLOCK)
#include "drvi_gpio.h"
#include "drvi_clock.h"
#include "clock.h"
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

/******************************************************************************
Declaration of static Global Variables & Functions
******************************************************************************/

int TEST_DynamicClock(uint32_t dwCount)
{
    TracerInfo("Clock dynamic change test...\n");

    do
    {
        drvi_ClockSysClkAdjust(CLOCK_16);
        for(uint32_t i=0;i<dwCount;i++)
        {
            drvi_GpioWrite(8, 0);
            drvi_ClockDelayUs(10);
            drvi_GpioWrite(8, 1);
            drvi_ClockDelayUs(10);
        }

        drvi_ClockSysClkAdjust(CLOCK_24);
        for(uint32_t i=0;i<dwCount;i++)
        {
            drvi_GpioWrite(8, 0);
            drvi_ClockDelayUs(10);
            drvi_GpioWrite(8, 1);
            drvi_ClockDelayUs(10);
        }

        drvi_ClockSysClkAdjust(CLOCK_32);
        for(uint32_t i=0;i<dwCount;i++)
        {
            drvi_GpioWrite(8, 0);
            drvi_ClockDelayUs(10);
            drvi_GpioWrite(8, 1);
            drvi_ClockDelayUs(10);
        }

        drvi_ClockSysClkAdjust(CLOCK_48);
        for(uint32_t i=0;i<dwCount;i++)
        {
            drvi_GpioWrite(8, 0);
            drvi_ClockDelayUs(10);
            drvi_GpioWrite(8, 1);
            drvi_ClockDelayUs(10);
        }

        drvi_ClockSysClkAdjust(CLOCK_64);
        for(uint32_t i=0;i<dwCount;i++)
        {
            drvi_GpioWrite(8, 0);
            drvi_ClockDelayUs(10);
            drvi_GpioWrite(8, 1);
            drvi_ClockDelayUs(10);
        }

    }while(0);

    TracerInfo("Clock dynamic change test Done...\n");
    return 0;
}

#endif //TEST_CLOCK
