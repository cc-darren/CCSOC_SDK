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

#ifndef APP_DATE_TIME_H__
#define APP_DATE_TIME_H__

#include <stdint.h>

#define APP_MON_JAN                            ((int8_t)1)
#define APP_MON_FEB                            ((int8_t)2)
#define APP_MON_MAR                            ((int8_t)3)
#define APP_MON_APR                            ((int8_t)4)
#define APP_MON_MAY                            ((int8_t)5)
#define APP_MON_JUN                            ((int8_t)6)
#define APP_MON_JUL                            ((int8_t)7)
#define APP_MON_AUG                            ((int8_t)8)
#define APP_MON_SEP                            ((int8_t)9)
#define APP_MON_OCT                            ((int8_t)10)
#define APP_MON_NOV                            ((int8_t)11)
#define APP_MON_DEC                            ((int8_t)12)

#define APP_WEEK_SUNDAY                         ((int8_t)0)
#define APP_WEEK_MONDAY                         ((int8_t)1)
#define APP_WEEK_TUESDAY                        ((int8_t)2)
#define APP_WEEK_WEDNESDAY                      ((int8_t)3)
#define APP_WEEK_THURSDAY                       ((int8_t)4)
#define APP_WEEK_FIRDAY                         ((int8_t)5)
#define APP_WEEK_SATURDAY                       ((int8_t)6)


typedef struct
{
    int16_t year;
    int8_t  month;
    int8_t  day;
    int8_t  hours;
    int8_t  minutes;
    int8_t  seconds;
    int8_t  dayofweek;
} app_date_time_t;

typedef struct{
  app_date_time_t systime;

  uint8_t is_updated;
  uint8_t inited;
  uint8_t is_db_init_done;  
  
}app_sys_time_t;

typedef struct app_interval {
    int8_t day;                                /**< -32768 ~ 32768 */
    int8_t hour;                               /**< 0 ~ 23         */
    int8_t minute;                             /**< 0 ~ 59         */
    int8_t second;                             /**< 0 ~ 59         */

} app_interval_t;

void app_Time_Init(void);
void app_UpdatTimeDate(const uint8_t *data);
app_date_time_t app_Time_Proc(uint8_t _cnt);
app_date_time_t app_getSysTime(void);
app_date_time_t app_time_update(const int8_t s);
app_date_time_t app_time_add(const app_date_time_t t, const app_interval_t n);
int8_t app_time_is_valid(const app_date_time_t t);
uint8_t app_Get_Update_Time_Flag(void);
uint8_t app_Check_Time_And_DB_Init(void);
void app_Set_DB_Init_Done(void);
void app_Force_Init_DB(void);
app_interval_t app_time_to_interval(const app_date_time_t t1,const app_date_time_t t2);
void  app_interval_make(app_interval_t *_stInterval ,uint32_t _dwTimeOfSeconds);
void  app_interval_make_64bits(app_interval_t *_stInterval ,unsigned long long _nTimeOfSeconds);
uint8_t app_caculation_dayofweek(const app_date_time_t tl);


#endif
