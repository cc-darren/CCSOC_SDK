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

#ifndef _CC_DB_STRUCTURE_H_
#define _CC_DB_STRUCTURE_H_


#include "project.h"
#include "project.h"
// Venus DB definition:


// System:

typedef struct 
{
    uint32_t boot_verify_code;
}db_sys_first_boot_record_t   __attribute__((aligned(4)));


typedef struct 
{
    uint8_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t min;
    uint8_t dayofweek;
    uint16_t rsvd;
    uint32_t magic_code;
}db_sys_first_use_record_t  __attribute__((aligned(4)));


typedef struct 
{
    uint8_t height;
    uint8_t weight;
    uint8_t age;
    uint8_t gender;
    uint8_t stride_len;
    uint8_t swim_pool_size;
    uint8_t bBandLocation;
    uint8_t bRestingHrMax;
    uint8_t bRestingHrMin;
    uint8_t bExerciseHrMax;
    uint8_t bExerciseHrMin;
    
    uint8_t rsvd;
}db_sys_general_info_t  __attribute__((aligned(4)));


typedef struct 
{
    uint8_t setting;
    uint8_t hour;
    uint8_t min;
    uint8_t rsvd;
}db_sys_alarm_t  __attribute__((aligned(4)));

typedef struct 
{
    eStete_t eIsHrsEnabled;
    eStete_t eIs24HrEnabled;

    uint32_t dw24HrPeriodicMeasurementTime;    // ms
    uint32_t dw24HrOneMeasurementMaxTime;      // ms
}db_sys_hr_setting_t  __attribute__((aligned(4)));

typedef struct 
{
    uint8_t incomming_call_en;
    uint8_t incomming_sms_en;
    uint8_t longsit_en;
    uint8_t lifearm_en;    
}db_sys_notify_enabled_t __attribute__((aligned(4)));


typedef struct 
{
    uint8_t length;
    uint8_t weight;
    uint8_t swim_len;
    uint8_t rsvd[1];
}db_sys_unit_t __attribute__((aligned(4)));

typedef struct 
{
    uint8_t start_time_hour;
    uint8_t start_time_min;
    uint8_t end_time_hour;
    uint8_t end_time_min;
}db_sys_longsit_t __attribute__((aligned(4)));

typedef struct 
{
    uint8_t start_time_hour;
    uint8_t start_time_min;
    uint8_t end_time_hour;
    uint8_t end_time_min;
}db_sys_sleep_monitor_t __attribute__((aligned(4)));

typedef struct 
{
    uint32_t id;
}db_sys_device_info_t __attribute__((aligned(4)));


typedef struct 
{
   db_sys_first_boot_record_t       first_boot;
}db_frist_boot_t __attribute__((aligned(4))); //rec id 0x0001


typedef struct 
{
   uint32_t     device_id;
}db_sys_reserved_t __attribute__((aligned(4))); 

typedef struct 
{
   uint16_t     x;
   uint16_t     y;
   uint16_t     z;
   uint16_t     rsvd;
}db_sys_dyna_gyro_t __attribute__((aligned(4))); 

typedef struct 
{
   int16_t    Data[3];
   uint16_t     wVaildFlag;
}db_sys_static_gyro_offset_t __attribute__((aligned(4))); 


typedef struct 
{
    db_sys_first_use_record_t   time;
    db_sys_general_info_t       info;
    db_sys_alarm_t              alarm;
    db_sys_notify_enabled_t     notify_en;
    db_sys_unit_t               unit;
    db_sys_sleep_monitor_t      sleep;
    db_sys_dyna_gyro_t          dyna_gyro_offset;
    db_sys_static_gyro_offset_t static_gyro_offset;
    db_sys_reserved_t           reserved;
}db_system_t __attribute__((aligned(4))); //recid 0x0002

typedef struct 
{
      db_sys_first_use_record_t     first_record;
}db_frist_use_t __attribute__((aligned(4))); //rec id 0x0003

typedef struct 
{
        db_sys_device_info_t      device_info;
}db_system_device_info_t __attribute__((aligned(4))); //0x0004


