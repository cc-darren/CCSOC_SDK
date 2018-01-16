/**
 ****************************************************************************************
 *
 * @file htpt_task.c
 *
 * @brief Health Thermometer Profile Thermometer Task implementation.
 *
 * Copyright (C) RivieraWaves 2009-2016
 *
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup HTPTTASK
 * @{
 ****************************************************************************************
 */


/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "rwip_config.h"

#if (BLE_HT_THERMOM)

#include "gap.h"
#include "gattc_task.h"
#include "attm.h"
#include "htpt.h"
#include "htpt_task.h"
#include "prf_utils.h"

#include "ke_mem.h"
#include "co_utils.h"
#include "project.h"
#include "tracer.h"
#include "CC_DB_Structure.h"

//#include "svc_mgr.h"
#ifdef APP_SERV_MGR_EN  
#include "CC_Sensor_Manager.h"
#endif

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */



/*
 * History function DEFINITIONS
 ****************************************************************************************
 */

#define HISTORY_NOTIFY_INTERVAL               10    
#define SPEED_AND_CADENCE_NOTIFY_INTERVAL  1000



typedef struct
{
    uint8_t _cNotify_General_Info_Flag;
    CC_Ble_General_Info_T _eGeneralInfo;
    uint8_t _cNotify_Unit_Flag;
    CC_Ble_Unit_Info_T _eUnitInfo;
    uint8_t _cNotify_ClockAlarm_Flag;
    CC_Ble_Clock_Alarm_T _eClockAlarmInfo;
    uint8_t _cNotify_SleepMonitorTimeSetting_Flag;
    db_sys_sleep_monitor_t  _stSleepMonitorTimeSetting;
}CC_SYNCDATA_T;


void CC_VENUS_RscTimerStart(uint32_t interval_ms);
void CC_VENUS_RscTimerStop(void);
void CC_VENUS_Lock_SwimOff_TimerStart(void);
void CC_VENUS_Lock_SwimOff_TimerStop(void);
//void CC_HRM_Post24HrModeEvent(uint8_t bSwitch);
//void CC_HRM_PostHeartRateStrapModeEvent(uint8_t bSwitch);
void CC_Swim_OLED_Update(uint8_t _bSwitch);
void CC_VENUS_AccelTimerStop(void);
void CC_VENUS_AccelTimerReset(void);
void CC_VENUS_AccelTimerFifoModeStart(void);
void CC_AppSrv_HR_Set24HrPeriod(uint32_t dwPeriodicMeasurementTime_ms, uint32_t dwOneMeasurementMaxTime_ms);


static uint8_t _cBleCmd_HistoryType=0;
static uint16_t _cBleCmd_HistoryRecordIndex = 1;
static uint8_t _cBleCmd_HistoryDayIndex = 1;
static uint8_t _cBleCmd_SwimmingFlag=0;
CC_SYNCDATA_T s_tVensuSyncData;
static uint8_t  _NotifyShowFlag = 0;
static eCALL_state_t _cBleCmd_NotifyCall;
static eSMS_state_t _cBleCmd_NotifySms;   
static eStete_t _cBleCmd_NotifyLongsit;
static eStete_t _cBleCmd_NotifyLiftarm;
static eStete_t _cBleCmd_NotifySetting_HeartRateStrapMode = eDisable;
static eStete_t _cBleCmd_NotifySetting_24HHeartRateMode   = eDisable;

void CC_BLE_Cmd_SetHistoryType(uint8_t cVal)
{
    _cBleCmd_HistoryRecordIndex = 1;
    _cBleCmd_HistoryDayIndex = 1;

    _cBleCmd_HistoryType = cVal;

    //CC_HRM_PostLockHR();
}

void CC_BLE_Cmd_ClrHistoryType(void)
{
    //CC_HRM_PostUnlockHR();

    _cBleCmd_HistoryType = 0x00;

    _cBleCmd_HistoryRecordIndex = 1;
    _cBleCmd_HistoryDayIndex = 1;
}

uint16_t CC_BLE_Cmd_GetHistoryRecordIndex(void)
{
    return _cBleCmd_HistoryRecordIndex;
}

void CC_BLE_Cmd_ClrHistoryRecordIndex(void)
{
    _cBleCmd_HistoryRecordIndex = 1;
}

uint8_t CC_BLE_Cmd_GetHistoryType(void)
{
    return _cBleCmd_HistoryType;
}

uint8_t CC_BLE_Cmd_GetHistoryDayIndex(void)
{
    return _cBleCmd_HistoryDayIndex;
}

void CC_BLE_Cmd_UpdateHistoryRecordIndex(void)
{
    _cBleCmd_HistoryRecordIndex++;
}

void CC_BLE_Cmd_UpdateHistoryDayIndex(void)
{
    _cBleCmd_HistoryDayIndex++;
}

void CC_BLE_Cmd_SetSwimmingEN(uint8_t cVal)
{
    _cBleCmd_SwimmingFlag = cVal;

    CC_VENUS_Lock_SwimOff_TimerStop();

    if(0x01 == cVal)
        CC_VENUS_Lock_SwimOff_TimerStart(); 
}

uint8_t CC_BLE_Cmd_GetSwimmingEN(void)
{
#ifdef FORCE_SWIM_TEST_EN
    return _cBleCmd_SwimmingFlag = 1;
#else
    return _cBleCmd_SwimmingFlag;
#endif
}



