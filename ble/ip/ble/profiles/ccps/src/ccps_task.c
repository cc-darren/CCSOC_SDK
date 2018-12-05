/* Copyright (c) 2018 Cloudchip, Inc. All Rights Reserved.
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
 * @file ccps_task.c
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
 * @addtogroup CCPSTASK
 * @{
 ****************************************************************************************
 */


/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "rwip_config.h"

#if (BLE_CCPS_SERVER)

#include "gap.h"
#include "gattc_task.h"
#include "attm.h"
#include "ccps.h"
#include "ccps_task.h"
#include "prf_utils.h"
#include "ke_mem.h"
#include "co_utils.h"
#include "project.h"
#include "tracer.h"
#include "app_ota.h"

#include "tracer.h"
#include "app_ccps.h"
/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */



/*
 * History function DEFINITIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Handles reception of the @ref CCPS_ENABLE_REQ message.
 * The handler enables the CCPS Role.
 * @param[in] msgid Id of the message received (probably unused).
 * @param[in] param Pointer to the parameters of the message.
 * @param[in] dest_id ID of the receiving task instance (probably unused).
 * @param[in] src_id ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int ccps_enable_req_handler(ke_msg_id_t const msgid,
                                   struct ccps_enable_req const *param,
                                   ke_task_id_t const dest_id,
                                   ke_task_id_t const src_id)
{
    uint8_t status = PRF_ERR_REQ_DISALLOWED;

    // check state of the task
    if(gapc_get_conhdl(param->conidx) != GAP_INVALID_CONHDL)
    {
        // restore Bond Data
        struct ccps_env_tag* ccps_env =  PRF_ENV_GET(CCPS, ccps);
        ccps_env->ntf_ind_cfg[param->conidx] = param->ntf_ind_cfg;
        status = GAP_ERR_NO_ERROR;

    }

    // send response
    struct ccps_enable_rsp *rsp = KE_MSG_ALLOC(CCPS_ENABLE_RSP, src_id, dest_id, ccps_enable_rsp);
    rsp->conidx = param->conidx;
    rsp->status = status;
    ke_msg_send(rsp);

    return (KE_MSG_CONSUMED);
}



static int ccps_notify_send_req_handler(ke_msg_id_t const msgid,
                                      struct ccps_ntf_ind_send_req const *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id)
{
    // Status
    int msg_status = KE_MSG_SAVED;
    uint8_t state = ke_state_get(dest_id);

    // check state of the task
    if(state == CCPS_IDLE)
    {
        // Get the address of the environment
        struct ccps_env_tag *ccps_env = PRF_ENV_GET(CCPS, ccps);

        // allocate operation to execute
        ccps_env->operation    = (struct ccps_op *) ke_malloc(sizeof(struct ccps_op) + CCPS_REPORT_MAX_LEN, KE_MEM_ATT_DB);

        // Initialize operation parameters
        ccps_env->operation->cursor  = 0;
        ccps_env->operation->dest_id = src_id;
        ccps_env->operation->conidx  = GAP_INVALID_CONIDX;
        ccps_env->operation->op      = CCPS_CFG_REPORT_NTF;
        ccps_env->operation->handle  = CCPS_HANDLE(CCPS_IDX_REPORT_VAL);
        ccps_env->operation->length  = param->lenth;

        memcpy(ccps_env->operation->data, param->eArray, param->lenth);

        // put task in a busy state
        ke_state_set(dest_id, CCPS_BUSY);

        // execute operation
        ccps_exe_operation();

        msg_status = KE_MSG_CONSUMED;
    }

    return (msg_status);
}


static int ccps_indicate_send_req_handler(ke_msg_id_t const msgid,
                                      struct ccps_ntf_ind_send_req const *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id)
{
    // Status
    int msg_status = KE_MSG_SAVED;
    uint8_t state = ke_state_get(dest_id);

    // check state of the task
    if(state == CCPS_IDLE)
    {
        // Get the address of the environment
        struct ccps_env_tag *ccps_env = PRF_ENV_GET(CCPS, ccps);

        {
            // allocate operation to execute
            ccps_env->operation    = (struct ccps_op *) ke_malloc(sizeof(struct ccps_op) + CCPS_REPORT_MAX_LEN, KE_MEM_ATT_DB);

            // Initialize operation parameters
            ccps_env->operation->cursor  = 0;
            ccps_env->operation->dest_id = src_id;
            ccps_env->operation->conidx  = GAP_INVALID_CONIDX;
            ccps_env->operation->op      = CCPS_CFG_REPORT_IND;
            ccps_env->operation->handle  = CCPS_HANDLE(CCPS_IDX_REPORT_VAL);
            ccps_env->operation->length  = param->lenth;

            memcpy(ccps_env->operation->data, param->eArray, param->lenth);

            // put task in a busy state
            ke_state_set(dest_id, CCPS_BUSY);

            // execute operation
            ccps_exe_operation();
        }

        msg_status = KE_MSG_CONSUMED;
    }

    return (msg_status);
}

#if 0
static void _ccps_send_gack(void)
{
    
    // Get the address of the environment
    struct ccps_env_tag *ccps_env = PRF_ENV_GET(CCPS, ccps);
    
    // allocate operation to execute
    ccps_env->operation    = (struct ccps_op *) ke_malloc(sizeof(struct ccps_op) + CCPS_REPORT_MAX_LEN, KE_MEM_ATT_DB);
    
    // Initialize operation parameters
    ccps_env->operation->cursor  = 0;
    ccps_env->operation->dest_id = TASK_APP;
    ccps_env->operation->conidx  = GAP_INVALID_CONIDX;
    ccps_env->operation->op      = CCPS_CFG_REPORT_NTF;
    ccps_env->operation->handle  = CCPS_HANDLE(CCPS_IDX_REPORT_VAL);
    ccps_env->operation->length  = (CCPS_PKT_HEADER_LENGTH_SIZE + CCPS_PKT_HEADER_TYPE_SIZE + CCPS_PKT_HEADER_OPCODE_SIZE);

    // g-ack content
    *((uint16_t *) &(ccps_env->operation->data[CCPS_PKT_HEADER_LENGTH_POSITION])) = (CCPS_PKT_HEADER_TYPE_SIZE + CCPS_PKT_HEADER_OPCODE_SIZE);
    ccps_env->operation->data[CCPS_PKT_HEADER_TYPE_POSITION]                      = ccps_env->tRcvReportPktCB.pbData[CCPS_PKT_HEADER_TYPE_POSITION];
    ccps_env->operation->data[CCPS_PKT_HEADER_OPCODE_POSITION]                    = CCPS_GACK(ccps_env->tRcvReportPktCB.pbData[CCPS_PKT_HEADER_OPCODE_POSITION]);

    // put task in a busy state
    ke_state_set(prf_get_task_from_id(TASK_ID_CCPS), CCPS_BUSY);
    
    // execute operation
    ccps_exe_operation();
}
#endif

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

    struct ccps_env_tag* ccps_env = PRF_ENV_GET(CCPS, ccps);
    uint8_t conidx  = KE_IDX_GET(src_id);
    uint8_t status = ATT_ERR_NO_ERROR;
    int msg_status = KE_MSG_CONSUMED;

    // to check if confirmation message should be send
    bool send_cfm = true;

    // retrieve handle information
    uint8_t att_idx = CCPS_IDX(param->handle);


    switch(att_idx)
    {

         case CCPS_IDX_REPORT_VAL:
         {
             uint8_t state = ke_state_get(dest_id);
             send_cfm = true;
             
             // check state of the task to know if it can be proceed immediately
             if(state == CCPS_IDLE)
             {
                 #ifdef CFG_SDK_CCPS_EN
                     // inform application that update of measurement interval is requested by peer device.
                     struct ccps_packet_send_cmd * req_ind = KE_MSG_ALLOC(CCPS_PACKET_SEND_CMD,
                             prf_dst_task_get(&ccps_env->prf_env, conidx), dest_id, ccps_packet_send_cmd);
                     
                     req_ind->length =  param->length;
                     memcpy(req_ind->value, param->value, param->length);
                     ke_msg_send(req_ind);
                 #else
                     if (0 == ccps_env->tRcvReportPktCB.wPktTotalSize)
                     {
                         S_CcpsGenericHeader   *_ptRcvPkt = ((S_CcpsGenericHeader *) param->value);
                     
                         ccps_env->tRcvReportPktCB.wPktTotalSize = (CCPS_PKT_HEADER_LENGTH_SIZE + _ptRcvPkt->wLength);
                         ccps_env->tRcvReportPktCB.wWriteIndex   = 0;
                         ccps_env->tRcvReportPktCB.pbData        = ((uint8_t *) ke_malloc(ccps_env->tRcvReportPktCB.wPktTotalSize, KE_MEM_ATT_DB));
                     }

                     {
                         uint16_t    _wLength = param->length;

                         if ((ccps_env->tRcvReportPktCB.wWriteIndex + _wLength) > ccps_env->tRcvReportPktCB.wPktTotalSize)
                             _wLength = (ccps_env->tRcvReportPktCB.wPktTotalSize - ccps_env->tRcvReportPktCB.wWriteIndex);

                         if ((ccps_env->tRcvReportPktCB.pbData) && (_wLength))
                         {
                             memcpy(  &ccps_env->tRcvReportPktCB.pbData[ccps_env->tRcvReportPktCB.wWriteIndex]
                                    , param->value
                                    , _wLength                                                              );
    
                         }

                         ccps_env->tRcvReportPktCB.wWriteIndex += _wLength;
                     }

                     if (ccps_env->tRcvReportPktCB.wWriteIndex == ccps_env->tRcvReportPktCB.wPktTotalSize)
                     {
                         // send GACK to peer
                         //_ccps_send_gack();

                         // signal upper layer than a complete packet is received
                         {
                             struct ccps_packet_send_cmd   *_ptReqIND = KE_MSG_ALLOC(  CCPS_PACKET_SEND_CMD
                                                                                     , prf_dst_task_get(&ccps_env->prf_env, conidx)
                                                                                     , dest_id
                                                                                     , ccps_packet_send_cmd                        );
                             
                             _ptReqIND->length = ccps_env->tRcvReportPktCB.wPktTotalSize;
    
                             memcpy(_ptReqIND->value, ccps_env->tRcvReportPktCB.pbData, ccps_env->tRcvReportPktCB.wPktTotalSize);
    
                             ke_msg_send(_ptReqIND);
                         }

                         // reset rx packet control block
                         ke_free(ccps_env->tRcvReportPktCB.pbData);
                         memset(&(ccps_env->tRcvReportPktCB), 0, sizeof(ccps_env->tRcvReportPktCB));
                     }
                 #endif
             }
             else
             {
                 msg_status = KE_MSG_SAVED;
             }  

         }break;

         case CCPS_IDX_IMAGE_VAL:
         {
             uint8_t state = ke_state_get(dest_id);
             send_cfm = true;
             
             // check state of the task to know if it can be proceed immediately
             if(state == CCPS_IDLE)
             {
                 // inform application that update of measurement interval is requested by peer device.
                 struct ccps_packet_send_image * req_ind = KE_MSG_ALLOC(CCPS_PACKET_SEND_IMAGE,
                         prf_dst_task_get(&ccps_env->prf_env, conidx), dest_id, ccps_packet_send_image);
                 
                 req_ind->length =  param->length;
                 memcpy(req_ind->value, param->value, param->length);
                 ke_msg_send(req_ind);
             }
             else
             {
                 msg_status = KE_MSG_SAVED;
             }  

         }break;


         case CCPS_IDX_REPORT_NTF_CFG:
         {
             status = ccps_update_ntf_ind_cfg(conidx, CCPS_CFG_REPORT_NTF, PRF_CLI_START_NTF, co_read16p(param->value));
             status = ccps_update_ntf_ind_cfg(conidx, CCPS_CFG_REPORT_IND, PRF_CLI_START_IND, co_read16p(param->value));

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
    struct ccps_env_tag* ccps_env = PRF_ENV_GET(CCPS, ccps);
    uint8_t conidx  = KE_IDX_GET(src_id);
    uint8_t value[CCPS_REPORT_MAX_LEN];
    uint8_t value_size = 0;
    uint8_t status = ATT_ERR_NO_ERROR;

    // retrieve handle information
    uint8_t att_idx = CCPS_IDX(param->handle);

    switch(att_idx)
    {

        case CCPS_IDX_REPORT_VAL:
        {
            ccps_packet_send_reply_handler(value, &value_size);
        
        }break;
        

        case CCPS_IDX_REPORT_NTF_CFG:
        {
            uint16_t ntf_ind_cfg = 0;
            
            value_size = CCPS_IND_NTF_CFG_MAX_LEN;

            if((ccps_env->ntf_ind_cfg[conidx] & CCPS_CFG_REPORT_NTF) != 0)
                ntf_ind_cfg |= PRF_CLI_START_NTF;

            if((ccps_env->ntf_ind_cfg[conidx] & CCPS_CFG_REPORT_IND) != 0)
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
    ccps_exe_operation();

    return (KE_MSG_CONSUMED);
}

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

/// Default State handlers definition
const struct ke_msg_handler ccps_default_state[] =
{
    {CCPS_ENABLE_REQ,            (ke_msg_func_t) ccps_enable_req_handler}, 

    {GATTC_WRITE_REQ_IND,        (ke_msg_func_t) gattc_write_req_ind_handler},  
    {GATTC_READ_REQ_IND,         (ke_msg_func_t) gattc_read_req_ind_handler},
    {GATTC_CMP_EVT,              (ke_msg_func_t) gattc_cmp_evt_handler},

    {CCPS_CTRL_PT_SEND_NOTIFY,   (ke_msg_func_t) ccps_notify_send_req_handler}, 
    {CCPS_CTRL_PT_SEND_INDICATE, (ke_msg_func_t) ccps_indicate_send_req_handler},
};


///Specifies the message handlers that are common to all states.
const struct ke_state_handler ccps_default_handler = KE_STATE_HANDLER(ccps_default_state);

#endif //BLE_HT_THERMOM

/// @} CCPSTASK