typedef struct 
{
    uint8_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t min;
    uint8_t sec;
    uint8_t rsvd[2];
}db_pedo_time_t __attribute__((aligned(4)));

typedef struct 
{
    uint8_t state;
    uint8_t rsvd[3];
}db_pedo_state_t __attribute__((aligned(4)));


typedef struct 
{
    uint16_t count;
    uint16_t padding;
}db_pedo_single_count_t __attribute__((aligned(4)));

typedef struct 
{
    uint8_t hour;
    uint8_t min;
    uint8_t sec;
    uint8_t rsvd[1];
}db_pedo_end_of_time_t __attribute__((aligned(4)));



typedef struct 
{
    //db_pedo_record_count_t    record;    
    db_pedo_time_t          time;
    db_pedo_state_t         ped_state;
    db_pedo_single_count_t  ped_single;
    db_pedo_end_of_time_t   endoftime;
    uint16_t                calorie;
    uint16_t                rsvd;
}db_pedometer_t __attribute__((aligned(4)));



#ifdef BatteryLog_EN
typedef struct 
{
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t min;
}db_MixLog_time_t __attribute__((aligned(4)));

typedef struct
{
    uint8_t     BatteryLevel;
    uint16_t    BatteryADCValue;
}db_MixlogBattery;

#define BAS_LOG_MAX_RECORDS    360
#define BAS_LOG_FULL           (BAS_LOG_MAX_RECORDS - 1)
typedef struct
{
    db_MixLog_time_t    time;
    db_MixlogBattery    battery[BAS_LOG_MAX_RECORDS];
}db_mixlog_t __attribute__((aligned(4)));
#endif


// HRM
/*
typedef struct 
{
    uint16_t count;
    uint8_t rsvd[2];
}db_hrm_record_count_t __attribute__((aligned(4)));
*/

typedef struct 
{
    uint8_t year;    
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t min;
    uint8_t sec;
    uint8_t rsvd[2];
}db_hrm_time_t __attribute__((aligned(4)));


typedef struct 
{
    uint8_t hr;
    uint8_t trust_level;
    uint8_t rsvd[2];
}db_hrm_data_t __attribute__((aligned(4)));


typedef struct 
{
    //db_hrm_record_count_t    record;
    db_hrm_time_t       time;
    db_hrm_data_t       data;
}db_heartrate_t __attribute__((aligned(4)));



// Sleep

typedef struct 
{
    uint16_t count;
    uint8_t rsvd[2];
}db_sleep_record_count_t __attribute__((aligned(4)));


typedef struct 
{
    uint8_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t min;
    uint8_t sec;
    uint8_t sleep_state;
    uint8_t bCommandHeader;
}db_sleep_time_t __attribute__((aligned(4)));


typedef struct 
{
    uint32_t exec_time_second;
    uint16_t period_min;
}db_sleep_time_period_t __attribute__((aligned(4)));


typedef struct 
{
    db_sleep_time_t         detect_time;
    db_sleep_time_period_t  period;
}db_sleep_t __attribute__((aligned(4)));


// Swimming:
typedef struct 
{
    uint16_t total_recordcount;
    uint8_t  section_num;
     union {
        uint8_t    cInfo;
        struct {
            uint8_t    swim_pool_sizes:4;
            uint8_t    swimming_type:4;
        }bInfo;
    } uInfo;
}db_swim_section_t __attribute__((aligned(4)));

typedef struct 
{
    uint8_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t min;
    uint8_t sec;
    uint16_t lap_number;
}db_swim_time_t __attribute__((aligned(4)));


typedef struct 
{
    uint16_t swimming_count;
    uint16_t time_period;    
}db_swim_end_of_time_t __attribute__((aligned(4)));


typedef struct 
{
    db_swim_section_t         section;
    db_swim_time_t            start_time;
    db_swim_end_of_time_t     endofperiod;
}db_swimming_t __attribute__((aligned(4)));


#endif // _CC_DB_STRUCTURE_H_
