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

#include "cc6801_reg.h"
#include "drvi_wdt.h"

#define WATCHDOG_CLK_32K 32768

volatile uint32_t WDT_INTR = 0;

void WDT_IRQHandler(void)
{
    regWDT->bf.intSts = WDT_INT_CLEAR_BIT;        //clear interrupt;
    WDT_INTR = 1;
}

void cc6801_WdtFeed(void)
{
    regWDT->bf.wdtEn = WDT_DISABLE_BIT;
    regWDT->bf.wdtEn = WDT_ENABLE_BIT;
}

void cc6801_WdtDisable(void)
{
    uint32_t dwIntStat = 0;

    NVIC_DisableIRQ(WDT_IRQn);
    regWDT->bf.wdtEn = WDT_DISABLE_BIT;

    dwIntStat = regWDT->dw.interrupt;
    dwIntStat &= ~WDT_INT_ENABLE_MASK;    //disable interrupt
    dwIntStat |= WDT_INT_CLEAR_MASK;      //clear interrupt

    regWDT->dw.interrupt = dwIntStat;
}

void cc6801_WdtEnable(void)
{
    uint32_t dwIntStat = 0;

    dwIntStat = regWDT->dw.interrupt;

    regWDT->bf.wdtEn = WDT_DISABLE_BIT;
    regWDT->bf.wdtEn = WDT_ENABLE_BIT;

    dwIntStat |= (WDT_INT_CLEAR_MASK |    //clear interrupt
                  WDT_INT_ENABLE_MASK);   //enable interrupt

    regWDT->dw.interrupt = dwIntStat;
}

void cc6801_WdtInit(uint32_t dwReload)
{
    uint32_t dwWdtCount = 0;

    dwWdtCount =  (dwReload * WATCHDOG_CLK_32K) / 1000;
    regWDT->dw.intCounter = dwWdtCount;
    regWDT->dw.rstCounter = dwWdtCount;

    regWDT->dw.ctrl = (WDT_NO_PRESCALER_MASK |           //default to highest clock
                       WDT_RESET_DISABLE_MASK |
                       WDT_SELECT_MASK |                 //select watchdog
                       WDT_DISABLE_MASK);

    NVIC_EnableIRQ(WDT_IRQn);
}

