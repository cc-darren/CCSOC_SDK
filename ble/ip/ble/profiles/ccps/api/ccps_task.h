/**
 ****************************************************************************************
 *
 * @file ccps_task.h
 *
 * @brief Header file - CCPSTASK.
 *
 * Copyright (C) CloudChip 2018
 *
 *
 ****************************************************************************************
 */

#ifndef CCPS_TASK_H_
#define CCPS_TASK_H_

/**
 ****************************************************************************************
 * @addtogroup CCPSTASK Task
 * @ingroup CCPS
 * @brief CloudChip Proprietary Services Task
 *
 * The CCPSTASK is responsible for handling the messages coming in and out of the
 * @ref CCPS reporter block of the BLE Host.
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

/// Messages for CloudChip Proprietary Services
enum ccps_msg_id
{
    /// Start the CCPS profile - at connection
    CCPS_ENABLE_REQ = TASK_FIRST_MSG(TASK_ID_CCPS),
    /// Enable confirmation
    CCPS_ENABLE_RSP,

    /// Send
    CCPS_PACKET_SEND_CMD,
    CCPS_CTRL_PT_SEND_NOTIFY,
    CCPS_CTRL_PT_SEND_INDICATE,

    /// Inform APP that Indication Configuration has been changed - use to update bond data
    CCPS_CFG_INDNTF_IND,
};


/// Database Feature Configuration Flags
enum ccps_features
{
    /// Indicate if REPORT Char. is supported
    CCPS_REPORT_CHAR_SUP              = 0x01,

    CCPS_ALL_FEAT_SUP              = 0x01,
};


/// Notification and indication configuration
enum ccps_ntf_ind_cfg
{
    /// REPORT notification enabled
    CCPS_CFG_REPORT_NTF    = (1 << 0),
    
    /// REPORT indication enabled
    CCPS_CFG_REPORT_IND    = (1 << 1),
};


/*
 * API MESSAGES STRUCTURES
 ****************************************************************************************
 */
/// Parameters of the CCPS database
struct ccps_db_cfg
{
    /// CCPS Feature (@see enum ccps_features)
    uint8_t features;
  
};

/// Parameters of the @ref CCPS_ENABLE_REQ message
struct ccps_enable_req
{
    /// Connection index
    uint8_t conidx;
    /// Notification configuration (Bond Data to restore: @see enum ccps_ntf_ind_cfg)
    uint8_t  ntf_ind_cfg;
};

/// Parameters of the @ref CCPS_ENABLE_RSP message
struct ccps_enable_rsp
{
    /// Connection index
    uint8_t conidx;
    /// Status of enable request
    uint8_t status;
};


/// Parameters of the @ref CCPS_TEMP_SEND_RSP message
struct ccps_temp_send_rsp
{
    /// Status
    uint8_t status;
};

/// Parameters of the @ref STEP_COUNT_SEND_REQ message
/*
struct ccps_period_meas_send_req
{
    struct htp_period_meas period_meas;
    
};
*/
struct ccps_ntf_ind_send_req
{
    uint8_t     lenth;
#ifdef CFG_DLE_EN    
    uint8_t     eArray[244];
#else
    uint8_t     eArray[20];
#endif
};


struct ccps_packet_send_cmd
{

    uint16_t    length;
#ifdef CFG_DLE_EN   
    uint8_t     value[244];
#else
    uint8_t     value[20];
#endif    
};

/// Parameters of the @ref CCPS_CFG_INDNTF_IND message
struct ccps_cfg_indntf_ind
{
    /// connection index
    uint8_t  conidx;
    /// Notification Configuration (@see enum ccps_ntf_ind_cfg)
    uint8_t  ntf_ind_cfg;
};


/// @} CCPSTASK
#endif // CCPS_TASK_H_
