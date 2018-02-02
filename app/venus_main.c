/**
 ****************************************************************************************
 *
 * @file venus_main.c
 *
 * @brief Main loop of the application.
 *
 * Copyright (C) CloudChip 2017-2019
 *
 *
 ****************************************************************************************
 */


/*
 * INCLUDES
 ****************************************************************************************
 */
#include <string.h>
#include "ll.h"
#include "drvi_init.h"
#include "tracer.h"
#include "project.h"

//#include "wktm.h"
//#include "drvi_wktm.h"
#include "sw_timer.h"
#include "appdisplay.h"
#include "CC_PageMgr.h"
#include "cc_algo_event.h"
#include "cc_vib_service.h"
#include "acc_lsm6ds3.h"
#include "mag_ak09912.h"
#include "CC_SleepMonitor_Service.h"
#include "clock.h"
#include "CC_FP_PED_8Bit.h"
#include "CC_Calorie_burn.h"
#include "cc_algo.h"
#include "cc_swim_detection.h"
#include "CC_Liftarm.h"
#include "CC_Longsit_Service.h"
#include "app_ht.h"
#include "htpt_task.h"
#include "pah8series_data_c.h"
#include "CC_Slpmtr.h"
#include "cc_appsrvc_heartrate.h"
#include "app_batt.h"
#include "app_ht.h"
#include "cc_hrm_drv.h"
#include "CC_DisplayService.h"
#include "CC_AppSrvc_HeartRate.h"
#include "llm.h"
#include "app.h"
#include "CC_DB.h"
#include "CC_DB_Structure.h"
#include "jump_table.h"
#include "app_ccps.h"
#include "Acc_Gyro_Controller.h"
#include "htpt_task.h"
#include "scheduler.h"
//#include "svc_mgr.h"
#ifdef APP_SERV_MGR_EN
#include "CC_AppSrvc_Manager.h"
#include "CC_Sensor_Manager.h"
#endif
#ifdef SW_TIMER_BY_KERNEL
#include "app_task.h"
#endif

#ifdef APP_VIB_MGR
#include "App_Vib_Manager.h"
#include "App_Vib_Pattern.h"
#endif

#define APP_TIMER_PRESCALER        0
#define APP_TIMER_OP_QUEUE_SIZE    4
#define DATETIME_CNT                  1000

#define SPEED_AND_CADENCE_MEAS_INTERVAL     1000

#define DB_PEDO_WALK_TIMEOUT_INTERVAL        360000// 6min //180000    // 3 min
#define DB_PEDO_RUN_TIMEOUT_INTERVAL        360000// 6min //180000    // 3 min

#define LOCK_SWIM_OFF_TIMEOUT_INTERVAL        180000 // 3min


#define ACCEL_FIFO_MODE_LONG_INTERVAL        1000        // in ms
#define ACCEL_FIFO_MODE_SHORT_INTERVAL        300        // in ms
#define ACCEL_FIFO_MODE_SWIM_INTERVAL        100         // in ms
#define ACCEL_POLL_MODE_INTERVAL        20        // in ms
#define MAG_BUF_LEN                     10

#define BATTERY_LEVEL_MEAS_INTERVAL     60000


#define VENUS_EVENT_ON(EventID,Data)                             \
        {                                                              \
            s_tVenusCB.taEvent[EventID].bCount = 1;                   \
            s_tVenusCB.taEvent[EventID].dwData= Data;          \
        }

#define VENUS_EVENT_OFF(EventID)             {    s_tVenusCB.taEvent[EventID].bCount = 0;    }
#define VENUS_EVENT_IS_ON(EventID)           (    s_tVenusCB.taEvent[EventID].bCount         )
// VENUS CONTROL BLOCK & DEFINITION =========================================

// VENUS CONTROL BLOCK & DEFINITION =========================================
enum
{
    E_VENUS_EVENT_SENSOR_DATA_READY_ACCEL = 0,
    E_VENUS_EVENT_SENSOR_DATA_READY_MAG,
    E_VENUS_EVENT_SENSOR_DATA_READY_GYRO,
    E_VENUS_EVENT_SENSOR_DATA_READY,

    E_VENUS_EVENT_HRM,
    E_VENUS_EVENT_HRM_TIMEOUT,
    E_VENUS_EVENT_HRM_SERVICE_24HR_START,
    E_VENUS_EVENT_HRM_SERVICE_24HR_STOP,
    E_VENUS_EVENT_HRM_SERVICE_24HR_TO_ONE_MEASUREMENT,
    E_VENUS_EVENT_HRM_SERVICE_24HR_TO_PERIODIC_MEASUREMENT,
    E_VENUS_EVENT_HRM_SERVICE_RESUME,
    E_VENUS_EVENT_HRM_SERVICE_HR_LOCK,
    E_VENUS_EVENT_HRM_SERVICE_HR_UNLOCK,
    E_VENUS_EVENT_HRM_SERVICE_HR_ALERT_OUT_OF_RANGE,

    E_VENUS_EVENT_OLED_UPDATE,
    E_VENUS_EVENT_OLED_UPDATE_LOWPOWER, //verify
    E_VENUS_EVENT_OLED_UPDATE_TOUCH,
    E_VENUS_EVENT_OLED_UPDATE_SWIM_ON,
    E_VENUS_EVENT_OLED_UPDATE_SWIM_OFF,
    E_VENUS_EVENT_OLED_UPDATE_SWIM_CONFIRM_ON,
    E_VENUS_EVENT_OLED_UPDATE_SWIM_CONFIRM_OFF,
    E_VENUS_EVENT_OLED_UPDATE_CHARGEIN,
    E_VENUS_EVENT_OLED_UPDATE_CHARGEOUT,
    E_VENUS_EVENT_OLED_UPDATE_CHARGEFULL,
    E_VENUS_EVENT_OLED_UPDATE_HRM_DATA,
    E_VENUS_EVENT_OLED_UPDATE_HRM_TIMEOUT,
    E_VENUS_EVENT_OLED_UPDATE_HRM_HEARTRATESTRAPMODE_ON,
    E_VENUS_EVENT_OLED_UPDATE_HRM_HEARTRATESTRAPMODE_OFF,
    E_VENUS_EVENT_OLED_UPDATE_LIFTARM_UP,
    E_VENUS_EVENT_OLED_UPDATE_LIFTARM_DOWN,
    E_VENUS_EVENT_OLED_UPDATE_LONGSIT,
    E_VENUS_EVENT_OLED_UPDATE_INCOMMINGCALL_ON,
    E_VENUS_EVENT_OLED_UPDATE_INCOMMINGCALL_OFF,
    E_VENUS_EVENT_OLED_UPDATE_INCOMMINGSMS,
    E_VENUS_EVENT_OLED_UPDATE_ALARM,
    E_VENUS_EVENT_OLED_UPDATE_PAIR_SUCCESS,
    E_VENUS_EVENT_OLED_UPDATE_PAIR_PASSKEY,
    E_VENUS_EVENT_OLED_UPDATE_PAIR_FAIL,
    E_VENUS_EVENT_OLED_UPDATE_PEDGOALACHIEVED,
    E_VENUS_EVENT_OLED_UPDATE_SWIMMINGGOALACHIEVED,

    E_VENUS_EVENT_OLED_WAKEUP,
    E_VENUS_EVENT_OLED_SLEEP,
    E_VENUS_EVENT_OLED_DSPSRV,
    E_VENUS_EVENT_PWM_VIBSRV,

    E_VENUS_EVENT_TOUCH_INT,
    E_VENUS_EVENT_LONG_TOUCH_INT,

    E_VENUS_EVENT_BLE_NOTIFY_TO,

    E_VENUS_EVENT_CHARGING_INT,

    E_VENUS_EVENT_CHARGING_FULL,

    E_VENUS_EVENT_DATETIMEFLUSH,

    E_VENUS_EVENT_LONGSITEVENT,

    E_VENUS_EVENT_BATTERYINDICATION_CHARGE_IN,
    E_VENUS_EVENT_BATTERYLIFE,
    E_VENUS_EVENT_BATTERYLIFE_CHARGEOUT,

    E_VENUS_EVENT_LONGSIT_LAUNCH_HRM_TO_CHECK_WEAR,
    E_VENUS_EVENT_LONGSIT_LAUNCH_1MIN_TO_MONITOR_MOTION,
#ifdef FACTORY_RESET
    E_VENUS_EVENT_OLED_UPDATE_FACTORYRESET_START,
    E_VENUS_EVENT_OLED_UPDATE_FACTORYRESET_STOP,
    E_VENUS_EVENT_FACTORY_RESET_START,
    E_VENUS_EVENT_OLED_UPDATE_FACTORYRESET_DONE,

#endif

#ifdef APP_VIB_MGR
    E_VENUS_EVENT_VIB_OPT,
    E_VENUS_EVENT_VIB_EXECTIME,
    E_VENUS_EVENT_VIB_DUTYCYCLECHANGE,
#endif

#ifdef BatteryLog_EN
    E_VENUS_EVENT_BATTERYLIFE_LOG_SAVE,
#endif

#ifdef LOG_SLEEP_MONITOR_RAW_DATA
    E_VENUS_EVENT_SET_SLEEP_MONITOR_RAW_LOG,
#endif

    E_VENUS_EVENT_DUMMY_END
};



// App timer definition:
// STANDBY TIMER ===============================
SW_TIMER_DEF(s_tVenusTimerAccel);     // disable this timer to unify standby timers
SW_TIMER_DEF(s_tVenusTimerDataTime);    // TBD: should rename like standby-base-timer
//SW_TIMER_DEF(s_tVenusTimerDiffTime); // for test

// FUNCTION TIMER ===============================
SW_TIMER_DEF(s_tTouchDebounceTime);
SW_TIMER_DEF(s_tLongTouchTriggerTime);
SW_TIMER_DEF(s_tChargePPRDebounceTime);
SW_TIMER_DEF(s_tChargeCHGDebounceTime);
SW_TIMER_DEF(s_tVenusTimerDisplayGeneral);
SW_TIMER_DEF(m_battery_timer_id);                         /**< Battery timer. */
SW_TIMER_DEF(m_rsc_meas_timer_id);                        /**< RSC measurement timer. */
SW_TIMER_DEF(s_tVenusTimerOLEDWakeup);
SW_TIMER_DEF(s_tVenusTimerOLEDSleep);
SW_TIMER_DEF(s_tVenusTimerOLEDDisplaySrvTimer);
SW_TIMER_DEF(s_tVenusTimerPWMVibSrvTimer);
SW_TIMER_DEF(s_tVenusTimerPedoWalkTimer);
SW_TIMER_DEF(s_tVenusTimerPedoRunTimer);
//SW_TIMER_DEF(s_tVenusTimerBatteryLiftImmediatelyCheck);
//SW_TIMER_DEF(s_tVenusTimerSwimCalTimer);
SW_TIMER_DEF(s_tLockSwimOffTimer);

#ifdef APP_VIB_MGR
SW_TIMER_DEF(s_tVenusVibOpt);
SW_TIMER_DEF(s_tVenusVibExecInterval);
SW_TIMER_DEF(s_tVenusVibDutyCycleChange);
#endif

typedef struct
{
    uint16_t wDataLen;
    i16_t wdata[FIFO_DEPTH_T/2];
    i16_t wbuf[FIFO_DEPTH_T/2];
    uint8_t cIndex;
}s_SensorData_t;

typedef struct
{
    AxesRaw_t s_tMagRaw[MAG_BUF_LEN];
    uint8_t bDataNum;
    uint8_t bAlgoNum;
}S_TMagBuf_t;


typedef struct
{
    uint8_t cNotify;
    uint8_t cEnable;
}S_TClockAlarm_t;


typedef struct
{
    bool            enabled;
    bool            saveDB;
    uint8_t         padding[2];
    app_date_time_t startTime;
    app_date_time_t stopTime;
    uint16_t        pedCount;
    uint16_t        calorie;

}S_DB_PedoRecord_t;



typedef struct
{
    //////// EXTERNAL EVENT POST ///////////////////////////////////////////
    S_VenusEvent    taEvent[E_VENUS_EVENT_DUMMY_END];
    eSystem_Battery_State_t eSystemPwrState;


    //////// APPLICATION :: PEDOMETER //////////////////////////////////////////
    uint32_t        dwPedTotalStepCount;
    uint32_t        dwPedWalkCount;
    uint32_t        dwPedRunCount;

    //////// APPLICATION :: CALORIE ////////////////////////////////////////////////
    uint32_t        dwTotalCalorie;
    uint32_t        dwWalkCalorie;
    uint32_t        dwRunCalorie;

    signed char    cPedState;
    signed char    cPedLastMovingState;
    uint8_t        baPedPadding[3];    // adjust this to keep 4-byte alignmenty
    S_DB_PedoRecord_t stPedRecordData_Run;
    S_DB_PedoRecord_t stPedRecordData_Walk;

    //////// APPLICATION :: TOUCH   ////////////////////////////////////////////////
    uint8_t cTouchDebounceFlag;

    //////// APPLICATION :: OLED DISPLAY   /////////////////////////////////////////
    bool bIsDisplayTimerActive;
    eHrm_Close_EventID m_HrmcloseID;

    //////// APPLICATION :: SWIMMING ////////////////////////////////////////////////
    result_event stSwimmingResult;
    uint8_t      cSwimmingEn;
    uint32_t    dwSwimLapCnt;
    uint32_t   dwLastSwimcount;
    uint32_t    wSwimmingLen;
    //db_swimming_t stDBSwim;
    app_date_time_t stLastLapTime;


    //////// APPLICATION :: BatteryLIFT////////////////////////////////////////////////
    uint16_t    wAdc_val;
    uint8_t     bAvg_BatLevel;
    uint8_t     bIsLowPower;
    uint8_t     bReadBatLevelCnt;
    uint8_t     bIsLowPowerNotify;
    uint8_t     bIsPreLowPower;


    //////// APPLICATION :: Notify Setting////////////////////////////////////////////////
    uint8_t     cNotify_flag;
    eCALL_state_t     cIncommingCallState;
    eSMS_state_t     cIncommingSMSState;
    eStete_t     cLongsitEn;
    eStete_t     cLiftarmEn;

    //////// APPLICATION :: Person Info////////////////////////////////////////////////
    CC_Ble_General_Info_T stGeneralInfo;

    //////// APPLICATION :: Unit Info////////////////////////////////////////////////
    CC_Ble_Unit_Info_T stUnitInfo;

    //////// APPLICATION :: Clock Alarm ////////////////////////////////////////////////
    CC_Ble_Clock_Alarm_T stClockAlarm;
    S_TClockAlarm_t stClock[4];
    uint8_t cNotifyPageFlag;
    app_date_time_t stSysCurTime;

    //////// APPLICATION :: HRM ////////////////////////////////////////////////
    eHrmOp_State     cHrmOnGoing;    // TODO: the flag should be moved to HRM driver
    uint8_t     cHrmData;
    uint8_t     chrmDataReady_Flag;


    //////// APPLICATION :: SLEEP MONITOR   /////////////////////////////////////////
    db_sys_sleep_monitor_t stSleepAlgExecPeriod;
    db_sleep_t stDBSleep;

    //////// APPLICATION :: SWIM CAL   /////////////////////////////////////////
    eSwim_Cal_State_t eSwimCalState;
    eSwim_Cal_ProcState_t eSwimCalEn;
    db_sys_static_gyro_offset_t  wSwimCalData;
    uint8_t bSwimCalFailRetry;


    //////// APPLICATION :: LONGSIT   /////////////////////////////////////////
    uint8_t LongSitWaitCnt;

    //////// APPLICATION :: Unlock Swim Off   /////////////////////////////////////////
    bool bIsLockSwimOff;

    //////// APPLICATION :: DB Init   /////////////////////////////////////////
    bool is_db_init_done;
    uint8_t db_init_type;

}   S_VenusCB;


//volatile uint8_t  *p_llm_le_event_mask = (volatile uint8_t *) 0x20000680;

//char deviceName[10] = {'V','N','S','_'};
//char deviceName[10] = {'Z','S','_'};
char deviceName[10] = {'Z','S','_','1','3','3','4','8'}; // for test

S_VenusCB    s_tVenusCB;
extern uint8_t g_GyroEnable;
static short _wGyroData[3]  = { 0 };
static short _wAccelData[3]  = { 0 };
float g_fSleepCalSeconds =0;
uint8_t g_bSleepEnCnt = 0;
#ifndef APP_SERV_MGR_EN  // not defined
s_SensorData_t s_tAcc;
s_SensorData_t s_tGyro;
#endif
S_TMagBuf_t s_tMagDataBuf;
AxesRaw_t s_tMagRaw;
//static uint8_t g_bMagEnCnt = 0;
#ifdef DB_EN
static db_pedometer_t m_db_pedo;
#endif
eLiftarm_Mode g_bLiftArm_State;

/*
static CC_Ble_Ped_Info_T _sPedInfo = {0xF1,0,0};
static CC_Ble_Hrm_Info_T _sHrmInfo = {0xF2,0,0};
static CC_Ble_Swim_Info_T _sSwimInfo = {0xF4,0,0};
*/
extern void _sensor_algorithm_sleepmeter_proc(void);
extern void CC_Dsp_Srv_Reflash_Screen(void);





void CC_VENUS_OLEDWakUpTimeOutTimerStart(uint16_t _wdata)
{
    sw_timer_start(s_tVenusTimerOLEDWakeup, _wdata, NULL);
}

void CC_VENUS_OLEDWakUpTimeOutTimerStop(void)
{
    sw_timer_stop(s_tVenusTimerOLEDWakeup);
}

void CC_VENUS_OLEDSleepTimeOutTimerStart(uint16_t _wdata)
{
    sw_timer_start(s_tVenusTimerOLEDSleep, _wdata, NULL);

}

void CC_VENUS_OLEDSleepTimeOutTimerStop(void)
{
    sw_timer_stop(s_tVenusTimerOLEDSleep);
}

