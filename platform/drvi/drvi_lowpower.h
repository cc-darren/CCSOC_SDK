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
