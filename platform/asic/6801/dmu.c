/* Copyright (c) 2018 Cloudchip, Inc. All Rights Reserved.
 *
 * The information contained herein is property of Cloudchip, Inc.
 * Terms and conditions of usage are described in detail in CLOUDCHIP
 * STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information.
 * NO WARRANTY of ANY KIND is provided. This heading must NOT be removed 
 * from the file.
 */

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
