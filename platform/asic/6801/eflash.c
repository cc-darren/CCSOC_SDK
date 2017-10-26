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

#include "drvi_eflash.h"
#include "eflash.h"
#include <stddef.h>

#if (defined FSTORAGE_ENABLED) && (FSTORAGE_ENABLED)
#include "fstorage.h"
#endif

static fpEflash_Callback g_fpEflashCB=NULL;

#if (EFLASH_DMAMODE==0)
uint32_t dwTotalBufSize;
uint32_t dwProgrammedSize;
unsigned char * pSrcData;
uint32_t dwTargetAdr;

void IndirectWrite(void) {
    uint32_t dwTmplen,dwTmpdata;
    //if (dwTotalBufSize) {
        //set eflash address
        wr(EF_ACCESS_REG,dwTargetAdr<<8);
    
        if (dwTotalBufSize>=16) {
            dwTmplen = 4;
            dwTotalBufSize -= 16;
            dwTargetAdr += 16;
        } else {
            dwTmplen = ((dwTotalBufSize+3)>>2);
            dwTotalBufSize=0;
        }
        //set eflash length in DW-1
        wr(EF_CONFIG_REG,((dwTmplen-1)<<16));
        
        dwTmpdata = EF_WR_DATA0_REG;
        while (dwTmplen) {
            //set eflash data
            wr(dwTmpdata,*(uint32_t*)pSrcData);
            dwTmplen--;
            dwTmpdata+=4;
            pSrcData+=4;
        }
        //set eflash mode to program
        wr(EF_FLASHMODE_REG, EF_FLASHMODE_REG_AHBEnable|EF_FLASHMODE_REG_ModeMain);//wait eflash status

//        //wait eflash status
//        do {
//            rd(EF_INTERRUPT_REG,tdata);
//        } while( (tdata&(EF_INTERRUPT_REG_EraPrgModeStatus|EF_INTERRUPT_REG_DataModeStatus))==0 );
//        //clear eflash status
//        wr(EF_INTERRUPT_REG, tdata);  // clear int status
    //}
}
#endif

void EFLASH_IRQHandler(void)
{
    uint32_t dwEfIntrSts;
    rd(EF_INTERRUPT_REG,dwEfIntrSts);

    setbit(EF_INTERRUPT_REG, (EF_INTERRUPT_REG_DMAModeStatus    | \
                              EF_INTERRUPT_REG_EraPrgModeStatus | \
                              EF_INTERRUPT_REG_DataModeStatus     \
                              ));

    if (dwEfIntrSts&(EF_INTERRUPT_REG_DataModeStatus|EF_INTERRUPT_REG_DMAModeStatus)) {
        //write complete
        #if (EFLASH_DMAMODE==0)
        if (dwTotalBufSize)
        {
            IndirectWrite();
        }
        else
        #endif
        {
            if (g_fpEflashCB!=NULL) {
                g_fpEflashCB(NRF_EVT_FLASH_OPERATION_SUCCESS);
            }
        }
    }
    else
    {
        //erase complete
        if (g_fpEflashCB!=NULL) {
            g_fpEflashCB(NRF_EVT_FLASH_OPERATION_SUCCESS);
        }
    }

}


void cc6801_EflashInit(void) 
{
    wr(EF_INTERRUPT_REG, (EF_INTERRUPT_REG_DMAModeStatus    |   \
                          EF_INTERRUPT_REG_EraPrgModeStatus |   \
                          EF_INTERRUPT_REG_DataModeStatus   |   \
                          EF_INTERRUPT_REG_EraPrgModeEn     |   \
                          EF_INTERRUPT_REG_DataModeEn));

    NVIC_EnableIRQ(EFLASH_IRQn);
}

void cc6801_EflashFlush(void) 
{
    uint32_t tdata;
    rd(SCU_ICACHE_REG,tdata);
    
    tdata|=FLUSH_EN;
    wr(SCU_ICACHE_REG,tdata);
    
    do {
        rd(SCU_ICACHE_REG,tdata);
    } while((tdata&FLUSH_EN)!=0);
}

