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

#include <stdint.h>
#include <string.h>
#include "project.h"
#include "cc_db.h"
#include "tracer.h"

#ifdef SLEEP_EN
extern void CC_SleepMonitor_Srv_Enable_SleepService(eStete_t eStete);
extern eStete_t CC_SleepMonitor_Srv_Get_SleepService(void);
#endif

#ifdef DB_EN
extern void CC_DB_Init(uint8_t _bState);
extern void CC_DB_Force_Execute_Init(uint8_t init_type);
extern uint8_t CC_DB_Get_Init_Type(void);
extern bool CC_DB_Check_Init_Done(void);
#endif

app_sys_time_t m_SystemTime; // no static for test


extern void CC_DB_Init(uint8_t _bState);


static __inline int32_t time_to_doc(const app_date_time_t t)
{
    int16_t y = t.year - (t.month < 3);
    int16_t era = (y >= 0 ? y : y - 399)/400;
    int16_t yoe = (y - era*400);
    int16_t doy = (153 * (t.month + (t.month > 2 ? -3 : 9)) + 2)/5 + t.day - 1;
    int32_t doe = (int32_t)(yoe*365 + yoe/4 - yoe/100 + doy);

    return era*146097 + doe - 719468;
}

/* Get number of days by month */
static __inline int16_t time_is_leap(const app_date_time_t t)
{
    return(t.year % 4 == 0) && (t.year % 100 != 0 || t.year % 400 == 0);
}

