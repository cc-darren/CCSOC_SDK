/**
 ****************************************************************************************
 *
 * @file app_ota.h
 *
 * @brief Health Thermometer Application entry point
 *
 * Copyright (C) RivieraWaves 2009-2015
 *
 *
 ****************************************************************************************
 */

#ifndef APP_OTA_H_
#define APP_OTA_H_

/**
 ****************************************************************************************
 * @addtogroup APP
 * @ingroup RICOW
 *
 * @brief Health Thermometer Application entry point.
 *
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwip_config.h"     // SW configuration

#if (BLE_APP_OTA)

#include <stdint.h>          // Standard Integer Definition
#include "ke_task.h"
#include "gattc_task.h"

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

extern struct app_env_tag app_env;

/// health thermometer application environment structure
struct app_ota_env_tag
{
    /// Measurement interval
    uint16_t otat_meas_intv;

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

/// Health Thermomter Application environment
extern struct app_ota_env_tag app_ota_env;

/// Table of message handlers
extern const struct ke_state_handler app_ota_table_handler;

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
void app_ota_init(void);

/**
 ****************************************************************************************
 * @brief Add a Health Thermometer instance in the DB
 ****************************************************************************************
 */
void app_ota_add_otas(void);

/**
 ****************************************************************************************
 * @brief Stop the interval measurement timer if used
 ****************************************************************************************
 */
void app_ota_stop_timer (void);

/**
 ****************************************************************************************
 * @brief Enable the health thermometer profile
 ****************************************************************************************
 */
void app_ota_enable_prf(uint8_t);

/**
 ****************************************************************************************
 * @brief Increase temp_value by temp_step
 ****************************************************************************************
 */
void app_ota_temp_inc(void);

/**
 ****************************************************************************************
 * @brief Decrease temp_value by temp_step
 ****************************************************************************************
 */
void app_ota_temp_dec(void);

/**
 ****************************************************************************************
 * @brief Decrease temp_type by 1
 ****************************************************************************************
 */
void app_ota_temp_type_dec(void);

/**
 ****************************************************************************************
 * @brief Increase temp_type by 1
 ****************************************************************************************
 */
//void app_ota_notify_send(uint8_t *tx_data);
void app_ota_notify_send(uint8_t *tx_data, uint8_t length);
void app_ota_ctrl_pt_write(struct gattc_write_req_ind const * param);
void app_ota_pkt_write_cmd(struct gattc_write_req_ind const * param);
void app_ota_ble_disconnect(void * p_context);




#endif //(BLE_APP_OTA)

/// @} APP

#endif // APP_OTA_H_
