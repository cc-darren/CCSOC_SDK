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

#ifndef EFLASH_ASYNCMODE
#define EFLASH_ASYNCMODE 1
#endif

#ifndef EFLASH_DMAMODE
#define EFLASH_DMAMODE 0
#endif

static fpEflash_Callback g_fpEflashCB=NULL;

#if (EFLASH_DMAMODE==0)
uint32_t dwTotalBufSize;
uint32_t dwProgrammedSize;
unsigned char * pSrcData;
uint32_t dwTargetAdr;

void IndirectWrite(void) {
    uint32_t dwTmplen;

    //set eflash address
    regEFLASH->dwIndirStart = (dwTargetAdr<<8);

    if (dwTotalBufSize>=16) {
        dwTmplen = 4;
        dwTotalBufSize -= 16;
        dwTargetAdr += 16;
    } else {
        dwTmplen = ((dwTotalBufSize+3)>>2);
        dwTotalBufSize=0;
    }
    //set eflash length in DW-1
    regEFLASH->dwCtrl = ((dwTmplen-1)<<16);


    for (uint32_t ii=0;ii<dwTmplen;ii++) {
        //set eflash data
        regEFLASH->dwProgBuf[ii] = (*(uint32_t*)pSrcData);
        pSrcData+=4;
    }
    //set eflash mode to program
    regEFLASH->dwAccessCtrl = (EF_FLASHMODE_REG_AHBEnable|EF_FLASHMODE_REG_ModeMain);
}
#endif //(EFLASH_DMAMODE==0)

void EFLASH_IRQHandler(void)
{
    uint32_t dwEfIntrSts;

    dwEfIntrSts = regEFLASH->dwInterrupt;

    regEFLASH->dwInterrupt |= (EF_INTERRUPT_REG_DMAModeStatus    | EF_INTERRUPT_REG_EraPrgModeStatus | EF_INTERRUPT_REG_DataModeStatus);

    if (dwEfIntrSts&(EF_INTERRUPT_REG_DataModeStatus|EF_INTERRUPT_REG_DMAModeStatus)) {
        //write complete
        #if ((EFLASH_DMAMODE==0) && (EFLASH_ASYNCMODE==1))
        if (dwTotalBufSize)
        {
            IndirectWrite();
        }
        else
        #endif
        {
            if (g_fpEflashCB!=NULL) {
                g_fpEflashCB(E_DRVI_EFLASH_OPERATION_SUCCESS);
            }
        }
    }
    else
    {
        //erase complete
        if (g_fpEflashCB!=NULL) {
            g_fpEflashCB(E_DRVI_EFLASH_OPERATION_SUCCESS);
        }
    }
}

void cc6801_EflashInit(void) 
{
#if (EFLASH_ASYNCMODE==1)
    NVIC_ClearPendingIRQ(EFLASH_IRQn);
    NVIC_EnableIRQ(EFLASH_IRQn);
    regEFLASH->dwInterrupt |= (EF_INTERRUPT_REG_DMAModeStatus | EF_INTERRUPT_REG_EraPrgModeStatus | EF_INTERRUPT_REG_DataModeStatus | EF_INTERRUPT_REG_EraPrgModeEn | EF_INTERRUPT_REG_DataModeEn);
#else
    regEFLASH->dwInterrupt |= (EF_INTERRUPT_REG_DMAModeStatus | EF_INTERRUPT_REG_EraPrgModeStatus | EF_INTERRUPT_REG_DataModeStatus);
#endif
}

IN_RET_RAM_BEGIN
void cc6801_EflashFlush(void) 
{
    uint32_t tdata;
	
    GLOBAL_INT_STOP();

    //setbit(SCU_ICACHE_REG,FLUSH_EN);
    regSCU->dw.cacheBootCtrl |= FLUSH_EN;
    do {
        //rd(SCU_ICACHE_REG,tdata);
        tdata = regSCU->dw.cacheBootCtrl;
    } while((tdata&FLUSH_EN)!=0);
	
    GLOBAL_INT_START();

}
IN_RET_RAM_END

