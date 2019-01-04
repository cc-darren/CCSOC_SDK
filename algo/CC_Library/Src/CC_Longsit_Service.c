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

#include "project.h"

#ifdef LONGSIT_EN
//#include "app_timer.h"
#include "CC_Longsit_Service.h"
#include "CC_SleepMonitor_Service.h"
#include "CC_AppSrvc_HeartRate.h"
#ifdef UTILITY_EN
#include "CC_Utility_Service.h"
#endif

#include "pah8series_data_c.h"
#include "sw_timer.h"
#include "tracer.h"

#define LONGSIT_TIME_TH 	60


/**********************************************************
 *** STRUCT / DEFINE / ENUM
 **********************************************************/
#define APP_TIMER_PRESCALER        0
#define APP_TIMER_OP_QUEUE_SIZE    4

extern uint8_t CC_Charge_Register(charge_cb_t cb);
extern eStete_t CC_Get_LongsitEnState(void );
extern uint8_t CC_GetLowPowerState(void);
extern eDEV_CHARGE_STATE_t CC_GetChargeStatus(void);
extern uint32_t CC_MainGet_PedCnt(void);
extern void CC_Longsit_Post_TO_HRM_CHECKWEAR(void);
extern void CC_Longsit_Post_TO_MonitorMotion(void);
extern uint8_t CC_MainGet_SwimmingEn(void);
extern void CC_MainGet_CurrentTime(app_date_time_t *_stCurTime);


void CC_Longsit_PostOLedUpdate(void);
void CC_LongSit_Srv_CheckNotifyProc(void);

eLongsit_Wear_State_t  g_bIsWearDevice = eLongsitNoResult;

uint32_t    g_dwLongsitRecordStepCnt = 0;
uint8_t    g_bIsWearOnGoing = false;
uint8_t    g_bIsLongSitRunOnPeriod = false;
db_sys_longsit_t g_stLongSitExecPeriod;

eStete_t g_dwIsActiveLongsit = eDisable;


SW_TIMER_DEF(s_tAppSrvLongSit_Timer_30Minute_PeriodicMeasurement);
SW_TIMER_DEF(s_tAppSrvLongSit_Timer_1Second_PeriodicMeasurement);
SW_TIMER_DEF(s_tAppSrvLongSit_Timer_5Second_PeriodicMeasurement);

#define DEFINE_1_SECOND         1000
#define DEFINE_1_MINUTE          60*DEFINE_1_SECOND

#define LONGSIT_30MINUTE        30*DEFINE_1_MINUTE//1800 s
#define LONGSIT_1Second          1*DEFINE_1_SECOND // 1 s
#define LONGSIT_5Second          5*DEFINE_1_SECOND // 60 s

void CC_Longsit_Srv_Start30MinuteTimer(void)
{
    sw_timer_start(s_tAppSrvLongSit_Timer_30Minute_PeriodicMeasurement, LONGSIT_30MINUTE, NULL);
}
void CC_Longsit_Srv_Stop30MinuteTimer(void)
{
    sw_timer_stop(s_tAppSrvLongSit_Timer_30Minute_PeriodicMeasurement);
}

void CC_Longsit_Srv_Start1SecondTimer(void)
{
    sw_timer_start(s_tAppSrvLongSit_Timer_1Second_PeriodicMeasurement, LONGSIT_1Second, NULL);
}
void CC_Longsit_Srv_Stop1SecondTimer(void)
{
    sw_timer_stop(s_tAppSrvLongSit_Timer_1Second_PeriodicMeasurement);
}

void CC_Longsit_Srv_Start5SecondMonitorMotionTimer(void)
{
    sw_timer_start(s_tAppSrvLongSit_Timer_5Second_PeriodicMeasurement, LONGSIT_5Second, NULL);
}
void CC_Longsit_Srv_Stop5SecondMonitorMotionTimer(void)
{
    sw_timer_stop(s_tAppSrvLongSit_Timer_5Second_PeriodicMeasurement);
}



static void _Longsit_Idle_Detection_handler(void * pvContext)
{

    CC_Longsit_Srv_Start1SecondTimer();
}

static void _Longsit_Idle_HrmProc_handler(void * pvContext)
{

    CC_Longsit_Post_TO_HRM_CHECKWEAR();
    
}

static void _Longsit_Idle_MonitorMotion_handler(void * pvContext)
{

    CC_Longsit_Post_TO_MonitorMotion();
    
}
    
