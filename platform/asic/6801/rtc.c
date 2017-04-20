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


void cc6801_rtcInit(void)
{
    //rtc init will set HW RTC to factory default time 2017-1-1 Sun 00:00:00
    S_rtcInfo rtc = RTC_DEFAULT;

    //disable all interrupt
    regRTC->dw.interrupt = 0;
    
    //Init RTC counter
    regRTC->bf.sec = rtc.sec;
    regRTC->bf.min = rtc.min;
    regRTC->bf.hour = rtc.hour;
    regRTC->bf.day = rtc.day;
    regRTC->bf.weekDay = rtc.weekDay;
    regRTC->bf.year = rtc.year;

    //clear alarm data
    regRTC->dw.alarm1Sec = 0;
    regRTC->dw.alarm1Min = 0;
    regRTC->dw.alarm1Hour = 0;
    regRTC->dw.alarm1Day = 0;
    regRTC->dw.alarm1Month = 0;
    regRTC->dw.alarm2Sec = 0;
    regRTC->dw.alarm2Min = 0;
    regRTC->dw.alarm2Hour = 0;
    regRTC->dw.alarm2Day = 0;
    regRTC->dw.alarm2Month = 0;

}