BOOL cc6801_EflashEraseALL(void)
{

    /* Add your Code */
    uint32_t tdata;  //32bit

    //set efalsh start address at 0
    regEFLASH->dwIndirStart = (0<<8);

    //set efalsh mode to mass erase
    tdata = (EF_FLASHMODE_REG_AHBEnable | EF_FLASHMODE_REG_ModeMassErase);
    regEFLASH->dwAccessCtrl = tdata;

#if (EFLASH_ASYNCMODE==0)
    //wait eflash status
    do {
        tdata = regEFLASH->dwInterrupt;
    } while( (tdata&(EF_INTERRUPT_REG_EraPrgModeStatus|EF_INTERRUPT_REG_DataModeStatus))==0 );
    EFLASH_IRQHandler();
#endif

    return (0);                                  // Finished without Errors
}
BOOL cc6801_EflashErasePage(uint32_t dwEflashAdr)
{
    if (dwEflashAdr & 0x07FF) {
        return FALSE;
    } else {
        uint32_t tdata;  //32bit

        //set efalsh start address at 0
        regEFLASH->dwIndirStart = (dwEflashAdr<<8);

        //set efalsh mode to mass erase
        tdata = (EF_FLASHMODE_REG_AHBEnable | EF_FLASHMODE_REG_ModeMainErase);
        regEFLASH->dwAccessCtrl = tdata;

#if (EFLASH_ASYNCMODE==0)
        //wait eflash status
        do {
            tdata = regEFLASH->dwInterrupt;
        } while( (tdata&(EF_INTERRUPT_REG_EraPrgModeStatus|EF_INTERRUPT_REG_DataModeStatus))==0 );
        EFLASH_IRQHandler();
#endif
        return (TRUE);                                  // Finished without Errors
    }
}

void cc6801_EflashRegisterCallback(fpEflash_Callback fpCB)
{
    g_fpEflashCB = fpCB;
}
void cc6801_EflashProgram(uint32_t dwEflashAdr,unsigned char * pBufAdr,uint32_t dwBufSize)
{
#if (EFLASH_DMAMODE==1)  //use cc6801 DMA mode 
    uint32_t tdata;

    //set eflash length in DW-1
    regEFLASH->dwCtrl = (((dwBufSize>>2)-1)<<16);
 
    //set eflash dwEflashAdr
    regEFLASH->dwIndirStart = (dwEflashAdr<<8);
    
    //set dma write address
    tdata = dwEflashAdr;
    regEFLASH->dwDmaWrAddr = dwEflashAdr;
    //set dma read address
    tdata = (uint32_t)pBufAdr;
    regEFLASH->dwDmaRdAddr = tdata;
    
    //set dma control
    dwBufSize--;
    tdata = (EF_DMA_CTRL_REG_DMAEnable | EF_DMA_CTRL_REG_OPModeWrite | dwBufSize<<16 | EF_DMA_CTRL_REG_DMABurst | dwBufSize);
    regEFLASH->dwDmaCtrl = tdata;
    
    //set eflash mode to program
    tdata = (EF_FLASHMODE_REG_AHBEnable | EF_FLASHMODE_REG_DMAChannel | EF_FLASHMODE_REG_ModeMain);
    regEFLASH->dwAccessCtrl = tdata;

#if (EFLASH_ASYNCMODE==0)
    //wait eflash status
    do {
        tdata = regEFLASH->dwInterrupt;
    } while( (tdata&(EF_INTERRUPT_REG_EraPrgModeStatus|EF_INTERRUPT_REG_DataModeStatus))==0 );
    EFLASH_IRQHandler();
#endif

#else //EFLASH_DMAMODE

    dwTotalBufSize = dwBufSize;
    dwProgrammedSize = 0;
    pSrcData = pBufAdr;
    dwTargetAdr = dwEflashAdr;

#if (EFLASH_ASYNCMODE==0)
    uint32_t tdata;

    while (dwTotalBufSize)
    {
        IndirectWrite();
        do {
            tdata = regEFLASH->dwInterrupt;
        } while( (tdata&(EF_INTERRUPT_REG_EraPrgModeStatus|EF_INTERRUPT_REG_DataModeStatus))==0 );
    }
    EFLASH_IRQHandler();
#else
    IndirectWrite();
#endif

#endif //EFLASH_DMAMODE
}

void cc6801_SecurityProtect(void)
{
    uint32_t protectval=0x00000000;
    cc6801_EflashProgram(0x1003FFFC,(unsigned char *)&protectval,4);
}
