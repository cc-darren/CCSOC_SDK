/******************************************************************************
*  Copyright 2017, Cloudchip, Inc.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Cloudchip, Inc. (C) 2017
******************************************************************************/

/******************************************************************************
*  Filename:
*  ---------
*
*
*  Project:
*  --------
*  CC6801
*
*  Description:
*  ------------
*  the description
*
*  Author:
*  -------
*  Mush     (cc-mush)
*
*===========================================================================
*
******************************************************************************/

/******************************************************************************
Head Block of The File
******************************************************************************/
#include "test.h"

#if (TEST_ALARM)

#include "test_alarm.h"
#include "drvi_rtc.h"

/******************************************************************************
Declaration of External Variables & Functions
******************************************************************************/

/******************************************************************************
Declaration of data structure
******************************************************************************/


/******************************************************************************
Declaration of Global Variables & Functions
******************************************************************************/
 uint32_t _calCnt;
void Tsec(void * p)
{
_calCnt+= 0x10000000;
}
void Tmin(void * p)
{
_calCnt+= 0x00010000;
}
void Thour(void * p)
{
_calCnt+= 0x00000100;
}
void Tday(void * p)
{
_calCnt+= 0x00000001;
}

 uint32_t _alarmcalCnt;
void Talarm(void * p)
{
_alarmcalCnt+= 0x00000001;
}

void Talarm2(void * p)
{
_alarmcalCnt+= 0x00010000;
}
/******************************************************************************
Declaration of static Global Variables & Functions
******************************************************************************/

void test_ALARM(void)
{
    T_Rtc tTMPRTC;
    T_RtcCallback calbacks={0};
    T_AlarmCallback alarmCBs;
    
    //calbacks.sec = &Tsec;
    calbacks.min = &Tmin;
    calbacks.hour = &Thour;
    calbacks.day = &Tday;
    drvi_RtcRegisterCallback(calbacks);
    
    drvi_RtcInit();
    tTMPRTC = drvi_RtcGetTime();
    
    
    tTMPRTC.sec         = 35;
    tTMPRTC.min         = 12;
    tTMPRTC.hour        = 13;
    tTMPRTC.h24         = RTC_HOUR_24H;
    tTMPRTC.day         = 11;
    tTMPRTC.month       = 2;
    tTMPRTC.weekDay     = 8;
    tTMPRTC.year        = 2019;

    drvi_RtcSetTime(tTMPRTC);

    alarmCBs.alarm = &Talarm;
    alarmCBs.alarm2 = &Talarm2;
    
    drvi_AlarmRegisterCallback(alarmCBs);
    drvi_AlarmInit();
    
    tTMPRTC.sec+=10;
    drvi_AlarmSetTime(RTC_ALARM,(*(T_Alarm *)(&tTMPRTC)));
    drvi_AlarmSetTime(RTC_ALARM2,(*(T_Alarm *)(&tTMPRTC)));
    drvi_AlarmEnable(RTC_ALARM);
    drvi_AlarmEnable(RTC_ALARM2);
}

#endif
