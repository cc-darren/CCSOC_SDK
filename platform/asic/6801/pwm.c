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
*  This is PWM driver
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

volatile uint32_t PWM0_INTR = 0;
volatile uint32_t PWM1_INTR = 0;

void PWM0_IRQHandler(void)
{
    wr(PWM0_ADDR_BASE+0x00, 1 << 16);     // clear interrupt
    PWM0_INTR = 1;
}

void PWM1_IRQHandler(void)
{
    wr(PWM1_ADDR_BASE+0x00, 1 << 16);     // clear interrupt
    PWM1_INTR = 1;
}
