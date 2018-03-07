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
#include <string.h>
#include "global.h"
#include "rtc.h"
#include "cc6801_reg.h"

T_RtcCallback g_rtcCB;

T_AlarmCallback g_AlarmCB;

/*
rtc IRQ handler
*/
//volatile uint32_t RTC_INT = 0;

void RTC_IRQHandler(void)
{
    uint32_t dwRtcSts;
    
    rd(RTC_INTR_REG,dwRtcSts);
    dwRtcSts &= RTC_INTR_REG_ST_MASK;

    if (dwRtcSts & RTC_INTR_REG_ST_SEC)
    {
        if (g_rtcCB.sec!=NULL)
        {
            g_rtcCB.sec((void*)NULL);
        }
    }
    if (dwRtcSts & RTC_INTR_REG_ST_ALARM)
    {
        if (g_AlarmCB.alarm!=NULL)
        {
            g_AlarmCB.alarm((void*)NULL);
        }
    }
    if (dwRtcSts & RTC_INTR_REG_ST_ALARM2)
    {
        if (g_AlarmCB.alarm2!=NULL)
        {
            g_AlarmCB.alarm2((void*)NULL);
        }
    }
    if (dwRtcSts & RTC_INTR_REG_ST_MIN)
    {
        if (g_rtcCB.min!=NULL)
        {
            g_rtcCB.min((void*)NULL);
        }
    }
    if (dwRtcSts & RTC_INTR_REG_ST_HOUR)
    {
        if (g_rtcCB.hour!=NULL)
        {
            g_rtcCB.hour((void*)NULL);
        }
    }
    if (dwRtcSts & RTC_INTR_REG_ST_DAY)
    {
        if (g_rtcCB.day!=NULL)
        {
            g_rtcCB.day((void*)NULL);
        }
    }
}