void CC_VENUS_OLEDGeneralOutTimerStart(uint16_t _wdata)
{
    sw_timer_start(s_tVenusTimerDisplayGeneral, _wdata, NULL);
}

void CC_VENUS_OLEDGeneralOutTimerReset(void)
{
    sw_timer_start(s_tVenusTimerDisplayGeneral, 0, NULL);
}

void CC_VENUS_OLEDGeneralOutTimerStop(void)
{
    sw_timer_stop(s_tVenusTimerDisplayGeneral);
}

void CC_VENUS_OLEDDisplayServiceTimerStart(uint16_t _wdata)
{
    sw_timer_start(s_tVenusTimerOLEDDisplaySrvTimer, _wdata, NULL);
}

void CC_VENUS_OLEDDisplayServiceTimerReset(void)
{
    sw_timer_start(s_tVenusTimerOLEDDisplaySrvTimer, 0, NULL);
}

void CC_VENUS_OLEDDisplayServiceTimerStop(void)
{
    sw_timer_stop(s_tVenusTimerOLEDDisplaySrvTimer);
}

void CC_VENUS_OLEDDisplayServiceEventClear(void)
{
    VENUS_EVENT_OFF(E_VENUS_EVENT_OLED_DSPSRV);
}

void CC_VENUS_TOUCHDebounceTimerStart(uint16_t _wdata)
{
    sw_timer_start(s_tTouchDebounceTime, _wdata, NULL);
}


void CC_VENUS_LongTOUCHTriggerTimerStart(uint16_t _wdata)
{
    sw_timer_start(s_tLongTouchTriggerTime, _wdata, NULL);
}


void CC_VENUS_CHARGE_PPR_DebounceTimerStart(uint16_t _wData)
{
    sw_timer_start(s_tChargePPRDebounceTime, _wData, NULL);
}

void CC_VENUS_CHARGE_CHG_DebounceTimerStart(uint16_t _wData)
{
    sw_timer_start(s_tChargeCHGDebounceTime, _wData, NULL);
}



void CC_VENUS_PWMVibrationServiceTimerStart(uint16_t _wdata)
{
    sw_timer_start(s_tVenusTimerPWMVibSrvTimer, _wdata, NULL);
}

void CC_VENUS_PWMVibrationServiceTimerReset(void)
{
    sw_timer_start(s_tVenusTimerPWMVibSrvTimer, 0, NULL);
}

void CC_VENUS_PWMVibrationServiceTimerStop(void)
{
    sw_timer_stop(s_tVenusTimerPWMVibSrvTimer);
}

void CC_VENUS_PWMVibrationServiceEventClear(void)
{
    VENUS_EVENT_OFF(E_VENUS_EVENT_PWM_VIBSRV);
}


void CC_VENUS_RscTimerStart(uint32_t interval_ms)
{
     sw_timer_start(m_rsc_meas_timer_id, interval_ms, NULL);
}

void CC_VENUS_RscTimerStop(void)
{
    sw_timer_stop(m_rsc_meas_timer_id);
}

#ifdef APP_VIB_MGR
void CC_VENUS_VIB_TIMER_OPT_Start(uint16_t _wdata)
{
    sw_timer_start(s_tVenusVibOpt, _wdata, NULL);
}

void CC_VENUS_VIB_TIMER_OPT_Stop(void)
{
    sw_timer_stop(s_tVenusVibOpt);
}

void CC_VENUS_VIB_TIMER_EXECINTERVAL_Start(uint16_t _wdata)
{
    sw_timer_start(s_tVenusVibExecInterval, _wdata, NULL);
}

void CC_VENUS_VIB_TIMER_EXECINTERVAL_Stop(void)
{
    sw_timer_stop(s_tVenusVibExecInterval);
}


void CC_VENUS_VIB_TIMER_DUTYCYCLECHANGE_Start(uint16_t _wdata)
{
    sw_timer_start(s_tVenusVibDutyCycleChange, _wdata, NULL);
}

void CC_VENUS_VIB_TIMER_DUTYCYCLECHANGE_Stop(void)
{
    sw_timer_stop(s_tVenusVibDutyCycleChange);
}

#endif

#ifdef APP_VIB_MGR
static void cc_vibopt_timeout_handler(void * p_context)
{
    VENUS_EVENT_ON(E_VENUS_EVENT_VIB_OPT, E_APP_SCHED_VIB_TIMER_OPT_TIMEOUT);
}

static void cc_vibexecinterval_timeout_handler(void * p_context)
{
    VENUS_EVENT_ON(E_VENUS_EVENT_VIB_EXECTIME, E_APP_SCHED_VIB_TIMER_EXECINTERVAL_TIMEOUT);
}

static void cc_vibdutycyclechange_timeout_handler(void * p_context)
{
    VENUS_EVENT_ON(E_VENUS_EVENT_VIB_DUTYCYCLECHANGE, E_APP_SCHED_VIB_TIMER_DUTYCYCLECHANGE_TIMEOUT);
}

void CC_VENUS_VIB_TIMER_OPT_Clear(void)
{
    VENUS_EVENT_OFF(E_VENUS_EVENT_VIB_OPT);
}

void CC_VENUS_VIB_TIMER_EXECINTERVAL_Clear(void)
{
    VENUS_EVENT_OFF(E_VENUS_EVENT_VIB_EXECTIME);
}

void CC_VENUS_VIB_TIMER_DUTYCYCLECHANGE_Clear(void)
{
    VENUS_EVENT_OFF(E_VENUS_EVENT_VIB_DUTYCYCLECHANGE);
}
#endif



#ifdef DB_EN

bool CC_DB_Check_Init_Done(void)
{
    return s_tVenusCB.is_db_init_done;

}

void CC_DB_Set_Init_Done(void)
{
	s_tVenusCB.is_db_init_done = true;
}

uint8_t CC_DB_Get_Init_Type(void)
{
	return s_tVenusCB.db_init_type;
}

void CC_DB_Force_Execute_Init(uint8_t init_type)
{
    s_tVenusCB.db_init_type = init_type;
	s_tVenusCB.is_db_init_done = false;
}

void CC_DB_Init(uint8_t _bState)
{
    uint8_t _bFirstInit = 0;
    uint32_t ret = CC_DB_Init_FileRec(&_bFirstInit, _bState);
    TracerInfo("CC_DB_Init_FileRec return = [0x%x]\r\n",ret);
    if (false==_bFirstInit)
        CC_Fds_Change_DayofWeek(s_tVenusCB.stSysCurTime.dayofweek,true);
    else
        CC_Fds_Change_DayofWeek(s_tVenusCB.stSysCurTime.dayofweek,false);

    CC_DB_Set_Init_Done();
}
#endif

void CC_GetStaticCalibrationInfo(db_sys_static_gyro_offset_t *pData)
{
    memcpy(pData,&s_tVenusCB.wSwimCalData,sizeof(db_sys_static_gyro_offset_t));
}

void CC_SetStaticCalibrationInfo(db_sys_static_gyro_offset_t *pData)
{
    memcpy(&s_tVenusCB.wSwimCalData,pData,sizeof(db_sys_static_gyro_offset_t));
}

void CC_VENUS_Lock_SwimOff_TimerStop(void)
{
    s_tVenusCB.bIsLockSwimOff = false;
    sw_timer_stop(s_tLockSwimOffTimer);
}

void CC_VENUS_Lock_SwimOff_TimerStart(void)
{
    s_tVenusCB.bIsLockSwimOff = true;
    sw_timer_start(s_tLockSwimOffTimer, LOCK_SWIM_OFF_TIMEOUT_INTERVAL, NULL);
}

#ifndef APP_SERV_MGR_EN // not defined
void CC_HRM_Post24HrModeEvent(uint8_t bSwitch)
{
    if (bSwitch)
    {
        VENUS_EVENT_ON(E_VENUS_EVENT_HRM_SERVICE_24HR_START, eEvent_None);
    }
    else
    {
        VENUS_EVENT_ON(E_VENUS_EVENT_HRM_SERVICE_24HR_STOP, eEvent_None);
    }
}
#endif

#if 1 //BAT_CHG_EN
uint8_t CC_GetLowPowerState(void)
{
      return 0;
//    return s_tVenusCB.bIsLowPower;
}

uint8_t CC_Charge_Register(charge_cb_t cb)
{
    return 1;
/*
    int ret;

    if (m_charge_users == CHARGE_MAX_USERS)
    {
        ret = 0;
    }
    else
    {
        m_charge_cb_table[m_charge_users] = cb;
        m_charge_users++;

        ret = 1;
    }
    return ret;
*/
}
/*
static void CC_Charge_Event_Send(eDEV_CHARGE_STATE_t eState)
{

    for (uint32_t user = 0; user < FDS_MAX_USERS; user++)
    {
        if (m_charge_cb_table[user] != NULL)
        {
            m_charge_cb_table[user](eState);
        }
    }

}
*/
/*
static void cc_charge_ppr_debounce_timeout(void * p_context)
{
//    VENUS_EVENT_ON(E_VENUS_EVENT_CHARGING_INT , eEvent_None);
}

static void cc_charge_chg_debounce_timeout(void * p_context)
{

    uint8_t _bIsFull = nrf_gpio_pin_read(CHG_PIN);
    if( eDEVICE_CHARGE_FULL==_bIsFull)
    {
        VENUS_EVENT_ON(E_VENUS_EVENT_CHARGING_FULL, eEvent_None);
    }
    else
    {
        s_tVenusCB.bChargCHGDebounceFlag=false;
    }

}

static void _CC_Reflash_BatteryLevel(void)
{

    //start time 40 ms to reflash battery level.
    CC_VENUS_BatteryImmediatelyCheckTimerStart(TIMER_CNT_CEHCKBATLEVEL_MS);

}

static uint8_t _CC_Is_CHARGE_FULL(void)
{
    return 0;

    uint8_t _bIsPPRCHARGE = nrf_gpio_pin_read(PPR_PIN);
    uint8_t _bIsFull = nrf_gpio_pin_read(CHG_PIN);

    if (eDEVICE_CHARGE_IN ==_bIsPPRCHARGE && eDEVICE_CHARGE_FULL == _bIsFull && s_tVenusCB.bIs_FullReported == false )
    {
        //TracerInfo("_CC_CHARGE_In_Cfg chg hi ~~~~~ \r\n");
        CC_VENUS_CHARGE_CHG_DebounceTimerStart(CHG_DEBOUNCE_TIME);
        s_tVenusCB.bChargCHGDebounceFlag=true;
    }
    else
    {
        //TracerInfo("_CC_CHARGE_In_Cfg chg LO !!!! \r\n");
        if(s_tVenusCB.bIs_FullReported == false)
            s_tVenusCB.eChargingFullState = eDEVICE_CHARGE_NOFULL;  //full
    }
    return _bIsFull;

}

static void CC_BatteryLevelReset(void)
{

    if (s_tVenusCB.eExChangingState != s_tVenusCB.eChargingState)
    {
        if (s_tVenusCB.eChargingState == eDEVICE_CHARGE_IN)
            s_tVenusCB.wAdc_val = 0;
        else
        {
            s_tVenusCB.wAdc_val = 0xffff;
            s_tVenusCB.bAvg_BatLevel = BAS_LEVEL_MAX_INVAILD;
        }

        s_tVenusCB.eExChangingState = s_tVenusCB.eChargingState;
    }

}

static void _CC_CHARGE_In_Cfg(void)
{

    s_tVenusCB.eChargingState = eDEVICE_CHARGE_IN;
    if (eDEVICE_CHARGE_NOFULL ==_CC_Is_CHARGE_FULL())
        VENUS_EVENT_ON(E_VENUS_EVENT_OLED_UPDATE , eEvent_CHARGINGIN);

    CC_Charge_Event_Send(s_tVenusCB.eChargingState);

    CC_VENUS_BatteryImmeditaelyCheckTimerReset();
    CC_VENUS_BatteryImmeditaelyCheckTimerStop();
    s_tVenusCB.bIsLowPowerNotify = false;

    //Set battery level reflash count when charge-out or charge full to re-flash battery level.
    s_tVenusCB.bReadBatLevelCnt=READ_BATLEVELCNT;

    CC_BatteryLevelReset();

}

static void _CC_CHARGE_Out_Cfg(void)
{

    s_tVenusCB.eChargingState = eDEVICE_CHARGE_OUT;
    s_tVenusCB.bIs_FullReported = false;

    CC_Charge_Event_Send(s_tVenusCB.eChargingState);

    CC_AppSrv_HR_SetAppLockHrm(CC_APPSRV_HR_APP_UNLOCK);

    if (1 == CC_BLE_Cmd_GetSwimmingEN())
    {
        VENUS_EVENT_ON(E_VENUS_EVENT_OLED_UPDATE , eEvent_SWIM_ON);
        CC_AppSrv_HR_SetAppLockHrm(CC_APPSRV_HR_APP_LOCK);
    }
    else if (eEnable == CC_BLE_Cme_Get_HeartRateStrapMode())
    {
        VENUS_EVENT_ON(E_VENUS_EVENT_OLED_UPDATE , eEvent_HEARTRATESTRAPMODE_ON);
    }
    else
    {
        VENUS_EVENT_ON(E_VENUS_EVENT_OLED_UPDATE , eEvent_CHARGINGOUT);
    }

    if (eEnable == CC_BLE_Cme_Get_24HourHeartRateMode())
    {
        CC_AppSrv_HR_Start24HR();
    }

    CC_BatteryLevelReset();

    _CC_Reflash_BatteryLevel();

}

void CC_CHARGING_Handler(void)
{

    uint8_t Charging = nrf_gpio_pin_read(PPR_PIN);
    if (Charging == eDEVICE_CHARGE_IN) // 0 = charge in , 1 = no charge
    {
        _CC_CHARGE_In_Cfg();
    }
    else
    {
        _CC_CHARGE_Out_Cfg();
    }

    nrf_drv_gpiote_in_event_enable(PPR_PIN,true);

}

void CHARGING_Full_Handler(void)
{

    TracerInfo("CHARGING_In_Handler Charging Full !!! \r\n");
    VENUS_EVENT_ON(E_VENUS_EVENT_OLED_UPDATE , eEvent_CHARGINGFULL);
    s_tVenusCB.eChargingFullState = eDEVICE_CHARGE_FULL;  //full
    s_tVenusCB.bIs_FullReported = true;
    s_tVenusCB.bFullIndicationtoBattery = true;

    //Reflash battery level immediately
    _CC_Reflash_BatteryLevel();

}

void CC_ChargeStatePolling(void)
{

    uint8_t Charging = nrf_gpio_pin_read(PPR_PIN);
    if (s_tVenusCB.eChargingState != Charging)
    {
        if (Charging == eDEVICE_CHARGE_IN) // 0 = charge in , 1 = no charge
        {
            _CC_CHARGE_In_Cfg();
        }
        else
        {
            _CC_CHARGE_Out_Cfg();
        }
    }
    else
    {
        if(Charging == eDEVICE_CHARGE_IN)
        {
            _CC_Is_CHARGE_FULL();
        }
    }

}

void CC_ChargePPR_PinStatInit(void)
{

    uint8_t Charging = nrf_gpio_pin_read(PPR_PIN);
    if (Charging == eDEVICE_CHARGE_IN) // 0 = charge in , 1 = no charge
    {
        _CC_CHARGE_In_Cfg();
    }
    else
    {
        //default  bIsLowPower = false
        //First boot check battery capacity,
        //Init state,
        //battery low power will go to low power page,

        #ifdef EN_APPPROTECTION_AND_LOWPWR
        if (s_tVenusCB.bAvg_BatLevel >BATTERYLEVEL_ENTER_LOWPOWER)
            s_tVenusCB.bIsLowPower = false;
        else
            s_tVenusCB.bIsLowPower = true;

         _CC_CHARGE_Out_Cfg();
        //[VNS-97]
        CC_Battery_PreLowPowerCheck();
        #else
        _CC_CHARGE_Out_Cfg();
        //[VNS-97]
        CC_Battery_PreLowPowerCheck();
        #endif
    }

}
*/
eDEV_CHARGE_STATE_t CC_GetChargeStatus(void)
{
    return eDEVICE_CHARGE_OUT;
    //return s_tVenusCB.eChargingState;
}

eDEV_CHARGEFULL_STATE_t CC_GetChargeFullStatus(void)
{
    return eDEVICE_CHARGE_NOFULL;
    //return s_tVenusCB.eChargingFullState;
}
#endif



uint8_t CC_MainGet_HrmData(void)
{
    return s_tVenusCB.cHrmData;
}

uint8_t CC_MainGet_HrmDataFlag(void)
{
    return s_tVenusCB.chrmDataReady_Flag;
}

void CC_MainSet_HrmData(uint8_t _bHrmData)
{
    s_tVenusCB.cHrmData = _bHrmData;
}

void CC_MainSet_HrmDataFlag(uint8_t _bHrmReadyFlag)
{
    s_tVenusCB.chrmDataReady_Flag= _bHrmReadyFlag;
}

#ifndef APP_SERV_MGR_EN
void CC_HRM_Post24HHR_TO_OneMeasurement(void)
{
    VENUS_EVENT_ON(E_VENUS_EVENT_HRM_SERVICE_24HR_TO_ONE_MEASUREMENT, eEvent_None);
}

void CC_HRM_Post24HHR_TO_PeriodicMeasurement(void)
{
    VENUS_EVENT_ON(E_VENUS_EVENT_HRM_SERVICE_24HR_TO_PERIODIC_MEASUREMENT, eEvent_None);
}

void CC_HRM_PostHeartRateStrapModeEvent(uint8_t bSwitch)
{
    if ( eDEVICE_CHARGE_IN==CC_GetChargeStatus())
        return;

    if (bSwitch)
    {
        if (0 == CC_BLE_Cmd_GetSwimmingEN() &&
           (false ==CC_AppSrv_HR_IsHrsEnabled()))
            VENUS_EVENT_ON(E_VENUS_EVENT_OLED_UPDATE, eEvent_HEARTRATESTRAPMODE_ON);
    }
    else
    {
        if (true ==CC_AppSrv_HR_IsHrsEnabled())
            VENUS_EVENT_ON(E_VENUS_EVENT_OLED_UPDATE, eEvent_HEARTRATESTRAPMODE_OFF);
    }
}
#endif


