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

#if (TEST_RETRAM)
#include <string.h>
#include "global.h"
#include "project.h"
#include "tracer.h"
#include "cc6801_reg.h"

#define CHK_SIZE 0x2000 //8K

#define SYSRAM0_RET_DIS 0x01
#define SYSRAM1_RET_DIS 0x02
#define SYSRAM2_RET_DIS 0x04
#define SYSRAM3_RET_DIS 0x08
#define SYSRAM4_RET_DIS 0x10
#define SYSRAM5_RET_DIS 0x20
#define SYSRAM6_RET_DIS 0x40

#define SYS_RAM0_START      (0x20000000)

#define SYS_RAM1_START      (0x20002000)
#define SYS_RAM1_SIZE       ((uint32_t)(SYS_RAM2_START-SYS_RAM1_START))

#define SYS_RAM2_START      (0x20006000)
#define SYS_RAM2_SIZE       ((uint32_t)(SYS_RAM3_START-SYS_RAM2_START))

#define SYS_RAM3_START      (0x2000A000)
#define SYS_RAM3_SIZE       ((uint32_t)(SYS_RAM4_START-SYS_RAM3_START))

#define SYS_RAM4_START      (0x20012000)
#define SYS_RAM4_SIZE       ((uint32_t)(SYS_RAM5_START-SYS_RAM4_START))

#define SYS_RAM5_START      (0x2001A000)
#define SYS_RAM5_SIZE       ((uint32_t)(SYS_RAM6_START-SYS_RAM5_START))

#define SYS_RAM6_START      (0x20022000)
#define SYS_RAM6_SIZE       ((uint32_t)(SYS_RAM_ENDADD-SYS_RAM6_START))

#define SYS_RAM_ENDADD      (0x2002A000)

/******************************************************************************
Declaration of External Variables & Functions
******************************************************************************/

/******************************************************************************
Declaration of data structure
******************************************************************************/
struct S_RamTest
{
    uint8_t  bRetBit;
    uint8_t  bPattern;
    uint32_t dwSource;
    uint32_t dwDest;
    int iSize;
    uint32_t dwRetSrc;
    uint32_t dwRetSize;
};

/******************************************************************************
Declaration of Global Variables & Functions
******************************************************************************/
uint32_t *Src4;
uint32_t *Dst4;
uint16_t *Src2;
uint16_t *Dst2;
uint8_t  *Src1;
uint8_t  *Dst1;

