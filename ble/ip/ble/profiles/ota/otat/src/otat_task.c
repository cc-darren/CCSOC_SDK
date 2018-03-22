/**
 ****************************************************************************************
 *
 * @file otat_task.c
 *
 * @brief Health Thermometer Profile Thermometer Task implementation.
 *
 * Copyright (C) RivieraWaves 2009-2016
 *
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup OTATTASK
 * @{
 ****************************************************************************************
 */


/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "rwip_config.h"

#if (BLE_OTA_TARGET)

#include "gap.h"
#include "gattc_task.h"
#include "attm.h"
#include "otat.h"
#include "otat_task.h"
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
 * @brief Handles reception of the @ref OTAT_ENABLE_REQ message.
 * The handler enables the Health Thermometer Profile Thermometer Role.
 * @param[in] msgid Id of the message received (probably unused).
 * @param[in] param Pointer to the parameters of the message.
 * @param[in] dest_id ID of the receiving task instance (probably unused).
 * @param[in] src_id ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int otat_enable_req_handler(ke_msg_id_t const msgid,
                                   struct otat_enable_req const *param,
                                   ke_task_id_t const dest_id,
                                   ke_task_id_t const src_id)
{
    uint8_t status = PRF_ERR_REQ_DISALLOWED;

    // check state of the task
    if(gapc_get_conhdl(param->conidx) != GAP_INVALID_CONHDL)
    {
        // restore Bond Data
        struct otat_env_tag* otat_env =  PRF_ENV_GET(OTAT, otat);
        otat_env->ntf_ind_cfg[param->conidx] = param->ntf_ind_cfg;
        status = GAP_ERR_NO_ERROR;

    }

    // send response
    struct otat_enable_rsp *rsp = KE_MSG_ALLOC(OTAT_ENABLE_RSP, src_id, dest_id, otat_enable_rsp);
    rsp->conidx = param->conidx;
    rsp->status = status;
    ke_msg_send(rsp);

    return (KE_MSG_CONSUMED);
}


/**
 ****************************************************************************************
 * @brief Handles reception of the @ref OTAT_TEMP_SEND_REQ message.
 * @param[in] msgid Id of the message received (probably unused).
 * @param[in] param Pointer to the parameters of the message.
 * @param[in] dest_id ID of the receiving task instance (probably unused).
 * @param[in] src_id ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
 /*
static int otat_temp_send_req_handler(ke_msg_id_t const msgid,
                                      struct otat_temp_send_req const *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id)
{
    // Status
    int msg_status = KE_MSG_SAVED;
    uint8_t state = ke_state_get(dest_id);

    // check state of the task
    if(state == OTAT_IDLE)
    {
        // Get the address of the environment
        struct otat_env_tag *otat_env = PRF_ENV_GET(OTAT, otat);

        // for intermediate measurement, feature must be enabled
        if(!(param->stable_meas) && (!OTAT_IS_FEATURE_SUPPORTED(otat_env->features, OTAT_INTERM_TEMP_CHAR_SUP)))
        {
            struct otat_temp_send_rsp *rsp = KE_MSG_ALLOC(OTAT_TEMP_SEND_RSP, src_id, dest_id, otat_temp_send_rsp);
            rsp->status = PRF_ERR_FEATURE_NOT_SUPPORTED;
            ke_msg_send(rsp);
        }
        else
        {
            // allocate operation to execute
            otat_env->operation    = (struct otat_op *) ke_malloc(sizeof(struct otat_op) + OTAT_TEMP_MEAS_MAX_LEN, KE_MEM_ATT_DB);

            // Initialize operation parameters
            otat_env->operation->cursor  = 0;
            otat_env->operation->dest_id = src_id;
            otat_env->operation->conidx  = GAP_INVALID_CONIDX;

            // Stable measurement indication or intermediate measurement notification
            if(param->stable_meas)
            {
                otat_env->operation->op      = OTAT_CFG_STABLE_MEAS_IND;
                otat_env->operation->handle  = OTAT_HANDLE(HTS_IDX_TEMP_MEAS_VAL);
            }
            else
            {
                otat_env->operation->op      = OTAT_CFG_INTERM_MEAS_NTF;
                otat_env->operation->handle  = OTAT_HANDLE(HTS_IDX_INTERM_TEMP_VAL);
            }

            //Pack the temperature measurement value
            otat_env->operation->length  = otat_pack_temp_value(&(otat_env->operation->data[0]), param->temp_meas);

            // put task in a busy state
            ke_state_set(dest_id, OTAT_BUSY);

            // execute operation
            otat_exe_operation();
        }

        msg_status = KE_MSG_CONSUMED;
    }

    return (msg_status);
}
*/


