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

#include "ll.h"
#include "drvi_init.h"
#include "tracer.h"
#include "project.h"
#include "drvi_rtc.h"

extern void sys_InitMain(void);
extern void cc6801_ClockDelayMs(volatile uint32_t);
static T_Rtc g_Rtc_time = {0};

uint8_t cTrigger = FALSE;

void intg_RtcHandler(void *para)
{
    g_Rtc_time = drvi_RtcGetTime();
    cTrigger = TRUE;
}

/*
 * MAIN FUNCTION
 ****************************************************************************************
 */
int main(void)
{
    T_RtcCallback tRtcHandler;
    
    //Must be first in main()
    sys_InitMain();

    //start interrupt handling
    GLOBAL_INT_START(); 

    drvi_initialize();

    printf("== CC6801 - RTC - Start ==\r\n");

    /*******************************/
    /****** Application Start ******/
    /*******************************/
    cTrigger = FALSE;
    
    drvi_RtcInit();
    
    tRtcHandler.day = NULL;
    tRtcHandler.hour = NULL;
    tRtcHandler.min = NULL;
    tRtcHandler.sec = intg_RtcHandler;
    drvi_RtcRegisterCallback(tRtcHandler);

    while(1)
    {
        if(cTrigger == TRUE)
        {
            cTrigger = FALSE;
            TracerInfo("[%02d:%02d:%02d] \r\n", g_Rtc_time.hour, g_Rtc_time.min, g_Rtc_time.sec);                    
        }
    }
    //return(0);
}
