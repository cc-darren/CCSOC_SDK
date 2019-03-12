/* Copyright (c) 2019 Cloudchip, Inc. All Rights Reserved.
 *
 * The information contained herein is property of Cloudchip, Inc.
 * Terms and conditions of usage are described in detail in CLOUDCHIP
 * STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information.
 * NO WARRANTY of ANY KIND is provided. This heading must NOT be removed 
 * from the file.
 */

/**
 ****************************************************************************************
 *
 * @file bleuarts_task.c
 *
 * @brief CloudChip Proprietary Services Task implementation.
 *
 * Copyright (C) CloudChip 2018
 *
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup BLEUARTSTASK
 * @{
 ****************************************************************************************
 */


/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "rwip_config.h"

#if (BLE_BLEUARTS_SERVER)
#include "gap.h"
#include "gattc_task.h"
#include "attm.h"
#include "bleuarts.h"
#include "bleuarts_task.h"
#include "app_bleuarts.h"
#include "prf_utils.h"
#include "ke_mem.h"
#include "co_utils.h"
#include "project.h"
#include "tracer.h"

/**
 ****************************************************************************************
 * @brief Handles reception of the @ref BLEUARTS_ENABLE_REQ message.
 * The handler enables the BLEUARTS Role.
 * @param[in] msgid Id of the message received (probably unused).
 * @param[in] param Pointer to the parameters of the message.
 * @param[in] dest_id ID of the receiving task instance (probably unused).
 * @param[in] src_id ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int bleuarts_enable_req_handler(ke_msg_id_t const msgid,
                                   struct bleuarts_enable_req const *param,
                                   ke_task_id_t const dest_id,
                                   ke_task_id_t const src_id)
{
    uint8_t status = PRF_ERR_REQ_DISALLOWED;

    // check state of the task
    if(gapc_get_conhdl(param->conidx) != GAP_INVALID_CONHDL)
    {
        // restore Bond Data
        struct bleuarts_env_tag* bleuarts_env =  PRF_ENV_GET(BLEUARTS, bleuarts);
        bleuarts_env->ntf_ind_cfg[param->conidx] = param->ntf_ind_cfg;
        status = GAP_ERR_NO_ERROR;
    }

    // send response
    struct bleuarts_enable_rsp *rsp = KE_MSG_ALLOC(BLEUARTS_ENABLE_RSP, src_id, dest_id, bleuarts_enable_rsp);
    rsp->conidx = param->conidx;
    rsp->status = status;
    ke_msg_send(rsp);

    return (KE_MSG_CONSUMED);
}

static int bleuarts_notify_send_req_handler(ke_msg_id_t const msgid,
                                      struct bleuarts_ntf_ind_send_req const *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id)
{
    // Status
    int msg_status = KE_MSG_SAVED;
    uint8_t state = ke_state_get(dest_id);

    // check state of the task
    if(state == BLEUARTS_IDLE)
    {
        // Get the address of the environment
        struct bleuarts_env_tag *bleuarts_env = PRF_ENV_GET(BLEUARTS, bleuarts);

        // allocate operation to execute
        bleuarts_env->operation    = (struct bleuarts_op *) ke_malloc(sizeof(struct bleuarts_op) + BLEUARTS_REPORT_MAX_LEN, KE_MEM_ATT_DB);

        // Initialize operation parameters
        bleuarts_env->operation->cursor  = 0;
        bleuarts_env->operation->dest_id = src_id;
        bleuarts_env->operation->conidx  = GAP_INVALID_CONIDX;
        bleuarts_env->operation->op      = BLEUARTS_CFG_TX_NTF;
        bleuarts_env->operation->handle  = BLEUARTS_HANDLE(BLEUARTS_IDX_TX_VAL);
        bleuarts_env->operation->length  = param->lenth;

        memcpy(bleuarts_env->operation->data, param->eArray, param->lenth);

        // put task in a busy state
        ke_state_set(dest_id, BLEUARTS_BUSY);

        // execute operation
        bleuarts_exe_operation();

        msg_status = KE_MSG_CONSUMED;
    }

    return (msg_status);
}


static int bleuarts_indicate_send_req_handler(ke_msg_id_t const msgid,
                                      struct bleuarts_ntf_ind_send_req const *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id)
{
    // Status
    int msg_status = KE_MSG_SAVED;
    uint8_t state = ke_state_get(dest_id);

    // check state of the task
    if(state == BLEUARTS_IDLE)
    {
        // Get the address of the environment
        struct bleuarts_env_tag *bleuarts_env = PRF_ENV_GET(BLEUARTS, bleuarts);

        {
            // allocate operation to execute
            bleuarts_env->operation    = (struct bleuarts_op *) ke_malloc(sizeof(struct bleuarts_op) + BLEUARTS_REPORT_MAX_LEN, KE_MEM_ATT_DB);

            // Initialize operation parameters
            bleuarts_env->operation->cursor  = 0;
            bleuarts_env->operation->dest_id = src_id;
            bleuarts_env->operation->conidx  = GAP_INVALID_CONIDX;
            bleuarts_env->operation->op      = BLEUARTS_CFG_TX_IND;
            bleuarts_env->operation->handle  = BLEUARTS_HANDLE(BLEUARTS_IDX_TX_VAL);
            bleuarts_env->operation->length  = param->lenth;

            memcpy(bleuarts_env->operation->data, param->eArray, param->lenth);

            // put task in a busy state
            ke_state_set(dest_id, BLEUARTS_BUSY);

            // execute operation
            bleuarts_exe_operation();
        }

        msg_status = KE_MSG_CONSUMED;
    }

    return (msg_status);
}


/**
 ****************************************************************************************
 * @brief Handles reception of the @ref GL2C_CODE_ATT_WR_CMD_NFD message.
 * The handler compares the new values with current ones and notifies them if they changed.
 * @param[in] msgid Id of the message received (probably unused).
 * @param[in] param Pointer to the parameters of the message.
 * @param[in] dest_id ID of the receiving task instance (probably unused).
 * @param[in] src_id ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int gattc_write_req_ind_handler(ke_msg_id_t const msgid,
                                      struct gattc_write_req_ind const *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id)
{

    struct bleuarts_env_tag* bleuarts_env = PRF_ENV_GET(BLEUARTS, bleuarts);
    uint8_t conidx  = KE_IDX_GET(src_id);
    uint8_t status = ATT_ERR_NO_ERROR;
    int msg_status = KE_MSG_CONSUMED;

    // to check if confirmation message should be send
    bool send_cfm = true;

    // retrieve handle information
    uint8_t att_idx = BLEUARTS_IDX(param->handle);

    switch(att_idx)
    {
		case BLEUARTS_IDX_RX_VAL:
        {
             uint8_t state = ke_state_get(dest_id);
             send_cfm = true;

             // check state of the task to know if it can be proceed immediately
             if(state == BLEUARTS_IDLE)
             {
                     // inform application that update of measurement interval is requested by peer device.
                     struct bleuarts_packet_send_cmd * req_ind = KE_MSG_ALLOC(BLEUARTS_PACKET_SEND_CMD,
                             prf_dst_task_get(&bleuarts_env->prf_env, conidx), dest_id, bleuarts_packet_send_cmd);

                     req_ind->length =  param->length;
                     memcpy(req_ind->value, param->value, param->length);
                     ke_msg_send(req_ind);
			}
			else
			{
				msg_status = KE_MSG_SAVED;
			}  

         }break;

         case BLEUARTS_IDX_TX_NTF_CFG:
         {
			status = bleuarts_update_ntf_ind_cfg(conidx, BLEUARTS_CFG_TX_NTF, PRF_CLI_START_NTF, co_read16p(param->value));
			//status = bleuarts_update_ntf_ind_cfg(conidx, BLEUARTS_CFG_REPORT_IND, PRF_CLI_START_IND, co_read16p(param->value));
         }break;

         default:
         {
			status = ATT_ERR_REQUEST_NOT_SUPPORTED;
         }break;
    }
    

    if(send_cfm)
    {
        //Send write response
        struct gattc_write_cfm * cfm = KE_MSG_ALLOC(GATTC_WRITE_CFM, src_id, dest_id, gattc_write_cfm);
        cfm->handle = param->handle;
        cfm->status = status;
        ke_msg_send(cfm);
    }

    return (msg_status);
}


/**
 ****************************************************************************************
 * @brief Handles reception of the @ref GATTC_READ_REQ_IND message.
 * @param[in] msgid Id of the message received (probably unused).
 * @param[in] param Pointer to the parameters of the message.
 * @param[in] dest_id ID of the receiving task instance (probably unused).
 * @param[in] src_id ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int gattc_read_req_ind_handler(ke_msg_id_t const msgid, struct gattc_write_req_ind const *param,
                                      ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    struct bleuarts_env_tag* bleuarts_env = PRF_ENV_GET(BLEUARTS, bleuarts);
    uint8_t conidx  = KE_IDX_GET(src_id);
    uint8_t value[BLEUARTS_REPORT_MAX_LEN];
    uint8_t value_size = 0;
    uint8_t status = ATT_ERR_NO_ERROR;

    // retrieve handle information
    uint8_t att_idx = BLEUARTS_IDX(param->handle);

    switch(att_idx)
    {
/*
        case BLEUARTS_IDX_TX_VAL:
        {
            bleuarts_packet_send_reply_handler(value, &value_size);
        }break;
*/

        case BLEUARTS_IDX_TX_NTF_CFG:
        {
            uint16_t ntf_ind_cfg = 0;
            
            value_size = BLEUARTS_IND_NTF_CFG_MAX_LEN;

            if((bleuarts_env->ntf_ind_cfg[conidx] & BLEUARTS_CFG_TX_NTF) != 0)
                ntf_ind_cfg |= PRF_CLI_START_NTF;

            if((bleuarts_env->ntf_ind_cfg[conidx] & BLEUARTS_CFG_TX_IND) != 0)
                ntf_ind_cfg |= PRF_CLI_START_IND;

            co_write16p(value, ntf_ind_cfg);            

        }break;

        default:
        {
            status = ATT_ERR_REQUEST_NOT_SUPPORTED;
        }break;
    }

    // Send data to peer device
    struct gattc_read_cfm* cfm = KE_MSG_ALLOC_DYN(GATTC_READ_CFM, src_id, dest_id, gattc_read_cfm, value_size);
    cfm->length = value_size;
    memcpy(cfm->value, value, value_size);
    cfm->handle = param->handle;
    cfm->status = status;

    // Send value to peer device.
    ke_msg_send(cfm);

    return (KE_MSG_CONSUMED);
}



