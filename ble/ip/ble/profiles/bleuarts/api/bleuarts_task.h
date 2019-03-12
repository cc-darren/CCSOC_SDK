/**
 ****************************************************************************************
 *
 * @file bleuarts_task.h
 *
 * @brief Header file - BLEUARTSTASK.
 *
 * Copyright (C) CloudChip 2018
 *
 *
 ****************************************************************************************
 */

#ifndef BLEUARTS_TASK_H_
#define BLEUARTS_TASK_H_

/**
 ****************************************************************************************
 * @addtogroup BLEUARTSTASK Task
 * @ingroup BLEUARTS
 * @brief CloudChip Proprietary Services Task
 *
 * The BLEUARTSTASK is responsible for handling the messages coming in and out of the
 * @ref BLEUARTS reporter block of the BLE Host.
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
enum bleuarts_msg_id
{
    /// Start the BLEUARTS profile - at connection
    BLEUARTS_ENABLE_REQ = TASK_FIRST_MSG(TASK_ID_BLEUARTS),
    /// Enable confirmation
    BLEUARTS_ENABLE_RSP,

    /// Send
    BLEUARTS_PACKET_SEND_CMD,
    BLEUARTS_CTRL_PT_SEND_NOTIFY,
    BLEUARTS_CTRL_PT_SEND_INDICATE,
    BLEUARTS_PACKET_SEND_IMAGE,

    /// Inform APP that Indication Configuration has been changed - use to update bond data
    BLEUARTS_CFG_INDNTF_IND,
};


/// Database Feature Configuration Flags
enum bleuarts_features
{
    /// Indicate if REPORT Char. is supported
    BLEUARTS_REPORT_CHAR_SUP           = 0x01,

    BLEUARTS_ALL_FEAT_SUP              = 0x01,
};


/// Notification and indication configuration
enum bleuarts_ntf_ind_cfg
{
    /// REPORT notification enabled
    BLEUARTS_CFG_TX_NTF    = (1 << 0),
    
    /// REPORT indication enabled
    BLEUARTS_CFG_TX_IND    = (1 << 1),
};


/*
 * API MESSAGES STRUCTURES
 ****************************************************************************************
 */
/// Parameters of the BLEUARTS database
struct bleuarts_db_cfg
{
    /// BLEUARTS Feature (@see enum bleuarts_features)
    uint8_t features;
  
};

/// Parameters of the @ref BLEUARTS_ENABLE_REQ message
struct bleuarts_enable_req
{
    /// Connection index
    uint8_t conidx;
    /// Notification configuration (Bond Data to restore: @see enum bleuarts_ntf_ind_cfg)
    uint8_t  ntf_ind_cfg;
};

/// Parameters of the @ref BLEUARTS_ENABLE_RSP message
struct bleuarts_enable_rsp
{
    /// Connection index
    uint8_t conidx;
    /// Status of enable request
    uint8_t status;
};


/// Parameters of the @ref BLEUARTS_TEMP_SEND_RSP message
struct bleuarts_temp_send_rsp
{
    /// Status
    uint8_t status;
};

/// Parameters of the @ref STEP_COUNT_SEND_REQ message
/*
struct bleuarts_period_meas_send_req
{
    struct htp_period_meas period_meas;
    
};
*/
struct bleuarts_ntf_ind_send_req
{
    uint8_t     lenth;
#ifdef CFG_DLE_EN    
    uint8_t     eArray[244];
#else
    uint8_t     eArray[20];
#endif
};


struct bleuarts_packet_send_cmd
{

    uint16_t    length;
#ifdef CFG_DLE_EN   
    uint8_t     value[244];
#else
    uint8_t     value[20];
#endif    
};

struct bleuarts_packet_send_image
{

    uint16_t    length;
#ifdef CFG_DLE_EN   
    uint8_t     value[244];
#else
    uint8_t     value[20];
#endif    
};

/// Parameters of the @ref BLEUARTS_CFG_INDNTF_IND message
struct bleuarts_cfg_indntf_ind
{
    /// connection index
    uint8_t  conidx;
    /// Notification Configuration (@see enum bleuarts_ntf_ind_cfg)
    uint8_t  ntf_ind_cfg;
};


/// @} BLEUARTSTASK
#endif // BLEUARTS_TASK_H_