void CC_SwimOffUnlockHrm(void)
{
     VENUS_EVENT_ON(E_VENUS_EVENT_HRM_SERVICE_RESUME, eEvent_None);
}


void CC_ResumeHrm(void)
{
    TracerInfo("\r\n[CC_ResumeHrm] \r\n");

    CC_AppSrv_HR_SetAppLockHrm(CC_APPSRV_HR_APP_UNLOCK);

    if (eEnable == CC_BLE_Cme_Get_24HourHeartRateMode())
        CC_AppSrv_HR_Start24HR();

    if (eEnable == CC_BLE_Cme_Get_HeartRateStrapMode())
        VENUS_EVENT_ON(E_VENUS_EVENT_OLED_UPDATE, eEvent_HEARTRATESTRAPMODE_ON);
}


void CC_Swim_OLED_Update(uint8_t _bSwitch)
{
    if(_bSwitch == 1)
    {
        VENUS_EVENT_ON(E_VENUS_EVENT_OLED_UPDATE , eEvent_SWIM_ON);
    }
    else
    {
        VENUS_EVENT_ON(E_VENUS_EVENT_OLED_UPDATE , eEvent_SWIM_OFF);
    }
}

#ifdef DB_EN

static void CC_DB_Save_Pedo_Data(uint8_t ped_type, S_DB_PedoRecord_t *ped_record_data)
{

    m_db_pedo.ped_state.state = ped_type;
    m_db_pedo.time.year = (uint8_t)(ped_record_data->startTime.year - 2000);
    m_db_pedo.time.month = ped_record_data->startTime.month;
    m_db_pedo.time.day = ped_record_data->startTime.day;
    m_db_pedo.time.hour = ped_record_data->startTime.hours;
    m_db_pedo.time.min = ped_record_data->startTime.minutes;
    m_db_pedo.time.sec = ped_record_data->startTime.seconds;
    m_db_pedo.ped_single.count = (uint16_t)ped_record_data->pedCount;
    m_db_pedo.endoftime.hour = ped_record_data->stopTime.hours;
    m_db_pedo.endoftime.min = ped_record_data->stopTime.minutes;
    m_db_pedo.endoftime.sec = ped_record_data->stopTime.seconds;
    m_db_pedo.calorie = ped_record_data->calorie;

    if(CC_SUCCESS != CC_Save_Record(ePed, (uint32_t*)&m_db_pedo, sizeof(db_pedometer_t)))
    {
        TracerInfo("HRM_DB_Save_Err\r\n");
    }
    else
    {
        TracerInfo("PEDO_DB_SAVE OK!!\r\n");
        //TracerInfo("start_time.year:%d\r\n",ped_record_data->startTime.year, ped_record_data->startTime.year);
        TracerInfo("pedo_state: %d, step: %d\r\n", m_db_pedo.ped_state.state, m_db_pedo.ped_single.count);
    }

}
#endif

void CC_VENUS_WALK_PedoTimerStart(uint32_t interval_ms)
{
    sw_timer_start(s_tVenusTimerPedoWalkTimer, interval_ms, NULL);
}

void CC_VENUS_WALK_PedoTimerStop(void)
{
    sw_timer_stop(s_tVenusTimerPedoWalkTimer);
}

void cc_pedo_walk_timeout_handler(void * p_context)
{


    s_tVenusCB.stPedRecordData_Walk.saveDB = 1;

}

void CC_VENUS_RUN_PedoTimerStart(uint32_t interval_ms)
{
    sw_timer_start(s_tVenusTimerPedoRunTimer, interval_ms, NULL);
}

void CC_VENUS_RUN_PedoTimerStop(void)
{
	sw_timer_stop(s_tVenusTimerPedoRunTimer);
}

void cc_pedo_run_timeout_handler(void * p_context)
{


    s_tVenusCB.stPedRecordData_Run.saveDB = 1;

}
#ifdef DB_EN

void CC_Pedo_Check_Save_Database(void)
{
    if(s_tVenusCB.stPedRecordData_Walk.saveDB == 1)
    {
        //Jason, caculate walk calorie
        s_tVenusCB.stPedRecordData_Walk.calorie = (uint16_t) CC_CalorieBurnCalStepV2(s_tVenusCB.stPedRecordData_Walk.pedCount,1,ePedo_Walk);
        CC_DB_Save_Pedo_Data(ePedo_Walk, &s_tVenusCB.stPedRecordData_Walk);


        s_tVenusCB.stPedRecordData_Walk.saveDB = 0;
        s_tVenusCB.stPedRecordData_Walk.pedCount = 0;
        s_tVenusCB.stPedRecordData_Walk.enabled = 0;

    }
    else if(s_tVenusCB.stPedRecordData_Run.saveDB == 1)
    {
        //Jason, caculate walk calorie
        s_tVenusCB.stPedRecordData_Run.calorie = (uint16_t) CC_CalorieBurnCalStepV2(s_tVenusCB.stPedRecordData_Run.pedCount,1,ePedo_Run);
        CC_DB_Save_Pedo_Data(ePedo_Run, &s_tVenusCB.stPedRecordData_Run);

        s_tVenusCB.stPedRecordData_Run.saveDB = 0;
        s_tVenusCB.stPedRecordData_Run.pedCount = 0;
        s_tVenusCB.stPedRecordData_Run.enabled = 0;
    }
}
#endif
void CC_MainGet_CurrentTime(app_date_time_t *_stCurTime)
{
    *_stCurTime = s_tVenusCB.stSysCurTime;
}


void CC_MainGet_Stride_LengthSetting(uint8_t *_pbStride, uint8_t *_pbUnit)
{
    *_pbStride = s_tVenusCB.stGeneralInfo.cStride_Lenght;
    *_pbUnit = s_tVenusCB.stUnitInfo.cUnitLength;
}

uint32_t CC_MainGet_Calorie(void)
{
    return s_tVenusCB.dwTotalCalorie;
}

void CC_MainGet_SwimmingInfo(eSWIM_LEN_SET_t *_bPoolSize, uint32_t *_dwSwimLen)
{
    *_bPoolSize = s_tVenusCB.stGeneralInfo.cSwim_Pool_Size;
    *_dwSwimLen =s_tVenusCB.wSwimmingLen;
}

uint8_t CC_MainGet_BatteryCapacity(void)
{
      return 100;  // for test
//    return s_tVenusCB.bAvg_BatLevel;
}

static void _TouchDebounce_Handler(uint16_t _wData)
{
    CC_VENUS_TOUCHDebounceTimerStart(_wData);
}


//void TOUCH_int_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
void TOUCH_int_handler(void) // no implement parameter
{

//    if(pin == TOUCH_INT_PIN)
    if(0x00 == drvi_GpioRead(TOUCH_INT_PIN))
    {
        if (s_tVenusCB.cTouchDebounceFlag == 0)
        {
            _TouchDebounce_Handler(50); // comment for test by Samuel

            s_tVenusCB.cTouchDebounceFlag = 1;

        }
#ifdef LONGTOUCH_SWIM_SWITCH_EN
        _LongTouchTrigger_Handler(100);
         s_tVenusCB.cLongTouchCount = 0;
#endif
    }
}

void TOUCH_Handler(void)
{

    VENUS_EVENT_ON(E_VENUS_EVENT_OLED_UPDATE , eEvent_TOUCH);

/*
    if((eDEVICE_CHARGE_OUT == CC_GetChargeStatus()) && (eEnable == CC_BLE_Cme_Get_HeartRateStrapMode()))
    {
        VENUS_EVENT_ON(E_VENUS_EVENT_OLED_UPDATE , eEvent_HEARTRATESTRAPMODE_ON);
    }
    else
    {
        VENUS_EVENT_ON(E_VENUS_EVENT_OLED_UPDATE , eEvent_TOUCH);
    }
*/
}


static void cc_touch_debounce_timeout(void * p_context)
{


    if (0x00 == drvi_GpioRead(TOUCH_INT_PIN))
    {
        VENUS_EVENT_ON(E_VENUS_EVENT_TOUCH_INT ,eEvent_None);
    }
    else
    {
        s_tVenusCB.cTouchDebounceFlag = 0;
    }
}


void CC_SetHrmClosePageEvent(void)
{
    VENUS_EVENT_ON(E_VENUS_EVENT_OLED_UPDATE, eEvent_HRM_TIMEOUT);
}

void CC_SetHrmDisplayHrmDataPageEvent(void)
{
    VENUS_EVENT_ON(E_VENUS_EVENT_OLED_UPDATE, eEvent_HRM_DATA);
}

static void CC_HrmTurnOffEventSet(void)
{
    VENUS_EVENT_ON(E_VENUS_EVENT_HRM_TIMEOUT ,eEvent_None);
}

static void cc_toolbox_Dispaly_General_handler(void * p_context)
{

    VENUS_EVENT_ON(E_VENUS_EVENT_OLED_UPDATE , eEvent_None);

}

static void cc_toolbox_OLED_Wakup_TimeOut(void * p_context)
{

//    TracerInfo(" cc_toolbox_OLED_Wakup_TimeOut \r\n");
    VENUS_EVENT_ON(E_VENUS_EVENT_OLED_WAKEUP , eEvent_None);
}

static void cc_toolbox_OLED_Sleep_TimeOut(void * p_context)
{

//    TracerInfo(" cc_toolbox_OLED_Sleep_TimeOut \r\n");
    VENUS_EVENT_ON(E_VENUS_EVENT_OLED_SLEEP , eEvent_None);
}

static void cc_toolbox_OLED_Display_Service_timeout(void * p_context)
{

    VENUS_EVENT_ON(E_VENUS_EVENT_OLED_DSPSRV, eEvent_None);
    //TracerInfo(" cc_toolbox_OLED_Display_Service_timeout \r\n");
}

static void cc_toolBox_PWM_Vib_Service_timeout(void * p_context)
{

    //TracerInfo(" cc_toolBox_PWM_Vib_Service_timeout \r\n");
    VENUS_EVENT_ON(E_VENUS_EVENT_PWM_VIBSRV, eEvent_None);
}


void CC_VENUS_AccelTimerFifoModeStart(void)
{
#ifdef FORCE_LIFTARM_TEST_EN
    if(1)   // force to enable lift arm
#else
    if(0x01 == CC_BLE_Cmd_GetLiftArmStatus())
#endif
        sw_timer_start(s_tVenusTimerAccel, ACCEL_FIFO_MODE_SHORT_INTERVAL, NULL);
    else
        sw_timer_start(s_tVenusTimerAccel, ACCEL_FIFO_MODE_LONG_INTERVAL, NULL);
}

void CC_VENUS_AccelTimerPollModeStart(void)
{
    sw_timer_start(s_tVenusTimerAccel, ACCEL_POLL_MODE_INTERVAL, NULL);
}

void CC_VENUS_AccelTimerReset(void)
{
    sw_timer_start(s_tVenusTimerAccel, 0, NULL);
}

void CC_VENUS_AccelTimerStop(void)
{
    sw_timer_stop(s_tVenusTimerAccel);
}

void CC_VENUS_SwimTimerFifoModeStart(void)
{
    sw_timer_start(s_tVenusTimerAccel, ACCEL_FIFO_MODE_SWIM_INTERVAL, NULL);
}

void CC_SetHrmCloseSwitch(eHrm_Close_EventID _bEvent)
{
    s_tVenusCB.m_HrmcloseID = _bEvent;
}

void CC_SetToTurnOffHrm(eHrm_Close_EventID _bID)
{
    CC_HrmTurnOffEventSet();
    CC_SetHrmCloseSwitch(_bID);
}

static void cc_toolbox_sensor_accel_on_change(void * p_context)
{


    VENUS_EVENT_ON(E_VENUS_EVENT_SENSOR_DATA_READY_ACCEL , eEvent_None);

#ifdef PEDandSWIMMING
    VENUS_EVENT_ON(E_VENUS_EVENT_SENSOR_DATA_READY_GYRO , eEvent_None);

#endif
}

static void cc_toolbox_Datatime_hanndler(void * p_context)
{
    //cc_toolbox_sensor_accel_on_change(p_context);

    VENUS_EVENT_ON(E_VENUS_EVENT_DATETIMEFLUSH , eEvent_None);

    #ifdef LONGSIT_EN
        VENUS_EVENT_ON(E_VENUS_EVENT_LONGSITEVENT , eEvent_None);
    #endif
}


uint8_t cc_get_battery_level(void)
{
    return s_tVenusCB.bAvg_BatLevel;
}

void battery_level_update(uint8_t battery_level)
{
    app_batt_send_lvl(battery_level);
}

static void battery_level_meas_timeout_handler(void * p_context)
{

    battery_level_update(cc_get_battery_level());
}

#ifdef CFG_BLE_APP

#ifdef DB_EN
static void _Ble_CommandHistroyPedoParse(void)
{
    uint32_t    _dwlen =0;
    uint16_t total;
    uint16_t rec_index;
    db_pedometer_t pedo_db_data;
    uint16_t rsize;
    app_date_time_t curr_time;
    uint8_t dayofweek;

    CC_MainGet_CurrentTime(&curr_time);

    if(CC_BLE_Cmd_GetHistoryDayIndex() <= 7)
    {
        dayofweek = (curr_time.dayofweek + CC_BLE_Cmd_GetHistoryDayIndex()) % 7;

        rec_index = CC_BLE_Cmd_GetHistoryRecordIndex();

        if(CC_SUCCESS == CC_Read_DayofRecoordLen(ePed, dayofweek, &total))
        {
            if((total != 0) && (rec_index <= total))
            {

                if(CC_SUCCESS == CC_Read_Record(ePed, dayofweek, rec_index, (uint32_t*)&pedo_db_data, &rsize))
                {
                    //TracerInfo("rsize:%d\r\n",total, rsize);
                    uint8_t db_array[14];

                    memset(db_array, 0x00, 14);
                    db_array[_dwlen++] = pedo_db_data.time.year;
                    db_array[_dwlen++] = pedo_db_data.time.month;
                    db_array[_dwlen++] = pedo_db_data.time.day;
                    db_array[_dwlen++] = pedo_db_data.time.hour;
                    db_array[_dwlen++] = pedo_db_data.time.min;
                    db_array[_dwlen++] = pedo_db_data.time.sec;
                    db_array[_dwlen++] = pedo_db_data.ped_state.state;
                    co_write16p(&db_array[_dwlen], pedo_db_data.ped_single.count);
                    _dwlen += 2;
                    db_array[_dwlen++] = pedo_db_data.endoftime.hour;
                    db_array[_dwlen++] = pedo_db_data.endoftime.min;
                    db_array[_dwlen++] = pedo_db_data.endoftime.sec;
                    co_write16p(&db_array[_dwlen], pedo_db_data.calorie);
                    _dwlen += 2;

                    app_ht_history_send(BLE_DB_PED_COMMAND_ID, total, rec_index, db_array, _dwlen);
                }
                else
                    return;
            }
            else
            {
                app_ht_history_send(BLE_DB_PED_COMMAND_ID, total, rec_index, NULL, _dwlen);
                TracerInfo("total:%d, index:%d, dayofweek:%d\r\n",total, rec_index, dayofweek);
            }

        }
        else
            return;


        CC_BLE_Cmd_UpdateHistoryRecordIndex();

        if(CC_BLE_Cmd_GetHistoryRecordIndex() > total)
        {
            if(total != 0)
                CC_DB_Delete_OneDay_File(ePed, dayofweek);

            CC_BLE_Cmd_ClrHistoryRecordIndex();
            CC_BLE_Cmd_UpdateHistoryDayIndex();

            return;
        }
    }
    else
    {
        if(0x00 != CC_BLE_Cmd_GetHistoryType())
        {
            CC_VENUS_RscTimerStop();

            CC_BLE_Cmd_ClrHistoryType();

            CC_VENUS_RscTimerStart(SPEED_AND_CADENCE_MEAS_INTERVAL);
        }

        return;
    }
}

static void _Ble_CommandHistroyHrmParse(void)
{
    uint32_t    _dwlen =0;
    uint16_t total;
    uint16_t rec_index;
    db_heartrate_t hr_db_data;
    uint16_t rsize;
    app_date_time_t curr_time;
    uint8_t dayofweek;


    CC_MainGet_CurrentTime(&curr_time);

    if(CC_BLE_Cmd_GetHistoryDayIndex() <= 7)
    {
        dayofweek = (curr_time.dayofweek + CC_BLE_Cmd_GetHistoryDayIndex()) % 7;

        rec_index = CC_BLE_Cmd_GetHistoryRecordIndex();

        if(CC_SUCCESS == CC_Read_DayofRecoordLen(eHrm, dayofweek, &total))
        {

            if((total != 0) && (rec_index <= total))
            {
                if(CC_SUCCESS == CC_Read_Record(eHrm, dayofweek, rec_index, (uint32_t*)&hr_db_data, &rsize))
                {
                    uint8_t db_array[14];

                    memset(db_array, 0x00, 14);

                    db_array[_dwlen++] = hr_db_data.time.year;
                    db_array[_dwlen++] = hr_db_data.time.month;
                    db_array[_dwlen++] = hr_db_data.time.day;
                    db_array[_dwlen++] = hr_db_data.time.hour;
                    db_array[_dwlen++] = hr_db_data.time.min;
                    db_array[_dwlen++] = hr_db_data.time.sec;
                    db_array[_dwlen++] = hr_db_data.data.hr;

                    app_ht_history_send(BLE_DB_HRM_COMMAND_ID, total, rec_index, db_array, _dwlen);

                    //TracerInfo("total:%d, index:%d\r\n",total, rec_index);
                }
                else
                    return;
            }
            else
            {
                app_ht_history_send(BLE_DB_HRM_COMMAND_ID, total, rec_index, NULL, _dwlen);

                TracerInfo("total:%d, index:%d, dayofweek:%d\r\n",total, rec_index, dayofweek);
            }


        }
        else
        	return;

        CC_BLE_Cmd_UpdateHistoryRecordIndex();

        if(CC_BLE_Cmd_GetHistoryRecordIndex() > total)
        {
            if(total != 0)
                CC_DB_Delete_OneDay_File(eHrm, dayofweek);

            CC_BLE_Cmd_ClrHistoryRecordIndex();
            CC_BLE_Cmd_UpdateHistoryDayIndex();

            return;
        }
    }
    else
    {
        if(0x00 != CC_BLE_Cmd_GetHistoryType())
        {
            CC_VENUS_RscTimerStop();

            CC_BLE_Cmd_ClrHistoryType();

            CC_VENUS_RscTimerStart(SPEED_AND_CADENCE_MEAS_INTERVAL);
        }
        return;
    }
}