/*
rtc tool functions 
uint32_t CheckRTC(S_rtcInfo * rtc) check the rtc info is correct or not, and also calculate the weekday
*/
const uint8_t mdays[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
const uint8_t mweekdays[12] = {0, 3, 3, 6, 1, 4, 6, 2, 5, 0, 3, 5};

static uint32_t CheckRTC(T_Rtc * pRtcData)
{
    uint32_t errcode;
    uint32_t day;
    uint32_t isLeapYear;
    
    errcode = 0;
    if (pRtcData->sec > 59) 
        errcode |= RTC_ERR_SEC;
    if (pRtcData->min > 59) 
        errcode |= RTC_ERR_MIN;
    if (pRtcData->h24 == RTC_HOUR_24H)
    {
        if (pRtcData->hour > 24)
        {
            errcode |= RTC_ERR_HOUR;
        }
    }
    else if ((pRtcData->h24 == RTC_HOUR_AM)||(pRtcData->h24 == RTC_HOUR_PM))
    {
        if (pRtcData->hour > 12)
        {
            errcode |= RTC_ERR_HOUR;
        }
    }
    else //select none-correct hour format
    {
         errcode |= RTC_ERR_HOUR;
    }
    if (pRtcData->month > 12) 
        errcode |= RTC_ERR_MONTH;
    if (pRtcData->year <2000) 
        errcode |= RTC_ERR_YEAR;
    
    //check day
    if( pRtcData->year%400==0 || ( pRtcData->year%4==0 && pRtcData->year%100!=0 ) )
         isLeapYear = 1;
    else
         isLeapYear = 0; 
    
    day = mdays[pRtcData->month-1];
    if (pRtcData->month ==2)
        day += isLeapYear;

    if (pRtcData->day > day) 
        errcode |= RTC_ERR_DAY;
    
    //cal weekday
    day = pRtcData->year + pRtcData->day - 1;
    day +=  pRtcData->year/4;
    day +=  pRtcData->year/400;
    day -=  pRtcData->year/100;
    day +=  mweekdays[pRtcData->month-1];
    if (pRtcData->month<3)
        day -= isLeapYear;
    pRtcData->weekDay = day%7;
    
    return errcode;
}

/*
rtc tool functions 
uint32_t CheckRTC(S_rtcInfo * rtc) check the rtc info is correct or not, and also calculate the weekday
*/
void cc6801_RtcInit(void)
{
    // RTC_DEFAULT is decided by project header file, set RTC to RTC_DEFAULT
    T_Rtc tRtcTime = RTC_DEFAULT;

    //clea all interrupt status
    setbit(RTC_INTR_REG,0);

    //set data mode to binary, normal mode
    wr(RTC_CTRL_REG, (RTC_CTRL_REG_DM_BIN|RTC_CTRL_REG_DV_NORMAL));

    cc6801_RtcSetTime(tRtcTime);
    NVIC_ClearPendingIRQ(RTC_IRQn);
    NVIC_EnableIRQ(RTC_IRQn);
}

T_Rtc cc6801_RtcGetTime(void)
{
    T_Rtc tRtcTime;
    uint32_t dwTmp;

    // clear call back functions
    memset(&tRtcTime,0x00,sizeof(T_Rtc));

    rd(RTC_SEC_REG,tRtcTime.sec);
    rd(RTC_MIN_REG,tRtcTime.min);
    rd(RTC_HRS_REG,tRtcTime.hour);
    rd(RTC_DOW_REG,tRtcTime.weekDay);
    rd(RTC_DAY_REG,tRtcTime.day);
    rd(RTC_MON_REG,tRtcTime.month);
    rd(RTC_YRS_REG,tRtcTime.year);

    tRtcTime.year += RTC_BASE_YEAR;
    
    rd(RTC_CTRL_REG,dwTmp);
    if (dwTmp&RTC_CTRL_REG_HF_24)
    {
        tRtcTime.h24 = RTC_HOUR_24H;
    }
    else if (tRtcTime.hour & RTC_HRS_REG_PM)
    {
        tRtcTime.hour &=(~RTC_HRS_REG_PM);
        tRtcTime.h24 = RTC_HOUR_PM;
    }
    else
    {
        tRtcTime.h24 = RTC_HOUR_AM;
    }
    return tRtcTime;
}

uint32_t cc6801_RtcSetTime(T_Rtc tTargetTime)
{
    uint32_t errcode;
    errcode = CheckRTC(&tTargetTime);
    if (errcode)
    {
        return errcode;
    }
    wr(RTC_SEC_REG,tTargetTime.sec);
    wr(RTC_MIN_REG,tTargetTime.min);
    
    if (tTargetTime.h24==RTC_HOUR_24H)
    {
        setbit(RTC_CTRL_REG, RTC_CTRL_REG_HF_24);
        wr(RTC_HRS_REG,tTargetTime.hour);
    }
    else
    {
        clrbit(RTC_CTRL_REG, RTC_CTRL_REG_HF_24);
        if (tTargetTime.h24==RTC_HOUR_PM)
        {
            wr(RTC_HRS_REG,(tTargetTime.hour|RTC_HRS_REG_PM));
        }
        else
        {
            wr(RTC_HRS_REG,tTargetTime.hour);
        }
    }
    wr(RTC_DOW_REG,tTargetTime.weekDay);
    wr(RTC_DAY_REG,tTargetTime.day);
    wr(RTC_MON_REG,tTargetTime.month);
    wr(RTC_YRS_REG,(tTargetTime.year-RTC_BASE_YEAR));
    
    return 0;
}

void cc6801_RtcRegisterCallback(T_RtcCallback rtcCB)
{
    // clear call back functions
    memset(&g_rtcCB,0x00,sizeof(T_RtcCallback));
    // clear rtc interrupt enable bits
    clrbit(RTC_INTR_REG,RTC_INTR_REG_IE_RTCMASK);

    if (rtcCB.sec != NULL)
    {
        g_rtcCB.sec = rtcCB.sec;
        setbit(RTC_INTR_REG,RTC_INTR_REG_IE_SEC);
    }

    if (rtcCB.min != NULL)
    {
        g_rtcCB.min = rtcCB.min;
        setbit(RTC_INTR_REG,RTC_INTR_REG_IE_MIN);
    }

    if (rtcCB.hour != NULL)
    {
        g_rtcCB.hour = rtcCB.hour;
        setbit(RTC_INTR_REG,RTC_INTR_REG_IE_HOUR);
    }

    if (rtcCB.day != NULL)
    {
        g_rtcCB.day = rtcCB.day;
        setbit(RTC_INTR_REG,RTC_INTR_REG_IE_DAY);
    }
}

static uint32_t CheckAlarm(T_Alarm * pAlarmData)
{
    uint32_t errcode;
    uint32_t day;
    uint32_t isLeapYear;
    uint32_t dwYear;
    uint32_t dwTmp;
    
    
    errcode = 0;
    //check sec
    if (pAlarmData->sec > 59) 
        errcode |= RTC_ERR_SEC;
    //check min
    if (pAlarmData->min > 59) 
        errcode |= RTC_ERR_MIN;
    //check hour
    rd(RTC_CTRL_REG, dwTmp);
    if (pAlarmData->h24 == RTC_HOUR_24H)
    {
        if (((dwTmp&RTC_CTRL_REG_HF_24)==0) || (pAlarmData->hour > 24))
        {
            errcode |= RTC_ERR_HOUR;
        }
    }
    else if ((pAlarmData->h24 == RTC_HOUR_AM)||(pAlarmData->h24 == RTC_HOUR_PM))
    {
        if ((dwTmp&RTC_CTRL_REG_HF_24)||(pAlarmData->hour > 12))
        {
            errcode |= RTC_ERR_HOUR;
        }
    }
    else
    {
         errcode |= RTC_ERR_HOUR;
    }
    //check month
    if (pAlarmData->month > 12) 
        errcode |= RTC_ERR_MONTH;
    
    //check month day
    rd(RTC_YRS_REG,dwYear);
    dwYear += RTC_BASE_YEAR;
    if( dwYear%400==0 || ( dwYear%4==0 && dwYear%100!=0 ) )
         isLeapYear = 1;
    else
         isLeapYear = 0; 

    day = mdays[pAlarmData->month-1];
    if (pAlarmData->month ==2)
        day += isLeapYear;

    if (pAlarmData->day > day) 
        errcode |= RTC_ERR_DAY;

    return errcode;
}
void cc6801_AlarmInit(void)
{
    //clear all alarm interrupt enable bits
    clrbit(RTC_INTR_REG,RTC_INTR_REG_IE_ALMMASK);
}
T_Alarm cc6801_AlarmGetTime(E_RTCALARMSupported EAlarmNum)
{
    T_Alarm tAlarm;
    uint32_t dwTmp;
    uint32_t dwAddr;

    // clear call back functions
    memset(&tAlarm,0x00,sizeof(T_Alarm));
    if (EAlarmNum==RTC_ALARM)
    {
        dwAddr = RTC_ALM_SEC_REG;
    }
    else if (EAlarmNum==RTC_ALARM2)
    {
        dwAddr = RTC_ALM2_SEC_REG;
    }
    else
    {
        return tAlarm;
    }
    
    rd(dwAddr   ,tAlarm.sec);
    rd(dwAddr+4 ,tAlarm.min);
    rd(dwAddr+8 ,tAlarm.hour);
    rd(dwAddr+12,tAlarm.day);
    rd(dwAddr+16,tAlarm.month);

    rd(RTC_CTRL_REG,dwTmp);
    if (dwTmp&RTC_CTRL_REG_HF_24)
    {
        tAlarm.h24 = RTC_HOUR_24H;
    }
    else if (tAlarm.hour & RTC_HRS_REG_PM)
    {
        tAlarm.hour &=(~RTC_HRS_REG_PM);
        tAlarm.h24 = RTC_HOUR_PM;
    }
    else
    {
        tAlarm.h24 = RTC_HOUR_AM;
    }
    return tAlarm;
}

uint32_t  cc6801_AlarmSetTime(E_RTCALARMSupported EAlarmNum,T_Alarm tAlarm)
{
    uint32_t errcode;
//    uint32_t dwTmp;
    uint32_t dwAddr;

    errcode = CheckAlarm(&tAlarm);

    if (errcode)
    {
        return errcode;
    }

    if (EAlarmNum==RTC_ALARM)
    {
        dwAddr = RTC_ALM_SEC_REG;
        
    }
    else if (EAlarmNum==RTC_ALARM2)
    {
        dwAddr = RTC_ALM2_SEC_REG;
        
    }
    else
    {
        return ALARM_ERR_MASK;
    }

    wr(dwAddr   ,tAlarm.sec);
    wr(dwAddr+4 ,tAlarm.min);
    if (tAlarm.h24==RTC_HOUR_PM)
    {
        wr(dwAddr+8 ,(tAlarm.hour|RTC_HRS_REG_PM));
    }
    else
    {
        wr(dwAddr+8 ,tAlarm.hour);
    }
    wr(dwAddr+12,tAlarm.day);
    wr(dwAddr+16,tAlarm.month);
    return 0;
}

void cc6801_AlarmRegisterCallback(T_AlarmCallback tAlarmCB)
{
    // clear call back functions
    memset(&g_AlarmCB,0x00,sizeof(g_AlarmCB));
    // clear interrupt
    clrbit(RTC_INTR_REG,RTC_INTR_REG_IE_ALMMASK);

    g_AlarmCB.alarm = tAlarmCB.alarm;

    g_AlarmCB.alarm2 = tAlarmCB.alarm2;
}

void cc6801_AlarmEnable(E_RTCALARMSupported EAlarmNum)
{
    if (EAlarmNum==RTC_ALARM)
    {
        setbit(RTC_INTR_REG,RTC_INTR_REG_IE_ALARM);
    }
    else if (EAlarmNum==RTC_ALARM2)
    {
        setbit(RTC_INTR_REG,RTC_INTR_REG_IE_ALARM2);
    }
}