BOOL cc6801_EflashEraseALL(void)
{

    /* Add your Code */
    uint32_t tdata;  //32bit

    //set efalsh start address at 0
    wr(EF_ACCESS_REG,0); 

    //set efalsh mode to mass erase
    tdata = (EF_FLASHMODE_REG_AHBEnable | EF_FLASHMODE_REG_ModeMassErase);
    wr(EF_FLASHMODE_REG, tdata);
    
//    //wait eflash status
//    do {
//      rd(EF_INTERRUPT_REG,tdata);
//    } while( (tdata&(EF_INTERRUPT_REG_EraPrgModeStatus|EF_INTERRUPT_REG_DataModeStatus))==0 );
    
//    // clear eflash status
//    wr(EF_INTERRUPT_REG, tdata);  
    return (0);                                  // Finished without Errors
}
BOOL cc6801_EflashErasePage(uint32_t dwEflashAdr)
{
    if (dwEflashAdr & 0x07FF) {
        return FALSE;
    } else {
        uint32_t tdata;  //32bit

        //set efalsh start address at 0
        wr(EF_ACCESS_REG,dwEflashAdr); 

        //set efalsh mode to mass erase
        tdata = (EF_FLASHMODE_REG_AHBEnable | EF_FLASHMODE_REG_ModeMainErase);
        wr(EF_FLASHMODE_REG, tdata);
    
//        //wait eflash status
//        do {
//            rd(EF_INTERRUPT_REG,tdata);
//        } while( (tdata&(EF_INTERRUPT_REG_EraPrgModeStatus|EF_INTERRUPT_REG_DataModeStatus))==0 );
    
//        // clear eflash status
//        wr(EF_INTERRUPT_REG, tdata);  
        return (TRUE);                                  // Finished without Errors
    }
}

void cc6801_EflashRegisterCallback(fpEflash_Callback fpCB)
{
    g_fpEflashCB = fpCB;
}

void cc6801_EflashProgram(uint32_t dwEflashAdr,unsigned char * pBufAdr,uint32_t dwBufSize)
{
#if EFLASH_DMAMODE
    //use cc6801 DMA mode 
    uint32_t tdata;
    
    //set eflash length in DW-1
    wr(EF_CONFIG_REG,(((dwBufSize>>2)-1)<<16));
 
    //set eflash dwEflashAdr
    wr(EF_ACCESS_REG,dwEflashAdr<<8);
    
    //set eflash mode to program
    tdata = (EF_FLASHMODE_REG_AHBEnable | EF_FLASHMODE_REG_DMAChannel | EF_FLASHMODE_REG_ModeMain);
    wr(EF_FLASHMODE_REG, tdata);
    
    //set dma write address
    tdata = dwEflashAdr;
    wr(EF_DMA_WADDR_REG,dwEflashAdr);
    //set dma read address
    tdata = (uint32_t)pBufAdr;
    wr(EF_DMA_RADDR_REG,tdata);
    
    //set dma control
    dwBufSize--;
    tdata = (EF_DMA_CTRL_REG_DMAEnable | EF_DMA_CTRL_REG_OPModeWrite | dwBufSize<<16 | EF_DMA_CTRL_REG_DMABurst | dwBufSize);
    wr(EF_DMA_CTRL_REG, tdata);
//    //wait eflash status
//    do {
//      rd(EF_INTERRUPT_REG,tdata);
//    } while( (tdata&(EF_INTERRUPT_REG_EraPrgModeStatus|EF_INTERRUPT_REG_DataModeStatus))==0 );
//    //clear eflash status
//    wr(EF_INTERRUPT_REG, tdata);  // clear int status
#else

    dwTotalBufSize = dwBufSize;
    dwProgrammedSize = 0;
    pSrcData = pBufAdr;
    dwTargetAdr = dwEflashAdr;

    





    uint32_t tdata;
    uint32_t tlen;
    while (dwBufSize) {
        //set eflash address
        wr(EF_ACCESS_REG,dwEflashAdr<<8);
    
        if (dwBufSize>16) {
            tlen = 4;
            dwBufSize -= 16;
            dwEflashAdr += 16;
        } else {
            tlen = ((dwBufSize+3)>>2);
            dwBufSize=0;
        }
        //set eflash length in DW-1
        wr(EF_CONFIG_REG,((tlen-1)<<16));
        
        tdata = EF_WR_DATA0_REG;
        while (tlen) {
            //set eflash data
            wr(tdata,*(uint32_t*)pBufAdr);
            tlen--;
            tdata+=4;
            pBufAdr+=4;
        }
        //set eflash mode to program
        wr(EF_FLASHMODE_REG, EF_FLASHMODE_REG_AHBEnable|EF_FLASHMODE_REG_ModeMain);//wait eflash status

//        //wait eflash status
//        do {
//            rd(EF_INTERRUPT_REG,tdata);
//        } while( (tdata&(EF_INTERRUPT_REG_EraPrgModeStatus|EF_INTERRUPT_REG_DataModeStatus))==0 );
//        //clear eflash status
//        wr(EF_INTERRUPT_REG, tdata);  // clear int status
    }
#endif
}