static void  _Ble_CommandHistroySleepParse(void)
{
    uint32_t    _dwlen =0;
    uint16_t    total;
    uint16_t    rec_index;
    db_sleep_t sleep_db_data;
    uint16_t rsize;
    app_date_time_t curr_time;
    uint8_t dayofweek;

    CC_MainGet_CurrentTime(&curr_time);

    if(CC_BLE_Cmd_GetHistoryDayIndex() <= 7)
    {
        dayofweek = (curr_time.dayofweek + CC_BLE_Cmd_GetHistoryDayIndex()) % 7;

        rec_index = CC_BLE_Cmd_GetHistoryRecordIndex();


        if(CC_SUCCESS == CC_Read_DayofRecoordLen(eSleep, dayofweek, &total))
        {
            if((total != 0) && (rec_index <= total))
            {
                if(CC_SUCCESS == CC_Read_Record(eSleep, dayofweek, rec_index, (uint32_t*)&sleep_db_data, &rsize))
                {

                    uint8_t db_array[14];

                    memset(db_array, 0x00, 14);

                    db_array[_dwlen++] = sleep_db_data.detect_time.year;
                    db_array[_dwlen++] = sleep_db_data.detect_time.month;
                    db_array[_dwlen++] = sleep_db_data.detect_time.day;
                    db_array[_dwlen++] = sleep_db_data.detect_time.hour;
                    db_array[_dwlen++] = sleep_db_data.detect_time.min;
                    db_array[_dwlen++] = sleep_db_data.detect_time.sec;
                    db_array[_dwlen++] = sleep_db_data.detect_time.sleep_state;
                    co_write32p(&db_array[_dwlen], sleep_db_data.period.exec_time_second);
                    _dwlen += 4;
                    db_array[_dwlen++] = sleep_db_data.detect_time.bCommandHeader;
                    co_write16p(&db_array[_dwlen], sleep_db_data.period.period_min);
                    _dwlen += 2;

                    app_ht_history_send(BLE_DB_SLEEP_COMMAND_ID, total, rec_index, db_array, _dwlen);

                }
                else
                    return;
            }
            else
            {
                app_ht_history_send(BLE_DB_SLEEP_COMMAND_ID, total, rec_index, NULL, _dwlen);

                TracerInfo("total:%d, index:%d, dayofweek:%d\r\n",total, rec_index, dayofweek);

            }

        }
        else
            return;

        CC_BLE_Cmd_UpdateHistoryRecordIndex();

        if(CC_BLE_Cmd_GetHistoryRecordIndex() > total)
        {
            if(total != 0)
                CC_DB_Delete_OneDay_File(eSleep, dayofweek);

            CC_BLE_Cmd_ClrHistoryRecordIndex();
            CC_BLE_Cmd_UpdateHistoryDayIndex();

            return;
        }
    }
    else
    {
        if(0x00 != CC_BLE_Cmd_GetHistoryType())
        {
            CC_VENUS_RscTimerStop();

            CC_BLE_Cmd_ClrHistoryType();

            CC_VENUS_RscTimerStart(SPEED_AND_CADENCE_MEAS_INTERVAL);
        }
        return;
    }
}

static void _Ble_CommandHistroySwimParse(void)
{
    uint32_t    _dwlen =0;
    uint16_t    total;
    uint16_t    rec_index;
    db_swimming_t swim_db_data;
    uint16_t rsize;
    app_date_time_t curr_time;
    uint8_t dayofweek;

    CC_MainGet_CurrentTime(&curr_time);

    if(CC_BLE_Cmd_GetHistoryDayIndex() <= 7)
    {
        dayofweek = (curr_time.dayofweek + CC_BLE_Cmd_GetHistoryDayIndex()) % 7;

        rec_index = CC_BLE_Cmd_GetHistoryRecordIndex();

        if(CC_SUCCESS == CC_Read_DayofRecoordLen(eSwim, dayofweek, &total))
        {
            if((total != 0) && (rec_index <= total))
            {

                if(CC_SUCCESS == CC_Read_Record(eSwim, dayofweek, rec_index, (uint32_t*)&swim_db_data, &rsize))
                {

                    uint8_t db_array[14];

                    memset(db_array, 0x00, 14);

                    db_array[_dwlen++] = swim_db_data.section.section_num;
                    db_array[_dwlen++] = swim_db_data.section.uInfo.cInfo;
                    db_array[_dwlen++] = swim_db_data.start_time.year;
                    db_array[_dwlen++] = swim_db_data.start_time.month;
                    db_array[_dwlen++] = swim_db_data.start_time.day;
                    db_array[_dwlen++] = swim_db_data.start_time.hour;
                    db_array[_dwlen++] = swim_db_data.start_time.min;
                    db_array[_dwlen++] = swim_db_data.start_time.sec;
                    co_write16p(&db_array[_dwlen], swim_db_data.endofperiod.swimming_count);
                    _dwlen += 2;
                    co_write16p(&db_array[_dwlen], swim_db_data.start_time.lap_number);
                    _dwlen += 2;
                    co_write16p(&db_array[_dwlen], swim_db_data.endofperiod.time_period);
                    _dwlen += 2;

                    app_ht_history_send(BLE_DB_SWIM_COMMAND_ID, total, rec_index, db_array, _dwlen);
                }
                else
                    return;
            }
            else
            {
                app_ht_history_send(BLE_DB_SWIM_COMMAND_ID, total, rec_index, NULL, _dwlen);

                TracerInfo("total:%d, index:%d, dayofweek:%d\r\n",total, rec_index, dayofweek);

            }

        }
        else
            return;

        CC_BLE_Cmd_UpdateHistoryRecordIndex();

        if(CC_BLE_Cmd_GetHistoryRecordIndex() > total)
        {
            if(total != 0)
                CC_DB_Delete_OneDay_File(eSwim, dayofweek);

            CC_BLE_Cmd_ClrHistoryRecordIndex();
            CC_BLE_Cmd_UpdateHistoryDayIndex();

            return;
        }
    }
    else
    {
        if(0x00 != CC_BLE_Cmd_GetHistoryType())
        {
            CC_VENUS_RscTimerStop();

            CC_BLE_Cmd_ClrHistoryType();

            CC_VENUS_RscTimerStart(SPEED_AND_CADENCE_MEAS_INTERVAL);
        }
        return;
    }
}

#endif


static void ble_notify_handler(void)
{
    //uint32_t        err_code;
    //uint8_t Index =0;
    //uint16_t total;
    uint8_t db_notify_id;

    if(0x00 != (db_notify_id = CC_BLE_Cmd_GetHistoryType()))
    {
        switch(db_notify_id)
        {
#ifdef DB_EN
            case 0xFA:
                _Ble_CommandHistroyPedoParse();
                break;
            case 0xFB:
                _Ble_CommandHistroyHrmParse();
                break;
            case 0xFC:
                _Ble_CommandHistroySleepParse();
                break;
            case 0xFD:
                _Ble_CommandHistroySwimParse();
                break;

#else   // w/o DB
            case 0xFA:
            case 0xFB:
            case 0xFC:
            case 0xFD:
                if(CC_BLE_Cmd_GetHistoryDayIndex() <= 7)
                {

                    app_ht_history_send(db_notify_id);

                    CC_BLE_Cmd_ClrHistoryRecordIndex();
                    CC_BLE_Cmd_UpdateHistoryDayIndex();
                    /*
                    CC_BLE_Cmd_UpdateHistoryRecordIndex();

                    if(CC_BLE_Cmd_GetHistoryRecordIndex() > total)
                    {
                        CC_BLE_Cmd_ClrHistoryRecordIndex();
                        CC_BLE_Cmd_UpdateHistoryDayIndex();
                    }
                    */
                }
                else
                {
                    if(0x00 != CC_BLE_Cmd_GetHistoryType())
                    {
                         CC_VENUS_RscTimerStop();

                         CC_BLE_Cmd_ClrHistoryType();

                         CC_VENUS_RscTimerStart(SPEED_AND_CADENCE_MEAS_INTERVAL);
                    }

                }

#endif

        }
    }
    else
    {
        if( 0x00 == s_tVenusCB.cSwimmingEn)
        {
            app_ht_period_meas_send(0x01, s_tVenusCB.dwPedTotalStepCount, s_tVenusCB.chrmDataReady_Flag, s_tVenusCB.cHrmData, s_tVenusCB.dwTotalCalorie);

            CC_MainSet_HrmDataFlag(false);
        }
        else
        {
            app_ht_swim_meas_send(0x01, s_tVenusCB.stSwimmingResult.swimstyle, s_tVenusCB.stSwimmingResult.swimcount, s_tVenusCB.dwSwimLapCnt, s_tVenusCB.stSwimmingResult.timestamp);
        }

    }
/*
    if(m_rscs.re_send == 0)
    {
        memset(&m_rscs.rscs_measurement,0,sizeof(ble_rscs_meas_t));

        if(0x00 != CC_Ble_CommandParse(Index,&m_rscs.rscs_measurement))
        {
            err_code = ble_rscs_measurement_send(&m_rscs, &m_rscs.rscs_measurement);

            if(err_code != CC_SUCCESS)
                m_rscs.re_send = 1;
            else
                m_rscs.re_send = 0;
        }
    }
    else
    {
        err_code = ble_rscs_measurement_send(&m_rscs, &m_rscs.rscs_measurement);

        if(err_code != CC_SUCCESS)
            m_rscs.re_send = 1;
        else
            m_rscs.re_send = 0;
    }
*/
}
#endif

static void rsc_meas_timeout_handler(void * p_context)
{


    VENUS_EVENT_ON(E_VENUS_EVENT_BLE_NOTIFY_TO, eEvent_None);
}


void ACC_init(void)
{
    if(LSM6DS3_X_Init() == MEMS_ERROR)
    {
        TracerInfo("LSM6DS3 Accelerometer initialize failed\r\n");
        cc6801_ClockDelayMs(10);
        g_GyroEnable = 0;
    }
    else
    {
        TracerInfo("LSM6DS3 Accelerometer initialize OK\r\n");
        cc6801_ClockDelayMs(10);
        g_GyroEnable = 1;
    }

#ifdef FORCE_SWIM_TEST_EN
    g_GyroEnable = 0;
#endif
}

void AKM_Data_Get(void)
{
    AK09912_MAG_GetMagRaw(&s_tMagRaw);
}


static void CC_ClockAlarm_Parse(void)
{
    #if 1
    for (int i =0 ; i< s_tVenusCB.stClockAlarm.cTotalNum ; i++)
    {
        if (s_tVenusCB.stClockAlarm.cAlarmTime[i].cSetting & 0x80)
        {
            s_tVenusCB.stClock[i].cEnable = 1;
            s_tVenusCB.stClock[i].cNotify = 0;
        }
        else
        {
            s_tVenusCB.stClock[i].cEnable = 0;
            s_tVenusCB.stClock[i].cNotify = 0;
        }
    }
    #endif
}

static void CC_ClockAlarm_Notified(void)
{
    for (int i =0; i <4 ;i++)
    {
        if (s_tVenusCB.stClock[i].cEnable == 1)
        {
            if (s_tVenusCB.stClockAlarm.cAlarmTime[i].cSetting & (1<< (6-s_tVenusCB.stSysCurTime.dayofweek)))
            {
                if ( s_tVenusCB.stClockAlarm.cAlarmTime[i].cHour == s_tVenusCB.stSysCurTime.hours )
                {
                     if (( s_tVenusCB.stClockAlarm.cAlarmTime[i].cMinute== s_tVenusCB.stSysCurTime.minutes) &&
                        (s_tVenusCB.stClock[i].cNotify == 0 ))
                     {
                        TracerInfo("Alarm Notify %d \r\n",i);
                        s_tVenusCB.stClock[i].cNotify=1;
                        VENUS_EVENT_ON(E_VENUS_EVENT_OLED_UPDATE_ALARM, eEvent_ALARM);
                     }
                }
            }
        }
    }
}

#ifdef HRM_EN

//void HRM_int_handler(nrf_drv_gpiote_pin_t pin, nrf_gpiote_polarity_t action)
void HRM_int_handler(void)
{
    VENUS_EVENT_ON(E_VENUS_EVENT_HRM , eEvent_None);
    CC_HRM_PPG_INTR_ISR();

}

void CC_SetHrmStatus(eHrmOp_State _eState)
{
    if (eHRM_On ==_eState)
        s_tVenusCB.cHrmOnGoing =eHRM_On;
    else
        s_tVenusCB.cHrmOnGoing =eHRM_Off;
}
eHrmOp_State CC_GetHrmStatus(void)
{
    return s_tVenusCB.cHrmOnGoing;
}
#endif

static void app_multiple_timer_init(void)
{

#ifndef SW_TIMER_BY_KERNEL  // not defined

    // Initialize timer module.
    APP_TIMER_INIT(APP_TIMER_PRESCALER, APP_TIMER_OP_QUEUE_SIZE, false);
#endif


    sw_timer_create(&m_battery_timer_id,
                      SW_TIMER_MODE_REPEATED,
                      battery_level_meas_timeout_handler);

    sw_timer_create(&m_rsc_meas_timer_id,
                     SW_TIMER_MODE_REPEATED,
                     rsc_meas_timeout_handler);

    sw_timer_create(&s_tVenusTimerAccel,
                     SW_TIMER_MODE_REPEATED,
                     cc_toolbox_sensor_accel_on_change);

    sw_timer_create(&s_tTouchDebounceTime,
                     SW_TIMER_MODE_SINGLE_SHOT,
                     cc_touch_debounce_timeout);

    sw_timer_create(&s_tVenusTimerDisplayGeneral,
                     SW_TIMER_MODE_SINGLE_SHOT,
                     cc_toolbox_Dispaly_General_handler);

    sw_timer_create(&s_tVenusTimerOLEDWakeup,
                     SW_TIMER_MODE_SINGLE_SHOT,
                     cc_toolbox_OLED_Wakup_TimeOut);

    sw_timer_create(&s_tVenusTimerOLEDSleep,
                     SW_TIMER_MODE_SINGLE_SHOT,
                     cc_toolbox_OLED_Sleep_TimeOut);

    sw_timer_create(&s_tVenusTimerOLEDDisplaySrvTimer,
                     SW_TIMER_MODE_SINGLE_SHOT,
                     cc_toolbox_OLED_Display_Service_timeout);

    sw_timer_create(&s_tVenusTimerDataTime,
                     SW_TIMER_MODE_REPEATED,
                     cc_toolbox_Datatime_hanndler);

    sw_timer_create(&s_tVenusTimerPWMVibSrvTimer,
                     SW_TIMER_MODE_SINGLE_SHOT,
                     cc_toolBox_PWM_Vib_Service_timeout);
    #ifdef APP_VIB_MGR
    sw_timer_create(&s_tVenusVibOpt,
                     SW_TIMER_MODE_SINGLE_SHOT,
                     cc_vibopt_timeout_handler);

    sw_timer_create(&s_tVenusVibExecInterval,
                     SW_TIMER_MODE_SINGLE_SHOT,
                     cc_vibexecinterval_timeout_handler);

    sw_timer_create(&s_tVenusVibDutyCycleChange,
                     SW_TIMER_MODE_SINGLE_SHOT,
                     cc_vibdutycyclechange_timeout_handler);
    #endif


}


#if 0
static uint32_t systime_ms = 0;
static void System_time_handler(void * pvContext)
{

    //systime_ms++;
    systime_ms+=20;

/*

  static uint8_t gpio_tmp = 0;

  gpio_tmp ^= 1;
  drvi_GpioWrite(GPIO_PIN_38, gpio_tmp);
  */
}

uint32_t Get_system_time_ms(void)
{
    return systime_ms;
}
#endif


static void application_timers_start(void)
{
    //uint32_t err_code;


    sw_timer_start(m_rsc_meas_timer_id, SPEED_AND_CADENCE_MEAS_INTERVAL, NULL);

    // Start application timers.
    //sw_timer_start(m_battery_timer_id, BATTERY_LEVEL_MEAS_INTERVAL, NULL);

    //err_code = sw_timer_start(m_rsc_meas_timer_id, SPEED_AND_CADENCE_MEAS_INTERVAL, NULL);
    //APP_ERROR_CHECK(err_code);

    // disable this timer to unify standby timers
    sw_timer_start(s_tVenusTimerAccel, ACCEL_FIFO_MODE_LONG_INTERVAL, NULL);

    sw_timer_start(s_tVenusTimerDataTime, DATETIME_CNT, NULL);

#if 0 //test time diff

    sw_timer_create(&s_tVenusTimerDiffTime,
                     SW_TIMER_MODE_REPEATED,
                     System_time_handler);

    sw_timer_start(s_tVenusTimerDiffTime, 20, NULL); // test by Samuel
#endif



}

