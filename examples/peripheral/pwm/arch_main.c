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
#include "drvi_gpio.h"
#include "drvi_pwm.h"
#include "tracer.h"
#include "project.h"

extern void sys_InitMain(void);
extern void cc6801_ClockDelayMs(volatile uint32_t);

/*
 * MAIN FUNCTION
 ****************************************************************************************
 */
int main(void)
{

    //Must be first in main()
    sys_InitMain();

    //start interrupt handling
    GLOBAL_INT_START(); 

    drvi_initialize();

    TracerInfo("== CC6801 - PWM - Start ==\r\n");

    /*******************************/
    /****** Application Start ******/
    /*******************************/

    cc6801_pwmInit(PWM_0);
    drvi_pwmStop(PWM_0);
    drvi_pwmLoadPrescaler(PWM_0, 1);
    drvi_pwmDuty(PWM_0, 2);
    drvi_pwmStart(PWM_0);

    cc6801_pwmInit(PWM_1);
    drvi_pwmStop(PWM_1);
    drvi_pwmLoadPrescaler(PWM_1, 1);
    drvi_pwmDuty(PWM_1, 3);
    drvi_pwmStart(PWM_1);

    while(1)
    {
    }
}
