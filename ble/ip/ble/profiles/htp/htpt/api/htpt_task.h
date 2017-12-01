/**
 ****************************************************************************************
 *
 * @file htpt_task.h
 *
 * @brief Header file - HTPTTASK.
 *
 * Copyright (C) RivieraWaves 2009-2016
 *
 *
 ****************************************************************************************
 */

#ifndef HTPT_TASK_H_
#define HTPT_TASK_H_

/**
 ****************************************************************************************
 * @addtogroup HTPTTASK Task
 * @ingroup HTPT
 * @brief Health Thermometer Profile Thermometer Task
 *
 * The HTPTTASK is responsible for handling the messages coming in and out of the
 * @ref HTPT reporter block of the BLE Host.
 *
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwip_task.h" // Task definitions
#include "htp_common.h"
#include "project.h"
#include "CC_DB_Structure.h"
/*
 * DEFINES
 ****************************************************************************************
 */

/// Messages for Health Thermometer Profile Thermometer
enum htpt_msg_id
{
    /// Start the Health Thermometer Profile Thermometer profile - at connection
    HTPT_ENABLE_REQ = TASK_FIRST_MSG(TASK_ID_HTPT),
    /// Enable confirmation
    HTPT_ENABLE_RSP,

    /// Send temperature value from APP
    HTPT_TEMP_SEND_REQ,
    /// Send period measurement value from APP
    HTPT_TEMP_PERIOD_MEAS_REQ,
    /// Send swim measurement value from APP
    HTPT_TEMP_SWIM_MEAS_REQ,    
    /// Send temperature response
    HTPT_TEMP_SEND_RSP,

    /// Indicate Measurement Interval
    HTPT_MEAS_INTV_UPD_REQ,
    /// Send Measurement Interval response
    HTPT_MEAS_INTV_UPD_RSP,

    /// Inform APP of new measurement interval value requested by a peer device
    HTPT_MEAS_INTV_CHG_REQ_IND,
    /// APP Confirm message of new measurement interval value requested by a peer device
    /// If accepted, it triggers indication on measurement interval attribute
    HTPT_MEAS_INTV_CHG_CFM,

    /// Inform APP that Indication Configuration has been changed - use to update bond data
    HTPT_CFG_INDNTF_IND,
};


/// Database Feature Configuration Flags
enum htpt_features
{
    /// Indicate if Temperature Type Char. is supported
    HTPT_TEMP_TYPE_CHAR_SUP        = 0x01,
    /// Indicate if Intermediate Temperature Char. is supported
    HTPT_INTERM_TEMP_CHAR_SUP      = 0x02,
    /// Indicate if Measurement Interval Char. is supported
    HTPT_MEAS_INTV_CHAR_SUP        = 0x04,
    /// Indicate if Measurement Interval Char. supports indications
    HTPT_MEAS_INTV_IND_SUP         = 0x08,
    /// Indicate if Measurement Interval Char. is writable
    HTPT_MEAS_INTV_WR_SUP          = 0x10,

    /// All Features supported
    HTPT_ALL_FEAT_SUP              = 0x1F,
};


/// Notification and indication configuration
enum htpt_ntf_ind_cfg
{
    /// Stable measurement interval indication enabled
    HTPT_CFG_STABLE_MEAS_IND    = (1 << 0),
    /// Intermediate measurement notification enabled
    HTPT_CFG_INTERM_MEAS_NTF    = (1 << 1),
    /// Measurement interval indication
    HTPT_CFG_MEAS_INTV_IND      = (1 << 2),
};


/*
 * API MESSAGES STRUCTURES
 ****************************************************************************************
 */
/// Parameters of the Health thermometer service database
struct htpt_db_cfg
{
    /// Health thermometer Feature (@see enum htpt_features)
    uint8_t features;
    /// Temperature Type Value
    uint8_t temp_type;

    /// Measurement Interval Valid Range - Minimal Value
    uint16_t valid_range_min;
    /// Measurement Interval Valid Range - Maximal Value
    uint16_t valid_range_max;
    /// Measurement interval (latest known interval range)
    uint16_t meas_intv;
};

/// Parameters of the @ref HTPT_ENABLE_REQ message
struct htpt_enable_req
{
    /// Connection index
    uint8_t conidx;
    /// Notification configuration (Bond Data to restore: @see enum htpt_ntf_ind_cfg)
    uint8_t  ntf_ind_cfg;
};

/// Parameters of the @ref HTPT_ENABLE_RSP message
struct htpt_enable_rsp
{
    /// Connection index
    uint8_t conidx;
    /// Status of enable request
    uint8_t status;
};

/// Parameters of the @ref HTPT_TEMP_SEND_REQ message
struct htpt_temp_send_req
{
    /// Temperature Measurement
    struct htp_temp_meas temp_meas;
    /// Stable or intermediary type of temperature (True stable meas, else false)
    bool stable_meas;
};

/// Parameters of the @ref HTPT_TEMP_SEND_RSP message
struct htpt_temp_send_rsp
{
    /// Status
    uint8_t status;
};

/// Parameters of the @ref STEP_COUNT_SEND_REQ message
struct htpt_period_meas_send_req
{
    struct htp_period_meas period_meas;
    
};

