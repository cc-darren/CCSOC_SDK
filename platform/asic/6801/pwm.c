/******************************************************************************
*  Copyright 2017, CloudChip, Inc.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of CloudChip, Inc. (C) 2017
******************************************************************************/

/******************************************************************************
*  Filename:
*  ---------
*  pwm.c
*
*  Project:
*  --------
*  cc6801
*
*  Description:
*  ------------
*  This is PWM0/PWM1 driver
*
*  Author:
*  -------
*  CloudChip
*
*===========================================================================/
*  20170206 PAT initial version
******************************************************************************/
#include <stdio.h>
#include "global.h"
#include "cc6801_reg.h"

volatile uint32_t PWM0_INTR = 0;
volatile uint32_t PWM1_INTR = 0;

T_callback  pwm0_callback_handler = NULL;
T_callback  pwm1_callback_handler = NULL;

void PWM0_IRQHandler(void)
{
    //wr(PWM0_ADDR_BASE+0x00, 1 << 16);     // clear interrupt
    regPWM0->bf.intSts = 1;
    PWM0_INTR = 1;

    if(NULL != pwm0_callback_handler)
    {
        pwm0_callback_handler((void*)NULL);
    }
}

void PWM1_IRQHandler(void)
{
    //wr(PWM1_ADDR_BASE+0x00, 1 << 16);     // clear interrupt
    regPWM1->bf.intSts = 1;
    PWM1_INTR = 1;

    if(NULL != pwm1_callback_handler)
    {
        pwm1_callback_handler((void*)NULL);
    }
}

void cc6801_pwm0_loadPrescaler(uint32_t prescaler)
{
    //0 and 1 : no pre-scaler. 2~255 : pre-scaler
    regPWM0->bf.prescaler = prescaler;
}

void cc6801_pwm1_loadPrescaler(uint32_t prescaler)
{
    //0 and 1 : no pre-scaler. 2~255 : pre-scaler
    regPWM1->bf.prescaler = prescaler;
}

void cc6801_pwm0_duty(uint32_t percentage)
{
    //only support duty 1~99
    if (percentage > 99) percentage = 99;
    if (percentage == 0) percentage = 1;

    percentage = 100 - percentage;
    regPWM0->dw.highCounter = percentage;
    regPWM0->dw.lowCounter = 100;
}

void cc6801_pwm1_duty(uint32_t percentage)
{
    //only support duty 1~99
    if (percentage > 99) percentage = 99;
    if (percentage == 0) percentage = 1;

    percentage = 100 - percentage;
    regPWM1->dw.highCounter = percentage;
    regPWM1->dw.lowCounter = 100;
}

void cc6801_pwm0_init(T_callback handler)
{
    regPWM0->bf.enable = 0;
    regPWM0->bf.intEn = 0;         //default disable interrupt;
    regPWM0->bf.intSts = 1;        //clear interrupt;
    regPWM0->dw.highCounter = 50;   //default high period = 50, duty = 50/50 when low period = 100
    regPWM0->dw.lowCounter = 100;   //default low period = 100, means input clock will always be divided by 100
    regPWM0->bf.pwmTimerSel = 0;   //select PWM
    regPWM0->bf.clear = 1;
    regPWM0->bf.repeat = 0;        //default output always (0:repeat, 1:single)
    regPWM0->bf.pwmOutEn = 1;

    regPWM0->bf.prescaler = 0;     //default to highest clock

    //register pwm0 callback function
    pwm0_callback_handler = handler;
}

void cc6801_pwm1_init(T_callback handler)
{
    regPWM1->bf.enable = 0;
    regPWM1->bf.intEn = 0;         //default disable interrupt;
    regPWM1->bf.intSts = 1;        //clear interrupt;
    regPWM1->dw.highCounter = 50;   //default high period = 50, duty = 50/50 when low period = 100
    regPWM1->dw.lowCounter = 100;   //default low period = 100, means input clock will always be divided by 100
    regPWM1->bf.pwmTimerSel = 0;   //select PWM
    regPWM1->bf.clear = 1;
    regPWM1->bf.repeat = 0;        //default output always (0:repeat, 1:single)
    regPWM1->bf.pwmOutEn = 1;

    regPWM1->bf.prescaler = 0;     //default to highest clock

    //register pwm1 callback function
    pwm1_callback_handler = handler;
}

void cc6801_pwm0_counterClear(void)
{
    regPWM0->bf.clear = 1;
    //Keep clear bit to 1 to make counter value always 0
    //start pwm API will clear clear_bit
}

void cc6801_pwm0_counterGet(uint32_t *cnt)
{
    *cnt = regPWM0->dw.counter;
}

void cc6801_pwm0_start(void)
{
    regPWM0->bf.intEn = 0;                 //disable interrupt;
    regPWM0->bf.clear = 0;
    regPWM0->bf.enable = 1;
}

void cc6801_pwm0_stop(void)
{
    regPWM0->bf.enable = 0;
    regPWM0->bf.intEn = 0;         //default disable interrupt;
    regPWM0->bf.intSts = 1;        //clear interrupt;

    //Note: stop pwm won't clear counter value
}



