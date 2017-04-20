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
















#endif //_RTC_H_


