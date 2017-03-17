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
*  spim.c
*
*  Project:
*  --------
*  cc6801
*
*  Description:
*  ------------
*  This is SPI master driver
*
*  Author:
*  -------
*  PATTAYA
*
*===========================================================================/
*  20170203 PAT initial version
******************************************************************************/
#include "global.h"
#include "cc6801_reg.h"

volatile uint32_t SPI0_M_INTR = 0;
volatile uint32_t SPI1_M_INTR = 0;
volatile uint32_t SPI2_M_INTR = 0;

void SPI0_M_IRQHandler(void)
{
    wr(SPI0_ADDR_BASE+0x00, 0x00010001);
    SPI0_M_INTR = 1;
}

void SPI1_M_IRQHandler(void)
{
    wr(SPI1_ADDR_BASE+0x00, 0x00010001);
    SPI1_M_INTR = 1;
}

void SPI2_M_IRQHandler(void)
{
    wr(SPI2_ADDR_BASE+0x00, 0x00010001);
    SPI2_M_INTR = 1;
}
