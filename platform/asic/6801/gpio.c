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
*  gpio.c
*
*  Project:
*  --------
*  cc6801
*
*  Description:
*  ------------
*  This is GPIO driver
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

volatile uint32_t GPIO_INTR = 0;

void GPIO_IRQHandler(void)
{
    wr(GPIO_ADDR_BASE+0x10, 0x00000000);   // clear interrupt
    wr(GPIO_ADDR_BASE+0x3c, 0x00000000);   // clear interrupt
    GPIO_INTR = 1;
}