void CC_BLE_Cmd_SetNotificaitonState(uint8_t state, uint8_t index)
{
    
    _NotifyShowFlag =1;
    if (index == eNOTIFYIDLE)
        _NotifyShowFlag =0;
    else if (index == eNOTIFYCALL)
    {
         _cBleCmd_NotifyCall = (eCALL_state_t)state;
    }
    else if (index == eNOTIFYSMS)
    {
        _cBleCmd_NotifySms= (eSMS_state_t)state;
    }
    else if (index == eNOTIFYLONGSIT)
    {
        _cBleCmd_NotifyLongsit= (eStete_t)state;
    }
    else if (index == eNOTIFYLIFTARM)
    {
        _cBleCmd_NotifyLiftarm= (eStete_t)state;
    }
    else if (eNOTIFY_SETTING_HEARTRATESTRAP_MODE == index)
    {
        if (_cBleCmd_NotifySetting_HeartRateStrapMode != ((eStete_t) state))
        {
#ifdef	APP_SERV_MGR_EN				            
            APP_SVCMGR_PostEvent_HrRequest(E_APP_SVC_HR_MODE_STRAP, state);
#else
            CC_HRM_PostHeartRateStrapModeEvent(state);
#endif					
            _cBleCmd_NotifySetting_HeartRateStrapMode = ((eStete_t) state);
        }
    }
    else if (eNOTIFY_SETTING_24HHEARTRATE_MODE == index)
    {
        if (_cBleCmd_NotifySetting_24HHeartRateMode != ((eStete_t) state))
        {
#ifdef	APP_SERV_MGR_EN						            
            APP_SVCMGR_PostEvent_HrRequest(E_APP_SVC_HR_MODE_24HR, state);
#else
            CC_HRM_Post24HrModeEvent(state);
#endif					
            _cBleCmd_NotifySetting_24HHeartRateMode = ((eStete_t) state);
        }
    }
    else
    {
        //no command, index  =0 or other.
        _NotifyShowFlag =0;
        
    }

    //CC_DB_System_Save_Request(DB_SYS_NOTIFY);

}

void CC_BLE_Cmd_SetHrSetting(db_sys_hr_setting_t *ptHrSetting)
{
    _cBleCmd_NotifySetting_HeartRateStrapMode = ptHrSetting->eIsHrsEnabled;
    _cBleCmd_NotifySetting_24HHeartRateMode   = ptHrSetting->eIs24HrEnabled;

#ifdef APP_SERV_MGR_EN	    
    APP_SVCMGR_PostEvent_HrRequest(E_APP_SVC_HR_MODE_STRAP, _cBleCmd_NotifySetting_HeartRateStrapMode);
    APP_SVCMGR_PostEvent_HrRequest(E_APP_SVC_HR_MODE_24HR , _cBleCmd_NotifySetting_24HHeartRateMode  );
#else
    CC_HRM_PostHeartRateStrapModeEvent(_cBleCmd_NotifySetting_HeartRateStrapMode);
    CC_HRM_Post24HrModeEvent(_cBleCmd_NotifySetting_24HHeartRateMode);
#endif	
}

void CC_BLE_Cmd_GetNotifySetting(db_sys_notify_enabled_t *notify_setting)
{
    notify_setting->incomming_call_en = _cBleCmd_NotifyCall;
    notify_setting->incomming_sms_en = _cBleCmd_NotifySms;
    notify_setting->longsit_en = _cBleCmd_NotifyLongsit;
    notify_setting->lifearm_en = _cBleCmd_NotifyLiftarm;
}

void CC_BLE_Cmd_SetNotifySetting(uint8_t *pData)
{
    _cBleCmd_NotifyCall = (eCALL_state_t)*pData++;
    _cBleCmd_NotifySms = (eSMS_state_t)*pData++;
    _cBleCmd_NotifyLongsit = (eStete_t)*pData++;
    _cBleCmd_NotifyLiftarm = (eStete_t)*pData;
}


uint8_t CC_BLE_Cmd_CheckGeneralInfo(void)
{
    return s_tVensuSyncData._cNotify_General_Info_Flag;
}


void CC_BLE_Cmd_SetGeneralInfo(const uint8_t *pData)
{
    s_tVensuSyncData._cNotify_General_Info_Flag = true;
    s_tVensuSyncData._eGeneralInfo.cHeight = (uint8_t)*pData++;
    s_tVensuSyncData._eGeneralInfo.cWeight= (uint8_t)*pData++;
    s_tVensuSyncData._eGeneralInfo.cAge= (uint8_t)*pData++;
    s_tVensuSyncData._eGeneralInfo.cGender= (CC_Gender_t)*pData++;
    s_tVensuSyncData._eGeneralInfo.cStride_Lenght= (uint8_t)*pData++;
    s_tVensuSyncData._eGeneralInfo.cSwim_Pool_Size= (eSWIM_LEN_SET_t)*pData++;
    s_tVensuSyncData._eGeneralInfo.bBandLocation= (uint8_t)*pData;
}
void CC_BLE_Cmd_GetGeneralInfo(CC_Ble_General_Info_T *pData, uint8_t _Option)
{
    memcpy(pData,&s_tVensuSyncData._eGeneralInfo,sizeof(CC_Ble_General_Info_T));
    // the option, true = get form one second polling, false get form DB,
    // avoid DB get data before one second polling. 
    if (_Option == true)
    s_tVensuSyncData._cNotify_General_Info_Flag = false;
}    


uint8_t CC_BLE_Cmd_CheckUnitInfo(void)
{
    return s_tVensuSyncData._cNotify_Unit_Flag;
}
void CC_BLE_Cmd_SetUnitInfo(const uint8_t *pData)
{
    s_tVensuSyncData._cNotify_Unit_Flag = true;
    s_tVensuSyncData._eUnitInfo.cUnitLength= (uint8_t)*pData++;
    s_tVensuSyncData._eUnitInfo.cUnitWeight= (uint8_t)*pData++;

}
void CC_BLE_Cmd_GetUnitInfo(CC_Ble_Unit_Info_T *pData,uint8_t _Option)
{
    memcpy(pData,&s_tVensuSyncData._eUnitInfo,sizeof(CC_Ble_Unit_Info_T));
    // the option, true = get form one second polling, false get form DB,
    // avoid DB get data before one second polling. 
    if (_Option == true)
    s_tVensuSyncData._cNotify_Unit_Flag = false;
}

uint8_t CC_BLE_Cmd_CheckClockAlarm(void)
{
    return s_tVensuSyncData._cNotify_ClockAlarm_Flag;
}

void CC_BLE_Cmd_SetClockAlarm(const uint8_t *pData)
{
    s_tVensuSyncData._cNotify_ClockAlarm_Flag = true;
    s_tVensuSyncData._eClockAlarmInfo.cTotalNum = *pData++;
    for (int i =0; i< (s_tVensuSyncData._eClockAlarmInfo.cTotalNum);i++)
    {
        memcpy(&s_tVensuSyncData._eClockAlarmInfo.cAlarmTime[i],pData,sizeof(CC_Ble_Clock_Set_T));
        pData+=4;
    }

}

