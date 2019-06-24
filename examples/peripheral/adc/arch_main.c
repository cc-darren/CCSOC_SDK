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
#include <stdlib.h>
#include <string.h>
#include "ll.h"
#include "tracer.h"
#include "project.h"
#include "drvi_init.h"
#include "drvi_clock.h"
#include "drvi_adc.h"

extern void sys_InitMain(void);
extern void cc6801_ClockDelayMs(volatile uint32_t);

/*
 * MAIN FUNCTION
 ****************************************************************************************
 */
int main(void)
{
    uint32_t volt;
    uint32_t dwTestCntr = 0;

    //Must be first in main()
    sys_InitMain();

    //start interrupt handling
    GLOBAL_INT_START(); 

    drvi_initialize();

    printf("== CC6801 - ADC - Start ==\r\n");

    /*******************************/
    /****** Application Start ******/
    /*******************************/
    drvi_AdcInit();

    while(1)
    {
        drvi_AdcEnable();
        drvi_ClockDelayMs(10);
        drvi_AdcVbatt(&volt);

        printf("[A:%04d] Vbat = %12d\r\n", ++dwTestCntr, volt);

        drvi_ClockDelayMs(1000);
    }
}