/******************************************************************************
Declaration of static Global Variables & Functions
******************************************************************************/
const struct S_RamTest RamTestTable[] =
{
    //2 retention off
    {(SYSRAM1_RET_DIS|SYSRAM2_RET_DIS), 0x5A, SYS_RAM1_START, SYS_RAM2_START, SYS_RAM2_SIZE, SYS_RAM3_START, (SYS_RAM_ENDADD - SYS_RAM3_START)},
    {(SYSRAM2_RET_DIS|SYSRAM3_RET_DIS), 0x5B, SYS_RAM2_START, SYS_RAM3_START, SYS_RAM3_SIZE, SYS_RAM4_START, (SYS_RAM_ENDADD - SYS_RAM4_START)},
    {(SYSRAM3_RET_DIS|SYSRAM4_RET_DIS), 0x38, SYS_RAM3_START, SYS_RAM4_START, SYS_RAM4_SIZE, SYS_RAM5_START, (SYS_RAM_ENDADD - SYS_RAM5_START)},
    {(SYSRAM4_RET_DIS|SYSRAM5_RET_DIS), 0x49, SYS_RAM4_START, SYS_RAM5_START, SYS_RAM5_SIZE, SYS_RAM1_START, (SYS_RAM4_START - SYS_RAM1_START)},
    {(SYSRAM5_RET_DIS|SYSRAM6_RET_DIS), 0x72, SYS_RAM5_START, SYS_RAM6_START, SYS_RAM6_SIZE, SYS_RAM1_START, (SYS_RAM5_START - SYS_RAM1_START)},
    {(SYSRAM1_RET_DIS|SYSRAM6_RET_DIS), 0x10, SYS_RAM1_START, SYS_RAM6_START, SYS_RAM6_SIZE, SYS_RAM2_START, (SYS_RAM6_START - SYS_RAM2_START)},
    {(SYSRAM2_RET_DIS|SYSRAM5_RET_DIS), 0x05, SYS_RAM2_START, SYS_RAM5_START, SYS_RAM5_SIZE, SYS_RAM3_START, (SYS_RAM5_START - SYS_RAM3_START)},
    {(SYSRAM3_RET_DIS|SYSRAM4_RET_DIS), 0x17, SYS_RAM3_START, SYS_RAM4_START, SYS_RAM4_SIZE, SYS_RAM5_START, (SYS_RAM_ENDADD - SYS_RAM5_START)},
    {(SYSRAM4_RET_DIS|SYSRAM2_RET_DIS), 0x14, SYS_RAM4_START, SYS_RAM2_START, SYS_RAM2_SIZE, SYS_RAM5_START, (SYS_RAM_ENDADD - SYS_RAM5_START)},
    {(SYSRAM6_RET_DIS|SYSRAM4_RET_DIS), 0x26, SYS_RAM6_START, SYS_RAM4_START, SYS_RAM4_SIZE, SYS_RAM1_START, (SYS_RAM4_START - SYS_RAM1_START)},

    //3 retention off
    {(SYSRAM1_RET_DIS|SYSRAM2_RET_DIS|SYSRAM3_RET_DIS), 0x99, SYS_RAM1_START, SYS_RAM2_START, SYS_RAM2_SIZE, SYS_RAM4_START, (SYS_RAM_ENDADD - SYS_RAM4_START)},
    {(SYSRAM1_RET_DIS|SYSRAM2_RET_DIS|SYSRAM3_RET_DIS), 0x07, SYS_RAM1_START, SYS_RAM3_START, SYS_RAM3_SIZE, SYS_RAM4_START, (SYS_RAM_ENDADD - SYS_RAM4_START)},
    {(SYSRAM1_RET_DIS|SYSRAM2_RET_DIS|SYSRAM3_RET_DIS), 0x30, SYS_RAM2_START, SYS_RAM3_START, SYS_RAM3_SIZE, SYS_RAM4_START, (SYS_RAM_ENDADD - SYS_RAM4_START)},
    {(SYSRAM1_RET_DIS|SYSRAM2_RET_DIS|SYSRAM3_RET_DIS), 0x81, SYS_RAM2_START, SYS_RAM1_START, SYS_RAM1_SIZE, SYS_RAM4_START, (SYS_RAM_ENDADD - SYS_RAM4_START)},
    {(SYSRAM1_RET_DIS|SYSRAM2_RET_DIS|SYSRAM3_RET_DIS), 0x28, SYS_RAM3_START, SYS_RAM1_START, SYS_RAM1_SIZE, SYS_RAM4_START, (SYS_RAM_ENDADD - SYS_RAM4_START)},
    {(SYSRAM1_RET_DIS|SYSRAM2_RET_DIS|SYSRAM3_RET_DIS), 0x41, SYS_RAM3_START, SYS_RAM2_START, SYS_RAM2_SIZE, SYS_RAM4_START, (SYS_RAM_ENDADD - SYS_RAM4_START)},
    {(SYSRAM4_RET_DIS|SYSRAM5_RET_DIS|SYSRAM6_RET_DIS), 0x98, SYS_RAM4_START, SYS_RAM5_START, SYS_RAM5_SIZE, SYS_RAM1_START, (SYS_RAM4_START - SYS_RAM1_START)},
    {(SYSRAM4_RET_DIS|SYSRAM5_RET_DIS|SYSRAM6_RET_DIS), 0xA5, SYS_RAM4_START, SYS_RAM6_START, SYS_RAM6_SIZE, SYS_RAM1_START, (SYS_RAM4_START - SYS_RAM1_START)},
    {(SYSRAM4_RET_DIS|SYSRAM5_RET_DIS|SYSRAM6_RET_DIS), 0xB1, SYS_RAM5_START, SYS_RAM4_START, SYS_RAM4_SIZE, SYS_RAM1_START, (SYS_RAM4_START - SYS_RAM1_START)},
    {(SYSRAM4_RET_DIS|SYSRAM5_RET_DIS|SYSRAM6_RET_DIS), 0x4F, SYS_RAM5_START, SYS_RAM6_START, SYS_RAM6_SIZE, SYS_RAM1_START, (SYS_RAM4_START - SYS_RAM1_START)},
    {(SYSRAM4_RET_DIS|SYSRAM5_RET_DIS|SYSRAM6_RET_DIS), 0x2B, SYS_RAM6_START, SYS_RAM4_START, SYS_RAM4_SIZE, SYS_RAM1_START, (SYS_RAM4_START - SYS_RAM1_START)},
    {(SYSRAM4_RET_DIS|SYSRAM5_RET_DIS|SYSRAM6_RET_DIS), 0x3C, SYS_RAM6_START, SYS_RAM5_START, SYS_RAM5_SIZE, SYS_RAM1_START, (SYS_RAM4_START - SYS_RAM1_START)},
    {(SYSRAM1_RET_DIS|SYSRAM3_RET_DIS|SYSRAM5_RET_DIS), 0xDF, SYS_RAM1_START, SYS_RAM3_START, SYS_RAM3_SIZE, SYS_RAM6_START, (SYS_RAM_ENDADD - SYS_RAM6_START)},
    {(SYSRAM1_RET_DIS|SYSRAM3_RET_DIS|SYSRAM5_RET_DIS), 0xDE, SYS_RAM1_START, SYS_RAM5_START, SYS_RAM5_SIZE, SYS_RAM6_START, (SYS_RAM_ENDADD - SYS_RAM6_START)},
    {(SYSRAM1_RET_DIS|SYSRAM3_RET_DIS|SYSRAM5_RET_DIS), 0xE5, SYS_RAM3_START, SYS_RAM1_START, SYS_RAM1_SIZE, SYS_RAM6_START, (SYS_RAM_ENDADD - SYS_RAM6_START)},
    {(SYSRAM1_RET_DIS|SYSRAM3_RET_DIS|SYSRAM5_RET_DIS), 0xAA, SYS_RAM3_START, SYS_RAM5_START, SYS_RAM5_SIZE, SYS_RAM6_START, (SYS_RAM_ENDADD - SYS_RAM6_START)},
    {(SYSRAM1_RET_DIS|SYSRAM3_RET_DIS|SYSRAM5_RET_DIS), 0xEE, SYS_RAM5_START, SYS_RAM1_START, SYS_RAM1_SIZE, SYS_RAM6_START, (SYS_RAM_ENDADD - SYS_RAM6_START)},
    {(SYSRAM1_RET_DIS|SYSRAM3_RET_DIS|SYSRAM5_RET_DIS), 0x3F, SYS_RAM5_START, SYS_RAM3_START, SYS_RAM3_SIZE, SYS_RAM6_START, (SYS_RAM_ENDADD - SYS_RAM6_START)},
    {(SYSRAM2_RET_DIS|SYSRAM4_RET_DIS|SYSRAM6_RET_DIS), 0xBC, SYS_RAM2_START, SYS_RAM4_START, SYS_RAM4_SIZE, SYS_RAM3_START, (SYS_RAM4_START - SYS_RAM3_START)},
    {(SYSRAM2_RET_DIS|SYSRAM4_RET_DIS|SYSRAM6_RET_DIS), 0xE1, SYS_RAM2_START, SYS_RAM6_START, SYS_RAM6_SIZE, SYS_RAM3_START, (SYS_RAM4_START - SYS_RAM3_START)},
    {(SYSRAM2_RET_DIS|SYSRAM4_RET_DIS|SYSRAM6_RET_DIS), 0x34, SYS_RAM4_START, SYS_RAM2_START, SYS_RAM2_SIZE, SYS_RAM3_START, (SYS_RAM4_START - SYS_RAM3_START)},
    {(SYSRAM2_RET_DIS|SYSRAM4_RET_DIS|SYSRAM6_RET_DIS), 0xA5, SYS_RAM4_START, SYS_RAM6_START, SYS_RAM6_SIZE, SYS_RAM3_START, (SYS_RAM4_START - SYS_RAM3_START)},
    {(SYSRAM2_RET_DIS|SYSRAM4_RET_DIS|SYSRAM6_RET_DIS), 0xFF, SYS_RAM6_START, SYS_RAM2_START, SYS_RAM2_SIZE, SYS_RAM3_START, (SYS_RAM4_START - SYS_RAM3_START)},
    {(SYSRAM2_RET_DIS|SYSRAM4_RET_DIS|SYSRAM6_RET_DIS), 0x00, SYS_RAM6_START, SYS_RAM4_START, SYS_RAM4_SIZE, SYS_RAM3_START, (SYS_RAM4_START - SYS_RAM3_START)},
};

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

