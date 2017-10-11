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
#include "gyr_lsm6ds3.h"
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



#define APP_TIMER_PRESCALER        0
#define APP_TIMER_OP_QUEUE_SIZE    4
#define DATETIME_CNT                  1000

#define SPEED_AND_CADENCE_MEAS_INTERVAL     1000    

#define DB_PEDO_WALK_TIMEOUT_INTERVAL		360000// 6min //180000	// 3 min
#define DB_PEDO_RUN_TIMEOUT_INTERVAL		360000// 6min //180000	// 3 min

#define LOCK_SWIM_OFF_TIMEOUT_INTERVAL		180000 // 3min


#define ACCEL_FIFO_MODE_LONG_INTERVAL		1000		// in ms
#define ACCEL_FIFO_MODE_SHORT_INTERVAL		300		// in ms
#define ACCEL_POLL_MODE_INTERVAL		20		// in ms
#define MAG_BUF_LEN                     10

#define BATTERY_LEVEL_MEAS_INTERVAL     APP_TIMER_TICKS(60000, APP_TIMER_PRESCALER)


#define VENUS_EVENT_ON(EventID,Data)                             \
        {                                                              \
            s_tVenusCB.taEvent[EventID].bCount = 1;                   \
            s_tVenusCB.taEvent[EventID].dwData= Data;          \
        }

#define VENUS_EVENT_OFF(EventID)             {    s_tVenusCB.taEvent[EventID].bCount = 0;    }
#define VENUS_EVENT_IS_ON(EventID)           (    s_tVenusCB.taEvent[EventID].bCount         )
// VENUS CONTROL BLOCK & DEFINITION =========================================


// App timer definition:
// STANDBY TIMER ===============================
APP_TIMER_DEF(s_tVenusTimerAccel);     // disable this timer to unify standby timers
APP_TIMER_DEF(s_tVenusTimerDataTime);    // TBD: should rename like standby-base-timer

// FUNCTION TIMER ===============================
APP_TIMER_DEF(s_tTouchDebounceTime);
APP_TIMER_DEF(s_tLongTouchTriggerTime);
APP_TIMER_DEF(s_tChargePPRDebounceTime);
APP_TIMER_DEF(s_tChargeCHGDebounceTime);
APP_TIMER_DEF(s_tVenusTimerDisplayGeneral);
APP_TIMER_DEF(m_battery_timer_id);                         /**< Battery timer. */
APP_TIMER_DEF(m_rsc_meas_timer_id);                        /**< RSC measurement timer. */
APP_TIMER_DEF(s_tVenusTimerOLEDWakeup);
APP_TIMER_DEF(s_tVenusTimerOLEDSleep);
APP_TIMER_DEF(s_tVenusTimerOLEDDisplaySrvTimer);
APP_TIMER_DEF(s_tVenusTimerPWMVibSrvTimer);
//APP_TIMER_DEF(s_tVenusTimerPedoWalkTimer);
//APP_TIMER_DEF(s_tVenusTimerPedoRunTimer);
//APP_TIMER_DEF(s_tVenusTimerBatteryLiftImmediatelyCheck);
//APP_TIMER_DEF(s_tVenusTimerSwimCalTimer);
APP_TIMER_DEF(s_tLockSwimOffTimer);

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

}   S_VenusCB;


volatile uint8_t  *p_llm_le_event_mask = (volatile uint8_t *) 0x20000680;

char deviceName[10] = {'V','N','S','_'};
S_VenusCB    s_tVenusCB;
uint8_t g_GyroEnable = 1;
static short _wGyroData[3]  = { 0 };
static short _wAccelData[3]  = { 0 };
float g_fSleepCalSeconds =0;
uint8_t g_bSleepEnCnt = 0;
s_SensorData_t s_tAcc;
s_SensorData_t s_tGyro;
S_TMagBuf_t s_tMagDataBuf;  
AxesRaw_t s_tMagRaw;
static uint8_t g_bMagEnCnt = 0;