eStete_t CC_BLE_Cme_Get_HeartRateStrapMode(void)
{
    return _cBleCmd_NotifySetting_HeartRateStrapMode;
}

eStete_t CC_BLE_Cme_Get_24HourHeartRateMode(void)
{
    return _cBleCmd_NotifySetting_24HHeartRateMode;
}

uint8_t CC_BLE_Cmd_CheckSleepTimeSetting(void)
{
    return s_tVensuSyncData._cNotify_SleepMonitorTimeSetting_Flag;
}

void CC_BLE_Cmd_SetSleepTimeSetting(uint8_t *pData)
{
    s_tVensuSyncData._cNotify_SleepMonitorTimeSetting_Flag = true;
    s_tVensuSyncData._stSleepMonitorTimeSetting.start_time_hour= (uint8_t)*pData++;
    s_tVensuSyncData._stSleepMonitorTimeSetting.start_time_min= (uint8_t)*pData++;
    s_tVensuSyncData._stSleepMonitorTimeSetting.end_time_hour= (uint8_t)*pData++;
    s_tVensuSyncData._stSleepMonitorTimeSetting.end_time_min= (uint8_t)*pData;
}

void CC_BLE_Cmd_GetSleepTimeSetting(db_sys_sleep_monitor_t *pData, uint8_t _bOption)
{
    memcpy(pData,&s_tVensuSyncData._stSleepMonitorTimeSetting,sizeof(db_sys_sleep_monitor_t));
    // the option, true = get form one second polling, false get form DB,
    // avoid DB get data before one second polling. 
    if (_bOption == true) 
    s_tVensuSyncData._cNotify_SleepMonitorTimeSetting_Flag = false;
    TracerInfo("CC_BLE_Cmd_GetSleepTimeSetting   [%d]   \r\n",s_tVensuSyncData._cNotify_SleepMonitorTimeSetting_Flag);

}


void CC_BLE_Cmd_GetClockAlarm(CC_Ble_Clock_Alarm_T *pData,uint8_t _Option)
{
    memcpy(pData,&s_tVensuSyncData._eClockAlarmInfo,sizeof(CC_Ble_Clock_Alarm_T));
    // the option, true = get form one second polling, false get form DB,
    // avoid DB get data before one second polling. 
    if (_Option == true)
    s_tVensuSyncData._cNotify_ClockAlarm_Flag = false;
}


void CC_BLE_Cmd_GetCallState(uint8_t *_Notify,eCALL_state_t *_stCall,
                                                            eSMS_state_t *_stSms,eStete_t *_stLongsit,
                                                            eStete_t * _stLiftarm)
{
    *_Notify = _NotifyShowFlag;
    *_stCall = _cBleCmd_NotifyCall;
    *_stSms = _cBleCmd_NotifySms;
    *_stLongsit = _cBleCmd_NotifyLongsit;
    *_stLiftarm = _cBleCmd_NotifyLiftarm;

   _NotifyShowFlag =0;
    //return _cBleCmd_NotifyCall;
    //return 0;
}


eStete_t CC_BLE_Cmd_GetLiftArmStatus(void)
{
#ifdef FORCE_LIFTARM_TEST_EN
    return _cBleCmd_NotifyLiftarm = eEnable;
#else
   return _cBleCmd_NotifyLiftarm;
#endif
}

uint8_t  CC_BLE_Cmd_GetLiftArmBandSetting(void)
{
   return s_tVensuSyncData._eGeneralInfo.bBandLocation;
}

eStete_t CC_BLE_Cmd_GetLongSitStatus(void)
{
   return _cBleCmd_NotifyLongsit;
}


/**
 ****************************************************************************************
 * @brief Handles reception of the @ref HTPT_ENABLE_REQ message.
 * The handler enables the Health Thermometer Profile Thermometer Role.
 * @param[in] msgid Id of the message received (probably unused).
 * @param[in] param Pointer to the parameters of the message.
 * @param[in] dest_id ID of the receiving task instance (probably unused).
 * @param[in] src_id ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int htpt_enable_req_handler(ke_msg_id_t const msgid,
                                   struct htpt_enable_req const *param,
                                   ke_task_id_t const dest_id,
                                   ke_task_id_t const src_id)
{
    uint8_t status = PRF_ERR_REQ_DISALLOWED;

    // check state of the task
    if(gapc_get_conhdl(param->conidx) != GAP_INVALID_CONHDL)
    {
        // restore Bond Data
        struct htpt_env_tag* htpt_env =  PRF_ENV_GET(HTPT, htpt);
        htpt_env->ntf_ind_cfg[param->conidx] = param->ntf_ind_cfg;
        status = GAP_ERR_NO_ERROR;

    }

    // send response
    struct htpt_enable_rsp *rsp = KE_MSG_ALLOC(HTPT_ENABLE_RSP, src_id, dest_id, htpt_enable_rsp);
    rsp->conidx = param->conidx;
    rsp->status = status;
    ke_msg_send(rsp);

    return (KE_MSG_CONSUMED);
}


/**
 ****************************************************************************************
 * @brief Handles reception of the @ref HTPT_TEMP_SEND_REQ message.
 * @param[in] msgid Id of the message received (probably unused).
 * @param[in] param Pointer to the parameters of the message.
 * @param[in] dest_id ID of the receiving task instance (probably unused).
 * @param[in] src_id ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
 /*
static int htpt_temp_send_req_handler(ke_msg_id_t const msgid,
                                      struct htpt_temp_send_req const *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id)
{
    // Status
    int msg_status = KE_MSG_SAVED;
    uint8_t state = ke_state_get(dest_id);

    // check state of the task
    if(state == HTPT_IDLE)
    {
        // Get the address of the environment
        struct htpt_env_tag *htpt_env = PRF_ENV_GET(HTPT, htpt);

        // for intermediate measurement, feature must be enabled
        if(!(param->stable_meas) && (!HTPT_IS_FEATURE_SUPPORTED(htpt_env->features, HTPT_INTERM_TEMP_CHAR_SUP)))
        {
            struct htpt_temp_send_rsp *rsp = KE_MSG_ALLOC(HTPT_TEMP_SEND_RSP, src_id, dest_id, htpt_temp_send_rsp);
            rsp->status = PRF_ERR_FEATURE_NOT_SUPPORTED;
            ke_msg_send(rsp);
        }
        else
        {
            // allocate operation to execute
            htpt_env->operation    = (struct htpt_op *) ke_malloc(sizeof(struct htpt_op) + HTPT_TEMP_MEAS_MAX_LEN, KE_MEM_ATT_DB);

            // Initialize operation parameters
            htpt_env->operation->cursor  = 0;
            htpt_env->operation->dest_id = src_id;
            htpt_env->operation->conidx  = GAP_INVALID_CONIDX;

            // Stable measurement indication or intermediate measurement notification
            if(param->stable_meas)
            {
                htpt_env->operation->op      = HTPT_CFG_STABLE_MEAS_IND;
                htpt_env->operation->handle  = HTPT_HANDLE(HTS_IDX_TEMP_MEAS_VAL);
            }
            else
            {
                htpt_env->operation->op      = HTPT_CFG_INTERM_MEAS_NTF;
                htpt_env->operation->handle  = HTPT_HANDLE(HTS_IDX_INTERM_TEMP_VAL);
            }

            //Pack the temperature measurement value
            htpt_env->operation->length  = htpt_pack_temp_value(&(htpt_env->operation->data[0]), param->temp_meas);

            // put task in a busy state
            ke_state_set(dest_id, HTPT_BUSY);

            // execute operation
            htpt_exe_operation();
        }

        msg_status = KE_MSG_CONSUMED;
    }

    return (msg_status);
}
*/

