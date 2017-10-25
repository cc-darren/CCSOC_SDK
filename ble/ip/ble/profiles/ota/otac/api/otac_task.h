/**
 ****************************************************************************************
 *
 * @file otac_task.h
 *
 * @brief Header file - OTACTASK.
 *
 * Copyright (C) RivieraWaves 2009-2016
 *
 *
 ****************************************************************************************
 */

#ifndef OTAC_TASK_H_
#define OTAC_TASK_H_

/**
 ****************************************************************************************
 * @addtogroup OTACTASK Health Thermometer Profile Collector Task
 * @ingroup OTAC
 * @brief Health Thermometer Profile Collector Task
 *
 * The OTACTASK is responsible for handling the messages coming in and out of the
 * @ref OTAC monitor block of the BLE Host.
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


/*
 * DEFINES
 ****************************************************************************************
 */


///  Message id
enum otac_msg_id
{
    /// Start the Health Thermometer Collector profile - at connection
    OTAC_ENABLE_REQ = TASK_FIRST_MSG(TASK_ID_OTAC),
    /// Confirm that cfg connection has finished with discovery results, or that normal cnx started
    OTAC_ENABLE_RSP,


    /// Write Health Thermometer Notification Configuration Value request
    OTAC_HEALTH_TEMP_NTF_CFG_REQ,
    /// Write Health Thermometer Notification Configuration Value response
    OTAC_HEALTH_TEMP_NTF_CFG_RSP,

    ///APP request for measurement interval write
    OTAC_WR_MEAS_INTV_REQ,
    ///APP request for measurement interval write
    OTAC_WR_MEAS_INTV_RSP,

    /// Temperature value received from peer sensor
    OTAC_TEMP_IND,
    /// Measurement interval update indication received from peer sensor
    OTAC_MEAS_INTV_IND,

    /// Generic message to read a HTP characteristic value
    OTAC_RD_CHAR_REQ,
    /// Read HTP characteristic value response
    OTAC_RD_CHAR_RSP,
};



/// Health Thermometer Service Characteristics - Char. Code
enum otac_chars
{
    /// Temperature Measurement
    OTAC_CHAR_HTS_TEMP_MEAS = 0x0,
    /// Temperature Type
    OTAC_CHAR_HTS_TEMP_TYPE,
    /// Intermediate Temperature
    OTAC_CHAR_HTS_INTM_TEMP,
    /// Measurement Interval
    OTAC_CHAR_HTS_MEAS_INTV,

    OTAC_CHAR_HTS_MAX,
};

/// Health Thermometer Service Characteristic Descriptors
enum otac_descs
{
    /// Temp. Meas. Client Config
    OTAC_DESC_HTS_TEMP_MEAS_CLI_CFG,
    /// Intm. Meas. Client Config
    OTAC_DESC_HTS_INTM_MEAS_CLI_CFG,
    /// Meas. Intv. Client Config
    OTAC_DESC_HTS_MEAS_INTV_CLI_CFG,
    /// Meas. Intv. Valid Range,
    OTAC_DESC_HTS_MEAS_INTV_VAL_RGE,

    OTAC_DESC_HTS_MAX,
};

/// Internal codes for reading a HTS characteristic with one single request
enum otac_rd_char
{
    ///Read HTS Temp. Type
    OTAC_RD_TEMP_TYPE           = 0,
    ///Read HTS Measurement Interval
    OTAC_RD_MEAS_INTV,

    ///Read HTS Temperature Measurement Client Cfg. Desc
    OTAC_RD_TEMP_MEAS_CLI_CFG,
    ///Read HTS Intermediate Temperature Client Cfg. Desc
    OTAC_RD_INTM_TEMP_CLI_CFG,
    ///Read HTS Measurement Interval Client Cfg. Desc
    OTAC_RD_MEAS_INTV_CLI_CFG,
    ///Read HTS Measurement Interval Client Cfg. Desc
    OTAC_RD_MEAS_INTV_VAL_RGE,
};

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */


/**
 * Structure containing the characteristics handles, value handles and descriptors for
 * the Health Thermometer Service
 */
struct otac_hts_content
{
    /// service info
    struct prf_svc svc;

    /// Characteristic info:
    struct prf_char_inf chars[OTAC_CHAR_HTS_MAX];

    /// Descriptor handles:
    struct prf_char_desc_inf descs[OTAC_DESC_HTS_MAX];
};

/// Parameters of the @ref OTAC_ENABLE_REQ message
struct otac_enable_req
{
    ///Connection type
    uint8_t con_type;

    ///Existing handle values hts
    struct otac_hts_content hts;
};

/// Parameters of the @ref OTAC_ENABLE_RSP message
struct otac_enable_rsp
{
    ///status
    uint8_t status;
    /// HTS handle values and characteristic properties
    struct otac_hts_content hts;
};

///Parameters of the @ref OTAC_HEALTH_TEMP_NTF_CFG_REQ message
struct otac_health_temp_ntf_cfg_req
{
    ///Stop/notify/indicate value to configure into the peer characteristic
    uint16_t cfg_val;
    ///Own code for differentiating between Temperature Measurement, Intermediate Temperature and Measurement Interval chars
    uint8_t char_code;
};

///Parameters of the @ref OTAC_HEALTH_TEMP_NTF_CFG_RSP message
struct otac_health_temp_ntf_cfg_rsp
{
    ///status
    uint8_t status;
};

///Parameters of the @ref OTAC_WR_MEAS_INTV_REQ message
struct otac_wr_meas_intv_req
{
    ///Interval value
    uint16_t intv;
};

///Parameters of the @ref OTAC_WR_MEAS_INTV_RSP message
struct otac_wr_meas_intv_rsp
{
    ///status
    uint8_t status;
};

///Parameters of the @ref OTAC_TEMP_IND message
struct otac_temp_ind
{
    /// Temperature Measurement Structure
    struct htp_temp_meas temp_meas;
    /// Stable or intermediary type of temperature
    bool stable_meas;
};

///Parameters of @ref OTAC_MEAS_INTV_IND message
struct otac_meas_intv_ind
{
    ///Interval
    uint16_t intv;
};

///Parameters of the @ref OTAC_RD_CHAR_REQ message
struct otac_rd_char_req
{
    ///Characteristic value code (@see enum otac_rd_char)
    uint8_t char_code;
};

///Parameters of the @ref OTAC_RD_CHAR_RSP message
struct otac_rd_char_rsp
{
    /// Attribute data information
    struct prf_att_info info;
};



/// @} OTACTASK

#endif // OTAC_TASK_H_
