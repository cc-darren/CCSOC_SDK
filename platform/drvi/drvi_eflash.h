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


#include "project.h"
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
__forceinline void drvi_EflashRegisterCallback(fpEflash_Callback fpCB)
{
    cc6801_EflashRegisterCallback(fpCB);
}


/**@brief SoC Events. */
enum NRF_SOC_EVTS
{
  NRF_EVT_FLASH_OPERATION_SUCCESS,              /**< Event indicating that the ongoing flash operation has completed successfully. */
  NRF_EVT_FLASH_OPERATION_ERROR,                /**< Event indicating that the ongoing flash operation has timed out with an error. */
  NRF_EVT_NUMBER_OF_EVTS
};
#endif
