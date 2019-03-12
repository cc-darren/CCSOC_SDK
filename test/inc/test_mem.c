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
#include "test_config.h"

#if (TEST_MEMRW)
#include <string.h>
#include "global.h"
#include "project.h"

/******************************************************************************
Declaration of External Variables & Functions
******************************************************************************/

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

uint8_t sram[CHK_SIZE];

extern void intc_init(void);
extern void testbed(void);

uint32_t *Src4;
uint32_t *Dst4;
uint16_t *Src2;
uint16_t *Dst2;
uint8_t  *Src1;
uint8_t  *Dst1;

void cpy4(uint32_t* dst, uint32_t* src, uint32_t size)
{
    size = size >> 2;
    while(size)
    {
        *dst++ = *src++;
        size--;
    }
}

void cpy2(uint16_t* dst, uint16_t* src, uint32_t size)
{
    size = size >> 1;
    while(size)
    {
        *dst++ = *src++;
        size--;
    }
}

void cpy1(uint8_t* dst, uint8_t* src, uint32_t size)
{
    while(size)
    {
        *dst++ = *src++;
        size--;
    }
}

uint32_t do_test(uint32_t size)
{

    *((volatile unsigned int *)(0x400002CC)) = 1;
    memset(Src4, 0x12, size);
    memcpy(Dst4, Src4, size);
    if( 0 != memcmp(Dst4, Src4, size)) return FAIL;

    *((volatile unsigned int *)(0x400003CC)) = 1;
    memset(Src4, 0x34, size);
    cpy4(Dst4, Src4, size);
    if( 0 != memcmp(Dst4, Src4, size)) return FAIL;

    *((volatile unsigned int *)(0x400004CC)) = 1;
    memset(Src2, 0x56, size);
    cpy2(Dst2, Src2, size);
    if( 0 != memcmp(Dst2, Src2, size)) return FAIL;

    *((volatile unsigned int *)(0x400005CC)) = 1;
    memset(Src1, 0x78, size);
    cpy1(Dst1, Src1, size);
    if( 0 != memcmp(Dst1, Src1, size)) return FAIL;

    return SUCCESS;
}

int TEST_MemRW(void)
{
    wr(GPIO1_INTR_PRIO_REG, 0x1);    // 0x1 = start
    
    while(1)
    {
        //retention ram is 8K, exchange memory is 8K
        uint32_t source, dest;

        source = 0x20020000;    //from retention
        dest = 0x40060000;      //to exchange memory

        Src4 = (uint32_t*)source;
        Src2 = (uint16_t*)source;
        Src1 = (uint8_t*) source;

        Dst4 = (uint32_t*)dest;
        Dst2 = (uint16_t*)dest;
        Dst1 = (uint8_t*) dest;

        //finish testing
        wr(GPIO_INTR_PRIO_REG, 0x1);    // notify sim to finish
        
        if(SUCCESS == do_test(CHK_SIZE))
        {
            wr(GPIO1_INTR_PRIO_REG, 0x2);    // 0x2 pass
        }
        else
        {
            wr(GPIO1_INTR_PRIO_REG, 0x4);    // 0x4 fail
            while(1);
        }
    }
}

#endif //TEST_MEMRW
