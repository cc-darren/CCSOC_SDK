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
#include "pwm.h"

static T_pwmPort g_taPwm[PWM_TOTAL];

void PWM0_IRQHandler(void)
{
    regPWM0->bf.intSts = 1;        //clear interrupt

    if(NULL != g_taPwm[PWM_0].fpCallback)
    {
        g_taPwm[PWM_0].fpCallback((void*)NULL);
    }
}

void PWM1_IRQHandler(void)
{
    regPWM1->bf.intSts = 1;        //clear interrupt

    if(NULL != g_taPwm[PWM_1].fpCallback)
    {
        g_taPwm[PWM_1].fpCallback((void*)NULL);
    }
}


void cc6801_pwmRegisterCallback(E_pwmTotal bPort, T_callback tCB)
{
    if(bPort < PWM_TOTAL)
    {
        g_taPwm[bPort].fpCallback = tCB;
    }
    else
    {
        //return error;
    }

}

void cc6801_pwmLoadPrescaler(E_pwmTotal bPort, uint32_t prescaler)
{
    //0 and 1 : no pre-scaler. 2~255 : pre-scaler
    if(bPort < PWM_TOTAL)
    {
        g_taPwm[bPort].pReg->bf.prescaler = prescaler;
    }
    else
    {
        //return error;
    }
}

void cc6801_pwmDuty(E_pwmTotal bPort, uint32_t percentage)
{
    //only support duty 1~99
    if (percentage > 99) percentage = 99;
    if (percentage == 0) percentage = 1;

    percentage = 100 - percentage;

    if(bPort < PWM_TOTAL)
    {
        g_taPwm[bPort].pReg->dw.highCounter = percentage;
        g_taPwm[bPort].pReg->dw.lowCounter = 100;
    }
    else
    {
        //return error;
    }
}

void cc6801_pwmCounterClear(E_pwmTotal bPort)
{
    if(bPort < PWM_TOTAL)
    {
        g_taPwm[bPort].pReg->bf.clear = 1;
        //Keep clear bit to 1 to make counter value always 0
        //start pwm API will clear clear_bit
    }
    else
    {
        //return error;
    }
}

void cc6801_pwm0_counterGet(E_pwmTotal bPort, uint32_t *cnt)
{
    if(bPort < PWM_TOTAL)
    {
        *cnt = g_taPwm[bPort].pReg->dw.counter;
    }
    else
    {
        //return error;
    }
}

void cc6801_pwmStart(E_pwmTotal bPort)
{
    if(bPort < PWM_TOTAL)
    {
        g_taPwm[bPort].pReg->bf.intEn = 0;                 //disable interrupt;
        g_taPwm[bPort].pReg->bf.clear = 0;
        g_taPwm[bPort].pReg->bf.enable = 1;
    }
    else
    {
        //return error;
    }
}

void cc6801_pwmStop(E_pwmTotal bPort)
{
    if(bPort < PWM_TOTAL)
    {
        g_taPwm[bPort].pReg->bf.enable = 0;
        g_taPwm[bPort].pReg->bf.intEn = 0;         //default disable interrupt;
        g_taPwm[bPort].pReg->bf.pwmTimerSel = 1;
        g_taPwm[bPort].pReg->bf.pwmTimerSel = 0;

        g_taPwm[bPort].pReg->bf.intSts = 1;        //clear interrupt;
    }
    else
    {
        //return error;
    }

    //Note: stop pwm won't clear counter value
}


void cc6801_pwmInit(E_pwmTotal bPort)
{
    if(bPort < PWM_TOTAL)
    {
        g_taPwm[bPort].pReg = (U_regPWMWKTM*)((uint32_t)regPWM0+(bPort*0x100));

        g_taPwm[bPort].pReg->bf.enable = 0;
        g_taPwm[bPort].pReg->bf.intEn = 0;         //default disable interrupt;
        g_taPwm[bPort].pReg->bf.intSts = 1;        //clear interrupt;
        g_taPwm[bPort].pReg->dw.highCounter = 50;   //default high period = 50, duty = 50/50 when low period = 100
        g_taPwm[bPort].pReg->dw.lowCounter = 100;   //default low period = 100, means input clock will always be divided by 100
        g_taPwm[bPort].pReg->bf.pwmTimerSel = 0;   //select PWM
        g_taPwm[bPort].pReg->bf.clear = 1;
        g_taPwm[bPort].pReg->bf.repeat = 0;        //default output always (0:repeat, 1:single)
        //g_taPwm[bPort].pReg->bf.pwmOutEn = 1;

        g_taPwm[bPort].pReg->bf.prescaler = 0;     //default to highest clock
    }
    else
    {
        //return error;
    }
}


