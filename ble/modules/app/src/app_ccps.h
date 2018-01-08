/**
 ****************************************************************************************
 *
 * @file app_ccps.h
 *
 * @brief CloudChip Proprietary Services Application entry point
 *
 * Copyright (C) RivieraWaves 2009-2015
 *
 *
 ****************************************************************************************
 */

#ifndef APP_CCPS_H_
#define APP_CCPS_H_

/**
 ****************************************************************************************
 * @addtogroup APP
 * @ingroup RICOW
 *
 * @brief CloudChip Proprietary Services Application entry point.
 *
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwip_config.h"     // SW configuration

#if (BLE_APP_CCPS)

#include <stdint.h>          // Standard Integer Definition
#include "ke_task.h"
#include "gattc_task.h"

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

extern struct app_env_tag app_env;

/// CloudChip Proprietary Services application environment structure
struct app_ccps_env_tag
{
    /// Measurement interval
    uint16_t ccpst_meas_intv;

    /// Temperature value
    uint32_t temp_value;
    /// Temperature step
    uint32_t temp_step;
    /// Temperature type
    uint8_t temp_meas_type;
    /// Temperature type string
    char temp_type_string[16];

    /// Measurement interval timer enable
    bool timer_enable;
};

/*
 * GLOBAL VARIABLES DECLARATIONS
 ****************************************************************************************
 */

/// CloudChip Proprietary Service Application environment
extern struct app_ccps_env_tag app_ccps_env;

/// Table of message handlers
extern const struct ke_state_handler app_ccps_table_handler;

/*
 * FUNCTIONS DECLARATION
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 *
 * Health Thermometer Application Functions
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Initialize Health Thermometer Application
 ****************************************************************************************
 */
void app_ccps_init(void);

/**
 ****************************************************************************************
 * @brief Add a Health Thermometer instance in the DB
 ****************************************************************************************
 */
void app_ccps_add_ccpss(void);

/**
 ****************************************************************************************
 * @brief Stop the interval measurement timer if used
 ****************************************************************************************
 */
void app_ccps_stop_timer (void);

/**
 ****************************************************************************************
 * @brief Enable the CloudChip Proprietary Services profile
 ****************************************************************************************
 */
void app_ccps_enable_prf(uint8_t);

/**
 ****************************************************************************************
 * @brief Increase temp_value by temp_step
 ****************************************************************************************
 */
void app_ccps_temp_inc(void);

/**
 ****************************************************************************************
 * @brief Decrease temp_value by temp_step
 ****************************************************************************************
 */
void app_ccps_temp_dec(void);

/**
 ****************************************************************************************
 * @brief Decrease temp_type by 1
 ****************************************************************************************
 */
void app_ccps_temp_type_dec(void);

/**
 ****************************************************************************************
 * @brief Increase temp_type by 1
 ****************************************************************************************
 */
//void app_ccps_notify_send(uint8_t *tx_data);
void app_ccps_notify_send(uint8_t *tx_data, uint8_t length);
void app_ccps_ctrl_pt_write(struct gattc_write_req_ind const * param);
void app_ccps_pkt_write_cmd(struct gattc_write_req_ind const * param);




#endif //(BLE_APP_CCPS)

/// @} APP

#endif // APP_CCPS_H_
