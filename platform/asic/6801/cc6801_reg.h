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
*  cc6801_reg.h
*
*  Project:
*  --------
*  cc6801
*
*  Description:
*  ------------
*  This header file defines all CC6801 HW registers in detail.
*  When there is any changes(ECO) from CC6801, this file is needed to be reviewed.
*
*  Author:
*  -------
*  PATTAYA
*
*===========================================================================/
*  20170123 PAT fill in all register
*  20170120 PAT initial version
******************************************************************************/

#ifndef CC6801_H
#define CC6801_H

#ifdef __cplusplus
extern "C" {
#endif


struct S_regSCU
{
    //cancelled:0x00
    UINT32  padding1;
    //ADC parameter:0x04
    UINT32  padding2:8;
    UINT32  VBG12:1;
    UINT32  ADC:7;
    UINT32  padding3:5;
    UINT32  BRG_SEL:3;
    UINT32  padding4:5;
    UINT32  ADC0TS1:1;
    UINT32  PD_AD_ONLY:1;
    UINT32  PD:1;
    //PLL assert/de-assert:0x08
    UINT32  padding5:31;
    UINT32  PLL_reset:1;



};

struct S_regBLE
{
    //remap control
    UINT32  padding1:31;
    UINT32  Rmap_en:1;
    //remap start address
    UINT32  padding2:18;
    UINT32  Rmap_st_addr:14;
    //remap end address
    UINT32  padding3:18;
    UINT32  Rmap_end_addr:14;
    //DMA write address
    UINT32  padding4:15;
    UINT32  Dma_str_waddr:17;
    //DMA read address
    UINT32  padding5:15;
    UINT32  Dma_str_raddr:17;
};








#ifdef __cplusplus
}
#endif

#endif  /* CC6801_H */

