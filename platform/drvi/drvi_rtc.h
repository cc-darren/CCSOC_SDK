/* Copyright (c) 2019 Cloudchip, Inc. All Rights Reserved.
 *
 * The information contained herein is property of Cloudchip, Inc.
 * Terms and conditions of usage are described in detail in CLOUDCHIP
 * STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information.
 * NO WARRANTY of ANY KIND is provided. This heading must NOT be removed 
 * from the file.
 */
 
#ifndef _DRVI_RTC_H_
#define _DRVI_RTC_H_
#include "rtc.h"

// RTC functions
__forceinline void drvi_RtcInit(void)
{
    cc6801_RtcInit();
}
__forceinline T_Rtc drvi_RtcGetTime(void)
{
    return cc6801_RtcGetTime();
}
__forceinline uint32_t drvi_RtcSetTime(T_Rtc tRtcTime)
{
    return cc6801_RtcSetTime(tRtcTime);
}
__forceinline void drvi_RtcRegisterCallback(T_RtcCallback tCB)
{
    cc6801_RtcRegisterCallback(tCB);
}

//ALARM functions
__forceinline void drvi_AlarmInit(void)
{
    cc6801_AlarmInit();
}
__forceinline T_Alarm drvi_AlarmGetTime(E_RTCALARMSupported EAlarmNum)
{
    return cc6801_AlarmGetTime(EAlarmNum);
}
__forceinline uint32_t drvi_AlarmSetTime(E_RTCALARMSupported EAlarmNum,T_Alarm tAlarmTime)
{
    return cc6801_AlarmSetTime(EAlarmNum,tAlarmTime);
}
__forceinline void drvi_AlarmRegisterCallback(T_AlarmCallback tCB)
{
    cc6801_AlarmRegisterCallback(tCB);
}
__forceinline void drvi_AlarmEnable(E_RTCALARMSupported EAlarmNum)
{
    cc6801_AlarmEnable(EAlarmNum);
}
#endif //_DRVI_RTC_H_