static void CC_CalorieInfoSetting(void)
{
    // Calorie set Person info
    CC_ActivityInfo_t TmpActivityinfo;
    TmpActivityinfo.age = s_tVenusCB.stGeneralInfo.cAge;
    TmpActivityinfo.gender= (CC_Gender_t)s_tVenusCB.stGeneralInfo.cGender;
    TmpActivityinfo.height= (float)s_tVenusCB.stGeneralInfo.cHeight;
    TmpActivityinfo.weight= (float)s_tVenusCB.stGeneralInfo.cWeight;
    CC_CalorieBurn_Set_Settings(&TmpActivityinfo);
}


static void _System_DayLine_Change(void)
{
    if ((s_tVenusCB.stSysCurTime.hours == 0x00)&&
        (s_tVenusCB.stSysCurTime.minutes ==0x00)&&
        (s_tVenusCB.stSysCurTime.seconds==0x00))
    {
        // over midnight, reset pedo count
        //[VNS-24]Step number isn't reset at midnight 00:00:00
        CC_Reset_PedCount();

        //if (s_tVenusCB.cSwimmingEn ==1)
        //{
        //    _CC_DB_Save_SwimmingEnd();
        //}
        // clear Swimming DB all
        //_CC_DB_Clear_SwimmingDBSetting(eDayofChange);
         //change dayofweek so ,set "true" to reset one day file
        //CC_Fds_Change_DayofWeek(s_tVenusCB.stSysCurTime.dayofweek,true);
        //if (s_tVenusCB.cSwimmingEn ==1)
        //{
          //  _CC_DB_Save_SwimmingStart();
        //}

        for (int i =0 ; i< s_tVenusCB.stClockAlarm.cTotalNum ; i++)
        {
            //time 00:00 reset notify parameter

            CC_ClockAlarm_Parse();
        }

        //Jason, reset pede step
        s_tVenusCB.dwPedTotalStepCount =0;
        s_tVenusCB.dwPedRunCount=0;
        s_tVenusCB.dwPedWalkCount=0;
        s_tVenusCB.dwTotalCalorie=0;
        s_tVenusCB.dwWalkCalorie=0;
        s_tVenusCB.dwRunCalorie=0;


    }

    // Each minute compare it.
    if (s_tVenusCB.stSysCurTime.seconds ==0)
        CC_ClockAlarm_Notified();
}

void CC_Incommingcall_onNotified(void)
{

    if (eCALLINCOMMING == s_tVenusCB.cIncommingCallState)
    {
        TracerInfo("eCALLINCOMMING -> ON \r\n");
        VENUS_EVENT_ON(E_VENUS_EVENT_OLED_UPDATE , eEvent_INCOMMINGCALL_ON);
    }
    else if(eCALLINCOMMINGOFF == s_tVenusCB.cIncommingCallState)
    {
        VENUS_EVENT_ON(E_VENUS_EVENT_OLED_UPDATE , eEvent_INCOMMINGCALL_OFF);
    }
    else
        TracerInfo("eCALLINCOMMING -> state =%d \r\n",s_tVenusCB.cIncommingCallState);


}

void CC_Incommingsms_onNotified(void)
{
    if (eSMSCOMMING== s_tVenusCB.cIncommingSMSState)
    {
        TracerInfo("eSMSNCOMMING -> ON \r\n");
        VENUS_EVENT_ON(E_VENUS_EVENT_OLED_UPDATE , eEvent_INCOMMINGSMS);
    }
}