static __inline int8_t time_to_dom_common(const app_date_time_t t)
{
    const int16_t tbl[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    return tbl[t.month - 1];
}

static __inline int8_t time_to_dom(const app_date_time_t t)
{
    return(t.month != APP_MON_FEB || !time_is_leap(t)) ? time_to_dom_common(t) : 29;
}

static __inline void move_carry__(int8_t *h,
                                  int8_t *l,
                                  const int8_t hbase,
                                  const int8_t lbase,
                                  const int8_t radix)
{
    int8_t vh = *h - hbase, vl = *l - lbase;

    if (vl >= radix) {
        vh += (vl / radix);
        vl = (vl % radix);

    } else if (vl < 0) {
        vh += ((vl - radix + 1)/radix);
        vl -= ((vl - radix + 1)/radix*radix);
    }

    vh += hbase;
    vl += lbase;

    *h = vh;
    *l = vl;

}

static __inline void move_carry_week_(int8_t *h,
                                  int8_t *l,
                                  int8_t *w,
                                  const int8_t hbase,
                                  const int8_t lbase,
                                  const int8_t radix)
{
    int8_t vh = *h - hbase, vl = *l - lbase;

    if (vl >= radix) {
        vh += (vl / radix);
        vl = (vl % radix);
        if (*w <6)
          *w+=1;
        else
          *w=0;
    } else if (vl < 0) {
        vh += ((vl - radix + 1)/radix);
        vl -= ((vl - radix + 1)/radix*radix);
    }

    vh += hbase;
    vl += lbase;

    *h = vh;
    *l = vl;

}



static __inline app_date_time_t time_move_carry(const app_date_time_t t)
{
    app_date_time_t r = t;

    int16_t dom;

    move_carry__(&r.minutes, &r.seconds, 0, 0, 60);
    move_carry__(&r.hours, &r.minutes, 0, 0, 60);
    move_carry_week_(&r.day, &r.hours, &r.dayofweek, 1, 0, 24);
  

    while (r.day > (dom = time_to_dom(r))) {
        r.day -= dom;
        ++r.month;
        if (r.month > APP_MON_DEC) {
            r.month = APP_MON_JAN;
            ++r.year;
        }
    }

    while (r.day < 1) {
        --r.month;
        if (r.month < APP_MON_JAN) {
            r.month = APP_MON_DEC;
            --r.year;
        }
        r.day += time_to_dom(r);
    }

    return r;
}

static __inline void time_set_ymd(app_date_time_t *t,
                                  const int16_t year,
                                  const int8_t month,
                                  const int8_t day,
                                  const int8_t hour,
                                  const int8_t minute,
                                  const int8_t second,
                                  const int8_t dayofweek)
{
    t->year = year;
    t->month = month;
    t->day = day;
    t->hours = hour;
    t->minutes = minute;
    t->seconds = second;
    t->dayofweek = dayofweek;
}

static __inline app_interval_t interval_move_carry(const app_interval_t n)
{
    app_interval_t r = n;

    move_carry__(&r.minute, &r.second, 0, 0, 60);
    move_carry__(&r.hour, &r.minute, 0, 0, 60);
    move_carry__(&r.day, &r.hour, 0, 0, 24);

    return r;
}


void app_Time_Init(void)
{
  if (1 != m_SystemTime.inited )
  {
    memset(&m_SystemTime,0x00,sizeof(m_SystemTime));
    time_set_ymd(&m_SystemTime.systime, 2018,APP_MON_JAN,2,17,45,00,APP_WEEK_TUESDAY);

    m_SystemTime.inited     = 1;
  }

}

uint8_t app_Get_Update_Time_Flag(void)
{
    return m_SystemTime.is_updated;
}

uint8_t app_Check_Time_And_DB_Init(void)
{

    if(m_SystemTime.is_db_init_done == 0)
        return 0x01;
    else
        return 0x00;    
}

void app_Set_DB_Init_Done(void)
{
    m_SystemTime.is_db_init_done = 1;
}

void app_Force_Init_DB(void)
{
    m_SystemTime.is_db_init_done = 0;
}

void app_UpdatTimeDate(const uint8_t *data)
{

    if(1 == m_SystemTime.inited)
    {

        m_SystemTime.systime.year         = (*data++) + 2000;
        m_SystemTime.systime.month         = (*data++);
        m_SystemTime.systime.day           = (*data++);
        m_SystemTime.systime.hours      = *data++;
        m_SystemTime.systime.minutes     = *data++;
        m_SystemTime.systime.seconds    = *data++;
        m_SystemTime.systime.dayofweek = *data;    

        if(0x00 == CC_DB_Check_Magic_Code())
        {
            CC_DB_System_Save_Request(DB_SYS_TIME);	
            // Save Init Data to DB
            CC_DB_System_Save_Request(DB_SYS_GENERAL_INFO);
            CC_DB_System_Save_Request(DB_SYS_ALARM);
            CC_DB_System_Save_Request(DB_SYS_NOTIFY);
            CC_DB_System_Save_Request(DB_SYS_UNIT);
            CC_DB_System_Save_Request(DB_SYS_LONGSIT_TIME_SETTING);
        }

        if( false ==app_Get_Update_Time_Flag())
        {
#ifdef SLEEP_EN            
            CC_SleepMonitor_Srv_Enable_SleepService(eEnable);
#endif
            //if((DB_INIT_FROM_APP_FACTORY_RESET == CC_DB_Get_Init_Type()) || (true == CC_DB_Check_Init_Done())) 
            //{
            //    // do nothing;
            //}
            //else
            //{
            //    CC_DB_Force_Execute_Init(DB_INIT_FROM_APP);
            //}
            
        }
        else
        {
#ifdef SLEEP_EN            
            if (eDisable == CC_SleepMonitor_Srv_Get_SleepService())
            {
                CC_SleepMonitor_Srv_Enable_SleepService(eEnable);
            }
#endif            
        }

        m_SystemTime.is_updated     = 1;

        // Jason, Algin dayofweek to DB current time fileID and RecID 
        //CC_Fds_Change_DayofWeek(m_SystemTime.systime.dayofweek,false);
    }
}
app_date_time_t app_Time_Proc(uint8_t _cnt)
{

  app_time_update(_cnt);    
  //TracerInfo("System Date %d.%d.%d %d \r\n",m_SystemTime.systime.year,m_SystemTime.systime.month,m_SystemTime.systime.day,m_SystemTime.systime.dayofweek);
  //TracerInfo("System Clock %d:%d:%d \r\n",m_SystemTime.systime.hours,m_SystemTime.systime.minutes,m_SystemTime.systime.seconds); 
  return m_SystemTime.systime;


}
app_date_time_t app_getSysTime(void)
{
  return m_SystemTime.systime;
}



app_date_time_t app_time_update(const int8_t s)
{
    app_interval_t n = {0, 0, 0, s};

    if (!m_SystemTime.inited)
        return m_SystemTime.systime;

    m_SystemTime.systime = app_time_add(m_SystemTime.systime, interval_move_carry(n));

    return m_SystemTime.systime;
}

app_date_time_t app_time_add(const app_date_time_t t, const app_interval_t n)
{
    app_date_time_t time = { 0 };
    app_interval_t ntv = n;

    if (app_time_is_valid(t)) {
        time = t;
        time.day += ntv.day;
        time.hours += ntv.hour;
        time.minutes += ntv.minute;
        time.seconds += ntv.second;

        return time_move_carry(time);
    }
    return time;
            
}

int8_t app_time_is_valid(const app_date_time_t t)
{
    
    if (t.seconds >= 0 && t.seconds < 60 &&
        t.minutes >= 0 && t.minutes < 60 &&
        t.hours >= 0 && t.hours < 24 &&
        t.month >= APP_MON_JAN && t.month <= APP_MON_DEC &&
        t.day > 0 && t.day <= time_to_dom(t)) {
        return 1;
    }
    return 0;
}

app_interval_t app_time_to_interval(const app_date_time_t t1,
                                    const app_date_time_t t2)
{
    app_interval_t n = {0};

    int32_t diff;

    if (app_time_is_valid(t1) && app_time_is_valid(t2)) {
        diff = time_to_doc(t2) - time_to_doc(t1);
        if (diff > -32768 && diff <= 32768) {
            n.day = diff;
            n.hour = t2.hours - t1.hours;
            n.minute = t2.minutes - t1.minutes;
            n.second = t2.seconds - t1.seconds;
            return interval_move_carry(n);
        }
    }
    return n;
}

app_interval_t app_interval_normalize(const app_interval_t n)
{
    return interval_move_carry(n);
}

void  app_interval_make(app_interval_t *_stInterval ,uint32_t _dwTimeOfSeconds)
{
    
    _stInterval->day  = _dwTimeOfSeconds/86400;
    _stInterval->hour = (_dwTimeOfSeconds %86400)/3600;
    _stInterval->minute = (_dwTimeOfSeconds%3600)/60;
    _stInterval->second  = ((_dwTimeOfSeconds%3600)%60);

}


void  app_interval_make_64bits(app_interval_t *_stInterval ,unsigned long long _qwTimeOfSeconds)
{

    _stInterval->day  = (int8_t)((unsigned long long)_qwTimeOfSeconds/86400);
    _stInterval->hour = (int8_t)((unsigned long long)(_qwTimeOfSeconds %86400)/3600);
    _stInterval->minute = (int8_t)((unsigned long long)(_qwTimeOfSeconds%3600)/60);
    _stInterval->second  = (int8_t)((unsigned long long)((_qwTimeOfSeconds%3600)%60));

}

uint8_t app_caculation_dayofweek(const app_date_time_t tl)
{
    uint32_t W[12]={6, 2, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};

    uint32_t y, m, d, w;

    app_date_time_t time = { 0 };
    if (app_time_is_valid(tl))
    {
        time = tl;
        y = time.year;
        m = time.month;
        d = time.day;

        w=W[m-1]+y+(y/4)-(y/100)+(y/400);

        if( ((y%4)==0) && (m<3) )
        {
            w--;

            if((y%100)==0)
                w++;
            if((y%400)==0)
                w--;

        }
        return (w+d)%7;
    }

    return time.dayofweek;
}
