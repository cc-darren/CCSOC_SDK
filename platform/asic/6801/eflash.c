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
*  eflash.c
*
*  Project:
*  --------
*  cc6801
*
*  Description:
*  ------------
*  This is EFLASH driver
*
*  Author:
*  -------
*  CloudChip
*
*===========================================================================/
*  20170206 PAT initial version
******************************************************************************/
#include "compiler.h"
#include "register.h"
#include "ll.h"
#include "addr_map.h"

volatile uint32_t EFLASH_INTR = 0;

void EFLASH_IRQHandler(void)
{
    wr(EF_INTERRUPT_REG, 0x00030003);   // clear interrupt
    EFLASH_INTR = 1;
}
