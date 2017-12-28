
/**********************************************************
 *** INCLUDE FILE
 **********************************************************/
#include "project.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "CC_HRM_drv.h"
#include "CC_AppSrvc_HeartRate.h"
#include "sw_timer.h"
#include "tracer.h"
#ifdef DB_EN
#include "fds.h"
#include "cc_db_structure.h"
#include "cc_db.h"
#endif
/**********************************************************
 *** STRUCT / DEFINE / ENUM
 **********************************************************/
#define APP_TIMER_PRESCALER        0
#define APP_TIMER_OP_QUEUE_SIZE    4

#define APPSRV_DEFAULT_TIME_PERIODIC_MEASUREMENT    (1800 * 1000)    // uint: ms
#define APPSRV_DEFAULT_MAX_TIME_ONE_MEASUREMENT     (  25 * 1000)    // uint: ms

#define APPSRV_24HR_EXPECTED_DATA_MEASUREMENT_COUNT    (3)

typedef enum
{
    E_APPSRV_HRM_ST_INACTIVE = 0,
    E_APPSRV_HRM_ST_IN_MEASUREMENT,
    E_APPSRV_HRM_ST_HALFTIME_BREAK,
    E_APPSRV_HRM_ST_DUMMY_END
}   E_AppSrv_HrmState;

typedef struct
{
    #if 0    // reserved for post processing, e.g. average
    uint16_t    wAverageWeighting;
    uint16_t    wWriteIndex;
    uint16_t    wRecordCountInBuffer;

    #define _APPSRV_HR_DATA_BUF    (180)
    int16_t     naData[_APPSRV_HR_DATA_BUF];    
    #endif

    int16_t    nData;
    int16_t    nTrustLevel;
    uint8_t   bResultStatus;

}   S_AppSrv_HrData;

typedef struct
{
    E_AppSrv_HrmState    eSingleHrState;
    E_AppSrv_HrmState    eHrsState;
    E_AppSrv_HrmState    e24HrState;

    S_AppSrv_HrData      tData;

    uint32_t             dwPeriodicMeasurementTime;
    uint32_t             dwOneMeasurementMaxTime;

    uint8_t              bRestingHrMax;
    uint8_t              bRestingHrMin;
    uint8_t              bExerciseHrMax;
    uint8_t              bExerciseHrMin;

    bool                 bIs1stMeasurementDropped;
    bool                 bIsAppLocked;
    bool                 bIsWarningLocked;

    uint8_t              b24HrDataCount;
    uint32_t             dwSysTick;

}   S_AppSrv_HR_CB;
/********************************************
**************
 *** VARIABLE
 **********************************************************/
S_AppSrv_HR_CB    s_tAppSrvHrCB;
#ifdef DB_EN 
db_heartrate_t    s_tDbHrm __attribute__((aligned(4)));
#endif
APP_TIMER_DEF(s_tAppSrvHR_Timer_24HR_PeriodicMeasurement);
APP_TIMER_DEF(s_tAppSrvHR_Timer_24HR_OneMeasurement     );
APP_TIMER_DEF(s_tAppSrvHR_Timer_SystemTick);


/**********************************************************
 *** EXTERNAL FUNCTION
 **********************************************************/
// RSCS for temporarily
#define CC_BLE_Cme_Get_HeartRateStrapMode()   eDisable

extern void CC_HRM_Post24HHR_TO_OneMeasurement(void);
extern void CC_HRM_Post24HHR_TO_PeriodicMeasurement(void);
extern void CC_HRM_PostHeartRateStrapModeEvent(uint8_t bSwitch);

extern void CC_MainGet_CurrentTime(app_date_time_t *_stCurTime);
extern void CC_MainSet_HrmData(uint8_t _bHrmData);
extern void CC_MainSet_HrmDataFlag(uint8_t _bHrmReadyFlag);
extern void CC_SetHrmDisplayHrmDataPageEvent(void);
extern uint8_t CC_Charge_Register(charge_cb_t cb);


/**********************************************************
 *** FUNCTION
 **********************************************************/