static int htpt_period_meas_send_req_handler(ke_msg_id_t const msgid,
                                      struct htpt_period_meas_send_req const *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id)
{
    // Status
    int msg_status = KE_MSG_SAVED;
    uint8_t state = ke_state_get(dest_id);

    // check state of the task
    if(state == HTPT_IDLE)
    {
        // Get the address of the environment
        struct htpt_env_tag *htpt_env = PRF_ENV_GET(HTPT, htpt);

#if 0
        // for intermediate measurement, feature must be enabled
        if(!(param->stable_meas) && (!HTPT_IS_FEATURE_SUPPORTED(htpt_env->features, HTPT_INTERM_TEMP_CHAR_SUP)))
        {
            struct htpt_temp_send_rsp *rsp = KE_MSG_ALLOC(HTPT_TEMP_SEND_RSP, src_id, dest_id, htpt_temp_send_rsp);
            rsp->status = PRF_ERR_FEATURE_NOT_SUPPORTED;
            ke_msg_send(rsp);
        }
        else
#endif            
        {
            // allocate operation to execute
            htpt_env->operation    = (struct htpt_op *) ke_malloc(sizeof(struct htpt_op) + HTPT_TEMP_MEAS_MAX_LEN, KE_MEM_ATT_DB);

            // Initialize operation parameters
            htpt_env->operation->cursor  = 0;
            htpt_env->operation->dest_id = src_id;
            htpt_env->operation->conidx  = GAP_INVALID_CONIDX;

            htpt_env->operation->op      = HTPT_CFG_INTERM_MEAS_NTF;
            htpt_env->operation->handle  = HTPT_HANDLE(HTS_IDX_INTERM_TEMP_VAL);
            
/*
            // Stable measurement indication or intermediate measurement notification
            if(param->stable_meas)
            {
                htpt_env->operation->op      = HTPT_CFG_STABLE_MEAS_IND;
                htpt_env->operation->handle  = HTPT_HANDLE(HTS_IDX_TEMP_MEAS_VAL);
            }
            else
            {
                htpt_env->operation->op      = HTPT_CFG_INTERM_MEAS_NTF;
                htpt_env->operation->handle  = HTPT_HANDLE(HTS_IDX_INTERM_TEMP_VAL);
            }
*/
            htpt_env->operation->length  = sizeof(struct htp_period_meas);

            memcpy(htpt_env->operation->data, &param->period_meas, sizeof(struct htp_period_meas));

            //Pack the temperature measurement value
            //htpt_env->operation->length  = htpt_pack_temp_value(&(htpt_env->operation->data[0]), param->temp_meas);

            // put task in a busy state
            ke_state_set(dest_id, HTPT_BUSY);

            // execute operation
            htpt_exe_operation();
        }

        msg_status = KE_MSG_CONSUMED;
    }

    return (msg_status);
}


static int htpt_swim_meas_send_req_handler(ke_msg_id_t const msgid,
                                      struct htpt_swim_meas_send_req const *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id)
{
    // Status
    int msg_status = KE_MSG_SAVED;
    uint8_t state = ke_state_get(dest_id);

    // check state of the task
    if(state == HTPT_IDLE)
    {
        // Get the address of the environment
        struct htpt_env_tag *htpt_env = PRF_ENV_GET(HTPT, htpt);

        if(1)    
        {
            // allocate operation to execute
            htpt_env->operation    = (struct htpt_op *) ke_malloc(sizeof(struct htpt_op) + HTPT_TEMP_MEAS_MAX_LEN, KE_MEM_ATT_DB);

            // Initialize operation parameters
            htpt_env->operation->cursor  = 0;
            htpt_env->operation->dest_id = src_id;
            htpt_env->operation->conidx  = GAP_INVALID_CONIDX;

            htpt_env->operation->op      = HTPT_CFG_INTERM_MEAS_NTF;
            htpt_env->operation->handle  = HTPT_HANDLE(HTS_IDX_INTERM_TEMP_VAL);
            
            htpt_env->operation->length  = sizeof(struct htp_swim_meas);

            memcpy(htpt_env->operation->data, &param->swim_meas, sizeof(struct htp_swim_meas));
                
            //Pack the temperature measurement value
            //htpt_env->operation->length  = htpt_pack_temp_value(&(htpt_env->operation->data[0]), param->temp_meas);

            // put task in a busy state
            ke_state_set(dest_id, HTPT_BUSY);

            // execute operation
            htpt_exe_operation();
        }

        msg_status = KE_MSG_CONSUMED;
    }

    return (msg_status);
}