extern void _sensor_algorithm_sleepmeter_proc(void);
extern void CC_Dsp_Srv_Reflash_Screen(void);





void CC_VENUS_OLEDWakUpTimeOutTimerStart(uint16_t _wdata)
{
    app_timer_start(s_tVenusTimerOLEDWakeup, APP_TIMER_TICKS(_wdata,APP_TIMER_PRESCALER), NULL);
}

void CC_VENUS_OLEDWakUpTimeOutTimerStop(void)
{
    app_timer_stop(s_tVenusTimerOLEDWakeup);
}

void CC_VENUS_OLEDSleepTimeOutTimerStart(uint16_t _wdata)
{
    app_timer_start(s_tVenusTimerOLEDSleep, APP_TIMER_TICKS(_wdata,APP_TIMER_PRESCALER), NULL);

}

void CC_VENUS_OLEDSleepTimeOutTimerStop(void)
{
    app_timer_stop(s_tVenusTimerOLEDSleep);
}

void CC_VENUS_OLEDGeneralOutTimerStart(uint16_t _wdata)
{
    app_timer_start(s_tVenusTimerDisplayGeneral, APP_TIMER_TICKS(_wdata,APP_TIMER_PRESCALER), NULL);
}

void CC_VENUS_OLEDGeneralOutTimerReset(void)
{
    app_timer_start(s_tVenusTimerDisplayGeneral, APP_TIMER_TICKS(0,APP_TIMER_PRESCALER), NULL);
}

void CC_VENUS_OLEDGeneralOutTimerStop(void)
{
    app_timer_stop(s_tVenusTimerDisplayGeneral);
}

void CC_VENUS_OLEDDisplayServiceTimerStart(uint16_t _wdata)
{
    app_timer_start(s_tVenusTimerOLEDDisplaySrvTimer, APP_TIMER_TICKS(_wdata,APP_TIMER_PRESCALER), NULL);
}

void CC_VENUS_OLEDDisplayServiceTimerReset(void)
{
    app_timer_start(s_tVenusTimerOLEDDisplaySrvTimer, APP_TIMER_TICKS(0,APP_TIMER_PRESCALER), NULL);
}

void CC_VENUS_OLEDDisplayServiceTimerStop(void)
{
    app_timer_stop(s_tVenusTimerOLEDDisplaySrvTimer);
}

void CC_VENUS_OLEDDisplayServiceEventClear(void)
{
    VENUS_EVENT_OFF(E_VENUS_EVENT_OLED_DSPSRV);        
}

void CC_VENUS_TOUCHDebounceTimerStart(uint16_t _wdata)
{
    app_timer_start(s_tTouchDebounceTime, APP_TIMER_TICKS(_wdata,APP_TIMER_PRESCALER), NULL);
}


void CC_VENUS_LongTOUCHTriggerTimerStart(uint16_t _wdata)
{
    app_timer_start(s_tLongTouchTriggerTime, APP_TIMER_TICKS(_wdata,APP_TIMER_PRESCALER), NULL);
}


void CC_VENUS_CHARGE_PPR_DebounceTimerStart(uint16_t _wData)
{
    app_timer_start(s_tChargePPRDebounceTime, APP_TIMER_TICKS(_wData,APP_TIMER_PRESCALER), NULL);
}

void CC_VENUS_CHARGE_CHG_DebounceTimerStart(uint16_t _wData)
{
    app_timer_start(s_tChargeCHGDebounceTime, APP_TIMER_TICKS(_wData,APP_TIMER_PRESCALER), NULL);
}



void CC_VENUS_PWMVibrationServiceTimerStart(uint16_t _wdata)
{
    app_timer_start(s_tVenusTimerPWMVibSrvTimer, APP_TIMER_TICKS(_wdata,APP_TIMER_PRESCALER), NULL);
}

void CC_VENUS_PWMVibrationServiceTimerReset(void)
{
    app_timer_start(s_tVenusTimerPWMVibSrvTimer, APP_TIMER_TICKS(0,APP_TIMER_PRESCALER), NULL);
}

