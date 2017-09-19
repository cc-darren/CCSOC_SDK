/**
 ****************************************************************************************
 *
 * @file drvi_rtc.h
 *
 * @brief Head file of drvi_rtc.c
 *
 * Copyright (C) CloudChip 2017-2019
 *
 *
 ****************************************************************************************
 */
#ifndef _DRVI_RTC_H_
#define _DRVI_RTC_H_
#include "rtc.h"
#include "project.h"

#define drvi_rtcInit()              cc6801_rtcInit()
#define drvi_rtcGetTime()           cc6801_rtcGetTime()
#define drvi_rtcSetTime(x,y)        cc6801_rtcSetTime(x,y)
#define drvi_rtcSetAlarmEN(x)       cc6801_rtcSetAlarmEN(x)
#define drvi_rtcGetAlarm()          cc6801_rtcGetAlarm()
#define drvi_rtcSetAlarm(x,y)       cc6801_rtcSetAlarm(x,y)
#define drvi_rtcRegisterCallback(x) cc6801_rtcRegisterCallback(x)








#endif //_DRVI_RTC_H_


