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
 * The handler enables the Health Thermometer Profile Thermometer Role.
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


/**
 ****************************************************************************************
 * @brief Handles reception of the @ref CCPS_TEMP_SEND_REQ message.
 * @param[in] msgid Id of the message received (probably unused).
 * @param[in] param Pointer to the parameters of the message.
 * @param[in] dest_id ID of the receiving task instance (probably unused).
 * @param[in] src_id ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
 /*
static int ccps_temp_send_req_handler(ke_msg_id_t const msgid,
                                      struct ccps_temp_send_req const *param,
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

        // for intermediate measurement, feature must be enabled
        if(!(param->stable_meas) && (!CCPS_IS_FEATURE_SUPPORTED(ccps_env->features, CCPS_INTERM_TEMP_CHAR_SUP)))
        {
            struct ccps_temp_send_rsp *rsp = KE_MSG_ALLOC(CCPS_TEMP_SEND_RSP, src_id, dest_id, ccps_temp_send_rsp);
            rsp->status = PRF_ERR_FEATURE_NOT_SUPPORTED;
            ke_msg_send(rsp);
        }
        else
        {
            // allocate operation to execute
            ccps_env->operation    = (struct ccps_op *) ke_malloc(sizeof(struct ccps_op) + CCPS_TEMP_MEAS_MAX_LEN, KE_MEM_ATT_DB);

            // Initialize operation parameters
            ccps_env->operation->cursor  = 0;
            ccps_env->operation->dest_id = src_id;
            ccps_env->operation->conidx  = GAP_INVALID_CONIDX;

            // Stable measurement indication or intermediate measurement notification
            if(param->stable_meas)
            {
                ccps_env->operation->op      = CCPS_CFG_STABLE_MEAS_IND;
                ccps_env->operation->handle  = CCPS_HANDLE(HTS_IDX_TEMP_MEAS_VAL);
            }
            else
            {
                ccps_env->operation->op      = CCPS_CFG_INTERM_MEAS_NTF;
                ccps_env->operation->handle  = CCPS_HANDLE(HTS_IDX_INTERM_TEMP_VAL);
            }

            //Pack the temperature measurement value
            ccps_env->operation->length  = ccps_pack_temp_value(&(ccps_env->operation->data[0]), param->temp_meas);

            // put task in a busy state
            ke_state_set(dest_id, CCPS_BUSY);

            // execute operation
            ccps_exe_operation();
        }

        msg_status = KE_MSG_CONSUMED;
    }

    return (msg_status);
}
*/


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

#if 0
        // for intermediate measurement, feature must be enabled
        if(!(param->stable_meas) && (!CCPS_IS_FEATURE_SUPPORTED(ccps_env->features, CCPS_INTERM_TEMP_CHAR_SUP)))
        {
            struct ccps_temp_send_rsp *rsp = KE_MSG_ALLOC(CCPS_TEMP_SEND_RSP, src_id, dest_id, ccps_temp_send_rsp);
            rsp->status = PRF_ERR_FEATURE_NOT_SUPPORTED;
            ke_msg_send(rsp);
        }
        else