void CC_VENUS_PWMVibrationServiceTimerStop(void)
{
    app_timer_stop(s_tVenusTimerPWMVibSrvTimer);
}

void CC_VENUS_PWMVibrationServiceEventClear(void)
{
    VENUS_EVENT_OFF(E_VENUS_EVENT_PWM_VIBSRV);        
}


void CC_VENUS_RscTimerStart(uint32_t interval_ms)
{
	 app_timer_start(m_rsc_meas_timer_id, APP_TIMER_TICKS(interval_ms, APP_TIMER_PRESCALER), NULL);
}

void CC_VENUS_RscTimerStop(void)
{
	app_timer_stop(m_rsc_meas_timer_id);
}



void CC_VENUS_Lock_SwimOff_TimerStop(void)
{
    s_tVenusCB.bIsLockSwimOff = false;
    app_timer_stop(s_tLockSwimOffTimer);
}

void CC_VENUS_Lock_SwimOff_TimerStart(void)
{    
    s_tVenusCB.bIsLockSwimOff = true;
    app_timer_start(s_tLockSwimOffTimer, APP_TIMER_TICKS(LOCK_SWIM_OFF_TIMEOUT_INTERVAL,APP_TIMER_PRESCALER), NULL);
}

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

void CC_MainGet_CurrentTime(app_date_time_t *_stCurTime)
{
    *_stCurTime = s_tVenusCB.stSysCurTime; 
}


void CC_MainGet_Stride_LengthSetting(uint8_t *_pbStride, uint8_t *_pbUnit)
{
    *_pbStride = s_tVenusCB.stGeneralInfo.cStride_Lenght;
    *_pbUnit = s_tVenusCB.stUnitInfo.cUnitLength;
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
			_TouchDebounce_Handler(50);
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
    UNUSED_PARAMETER(p_context);

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
    UNUSED_PARAMETER(p_context);
    VENUS_EVENT_ON(E_VENUS_EVENT_OLED_UPDATE , eEvent_None);
        
}

static void cc_toolbox_OLED_Wakup_TimeOut(void * p_context)
{
    UNUSED_PARAMETER(p_context);
//    TracerInfo(" cc_toolbox_OLED_Wakup_TimeOut \r\n");
    VENUS_EVENT_ON(E_VENUS_EVENT_OLED_WAKEUP , eEvent_None);
}

static void cc_toolbox_OLED_Sleep_TimeOut(void * p_context)
{
    UNUSED_PARAMETER(p_context);
//    TracerInfo(" cc_toolbox_OLED_Sleep_TimeOut \r\n");
    VENUS_EVENT_ON(E_VENUS_EVENT_OLED_SLEEP , eEvent_None);
}

static void cc_toolbox_OLED_Display_Service_timeout(void * p_context)
{
    UNUSED_PARAMETER(p_context);
    VENUS_EVENT_ON(E_VENUS_EVENT_OLED_DSPSRV, eEvent_None);
    //TracerInfo(" cc_toolbox_OLED_Display_Service_timeout \r\n");
}

static void cc_toolBox_PWM_Vib_Service_timeout(void * p_context)
{
    UNUSED_PARAMETER(p_context);
    //TracerInfo(" cc_toolBox_PWM_Vib_Service_timeout \r\n");
    VENUS_EVENT_ON(E_VENUS_EVENT_PWM_VIBSRV, eEvent_None);
}


void CC_VENUS_AccelTimerFifoModeStart(void)
{
    //if(0x01 == CC_BLE_Cmd_GetLiftArmStatus())
    if(1)   // force to enable lift arm
        app_timer_start(s_tVenusTimerAccel, APP_TIMER_TICKS(ACCEL_FIFO_MODE_SHORT_INTERVAL, APP_TIMER_PRESCALER), NULL);
    else
    	app_timer_start(s_tVenusTimerAccel, APP_TIMER_TICKS(ACCEL_FIFO_MODE_LONG_INTERVAL, APP_TIMER_PRESCALER), NULL);
}

