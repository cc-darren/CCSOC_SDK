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
*  wdt.c
*
*  Project:
*  --------
*  cc6801
*
*  Description:
*  ------------
*  This is WDT driver
*
*  Author:
*  -------
*  CloudChip
*
*===========================================================================/
*  20170206 PAT initial version
******************************************************************************/
#include "global.h"
#include "cc6801_reg.h"

volatile uint32_t WDT_INTR = 0;

void WDT_IRQHandler(void)
{
    regWDT->bf.intSts = 1;        //clear interrupt;
    WDT_INTR = 1;
}

void cc6801_wdt_feed(void)
{
    regWDT->bf.wdtEn = 1;
    regWDT->bf.wdtEn = 0;
}

void cc6801_wdt_enable(void)
{
    regWDT->bf.wdtEn = 1;
    regWDT->bf.wdtEn = 0;

    regWDT->bf.intSts = 1;        //clear interrupt;
    regWDT->bf.intEn = 1;         //default enable interrupt;
}

void cc6801_wdt_init(uint32_t reload_value)
{
    regWDT->dw.intCounter = (reload_value * 32768) / 1000;
    regWDT->dw.rstCounter = (reload_value * 32768) / 1000;

    regWDT->bf.prescaler = 1;     //default to highest clock
    regWDT->bf.timerSel = 0;      //select watchdog

    NVIC_EnableIRQ(WDT_IRQn);
}

