/**
 ****************************************************************************************
 *
 * @file otat_task.h
 *
 * @brief Header file - OTATTASK.
 *
 * Copyright (C) RivieraWaves 2009-2016
 *
 *
 ****************************************************************************************
 */

#ifndef OTAT_TASK_H_
#define OTAT_TASK_H_

/**
 ****************************************************************************************
 * @addtogroup OTATTASK Task
 * @ingroup OTAT
 * @brief Health Thermometer Profile Thermometer Task
 *
 * The OTATTASK is responsible for handling the messages coming in and out of the
 * @ref OTAT reporter block of the BLE Host.
 *
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwip_task.h" // Task definitions
//#include "htp_common.h"
#include "project.h"

/*
 * DEFINES
 ****************************************************************************************
 */

/// Messages for Health Thermometer Profile Thermometer
enum otat_msg_id
{
    /// Start the Health Thermometer Profile Thermometer profile - at connection
    OTAT_ENABLE_REQ = TASK_FIRST_MSG(TASK_ID_OTAT),
    /// Enable confirmation
    OTAT_ENABLE_RSP,

#ifdef BLE_OTA_BL_MODE_EN

    /// Send packet cmd
    OTAT_PACKET_SEND_CMD,

    /// Send control point 
    OTAT_CTRL_PT_SEND_REQ,
    OTAT_CTRL_PT_SEND_RSP,
    OTAT_CTRL_PT_SEND_NOTIFY,
#else
    /// Send
    OTAT_PACKET_SEND_CMD,
    OTAT_CTRL_PT_SEND_NOTIFY,    
#endif
    /// Inform APP that Indication Configuration has been changed - use to update bond data
    OTAT_CFG_INDNTF_IND,
};


/// Database Feature Configuration Flags
enum otat_features
{
    /// Indicate if OTA Char. is supported
    OTAT_OTA_CHAR_SUP              = 0x01,
/*
    /// Indicate if Temperature Type Char. is supported
    OTAT_TEMP_TYPE_CHAR_SUP        = 0x01,
    /// Indicate if Intermediate Temperature Char. is supported
    OTAT_INTERM_TEMP_CHAR_SUP      = 0x02,
    /// Indicate if Measurement Interval Char. is supported
    OTAT_MEAS_INTV_CHAR_SUP        = 0x04,
    /// Indicate if Measurement Interval Char. supports indications
    OTAT_MEAS_INTV_IND_SUP         = 0x08,
    /// Indicate if Measurement Interval Char. is writable
    OTAT_MEAS_INTV_WR_SUP          = 0x10,

    /// All Features supported
    OTAT_ALL_FEAT_SUP              = 0x1F,
*/
    /// All Features supported
    OTAT_ALL_FEAT_SUP              = 0x01,
};


/// Notification and indication configuration
enum otat_ntf_ind_cfg
{
    /// OTA notification enabled
    OTAT_CFG_OTA_NTF    = (1 << 0),
    /// Intermediate measurement notification enabled
    //OTAT_CFG_INTERM_MEAS_NTF    = (1 << 1),
    /// Measurement interval indication
    //OTAT_CFG_MEAS_INTV_IND      = (1 << 2),
};


/*
 * API MESSAGES STRUCTURES
 ****************************************************************************************
 */
/// Parameters of the Health thermometer service database
struct otat_db_cfg
{
    /// Health thermometer Feature (@see enum otat_features)
    uint8_t features;
 /*   
    /// Temperature Type Value
    uint8_t temp_type;

    /// Measurement Interval Valid Range - Minimal Value
    uint16_t valid_range_min;
    /// Measurement Interval Valid Range - Maximal Value
    uint16_t valid_range_max;
    /// Measurement interval (latest known interval range)
    uint16_t meas_intv;
 */  
};

/// Parameters of the @ref OTAT_ENABLE_REQ message
struct otat_enable_req
{
    /// Connection index
    uint8_t conidx;
    /// Notification configuration (Bond Data to restore: @see enum otat_ntf_ind_cfg)
    uint8_t  ntf_ind_cfg;
};

/// Parameters of the @ref OTAT_ENABLE_RSP message
struct otat_enable_rsp
{
    /// Connection index
    uint8_t conidx;
    /// Status of enable request
    uint8_t status;
};

/// Parameters of the @ref OTAT_TEMP_SEND_REQ message
/*
struct otat_temp_send_req
{
    /// Temperature Measurement
    struct htp_temp_meas temp_meas;
    /// Stable or intermediary type of temperature (True stable meas, else false)
    bool stable_meas;
};
*/
/// Parameters of the @ref OTAT_TEMP_SEND_RSP message
struct otat_temp_send_rsp
{
    /// Status
    uint8_t status;
};

/// Parameters of the @ref STEP_COUNT_SEND_REQ message
/*
struct otat_period_meas_send_req
{
    struct htp_period_meas period_meas;
    
};
*/
struct otat_notify_send_req
{
    uint8_t     lenth;
    uint8_t     eArray[20];
};



/// Parameters of the @ref STEP_COUNT_SEND_REQ message
/*
struct otat_swim_meas_send_req
{
    struct htp_swim_meas swim_meas;
    
};
*/

/// Parameters of the @ref STEP_COUNT_SEND_REQ message
/*
struct otat_history_send_req
{
    struct htp_history_meas history_meas;
    
};
*/
/*
/// Parameters of the @ref OTAT_MEAS_INTV_UPD_REQ message
struct otat_meas_intv_upd_req
{
    /// Measurement Interval value
    uint16_t meas_intv;
};

/// Parameters of the @ref OTAT_MEAS_INTV_UPD_RSP message
struct otat_meas_intv_upd_rsp
{
    /// status
    uint8_t status;
};


/// Parameters of the @ref OTAT_MEAS_INTV_CHG_REQ_IND message
struct otat_meas_intv_chg_req_ind
{
    /// Connection index
    uint8_t conidx;
    /// new measurement interval
    uint16_t intv;
};

/// Parameters of the @ref OTAT_MEAS_INTV_CHG_CFM message
struct otat_meas_intv_chg_cfm
{
    /// Connection index
    uint8_t conidx;
    /// status of the request
    uint8_t status;
};
*/

struct otat_packet_send_cmd
{
    //uint8_t     conidx;
    uint16_t    length;
    uint8_t  value[50];
};

/// Parameters of the @ref OTAT_CFG_INDNTF_IND message
struct otat_cfg_indntf_ind
{
    /// connection index
    uint8_t  conidx;
    /// Notification Configuration (@see enum otat_ntf_ind_cfg)
    uint8_t  ntf_ind_cfg;
};


/// @} OTATTASK
#endif // OTAT_TASK_H_
