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
#include "drvi_wdt.h"

extern void sys_InitMain(void);
uint8_t g_Wdt_Trigger = FALSE;

static void test_WdtHandler(void *event)
{
    g_Wdt_Trigger = TRUE;
} 


/*
 * MAIN FUNCTION
 ****************************************************************************************
 */
int main(void)
{
    uint32_t dwCounter = 0;
    
    //Must be first in main()
    sys_InitMain();

    //start interrupt handling
    GLOBAL_INT_START(); 

    drvi_initialize();

    printf("== CC6801 - WDT - Start ==\r\n");

    g_Wdt_Trigger = TRUE;
    
    /*******************************/
    /****** Application Start ******/
    /*******************************/
    while(1)
    {
        if(g_Wdt_Trigger == TRUE)
        {
            g_Wdt_Trigger = FALSE;
            drvi_WdtRegisterCallback(1000, test_WdtHandler);            
            drvi_WdtEnable();
            TracerInfo("[W:%04d] watchdog trigger\r\n", ++dwCounter);
        }
    }
    //return(0);
}
