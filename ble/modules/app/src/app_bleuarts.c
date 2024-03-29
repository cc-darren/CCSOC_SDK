/**
 ****************************************************************************************
 *
 * @file app_bleuarts.c
 *
 * @brief CloudChip Proprietary Services Application entry point
 *
 * Copyright (C) CloudChip 2009-2015
 *
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup APP
 * @{
 ****************************************************************************************
 */

#include "rwip_config.h"     // SW configuration

#if (BLE_APP_BLEUARTS)

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "app_bleuarts.h"		// CloudChip Proprietary Services Application Definitions
#include "app.h"				// Application Definitions
#include "app_task.h"			// application task definitions
#include "bleuarts_task.h"		// health thermometer functions
#include "co_bt.h"
#include "prf_types.h"
#include "prf_utils.h"
#include "arch.h"				// Platform Definitions
#include "bleuarts.h"
#include "co_math.h"
#include "ke_timer.h"
#include "drvi_clock.h"
#include "sw_timer.h"
#include "tracer.h"
#include "rwip.h"
#include "stdio.h"

/*
 * LOCAL FUNCTION DEFINITIONS
 ****************************************************************************************
 */


 /**
 ****************************************************************************************
 * @brief Handles reception of the write request event via "Report characteristic"
 *
 * @param[in] msgid Id of the message received (probably unused).
 * @param[in] param Pointer to the parameters of the message.
 * @param[in] dest_id ID of the receiving task instance (probably unused).
 * @param[in] src_id ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
extern void ble_bleuarts_rx_req_data(const uint8_t *rx_data, uint16_t length);
static int bleuarts_packet_send_cmd_handler(ke_msg_id_t const msgid,
                                        struct bleuarts_packet_send_cmd const *param,
                                        ke_task_id_t const dest_id,
                                        ke_task_id_t const src_id)
{
	ble_bleuarts_rx_req_data(param->value, param->length);

    return (KE_MSG_CONSUMED);
}

static int bleuarts_cfg_indntf_ind_handler(ke_msg_id_t const msgid,
                                        struct bleuarts_cfg_indntf_ind const *param,
                                        ke_task_id_t const dest_id,
                                        ke_task_id_t const src_id)
{
    // Do nothing
    return (KE_MSG_CONSUMED);
}


/*
 * GLOBAL FUNCTION DEFINITIONS
 ****************************************************************************************
 */
void app_bleuarts_notify_send(uint8_t *tx_data, uint8_t length)
{
    // Allocate the BLEUARTS_TEMP_SEND_REQ message
    struct bleuarts_ntf_ind_send_req * req = KE_MSG_ALLOC(BLEUARTS_CTRL_PT_SEND_NOTIFY,
                                                    prf_get_task_from_id(TASK_ID_BLEUARTS),
                                                    TASK_APP,
                                                    bleuarts_ntf_ind_send_req);

    req->lenth = length;
    memcpy(req->eArray, tx_data, length);

    ke_msg_send(req);
}

void app_bleuarts_indicate_send(uint8_t *tx_data, uint8_t length)
{
    // Allocate the BLEUARTS_TEMP_SEND_REQ message
    struct bleuarts_ntf_ind_send_req * req = KE_MSG_ALLOC(BLEUARTS_CTRL_PT_SEND_INDICATE,
                                                    prf_get_task_from_id(TASK_ID_BLEUARTS),
                                                    TASK_APP,
                                                    bleuarts_ntf_ind_send_req);

    req->lenth = length;
    memcpy(req->eArray, tx_data, length);

    ke_msg_send(req);
}

void bleuarts_packet_send_reply_handler(uint8_t *tx_data, uint8_t *length)
{
    // write your response data from local
	/*
	// test code:
    uint8_t test_buf[] = "CloudChip";

    *length = sizeof(test_buf);
    memcpy(tx_data, test_buf, sizeof(test_buf));
	*/    
    tx_data[0] = 0;
    *length = 0;
}

void app_bleuarts_init(void)
{
    // Reset the environment: no value should be initilized.
	// memset(&app_bleuarts_env, 0, sizeof(app_bleuarts_env));
}

void app_bleuarts_add_bleuartss(void)
{
    struct bleuarts_db_cfg* db_cfg;
    // Allocate the BLEUARTS_CREATE_DB_REQ
    struct gapm_profile_task_add_cmd *req = KE_MSG_ALLOC_DYN(GAPM_PROFILE_TASK_ADD_CMD,
                                                  TASK_GAPM, TASK_APP,
                                                  gapm_profile_task_add_cmd, sizeof(struct bleuarts_db_cfg));
    // Fill message
    req->operation = GAPM_PROFILE_TASK_ADD;
    req->sec_lvl = PERM(SVC_AUTH, ENABLE);
    req->prf_task_id = TASK_ID_BLEUARTS;
    req->app_task = TASK_APP;
    req->start_hdl = 0;

    // Set parameters
    db_cfg = (struct bleuarts_db_cfg* ) req->param;
    // All features are supported
    db_cfg->features = BLEUARTS_ALL_FEAT_SUP;

    // Send the message
    ke_msg_send(req);
}


void app_bleuarts_enable_prf(uint8_t conidx)
{
    // Allocate the message
    struct bleuarts_enable_req * req = KE_MSG_ALLOC(BLEUARTS_ENABLE_REQ,
                                                prf_get_task_from_id(TASK_ID_BLEUARTS),
                                                TASK_APP,
                                                bleuarts_enable_req);

    // Fill in the parameter structure
    req->conidx        = conidx;
    // NTF/IND initial status - Disabled
    req->ntf_ind_cfg   = PRF_CLI_STOP_NTFIND;

    // Send the message
    ke_msg_send(req);
}

                                        
/**
 ****************************************************************************************
 * @brief CloudChip Proprietary Services timer
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAP).
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int app_bleuarts_msg_handler(ke_msg_id_t const msgid,
                              void const *param,
                              ke_task_id_t const dest_id,
                              ke_task_id_t const src_id)
{
    // Do nothing
    return (KE_MSG_CONSUMED);
}

/*
 * LOCAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

/// Default State handlers definition
const struct ke_msg_handler app_bleuarts_msg_handler_list[] =
{
    // Note: first message is latest message checked by kernel so default is put on top.
    {KE_MSG_DEFAULT_HANDLER,        (ke_msg_func_t)app_bleuarts_msg_handler},

    {BLEUARTS_PACKET_SEND_CMD,          (ke_msg_func_t)bleuarts_packet_send_cmd_handler},       
    {BLEUARTS_CFG_INDNTF_IND,           (ke_msg_func_t)bleuarts_cfg_indntf_ind_handler},
};

const struct ke_state_handler app_bleuarts_table_handler =
    {&app_bleuarts_msg_handler_list[0], (sizeof(app_bleuarts_msg_handler_list)/sizeof(struct ke_msg_handler))};

#endif //BLE_APP_REPORT

/// @} APP

