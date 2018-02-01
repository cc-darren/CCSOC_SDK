/* Copyright (c) 2018 Cloudchip, Inc. All Rights Reserved.
 *
 * The information contained herein is property of Cloudchip, Inc.
 * Terms and conditions of usage are described in detail in CLOUDCHIP
 * STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information.
 * NO WARRANTY of ANY KIND is provided. This heading must NOT be removed 
 * from the file.
 */
 
#include "global.h"
#include "project.h"
#include "drvi_pwm.h"

#if defined(PWM_INUSE) && PWM_INUSE
void drvi_pwmInit(void)
{
    #if defined(PWM0_INUSE) && PWM0_INUSE
    cc6801_pwmInit(PWM_0);
    #endif
    
    #if defined(PWM1_INUSE) && PWM1_INUSE
    cc6801_pwmInit(PWM_1);
    #endif
}
#endif


void pwm_start_test(void)
{
    uint32_t i;
    while(1)
    {
        for(i=1;i<100;i++)
        {
            drvi_pwmStop(PWM_0);
            drvi_pwmLoadPrescaler(PWM_0,i);
            drvi_pwmDuty(PWM_0,50);
            drvi_pwmStart(PWM_0);
            delayns(100000);
        }
        for(i=99;i>0;i--)
        {
            drvi_pwmStop(PWM_0);
            drvi_pwmLoadPrescaler(PWM_0,i);
            drvi_pwmDuty(PWM_0,50);
            drvi_pwmStart(PWM_0);
            delayns(100000);
        }
        for(i=1;i<100;i++)
        {
            drvi_pwmStop(PWM_0);
            drvi_pwmLoadPrescaler(PWM_0,0);
            drvi_pwmDuty(PWM_0,i);
            drvi_pwmStart(PWM_0);
            delayns(100000);
        }
        for(i=99;i>0;i--)
        {
            drvi_pwmStop(PWM_0);
            drvi_pwmLoadPrescaler(PWM_0,0);
            drvi_pwmDuty(PWM_0,i);
            drvi_pwmStart(PWM_0);
            delayns(100000);
        }
    }
}