#endif            
        {
            // allocate operation to execute
            ccps_env->operation    = (struct ccps_op *) ke_malloc(sizeof(struct ccps_op) + CCPS_NOTIFY_SEND_MAX_LEN, KE_MEM_ATT_DB);

            // Initialize operation parameters
            ccps_env->operation->cursor  = 0;
            ccps_env->operation->dest_id = src_id;
            ccps_env->operation->conidx  = GAP_INVALID_CONIDX;
            ccps_env->operation->op      = CCPS_CFG_REPORT_NTF;
            ccps_env->operation->handle  = CCPS_HANDLE(CCPS_IDX_REPORT_VAL);
            ccps_env->operation->length  = param->lenth;//sizeof(struct ccps_notify_send_req);

            memcpy(ccps_env->operation->data, param->eArray, param->lenth);

            //Pack the temperature measurement value
            //ccps_env->operation->length  = ccps_pack_temp_value(&(ccps_env->operation->data[0]), param->temp_meas);

            // put task in a busy state
            ke_state_set(dest_id, CCPS_BUSY);

            // execute operation
            ccps_exe_operation();
        }

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
            ccps_env->operation    = (struct ccps_op *) ke_malloc(sizeof(struct ccps_op) + CCPS_NOTIFY_SEND_MAX_LEN, KE_MEM_ATT_DB);

            // Initialize operation parameters
            ccps_env->operation->cursor  = 0;
            ccps_env->operation->dest_id = src_id;
            ccps_env->operation->conidx  = GAP_INVALID_CONIDX;
            ccps_env->operation->op      = CCPS_CFG_REPORT_IND;
            ccps_env->operation->handle  = CCPS_HANDLE(CCPS_IDX_REPORT_VAL);
            ccps_env->operation->length  = param->lenth;//sizeof(struct ccps_notify_send_req);

            memcpy(ccps_env->operation->data, param->eArray, param->lenth);

            //Pack the temperature measurement value
            //ccps_env->operation->length  = ccps_pack_temp_value(&(ccps_env->operation->data[0]), param->temp_meas);

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
static int ccps_period_meas_send_req_handler(ke_msg_id_t const msgid,
                                      struct ccps_period_meas_send_req const *param,
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

#if 0
        // for intermediate measurement, feature must be enabled
        if(!(param->stable_meas) && (!CCPS_IS_FEATURE_SUPPORTED(ccps_env->features, CCPS_INTERM_TEMP_CHAR_SUP)))
        {
            struct ccps_temp_send_rsp *rsp = KE_MSG_ALLOC(CCPS_TEMP_SEND_RSP, src_id, dest_id, ccps_temp_send_rsp);
            rsp->status = PRF_ERR_FEATURE_NOT_SUPPORTED;
            ke_msg_send(rsp);
        }
        else
#endif            
        {
            // allocate operation to execute
            ccps_env->operation    = (struct ccps_op *) ke_malloc(sizeof(struct ccps_op) + CCPS_TEMP_MEAS_MAX_LEN, KE_MEM_ATT_DB);

            // Initialize operation parameters
            ccps_env->operation->cursor  = 0;
            ccps_env->operation->dest_id = src_id;
            ccps_env->operation->conidx  = GAP_INVALID_CONIDX;

            ccps_env->operation->op      = CCPS_CFG_INTERM_MEAS_NTF;
            ccps_env->operation->handle  = CCPS_HANDLE(HTS_IDX_INTERM_TEMP_VAL);
            
/*
            // Stable measurement indication or intermediate measurement notification
            if(param->stable_meas)
            {
                ccps_env->operation->op      = CCPS_CFG_STABLE_MEAS_IND;
                ccps_env->operation->handle  = CCPS_HANDLE(HTS_IDX_TEMP_MEAS_VAL);
            }
            else
            {
                ccps_env->operation->op      = CCPS_CFG_INTERM_MEAS_NTF;
                ccps_env->operation->handle  = CCPS_HANDLE(HTS_IDX_INTERM_TEMP_VAL);
            }
*/
            ccps_env->operation->length  = sizeof(struct htp_period_meas);

            memcpy(ccps_env->operation->data, &param->period_meas, sizeof(struct htp_period_meas));

            //Pack the temperature measurement value
            //ccps_env->operation->length  = ccps_pack_temp_value(&(ccps_env->operation->data[0]), param->temp_meas);

            // put task in a busy state
            ke_state_set(dest_id, CCPS_BUSY);

            // execute operation
            ccps_exe_operation();
        }

        msg_status = KE_MSG_CONSUMED;
    }

    return (msg_status);
}


static int ccps_swim_meas_send_req_handler(ke_msg_id_t const msgid,
                                      struct ccps_swim_meas_send_req const *param,
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

        if(1)    
        {
            // allocate operation to execute
            ccps_env->operation    = (struct ccps_op *) ke_malloc(sizeof(struct ccps_op) + CCPS_TEMP_MEAS_MAX_LEN, KE_MEM_ATT_DB);

            // Initialize operation parameters
            ccps_env->operation->cursor  = 0;
            ccps_env->operation->dest_id = src_id;
            ccps_env->operation->conidx  = GAP_INVALID_CONIDX;

            ccps_env->operation->op      = CCPS_CFG_INTERM_MEAS_NTF;
            ccps_env->operation->handle  = CCPS_HANDLE(HTS_IDX_INTERM_TEMP_VAL);
            
            ccps_env->operation->length  = sizeof(struct htp_swim_meas);

            memcpy(ccps_env->operation->data, &param->swim_meas, sizeof(struct htp_swim_meas));
                
            //Pack the temperature measurement value
            //ccps_env->operation->length  = ccps_pack_temp_value(&(ccps_env->operation->data[0]), param->temp_meas);

            // put task in a busy state
            ke_state_set(dest_id, CCPS_BUSY);

            // execute operation
            ccps_exe_operation();
        }

        msg_status = KE_MSG_CONSUMED;
    }

    return (msg_status);
}
#endif
/**
 ****************************************************************************************
 * @brief Request to update Measurement Interval Value
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance
 * @param[in] src_id    ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */

 #if 0
