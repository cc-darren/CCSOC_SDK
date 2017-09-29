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

#ifndef _DRVI_EFLASH_H_
#define _DRVI_EFLASH_H_

#include "eflash.h"


__forceinline void drvi_EflashInit(void)
{
    cc6801_EflashInit();
}

__forceinline void drvi_EflashFlush(void)
{
    cc6801_EflashFlush();
}
__forceinline BOOL drvi_EflashEraseALL(void)
{
    return cc6801_EflashEraseALL();
}
__forceinline BOOL drvi_EflashErasePage(uint32_t dwEflashAdr)
{
    return cc6801_EflashErasePage(dwEflashAdr);
}
__forceinline void drvi_EflashProgram(uint32_t dwEflashAdr,unsigned char * pBufAdr,uint32_t dwBufSize)
{
    cc6801_EflashProgram(dwEflashAdr,pBufAdr,dwBufSize);
}


#endif
