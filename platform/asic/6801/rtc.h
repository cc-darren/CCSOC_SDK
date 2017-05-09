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

//RTC default time is 2017-1-1 Sun 00:00:00
#define RTC_DEFAULT                         \
    {                                       \
        .sec = 0,                           \
        .min = 0,                           \
        .hour = 0,                          \
        .day = 1,                           \
        .month = 1,                         \
        .weekDay = 0,                       \
        .year = 2017,                       \
    }

#define RTC_START_YEAR 2000
    
typedef struct
{
    uint8_t  sec;
    uint8_t  min;
    uint8_t  hour;
    uint8_t  day;
    uint8_t  month;
    uint8_t  weekDay;
    uint16_t year;
} S_rtcInfo;


#define SETRTC_ERR_SEC      0x00000001
#define SETRTC_ERR_MIN      0x00000002
#define SETRTC_ERR_HOUR     0x00000004
#define SETRTC_ERR_DAY      0x00000008
#define SETRTC_ERR_MONTH    0x00000010
#define SETRTC_ERR_YEAR     0x00000020



void cc6801_rtcInit(void);
S_rtcInfo cc6801_rtcGetTime(void);
uint32_t cc6801_rtcSetTime(S_rtcInfo rtcTarget,uint32_t ErrorMask);

void cc6801_rtcSetAlarmEN(uint32_t isEnable);
S_rtcInfo cc6801_rtcGetAlarm(void);
uint32_t cc6801_rtcSetAlarm(S_rtcInfo rtcTarget,uint32_t ErrorMask);







#endif //_RTC_H_


