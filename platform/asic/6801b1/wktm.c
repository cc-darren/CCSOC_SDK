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
*  wktm.c
*
*  Project:
*  --------
*  cc6801
*
*  Description:
*  ------------
*  This is Timer0/Timer1 driver
*
*  Author:
*  -------
*  CloudChip
*
*===========================================================================/
*  20170206 PAT initial version
******************************************************************************/
#include <stdio.h>
#include "wktm.h"
#include "drvi_wktm.h"

static T_wktmPort  g_taWktm[WKTM_TOTAL];

void WKTM0_IRQHandler(void)
{
    //drvi_GpioWrite(GPIO_PIN_38, 1);
    regWKTM0->bf.intSts = 1;    // clear interrupt
    
    if(NULL != g_taWktm[WKTM_0].fpCallback)
    {
        g_taWktm[WKTM_0].fpCallback((void*)NULL);
    }
    //drvi_GpioWrite(GPIO_PIN_38, 0);
}

void WKTM1_IRQHandler(void)
{
    regWKTM1->bf.intSts = 1;    // clear interrupt
    
    if(NULL != g_taWktm[WKTM_1].fpCallback)
    {
        g_taWktm[WKTM_1].fpCallback((void*)NULL);
    }
}

void cc6801_wktmRegisterCallback(E_wktmTotal bPort, T_callback tCB)
{
    if(bPort < WKTM_TOTAL)
    {
        g_taWktm[bPort].fpCallback = tCB;
    }
    else
    {
        //return error;
    }
        
}

void cc6801_wktmLoadPrescaler(E_wktmTotal bPort, uint32_t prescaler)
{
    //0 and 1 : no pre-scaler. 2~255 : pre-scaler
    if(bPort < WKTM_TOTAL)
    {
        g_taWktm[bPort].pReg->bf.prescaler = prescaler;
    }
    else
    {
        //return error;
    }
        
}

void cc6801_wktmCounterLoad(E_wktmTotal bPort, uint32_t cnt)
{
    if(bPort < WKTM_TOTAL)
    {
        g_taWktm[bPort].pReg->dw.lowCounter = cnt;
    }
    else
    {
        //return error;
    }
}

void cc6801_wktmCounterClear(E_wktmTotal bPort)
{
    if(bPort < WKTM_TOTAL)
    {
        g_taWktm[bPort].pReg->bf.clear = 1;
        g_taWktm[bPort].pReg->bf.clear = 0;
        //Keep clear bit to 1 to make counter value always 0
        //start timer API will clear clear_bit
    }
    else
    {
        //return error;
    }
}

void cc6801_wktmCounterGet(E_wktmTotal bPort, uint32_t *cnt)
{
    if(bPort < WKTM_TOTAL)
    {
        *cnt = g_taWktm[bPort].pReg->dw.counter;
    }
    else
    {
        //return error;
    }
}

void cc6801_wktmStart(E_wktmTotal bPort)
{
    if(bPort < WKTM_TOTAL)
    {
        g_taWktm[bPort].pReg->bf.intEn = 1;                 //enable interrupt;
        g_taWktm[bPort].pReg->dw.lowCounter = 0xFFFFFF;     //default load 24bit counter to match design
        g_taWktm[bPort].pReg->bf.clear = 0;
        g_taWktm[bPort].pReg->bf.repeat = 0;                //start from repeat mode
        g_taWktm[bPort].pReg->bf.enable = 1;
    }
    else
    {
        //return error;
    }
}

void cc6801_wktmStop(E_wktmTotal bPort)
{
    if(bPort < WKTM_TOTAL)
    {
        g_taWktm[bPort].pReg->bf.enable = 0;
        g_taWktm[bPort].pReg->bf.intEn = 0;         //default disable interrupt;
        g_taWktm[bPort].pReg->bf.intSts = 1;        //clear interrupt;
        g_taWktm[bPort].pReg->bf.repeat = 0;        //default one-shot
    }
    else
    {
        //return error;
    }
    
    //Note: stop timer won't clear counter value
}

void cc6801_wktmInit(E_wktmTotal bPort)
{
    if(bPort < WKTM_TOTAL)
    {
        g_taWktm[bPort].pReg = (U_regPWMWKTM*)((uint32_t)regWKTM0+(bPort*0x100));
    
        g_taWktm[bPort].pReg->bf.enable = 0;
        g_taWktm[bPort].pReg->bf.intEn = 0;         //default disable interrupt;
        g_taWktm[bPort].pReg->bf.intSts = 1;        //clear interrupt;
        g_taWktm[bPort].pReg->dw.highCounter = 0;
        g_taWktm[bPort].pReg->dw.lowCounter = 0xFFFFFF;
        g_taWktm[bPort].pReg->bf.pwmTimerSel = 1;   //select Timer
        g_taWktm[bPort].pReg->bf.clear = 1;
        g_taWktm[bPort].pReg->bf.repeat = 0;        //default one-shot
        g_taWktm[bPort].pReg->bf.pwmOutEn = 0;      //this is timer, no PWM output
    
        g_taWktm[bPort].pReg->bf.prescaler = 0;     //default to highest clock
    
        //default NULL for callback, app must register callback function "AFTER" drvi init
        g_taWktm[bPort].fpCallback = NULL;
    }
    else
    {
        //return error;
    }
        
}


