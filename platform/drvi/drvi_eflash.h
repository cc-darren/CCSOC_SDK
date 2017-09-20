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

#define drvi_EflashInit()        cc6801_EflashInit()
#define drvi_EflashFlush()       cc6801_EflashFlush()
#define drvi_EflashEraseALL()    cc6801_EflashEraseALL()
#define drvi_EflashErasePage(dwEflashAdr)  cc6801_EflashErasePage(dwEflashAdr)
#define drvi_EflashProgram(dwEflashAdr,pBufAdr,dwBufSize)  cc6801_EflashProgram(dwEflashAdr,pBufAdr,dwBufSize)



#endif