/**
 ****************************************************************************************
 * @brief Request to update Measurement Interval Value
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance
 * @param[in] src_id    ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int htpt_meas_intv_upd_req_handler(ke_msg_id_t const msgid,
                                          struct htpt_meas_intv_upd_req const *param,
                                          ke_task_id_t const dest_id,
                                          ke_task_id_t const src_id)
{
    int msg_status = KE_MSG_SAVED;
    uint8_t state = ke_state_get(dest_id);

    // check state of the task
    if(state == HTPT_IDLE)
    {
        // Get the address of the environment
        struct htpt_env_tag *htpt_env = PRF_ENV_GET(HTPT, htpt);

        // update measurement interval
        htpt_env->meas_intv = param->meas_intv;

        //Check if Measurement Interval indication is supported
        if(!HTPT_IS_FEATURE_SUPPORTED(htpt_env->features, HTPT_MEAS_INTV_CHAR_SUP))
        {
            struct htpt_meas_intv_upd_rsp *rsp = KE_MSG_ALLOC(HTPT_MEAS_INTV_UPD_RSP, src_id, dest_id, htpt_meas_intv_upd_rsp);
            rsp->status = PRF_ERR_FEATURE_NOT_SUPPORTED;
            ke_msg_send(rsp);
        }
        else
        {
            // update internal measurement interval value
            htpt_env->meas_intv = param->meas_intv;

            // no indication to trigger
            if(!HTPT_IS_FEATURE_SUPPORTED(htpt_env->features, HTPT_MEAS_INTV_IND_SUP))
            {
                struct htpt_meas_intv_upd_rsp *rsp = KE_MSG_ALLOC(HTPT_MEAS_INTV_UPD_RSP, src_id, dest_id, htpt_meas_intv_upd_rsp);
                rsp->status = GAP_ERR_NO_ERROR;
                ke_msg_send(rsp);
            }
            // trigger measurement update indication
            else
            {
                // allocate operation to execute
                htpt_env->operation    = (struct htpt_op *) ke_malloc(sizeof(struct htpt_op) + HTPT_MEAS_INTV_MAX_LEN, KE_MEM_ATT_DB);

                // Initialize operation parameters
                htpt_env->operation->op      = HTPT_CFG_MEAS_INTV_IND;
                htpt_env->operation->handle  = HTPT_HANDLE(HTS_IDX_MEAS_INTV_VAL);
                htpt_env->operation->dest_id = src_id;
                htpt_env->operation->cursor  = 0;
                htpt_env->operation->conidx  = GAP_INVALID_CONIDX;

                // Pack the interval value
                htpt_env->operation->length = HTPT_MEAS_INTV_MAX_LEN;
                co_write16p(htpt_env->operation->data, param->meas_intv);

                // put task in a busy state
                ke_state_set(dest_id, HTPT_BUSY);

                // execute operation
                htpt_exe_operation();
            }
        }

        msg_status = KE_MSG_CONSUMED;
    }

    return (msg_status);
}

/**
 ****************************************************************************************
 * @brief Handles reception of the attribute info request message.
 *
 * @param[in] msgid Id of the message received (probably unused).
 * @param[in] param Pointer to the parameters of the message.
 * @param[in] dest_id ID of the receiving task instance (probably unused).
 * @param[in] src_id ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int gattc_att_info_req_ind_handler(ke_msg_id_t const msgid,
        struct gattc_att_info_req_ind *param,
        ke_task_id_t const dest_id,
        ke_task_id_t const src_id)
{
    struct htpt_env_tag* htpt_env = PRF_ENV_GET(HTPT, htpt);
    uint8_t att_idx = HTPT_IDX(param->handle);
    struct gattc_att_info_cfm * cfm;

    //Send write response
    cfm = KE_MSG_ALLOC(GATTC_ATT_INFO_CFM, src_id, dest_id, gattc_att_info_cfm);
    cfm->handle = param->handle;

    switch(att_idx)
    {
        case HTS_IDX_MEAS_INTV_VAL:
        {
            // force length to zero to reject any write starting from something != 0
            cfm->length = 0;
            cfm->status = GAP_ERR_NO_ERROR;
        }break;

        case HTS_IDX_TEMP_MEAS_IND_CFG:
        case HTS_IDX_INTERM_TEMP_CFG:
        case HTS_IDX_MEAS_INTV_CFG:
        {
            cfm->length = HTPT_IND_NTF_CFG_MAX_LEN;
            cfm->status = GAP_ERR_NO_ERROR;
        }break;

        default:
        {
            cfm->status = ATT_ERR_REQUEST_NOT_SUPPORTED;
        }break;
    }

    ke_msg_send(cfm);

    return (KE_MSG_CONSUMED);
}



/**
 ****************************************************************************************
 * @brief Handles reception of the @ref GL2C_CODE_ATT_WR_CMD_IND message.
 * The handler compares the new values with current ones and notifies them if they changed.
 * @param[in] msgid Id of the message received (probably unused).
 * @param[in] param Pointer to the parameters of the message.
 * @param[in] dest_id ID of the receiving task instance (probably unused).
 * @param[in] src_id ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int gattc_write_req_ind_handler(ke_msg_id_t const msgid,
                                      struct gattc_write_req_ind const *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id)
{

    struct htpt_env_tag* htpt_env = PRF_ENV_GET(HTPT, htpt);
    uint8_t conidx  = KE_IDX_GET(src_id);
    uint8_t status = ATT_ERR_NO_ERROR;
    int msg_status = KE_MSG_CONSUMED;

    // to check if confirmation message should be send
    bool send_cfm = true;

    // retrieve handle information
    uint8_t att_idx = HTPT_IDX(param->handle);

    //if(param->length != HTPT_MEAS_INTV_MAX_LEN)
    if(0) // modified by Samuel
    {
        status = PRF_ERR_UNEXPECTED_LEN;
    }
    else
    {
        switch(att_idx)
        {
            case HTS_IDX_MEAS_INTV_VAL:
            {

 #if 1           
                // modified for Venus wristband:
                if(0x55 == param->value[0])
                {
                
                    switch (param->value[1])
                    {
                        case 0xF4:  //swimming command
                            CC_BLE_Cmd_SetSwimmingEN(param->value[2]);
                            CC_Swim_OLED_Update(param->value[2]);
                            break;                
                        case 0xF5:  // sync time
                            app_UpdatTimeDate(&param->value[2]);
                            break;
                        case 0xF6://  Incoming call Enable:0x01 Disable:0x00
                            if (eCALLDISABLE == param->value[2])                    
                            {
                                TracerInfo("Dis EN Incoming call = %d\r\n",param->value[2]);    
                                CC_BLE_Cmd_SetNotificaitonState(eCALLDISABLE, eNOTIFYCALL);
                            }
                            else if (eCALLENABLE == param->value[2])  
                            {
                                TracerInfo("EN Incoming call = %d\r\n",param->value[2]);    
                                CC_BLE_Cmd_SetNotificaitonState(eCALLENABLE, eNOTIFYCALL);
                            }
                            else if (eCALLINCOMMING == param->value[2])  
                            {
                                TracerInfo("Have Incoming call = %d\r\n",param->value[2]);    
                                CC_BLE_Cmd_SetNotificaitonState(eCALLINCOMMING, eNOTIFYCALL);
                            }
                            else if (eCALLINCOMMINGOFF == param->value[2])  
                            {
                                TracerInfo("Close Incoming call = %d\r\n",param->value[2]);    
                                CC_BLE_Cmd_SetNotificaitonState(eCALLINCOMMINGOFF, eNOTIFYCALL);
                            }
                            else
                            {
                                TracerInfo("Wrong Setting on Incomming call = %d\r\n",param->value[2]);    
                                CC_BLE_Cmd_SetNotificaitonState(eCALLUNKNOWN, eNOTIFYIDLE);
                            }
                            
                            //  Incoming SMS Enable:0x01 Disable:0x00
                            if (eSMSDISABLE == param->value[3])                    
                            {
                                TracerInfo("Dis EN Incoming SMS = %d\r\n",param->value[3]);    
                                CC_BLE_Cmd_SetNotificaitonState(eSMSDISABLE, eNOTIFYSMS);

                            }
                            else  if (eSMSENABLE == param->value[3]) 
                            {
                                TracerInfo("EN Incoming SMS = %d\r\n",param->value[3]);    
                                CC_BLE_Cmd_SetNotificaitonState(eSMSENABLE, eNOTIFYSMS);
                            }
                            else  if (eSMSCOMMING == param->value[3]) 
                            {
                                TracerInfo("Have Incoming SMS = %d\r\n",param->value[3]);  
                                CC_BLE_Cmd_SetNotificaitonState(eSMSCOMMING, eNOTIFYSMS);
                            }
                            else
                            {
                                TracerInfo("Wrong Setting on Incomming SMS = %d\r\n",param->value[3]);    
                                CC_BLE_Cmd_SetNotificaitonState(eSMSUNKNOWN, eNOTIFYIDLE);
                            }
        
                            
                            //  LongSit Enable:0x01 Disable:0x00
                            if (eLONGSITDISABLE == param->value[4])                    
                            {
                                TracerInfo("LongSit DisEN = %d\r\n",param->value[4]);    
                                CC_BLE_Cmd_SetNotificaitonState(eLONGSITDISABLE, eNOTIFYLONGSIT);
                            }
                            else if (eLONGSITENABLE == param->value[4])                    
                            {
                                TracerInfo("LongSit call  EN = %d\r\n",param->value[4]);    
                                CC_BLE_Cmd_SetNotificaitonState(eLONGSITENABLE, eNOTIFYLONGSIT);
                            }
                            else
                            {
                                TracerInfo("Wrong longsit setting = %d\r\n",param->value[4]);    
                                CC_BLE_Cmd_SetNotificaitonState(eLONGSITUNKNOWN, eNOTIFYIDLE);

                            }

                            //  Liftarm Enable:0x01 Disable:0x00
                            if (eLIFTARMDISABLE == param->value[5])                    
                            {
                                TracerInfo("Liftarm DisEN = %d\r\n",param->value[5]);    
                                CC_BLE_Cmd_SetNotificaitonState(eLIFTARMDISABLE, eNOTIFYLIFTARM);
                            }
                            else if (eLIFTARMENABLE == param->value[5])                    
                            {
                                TracerInfo("Liftarm call  EN = %d\r\n",param->value[5]);    
                                CC_BLE_Cmd_SetNotificaitonState(eLIFTARMENABLE, eNOTIFYLIFTARM);
                            }
                            else
                            {
                                TracerInfo("Wrong Liftarm setting = %d\r\n",param->value[5]);    
                                CC_BLE_Cmd_SetNotificaitonState(eLIFTARMUNKNOWN, eNOTIFYIDLE);

                            }

                            // HeartRate Strap Mode Control:
                            // 0: DISABLE / 1: ENABLE
                            CC_BLE_Cmd_SetNotificaitonState(param->value[6], eNOTIFY_SETTING_HEARTRATESTRAP_MODE);

                            // 24H HeartRate Mode Control:
                            // 0: DISABLE / 1: ENABLE
                            CC_BLE_Cmd_SetNotificaitonState(param->value[7], eNOTIFY_SETTING_24HHEARTRATE_MODE);
                            // 24HR settings to Service (TBD: refine setting storage)
                            {
                                uint32_t    _dwPeriodicMeasurementTime_ms = 0;
                                uint32_t    _dwOneMeasurementMaxTime_ms   = 0;

                                _dwPeriodicMeasurementTime_ms = (  (param->value[8] * 3600)
                                                                 + (param->value[9] *   60)
                                                                 + (param->value[10]      ));
                                
                                _dwOneMeasurementMaxTime_ms   = (  (param->value[11] * 3600)
                                                                 + (param->value[12] *   60)
                                                                 + (param->value[13]       ));


                                if (   ((0 != _dwPeriodicMeasurementTime_ms) && (0 != _dwOneMeasurementMaxTime_ms))
                                    && (_dwOneMeasurementMaxTime_ms < _dwPeriodicMeasurementTime_ms                 ))
                                {
                                    CC_AppSrv_HR_Set24HrPeriod((_dwPeriodicMeasurementTime_ms * 1000), (_dwOneMeasurementMaxTime_ms * 1000));
                                }
                            }

                            break;
                        case 0xF7:
                            CC_BLE_Cmd_SetGeneralInfo(&param->value[2]);
                            //CC_DB_System_Save_Request(DB_SYS_GENERAL_INFO);
                            break;                    
                        case 0xF8:
                            CC_BLE_Cmd_SetUnitInfo(&param->value[2]);
                            //CC_DB_System_Save_Request(DB_SYS_UNIT);
                            break;
                        case 0xF9:
                            CC_BLE_Cmd_SetClockAlarm(&param->value[2]);
                            //CC_DB_System_Save_Request(DB_SYS_ALARM);
                            break;
                        case 0xFA: // Pedometer
                        case 0xFB: // HRM
                        case 0xFC: // Sleep
                        case 0xFD: // Swimming
                        case 0xF0: // System    
                            if(0x01 == param->value[2]) // 0x01: enabled
                            {
                                  if(0x00 == CC_BLE_Cmd_GetHistoryType())
                                  {
                                         CC_VENUS_RscTimerStop();
                            
                                         CC_BLE_Cmd_SetHistoryType(param->value[1]);
                            
                                         CC_VENUS_RscTimerStart(HISTORY_NOTIFY_INTERVAL);

                                  }
                                  else
                                  {
                                        if(0x00 != CC_BLE_Cmd_GetHistoryType())
                                        {
                                              CC_BLE_Cmd_ClrHistoryType();
                                              
                                              CC_VENUS_RscTimerStop();
                            
                                              CC_VENUS_RscTimerStart(SPEED_AND_CADENCE_NOTIFY_INTERVAL);
                                        }
                                  }
                            }
                        
                            break;
                    }
                }
#endif                

#if 0            
                //uint16_t meas_intv = co_read16p(param->value);

                // check measurement length validity
                //if(((meas_intv >= htpt_env->meas_intv_min) && (meas_intv <= htpt_env->meas_intv_max))
                    // notification can be disabled anyway
                //    || (meas_intv == 0))
                //{
                    uint8_t state = ke_state_get(dest_id);
                    send_cfm = false;

                    // check state of the task to know if it can be proceed immediately
                    if(state == HTPT_IDLE)
                    {
                        // inform application that update of measurement interval is requested by peer device.
                        struct htpt_meas_intv_chg_req_ind * req_ind = KE_MSG_ALLOC(HTPT_MEAS_INTV_CHG_REQ_IND,
                                prf_dst_task_get(&htpt_env->prf_env, conidx), dest_id, htpt_meas_intv_chg_req_ind);
                        req_ind->conidx = conidx;
                        req_ind->intv   = meas_intv;
                        ke_msg_send(req_ind);

                        // allocate operation to execute
                        htpt_env->operation    = (struct htpt_op *) ke_malloc(sizeof(struct htpt_op) + HTPT_MEAS_INTV_MAX_LEN, KE_MEM_ATT_DB);

                        // Initialize operation parameters
                        htpt_env->operation->op      = HTPT_CFG_MEAS_INTV_IND;
                        htpt_env->operation->handle  = HTPT_HANDLE(HTS_IDX_MEAS_INTV_VAL);
                        htpt_env->operation->dest_id = dest_id;
                        htpt_env->operation->conidx  = conidx;
                        // to be sure that no notification will be triggered
                        htpt_env->operation->cursor  = 0xFF;

                        // Pack the interval value
                        htpt_env->operation->length = HTPT_MEAS_INTV_MAX_LEN;
                        co_write16p(htpt_env->operation->data, meas_intv);

                        // put task in a busy state
                        ke_state_set(dest_id, HTPT_BUSY);
                    }
                    else
                    {
                        msg_status = KE_MSG_SAVED;
                    }
                  
                //}
                //else
               // {
                    // value not in expected range
                //    status = HTP_OUT_OF_RANGE_ERR_CODE;
                //}

#endif                  
            }break;

            case HTS_IDX_TEMP_MEAS_IND_CFG:
            {
                status = htpt_update_ntf_ind_cfg(conidx, HTPT_CFG_STABLE_MEAS_IND, PRF_CLI_START_IND, co_read16p(param->value));
            }break;

            case HTS_IDX_INTERM_TEMP_CFG:
            {
                status = htpt_update_ntf_ind_cfg(conidx, HTPT_CFG_INTERM_MEAS_NTF, PRF_CLI_START_NTF, co_read16p(param->value));
            }break;

            case HTS_IDX_MEAS_INTV_CFG:
            {
                status = htpt_update_ntf_ind_cfg(conidx, HTPT_CFG_MEAS_INTV_IND, PRF_CLI_START_IND, co_read16p(param->value));
            }break;

            default:
            {
                status = ATT_ERR_REQUEST_NOT_SUPPORTED;
            }break;
        }
    }

    if(send_cfm)
    {
        //Send write response
        struct gattc_write_cfm * cfm = KE_MSG_ALLOC(GATTC_WRITE_CFM, src_id, dest_id, gattc_write_cfm);
        cfm->handle = param->handle;
        cfm->status = status;
        ke_msg_send(cfm);
    }

    return (msg_status);
}


/**
 ****************************************************************************************
 * @brief Handles reception of the @ref GATTC_READ_REQ_IND message.
 * @param[in] msgid Id of the message received (probably unused).
 * @param[in] param Pointer to the parameters of the message.
 * @param[in] dest_id ID of the receiving task instance (probably unused).
 * @param[in] src_id ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int gattc_read_req_ind_handler(ke_msg_id_t const msgid, struct gattc_write_req_ind const *param,
                                      ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    struct htpt_env_tag* htpt_env = PRF_ENV_GET(HTPT, htpt);
    uint8_t conidx  = KE_IDX_GET(src_id);
    uint8_t value[HTPT_MEAS_INTV_RANGE_MAX_LEN];
    uint8_t value_size = 0;
    uint8_t status = ATT_ERR_NO_ERROR;

    // retrieve handle information
    uint8_t att_idx = HTPT_IDX(param->handle);

    switch(att_idx)
    {
        case HTS_IDX_MEAS_INTV_VAL:
        {
            value_size = HTPT_MEAS_INTV_MAX_LEN;
            co_write16p(&(value[0]), htpt_env->meas_intv);
        }break;

        case HTS_IDX_MEAS_INTV_VAL_RANGE:
        {
            value_size = HTPT_MEAS_INTV_RANGE_MAX_LEN;
            co_write16p(&(value[0]), htpt_env->meas_intv_min);
            co_write16p(&(value[2]), htpt_env->meas_intv_max);
        }break;

        case HTS_IDX_TEMP_MEAS_IND_CFG:
        {
            value_size = HTPT_IND_NTF_CFG_MAX_LEN;
            co_write16p(value, ((htpt_env->ntf_ind_cfg[conidx] & HTPT_CFG_STABLE_MEAS_IND) != 0) ? PRF_CLI_START_IND : PRF_CLI_STOP_NTFIND);
        }break;

        case HTS_IDX_INTERM_TEMP_CFG:
        {
            value_size = HTPT_IND_NTF_CFG_MAX_LEN;
            co_write16p(value, ((htpt_env->ntf_ind_cfg[conidx] & HTPT_CFG_INTERM_MEAS_NTF) != 0) ? PRF_CLI_START_NTF : PRF_CLI_STOP_NTFIND);
        }break;

        case HTS_IDX_MEAS_INTV_CFG:
        {
            value_size = HTPT_IND_NTF_CFG_MAX_LEN;
            co_write16p(value, ((htpt_env->ntf_ind_cfg[conidx] & HTPT_CFG_MEAS_INTV_IND) != 0) ? PRF_CLI_START_IND : PRF_CLI_STOP_NTFIND);
        }break;

        case HTS_IDX_TEMP_TYPE_VAL:
        {
            value_size = HTPT_TEMP_TYPE_MAX_LEN;
            value[0]   = htpt_env->temp_type;
        }break;

        default:
        {
            status = ATT_ERR_REQUEST_NOT_SUPPORTED;
        }break;
    }

    // Send data to peer device
    struct gattc_read_cfm* cfm = KE_MSG_ALLOC_DYN(GATTC_READ_CFM, src_id, dest_id, gattc_read_cfm, value_size);
    cfm->length = value_size;
    memcpy(cfm->value, value, value_size);
    cfm->handle = param->handle;
    cfm->status = status;

    // Send value to peer device.
    ke_msg_send(cfm);

    return (KE_MSG_CONSUMED);
}

/**
 ****************************************************************************************
 * @brief Handles reception of the @ref HTPT_MEAS_INTV_UPD_CFM message.
 * @param[in] msgid Id of the message received (probably unused).
 * @param[in] param Pointer to the parameters of the message.
 * @param[in] dest_id ID of the receiving task instance (probably unused).
 * @param[in] src_id ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int htpt_meas_intv_chg_cfm_handler(ke_msg_id_t const msgid, struct htpt_meas_intv_chg_cfm const *param,
                                      ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    struct htpt_env_tag* htpt_env = PRF_ENV_GET(HTPT, htpt);

    uint8_t state = ke_state_get(dest_id);

    // check state of the task
    if(state == HTPT_BUSY)
    {
        // retrieve connection index from operation
        uint8_t conidx = htpt_env->operation->conidx;

        //Send write response
        struct gattc_write_cfm * cfm = KE_MSG_ALLOC(GATTC_WRITE_CFM,
                KE_BUILD_ID(TASK_GATTC, conidx), dest_id, gattc_write_cfm);
        cfm->handle = HTPT_HANDLE(HTS_IDX_MEAS_INTV_VAL);
        cfm->status = (param->conidx == conidx) ? param->status : PRF_APP_ERROR;
        ke_msg_send(cfm);


        // check if no error occurs
        if(cfm->status == GAP_ERR_NO_ERROR)
        {
            // update the current measurement interval
            htpt_env->meas_intv = co_read16p(htpt_env->operation->data);

            // check if an indication of new measurement interval should be triggered
            if(HTPT_IS_FEATURE_SUPPORTED(htpt_env->features, HTPT_MEAS_INTV_IND_SUP))
            {
                // set back cursor to zero in order to send indication
                htpt_env->operation->cursor  = 0;
            }
        }

        // send indication or terminate operation
        htpt_exe_operation();
    }

    return (KE_MSG_CONSUMED);
}




/**
 ****************************************************************************************
 * @brief Handles @ref GATTC_CMP_EVT for GATTC_NOTIFY and GATT_INDICATE message meaning
 * that Measurement notification/indication has been correctly sent to peer device
 *
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance
 * @param[in] src_id    ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int gattc_cmp_evt_handler(ke_msg_id_t const msgid, struct gattc_cmp_evt const *param,
                                 ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    // continue operation execution
    htpt_exe_operation();

    return (KE_MSG_CONSUMED);
}

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

/// Default State handlers definition
const struct ke_msg_handler htpt_default_state[] =
{
    {HTPT_ENABLE_REQ,            (ke_msg_func_t) htpt_enable_req_handler},

    {GATTC_ATT_INFO_REQ_IND,     (ke_msg_func_t) gattc_att_info_req_ind_handler},   
    {GATTC_WRITE_REQ_IND,        (ke_msg_func_t) gattc_write_req_ind_handler},       // sent from remote
    {GATTC_READ_REQ_IND,         (ke_msg_func_t) gattc_read_req_ind_handler},        // sent from remote
    {GATTC_CMP_EVT,              (ke_msg_func_t) gattc_cmp_evt_handler},    

    //{HTPT_TEMP_SEND_REQ,         (ke_msg_func_t) htpt_temp_send_req_handler},
    {HTPT_TEMP_SEND_REQ,         (ke_msg_func_t) htpt_period_meas_send_req_handler}, // sent from local
    {HTPT_TEMP_SWIM_MEAS_REQ,    (ke_msg_func_t) htpt_swim_meas_send_req_handler},   // sent from local
    {HTPT_MEAS_INTV_UPD_REQ,     (ke_msg_func_t) htpt_meas_intv_upd_req_handler},    // Not used now?
    {HTPT_MEAS_INTV_CHG_CFM,     (ke_msg_func_t) htpt_meas_intv_chg_cfm_handler},    // sent from remote, called by htpt_meas_intv_chg_req_ind_handler()@ app_ht.c
};


///Specifies the message handlers that are common to all states.
const struct ke_state_handler htpt_default_handler = KE_STATE_HANDLER(htpt_default_state);

#endif //BLE_HT_THERMOM

/// @} HTPTTASK