static void CC_LongSit_CheckNotifyProc(void)
{
    uint8_t _bHrmStatus=0xff;

    #if 0
    if (false == CC_SleepMonitor_GetSleepState())
    {
    #endif
    if ( false== CC_AppSrv_HR_IsHrmWorking())
    {
        if (false==CC_AppSrv_HR_IsSingleHrEnabled())
        {
            CC_AppSrv_HR_ClrHrmStatus();
            TracerInfo();
            TracerInfo("CC_LongSit_onNotified OPEN SINGLE HRM\r\n");
        }
        else
        {
            // should be error case
            CC_Longsit_Srv_Reset_Notification();
            TracerInfo("CC_LongSit_onNotified AVOID ERROR CASE TO RESET LONGSIT\r\n");
        }
        s_tVenusCB.LongSitWaitCnt =0;
    }
    else
    {
        _bHrmStatus = CC_AppSrv_HR_GetHrmStatus();

        if (0xff == _bHrmStatus)
        {
            TracerInfo("CC_LongSit_onNotified HRM PROCESS WAIT RESULT\r\n");
        }
        else
        {
            _bHrmStatus &=0x0f;
            if (( MSG_NO_TOUCH == _bHrmStatus))
            {
                CC_Longsit_Srv_Reset_Notification();
                CC_AppSrv_HR_StopSingleHR();
                TracerInfo("CC_LongSit_onNotified HRM NO TOUCH - NO OPEN OLED\r\n");

            }
            else
            {
                VENUS_EVENT_ON(E_VENUS_EVENT_OLED_UPDATE , eEvent_LONGSIT);
                CC_Longsit_Srv_Reset_Notification();
                TracerInfo("CC_LongSit_onNotified HRM HAS TOUCH  - OPEN OLED\r\n");
                CC_AppSrv_HR_StopSingleHR();
            }
        }

    }

}


void CC_LongSit_onNotified(uint8_t notified)
{
    TracerInfo("CC_LongSit_onNotified notified= %d \r\n",notified);
    //Enable vib
    if (notified == 1)
    {
        CC_LongSit_CheckNotifyProc();
    }
}


uint32_t g_dwSleepState = 0;
uint32_t g_dwSleepStateCount =0;
uint32_t CC_GetSleepAlgorithmReport_State(void)
{
    return g_dwSleepState;
}

uint32_t CC_GetSleep_StateCount(void)
{
    return g_dwSleepStateCount;
}

void CC_ResetSleep_StateCount(void)
{
    g_dwSleepStateCount = 0;
}

void _sensor_algorithm_sleepmeter_proc(void)
{

    slpmtr_input_t slpmtr_input;
    slpmtr_output_t slpmtr_output;

    if ((++g_bSleepEnCnt % 11)  == 0)
    {
        g_fSleepCalSeconds+=0.25f;
        slpmtr_input.mpss[0] = (float)(((double)_wAccelData[0]*9.8)/1024);
        slpmtr_input.mpss[1] = (float)(((double)_wAccelData[1]*9.8)/1024);
        slpmtr_input.mpss[2] = (float)(((double)_wAccelData[2]*9.8)/1024);


        slpmtr_detect(&slpmtr_input, g_fSleepCalSeconds, &slpmtr_output);


        if (slpmtr_output.NewStage != slpmtr_output.OldStage)
        {

            g_dwSleepState = slpmtr_output.NewStage;
            g_dwSleepStateCount++;

           //_CC_DB_Save_SleepProc(slpmtr_output);
           TracerInfo("CCSleep_onStateChange OldStage=%d, NewStage=%d \r\n",
                                            slpmtr_output.OldStage,
                                            slpmtr_output.NewStage);
           TracerInfo("CCSleep_onStateChange period=%d, time=%d \r\n",slpmtr_output.period, slpmtr_output.time);

           TracerInfo("System Date %d.%d.%d \r\n",s_tVenusCB.stSysCurTime.year,s_tVenusCB.stSysCurTime.month,s_tVenusCB.stSysCurTime.day);
           TracerInfo("System Clock %d:%d:%d \r\n",s_tVenusCB.stSysCurTime.hours,s_tVenusCB.stSysCurTime.minutes,s_tVenusCB.stSysCurTime.seconds);

        }
    }

    if (g_bSleepEnCnt == 50)
    {
        g_bSleepEnCnt= 0;
    }
}


eStete_t CC_Get_LongsitEnState(void)
{
    return s_tVenusCB.cLongsitEn;
}

uint32_t CC_MainGet_PedCnt(void)
{
    return s_tVenusCB.dwPedTotalStepCount;
}

#ifndef APP_SERV_MGR_EN    // not defined
static void _sensor_power_down_gryo(void)
{
    LSM6DS3_ACC_GYRO_GetRawGyroData16(NULL, (i16_t *) _wGyroData);
    CC_GYRO_Set_ODR(LSM6DS3_ACC_GYRO_ODR_G_POWER_DOWN);
    //nrf_gpio_cfg_input(14,NRF_GPIO_PIN_NOPULL);
}

static void _sensor_power_down_mag(void)
{
    //AKM_Data_Get(); // remarked by Samuel
    AK09912_MAG_SLEEP();
    //nrf_gpio_cfg_input(AKM_I2C_DAT,NRF_GPIO_PIN_PULLUP);
    //nrf_gpio_cfg_input(AKM_I2C_CLK,NRF_GPIO_PIN_PULLUP);
    //nrf_gpio_cfg_input(15,NRF_GPIO_PIN_NOPULL);
}
#endif

#ifdef PEDO_EN
static void _sensor_algorithm_liftarm_proc()
{
    //TracerInfo("_sensor_algorithm_liftarm_proc \r\n");
    if (eEnable== s_tVenusCB.cLiftarmEn)
    {
        //static uint8_t  _cTmp =0;
        //if (_cTmp ==0)
        if(1)
        {
            int32_t input[3] = {0};
            int8_t _GestureOut=0;
            int8_t _WinStatus=0;
            //y  to  ground

            #if 1
            input[0] = (int32_t)  _wAccelData[1]*9.8; //x
            input[1] = (int32_t)  0-_wAccelData[0]*9.8; //y
            input[2] = (int32_t)  _wAccelData[2]*9.8; //z
            #endif


            //TracerInfo( "oringin  x=[%d] y=[%d] z=[%d]\r\n",_wAccelData[0],_wAccelData[1],_wAccelData[2]);
            //TracerInfo( "change x=[%d] y=[%d] z=[%d]\r\n",input[0],input[1],input[2]);

#if 0 // test app loop time
            static uint32_t old_tick = 0;

            old_tick = Get_system_time_ms();
#endif


            liftarm_process( input,&_GestureOut, &_WinStatus);

            if (1 == _WinStatus)
            {
                if ( (eLiftarm_Up == (eLiftarm_Mode)_GestureOut)
                    || (eLiftarm_Down == (eLiftarm_Mode) _GestureOut))       /**< 1: up, 2: down */
                {
                    if (eLiftarm_Up == (eLiftarm_Mode)_GestureOut)
                    {
                        if (!CC_PageMgr_IsOLEDActive() && !CC_PageMgr_IsBlockingOLED())
                        {
                            CC_PageMgr_ActiveOLED();
                            VENUS_EVENT_ON(E_VENUS_EVENT_OLED_UPDATE_LIFTARM_UP, eEvent_LIFTARM_UP);
							g_bLiftArm_State = (eLiftarm_Mode)_GestureOut;
                        }

                    }
                    else if (( eLiftarm_Down == (eLiftarm_Mode) _GestureOut )
                             && ( eLiftarm_Up == g_bLiftArm_State)) // have liftarm up than have liftarm down
                    {
                        if (CC_PageMgr_IsOLEDActive())
                        {
                            VENUS_EVENT_ON(E_VENUS_EVENT_OLED_UPDATE_LIFTARM_DOWN, eEvent_LIFTARM_DOWN);
                            g_bLiftArm_State = (eLiftarm_Mode)_GestureOut;
                        }
                    }
                }
            }


        }
        //_cTmp^=1;
    }

}



uint32_t g_dwLapCnt =0;
uint32_t g_dwStroke =0;
uint32_t CC_Get_SwimmingLapCount(void)
{
    return g_dwLapCnt;
}

uint32_t CC_Get_SwimmingStrokeCount(void)
{
    return g_dwStroke;
}

static void _sensor_algorithm_swimming_proc(void)
{

    raw_sensor_event _stSwmimingData;
    static int16_t wacc_data[FIFO_DEPTH_T];    // offer double memory space
    static int16_t wgyro_data[FIFO_DEPTH_T];   // offer double memory space

    //static uint32_t old_time = 0;
    //static uint32_t curr_time = 0;
    //TracerInfo("_sensor_algorithm_swimming_proc \r\n");

    uint32_t fifo_total_samples = 0;


#ifdef APP_SERV_MGR_EN
    static uint32_t fifo_acc_index = 0;
    static uint32_t fifo_gyro_index = 0;
    uint32_t size_in_bytes;
    uint32_t dummy_size;

    if(E_SEN_ERROR_NONE == CC_SenMgr_Acc_GetData(E_SEN_USER_ID_SWIM, &wacc_data[fifo_acc_index], &size_in_bytes))
    {
        fifo_acc_index += size_in_bytes/2; // to 16bits data
    }
    else
        TracerInfo("CC_SenMgr_Acc_GetData fail!\r\n");


    //TracerInfo("pGyro_addr0: 0x%x\r\n", &wgyro_data[fifo_gyro_index]);

    if(E_SEN_ERROR_NONE == CC_SenMgr_Gyro_GetData(E_SEN_USER_ID_SWIM, &wgyro_data[fifo_gyro_index], &size_in_bytes))
    {
        fifo_gyro_index += size_in_bytes/2; // to 16bits data
    }
    else
        TracerInfo("CC_SenMgr_Gyro_GetData fail!\r\n");


    fifo_total_samples = fifo_acc_index/3; // xyz to 1 samples

    if(E_SEN_ERROR_NONE != CC_SenMgr_Mag_GetData(E_SEN_USER_ID_SWIM, (int16_t*)&s_tMagRaw, &dummy_size))
        TracerInfo("CC_SenMgr_Mag_GetData fail!\r\n");

#else


    CC_LSM6DSX_Fifo_Update_Data();

    fifo_total_samples = CC_LSM6DSX_Fifo_Get_Accel_UnRead_Samples(MEMS_FIFO_USER_SWIM);

    //TracerInfo("swim_len: %d\r\n", fifo_len);

    AKM_Data_Get();
#endif


    if ((s_tMagRaw.AXIS_X == 0) &&
        (s_tMagRaw.AXIS_Y == 0) &&
        (s_tMagRaw.AXIS_Z == 0))
    {
         TracerInfo( "Swimming Send Mag data  invaild !!!!!!!!!!!!!\r\n");
    }
    else
    {
         memcpy( &_stSwmimingData.mag_data,&s_tMagRaw,sizeof(s_tMagRaw));
                _stSwmimingData.type |= MAG_SENSOR;

         CC_Swimming_Proc(_stSwmimingData, &s_tVenusCB.stSwimmingResult);

         _stSwmimingData.type = 0;
    }


    if(fifo_total_samples >= 260) // 260 samples(x,y,z) @5sec
    {

#ifdef APP_SERV_MGR_EN
        fifo_acc_index = fifo_gyro_index = 0;

#else
        memcpy(wacc_data, s_tAcc.wbuf, (fifo_total_samples*3*2));   // *3 => x,y,z; *2 => 16bits
        memcpy(wgyro_data, s_tGyro.wbuf, (fifo_total_samples*3*2)); // *3 => x,y,z; *2 => 16bits

         //curr_time = Hrm_get_sys_tick();
         //TracerInfo("diff_time: %d\r\n", curr_time - old_time);

         //TracerInfo("acc_nb: %d, gyro_nb: %d\r\n", CC_LSM6DSX_Fifo_Get_Accel_UnRead_Samples(MEMS_FIFO_USER_SWIM),
         //                                            CC_LSM6DSX_Fifo_Get_Gyro_UnRead_Samples(MEMS_FIFO_USER_SWIM));
         //old_time = curr_time;

         //old_time = Hrm_get_sys_tick();

        CC_LSM6DSX_Fifo_Accel_Read_Done(MEMS_FIFO_USER_SWIM);
        CC_LSM6DSX_Fifo_Gyro_Read_Done(MEMS_FIFO_USER_SWIM);

#endif


        for(int i = 0; i < fifo_total_samples; i++)
        {


             int k = 3*i;

             _wAccelData[0] = wacc_data[k];
             _wAccelData[1] = wacc_data[k+1];
             _wAccelData[2] = wacc_data[k+2];

             _wGyroData[0] = wgyro_data[k];
             _wGyroData[1] = wgyro_data[k+1];
             _wGyroData[2] = wgyro_data[k+2];

#ifdef FORCE_SWIM_TEST_EN
            if(k == 0)  // only print 1st data
            {
                TracerInfo( "ACC_Data[0] %d\r\n",_wAccelData[0]);
                TracerInfo( "ACC_Data[1] %d\r\n",_wAccelData[1]);
                TracerInfo( "ACC_Data[2] %d\r\n",_wAccelData[2]);

                TracerInfo( "GYRO_Data[0] %d\r\n",_wGyroData[0]);
                TracerInfo( "GYRO_Data[1] %d\r\n",_wGyroData[1]);
                TracerInfo( "GYRO_Data[2] %d\r\n",_wGyroData[2]);

                TracerInfo( "MAG_Data[0] %d\r\n",s_tMagRaw.AXIS_X);
                TracerInfo( "MAG_Data[1] %d\r\n",s_tMagRaw.AXIS_Y);
                TracerInfo( "MAG_Data[2] %d\r\n",s_tMagRaw.AXIS_Z);
            }
#endif
            memcpy( &_stSwmimingData.acc_data,_wAccelData,sizeof(_wAccelData));
            memcpy( &_stSwmimingData.gyro_data,_wGyroData,sizeof(_wGyroData));
            _stSwmimingData.type= ACC_SENSOR | GYRO_SENSOR;

#if 0 // test app loop time
            static uint32_t old_tick = 0;
            static uint32_t diff_time = 0;
            static uint32_t max_diff_time = 0;

            old_tick = Get_system_time_ms();
#endif

            CC_Swimming_Proc(_stSwmimingData, &s_tVenusCB.stSwimmingResult);

#if 0 // test app loop time
            diff_time = Get_system_time_ms() - old_tick;

            if(max_diff_time < diff_time)
                max_diff_time = diff_time;

            TracerInfo("sw:%d / %d\r\n", diff_time, max_diff_time);
#endif

            g_dwLapCnt = s_tVenusCB.stSwimmingResult.swimlap;
            g_dwStroke = s_tVenusCB.stSwimmingResult.swimcount;


            if ( s_tVenusCB.dwSwimLapCnt != s_tVenusCB.stSwimmingResult.swimlap) // swim lap
            {
                s_tVenusCB.dwSwimLapCnt = s_tVenusCB.stSwimmingResult.swimlap;

                TracerInfo( "Swimming dwSwimLapCnt = %d \r\n",s_tVenusCB.dwSwimLapCnt);
    #ifdef DB_EN
                //_CC_DB_Save_SwimmingProc();
    #endif

                switch (s_tVenusCB.stGeneralInfo.cSwim_Pool_Size)
                {
                    case eSWIM_25M:
                        s_tVenusCB.wSwimmingLen = s_tVenusCB.dwSwimLapCnt * 25;
                    break;
                    case eSWIM_50M:
                        s_tVenusCB.wSwimmingLen = s_tVenusCB.dwSwimLapCnt * 50;
                    break;
                    case eSWIM_25YD:
                        s_tVenusCB.wSwimmingLen = s_tVenusCB.dwSwimLapCnt * 25;
                    break;
                    case eSWIM_33_33M:
                        s_tVenusCB.wSwimmingLen = (double)s_tVenusCB.dwSwimLapCnt * 33.33;
                    break;
                    case eSWIM_33_33YD:
                        s_tVenusCB.wSwimmingLen = (double) s_tVenusCB.dwSwimLapCnt * 33.33;
                    break;
                    default:
                        break;
                }
            }

        }


        //curr_time = Hrm_get_sys_tick();

        //TracerInfo("diff_time: %d\r\n", curr_time - old_time);
    }
#if 0 // old
    raw_sensor_event _stSwmimingData;

    //TracerInfo("_sensor_algorithm_swimming_proc \r\n");

    LSM6DS3_ACC_GYRO_GetRawAccData(NULL, (i16_t *) _wAccelData);
    LSM6DS3_ACC_GYRO_GetRawGyroData16(NULL, (i16_t *) _wGyroData);

    memcpy( &_stSwmimingData.acc_data,_wAccelData,sizeof(_wAccelData));
    memcpy( &_stSwmimingData.gyro_data,_wGyroData,sizeof(_wGyroData));
    _stSwmimingData.type= ACC_SENSOR | GYRO_SENSOR;

    if ((++g_bMagEnCnt % 5)  == 0) // 1 s
    {
        AKM_Data_Get();
        if ((s_tMagRaw.AXIS_X == 0) &&
            (s_tMagRaw.AXIS_Y == 0) &&
            (s_tMagRaw.AXIS_Z == 0))
        {
            TracerInfo( "Swimming Send Mag data  invaild !!!!!!!!!!!!!\r\n");
        }
        else
        {
            memcpy( &_stSwmimingData.mag_data,&s_tMagRaw,sizeof(s_tMagRaw));
            _stSwmimingData.type |= MAG_SENSOR;
            if (g_bMagEnCnt == 5)
            {
                g_bMagEnCnt= 0;
                //TracerInfo("reset Mag Count = %d\r\n",_cMagEnCnt);

            }

        }

    }

    CC_Swimming_Proc(_stSwmimingData, &s_tVenusCB.stSwimmingResult);

    g_dwLapCnt = s_tVenusCB.stSwimmingResult.swimlap;
    g_dwStroke = s_tVenusCB.stSwimmingResult.swimcount;


    //if(SWIMMING_OTHERS != s_tVenusCB.stSwimmingResult.swimstyle) //TBD
    //if(true == CC_CheckSwimStrokeChanged(s_tVenusCB.stSwimmingResult.swimcount))
    //{
      //  CC_VENUS_Lock_SwimOff_TimerStop();
      //  CC_VENUS_Lock_SwimOff_TimerStart();
    //}


    if ( s_tVenusCB.dwSwimLapCnt != s_tVenusCB.stSwimmingResult.swimlap) // swim lap
    {
        s_tVenusCB.dwSwimLapCnt = s_tVenusCB.stSwimmingResult.swimlap;

        TracerInfo( "Swimming dwSwimLapCnt = %d \r\n",s_tVenusCB.dwSwimLapCnt);
        #ifdef DB_EN
        //_CC_DB_Save_SwimmingProc();
        #endif



        switch (s_tVenusCB.stGeneralInfo.cSwim_Pool_Size)
        {
            case eSWIM_25M:
                s_tVenusCB.wSwimmingLen = s_tVenusCB.dwSwimLapCnt * 25;
            break;
            case eSWIM_50M:
                s_tVenusCB.wSwimmingLen = s_tVenusCB.dwSwimLapCnt * 50;
            break;
            case eSWIM_25YD:
                s_tVenusCB.wSwimmingLen = s_tVenusCB.dwSwimLapCnt * 25;
            break;
            case eSWIM_33_33M:
                s_tVenusCB.wSwimmingLen = (double)s_tVenusCB.dwSwimLapCnt * 33.33;
            break;
            case eSWIM_33_33YD:
                s_tVenusCB.wSwimmingLen = (double) s_tVenusCB.dwSwimLapCnt * 33.33;
            break;
            default:
                break;
        }
    }
#endif
}

//void _DumpSensorRegisterMap(void); //test


void _sensor_accel_gyro_on_change(void)
{

    s_tVenusCB.cSwimmingEn = CC_BLE_Cmd_GetSwimmingEN();

    if ( 0== s_tVenusCB.cSwimmingEn)
    {

        if (g_GyroEnable ==1)
        {
            CC_VENUS_AccelTimerStop();
            CC_VENUS_AccelTimerReset();

#ifdef APP_SERV_MGR_EN
            APP_SVCMGR_PostEvent_PedoRequest(ENABLE);
#else
            CC_LSM6DSX_Fifo_Accel_UnRegister(MEMS_FIFO_USER_SWIM);
            CC_LSM6DSX_Fifo_Gyro_UnRegister(MEMS_FIFO_USER_SWIM);
            CC_LSM6DSX_FifoDisable(E_LSM6DSX_FIFO_CONTROL_ACCEL_GYRO);
            CC_LSM6DSX_FifoEnable(E_LSM6DSX_FIFO_CONTROL_ACCEL);
            //CC_LSM6DSX_FifoEnable(E_LSM6DSX_FIFO_CONTROL_GYRO);  // test by Samuel!!!
            //CC_LSM6DSX_FifoEnable(E_LSM6DSX_FIFO_CONTROL_ACCEL_GYRO);  // test by Samuel!!!
            CC_LSM6DSX_Fifo_Accel_Register(MEMS_FIFO_USER_PEDO, s_tAcc.wbuf, (FIFO_DEPTH_T/2));
            //CC_LSM6DSX_Fifo_Gyro_Register(MEMS_FIFO_USER_PEDO, s_tGyro.wbuf, (FIFO_DEPTH_T/2));//test

            _sensor_power_down_gryo();
            _sensor_power_down_mag();
#endif

            g_GyroEnable = 0;
            //_CC_DB_Save_SwimmingEnd();
            //_DumpSensorRegisterMap(); //test!!!
            CC_VENUS_AccelTimerFifoModeStart();

        }
        else
        {

                uint32_t fifo_len;
                int16_t wacc_data[FIFO_DEPTH_T/2];
                //int16_t wgyro_data[FIFO_DEPTH_T/2];
                //static uint32_t old_tick = 0;

#ifdef APP_SERV_MGR_EN
                uint32_t size_in_bytes;

                if(E_SEN_ERROR_NONE != CC_SenMgr_Acc_GetData(E_SEN_USER_ID_PEDO, wacc_data, &size_in_bytes))
                    TracerInfo("CC_SenMgr_Acc_GetData fail!\r\n");

                fifo_len = size_in_bytes/(2*3); // to 16bits xyz => sampls numbers


               // TracerInfo("size_in_bytes: %d\r\n", size_in_bytes);
#else
                CC_LSM6DSX_Fifo_Update_Data();

                fifo_len = CC_LSM6DSX_Fifo_Get_Accel_UnRead_Samples(MEMS_FIFO_USER_PEDO);

                //TracerInfo("fifo_len: %d\r\n", fifo_len);

                //TracerInfo("Acc_fifo_len: %d, time_diff: %d\r\n", fifo_len, Get_system_time_ms() - old_tick);
                //fifo_len = CC_LSM6DSX_Fifo_Get_Gyro_UnRead_Samples(MEMS_FIFO_USER_PEDO);         // test

                //TracerInfo("Gyro_fifo_len: %d, time_diff: %d\r\n", fifo_len, Hrm_get_sys_tick() - old_tick);


                //old_tick = Get_system_time_ms();

                memcpy(wacc_data, s_tAcc.wbuf, (fifo_len*3*2));  // *3 => x,y,z; *2 => 16bits

                //memcpy(wgyro_data, s_tGyro.wbuf, (fifo_len*3*2));  // test

                CC_LSM6DSX_Fifo_Accel_Read_Done(MEMS_FIFO_USER_PEDO);

                //CC_LSM6DSX_Fifo_Gyro_Read_Done(MEMS_FIFO_USER_PEDO); //test
#endif

                // Charge-in or low power, skip to execute pedometer, liftarm and sleep algorithm
               // if ((eDEVICE_CHARGE_IN== s_tVenusCB.eChargingState) ||
                 //   (true == s_tVenusCB.bIsLowPower))
                   // return;


                for(uint16_t i = 0; i < fifo_len; i++)
                {
                    uint16_t k = 3*i;
#ifdef SWAP_ACC_DIRECTION_EN
                   _wAccelData[0] = (wacc_data[k+1] / 4);
                   _wAccelData[1] = 0-(wacc_data[k] / 4);
                   _wAccelData[2] = (wacc_data[k+2] / 4);
#else
                   _wAccelData[0] = (wacc_data[k] / 4);
                   _wAccelData[1] = (wacc_data[k+1] / 4);
                   _wAccelData[2] = (wacc_data[k+2] / 4);
#endif
                     _sensor_algorithm_liftarm_proc();


                    //TracerInfo( "ACC_Data[0] %d\r\n",wacc_data[k]);
                    //TracerInfo( "ACC_Data[1] %d\r\n",wacc_data[k+1]);
                    //TracerInfo( "ACC_Data[2] %d\r\n",wacc_data[k+2]);

                    //TracerInfo( "GYRO_Data[0] %d\r\n",wgyro_data[k]);
                    //TracerInfo( "GYRO_Data[1] %d\r\n",wgyro_data[k+1]);
                    //TracerInfo( "GYRO_Data[2] %d\r\n",wgyro_data[k+2]);


                    uint32_t       _dwPedTotalStepCount = 0;
                    signed char    _cPedState = ePedo_Stop;
#ifdef SWAP_ACC_DIRECTION_EN
                    _wAccelData[0] = (wacc_data[k] / 4);
                    _wAccelData[1] = (wacc_data[k+1] / 4);
                    _wAccelData[2] = (wacc_data[k+2] / 4);
#else
                    // Made same with Apollo axis
                    _wAccelData[0] = 0-(wacc_data[k+1] / 4); // x->y
                    _wAccelData[1] = (wacc_data[k] / 4); //y ->-x
                    _wAccelData[2] = (wacc_data[k+2] / 4);
                    // x -> to ground, so x shift to y, y shit to -x
#endif

#if 0 // test app loop time

                    old_tick = Get_system_time_ms();
#endif

                    FP_PED_8Bit(((const short *) _wAccelData), &_dwPedTotalStepCount, &_cPedState);

#if 0 // test app loop time
                    TracerInfo("%d\r\n", Get_system_time_ms() - old_tick);
#endif

                    //TracerInfo("%d,%d,%d\r\n", _wAccelData[0], _wAccelData[1], _wAccelData[2]);
                    if (s_tVenusCB.dwPedTotalStepCount != _dwPedTotalStepCount)
                    {
                        TracerInfo("ped_cnt: %d, ped_st: %d\r\n", _dwPedTotalStepCount, _cPedState);

#ifdef DB_EN

                        if(ePedo_Walk == _cPedState)
                        {
                                s_tVenusCB.dwPedWalkCount = s_tVenusCB.dwPedWalkCount + ( _dwPedTotalStepCount - s_tVenusCB.dwPedTotalStepCount );
                                s_tVenusCB.dwWalkCalorie = CC_CalorieBurnCalStepV2(s_tVenusCB.dwPedWalkCount, 1,ePedo_Walk);


                                if(0x00 == s_tVenusCB.stPedRecordData_Walk.enabled)
                                {
                                    // init:
                                    s_tVenusCB.stPedRecordData_Walk.enabled = 1;
                                    //s_tVenusCB.stPedRecordData_Walk.saveDB = 0;
                                    s_tVenusCB.stPedRecordData_Walk.pedCount = 0;

                                    CC_MainGet_CurrentTime(&s_tVenusCB.stPedRecordData_Walk.startTime);
                                }

                                if(_dwPedTotalStepCount > s_tVenusCB.dwPedTotalStepCount)
                                {
                                    s_tVenusCB.stPedRecordData_Walk.pedCount += _dwPedTotalStepCount - s_tVenusCB.dwPedTotalStepCount;

                                    if(ePedo_Run == s_tVenusCB.cPedLastMovingState)
                                    {
                                        s_tVenusCB.stPedRecordData_Run.saveDB = 1;

                                        CC_VENUS_RUN_PedoTimerStop();
                                    }

                                    s_tVenusCB.cPedLastMovingState = ePedo_Walk;
                                }

                                s_tVenusCB.stPedRecordData_Walk.saveDB = 0;

                                CC_VENUS_WALK_PedoTimerStop();
                                CC_VENUS_WALK_PedoTimerStart(DB_PEDO_WALK_TIMEOUT_INTERVAL);

                        }
                        else if(ePedo_Run == _cPedState)
                        {
                                s_tVenusCB.dwPedRunCount = s_tVenusCB.dwPedRunCount+ ( _dwPedTotalStepCount - s_tVenusCB.dwPedTotalStepCount );
                                s_tVenusCB.dwRunCalorie = CC_CalorieBurnCalStepV2(s_tVenusCB.dwPedRunCount, 1,ePedo_Run);

                                if(0x00 == s_tVenusCB.stPedRecordData_Run.enabled)
                                {
                                    // init:
                                    s_tVenusCB.stPedRecordData_Run.enabled = 1;
                                    //s_tVenusCB.stPedRecordData_Run.saveDB = 0;
                                    s_tVenusCB.stPedRecordData_Run.pedCount = 0;

                                    CC_MainGet_CurrentTime(&s_tVenusCB.stPedRecordData_Run.startTime);
                                }

                                if(_dwPedTotalStepCount > s_tVenusCB.dwPedTotalStepCount)
                                {

                                    s_tVenusCB.stPedRecordData_Run.pedCount += _dwPedTotalStepCount - s_tVenusCB.dwPedTotalStepCount;

                                    if(ePedo_Walk == s_tVenusCB.cPedLastMovingState)
                                    {
                                        s_tVenusCB.stPedRecordData_Walk.saveDB = 1;

                                        CC_VENUS_WALK_PedoTimerStop();
                                    }

                                    s_tVenusCB.cPedLastMovingState = ePedo_Run;
                                }

                                s_tVenusCB.stPedRecordData_Run.saveDB = 0;

                                CC_VENUS_RUN_PedoTimerStop();
                                CC_VENUS_RUN_PedoTimerStart(DB_PEDO_RUN_TIMEOUT_INTERVAL);

                        }
#endif
                        s_tVenusCB.dwPedTotalStepCount = _dwPedTotalStepCount;
                        s_tVenusCB.dwTotalCalorie = s_tVenusCB.dwWalkCalorie + s_tVenusCB.dwRunCalorie;


                        TracerInfo("calorie %d\r\n", s_tVenusCB.dwTotalCalorie);

                    }

#ifdef DB_EN
                    if(s_tVenusCB.cPedState != _cPedState)
                    {

                        if((_cPedState == ePedo_Walk) || (_cPedState == ePedo_Stop))// walk or no motion
                        {
                            // set Running stop time:
                            if((0x01 == s_tVenusCB.stPedRecordData_Run.enabled)
                                && (s_tVenusCB.stPedRecordData_Run.saveDB == 0))
                            {
                                CC_MainGet_CurrentTime(&s_tVenusCB.stPedRecordData_Run.stopTime);
                            }

                        }

                        if((_cPedState == ePedo_Run) || (_cPedState == ePedo_Stop))// run or no motion
                        {
                            // set Walking stop time:
                            if((0x01 == s_tVenusCB.stPedRecordData_Walk.enabled)
                                && (s_tVenusCB.stPedRecordData_Walk.saveDB == 0))
                            {
                                CC_MainGet_CurrentTime(&s_tVenusCB.stPedRecordData_Walk.stopTime);
                            }

                        }

                    }

                    s_tVenusCB.cPedState = _cPedState;
#endif

#ifdef SLEEP_EN
                    CC_SleepMonitor_Srv_Handle();
#endif
                }
        }
    }
    else
    {

        if (g_GyroEnable ==0 )
        {
                CC_VENUS_AccelTimerStop();
                CC_VENUS_AccelTimerReset();
#ifdef APP_SERV_MGR_EN
                APP_SVCMGR_PostEvent_PedoRequest(DISABLE);

                APP_SVCMGR_PostEvent_SwimRequest(ENABLE);

#else
                CC_LSM6DSX_Fifo_Accel_UnRegister(MEMS_FIFO_USER_PEDO);
                CC_LSM6DSX_FifoDisable(E_LSM6DSX_FIFO_CONTROL_ACCEL_GYRO);
                CC_LSM6DSX_FifoEnable(E_LSM6DSX_FIFO_CONTROL_ACCEL_GYRO);
                CC_LSM6DSX_Fifo_Accel_Register(MEMS_FIFO_USER_SWIM, s_tAcc.wbuf, (FIFO_DEPTH_T/2));
                CC_LSM6DSX_Fifo_Gyro_Register(MEMS_FIFO_USER_SWIM, s_tGyro.wbuf, (FIFO_DEPTH_T/2));


                CC_AK09912_MAG_SET_ODR(AK09912_MAG_DO_10_Hz);
#endif
                //Init Swimming algo and data
                CC_Swim_Init(s_tVenusCB.stGeneralInfo.cSwim_Pool_Size,(band_location)s_tVenusCB.stGeneralInfo.bBandLocation);
                s_tVenusCB.dwSwimLapCnt = 0;
                s_tVenusCB.dwLastSwimcount =0;
                s_tVenusCB.wSwimmingLen = 0;
                memset(&s_tVenusCB.stLastLapTime,0x00,sizeof(app_date_time_t));

                memset(&s_tVenusCB.stSwimmingResult,0x00,sizeof(result_event));
                g_GyroEnable = 1;

                //Start Record Swimming
                //_CC_DB_Save_SwimmingStart();

                CC_VENUS_SwimTimerFifoModeStart();

        }
        else
        {
            _sensor_algorithm_swimming_proc();
        }

    }
}


#endif

#ifdef FACTORY_RESET
void CC_SYS_FactroyReset_Setting(void)
{
    CC_Vib_Srv_Reset();
//    CC_BLE_Cmd_SetSwimmingEN(0);
    s_tVenusCB.cSwimmingEn = CC_BLE_Cmd_GetSwimmingEN();

#if 0 // it should be open here, but it can make re-connection fail?
    if (CC_AppSrv_HR_IsHrmWorking())
    {

        if (CC_AppSrv_HR_IsSingleHrEnabled())
            CC_AppSrv_HR_StopSingleHR();

        if (CC_AppSrv_HR_IsHrsEnabled())
        {
            CC_BLE_Cmd_SetNotificaitonState(eDisable,eNOTIFY_SETTING_HEARTRATESTRAP_MODE);
            CC_AppSrv_HR_StopHRS();
        }
        if (CC_AppSrv_HR_Is24HrEnabled())
        {
            CC_BLE_Cmd_SetNotificaitonState(eDisable,eNOTIFY_SETTING_24HHEARTRATE_MODE);
            CC_AppSrv_HR_Stop24HR();
        }

    }
#endif

}


void CC_SYS_FactoryReset_Handler(void)
{
#ifdef DB_EN
    //DB data reset
    CC_DB_Factory_System_DataReset();

    CC_DB_Set_Init_Done();
#endif
    //System data reset
    CC_BLE_Cmd_GetGeneralInfo(&s_tVenusCB.stGeneralInfo,true);
    CC_AppSrv_HR_ResetLimited(s_tVenusCB.stGeneralInfo.cAge);

    CC_CalorieInfoSetting();

    CC_BLE_Cmd_GetUnitInfo(&s_tVenusCB.stUnitInfo,true);

    CC_BLE_Cmd_GetClockAlarm(&s_tVenusCB.stClockAlarm,true);
    CC_ClockAlarm_Parse();

    CC_BLE_Cmd_GetSleepTimeSetting(&s_tVenusCB.stSleepAlgExecPeriod,true);

    CC_BLE_Cmd_GetCallState(&s_tVenusCB.cNotify_flag,
                                                    &s_tVenusCB.cIncommingCallState,
                                                    &s_tVenusCB.cIncommingSMSState,
                                                    &s_tVenusCB.cLongsitEn,
                                                    &s_tVenusCB.cLiftarmEn);

#ifdef DB_EN

    CC_DB_System_Save_Request(DB_SYS_GENERAL_INFO);
    CC_DB_System_Save_Request(DB_SYS_ALARM);
    CC_DB_System_Save_Request(DB_SYS_NOTIFY);
    CC_DB_System_Save_Request(DB_SYS_UNIT);
    CC_DB_System_Save_Request(DB_SYS_SLEEP_MONITOR_TIME_SETTING);

    CC_DB_System_Check_Request_And_Save();
#endif
    //Algorithm Reset
    //Swim
    CC_Swim_DeInit();
    CC_Swim_Init(s_tVenusCB.stGeneralInfo.cSwim_Pool_Size,(band_location)s_tVenusCB.stGeneralInfo.bBandLocation);
    s_tVenusCB.dwSwimLapCnt = 0;
    s_tVenusCB.dwLastSwimcount =0;
    s_tVenusCB.wSwimmingLen = 0;
    memset(&s_tVenusCB.stLastLapTime,0x00,sizeof(app_date_time_t));

    memset(&s_tVenusCB.stSwimmingResult,0x00,sizeof(result_event));

    //Pedo and calorie
    s_tVenusCB.dwPedTotalStepCount =0;
    s_tVenusCB.dwPedRunCount=0;
    s_tVenusCB.dwPedWalkCount=0;
    s_tVenusCB.dwTotalCalorie=0;
    s_tVenusCB.dwWalkCalorie=0;
    s_tVenusCB.dwRunCalorie=0;

    FP_PED_8Bit_terminate();
    FP_PED_8Bit_initialize();
    CC_CalorieBurn_Close();
    CC_CalorieBurn_Open();

    //Sleep
#ifdef SLEEP_EN
    g_fSleepCalSeconds = 0;
    g_bSleepEnCnt =0.0f;
    //CC_SleepMonitor_Srv_SyncTimeSlot(&s_tVenusCB.stSleepAlgExecPeriod);
    // factory reset will erase sleep algorithm start and end time records,
    // So, call disable
    // after reset done, device will reinit sleep algorithm and save new start and end time records.
    CC_SleepMonitor_Srv_Disable();

    CC_SleepMonitor_InitSetTimePeriod(&s_tVenusCB.stSleepAlgExecPeriod);
    CC_SleepMonitor_Srv_StartService();
    CC_SleepMonitor_Srv_PollingCheck();
#endif

    //Liftarm
#ifdef LIFTARM_EN
    s_tVenusCB.cLiftarmEn = CC_BLE_Cmd_GetLiftArmStatus();
    if ( eEnable==s_tVenusCB.cLiftarmEn )
    {
        liftarm_open();
        s_tVenusCB.stGeneralInfo.bBandLocation = CC_BLE_Cmd_GetLiftArmBandSetting();
        liftarm_set_hangconfig(s_tVenusCB.stGeneralInfo.bBandLocation);
    }
    else
    {
        liftarm_close();     // default disable liftarm
    }

    if(0 == CC_BLE_Cmd_GetSwimmingEN())
    {
        CC_VENUS_AccelTimerStop();
        CC_VENUS_AccelTimerReset();
        CC_VENUS_AccelTimerFifoModeStart();
    }
#endif

    //Longsit
    CC_Longsit_Srv_LowPowerStateChange(s_tVenusCB.cLongsitEn, s_tVenusCB.bIsLowPower);
}
#endif



bool _app_scheduler(void)
{

    bool _bScheduleOperating = false;


    if (VENUS_EVENT_IS_ON(E_VENUS_EVENT_SENSOR_DATA_READY_ACCEL))
    {
          VENUS_EVENT_OFF(E_VENUS_EVENT_SENSOR_DATA_READY_ACCEL);

          if ( (eSwimCalProcStart == s_tVenusCB.eSwimCalEn) )
             //&&  (eDEVICE_CHARGE_IN == s_tVenusCB.eChargingState ))
          {
                //CC_SwimCal_Handle();
          }
          else
          {
#ifdef PEDO_EN
                _sensor_accel_gyro_on_change();
#endif
          }
          _bScheduleOperating =true;
    }


#ifdef HRM_EN
    if (VENUS_EVENT_IS_ON(E_VENUS_EVENT_HRM))
    {
        if (eHRM_On == s_tVenusCB.cHrmOnGoing)
            CC_HRM_PPG_PROC();

        _bScheduleOperating =true;

        VENUS_EVENT_OFF(E_VENUS_EVENT_HRM);
    }

    if (VENUS_EVENT_IS_ON(E_VENUS_EVENT_HRM_TIMEOUT) )
    {
        if ( eHRMCLOSE_ID_GENERALTIMEOUT == s_tVenusCB.m_HrmcloseID)
            CC_SetHrmClosePageEvent();

        VENUS_EVENT_OFF(E_VENUS_EVENT_HRM_TIMEOUT);
        _bScheduleOperating =true;
    }
#endif


    if (VENUS_EVENT_IS_ON(E_VENUS_EVENT_OLED_UPDATE_LIFTARM_UP) )
    {
        if (CC_PageMgr_IsBlockingOLED())
        {
            //NRF_LOG_INFO(" OLED IS BLOCKING \r\n");
        }
        else
        {
            if (CC_PageMgr_ActiveOLED())
            {
                CC_PageMgr_Proc(&s_tVenusCB.taEvent[E_VENUS_EVENT_OLED_UPDATE_LIFTARM_UP]);

                VENUS_EVENT_OFF(E_VENUS_EVENT_OLED_UPDATE_LIFTARM_UP);
                _bScheduleOperating =true;
            }
        }
    }

    if (VENUS_EVENT_IS_ON(E_VENUS_EVENT_OLED_UPDATE_LIFTARM_DOWN) )
    {
        CC_PageMgr_Proc(&s_tVenusCB.taEvent[E_VENUS_EVENT_OLED_UPDATE_LIFTARM_DOWN]);
        VENUS_EVENT_OFF(E_VENUS_EVENT_OLED_UPDATE_LIFTARM_DOWN);
        _bScheduleOperating =true;
    }


    if (VENUS_EVENT_IS_ON(E_VENUS_EVENT_OLED_UPDATE) )
    {
        if (CC_PageMgr_IsBlockingOLED())
        {
            //TracerInfo(" OLED IS BLOCKING \r\n");
        }
        else
        {
            if (CC_PageMgr_ActiveOLED())
            {
                CC_PageMgr_Proc(&s_tVenusCB.taEvent[E_VENUS_EVENT_OLED_UPDATE]);

                VENUS_EVENT_OFF(E_VENUS_EVENT_OLED_UPDATE);
                _bScheduleOperating =true;
            }
        }
    }

    if (VENUS_EVENT_IS_ON(E_VENUS_EVENT_OLED_UPDATE_ALARM) )
    {
        if (CC_PageMgr_IsBlockingOLED())
        {
            //TracerInfo(" OLED IS BLOCKING \r\n");
        }
        else
        {
            if (CC_PageMgr_ActiveOLED())
            {
                CC_PageMgr_Proc(&s_tVenusCB.taEvent[E_VENUS_EVENT_OLED_UPDATE_ALARM]);

                VENUS_EVENT_OFF(E_VENUS_EVENT_OLED_UPDATE_ALARM);
                _bScheduleOperating =true;
            }
        }
    }

    if (VENUS_EVENT_IS_ON(E_VENUS_EVENT_OLED_WAKEUP) )
    {
        CC_PageMgr_ExecWakeup();
        TracerInfo(" OLED IS E_VENUS_EVENT_OLED_WAKEUP \r\n");
        VENUS_EVENT_OFF(E_VENUS_EVENT_OLED_WAKEUP);

        _bScheduleOperating =true;

    }

    if (VENUS_EVENT_IS_ON(E_VENUS_EVENT_OLED_SLEEP) )
    {
        TracerInfo(" OLED IS E_VENUS_EVENT_OLED_SLEEP \r\n");
        CC_PageMgr_ExecSleep();
        VENUS_EVENT_OFF(E_VENUS_EVENT_OLED_SLEEP);

        _bScheduleOperating =true;
    }

    if (VENUS_EVENT_IS_ON(E_VENUS_EVENT_OLED_DSPSRV))
    {

        CC_Dsp_Srv_Reflash_Screen();
        VENUS_EVENT_OFF(E_VENUS_EVENT_OLED_DSPSRV);
        _bScheduleOperating =true;
    }

    if (VENUS_EVENT_IS_ON(E_VENUS_EVENT_PWM_VIBSRV))
    {
        TracerInfo(" OLED IS E_VENUS_EVENT_PWM_VIBSRV \r\n");
        CC_Vib_Srv_Handler();
        VENUS_EVENT_OFF(E_VENUS_EVENT_PWM_VIBSRV);
        _bScheduleOperating =true;
    }


    if (VENUS_EVENT_IS_ON(E_VENUS_EVENT_TOUCH_INT) )
    {
         TracerInfo("touch!\r\n");
         TOUCH_Handler();
         VENUS_EVENT_OFF(E_VENUS_EVENT_TOUCH_INT);
         s_tVenusCB.cTouchDebounceFlag=0;
         _bScheduleOperating =true;
    }


    if (VENUS_EVENT_IS_ON(E_VENUS_EVENT_BLE_NOTIFY_TO) )
    {
#ifdef CFG_BLE_APP
        ble_notify_handler();
#endif
        VENUS_EVENT_OFF(E_VENUS_EVENT_BLE_NOTIFY_TO);
        _bScheduleOperating =true;
    }



    if (VENUS_EVENT_IS_ON(E_VENUS_EVENT_DATETIMEFLUSH) )
    {

         s_tVenusCB.stSysCurTime = app_Time_Proc(1); //1 second
         VENUS_EVENT_OFF(E_VENUS_EVENT_DATETIMEFLUSH);

         #ifdef BATTERYLIFE_EN
         // BAttery Lift detect
         CC_BatteryLifeEnable();
         #endif

         #ifdef BAT_CHG_EN
         CC_ChargeStatePolling(); //polling
         #endif


         CC_BLE_Cmd_GetCallState(&s_tVenusCB.cNotify_flag,
                                                 &s_tVenusCB.cIncommingCallState,
                                                 &s_tVenusCB.cIncommingSMSState,
                                                 &s_tVenusCB.cLongsitEn,
                                                 &s_tVenusCB.cLiftarmEn);
         if (s_tVenusCB.cNotify_flag)
         {
             //check incommaingcall
             CC_Incommingcall_onNotified();

             CC_Incommingsms_onNotified();

             //init long sig
             CC_Longsit_Srv_LowPowerStateChange(s_tVenusCB.cLongsitEn, s_tVenusCB.bIsLowPower);

             if (eEnable ==s_tVenusCB.cLiftarmEn)
             {
                 if (!liftarm_is_open())
                 {
                     liftarm_open();
                     liftarm_set_hangconfig(s_tVenusCB.stGeneralInfo.bBandLocation);
                 }
             }

            if(0 == CC_BLE_Cmd_GetSwimmingEN())
            {
                CC_VENUS_AccelTimerStop();
                CC_VENUS_AccelTimerReset();
                CC_VENUS_AccelTimerFifoModeStart();
            }

         }


         #ifdef SLEEP_EN
         if (CC_BLE_Cmd_CheckSleepTimeSetting())
         {
             CC_BLE_Cmd_GetSleepTimeSetting(&s_tVenusCB.stSleepAlgExecPeriod,true);
             CC_SleepMonitor_Srv_SyncTimeSlot(&s_tVenusCB.stSleepAlgExecPeriod);
             TracerInfo("Updated Sleep Monitor Time Setting \r\n");
         }

         CC_SleepMonitor_Srv_PollingCheck();
         #endif

         if (CC_BLE_Cmd_CheckGeneralInfo())
         {
             CC_BLE_Cmd_GetGeneralInfo(&s_tVenusCB.stGeneralInfo,true);
             CC_CalorieInfoSetting();

             //set liftarm algo new band setting.  0: left hand, 1 right hand
             if (eEnable ==s_tVenusCB.cLiftarmEn)
             {
                 if (liftarm_is_open())
                     liftarm_set_hangconfig(s_tVenusCB.stGeneralInfo.bBandLocation);
             }

             TracerInfo("Updated GeneralInfo \r\n");
         }

         if (CC_BLE_Cmd_CheckUnitInfo())
         {
             CC_BLE_Cmd_GetUnitInfo(&s_tVenusCB.stUnitInfo,true);
             TracerInfo("Updated UnitInfo \r\n");
         }

         if (CC_BLE_Cmd_CheckClockAlarm())
         {
             CC_BLE_Cmd_GetClockAlarm(&s_tVenusCB.stClockAlarm,true);
             CC_ClockAlarm_Parse();
             #ifdef CLOCKALARM_EN
             //CC_ClockAlarm_Notified();
             #endif
             TracerInfo("Updated ClockAlarm \r\n");
         }
         else
         {
             _System_DayLine_Change();
         }

#ifdef DB_EN
         if((false == CC_DB_Check_Init_Done()) && (0x01 == app_Get_Update_Time_Flag()))
         {
             TracerInfo("DB_Init:%d\r\n",CC_DB_Get_Init_Type());

             if(DB_INIT_FROM_APP_FACTORY_RESET == CC_DB_Get_Init_Type())
             {
                 CC_SYS_FactroyReset_Setting();
                 CC_SYS_FactoryReset_Handler();
             }
             else
                 CC_DB_Init(CC_DB_Get_Init_Type());

             CC_DB_Set_Init_Done();
         }

         CC_DB_System_Check_Request_And_Save();

         CC_Pedo_Check_Save_Database();
#endif
         _bScheduleOperating =true;
    }


    if (VENUS_EVENT_IS_ON(E_VENUS_EVENT_HRM_SERVICE_24HR_START) )
    {
         TracerInfo("\r\n<E_VENUS_EVENT_HRM_SERVICE_24HR_START>\r");

         CC_AppSrv_HR_Start24HR();

         VENUS_EVENT_OFF(E_VENUS_EVENT_HRM_SERVICE_24HR_START);

         _bScheduleOperating = true;
    }

    if (VENUS_EVENT_IS_ON(E_VENUS_EVENT_HRM_SERVICE_24HR_TO_PERIODIC_MEASUREMENT) )
    {
         TracerInfo("\r\n<E_VENUS_EVENT_HRM_SERVICE_24HR_TO_PERIODIC_MEASUREMENT>\r");

         CC_AppSrv_24HR_Handler_ToPeriodicMeasurement();

         VENUS_EVENT_OFF(E_VENUS_EVENT_HRM_SERVICE_24HR_TO_PERIODIC_MEASUREMENT);

         _bScheduleOperating = true;
    }

    if (VENUS_EVENT_IS_ON(E_VENUS_EVENT_HRM_SERVICE_24HR_TO_ONE_MEASUREMENT) )
    {
         TracerInfo("\r\n<E_VENUS_EVENT_HRM_SERVICE_24HR_TO_ONE_MEASUREMENT>\r");

         CC_AppSrv_24HR_Handler_ToOneMeasurement();

         VENUS_EVENT_OFF(E_VENUS_EVENT_HRM_SERVICE_24HR_TO_ONE_MEASUREMENT);

         _bScheduleOperating = true;
    }

    if (VENUS_EVENT_IS_ON(E_VENUS_EVENT_HRM_SERVICE_24HR_STOP) )
    {
         TracerInfo("\r\n<E_VENUS_EVENT_HRM_SERVICE_24HR_STOP>\r");

         CC_AppSrv_HR_Stop24HR();

         VENUS_EVENT_OFF(E_VENUS_EVENT_HRM_SERVICE_24HR_STOP);

         _bScheduleOperating = true;
    }

    if (VENUS_EVENT_IS_ON(E_VENUS_EVENT_HRM_SERVICE_RESUME) )
    {
         TracerInfo("\r\n<E_VENUS_EVENT_HRM_SERVICE_RESUME>\r\n");

         CC_ResumeHrm();

         VENUS_EVENT_OFF(E_VENUS_EVENT_HRM_SERVICE_RESUME);

         _bScheduleOperating = true;
    }

    if (VENUS_EVENT_IS_ON(E_VENUS_EVENT_HRM_SERVICE_HR_LOCK) )
    {
         VENUS_EVENT_OFF(E_VENUS_EVENT_HRM_SERVICE_HR_LOCK);

         TracerInfo("\r\n<E_VENUS_EVENT_HRM_SERVICE_HR_LOCK>\r\n");

         CC_AppSrv_HR_SetAppLockHrm(CC_APPSRV_HR_APP_LOCK);

         _bScheduleOperating = true;
    }

    if (VENUS_EVENT_IS_ON(E_VENUS_EVENT_HRM_SERVICE_HR_UNLOCK) )
    {
         VENUS_EVENT_OFF(E_VENUS_EVENT_HRM_SERVICE_HR_UNLOCK);

         TracerInfo("\r\n<E_VENUS_EVENT_HRM_SERVICE_HR_UNLOCK>\r\n");

         CC_AppSrv_HR_SetAppLockHrm(CC_APPSRV_HR_APP_UNLOCK);

         _bScheduleOperating = true;
    }

    if (VENUS_EVENT_IS_ON(E_VENUS_EVENT_HRM_SERVICE_HR_ALERT_OUT_OF_RANGE) )
    {
        TracerInfo("\r\n<E_VENUS_EVENT_HRM_SERVICE_HR_ALERT_OUT_OF_RANGE>\r\n");

         if (CC_PageMgr_IsBlockingOLED())
        {
            //TracerInfo(" OLED IS BLOCKING \r\n");
        }
        else
        {
            if (CC_PageMgr_ActiveOLED())
            {
                CC_PageMgr_Proc(&s_tVenusCB.taEvent[E_VENUS_EVENT_HRM_SERVICE_HR_ALERT_OUT_OF_RANGE]);

                VENUS_EVENT_OFF(E_VENUS_EVENT_HRM_SERVICE_HR_ALERT_OUT_OF_RANGE);
                _bScheduleOperating =true;
            }
        }

        _bScheduleOperating = true;
    }

#ifdef FACTORY_RESET
    if (VENUS_EVENT_IS_ON(E_VENUS_EVENT_FACTORY_RESET_START))
    {
        TracerInfo("\r\n<E_VENUS_EVENT_FACTORY_RESET_START>\r\n");


        VENUS_EVENT_OFF(E_VENUS_EVENT_FACTORY_RESET_START);

        _bScheduleOperating = true;

        if (false == s_tVenusCB.bIsPreLowPower)
        {
            VENUS_EVENT_ON(E_VENUS_EVENT_OLED_UPDATE_FACTORYRESET_START, eEvent_FACTORY_RESET_START);
        }
        else
        {
            //if (s_tVenusCB.eChargingState == eDEVICE_CHARGE_IN )
            //{
            //    VENUS_EVENT_ON(E_VENUS_EVENT_OLED_UPDATE_FACTORYRESET_START, eEvent_FACTORY_RESET_START);
            //}
            //else
            {
                VENUS_EVENT_ON(E_VENUS_EVENT_OLED_UPDATE_FACTORYRESET_STOP, eEvent_FACTORY_RESET_STOP);
            }
        }

    }

    if (VENUS_EVENT_IS_ON(E_VENUS_EVENT_OLED_UPDATE_FACTORYRESET_START))
    {
        TracerInfo("\r\n<E_VENUS_EVENT_FACTORY_RESET_PROC>\r\n");

        if (CC_PageMgr_IsBlockingOLED())
        {
             //TracerInfo(" OLED IS BLOCKING \r\n");
        }
        else
        {
             if (CC_PageMgr_ActiveOLED())
             {
                 CC_PageMgr_Proc(&s_tVenusCB.taEvent[E_VENUS_EVENT_OLED_UPDATE_FACTORYRESET_START]);

                 _bScheduleOperating = true;

                 CC_SYS_FactroyReset_Setting();
                 CC_SYS_FactoryReset_Handler();
                 VENUS_EVENT_OFF(E_VENUS_EVENT_OLED_UPDATE_FACTORYRESET_START);
                 _bScheduleOperating =true;
                 VENUS_EVENT_ON(E_VENUS_EVENT_OLED_UPDATE_FACTORYRESET_DONE, eEvent_FACTORY_RESET_DONE);

             }
        }

    }


    if (VENUS_EVENT_IS_ON(E_VENUS_EVENT_OLED_UPDATE_FACTORYRESET_DONE))
    {
         TracerInfo("\r\n<E_VENUS_EVENT_OLED_UPDATE_FACTORYRESET_DONE>\r\n");

         if (CC_PageMgr_IsBlockingOLED())
         {
              //TracerInfo(" OLED IS BLOCKING \r\n");
         }
         else
         {
              if (CC_PageMgr_ActiveOLED())
              {
                  CC_PageMgr_Proc(&s_tVenusCB.taEvent[E_VENUS_EVENT_OLED_UPDATE_FACTORYRESET_DONE]);

                  _bScheduleOperating = true;

                  VENUS_EVENT_OFF(E_VENUS_EVENT_OLED_UPDATE_FACTORYRESET_DONE);
                  _bScheduleOperating =true;
              }
         }
            _bScheduleOperating =true;
    }

    if (VENUS_EVENT_IS_ON(E_VENUS_EVENT_OLED_UPDATE_FACTORYRESET_STOP))
    {
         TracerInfo("\r\n<E_VENUS_EVENT_OLED_UPDATE_FACTORYRESET_STOP>\r\n");

         if (CC_PageMgr_IsBlockingOLED())
         {
            //TracerInfo(" OLED IS BLOCKING \r\n");
         }
         else
         {
             if (CC_PageMgr_ActiveOLED())
             {
                 CC_PageMgr_Proc(&s_tVenusCB.taEvent[E_VENUS_EVENT_OLED_UPDATE_FACTORYRESET_STOP]);

                 _bScheduleOperating = true;

                 VENUS_EVENT_OFF(E_VENUS_EVENT_OLED_UPDATE_FACTORYRESET_STOP);
                 _bScheduleOperating =true;
             }
         }
         _bScheduleOperating =true;
    }
#endif

    #ifdef APP_VIB_MGR
        if (VENUS_EVENT_IS_ON(E_VENUS_EVENT_VIB_OPT))
        {
            //TracerInfo("\r\n<E_VENUS_EVENT_VIB_OPT>\r\n");

            APP_VibMgr_RegEventHandler(&s_tVenusCB.taEvent[E_VENUS_EVENT_VIB_OPT]);
            VENUS_EVENT_OFF(E_VENUS_EVENT_VIB_OPT);
            _bScheduleOperating = true;
        }

       if (VENUS_EVENT_IS_ON(E_VENUS_EVENT_VIB_EXECTIME))
        {
            //TracerInfo("\r\n<E_VENUS_EVENT_VIB_EXECTIME>\r\n");
            APP_VibMgr_RegEventHandler(&s_tVenusCB.taEvent[E_VENUS_EVENT_VIB_EXECTIME]);

            VENUS_EVENT_OFF(E_VENUS_EVENT_VIB_EXECTIME);

            _bScheduleOperating = true;
        }

        if (VENUS_EVENT_IS_ON(E_VENUS_EVENT_VIB_DUTYCYCLECHANGE))
        {
            //TracerInfo("\r\n<E_VENUS_EVENT_VIB_DUTYCYCLECHANGE>\r\n");
            APP_VibMgr_RegEventHandler(&s_tVenusCB.taEvent[E_VENUS_EVENT_VIB_DUTYCYCLECHANGE]);

            VENUS_EVENT_OFF(E_VENUS_EVENT_VIB_DUTYCYCLECHANGE);

            _bScheduleOperating = true;
        }

    #endif



    return _bScheduleOperating;
}


/*
void CC_BLE_Cmd_GetSleepTimeSetting(db_sys_sleep_monitor_t *pData, uint8_t _bOption)
{
    memcpy(pData,&s_tVensuSyncData._stSleepMonitorTimeSetting,sizeof(db_sys_sleep_monitor_t));
    // the option, true = get form one second polling, false get form DB,
    // avoid DB get data before one second polling.
    if (_bOption == true)
        s_tVensuSyncData._cNotify_SleepMonitorTimeSetting_Flag = false;
    TracerInfo("CC_BLE_Cmd_GetSleepTimeSetting   [%d]   \r\n",s_tVensuSyncData._cNotify_SleepMonitorTimeSetting_Flag);

}
*/

void venus_algorithm_init()
{
    FP_PED_8Bit_initialize();
    //Init Swim algorithm and init MMI data
    CC_BLE_Cmd_GetGeneralInfo(&s_tVenusCB.stGeneralInfo,true);
    CC_Swim_Init(s_tVenusCB.stGeneralInfo.cSwim_Pool_Size,(band_location)s_tVenusCB.stGeneralInfo.bBandLocation);  // remarked for temp by Samuel
    //init_gyro_cali_when_on(s_tVenusCB.wSwimCalData.Data);

    // Calorie open
    CC_CalorieBurn_Open();

    //#ifdef LIFTARM_EN
    s_tVenusCB.cLiftarmEn = CC_BLE_Cmd_GetLiftArmStatus();
    if ( eEnable==s_tVenusCB.cLiftarmEn )
    {
        liftarm_open();
        s_tVenusCB.stGeneralInfo.bBandLocation = CC_BLE_Cmd_GetLiftArmBandSetting();
        liftarm_set_hangconfig(s_tVenusCB.stGeneralInfo.bBandLocation);
    }
    else
    {
        liftarm_close();     // default disable liftarm
    }
    //#endif

    #ifdef LONGSIT_EN
        s_tVenusCB.cLongsitEn = CC_BLE_Cmd_GetLongSitStatus();
        if ( eEnable==s_tVenusCB.cLongsitEn)
            CC_Longsit_Srv_Init();
        else
            CC_Longsit_Srv_Deinit();     // default disable Longsit
    #endif

    #ifdef SLEEP_EN
    CC_SleepMonitor_Srv_Register();
    CC_BLE_Cmd_GetSleepTimeSetting(&s_tVenusCB.stSleepAlgExecPeriod,true);
    CC_SleepMonitor_InitSetTimePeriod(&s_tVenusCB.stSleepAlgExecPeriod);
    #endif
}


void venus_platform_init(void)
{
    uint32_t ret;

    drvi_RequestIrq(TOUCH_INT_PIN, TOUCH_int_handler, IRQ_TYPE_EDGE_FALLING);
    drvi_EnableIrq(TOUCH_INT_PIN);
#ifdef HRM_EN
    drvi_RequestIrq(HRM_INT_PIN, HRM_int_handler, IRQ_TYPE_EDGE_FALLING);
#endif
    ACC_init();

#if 0 // test acc/gyro
    if(1)
    {
        CC_LSM6DSX_AccelPowerDown();
        CC_LSM6DSX_GyroPowerDown();

        CC_LSM6DSX_AccelPowerON(LSM6DS3_ACC_GYRO_ODR_XL_52Hz);
        CC_LSM6DSX_GyroPowerON (LSM6DS3_ACC_GYRO_ODR_G_52Hz );

        while(1)
        {
            LSM6DS3_ACC_GYRO_GetRawAccData(NULL, (i16_t *) _wAccelData);
            LSM6DS3_ACC_GYRO_GetRawGyroData16(NULL, (i16_t *) _wGyroData);

            TracerInfo("acc x: %d, y: %d, z: %d\r\n", _wAccelData[0], _wAccelData[1], _wAccelData[2]);
            TracerInfo("gyro x: %d, y: %d, z: %d\r\n", _wGyroData[0], _wGyroData[1], _wGyroData[2]);

             cc6801_ClockDelayMs(100);
        }


    }
#endif

    //GYR_Init(); // Not nessary if ACC_init() was done.
    ret = MAG_Init();

    if(MEMS_SUCCESS != ret)
    {
        TracerInfo("mag_init error: 0x%x\r\n", ret);

         cc6801_ClockDelayMs(1000);
    }
#if 0 // test mag
    else
    {
        CC_AK09912_MAG_SET_ODR(AK09912_MAG_DO_10_Hz);


        while(1)
        {
            AKM_Data_Get();

            TracerInfo("mag x: %d, y: %d, z: %d\r\n", s_tMagRaw.AXIS_X, s_tMagRaw.AXIS_Y, s_tMagRaw.AXIS_Z);

        }
    }
#endif

    CC_GYRO_Set_ODR(LSM6DS3_ACC_GYRO_ODR_G_POWER_DOWN);

    app_displayoled_init();

    app_displayoled_start();

#ifdef DB_EN
    ret = CC_Fds_init();

    if(CC_SUCCESS != ret)
        TracerInfo("fds_init error: 0x%x\r\n", ret);
#endif
}

void venus_app_init(void)
{

    app_multiple_timer_init();

    app_Time_Init();

    APP_SCHED_Init();
#ifdef APP_SERV_MGR_EN
    APP_SVCMGR_Init();
#endif
    s_tVenusCB.stSysCurTime = app_getSysTime();
#ifdef HRM_EN
    CC_AppSrv_HR_Init();
#endif

    CC_PageMgr_Init();

    CC_VENUS_OLEDGeneralOutTimerStart(5000);

}

void venus_ready_to_bootloader(void)
{
    //TracerInfo("venus_ready_to_bootloader!\r\n");
#ifdef CFG_BLE_APP
    appm_disconnect();
#endif
}




/*
 * VENUS MAIN FUNCTION
 ****************************************************************************************
 */

int venus_main(void)
{

    TracerInfo("== Venus Main Start==\r\n");

#ifdef CFG_BLE_APP
    uint32_t error = 0;

    GLOBAL_INT_STOP();
#ifndef CFG_JUMP_TABLE_2
    memset(((void *) JUMP_TABLE_2_BASE_ADDR), 0, 1024);
#endif
    memset (((void *) 0x40006000), 0, 8192);
    memset (((void *) 0x20000048), 0, 0x820);
    *((uint32_t *) 0x4000011C) = 0x00000008;
    *((uint32_t *) 0x40000104) = (*((uint32_t *) 0x40000104) & 0xFFFFFE0) | 0x04;
    //*((uint32_t *) 0x20000648) = 0x00;
     //regCKGEN->bf.bleClkDiv = 0x04;

    // Initialize RW SW stack
    rwip_init(error);

    GLOBAL_INT_START();

#ifdef SW_TIMER_BY_KERNEL
    while(APPM_ADVERTISING != ke_state_get(TASK_APP))
    {
        rwip_schedule();
    }
#endif
#endif


#ifdef SM_TEST_EN
    SM_Test();
#endif


    venus_platform_init();

#ifdef DB_EN
    CC_DB_Init(DB_INIT_FROM_SYSTEM);
#endif

    venus_app_init();

    application_timers_start();


    s_tVenusCB.bAvg_BatLevel = 100;

    s_tVenusCB.eSystemPwrState = eSysStateInit;

    TracerInfo("> Venus While\r\n");
#if 0
    app_ccps_test_code();
#endif
    while(1)
    {
        switch (s_tVenusCB.eSystemPwrState)
        {
            case eSysStateInit:
                venus_algorithm_init();
        #ifdef SWIM_CAL_EN
                //if(false == s_tVenusCB.wSwimCalData.wVaildFlag)
                  //  CC_Charge_Register(_SwimCal_Evt_Handler);
        #endif

                //CC_ChargePPR_PinStatInit();

#ifdef FORCE_HRS_TEST_EN
                CC_HRM_PostHeartRateStrapModeEvent(1);
#endif
                s_tVenusCB.eSystemPwrState = eSysStateNormal;
                break;

            case eSysStateNormal:
#if 0 // test app loop time
                //static uint32_t loop_cnt = 0;

                static uint32_t old_tick = 0;
                static uint32_t diff_time = 0;
                static uint32_t max_diff_time = 0;
                static uint8_t ignore = 0;

                old_tick = Get_system_time_ms();
#endif


                _app_scheduler();
                APP_SCHED_RunScheduler();
#if 0 // test app loop time
                diff_time = Get_system_time_ms() - old_tick;

                if(ignore > 100)
                {
                     if(max_diff_time < diff_time)
                          max_diff_time = diff_time;
                }
                else
                     ignore++;

                TracerInfo("%d / %d\r\n", diff_time, max_diff_time);
#endif

                #ifdef CFG_BLE_APP
                    rwip_schedule();
                    rwip_ignore_ll_conn_param_update_patch();
                #endif

        }


        __WFE();

    }
}