void CC_VENUS_AccelTimerPollModeStart(void)
{
	app_timer_start(s_tVenusTimerAccel, APP_TIMER_TICKS(ACCEL_POLL_MODE_INTERVAL, APP_TIMER_PRESCALER), NULL);
}

void CC_VENUS_AccelTimerReset(void)
{
	app_timer_start(s_tVenusTimerAccel, APP_TIMER_TICKS(0,APP_TIMER_PRESCALER), NULL);
}

void CC_VENUS_AccelTimerStop(void)
{
	app_timer_stop(s_tVenusTimerAccel);
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
    UNUSED_PARAMETER(p_context);

    VENUS_EVENT_ON(E_VENUS_EVENT_SENSOR_DATA_READY_ACCEL , eEvent_None);

#ifdef PEDandSWIMMING
    VENUS_EVENT_ON(E_VENUS_EVENT_SENSOR_DATA_READY_GYRO , eEvent_None);

#endif
}

static void cc_toolbox_Datatime_hanndler(void * p_context)
{   
    cc_toolbox_sensor_accel_on_change(p_context);

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
    UNUSED_PARAMETER(p_context);
    battery_level_update(cc_get_battery_level());
}


/*
static uint8_t CC_Ble_CommandParse(const uint8_t index, ble_rscs_meas_t * p_rsc_measurement)
{
    p_rsc_measurement->eHeader = 0xCC;

    if( 0x00 ==s_tVenusCB.cSwimmingEn )
    {
            _sPedInfo.is_update_steps =  1;
            _sPedInfo.dwTotal_steps = s_tVenusCB.dwPedTotalStepCount;
            _sPedInfo.dwTotal_calorie = s_tVenusCB.dwTotalCalorie;
    
            _sHrmInfo.is_update_hrm = s_tVenusCB.chrmDataReady_Flag;
            _sHrmInfo.hrmdata       = s_tVenusCB.cHrmData;
            CC_MainSet_HrmDataFlag(false);
    
            p_rsc_measurement->eArray[_dwlen++] = _sPedInfo.command;
            p_rsc_measurement->eArray[_dwlen++] = _sPedInfo.is_update_steps;
            _dwlen += uint32_encode(_sPedInfo.dwTotal_steps,&p_rsc_measurement->eArray[_dwlen]);
            
        
            p_rsc_measurement->eArray[_dwlen++] = _sHrmInfo.command;
            p_rsc_measurement->eArray[_dwlen++] = _sHrmInfo.is_update_hrm;
            _dwlen += uint16_encode(_sHrmInfo.hrmdata,&p_rsc_measurement->eArray[_dwlen]);
    
            //total calorie
            _dwlen += uint32_encode(_sPedInfo.dwTotal_calorie,&p_rsc_measurement->eArray[_dwlen]);
    }
    else
    {
        _sSwimInfo.is_Swim_En = s_tVenusCB.cSwimmingEn;
        _sSwimInfo.style_type= s_tVenusCB.stSwimmingResult.swimstyle;
        _sSwimInfo.dwSwimCnt = s_tVenusCB.stSwimmingResult.swimcount;
        _sSwimInfo.cSwimLap= s_tVenusCB.dwSwimLapCnt;
        _sSwimInfo.dwTimestamp= s_tVenusCB.stSwimmingResult.timestamp;
    
        p_rsc_measurement->eArray[_dwlen++] = _sSwimInfo.command;
        p_rsc_measurement->eArray[_dwlen++] = _sSwimInfo.is_Swim_En;
        p_rsc_measurement->eArray[_dwlen++] = _sSwimInfo.style_type;
        _dwlen += uint32_encode(_sSwimInfo.dwSwimCnt,&p_rsc_measurement->eArray[_dwlen]);
        _dwlen += uint32_encode(_sSwimInfo.cSwimLap,&p_rsc_measurement->eArray[_dwlen]);
        _dwlen += uint32_encode(_sSwimInfo.dwTimestamp,&p_rsc_measurement->eArray[_dwlen]);
    }

}
*/

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

            if(err_code != NRF_SUCCESS)
                m_rscs.re_send = 1;
            else
                m_rscs.re_send = 0;
        }
    }
    else
    {
        err_code = ble_rscs_measurement_send(&m_rscs, &m_rscs.rscs_measurement);

        if(err_code != NRF_SUCCESS)
            m_rscs.re_send = 1;
        else
            m_rscs.re_send = 0;
    }