static int ccps_meas_intv_upd_req_handler(ke_msg_id_t const msgid,
                                          struct ccps_meas_intv_upd_req const *param,
                                          ke_task_id_t const dest_id,
                                          ke_task_id_t const src_id)
{
    int msg_status = KE_MSG_SAVED;
    uint8_t state = ke_state_get(dest_id);

    // check state of the task
    if(state == CCPS_IDLE)
    {
        // Get the address of the environment
        struct ccps_env_tag *ccps_env = PRF_ENV_GET(CCPS, ccps);

        // update measurement interval
        ccps_env->meas_intv = param->meas_intv;

        //Check if Measurement Interval indication is supported
        if(!CCPS_IS_FEATURE_SUPPORTED(ccps_env->features, CCPS_MEAS_INTV_CHAR_SUP))
        {
            struct ccps_meas_intv_upd_rsp *rsp = KE_MSG_ALLOC(CCPS_MEAS_INTV_UPD_RSP, src_id, dest_id, ccps_meas_intv_upd_rsp);
            rsp->status = PRF_ERR_FEATURE_NOT_SUPPORTED;
            ke_msg_send(rsp);
        }
        else
        {
            // update internal measurement interval value
            ccps_env->meas_intv = param->meas_intv;

            // no indication to trigger
            if(!CCPS_IS_FEATURE_SUPPORTED(ccps_env->features, CCPS_MEAS_INTV_IND_SUP))
            {
                struct ccps_meas_intv_upd_rsp *rsp = KE_MSG_ALLOC(CCPS_MEAS_INTV_UPD_RSP, src_id, dest_id, ccps_meas_intv_upd_rsp);
                rsp->status = GAP_ERR_NO_ERROR;
                ke_msg_send(rsp);
            }
            // trigger measurement update indication
            else
            {
                // allocate operation to execute
                ccps_env->operation    = (struct ccps_op *) ke_malloc(sizeof(struct ccps_op) + CCPS_MEAS_INTV_MAX_LEN, KE_MEM_ATT_DB);

                // Initialize operation parameters
                ccps_env->operation->op      = CCPS_CFG_MEAS_INTV_IND;
                ccps_env->operation->handle  = CCPS_HANDLE(HTS_IDX_MEAS_INTV_VAL);
                ccps_env->operation->dest_id = src_id;
                ccps_env->operation->cursor  = 0;
                ccps_env->operation->conidx  = GAP_INVALID_CONIDX;

                // Pack the interval value
                ccps_env->operation->length = CCPS_MEAS_INTV_MAX_LEN;
                co_write16p(ccps_env->operation->data, param->meas_intv);

                // put task in a busy state
                ke_state_set(dest_id, CCPS_BUSY);

                // execute operation
                ccps_exe_operation();
            }
        }

        msg_status = KE_MSG_CONSUMED;
    }

    return (msg_status);
}

