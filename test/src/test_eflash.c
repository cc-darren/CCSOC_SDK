/******************************************************************************
*  Copyright 2017, Cloudchip, Inc.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Cloudchip, Inc. (C) 2017
******************************************************************************/

/******************************************************************************
*  Filename:
*  ---------
*  test_eflash.c
*
*  Project:
*  --------
*  CC6801
*
*  Description:
*  ------------
*  the description
*
*  Author:
*  -------
*  Mush   (cc-mush)
*
*===========================================================================
*
******************************************************************************/

/******************************************************************************
Head Block of The File
******************************************************************************/
#include "test.h"

#if (TEST_EFLASH)

#include "drvi_eflash.h"

/******************************************************************************
Declaration of External Variables & Functions
******************************************************************************/

/******************************************************************************
Declaration of data structure
******************************************************************************/

/******************************************************************************
Declaration of Global Variables & Functions
******************************************************************************/

#define EFLASH_PAGE_NUMBER  128         //total numbers of page
#define EFLASH_PAGE_SIZE   2048         //byte number of a page

/******************************************************************************
Declaration of static Global Variables & Functions
******************************************************************************/

#define AVALOFF 0x02

void test_eflash(uint32_t dwRepeat,unsigned char *dmaAdr)
{
    uint32_t *dwPtr;
    uint32_t dma_flash_waddr;
    uint32_t ii,jj;

    uint32_t failCnt;
    uint32_t dwRd1,dwRd2;

    drvi_EflashInit();

    while (dwRepeat--) {

        drvi_EflashEraseALL();

        //program eflash data
        dma_flash_waddr = 0x10000000;
        for (ii=0;ii<EFLASH_PAGE_NUMBER;ii++){
            //prepare data in dma
            dwPtr = (uint32_t *)dmaAdr;
            for (jj=0;jj<(EFLASH_PAGE_SIZE/sizeof(uint32_t));jj++) {
                *dwPtr = ((dwRepeat+AVALOFF/3)<<24)+((ii+AVALOFF)<<16)+jj;
                dwPtr++;
            }
            drvi_EflashProgram(dma_flash_waddr,dmaAdr,EFLASH_PAGE_SIZE);
            dma_flash_waddr += EFLASH_PAGE_SIZE;
        }

        //flush
        drvi_EflashFlush();

        //check eflash value
        dma_flash_waddr = 0x10000000;
        failCnt = 0;
        for (ii=0;ii<EFLASH_PAGE_NUMBER;ii++) {
            dwPtr = (uint32_t *)dma_flash_waddr;
            for (jj=0;jj<(EFLASH_PAGE_SIZE/sizeof(uint32_t));jj++) {
                dwRd1 = ((dwRepeat+AVALOFF/3)<<24)+((ii+AVALOFF)<<16)+jj;

                dwRd2 = *dwPtr;

                if (dwRd1!= dwRd2) {
                    failCnt++;
                }
                dwPtr++;
            }
            dma_flash_waddr += EFLASH_PAGE_SIZE;
        }

    }
}

#endif //TEST_EFLASH

