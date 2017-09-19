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

/******************************************************************************
*  Filename:
*  ---------
*  rtc.c
*
*  Project:
*  --------
*  cc6801
*
*  Description:
*  ------------
*  This is RTC driver
*
*   -- use 24HR format in RTC driver, caller should transform the format by itself
*
*
*   cc6801_rtcInit      : set rtc time to 2017-1-1 Sun 00:00:00, and cealr alarm1 and alarm2
*
*   cc6801_rtcGetTime   : get rtc time 
*   cc6801_rtcSetTime   : set rtc time, sec and weekday will be ignored. 
*                         RTC sec write will make sec cunter to 0.
*                         Weekday will be generated automatically.
*
*   cc6801_rtcGetAlram  : get rtc alarm
*   cc6801_rtcSetAlram  : set rtc alarm
*
*
*  Author:
*  -------
*  CloudChip
*
*===========================================================================/
*  20170331 PAT initial version
******************************************************************************/
#include <stdio.h>
#include "global.h"
#include "rtc.h"
#include "cc6801_reg.h"


/*
rtc tool functions 
uint32_t CheckRTC(S_rtcInfo * rtc) check the rtc info is correct or not, and also calculate the weekday
*/
const uint8_t mdays[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
const uint8_t mweekdays[12] = {0, 3, 3, 6, 1, 4, 6, 2, 5, 0, 3, 5};

static uint32_t CheckRTC(S_rtcInfo * rtc)
{
    uint32_t errcode;
    uint32_t day;
    uint32_t isLeapYear;
    
    if( rtc->year%400==0 || ( rtc->year%4==0 && rtc->year%100!=0 ) )
         isLeapYear = 1;
    else
         isLeapYear = 0; 
    
    errcode = 0;
    if (rtc->sec > 60) 
        errcode |= SETRTC_ERR_SEC;
    if (rtc->min > 60) 
        errcode |= SETRTC_ERR_MIN;
    if (rtc->hour > 24) 
        errcode |= SETRTC_ERR_HOUR;
    if (rtc->month > 12) 
        errcode |= SETRTC_ERR_MONTH;
    if (rtc->year <2000) 
        errcode |= SETRTC_ERR_YEAR;
    
    
    day = mdays[rtc->month-1];
    if (rtc->month ==2)
        day += isLeapYear;

    if (rtc->day > day) 
        errcode |= SETRTC_ERR_DAY;
    
    //cal weekday
    day = rtc->year + rtc->day - 1;
    day +=  rtc->year/4;
    day +=  rtc->year/400;
    day -=  rtc->year/100;
    day +=  mweekdays[rtc->month-1];
    if (rtc->month<3)
        day -= isLeapYear;
    rtc->weekDay = day%7;
    
    return errcode;
}

/*
rtc IRQ handler
*/
volatile uint32_t RTC_INT = 0;

void RTC_IRQHandler(void)
{
    RTC_INT = (regRTC->dw.interrupt);
    RTC_INT &= 0x003F0000;
}

/*
rtc tool functions 
uint32_t CheckRTC(S_rtcInfo * rtc) check the rtc info is correct or not, and also calculate the weekday
*/
void cc6801_rtcInit(void)
{
    //rtc init will set HW RTC to factory default time 2017-1-1 Sun 00:00:00
    S_rtcInfo rtc = RTC_DEFAULT;

    //reset all interrupt
    regRTC->dw.interrupt    = 0;
    regRTC->bf.secIntEn     = 1;
    regRTC->bf.alarm1IntEn  = 0;
    regRTC->bf.alarm2IntEn  = 0;
    regRTC->bf.minIntEn     = 1;
    regRTC->bf.hourIntEn    = 1;
    regRTC->bf.dayIntEn     = 1;

    //set data mode to BCD
    regRTC->bf.dataMode = 1;
    
    //set hourformat to 24-hour
    regRTC->bf.hourFormat = rtc.h24;
    
    //Init RTC and alarm counter
    cc6801_rtcSetTime(rtc,0);
    cc6801_rtcSetAlarm(rtc,0);
}

S_rtcInfo cc6801_rtcGetTime(void)
{
    S_rtcInfo time;
    
    time.sec        = regRTC->bf.sec;
    time.min        = regRTC->bf.min;
    time.hour       = regRTC->bf.hour;
    time.day        = regRTC->bf.day;
    time.weekDay    = regRTC->bf.weekDay;
    time.year       = regRTC->bf.year+RTC_BASE_YEAR;
    return time;
}

uint32_t cc6801_rtcSetTime(S_rtcInfo rtcTarget,uint32_t ErrorMask)
{
    uint32_t errcode;
    errcode = CheckRTC(&rtcTarget);
    if (errcode & ErrorMask)
    {
        return errcode;
    }
    regRTC->bf.sec      = rtcTarget.sec;
    regRTC->bf.min      = rtcTarget.min;
    regRTC->bf.hour     = rtcTarget.hour;
    regRTC->bf.day      = rtcTarget.day;
    regRTC->bf.month    = rtcTarget.month;
    regRTC->bf.weekDay  = rtcTarget.weekDay;
    regRTC->bf.year     = (rtcTarget.year-RTC_BASE_YEAR);
    return errcode;
}


void cc6801_rtcSetAlarmEN(uint32_t isEnable)
{
    if (isEnable)
    {
        regRTC->bf.alarm1IntEn = 1;
    }
    else
    {
        regRTC->bf.alarm1IntEn = 0;
    }
}

S_rtcInfo cc6801_rtcGetAlarm(void)
{
    S_rtcInfo alarm;
    
    alarm.sec       = regRTC->bf.alarm1Sec;
    alarm.min       = regRTC->bf.alarm1Min;
    alarm.hour      = regRTC->bf.alarm1Hour;
    alarm.day       = regRTC->bf.alarm1Day;
    alarm.month     = regRTC->bf.alarm1Month;
    alarm.weekDay   = 0xFF;
    alarm.year      = 0xFF;
    return alarm;
}

uint32_t cc6801_rtcSetAlarm(S_rtcInfo rtcTarget,uint32_t ErrorMask)
{
    uint32_t errcode;
    errcode = CheckRTC(&rtcTarget);
    if (errcode & ErrorMask)
    {
        return errcode;
    }
    regRTC->bf.alarm1Sec    = rtcTarget.sec;
    regRTC->bf.alarm1Min    = rtcTarget.min;
    regRTC->bf.alarm1Hour   = rtcTarget.hour;
    regRTC->bf.alarm1Day    = rtcTarget.day;
    regRTC->bf.alarm1Month  = rtcTarget.month;
    return errcode;
}

void cc6801_rtcRegisterCallback(T_callback tCB)
{
    
    
}