/**
 ****************************************************************************************
 * @brief Handles reception of the attribute info request message.
 *
 * @param[in] msgid Id of the message received (probably unused).
 * @param[in] param Pointer to the parameters of the message.
 * @param[in] dest_id ID of the receiving task instance (probably unused).
 * @param[in] src_id ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int gattc_att_info_req_ind_handler(ke_msg_id_t const msgid,
        struct gattc_att_info_req_ind *param,
        ke_task_id_t const dest_id,
        ke_task_id_t const src_id)
{
    struct ccps_env_tag* ccps_env = PRF_ENV_GET(CCPS, ccps);
    uint8_t att_idx = CCPS_IDX(param->handle);
    struct gattc_att_info_cfm * cfm;

    //Send write response
    cfm = KE_MSG_ALLOC(GATTC_ATT_INFO_CFM, src_id, dest_id, gattc_att_info_cfm);
    cfm->handle = param->handle;

    switch(att_idx)
    {
        case HTS_IDX_MEAS_INTV_VAL:
        {
            // force length to zero to reject any write starting from something != 0
            cfm->length = 0;
            cfm->status = GAP_ERR_NO_ERROR;
        }break;

        case HTS_IDX_TEMP_MEAS_IND_CFG:
        case HTS_IDX_INTERM_TEMP_CFG:
        case HTS_IDX_MEAS_INTV_CFG:
        {
            cfm->length = CCPS_IND_NTF_CFG_MAX_LEN;
            cfm->status = GAP_ERR_NO_ERROR;
        }break;

        default:
        {
            cfm->status = ATT_ERR_REQUEST_NOT_SUPPORTED;
        }break;
    }

    ke_msg_send(cfm);

    return (KE_MSG_CONSUMED);
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
             //TracerInfo("CCPS_IDX_REPORT_PKT_VAL\r\n");
             
             // check state of the task to know if it can be proceed immediately
             if(state == CCPS_IDLE)
             {
                 // inform application that update of measurement interval is requested by peer device.
                 struct ccps_packet_send_cmd * req_ind = KE_MSG_ALLOC(CCPS_PACKET_SEND_CMD,
                         prf_dst_task_get(&ccps_env->prf_env, conidx), dest_id, ccps_packet_send_cmd);
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
 #include "tracer.h"
static int gattc_read_req_ind_handler(ke_msg_id_t const msgid, struct gattc_write_req_ind const *param,
                                      ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    struct ccps_env_tag* ccps_env = PRF_ENV_GET(CCPS, ccps);
    uint8_t conidx  = KE_IDX_GET(src_id);
    uint8_t value[CCPS_NOTIFY_SEND_MAX_LEN];
    uint8_t value_size = 0;
    uint8_t status = ATT_ERR_NO_ERROR;

    // retrieve handle information
    uint8_t att_idx = CCPS_IDX(param->handle);

    switch(att_idx)
    {

        case CCPS_IDX_REPORT_NTF_CFG:
        {
            uint16_t ntf_ind_cfg = 0;
            
            value_size = CCPS_IND_NTF_CFG_MAX_LEN;

            if((ccps_env->ntf_ind_cfg[conidx] & CCPS_CFG_REPORT_NTF) != 0)
                ntf_ind_cfg |= PRF_CLI_START_NTF;

            if((ccps_env->ntf_ind_cfg[conidx] & CCPS_CFG_REPORT_IND) != 0)
                ntf_ind_cfg |= PRF_CLI_START_IND;

            co_write16p(value, ntf_ind_cfg);            
            //co_write16p(value, ((ccps_env->ntf_ind_cfg[conidx] & CCPS_CFG_REPORT_NTF) != 0) ? PRF_CLI_START_NTF : PRF_CLI_STOP_NTFIND);

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

#if 0
/**
 ****************************************************************************************
 * @brief Handles reception of the @ref CCPS_MEAS_INTV_UPD_CFM message.
 * @param[in] msgid Id of the message received (probably unused).
 * @param[in] param Pointer to the parameters of the message.
 * @param[in] dest_id ID of the receiving task instance (probably unused).
 * @param[in] src_id ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int ccps_meas_intv_chg_cfm_handler(ke_msg_id_t const msgid, struct ccps_meas_intv_chg_cfm const *param,
                                      ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    struct ccps_env_tag* ccps_env = PRF_ENV_GET(CCPS, ccps);

    uint8_t state = ke_state_get(dest_id);

    // check state of the task
    if(state == CCPS_BUSY)
    {
        // retrieve connection index from operation
        uint8_t conidx = ccps_env->operation->conidx;

        //Send write response
        struct gattc_write_cfm * cfm = KE_MSG_ALLOC(GATTC_WRITE_CFM,
                KE_BUILD_ID(TASK_GATTC, conidx), dest_id, gattc_write_cfm);
        cfm->handle = CCPS_HANDLE(HTS_IDX_MEAS_INTV_VAL);
        cfm->status = (param->conidx == conidx) ? param->status : PRF_APP_ERROR;
        ke_msg_send(cfm);


        // check if no error occurs
        if(cfm->status == GAP_ERR_NO_ERROR)
        {
            // update the current measurement interval
            ccps_env->meas_intv = co_read16p(ccps_env->operation->data);

            // check if an indication of new measurement interval should be triggered
            if(CCPS_IS_FEATURE_SUPPORTED(ccps_env->features, CCPS_MEAS_INTV_IND_SUP))
            {
                // set back cursor to zero in order to send indication
                ccps_env->operation->cursor  = 0;
            }
        }

        // send indication or terminate operation
        ccps_exe_operation();
    }

    return (KE_MSG_CONSUMED);
}

#endif


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

 // Must!
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
    {CCPS_ENABLE_REQ,            (ke_msg_func_t) ccps_enable_req_handler}, // not used??

    //{GATTC_ATT_INFO_REQ_IND,     (ke_msg_func_t) gattc_att_info_req_ind_handler},
    {GATTC_WRITE_REQ_IND,        (ke_msg_func_t) gattc_write_req_ind_handler},    // sent from remote
    {GATTC_READ_REQ_IND,         (ke_msg_func_t) gattc_read_req_ind_handler},     // sent from remote
    {GATTC_CMP_EVT,              (ke_msg_func_t) gattc_cmp_evt_handler},

    {CCPS_CTRL_PT_SEND_NOTIFY,   (ke_msg_func_t) ccps_notify_send_req_handler}, // sent from local
    {CCPS_CTRL_PT_SEND_INDICATE, (ke_msg_func_t) ccps_indicate_send_req_handler},
};


///Specifies the message handlers that are common to all states.
const struct ke_state_handler ccps_default_handler = KE_STATE_HANDLER(ccps_default_state);

#endif //BLE_HT_THERMOM

/// @} CCPSTASK
