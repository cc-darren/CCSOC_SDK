/* Copyright (c) 2019 Cloudchip, Inc. All Rights Reserved.
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
#include <stdio.h>
#include "global.h"
#include "cc6801_reg.h"

volatile uint32_t g_dwDMIC_INTR = 0;

T_callback  dmic_callback_handler = NULL;

void DMIC_IRQHandler(void)
{
    uint32_t data;
    
	rd(DMIC_INTR_REG, data);
    wr(DMIC_INTR_REG, (data | (1<<16)));     // clear interrupt
    g_dwDMIC_INTR = 1;
    
    if(NULL != dmic_callback_handler)
    {
        dmic_callback_handler((void*)NULL);
    }
}

void cc6801_dmicInit(T_callback handler)
{
    wr(DMIC_INTR_REG,       0x00010000);        // clear interrupt status(bit16) and disable interrupt(bit0)
    wr(DMIC_CTRL_REG,       0x00000000);        // disable L/R channel(bit1:R, bit0:L)
    wr(DMIC_DMA_BYTE_REG,   0x00000000);        // clear DMA byte (0:1byte, FF:256bytes), DW boundary
    wr(DMIC_DMA_L_START_REG,0x00000000);        // clear L start address [16:0], DW boundary
    wr(DMIC_DMA_L_END_REG,  0x00000000);        // clear L end address [16:0], DW boundary
    wr(DMIC_DMA_R_START_REG,0x00000000);        // clear L start address [16:0], DW boundary
    wr(DMIC_DMA_R_END_REG,  0x00000000);        // clear L end address [16:0], DW boundary
    wr(DMIC_DMA_ENABLE_REG, 0x00000000);        // disable both L/R DMA(bit0)

    g_dwDMIC_INTR = 0;
    dmic_callback_handler = handler;
}

void cc6801_dmicLeftConfig(uint32_t dwDmaBufStart, uint32_t dwDmaBufEnd)
{
    //TODO: dwDmaBufEnd unsigned address must be after dwDmaBufStart unsigned address
    //if(dwDmaBufEnd - dwDmaBufStart)
    {
        wr(DMIC_DMA_L_START_REG, dwDmaBufStart);
        wr(DMIC_DMA_L_END_REG,   dwDmaBufEnd);
    }
    //else
    {
        //return error;
    }
}

void cc6801_dmicRightConfig(uint32_t dwDmaBufStart, uint32_t dwDmaBufEnd)
{
    //TODO: dwDmaBufEnd unsigned address must be after dwDmaBufStart unsigned address
    //if(dwDmaBufEnd - dwDmaBufStart)
    {
        wr(DMIC_DMA_R_START_REG, dwDmaBufStart);
        wr(DMIC_DMA_R_END_REG,   dwDmaBufEnd);
    }
    //else
    {
        //return error;
    }
}

void cc6801_dmicEnableBoth(uint32_t dwIntTriggerLevelWord)
{
    //TODO: dwIntTriggerLevel must be less than or equal to the range (dwDmaBufEnd - dwDmaBufStart)
    wr(DMIC_DMA_BYTE_REG, ((dwIntTriggerLevelWord<<1)-1));       //0:1byte, FF:256bytes.
    wr(DMIC_INTR_REG, 0x00000001);                      //enable interrupt
    //TODO: implement L/R channel respectively
    wr(DMIC_CTRL_REG, 0x00000003);                      //enable both L/R channel
    wr(DMIC_DMA_ENABLE_REG, 0x00000001);                //start receiving data

  #if defined(FPGA) && FPGA //used for old DMIC design in bitfile-127, one channel
    wr(DMIC_DMA_BYTE_REG, 4);       //0:1byte, FF:256bytes.
    wr(DMIC_INTR_REG, 0x00000001);                      //enable interrupt
    //TODO: implement L/R channel respectively
    wr(DMIC_CTRL_REG, 0x00000000);                      //enable both L/R channel
    wr(DMIC_DMA_R_START_REG, 0x00000001);                //start receiving data
  #endif
    
    NVIC_EnableIRQ(DMIC_IRQn       );

}

void cc6801_dmicDisableBoth(void)
{
    NVIC_DisableIRQ(DMIC_IRQn       );
    
    wr(DMIC_INTR_REG, 0x00000000);              //disable interrupt
    //TODO: implement L/R channel respectively
    wr(DMIC_CTRL_REG, 0x00000000);              //disable both L/R channel
    wr(DMIC_DMA_ENABLE_REG, 0x00000000);        //stop receiving data
}

void cc6801_dmicReadCurrentPos(uint32_t dwLeftPos, uint32_t dwRightPos)
{

}