void CC_Longsit_Srv_TimerInit(void)
{
    sw_timer_create(&s_tAppSrvLongSit_Timer_30Minute_PeriodicMeasurement, SW_TIMER_MODE_REPEATED, _Longsit_Idle_Detection_handler);
    sw_timer_create(&s_tAppSrvLongSit_Timer_1Second_PeriodicMeasurement, SW_TIMER_MODE_REPEATED, _Longsit_Idle_HrmProc_handler);
    sw_timer_create(&s_tAppSrvLongSit_Timer_5Second_PeriodicMeasurement, SW_TIMER_MODE_SINGLE_SHOT, _Longsit_Idle_MonitorMotion_handler);
}

eStete_t CC_Longsit_Srv_Get_LongSitServiceState(void)
{
    return g_dwIsActiveLongsit;
}

void CC_Longsit_Srv_Enable(void)
{

    if (eDisable==CC_Get_LongsitEnState())
        return;
    
    if ( eDEVICE_CHARGE_IN == CC_GetChargeStatus()) 
        return;
    
    if  (true == CC_GetLowPowerState())
        return;
            
    if (false == g_bIsLongSitRunOnPeriod)
        return;
            
    if (true == CC_MainGet_SwimmingEn())
        return;
            
    //if (!LongSit_is_open())
    {
        //LongSit_open();
        //LongSit_set_time_threshold(LONGSIT_TIME_TH);
        //LongSit_reset_notification();
        CC_Longsit_Srv_Start30MinuteTimer();
        g_dwIsActiveLongsit = eEnable;
        //TracerInfo("CC_Longsit_Srv_Enable LongSit_open \r\n");
    }
}

void CC_Longsit_Srv_Disable(void)
{
    //if (LongSit_is_open())
    {
		//LongSit_close();
        CC_Longsit_Srv_Stop30MinuteTimer();
        CC_Longsit_Srv_Stop5SecondMonitorMotionTimer();
        CC_Longsit_Srv_Stop1SecondTimer();
        g_dwIsActiveLongsit = eDisable;
        //TracerInfo("CC_Longsit_Srv_Disable  LongSit_close \r\n");
    }
}

static void _LongSit_Charge_Evt_CB(eDEV_CHARGE_STATE_t eState)
{
    if (eDEVICE_CHARGE_IN == eState)
    {
         // Jason, 20170425, Disable longsit algorithm when plug in charging.
            CC_Longsit_Srv_Disable();
        
    }
    else if (eDEVICE_CHARGE_OUT== eState)
    {
            // Jason, 20170425, // plug out charging, reset longsit algorithm
        if ( eEnable ==CC_Get_LongsitEnState())
        {
            CC_Longsit_Srv_Enable();
        }   
    }
    else
    {
        //no this case;
    }
}


void CC_LongSit_Srv_TimeSetting(db_sys_longsit_t *_stExecPeriod)
{
    // Set new time setting to here.
    if ((g_stLongSitExecPeriod.start_time_hour != _stExecPeriod->start_time_hour)
        ||(g_stLongSitExecPeriod.start_time_min != _stExecPeriod->start_time_min)
        ||(g_stLongSitExecPeriod.end_time_hour != _stExecPeriod->end_time_hour)
        ||(g_stLongSitExecPeriod.end_time_min != _stExecPeriod->end_time_min))
    {
        g_stLongSitExecPeriod.start_time_hour= _stExecPeriod->start_time_hour;
        g_stLongSitExecPeriod.start_time_min = _stExecPeriod->start_time_min;
        g_stLongSitExecPeriod.end_time_hour = _stExecPeriod->end_time_hour;
        g_stLongSitExecPeriod.end_time_min  = _stExecPeriod->end_time_min;
        //TracerInfo("CC_LongSit_Srv_TimeSetting Set New Time \r\n");
    }
    else
    {
        //TracerInfo("CC_LongSit_Srv_TimeSetting Time is same with before \r\n");
    }

}

