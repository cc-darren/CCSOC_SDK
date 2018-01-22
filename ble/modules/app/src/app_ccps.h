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
/*
struct app_ccps_env_tag
{

};
*/
/*
 * GLOBAL VARIABLES DECLARATIONS
 ****************************************************************************************
 */

/// CloudChip Proprietary Service Application environment
//extern struct app_ccps_env_tag app_ccps_env;

/// Table of message handlers
extern const struct ke_state_handler app_ccps_table_handler;

/*
 * FUNCTIONS DECLARATION
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 *
 * CloudChip Proprietary Services Functions
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Initialize CCPS Application
 ****************************************************************************************
 */
void app_ccps_init(void);

/**
 ****************************************************************************************
 * @brief Add a CCPS instance in the DB
 ****************************************************************************************
 */
void app_ccps_add_ccpss(void);

/**
 ****************************************************************************************
 * @brief Enable the CloudChip Proprietary Services profile
 ****************************************************************************************
 */
void app_ccps_enable_prf(uint8_t);



/**
 ****************************************************************************************
 * @brief Notify user defined data
 *
 * @param[in] tx_data    Pointer to the data that would be sent by notifiction packets
 * @param[in] length     Length of the data
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
void app_ccps_notify_send(uint8_t *tx_data, uint8_t length);

/**
 ****************************************************************************************
 * @brief Indicate user defined data
 *
 * @param[in] tx_data    Pointer to the data that would be sent by indication packets
 * @param[in] length     Length of the data
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
void app_ccps_indicate_send(uint8_t *tx_data, uint8_t length);


/**
 ****************************************************************************************
 * @brief Read Response via "Report characteristic"
 *
 * @param[in] tx_data    Pointer to the data that would be sent by read response packets
 * @param[in] length     Length of the data
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
void ccps_packet_send_reply_handler(uint8_t *tx_data, uint8_t *length);

/**
 ****************************************************************************************
 * @brief CCPS Sample Code
 ****************************************************************************************
 */
void app_ccps_test_code(void);



#endif //(BLE_APP_CCPS)

/// @} APP

#endif // APP_CCPS_H_