#ifdef DB_EN 
static void _DB_Save_Hrm_Data(int16_t nHrData, int16_t nTrustLevel)
{
    app_date_time_t    _tTime;
    
    CC_MainGet_CurrentTime(&_tTime);
    
    s_tDbHrm.time.year  = (_tTime.year - 2000);
    s_tDbHrm.time.month = _tTime.month;
    s_tDbHrm.time.day   = _tTime.day;
    s_tDbHrm.time.hour  = _tTime.hours;
    s_tDbHrm.time.min   = _tTime.minutes;
    s_tDbHrm.time.sec   = _tTime.seconds;

    s_tDbHrm.data.hr          = ((uint8_t) nHrData    );
    s_tDbHrm.data.trust_level = ((uint8_t) nTrustLevel);

    if (FDS_SUCCESS != CC_Save_Record(eHrm, ((uint32_t *) &s_tDbHrm), sizeof(db_heartrate_t)))
        TracerInfo("!!! ERROR !!! fail to save HRM to DB...\r\n");
}
#endif
void Hrm_SysTick_Handler(void * pvContext)
{
    UNUSED_PARAMETER(pvContext);
    s_tAppSrvHrCB.dwSysTick++;
}

uint32_t Hrm_get_sys_tick(void)
{
    return s_tAppSrvHrCB.dwSysTick;
}


static void _TO_24HR_PeriodicMeasurement(void * pvContext)
{
    UNUSED_PARAMETER(pvContext);

    s_tAppSrvHrCB.e24HrState = E_APPSRV_HRM_ST_IN_MEASUREMENT;

    CC_HRM_Post24HHR_TO_PeriodicMeasurement();
}


static void _TO_24HR_OneMeasurement(void * pvContext)
{
    UNUSED_PARAMETER(pvContext);

    CC_HRM_Post24HHR_TO_OneMeasurement();
}

static void _AppSrv_HR_Reset(void)
{
    s_tAppSrvHrCB.tData.nData       = 0;
    s_tAppSrvHrCB.tData.nTrustLevel = 0;

    s_tAppSrvHrCB.bIs1stMeasurementDropped = 0;
}

static void _AppSrv_HR_Charge_Evt_CB(eDEV_CHARGE_STATE_t eState)
{
    if (eDEVICE_CHARGE_IN == eState)
    {
        // nothing.    
    }
    else if (eDEVICE_CHARGE_OUT== eState)
    {
        //eAlgoProcState = eStartToService;
        if ( eEnable == CC_BLE_Cme_Get_HeartRateStrapMode())
            CC_HRM_PostHeartRateStrapModeEvent(eEnable);
        
    }
    else
    {
        //no this case;
    }
}

void CC_AppSrv_HR_Register(void)
{
      CC_Charge_Register(_AppSrv_HR_Charge_Evt_CB);
}

void CC_AppSrv_HR_Init(void)
{
    TracerInfo("[CC_AppSrv_HR_Init]\r\n");

    memset(&s_tAppSrvHrCB, 0, sizeof(s_tAppSrvHrCB));

    s_tAppSrvHrCB.dwPeriodicMeasurementTime = APPSRV_DEFAULT_TIME_PERIODIC_MEASUREMENT;
    s_tAppSrvHrCB.dwOneMeasurementMaxTime   = APPSRV_DEFAULT_MAX_TIME_ONE_MEASUREMENT;

    app_timer_create(&s_tAppSrvHR_Timer_SystemTick, APP_TIMER_MODE_REPEATED, Hrm_SysTick_Handler);
    app_timer_create(&s_tAppSrvHR_Timer_24HR_PeriodicMeasurement, APP_TIMER_MODE_SINGLE_SHOT, _TO_24HR_PeriodicMeasurement);
    app_timer_create(&s_tAppSrvHR_Timer_24HR_OneMeasurement     , APP_TIMER_MODE_SINGLE_SHOT, _TO_24HR_OneMeasurement     );
}


