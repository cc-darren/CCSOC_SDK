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
*  dmu.c
*
*  Project:
*  --------
*  cc6801
*
*  Description:
*  ------------
*  This is DMU driver
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

volatile uint32_t DMU_INTR = 0;

void DMU_IRQHandler(void)
{
    int i = 0;
    uint32_t idata;
    //__dsb(0xF);
    for (i=0; i<5; i++) {
        rd(DMU_ADDR_BASE+i*0x20+0x00, idata);

        if ((idata & (1<<16))>0) {
            wr(0x2000E00C, idata);
        } else {
            wr(0x2000E010, idata);
        }

        if (idata & (1<<16)) {
            wr(0x2000E014, idata);
        } else {
            wr(0x2000E018, idata);
        }

        if ((idata & (1<<16))>0) {
            wr(DMU_ADDR_BASE+i*0x20+0x00, (0x00000001 | 1 <<16)); 
            wr(DMU_ADDR_BASE+i*0x20+0x0c, (0x00000000 | 1 <<11 | 0 <<10));
            wr(0x2000E004, 0x01);
            wr(0x2000E008, i);
            break;
        }
    }
    wr(0x2000E01C, i);
    if (i==6) {
        wr(0x2000E004, 0x02);
    }
    DMU_INTR = 1;
}
