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

#ifndef _DRVI_LOWPOWER_H_
#define _DRVI_LOWPOWER_H_


#include "project.h"
#include "lowpower.h"

__forceinline void drvi_LowPower_Sleep(Lowpower_PowerMode_t lpMode)
{
    cc6801_LowPower_Sleep(lpMode);
}
__forceinline uint32_t drvi_LowPower_SetWakeupSource(Lowpower_WakeUp_t SetID,uint32_t dwPinNum,BOOL bIsGetData,uint32_t * pBuf)
{
    return cc6801_LowPower_SetWakeupSource(SetID,dwPinNum,bIsGetData,pBuf);
}

__forceinline void drvi_LowPower_PeripheralOn(uint32_t dwPeripheral)
{
    cc6801_LowPower_PeripheralOn(dwPeripheral);
}

__forceinline void drvi_LowPower_PeripheralOff(uint32_t dwPeripheral)
{
    cc6801_LowPower_PeripheralOff(dwPeripheral);
}

__forceinline void drvi_LowPower_PowerOn(void)
{
    cc6801_LowPower_PowerOn();
}
__forceinline void drvi_LowPower_PowerOff(void)
{
    cc6801_LowPower_PowerOff();
}


#endif //_DRVI_LOWPOWER_H_