void CC_AppSrv_HR_SetLimited(uint8_t _bRestingHrMax,uint8_t bRestingHrMin,uint8_t bExerciseHrMax,uint8_t bExerciseHrMin)
{
  
    s_tAppSrvHrCB.bRestingHrMax = _bRestingHrMax;
    s_tAppSrvHrCB.bRestingHrMin = bRestingHrMin;
    s_tAppSrvHrCB.bExerciseHrMax= bExerciseHrMax;
    s_tAppSrvHrCB.bExerciseHrMin= bExerciseHrMin;
    TracerInfo("bRestingHrMax = %d\r\n",_bRestingHrMax);
    TracerInfo("bRestingHrMin = %d\r\n",bRestingHrMin);
    TracerInfo("bExerciseHrMax = %d\r\n",bExerciseHrMax);
    TracerInfo("bExerciseHrMin = %d\r\n",bExerciseHrMin);     
}

void CC_AppSrv_HR_ResetLimited(uint8_t _bAge)
{

    s_tAppSrvHrCB.bRestingHrMax = APPSRV_HRM_DEFAULT_RESTING_MAX;
    s_tAppSrvHrCB.bRestingHrMin = APPSRV_HRM_DEFAULT_RESTING_MIN;
    s_tAppSrvHrCB.bExerciseHrMax = APPSRV_HRM_DEFAULT_EXERCISE_MAX(_bAge);
    s_tAppSrvHrCB.bExerciseHrMin = APPSRV_HRM_DEFAULT_EXERCISE_MIN(_bAge);
}

void CC_AppSrv_HR_StartSystemTick(void)
{
    APP_ERROR_CHECK(app_timer_start(s_tAppSrvHR_Timer_SystemTick, APP_TIMER_TICKS(1, APP_TIMER_PRESCALER), NULL));
}

void CC_AppSrv_HR_StopSystemTick(void)
{
    APP_ERROR_CHECK(app_timer_stop(s_tAppSrvHR_Timer_SystemTick));
}

void CC_AppSrv_HR_Set24HrPeriod(uint32_t dwPeriodicMeasurementTime_ms, uint32_t dwOneMeasurementMaxTime_ms)
{
    TracerInfo("[CC_AppSrv_HR_Set24HrPeriod] %d / %d\r\n", dwOneMeasurementMaxTime_ms, dwPeriodicMeasurementTime_ms);

    s_tAppSrvHrCB.dwPeriodicMeasurementTime = dwPeriodicMeasurementTime_ms;
    s_tAppSrvHrCB.dwOneMeasurementMaxTime   = dwOneMeasurementMaxTime_ms;
}

void CC_AppSrv_HR_Get24HrPeriod(uint32_t *pdwPeriodicMeasurementTime_ms, uint32_t *pdwOneMeasurementMaxTime_ms)
{
    *pdwPeriodicMeasurementTime_ms = s_tAppSrvHrCB.dwPeriodicMeasurementTime;
    *pdwOneMeasurementMaxTime_ms   = s_tAppSrvHrCB.dwOneMeasurementMaxTime;
}

void CC_AppSrv_HR_StartSingleHR(void)
{
    TracerInfo("[CC_AppSrv_HR_StartSingleHR] S(%d) H(%d) 2(%d)\r\n", s_tAppSrvHrCB.eSingleHrState, s_tAppSrvHrCB.eHrsState, s_tAppSrvHrCB.e24HrState);

    if (E_APPSRV_HRM_ST_INACTIVE != s_tAppSrvHrCB.eSingleHrState)
        return;
#ifdef HRM_EN
    if (false == s_tAppSrvHrCB.bIsAppLocked)
    {
        if (true == CC_HRM_PPG_INIT())
            _AppSrv_HR_Reset();
    }
#endif
    s_tAppSrvHrCB.eSingleHrState = E_APPSRV_HRM_ST_IN_MEASUREMENT;
}

void CC_AppSrv_HR_StopSingleHR(void)
{
    TracerInfo("[CC_AppSrv_HR_StopSingleHR] S(%d) H(%d) 2(%d)\r\n", s_tAppSrvHrCB.eSingleHrState, s_tAppSrvHrCB.eHrsState, s_tAppSrvHrCB.e24HrState);

    if (E_APPSRV_HRM_ST_INACTIVE == s_tAppSrvHrCB.eSingleHrState)
        return;

    if ((E_APPSRV_HRM_ST_IN_MEASUREMENT != s_tAppSrvHrCB.eHrsState) && (E_APPSRV_HRM_ST_IN_MEASUREMENT != s_tAppSrvHrCB.e24HrState))
    {
#ifdef HRM_EN        
        CC_HRM_PPG_DEINIT();
#endif        
        _AppSrv_HR_Reset();
    }

    s_tAppSrvHrCB.eSingleHrState = E_APPSRV_HRM_ST_INACTIVE;
}