*/    
}

static void rsc_meas_timeout_handler(void * p_context)
{
    UNUSED_PARAMETER(p_context);
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

    // Initialize timer module.
    APP_TIMER_INIT(APP_TIMER_PRESCALER, APP_TIMER_OP_QUEUE_SIZE, false);


     app_timer_create(&m_battery_timer_id,
                      APP_TIMER_MODE_REPEATED,
                      battery_level_meas_timeout_handler);

    app_timer_create(&m_rsc_meas_timer_id,
                     APP_TIMER_MODE_REPEATED,
                     rsc_meas_timeout_handler);

    app_timer_create(&s_tVenusTimerAccel,
                     APP_TIMER_MODE_REPEATED,
                     cc_toolbox_sensor_accel_on_change);

    app_timer_create(&s_tTouchDebounceTime,
                     APP_TIMER_MODE_SINGLE_SHOT,
                     cc_touch_debounce_timeout);    

    app_timer_create(&s_tVenusTimerDisplayGeneral,
                     APP_TIMER_MODE_SINGLE_SHOT,
                     cc_toolbox_Dispaly_General_handler);

    app_timer_create(&s_tVenusTimerOLEDWakeup,
                     APP_TIMER_MODE_SINGLE_SHOT,
                     cc_toolbox_OLED_Wakup_TimeOut);

    app_timer_create(&s_tVenusTimerOLEDSleep,
                     APP_TIMER_MODE_SINGLE_SHOT,
                     cc_toolbox_OLED_Sleep_TimeOut);

    app_timer_create(&s_tVenusTimerOLEDDisplaySrvTimer,
                     APP_TIMER_MODE_SINGLE_SHOT,
                     cc_toolbox_OLED_Display_Service_timeout);

    app_timer_create(&s_tVenusTimerDataTime,
                     APP_TIMER_MODE_REPEATED,
                     cc_toolbox_Datatime_hanndler);

    app_timer_create(&s_tVenusTimerPWMVibSrvTimer,
                     APP_TIMER_MODE_SINGLE_SHOT,
                     cc_toolBox_PWM_Vib_Service_timeout);       
     

}

