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

#define EflashInit()        cc6801_EflashInit()
#define EflashFlush()       cc6801_EflashFlush()
#define EflashEraseALL()    cc6801_EflashEraseALL()
#define EflashErasePage(dwEflashAdr)  cc6801_EflashErasePage(dwEflashAdr)
#define EflashProgram(dwEflashAdr,pBufAdr,dwBufSize)  cc6801_EflashProgram(dwEflashAdr,pBufAdr,dwBufSize)



#endif