void CC_AppSrv_HR_StartHRS(void)
{
    TracerInfo("[CC_AppSrv_HR_StartHRS] S(%d) H(%d) 2(%d)\r\n", s_tAppSrvHrCB.eSingleHrState, s_tAppSrvHrCB.eHrsState, s_tAppSrvHrCB.e24HrState);

    if (E_APPSRV_HRM_ST_INACTIVE != s_tAppSrvHrCB.eHrsState)
        return;
#ifdef HRM_EN
    if (false == s_tAppSrvHrCB.bIsAppLocked)
    {
        if (true == CC_HRM_PPG_INIT())
            _AppSrv_HR_Reset();
    }
#endif
    s_tAppSrvHrCB.eHrsState = E_APPSRV_HRM_ST_IN_MEASUREMENT;
}

void CC_AppSrv_HR_StopHRS(void)
{
    TracerInfo("[CC_AppSrv_HR_StopHRS] S(%d) H(%d) 2(%d)\r\n", s_tAppSrvHrCB.eSingleHrState, s_tAppSrvHrCB.eHrsState, s_tAppSrvHrCB.e24HrState);

    if (E_APPSRV_HRM_ST_INACTIVE == s_tAppSrvHrCB.eHrsState)
        return;
#ifdef HRM_EN
    if ((E_APPSRV_HRM_ST_IN_MEASUREMENT != s_tAppSrvHrCB.eSingleHrState) && (E_APPSRV_HRM_ST_IN_MEASUREMENT != s_tAppSrvHrCB.e24HrState))
    {
        CC_HRM_PPG_DEINIT();
        _AppSrv_HR_Reset();
    }
#endif
    s_tAppSrvHrCB.eHrsState = E_APPSRV_HRM_ST_INACTIVE;
}

void CC_AppSrv_HR_Start24HR(void)
{
    TracerInfo("[CC_AppSrv_HR_Start24HR] S(%d) H(%d) 2(%d)\r\n", s_tAppSrvHrCB.eSingleHrState, s_tAppSrvHrCB.eHrsState, s_tAppSrvHrCB.e24HrState);

    if (E_APPSRV_HRM_ST_INACTIVE != s_tAppSrvHrCB.e24HrState)
        return;

    #if 0    // reserved for post processing, e.g. average
    // TODO: this should be performed when new record is created
    s_tAppSrvHrCB.tData.wWriteIndex          = 0;
    s_tAppSrvHrCB.tData.wRecordCountInBuffer = 0;
    s_tAppSrvHrCB.tData.wAverageWeighting    = 60;
    #endif

    s_tAppSrvHrCB.b24HrDataCount = 0;
    s_tAppSrvHrCB.e24HrState = E_APPSRV_HRM_ST_HALFTIME_BREAK;

    _TO_24HR_PeriodicMeasurement(NULL);
}

void CC_AppSrv_HR_Stop24HR(void)
{
    TracerInfo("[CC_AppSrv_HR_Stop24HR] S(%d) H(%d) 2(%d)\r\n", s_tAppSrvHrCB.eSingleHrState, s_tAppSrvHrCB.eHrsState, s_tAppSrvHrCB.e24HrState);

    if (E_APPSRV_HRM_ST_INACTIVE == s_tAppSrvHrCB.e24HrState)
        return;
#ifdef HRM_EN
    if ((E_APPSRV_HRM_ST_IN_MEASUREMENT != s_tAppSrvHrCB.eSingleHrState) && (E_APPSRV_HRM_ST_IN_MEASUREMENT != s_tAppSrvHrCB.eHrsState))
    {
        CC_HRM_PPG_DEINIT();
        _AppSrv_HR_Reset();
    }
#endif
    app_timer_stop(s_tAppSrvHR_Timer_24HR_PeriodicMeasurement);
    app_timer_stop(s_tAppSrvHR_Timer_24HR_OneMeasurement     );

    s_tAppSrvHrCB.e24HrState = E_APPSRV_HRM_ST_INACTIVE;
}