void TEST_RetRam(uint32_t dwCount)
{
    uint32_t dwIndex = 0;

    TracerInfo("Retention Ram On/Off test...\n");

    while(dwIndex<dwCount)
    //while(1)
    {
        //retention ram is 8K, exchange memory is 8K
        uint32_t dwSource, dwDest;
        int iSize;
        int i;
        uint8_t *bRetSrc;

        for (i=0; i<(sizeof(RamTestTable)/sizeof(struct S_RamTest)); i++)
        {
            TracerInfo("System RAM to System RAM...pass%02d  %d\n", i, dwIndex);

            //write pattern to retention block
            memset((void*)RamTestTable[i].dwRetSrc, RamTestTable[i].bPattern, RamTestTable[i].dwRetSize);

            //enable retention
            regSCU->dw.retentionEn = ((~RamTestTable[i].bRetBit)&0x7E) << 3;

            //do normal block r/w test
            dwSource = RamTestTable[i].dwSource;
            dwDest = RamTestTable[i].dwDest;
            iSize = RamTestTable[i].iSize;
            test_mem(dwDest, dwSource, iSize);
            
            //disable retention
            regSCU->dw.retentionEn = 0;
            
            //read retention block and compare
            bRetSrc = (uint8_t*)RamTestTable[i].dwRetSrc;
            for(uint32_t j=0;j<RamTestTable[i].dwRetSize;j++)
            {
                if(RamTestTable[i].bPattern != *(bRetSrc++))
                {
                    TracerInfo("Retention fail at 0x%8x\n", bRetSrc);
                    while(1);
                }
            }
        }
        dwIndex++;
    }

    TracerInfo("Retention Ram On/Off test Done...\n");
}

#endif //TEST_RETRAM
