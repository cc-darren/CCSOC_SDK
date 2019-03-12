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
*
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
*  Blake   (cc-blake)
*
*===========================================================================
*
******************************************************************************/

/******************************************************************************
Head Block of The File
******************************************************************************/
#include "test.h"

#if (TEST_MEMRW)
#include <string.h>
#include "global.h"
#include "project.h"
#include "tracer.h"

/******************************************************************************
Declaration of External Variables & Functions
******************************************************************************/
extern uint8_t Image$$RAM_START$$Limit[];
#define SYS_RAM_START      ((uint32_t)Image$$RAM_START$$Limit & 0x7FFFFFFF)

extern uint8_t Image$$RAM_END$$Base[];
#define SYS_RAM_END        ((uint32_t)Image$$RAM_END$$Base & 0x7FFFFFFF)

#define SYS_RAM_SIZE ((uint32_t)(SYS_RAM_END-SYS_RAM_START))

extern uint8_t Image$$RET_RAM$$Base[];
#define RET_RAM_START        ((uint32_t)Image$$RET_RAM$$Base & 0x7FFFFFFF)

extern uint8_t Image$$EX_MEM$$Base[];
#define EX_MEM_START        ((uint32_t)Image$$EX_MEM$$Base & 0x7FFFFFFF)

/******************************************************************************
Declaration of data structure
******************************************************************************/


/******************************************************************************
Declaration of Global Variables & Functions
******************************************************************************/

/******************************************************************************
Declaration of static Global Variables & Functions
******************************************************************************/
#define CHK_SIZE 0x2000 //8K

uint32_t *Src4;
uint32_t *Dst4;
uint16_t *Src2;
uint16_t *Dst2;
uint8_t  *Src1;
uint8_t  *Dst1;

void cpy4(uint32_t* cDst, uint32_t* cSrc, uint32_t iSize)
{
    iSize = iSize >> 2;
    while(iSize)
    {
        *cDst++ = *cSrc++;
        iSize--;
    }
}

void cpy2(uint16_t* cDst, uint16_t* cSrc, uint32_t iSize)
{
    iSize = iSize >> 1;
    while(iSize)
    {
        *cDst++ = *cSrc++;
        iSize--;
    }
}

void cpy1(uint8_t* cDst, uint8_t* cSrc, uint32_t iSize)
{
    while(iSize)
    {
        *cDst++ = *cSrc++;
        iSize--;
    }
}

uint32_t do_test(uint32_t iSize)
{

    memset(Src4, 0x12, iSize);
    memcpy(Dst4, Src4, iSize);
    if( 0 != memcmp(Dst4, Src4, iSize)) return FAIL;

    memset(Src4, 0x34, iSize);
    cpy4(Dst4, Src4, iSize);
    if( 0 != memcmp(Dst4, Src4, iSize)) return FAIL;

    memset(Src2, 0x56, iSize);
    cpy2(Dst2, Src2, iSize);
    if( 0 != memcmp(Dst2, Src2, iSize)) return FAIL;

    memset(Src1, 0x78, iSize);
    cpy1(Dst1, Src1, iSize);
    if( 0 != memcmp(Dst1, Src1, iSize)) return FAIL;

    return SUCCESS;
}

uint32_t test_mem(uint32_t dwDest, uint32_t dwSource, int iSize)
{
    uint32_t dwResult = SUCCESS;
    int iCount;

    Src4 = (uint32_t*)dwSource;
    Src2 = (uint16_t*)dwSource;
    Src1 = (uint8_t*) dwSource;

    do
    {
        Dst4 = (uint32_t*)dwDest;
        Dst2 = (uint16_t*)dwDest;
        Dst1 = (uint8_t*) dwDest;

        if (iSize >= CHK_SIZE)
            iCount = CHK_SIZE;
        else
            iCount = iSize;

        if(FAIL == do_test(iCount))
        {
            dwResult = FAIL;
            TracerErr("Memory R/W test Fail...\n");
            while(1);
            //break;
        }

        iSize -= CHK_SIZE;
        dwDest += CHK_SIZE;

    } while(iSize > 0);

    return dwResult;
}

void TEST_MemRW(uint32_t dwCount)
{
    uint32_t dwIndex = 0;

    TracerInfo("Memory R/W test...\n");

    while(dwIndex<dwCount)
    {
        //retention ram is 8K, exchange memory is 8K
        uint32_t dwSource, dwDest;
        int iSize;

        TracerInfo("System RAM to Exchange RAM...\n");
        dwSource = SYS_RAM_START;    //from retention
        dwDest = EX_MEM_START;      //to system memory
        iSize = CHK_SIZE;
        test_mem(dwDest, dwSource, iSize);

//        TracerErr("Retention RAM to Exchange RAM...\n");
//        dwSource = RET_RAM_START;    //from retention
//        dwDest = EX_MEM_START;      //to exchange memory
//        iSize = CHK_SIZE;
//        test_mem(dwDest, dwSource, iSize);

        TracerInfo("Exchange RAM to System RAM...\n");
        dwSource = EX_MEM_START;    //from exchange memory
        dwDest = SYS_RAM_START;      //to retention
        iSize = CHK_SIZE;
        test_mem(dwDest, dwSource, iSize);

        dwIndex++;
    }

    TracerInfo("Memory R/W test Done...\n");
}

#endif //TEST_MEMRW
