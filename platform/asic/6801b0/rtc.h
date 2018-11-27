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

#ifndef _RTC_H_
#define _RTC_H_
#include "global.h"
#include "project.h"

#define RTC_BASE_YEAR           2000

//RTC default time is define by project header file
#define RTC_DEFAULT                                                 \
    {                                                               \
        .sec        = RTC_DEFAULT_SEC,                          \
        .min        = RTC_DEFAULT_MIN,                          \
        .hour       = RTC_DEFAULT_HOUR,                         \
        .h24        = RTC_DEFAULT_H24,                          \
        .day        = RTC_DEFAULT_DAY,                          \
        .month      = RTC_DEFAULT_MONTH,                        \
        .weekDay    = RTC_DEFAULT_WEEK,                         \
        .year       = RTC_DEFAULT_YEAR                          \
    }

typedef struct
{
    uint8_t  sec;
    uint8_t  min;
    uint8_t  hour;
    uint8_t  h24;
    uint8_t  day;
    uint8_t  month;
    uint8_t  weekDay;
    uint16_t year;
} T_Rtc;


typedef struct 
{ 
    uint8_t  sec;
    uint8_t  min;
    uint8_t  hour;
    uint8_t  h24;
    uint8_t  day;
    uint8_t  month;
} T_Alarm;

/*
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
} S_RtcInfo;


typedef struct
{
    uint8_t  sec;
    uint8_t  min;
    uint8_t  hour;
    uint8_t  day;
    uint8_t  month;
    uint8_t  h24;
} S_AlarmInfo;
*/
typedef struct
{
    T_callback  sec;
    T_callback  min;
    T_callback  hour;
    T_callback  day;
} T_RtcCallback;

typedef struct
{
    T_callback  alarm;
    T_callback  alarm2;
} T_AlarmCallback;

typedef enum
{
    RTC_ALARM = 0,
    RTC_ALARM2,
    RTC_ALARM_TOTAL_SUPPORTED
}E_RTCALARMSupported;

#define RTC_ERR_SEC      0x00000001
#define RTC_ERR_MIN      0x00000002
#define RTC_ERR_HOUR     0x00000004
#define RTC_ERR_DAY      0x00000008
#define RTC_ERR_MONTH    0x00000010
#define RTC_ERR_YEAR     0x00000020
#define RTC_ERR_MASK     0x0000003F
#define ALARM_ERR_MASK   0x0000001F

void      cc6801_RtcInit(void);
T_Rtc     cc6801_RtcGetTime(void);
uint32_t  cc6801_RtcSetTime(T_Rtc tTargetTime);
void      cc6801_RtcRegisterCallback(T_RtcCallback tCB);

void      cc6801_AlarmInit(void);
T_Alarm   cc6801_AlarmGetTime(E_RTCALARMSupported bAlarmNum);
uint32_t  cc6801_AlarmSetTime(E_RTCALARMSupported bAlarmNum,T_Alarm tAlarm);
void      cc6801_AlarmRegisterCallback(T_AlarmCallback tCB);
void      cc6801_AlarmEnable(E_RTCALARMSupported bAlarmNum);




//rtc, alarm, and alarm2 use the same definition for PM bit 
#define RTC_HRS_REG_AM                  0x00000000
#define RTC_HRS_REG_PM                  0x00000080



//#define RTC_CTRL_REG                (RTC_ADDR_BASE + 0x00000044)
#define RTC_CTRL_REG_DM                 0x10000000  //data mode
    #define RTC_CTRL_REG_DM_BIN         0x10000000  //data mode binary
    #define RTC_CTRL_REG_DM_BCD         0x00000000  //data mode BCD
#define RTC_CTRL_REG_HF                 0x08000000  //hour format
    #define RTC_CTRL_REG_HF_24          0x08000000  //24-hour
    #define RTC_CTRL_REG_HF_12          0x00000000  //12-hor
#define RTC_CTRL_REG_DSE                0x04000000  //day time saving enable
#define RTC_CTRL_REG_DV                 0x03000000  //division chain selec
    #define RTC_CTRL_REG_DV_TEST        0x00000000  //test mode
    #define RTC_CTRL_REG_DV_CAB         0x01000000  //calibration mode
    #define RTC_CTRL_REG_DV_NORMAL      0x02000000  //normal mode
    #define RTC_CTRL_REG_DV_DISABLE     0x03000000  //rtc disable
#define RTC_CTRL_REG_CAB_S              0x00200000  //clock calibration delta sign bit
    #define RTC_CTRL_REG_CAB_SP         0x00000000  //plus sign
    #define RTC_CTRL_REG_CAB_SN         0x00200000  //negtive sign
#define RTC_CTRL_REG_CABMASK            0x001FFFFF


//#define RTC_INTR_REG                (RTC_ADDR_BASE + 0x00000050)
#define RTC_INTR_REG_IE_SEC     0x00000001
#define RTC_INTR_REG_IE_ALARM   0x00000002
#define RTC_INTR_REG_IE_ALARM2  0x00000004
#define RTC_INTR_REG_IE_MIN     0x00000008
#define RTC_INTR_REG_IE_HOUR    0x00000010
#define RTC_INTR_REG_IE_DAY     0x00000020
#define RTC_INTR_REG_IE_RTCMASK 0x00000039
#define RTC_INTR_REG_IE_ALMMASK 0x00000006

#define RTC_INTR_REG_ST_SEC     0x00010000
#define RTC_INTR_REG_ST_ALARM   0x00020000
#define RTC_INTR_REG_ST_ALARM2  0x00040000
#define RTC_INTR_REG_ST_MIN     0x00080000
#define RTC_INTR_REG_ST_HOUR    0x00100000
#define RTC_INTR_REG_ST_DAY     0x00200000
#define RTC_INTR_REG_ST_MASK    0x003F0000

#endif //_RTC_H_


