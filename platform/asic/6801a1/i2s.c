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
*  i2s.c
*
*  Project:
*  --------
*  cc6801
*
*  Description:
*  ------------
*  This is I2S driver
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

volatile uint32_t I2S_RXDMA_INTR = 0;
volatile uint32_t I2S_TXDMA_INTR = 0;

void I2S_RXDMA_IRQHandler(void)
{
    uint32_t rdata;
    rd(I2S_ADDR_BASE+0x20, rdata);
    wr(I2S_ADDR_BASE+0x20, (rdata | 1<<16) & ~(1<<0));   // clear interrupt
    I2S_RXDMA_INTR = 1;
}

void I2S_TXDMA_IRQHandler(void)
{
    uint32_t rdata;
    rd(I2S_ADDR_BASE+0x20, rdata);
    wr(I2S_ADDR_BASE+0x20, (rdata | 1<<17) & ~(1<<1));   // clear interrupt
    I2S_TXDMA_INTR = 1;
}
