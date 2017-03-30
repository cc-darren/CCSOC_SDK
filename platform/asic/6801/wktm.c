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
#include "global.h"
#include "cc6801_reg.h"

volatile uint32_t WKTM0_INTR = 0;
volatile uint32_t WKTM1_INTR = 0;

T_callback  timer0_callback_handler = NULL;
T_callback  timer1_callback_handler = NULL;

void WKTM0_IRQHandler(void)
{
    //wr(PWM0_ADDR_BASE+0x00, 1 << 16);     // clear interrupt
    regWKTM0->bf.intSts = 1;
    WKTM0_INTR = 1;
    
    if(NULL != timer0_callback_handler)
    {
        timer0_callback_handler((void*)NULL);
    }
}

void WKTM1_IRQHandler(void)
{
    //wr(PWM1_ADDR_BASE+0x00, 1 << 16);     // clear interrupt
    regWKTM1->bf.intSts = 1;
    WKTM1_INTR = 1;
    
    if(NULL != timer1_callback_handler)
    {
        timer1_callback_handler((void*)NULL);
    }
}

void cc6801_timer0_loadPrescaler(uint32_t prescaler)
{
    //0 and 1 : no pre-scaler. 2~255 : pre-scaler
    regWKTM0->bf.prescaler = prescaler;
}

void cc6801_timer1_loadPrescaler(uint32_t prescaler)
{
    //0 and 1 : no pre-scaler. 2~255 : pre-scaler
    regWKTM1->bf.prescaler = prescaler;
}

void cc6801_timer0_counterLoad(uint32_t cnt)
{
    regWKTM0->dw.highCounter = cnt;
}

void cc6801_timer1_counterLoad(uint32_t cnt)
{
    regWKTM1->dw.highCounter = cnt;
}

void cc6801_timer0_init(T_callback handler)
{
    regWKTM0->bf.intSts = 1;        //clear interrupt;
    regWKTM0->bf.intEn = 0;         //default disable interrupt;
    regWKTM0->dw.highCounter = 0;
    regWKTM0->dw.lowCounter = 0;
    regWKTM0->bf.pwmTimerSel = 1;   //select Timer
    regWKTM0->bf.clear = 1;
    regWKTM0->bf.enable = 0;
    regWKTM0->bf.repeat = 1;        //default one-shot
    regWKTM0->bf.pwmOutEn = 0;      //this is timer, no PWM output
    
    regWKTM0->bf.prescaler = 0;     //default to highest clock
    
    //register timer0 callback function
    timer0_callback_handler = handler;
}

void cc6801_timer1_init(T_callback handler)
{
    regWKTM1->bf.intSts = 1;        //clear interrupt;
    regWKTM1->bf.intEn = 0;         //default disable interrupt;
    regWKTM1->dw.highCounter = 0;
    regWKTM1->dw.lowCounter = 0;
    regWKTM1->bf.pwmTimerSel = 1;   //select Timer
    regWKTM1->bf.clear = 1;
    regWKTM1->bf.enable = 0;
    regWKTM1->bf.repeat = 1;        //default one-shot
    regWKTM1->bf.pwmOutEn = 0;      //this is timer, no PWM output
    
    //register timer1 callback function
    timer1_callback_handler = handler;
}

void cc6801_timer0_counterClear(void)
{
    regWKTM1->bf.clear = 1;
    //Keep clear bit to 1 to make counter value always 0
    //start timer API will clear clear_bit
}

void cc6801_timer0_counterGet(uint32_t *cnt)
{
    *cnt = regWKTM0->dw.counter;
}

void cc6801_timer0_start(void)
{
    regWKTM0->bf.intEn = 1;                 //enable interrupt;
    regWKTM0->dw.highCounter = 0xFFFFFF;    //default load 24bit counter to match Nordic design
    regWKTM0->bf.clear = 0;
    regWKTM0->bf.enable = 1;
    regWKTM0->bf.repeat = 0;                //start from repeat mode
}

void cc6801_timer0_stop(void)
{
    regWKTM1->bf.intSts = 1;        //clear interrupt;
    regWKTM1->bf.intEn = 0;         //default disable interrupt;
    regWKTM1->bf.enable = 0;
    regWKTM1->bf.repeat = 1;        //default one-shot
    
    //Note: stop timer won't clear counter value
}



