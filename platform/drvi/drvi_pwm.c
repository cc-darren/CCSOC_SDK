/**
 ****************************************************************************************
 *
 * @file drvi_pwm.c
 *
 * @brief HW PWM interface for all SOC.
 *
 * Copyright (C) CloudChip 2017-2019
 *
 *
 ****************************************************************************************
 */
#include "global.h"
#include "drvi_pwm.h"
#include "cc6801_reg.h"



void pwm_start_test(void)
{
    uint32_t i;
    while(1)
    {
        for(i=1;i<100;i++)
        {
            drvi_pwm0_stop();
            drvi_pwm0_loadPrescaler(i);
            drvi_pwm0_duty(50);
            drvi_pwm0_start();
            delayns(100000);
        }
        for(i=99;i>0;i--)
        {
            drvi_pwm0_stop();
            drvi_pwm0_loadPrescaler(i);
            drvi_pwm0_duty(50);
            drvi_pwm0_start();
            delayns(100000);
        }
        for(i=1;i<100;i++)
        {
            drvi_pwm0_stop();
            drvi_pwm0_loadPrescaler(0);
            drvi_pwm0_duty(i);
            drvi_pwm0_start();
            delayns(100000);
        }
        for(i=99;i>0;i--)
        {
            drvi_pwm0_stop();
            drvi_pwm0_loadPrescaler(0);
            drvi_pwm0_duty(i);
            drvi_pwm0_start();
            delayns(100000);
        }
    }
}

