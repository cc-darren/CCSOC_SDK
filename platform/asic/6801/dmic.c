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
*  dmic.c
*
*  Project:
*  --------
*  cc6801
*
*  Description:
*  ------------
*  This is DMIC driver
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

volatile uint32_t DMIC_INTR = 0;

void DMIC_IRQHandler(void)
{
    wr(DMIC_ADDR_BASE+0x00, 1<<16);     // clear interrupt
    DMIC_INTR = 1;
}