void CC_AppSrv_HR_DataReport(int16_t nHrData, int16_t nTrustLevel)
{
    TracerInfo("[CC_AppSrv_HR_DataReport] %d, %d, %d\r\n", nHrData, nTrustLevel, s_tAppSrvHrCB.bIs1stMeasurementDropped);

    if (true == s_tAppSrvHrCB.bIsAppLocked)
        return;

    if (   (E_APPSRV_HRM_ST_IN_MEASUREMENT != s_tAppSrvHrCB.eSingleHrState)
        && (E_APPSRV_HRM_ST_IN_MEASUREMENT != s_tAppSrvHrCB.eHrsState     )
        && (E_APPSRV_HRM_ST_IN_MEASUREMENT != s_tAppSrvHrCB.e24HrState    ))
    {
        return;
    }

    if (0 == s_tAppSrvHrCB.bIs1stMeasurementDropped)
    {
        s_tAppSrvHrCB.bIs1stMeasurementDropped = 1;

        return;
    }

    #if 0    // reserved for post processing, e.g. average
    s_tAppSrvHrCB.tData.naData[s_tAppSrvHrCB.tData.wWriteIndex] = nHrData;
    s_tAppSrvHrCB.tData.wWriteIndex                             = ((s_tAppSrvHrCB.tData.wWriteIndex + 1) % s_tAppSrvHrCB.tData.wAverageWeighting);
    #endif

    s_tAppSrvHrCB.tData.nData       = nHrData;
    s_tAppSrvHrCB.tData.nTrustLevel = nTrustLevel;
#ifdef DB_EN
    _DB_Save_Hrm_Data(nHrData, nTrustLevel);
#endif
    if (   (E_APPSRV_HRM_ST_IN_MEASUREMENT == s_tAppSrvHrCB.eSingleHrState)
        || (E_APPSRV_HRM_ST_IN_MEASUREMENT == s_tAppSrvHrCB.eHrsState     ))
    {
        CC_MainSet_HrmData((uint8_t) nHrData);
        CC_MainSet_HrmDataFlag(true);
        CC_SetHrmDisplayHrmDataPageEvent();
    }
    else if (E_APPSRV_HRM_ST_IN_MEASUREMENT == s_tAppSrvHrCB.e24HrState)
    {
        CC_MainSet_HrmData((uint8_t) nHrData);
        CC_MainSet_HrmDataFlag(true);

        s_tAppSrvHrCB.b24HrDataCount++;

        if (s_tAppSrvHrCB.b24HrDataCount >= APPSRV_24HR_EXPECTED_DATA_MEASUREMENT_COUNT)
        {
            app_timer_stop(s_tAppSrvHR_Timer_24HR_OneMeasurement);

            CC_AppSrv_24HR_Handler_ToOneMeasurement();
        }
    }
}

void CC_AppSrv_24HR_Handler_ToOneMeasurement(void)
{
    TracerInfo("[CC_AppSrv_24HR_Handler_ToOneMeasurement] S(%d) H(%d) 2(%d)\r\n", s_tAppSrvHrCB.eSingleHrState, s_tAppSrvHrCB.eHrsState, s_tAppSrvHrCB.e24HrState);

    if (E_APPSRV_HRM_ST_INACTIVE == s_tAppSrvHrCB.e24HrState)
        return;
#ifdef HRM_EN
    if ((E_APPSRV_HRM_ST_IN_MEASUREMENT != s_tAppSrvHrCB.eSingleHrState) && (E_APPSRV_HRM_ST_IN_MEASUREMENT != s_tAppSrvHrCB.eHrsState))
    {
        CC_HRM_PPG_DEINIT();
        _AppSrv_HR_Reset();
    }
#endif
    s_tAppSrvHrCB.e24HrState = E_APPSRV_HRM_ST_HALFTIME_BREAK;
}

