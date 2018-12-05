/**
 ****************************************************************************************
 *
 * @file app_ccps.c
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

#if (BLE_APP_CCPS)

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "app_ccps.h"                  // CloudChip Proprietary Services Application Definitions
#include "app.h"                     // Application Definitions
#include "app_task.h"                // application task definitions
#include "ccps_task.h"               // health thermometer functions
#include "co_bt.h"
#include "prf_types.h"
#include "prf_utils.h"
#include "arch.h"                    // Platform Definitions
#include "ccps.h"
#include "co_math.h"
#include "ke_timer.h"
#include "drvi_clock.h"
#include "sw_timer.h"
#include "tracer.h"
#include "rwip.h"
#include "app_ota.h"
#ifdef CFG_SDK_CCPS_EN
#include "sdk_ccps.h"
#endif
/*
 * DEFINES
 ****************************************************************************************
 */


/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

/// CCPS application environment structure
//struct app_ccps_env_tag app_ccps_env;

/*
 * LOCAL FUNCTION DEFINITIONS
 ****************************************************************************************
 */


#ifndef CFG_SDK_CCPS_EN 
static void ccps_packet_send_generic_ack(uint8_t type, uint8_t op_code)
{

    S_App_CC_Messages tx_msg;                    

    tx_msg.len      = SIZE_OF_CC_MSG_HDR;
    tx_msg.type     = type;
    tx_msg.op       = op_code | 0x80;    

    app_ota_notify_send((uint8_t*)&tx_msg, (tx_msg.len + SIZE_OF_CC_MSG_LEN)); 

}
#endif

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

static int ccps_packet_send_cmd_handler(ke_msg_id_t const msgid,
                                        struct ccps_packet_send_cmd const *param,
                                        ke_task_id_t const dest_id,
                                        ke_task_id_t const src_id)
{

#ifdef CFG_SDK_CCPS_EN // for SDK only

    sdk_ccps_rx_req_data(param->value, param->length);
    
#else    
    S_App_CC_Messages *ptAppCCMessage = (S_App_CC_Messages *) param->value;
    

    if(ptAppCCMessage->len >= SIZE_OF_CC_MSG_HDR)
    {
        ccps_packet_send_generic_ack(ptAppCCMessage->type, ptAppCCMessage->op);
    
        switch(ptAppCCMessage->type)
        {
            case E_CCPS_FTYPE_OTA: 
            
                APP_OTA_MsgHandler(ptAppCCMessage);
                
                break;
    
            default:
                break;
        }
    }

#endif

    return (KE_MSG_CONSUMED);
}


#ifdef BOOTLOADER
static int ccps_packet_send_image_handler(ke_msg_id_t const msgid,
                                        struct ccps_packet_send_image const *param,
                                        ke_task_id_t const dest_id,
                                        ke_task_id_t const src_id)
{

    
    APP_OTA_Image_Write(param->value, param->length);
    
    return (KE_MSG_CONSUMED);
}
#endif


static int ccps_cfg_indntf_ind_handler(ke_msg_id_t const msgid,
                                        struct ccps_cfg_indntf_ind const *param,
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

void app_ccps_notify_send(uint8_t *tx_data, uint8_t length)
{
    // Allocate the CCPS_TEMP_SEND_REQ message
    struct ccps_ntf_ind_send_req * req = KE_MSG_ALLOC(CCPS_CTRL_PT_SEND_NOTIFY,
                                                    prf_get_task_from_id(TASK_ID_CCPS),
                                                    TASK_APP,
                                                    ccps_ntf_ind_send_req);


    req->lenth = length;
    memcpy(req->eArray, tx_data, length);
                                             

    ke_msg_send(req);
}



void app_ccps_indicate_send(uint8_t *tx_data, uint8_t length)
{
    // Allocate the CCPS_TEMP_SEND_REQ message
    struct ccps_ntf_ind_send_req * req = KE_MSG_ALLOC(CCPS_CTRL_PT_SEND_INDICATE,
                                                    prf_get_task_from_id(TASK_ID_CCPS),
                                                    TASK_APP,
                                                    ccps_ntf_ind_send_req);


    req->lenth = length;
    memcpy(req->eArray, tx_data, length);
                                             

    ke_msg_send(req);
}


void ccps_packet_send_reply_handler(uint8_t *tx_data, uint8_t *length)
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


void app_ccps_init(void)
{
    // Reset the environment: no value should be initilized.
//    memset(&app_ccps_env, 0, sizeof(app_ccps_env));

}


void app_ccps_add_ccpss(void)
{
    struct ccps_db_cfg* db_cfg;
    // Allocate the CCPS_CREATE_DB_REQ
    struct gapm_profile_task_add_cmd *req = KE_MSG_ALLOC_DYN(GAPM_PROFILE_TASK_ADD_CMD,
                                                  TASK_GAPM, TASK_APP,
                                                  gapm_profile_task_add_cmd, sizeof(struct ccps_db_cfg));
    // Fill message
    req->operation = GAPM_PROFILE_TASK_ADD;
    req->sec_lvl = PERM(SVC_AUTH, ENABLE);
    req->prf_task_id = TASK_ID_CCPS;
    req->app_task = TASK_APP;
    req->start_hdl = 0;

    // Set parameters
    db_cfg = (struct ccps_db_cfg* ) req->param;
    // All features are supported
    db_cfg->features = CCPS_ALL_FEAT_SUP;

    // Send the message
    ke_msg_send(req);
}


void app_ccps_enable_prf(uint8_t conidx)
{
    // Allocate the message
    struct ccps_enable_req * req = KE_MSG_ALLOC(CCPS_ENABLE_REQ,
                                                prf_get_task_from_id(TASK_ID_CCPS),
                                                TASK_APP,
                                                ccps_enable_req);

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
static int app_ccps_msg_handler(ke_msg_id_t const msgid,
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
const struct ke_msg_handler app_ccps_msg_handler_list[] =
{
    // Note: first message is latest message checked by kernel so default is put on top.
    {KE_MSG_DEFAULT_HANDLER,        (ke_msg_func_t)app_ccps_msg_handler},

    {CCPS_PACKET_SEND_CMD,          (ke_msg_func_t)ccps_packet_send_cmd_handler},       
    {CCPS_CFG_INDNTF_IND,           (ke_msg_func_t)ccps_cfg_indntf_ind_handler},
    #ifdef BOOTLOADER
    {CCPS_PACKET_SEND_IMAGE,        (ke_msg_func_t)ccps_packet_send_image_handler},           
    #endif
};

const struct ke_state_handler app_ccps_table_handler =
    {&app_ccps_msg_handler_list[0], (sizeof(app_ccps_msg_handler_list)/sizeof(struct ke_msg_handler))};

#endif //BLE_APP_REPORT

/// @} APP