/// Parameters of the @ref STEP_COUNT_SEND_REQ message
struct htpt_swim_meas_send_req
{
    struct htp_swim_meas swim_meas;
    
};


/// Parameters of the @ref STEP_COUNT_SEND_REQ message
struct htpt_history_send_req
{
    struct htp_history_meas history_meas;
    
};


/// Parameters of the @ref HTPT_MEAS_INTV_UPD_REQ message
struct htpt_meas_intv_upd_req
{
    /// Measurement Interval value
    uint16_t meas_intv;
};

/// Parameters of the @ref HTPT_MEAS_INTV_UPD_RSP message
struct htpt_meas_intv_upd_rsp
{
    /// status
    uint8_t status;
};


/// Parameters of the @ref HTPT_MEAS_INTV_CHG_REQ_IND message
struct htpt_meas_intv_chg_req_ind
{
    /// Connection index
    uint8_t conidx;
    /// new measurement interval
    uint16_t intv;
};

/// Parameters of the @ref HTPT_MEAS_INTV_CHG_CFM message
struct htpt_meas_intv_chg_cfm
{
    /// Connection index
    uint8_t conidx;
    /// status of the request
    uint8_t status;
};

/// Parameters of the @ref HTPT_CFG_INDNTF_IND message
struct htpt_cfg_indntf_ind
{
    /// connection index
    uint8_t  conidx;
    /// Notification Configuration (@see enum htpt_ntf_ind_cfg)
    uint8_t  ntf_ind_cfg;
};


typedef enum 
{
    eNOTIFYIDLE =0,
    eNOTIFYCALL,
    eNOTIFYSMS,
    eNOTIFYLONGSIT,
    eNOTIFYLIFTARM,
    eNOTIFY_SETTING_HEARTRATESTRAP_MODE,
    eNOTIFY_SETTING_24HHEARTRATE_MODE,
    eNOTIFYNKNOWN
}eNOTIFY_state_t;



void CC_BLE_Cmd_SetHistoryType(uint8_t cVal);
void CC_BLE_Cmd_ClrHistoryType(void);
void CC_BLE_Cmd_ClrHistoryRecordIndex(void);
uint8_t CC_BLE_Cmd_GetHistoryType(void);
uint16_t CC_BLE_Cmd_GetHistoryRecordIndex(void);
uint8_t CC_BLE_Cmd_GetHistoryDayIndex(void);
void CC_BLE_Cmd_UpdateHistoryRecordIndex(void);
void CC_BLE_Cmd_UpdateHistoryDayIndex(void);
void CC_BLE_Cmd_GetCallState(uint8_t *_Notify,eCALL_state_t *_stCall,
                                                            eSMS_state_t *_stSms,eStete_t *_stLongsit,
                                                            eStete_t * _stLiftarm);
void CC_BLE_Cmd_GetSleepTimeSetting(db_sys_sleep_monitor_t *pData, uint8_t _bOption);
eStete_t CC_BLE_Cme_Get_HeartRateStrapMode(void);
eStete_t CC_BLE_Cme_Get_24HourHeartRateMode(void);
uint8_t CC_BLE_Cmd_GetSwimmingEN(void);
uint8_t CC_BLE_Cmd_CheckGeneralInfo(void);
uint8_t CC_BLE_Cmd_CheckUnitInfo(void);
uint8_t CC_BLE_Cmd_CheckClockAlarm(void);
void CC_BLE_Cmd_GetGeneralInfo(CC_Ble_General_Info_T *pData, uint8_t _Option);
void CC_BLE_Cmd_GetUnitInfo(CC_Ble_Unit_Info_T *pData,uint8_t _Option);
void CC_BLE_Cmd_GetClockAlarm(CC_Ble_Clock_Alarm_T *pData,uint8_t _Option);
void CC_BLE_Cmd_GetGeneralInfo(CC_Ble_General_Info_T *pData, uint8_t _Option);
eStete_t CC_BLE_Cmd_GetLiftArmStatus(void);
uint8_t  CC_BLE_Cmd_GetLiftArmBandSetting(void);
eStete_t CC_BLE_Cmd_GetLongSitStatus(void);
void CC_BLE_Cmd_SetGeneralInfo(const uint8_t *pData);
void CC_BLE_Cmd_SetClockAlarm(const uint8_t *pData);
void CC_BLE_Cmd_GetNotifySetting(db_sys_notify_enabled_t *notify_setting);
void CC_BLE_Cmd_SetNotifySetting(uint8_t *pData);
void CC_BLE_Cmd_SetUnitInfo(const uint8_t *pData);
void CC_BLE_Cmd_SetHrSetting(db_sys_hr_setting_t *ptHrSetting);
uint8_t CC_BLE_Cmd_CheckSleepTimeSetting(void);
void CC_BLE_Cmd_SetSleepTimeSetting(uint8_t * pData);
void CC_BLE_Cmd_GetSleepTimeSetting(db_sys_sleep_monitor_t *pData, uint8_t _bOption);
void CC_BLE_Cmd_SetNotificaitonState(uint8_t state, uint8_t index);

/// @} HTPTTASK
#endif // HTPT_TASK_H_