void CC_LongSit_Srv_PollingHandler(void)
{
    //TracerInfo("CC_LongSit_Srv_PollingHandler \r\n");

    if (eDisable==CC_Get_LongsitEnState())
        return;

    if ( eDEVICE_CHARGE_IN == CC_GetChargeStatus())
        return;

    if  (true == CC_GetLowPowerState())
        return;

    if (true == CC_MainGet_SwimmingEn())
        return;

    app_date_time_t _stCurTime;
    CC_MainGet_CurrentTime(&_stCurTime);
    uint16_t _wStartTime = g_stLongSitExecPeriod.start_time_hour *60 + g_stLongSitExecPeriod.start_time_min;
    uint16_t _wEndTime = g_stLongSitExecPeriod.end_time_hour *60 + g_stLongSitExecPeriod.end_time_min;
    uint16_t _wCurTime = _stCurTime.hours*60 + _stCurTime.minutes;


    if (_wStartTime < _wEndTime)
    {
        if ((_wCurTime>= _wStartTime) && (_wCurTime <= _wEndTime))
        {
            //TracerInfo("CC_LongSit_Srv_PollingHandler Enable  1 !!!!!!\r\n");
            if(false == g_bIsLongSitRunOnPeriod)
            {
                g_bIsLongSitRunOnPeriod = true;
                //CC_Longsit_Srv_Enable();
    
            }
        }
        else
        {
            //TracerInfo("CC_LongSit_Srv_PollingHandler Disable  2 !!!!!!\r\n");
            if (true == g_bIsLongSitRunOnPeriod)
            {
                g_bIsLongSitRunOnPeriod = false;
                //CC_Longsit_Srv_Disable();


            }
        }
    }
    else
    {
        if ((_wCurTime < _wStartTime) && (_wCurTime > _wEndTime))
        {
            //TracerInfo("CC_LongSit_Srv_PollingHandler Disable  3 !!!!!!\r\n");

            if (true == g_bIsLongSitRunOnPeriod)
            {
                g_bIsLongSitRunOnPeriod = false;
                //CC_Longsit_Srv_Disable();


            }
        }
        else
        {
            //TracerInfo("CC_LongSit_Srv_PollingHandler enable  4 !!!!!!\r\n");

            if(false == g_bIsLongSitRunOnPeriod)
            {
                g_bIsLongSitRunOnPeriod = true;
                //CC_Longsit_Srv_Enable();
            }
        }
    }
}


void CC_LongSit_Srv_Register(void)
{
    CC_Charge_Register(_LongSit_Charge_Evt_CB);
}
    
void CC_Longsit_Srv_LowPowerStateChange(eStete_t _bEnable , uint8_t _bIsLowPower)
{
    if ((eEnable==_bEnable) && (false == _bIsLowPower))
        CC_Longsit_Srv_Enable();
    else
        CC_Longsit_Srv_Disable();

}

void CC_Longsit_Srv_UtilityAlgoStateChange(uint32_t _dwState)
{
#if 0
    switch (_dwState)
    {
        case UTILITY_ENERGY_IDLE:
        case UTILITY_ENERGY_LOW:
                CC_Longsit_Srv_Disable();   
                break;
        case UTILITY_ENERGY_MEDIUM:
        case UTILITY_ENERGY_HIGH:
                CC_Longsit_Srv_Enable();
            break;
        case UTILITY_ENERGY_UNKNOWN:
        default:
            break;
    }
#endif    
   
}

void CC_Longsit_Srv_handle(uint32_t _dwPedStep)
{
//    LongSit_Process(_dwPedStep); 
}


void CC_Longsit_Srv_Reset_Notification(void)
{
	//LongSit_reset_notification(); 
}

void CC_LongSit_onNotified(uint8_t notified)
{
    TracerInfo("CC_LongSit_onNotified notified= %d \r\n",notified);
    //Enable vib 
    if (notified == 1)
    {
        #ifdef LONGSIT_EN
        if (false == g_bIsWearOnGoing)
            CC_LongSit_Srv_CheckNotifyProc();
        #endif
    }    
}

void CC_LongSit_Srv_CheckNotifyProc(void)
{
    uint8_t _bHrmStatus=0xff;
    
    #ifdef LONGSIT_EN
    if ( false== CC_AppSrv_HR_IsHrmWorking())
    {
        if (false==CC_AppSrv_HR_IsSingleHrEnabled())
        {
            CC_AppSrv_HR_ClrHrmStatus();
            CC_AppSrv_HR_StartSingleHR();
            TracerInfo("CC_LongSit_onNotified OPEN SINGLE HRM\r\n");
        }
        else
        {
            // should be error case 
            CC_Longsit_Srv_Reset_Notification(); 
            TracerInfo("CC_LongSit_onNotified AVOID ERROR CASE TO RESET LONGSIT\r\n");  
        }
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
                
                CC_Longsit_PostOLedUpdate();
                CC_Longsit_Srv_Reset_Notification();       
                TracerInfo("CC_LongSit_onNotified HRM HAS TOUCH  - OPEN OLED\r\n");    
                CC_AppSrv_HR_StopSingleHR();
            }
        }
    
    }
    #endif
}

