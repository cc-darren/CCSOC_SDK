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
*  i2c.c
*
*  Project:
*  --------
*  cc6801
*
*  Description:
*  ------------
*  This is I2C driver
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

volatile uint32_t I2C0_M_INTR = 0;
volatile uint32_t I2C1_M_INTR = 0;

void I2C0_M_IRQHandler(void)
{
    wr(I2C0_ADDR_BASE+0, 0x00010007);   // clear interrupt
    I2C0_M_INTR = 1;
}

void I2C1_M_IRQHandler(void)
{
    wr(I2C1_ADDR_BASE+0, 0x00010007);   // clear interrupt
    I2C1_M_INTR = 1;
}
