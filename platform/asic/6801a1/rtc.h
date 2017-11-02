/**
 ****************************************************************************************
 *
 * @file rtc.h
 *
 * @brief Head file of rtc.c
 *
 * Copyright (C) CloudChip 2017-2019
 *
 *
 ****************************************************************************************
 */
#ifndef _RTC_H_
#define _RTC_H_
#include "global.h"
#include "drvi_rtc.h"

#define RTC_BASE_YEAR           2000

//RTC default time is 2017-1-1 Sun 00:00:00
#define RTC_DEFAULT                                         \
    {                                                       \
        .sec = RTC_DEFAULT_SEC,                             \
        .min = RTC_DEFAULT_MIN,                             \
        .hour = RTC_DEFAULT_HOUR,                           \
        .day = RTC_DEFAULT_DAY,                             \
        .month = RTC_DEFAULT_MONTH,                         \
        .weekDay = RTC_DEFAULT_WEEK,                        \
        .year = (RTC_DEFAULT_YEAR-RTC_BASE_YEAR),           \
        .h24 = RTC_DEFAULT_H24                              \
    }

typedef struct
{
    uint8_t  sec;
    uint8_t  min;
    uint8_t  hour;
    uint8_t  day;
    uint8_t  month;
    uint8_t  weekDay;
    uint8_t  year;
    uint8_t  h24;
} S_rtcInfo;

typedef struct
{
    T_callback  sec;
    T_callback  min;
    T_callback  hour;
    T_callback  day;
    T_callback  a1;
    T_callback  a2;
} S_rtcCallback;


#define SETRTC_ERR_SEC      0x00000001
#define SETRTC_ERR_MIN      0x00000002
#define SETRTC_ERR_HOUR     0x00000004
#define SETRTC_ERR_DAY      0x00000008
#define SETRTC_ERR_MONTH    0x00000010
#define SETRTC_ERR_YEAR     0x00000020



void      cc6801_rtcInit(void);
S_rtcInfo cc6801_rtcGetTime(void);
uint32_t  cc6801_rtcSetTime(S_rtcInfo rtcTarget,uint32_t ErrorMask);

void      cc6801_rtcSetAlarmEN(uint32_t isEnable);
S_rtcInfo cc6801_rtcGetAlarm(void);
uint32_t  cc6801_rtcSetAlarm(S_rtcInfo rtcTarget,uint32_t ErrorMask);







#endif //_RTC_H_