static void application_timers_start(void)
{
    //uint32_t err_code;


    app_timer_start(m_rsc_meas_timer_id, APP_TIMER_TICKS(SPEED_AND_CADENCE_MEAS_INTERVAL, APP_TIMER_PRESCALER), NULL);

    // Start application timers.
    app_timer_start(m_battery_timer_id, BATTERY_LEVEL_MEAS_INTERVAL, NULL); 

    //err_code = app_timer_start(m_rsc_meas_timer_id, APP_TIMER_TICKS(SPEED_AND_CADENCE_MEAS_INTERVAL, APP_TIMER_PRESCALER), NULL);
    //APP_ERROR_CHECK(err_code);

    // disable this timer to unify standby timers
	  app_timer_start(s_tVenusTimerAccel, APP_TIMER_TICKS(ACCEL_FIFO_MODE_LONG_INTERVAL, APP_TIMER_PRESCALER), NULL);

    app_timer_start(s_tVenusTimerDataTime, APP_TIMER_TICKS(DATETIME_CNT,APP_TIMER_PRESCALER), NULL);

                          
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

#ifdef PEDO_EN
static void _sensor_algorithm_liftarm_proc()
{
    //TracerInfo("_sensor_algorithm_liftarm_proc \r\n");
    //if (eEnable== s_tVenusCB.cLiftarmEn) // remarked by Samuel
    if(1) 
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
            

            //NRF_LOG_INFO( "oringin  x=[%d] y=[%d] z=[%d]\r\n",_wAccelData[0],_wAccelData[1],_wAccelData[2]);
            //NRF_LOG_INFO( "change x=[%d] y=[%d] z=[%d]\r\n",input[0],input[1],input[2]);
            liftarm_process( input,&_GestureOut, &_WinStatus);
            if (1 == _WinStatus) 
            {
                if (1 == _GestureOut || 2 == _GestureOut)       /**< 1: up, 2: down */
                {
                    if (1 == _GestureOut)
                    {
                        if (!CC_PageMgr_IsOLEDActive() && !CC_PageMgr_IsBlockingOLED())
                        {
                            CC_PageMgr_ActiveOLED();
                            VENUS_EVENT_ON(E_VENUS_EVENT_OLED_UPDATE, eEvent_LIFTARM);
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

    //NRF_LOG_INFO("_sensor_algorithm_swimming_proc \r\n");
    
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
                //NRF_LOG_INFO("reset Mag Count = %d\r\n",_cMagEnCnt);

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
        _CC_DB_Save_SwimmingProc();
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
            CC_LSM6DSX_FifoEnable(E_LSM6DSX_FIFO_CONTROL_ACCEL);
            CC_Mems_Fifo_Register(MEMS_FIFO_USER_PEDO, s_tAcc.wbuf, (FIFO_DEPTH_T/2));
            _sensor_power_down_gryo();
            _sensor_power_down_mag();
            g_GyroEnable = 0;
            //_CC_DB_Save_SwimmingEnd();
            //_DumpSensorRegisterMap(); //test!!!
            CC_VENUS_AccelTimerFifoModeStart();

        }
        else
        {

                uint16_t fifo_len;

                CC_Mems_Fifo_Update_Data();

                fifo_len = CC_Mems_Fifo_Get_UnRead_Length(MEMS_FIFO_USER_PEDO);		


                //TracerInfo("fifo_len: %d\r\n", fifo_len);

                memcpy(s_tAcc.wdata, s_tAcc.wbuf, (fifo_len*2));

                CC_Mems_Fifo_Reset(MEMS_FIFO_USER_PEDO);

                // Charge-in or low power, skip to execute pedometer, liftarm and sleep algorithm
               // if ((eDEVICE_CHARGE_IN== s_tVenusCB.eChargingState) || 
                 //   (true == s_tVenusCB.bIsLowPower))
                   // return;


                fifo_len /= 3;

                for(int i = 0; i < fifo_len; i++)
                {
                    int k = 3*i;

                   _wAccelData[0] = (s_tAcc.wdata[k] / 4); // x->y 
                   _wAccelData[1] = (s_tAcc.wdata[k+1] / 4); //y ->-x 
                   _wAccelData[2] = (s_tAcc.wdata[k+2] / 4);
                 	_sensor_algorithm_liftarm_proc();    
            
                    uint32_t       _dwPedTotalStepCount = 0;
                    signed char    _cPedState = ePedo_Stop;                     

                    FP_PED_8Bit(((const short *) _wAccelData), &_dwPedTotalStepCount, &_cPedState);   
                    //TracerInfo("%d,%d,%d\r\n", _wAccelData[0], _wAccelData[1], _wAccelData[2]);                    
                    if (s_tVenusCB.dwPedTotalStepCount != _dwPedTotalStepCount)
                    {     
                        //TracerInfo("ped_cnt: %d, ped_st: %d\r\n", _dwPedTotalStepCount, _cPedState);

#if 0 // for DB
                        
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

                    } 

#if 0 // for DB
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

                CC_LSM6DSX_FifoDisable(E_LSM6DSX_FIFO_CONTROL_ACCEL_GYRO);

                CC_LSM6DSX_AccelPowerDown();
                CC_LSM6DSX_GyroPowerDown();

                CC_LSM6DSX_AccelPowerON(LSM6DS3_ACC_GYRO_ODR_XL_52Hz);
                CC_LSM6DSX_GyroPowerON (LSM6DS3_ACC_GYRO_ODR_G_52Hz );

                CC_AK09912_MAG_SET_ODR(AK09912_MAG_DO_10_Hz); 

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

                CC_Mems_Fifo_UnRegister(MEMS_FIFO_USER_PEDO);

                CC_VENUS_AccelTimerPollModeStart();
                g_bMagEnCnt = 0; 
        }
        else
        {
            _sensor_algorithm_swimming_proc();
        }    
        
    }
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
        ble_notify_handler();
    
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


         }   

/*
         if (CC_BLE_Cmd_CheckSleepTimeSetting())
         {
             CC_BLE_Cmd_GetSleepTimeSetting(&s_tVenusCB.stSleepAlgExecPeriod,true);            
             CC_SleepMonitor_Srv_SyncTimeSlot(&s_tVenusCB.stSleepAlgExecPeriod);
             TracerInfo("Updated Sleep Monitor Time Setting \r\n");
         }
*/            
         #ifdef SLEEP_EN
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



#ifdef DB_FIRST_USE_EN
         if(app_Check_Time_And_DB_Init() == 0x01)
         {
             CC_DB_Init(DB_INIT_FROM_APP);
             app_Set_DB_Init_Done();
         }
#endif

//         CC_DB_System_Check_Request_And_Save();

//         CC_Pedo_Check_Save_Database();            
            
         _bScheduleOperating =true;
    }



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
    s_tVenusCB.cLiftarmEn = eEnable; //CC_BLE_Cmd_GetLiftArmStatus();  //// modified by Samuel
    if ( eEnable==s_tVenusCB.cLiftarmEn )
    {
        liftarm_open();
        s_tVenusCB.stGeneralInfo.bBandLocation = 0x01; // right hand //CC_BLE_Cmd_GetLiftArmBandSetting();
        //liftarm_set_hangconfig(s_tVenusCB.stGeneralInfo.bBandLocation);
        liftarm_set_hangconfig(0x01); // right hand for test!!!!!!!!
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


    drvi_RequestIrq(TOUCH_INT_PIN, TOUCH_int_handler, IRQ_TYPE_EDGE_FALLING);
    drvi_EnableIrq(TOUCH_INT_PIN);
#ifdef HRM_EN
    drvi_RequestIrq(HRM_INT_PIN, HRM_int_handler, IRQ_TYPE_EDGE_FALLING);   
#endif
    ACC_init();
    //GYR_Init(); // Not nessary if ACC_init() was done.
    MAG_Init();
    CC_GYRO_Set_ODR(LSM6DS3_ACC_GYRO_ODR_G_POWER_DOWN);

    app_displayoled_init();

    app_displayoled_start();


}

void venus_app_init(void)
{
    
    app_multiple_timer_init();

    app_Time_Init();
    s_tVenusCB.stSysCurTime = app_getSysTime();

    CC_AppSrv_HR_Init();


    CC_PageMgr_Init();

    CC_VENUS_OLEDGeneralOutTimerStart(5000);

}





void rwip_ignore_ll_conn_param_update_patch(void)
{
    *p_llm_le_event_mask &= ~LE_REM_CON_PARA_REQ_EVT_MSK; // test by Samuel
}


/*
 * VENUS MAIN FUNCTION
 ****************************************************************************************
 */

int venus_main(void)
{
    
    TracerInfo("== Venus Main Start ==\r\n");

    venus_platform_init();

    venus_app_init();

    application_timers_start();

    s_tVenusCB.bAvg_BatLevel = 100;

    s_tVenusCB.eSystemPwrState = eSysStateInit;

    TracerInfo("> Venus While\r\n");


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
                s_tVenusCB.eSystemPwrState = eSysStateNormal;
                break;

            case eSysStateNormal:
                _app_scheduler();
                rwip_schedule();
                rwip_ignore_ll_conn_param_update_patch();
        
        }
    }
}

