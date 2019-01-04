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

#include <stdio.h>
#include "cc6801_reg.h"
#include "drvi_wdt.h"

#define WATCHDOG_CLK_32K 32768

volatile uint32_t WDT_INTR = 0;
T_callback g_fpWdtCallback;

void WDT_IRQHandler(void)
{
    regWDT->dw.interrupt |= WDT_INT_CLEAR_MASK;   //clear interrupt;

    if(NULL != g_fpWdtCallback)
        g_fpWdtCallback((void*)NULL);

    WDT_INTR = 1;
}

void cc6801_WdtFeed(void)
{
    regWDT->dw.ctrl |= WDT_DISABLE_BIT;
    regWDT->dw.ctrl &= ~WDT_DISABLE_BIT;
}

void cc6801_WdtDisable(void)
{
    uint32_t dwIntStat = 0;

    NVIC_DisableIRQ(WDT_IRQn);
    regWDT->dw.ctrl |= WDT_DISABLE_BIT;

    dwIntStat = regWDT->dw.interrupt;
    dwIntStat &= ~WDT_INT_ENABLE_MASK;    //disable interrupt
    dwIntStat |= WDT_INT_CLEAR_MASK;      //clear interrupt

    regWDT->dw.interrupt = dwIntStat;
}

void cc6801_WdtRstSet(uint32_t dwReload)
{
    uint32_t dwWdtCount = 0;

    dwWdtCount =  (dwReload * WATCHDOG_CLK_32K) / 1000;
    //Workaround
    //6801b0 clear interrupt status will reset counter
    //intCounter and rst Counter should be the same
    regWDT->dw.intCounter = dwWdtCount;
    regWDT->dw.rstCounter = dwWdtCount;

    regWDT->dw.ctrl |= WDT_RESET_ENABLE_MASK;
}

void cc6801_WdtEnable(void)
{
    uint32_t dwIntStat = 0;

    dwIntStat = regWDT->dw.interrupt;

    regWDT->dw.ctrl |= WDT_DISABLE_BIT;
    regWDT->dw.ctrl &= ~WDT_DISABLE_BIT;

    dwIntStat |= (WDT_INT_CLEAR_MASK |    //clear interrupt
                  WDT_INT_ENABLE_MASK);   //enable interrupt

    regWDT->dw.interrupt = dwIntStat;

    NVIC_EnableIRQ(WDT_IRQn);
}

void cc6801_WdtRegisterCallback(uint32_t dwReload, T_callback tCB)
{
    uint32_t dwWdtCount = 0;

    dwWdtCount =  (dwReload * WATCHDOG_CLK_32K) / 1000;
    regWDT->dw.intCounter = dwWdtCount;

    if(tCB)
        g_fpWdtCallback = tCB;
}

void cc6801_WdtInit(void)
{
    regAOCKGEN->clkEn |= WDT_CLK_ENABLE_MASK;
    regAOCKGEN->swReset |= WDT_RST_DISABLE_MASK;

    regWDT->dw.ctrl = (WDT_NO_PRESCALER_MASK |           //default to highest clock
                       WDT_RESET_DISABLE_MASK |
                       WDT_SELECT_MASK |                 //select watchdog
                       WDT_DISABLE_MASK);
}