/**
 ****************************************************************************************
 * @brief Handles @ref GATTC_CMP_EVT for GATTC_NOTIFY and GATT_INDICATE message meaning
 * that Measurement notification/indication has been correctly sent to peer device
 *
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance
 * @param[in] src_id    ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int gattc_cmp_evt_handler(ke_msg_id_t const msgid, struct gattc_cmp_evt const *param,
                                 ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    // continue operation execution
    bleuarts_exe_operation();

    return (KE_MSG_CONSUMED);
}

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

/// Default State handlers definition
const struct ke_msg_handler bleuarts_default_state[] =
{
    {BLEUARTS_ENABLE_REQ,			(ke_msg_func_t) bleuarts_enable_req_handler}, 

    {GATTC_WRITE_REQ_IND,			(ke_msg_func_t) gattc_write_req_ind_handler},  
    {GATTC_READ_REQ_IND,			(ke_msg_func_t) gattc_read_req_ind_handler},
    {GATTC_CMP_EVT,					(ke_msg_func_t) gattc_cmp_evt_handler},

    {BLEUARTS_CTRL_PT_SEND_NOTIFY,   (ke_msg_func_t) bleuarts_notify_send_req_handler}, 
    {BLEUARTS_CTRL_PT_SEND_INDICATE, (ke_msg_func_t) bleuarts_indicate_send_req_handler},
};


///Specifies the message handlers that are common to all states.
const struct ke_state_handler bleuarts_default_handler = KE_STATE_HANDLER(bleuarts_default_state);

#endif //BLE_BLEUARTS_SERVER

/// @} BLEUARTSTASK