static int otat_notify_send_req_handler(ke_msg_id_t const msgid,
                                      struct otat_notify_send_req const *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id)
{
    // Status
    int msg_status = KE_MSG_SAVED;
    uint8_t state = ke_state_get(dest_id);

    // check state of the task
    if(state == OTAT_IDLE)
    {
        // Get the address of the environment
        struct otat_env_tag *otat_env = PRF_ENV_GET(OTAT, otat);


        // allocate operation to execute
        otat_env->operation    = (struct otat_op *) ke_malloc(sizeof(struct otat_op) + OTAT_NOTIFY_SEND_MAX_LEN, KE_MEM_ATT_DB);

        // Initialize operation parameters
        otat_env->operation->cursor  = 0;
        otat_env->operation->dest_id = src_id;
        otat_env->operation->conidx  = GAP_INVALID_CONIDX;

        otat_env->operation->op      = OTAT_CFG_OTA_NTF;
#ifdef BLE_OTA_BL_MODE_EN
        otat_env->operation->handle  = OTAT_HANDLE(OTAS_IDX_OTA_CTRL_PT_VAL);
#else
        otat_env->operation->handle  = OTAT_HANDLE(OTAS_IDX_OTA_VAL);
#endif            

        otat_env->operation->length  = param->lenth;

        memcpy(otat_env->operation->data, param->eArray, param->lenth);

        // put task in a busy state
        ke_state_set(dest_id, OTAT_BUSY);

        // execute operation
        otat_exe_operation();


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
 // write notification enabled handler 
 extern uint8_t test_flag;
static int gattc_write_req_ind_handler(ke_msg_id_t const msgid,
                                      struct gattc_write_req_ind const *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id)
{

    struct otat_env_tag* otat_env = PRF_ENV_GET(OTAT, otat);
    uint8_t conidx  = KE_IDX_GET(src_id);
    uint8_t status = ATT_ERR_NO_ERROR;
    int msg_status = KE_MSG_CONSUMED;

    // to check if confirmation message should be send
    bool send_cfm = true;

    // retrieve handle information
    uint8_t att_idx = OTAT_IDX(param->handle);


    switch(att_idx)
    {

#ifdef BLE_OTA_BL_MODE_EN
         case OTAS_IDX_OTA_PKT_VAL:
         {
                //app_ota_pkt_write_cmd(param);

                uint8_t state = ke_state_get(dest_id);
                send_cfm = true;
                //TracerInfo("OTAS_IDX_OTA_PKT_VAL\r\n");

                // check state of the task to know if it can be proceed immediately
                if(state == OTAT_IDLE)
                {
                    // inform application that update of measurement interval is requested by peer device.
                    struct otat_packet_send_cmd * req_ind = KE_MSG_ALLOC(OTAT_PACKET_SEND_CMD,
                            prf_dst_task_get(&otat_env->prf_env, conidx), dest_id, otat_packet_send_cmd);
                    req_ind->length =  param->length;
                    memcpy(req_ind->value, param->value, param->length);
                    ke_msg_send(req_ind);
                    
                }
                else
                {
                    msg_status = KE_MSG_SAVED;
                }               
               
         }break;

         case OTAS_IDX_OTA_CTRL_PT_VAL:
         {
                uint8_t state = ke_state_get(dest_id);
                //send_cfm = false;

                //app_ota_ctrl_pt_write(param); 
                // check state of the task to know if it can be proceed immediately
                if(state == OTAT_IDLE)
                {
                    // inform application that update of measurement interval is requested by peer device.
                    struct otat_packet_send_cmd * req_ind = KE_MSG_ALLOC(OTAT_CTRL_PT_SEND_REQ,
                            prf_dst_task_get(&otat_env->prf_env, conidx), dest_id, otat_packet_send_cmd);
                    req_ind->length =  param->length;
                    memcpy(req_ind->value, param->value, param->length);
                    ke_msg_send(req_ind);
                }
                else
                {
                    msg_status = KE_MSG_SAVED;
                }

                

         }break;

         case OTAS_IDX_OTA_CTRL_PT_NTF_CFG:
         {
                status = otat_update_ntf_ind_cfg(conidx, OTAT_CFG_OTA_NTF, PRF_CLI_START_NTF, co_read16p(param->value));
         }break;

#else
         case OTAS_IDX_OTA_VAL:
         {
             uint8_t state = ke_state_get(dest_id);
             send_cfm = true;
             //TracerInfo("OTAS_IDX_OTA_PKT_VAL\r\n");
             
             // check state of the task to know if it can be proceed immediately
             if(state == OTAT_IDLE)
             {
                 // inform application that update of measurement interval is requested by peer device.
                 struct otat_packet_send_cmd * req_ind = KE_MSG_ALLOC(OTAT_PACKET_SEND_CMD,
                         prf_dst_task_get(&otat_env->prf_env, conidx), dest_id, otat_packet_send_cmd);
                 req_ind->length =  param->length;
                 memcpy(req_ind->value, param->value, param->length);
                 ke_msg_send(req_ind);
                 
             }
             else
             {
                 msg_status = KE_MSG_SAVED;
             }  

         }break;

         case OTAS_IDX_OTA_NTF_CFG:
         {
                status = otat_update_ntf_ind_cfg(conidx, OTAT_CFG_OTA_NTF, PRF_CLI_START_NTF, co_read16p(param->value));
         }break;
#endif

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
    struct otat_env_tag* otat_env = PRF_ENV_GET(OTAT, otat);
    uint8_t conidx  = KE_IDX_GET(src_id);
    uint8_t value[OTAT_NOTIFY_SEND_MAX_LEN];
    uint8_t value_size = 0;
    uint8_t status = ATT_ERR_NO_ERROR;

    // retrieve handle information
    uint8_t att_idx = OTAT_IDX(param->handle);

    switch(att_idx)
    {
#ifdef BLE_OTA_BL_MODE_EN
        case OTAS_IDX_OTA_CTRL_PT_NTF_CFG:
        {
            value_size = OTAT_IND_NTF_CFG_MAX_LEN;
            co_write16p(value, ((otat_env->ntf_ind_cfg[conidx] & OTAT_CFG_OTA_NTF) != 0) ? PRF_CLI_START_NTF : PRF_CLI_STOP_NTFIND);

        }break;

#else
        case OTAS_IDX_OTA_NTF_CFG:
        {
            value_size = OTAT_IND_NTF_CFG_MAX_LEN;
            co_write16p(value, ((otat_env->ntf_ind_cfg[conidx] & OTAT_CFG_OTA_NTF) != 0) ? PRF_CLI_START_NTF : PRF_CLI_STOP_NTFIND);

        }break;
#endif        

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

 // Must!
static int gattc_cmp_evt_handler(ke_msg_id_t const msgid, struct gattc_cmp_evt const *param,
                                 ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    // continue operation execution
    otat_exe_operation();

    return (KE_MSG_CONSUMED);
}

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

/// Default State handlers definition
const struct ke_msg_handler otat_default_state[] =
{
    {OTAT_ENABLE_REQ,            (ke_msg_func_t) otat_enable_req_handler}, 

    {GATTC_WRITE_REQ_IND,        (ke_msg_func_t) gattc_write_req_ind_handler},
    {GATTC_READ_REQ_IND,         (ke_msg_func_t) gattc_read_req_ind_handler}, 
    {GATTC_CMP_EVT,              (ke_msg_func_t) gattc_cmp_evt_handler},

    {OTAT_CTRL_PT_SEND_NOTIFY,         (ke_msg_func_t) otat_notify_send_req_handler}, 
};


///Specifies the message handlers that are common to all states.
const struct ke_state_handler otat_default_handler = KE_STATE_HANDLER(otat_default_state);

#endif //BLE_HT_THERMOM

/// @} OTATTASK