void CC_AppSrv_24HR_Handler_ToPeriodicMeasurement(void)
{
    TracerInfo("[CC_AppSrv_24HR_Handler_ToPeriodicMeasurement] S(%d) H(%d) 2(%d)\r\n", s_tAppSrvHrCB.eSingleHrState, s_tAppSrvHrCB.eHrsState, s_tAppSrvHrCB.e24HrState);

    if (E_APPSRV_HRM_ST_INACTIVE == s_tAppSrvHrCB.e24HrState)
        return;
#ifdef HRM_EN
    if (false == s_tAppSrvHrCB.bIsAppLocked)
    {
        if (true == CC_HRM_PPG_INIT())
            _AppSrv_HR_Reset();
    }
#endif
    s_tAppSrvHrCB.e24HrState = E_APPSRV_HRM_ST_IN_MEASUREMENT;

    app_timer_start(s_tAppSrvHR_Timer_24HR_OneMeasurement,
                                    APP_TIMER_TICKS(s_tAppSrvHrCB.dwOneMeasurementMaxTime, APP_TIMER_PRESCALER), NULL);

    app_timer_start(s_tAppSrvHR_Timer_24HR_PeriodicMeasurement,
                                    APP_TIMER_TICKS(s_tAppSrvHrCB.dwPeriodicMeasurementTime, APP_TIMER_PRESCALER), NULL);
}

bool CC_AppSrv_HR_IsSingleHrEnabled(void)
{
    return (E_APPSRV_HRM_ST_INACTIVE != s_tAppSrvHrCB.eSingleHrState);
}

bool CC_AppSrv_HR_IsHrsEnabled(void)
{
    return (E_APPSRV_HRM_ST_INACTIVE != s_tAppSrvHrCB.eHrsState);
}

bool CC_AppSrv_HR_Is24HrEnabled(void)
{
    return (E_APPSRV_HRM_ST_INACTIVE != s_tAppSrvHrCB.e24HrState);
}

bool CC_AppSrv_HR_IsHrmWorking(void)
{
    if (   (E_APPSRV_HRM_ST_IN_MEASUREMENT == s_tAppSrvHrCB.eSingleHrState)
        || (E_APPSRV_HRM_ST_IN_MEASUREMENT == s_tAppSrvHrCB.eHrsState     )
        || (E_APPSRV_HRM_ST_IN_MEASUREMENT == s_tAppSrvHrCB.e24HrState    ))
    {
        return (true);
    }

    return (false);
}

void CC_AppSrv_HR_SetAppLockHrm(bool bIsAppLocked)
{
    s_tAppSrvHrCB.bIsAppLocked = bIsAppLocked;

    if (true == s_tAppSrvHrCB.bIsAppLocked)
    {
#ifdef HRM_EN        
        CC_HRM_PPG_DEINIT();
        _AppSrv_HR_Reset();
#endif        
    }
    else
    {
        if (   (E_APPSRV_HRM_ST_IN_MEASUREMENT == s_tAppSrvHrCB.eSingleHrState)
            || (E_APPSRV_HRM_ST_IN_MEASUREMENT == s_tAppSrvHrCB.eHrsState     )
            || (E_APPSRV_HRM_ST_IN_MEASUREMENT == s_tAppSrvHrCB.e24HrState    ))
        {
#ifdef HRM_EN            
            if (true == CC_HRM_PPG_INIT())
                _AppSrv_HR_Reset();
#endif            
        }
    }
}

void CC_AppSrv_HR_StatusReport(uint8_t _bResult)
{
    s_tAppSrvHrCB.tData.bResultStatus = _bResult;
}

void CC_AppSrv_HR_ClrHrmStatus(void)
{
    s_tAppSrvHrCB.tData.bResultStatus = 0xff;
}
uint8_t CC_AppSrv_HR_GetHrmStatus(void)
{
    TracerInfo("CC_AppSrv_HR_GetHrmStatus bResultStatus =%d \r\n",s_tAppSrvHrCB.tData.bResultStatus);
    return s_tAppSrvHrCB.tData.bResultStatus;
}