void CC_LongSit_Srv_Idle_CheckWearProc(void)
{
    uint8_t _bHrmStatus=0xff;
    TracerInfo("CC_LongSit_Srv_Idle_CheckWearProc !!!!!!!!\r\n");

    //Stop motion check timer to wait new result 
    CC_Longsit_Srv_Stop5SecondMonitorMotionTimer();
    
    if ( false== CC_AppSrv_HR_IsHrmWorking())
    {
        
        if (false==CC_AppSrv_HR_IsSingleHrEnabled())
        {
            CC_AppSrv_HR_ClrHrmStatus();
            CC_AppSrv_HR_StartSingleHR();
            g_bIsWearDevice = eLongsitNoResult;
            TracerInfo("CC_LongSit_Srv_Idle_CheckWearProc OPEN SINGLE HRM\r\n");
            g_bIsWearOnGoing = true;
        }
        else
        {
            // should be error case 
            CC_Longsit_Srv_Reset_Notification(); 
            g_bIsWearDevice = eLongsitNoResult;
            TracerInfo("CC_LongSit_Srv_Idle_CheckWearProc AVOID ERROR CASE TO RESET LONGSIT\r\n");  
            g_bIsWearOnGoing = true;
        }
    }
    else
    {
        _bHrmStatus = CC_AppSrv_HR_GetHrmStatus();

        if (0xff == _bHrmStatus)
        {
            TracerInfo("CC_LongSit_Srv_Idle_CheckWearProc HRM PROCESS WAIT RESULT\r\n");
            g_bIsWearDevice = eLongsitNoResult;
            g_bIsWearOnGoing = true;
        }
        else
        {
            _bHrmStatus &=0x0f;
            if (( MSG_NO_TOUCH == _bHrmStatus))
            {
                TracerInfo("CC_LongSit_Srv_Idle_CheckWearProc HRM NO TOUCH - NO OPEN OLED\r\n");    
                CC_AppSrv_HR_StopSingleHR();
                CC_Longsit_Srv_Stop1SecondTimer();
                CC_Longsit_Srv_Reset_Notification(); 
                g_bIsWearDevice = eLongsitNoWear;
                g_dwLongsitRecordStepCnt = CC_MainGet_PedCnt();
                // launch 1 min to monitor motion behavior 
                CC_Longsit_Srv_Start5SecondMonitorMotionTimer();
                g_bIsWearOnGoing = false;
            }
            else
            {
                
                TracerInfo("CC_LongSit_Srv_Idle_CheckWearProc HRM HAS TOUCH  - OPEN OLED\r\n");    
                CC_AppSrv_HR_StopSingleHR();
                CC_Longsit_Srv_Stop1SecondTimer();
                CC_Longsit_Srv_Stop5SecondMonitorMotionTimer();
                g_bIsWearDevice = eLongsitWearOnHand;
                g_bIsWearOnGoing = false;
            }
        }
    
    }
    
    
    
}


#ifdef UTILITY_EN
void CC_LongSit_Srv_Idle_MonitorMotionHanlder(void)
{

    // check pedo step update or motion level over UTILITY_ENERGY_LOW.
    if  (( eLongsitNoWear == g_bIsWearDevice ) &&
          ( g_dwLongsitRecordStepCnt != CC_MainGet_PedCnt() ) ||
          (CC_Utility_Srv_Get_Energy_state() > UTILITY_ENERGY_LOW))
    {
        TracerInfo("CC_LongSit_Srv_Idle_MonitorMotionHanlder  Detect Motion  ------\r\n");  
        CC_Longsit_Srv_Stop5SecondMonitorMotionTimer();
        CC_Longsit_Srv_Reset_Notification();
    }
    else
    {
        TracerInfo("CC_LongSit_Srv_Idle_MonitorMotionHanlder  No Motion !!!!!\r\n");  
        CC_Longsit_Srv_Start5SecondMonitorMotionTimer();
    }
}
#endif

#endif

