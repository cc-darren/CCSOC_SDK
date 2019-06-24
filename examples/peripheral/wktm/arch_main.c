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
#include "drvi_wktm.h"

extern void sys_InitMain(void);
uint32_t g_Wktm_Trigger = FALSE;

static void test_WktmHandler(void *event)
{
    g_Wktm_Trigger = TRUE;
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

    printf("== CC6801 - WKTM - Start ==\r\n");

    g_Wktm_Trigger = TRUE;

    /*******************************/
    /****** Application Start ******/
    /*******************************/
    while(1)
    {
        if(g_Wktm_Trigger == TRUE)
        {
            g_Wktm_Trigger = FALSE;

            cc6801_wktmInit(WKTM_0);
            drvi_wktmRegisterCallback(WKTM_PORT_0, test_WktmHandler);
            NVIC_EnableIRQ(WKTM0_IRQn);

            drvi_wktmStart(WKTM_PORT_0);
            drvi_wktmCounterLoad(WKTM_PORT_0, 32768);
            drvi_wktmCounterClear(WKTM_PORT_0);

            printf("[W:%04d] wktm trigger\r\n", ++dwCounter);
        }
    }
}
