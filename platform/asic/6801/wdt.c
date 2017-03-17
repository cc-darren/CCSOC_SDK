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
    wr(WDT_ADDR_BASE+0x0, 0x00010000);   // clear interrupt
    WDT_INTR = 1;
}
