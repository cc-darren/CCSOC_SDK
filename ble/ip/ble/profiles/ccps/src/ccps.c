/**
 ****************************************************************************************
 *
 * @file ccps.c
 *
 * @brief CloudChip Proprietary Services implementation.
 *
 * Copyright (C) CloudChip 2018
 *
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup CCPS
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "rwip_config.h"

#if (BLE_CCPS_SERVER)
#include "attm.h"
#include "ccps.h"
#include "ccps_task.h"
#include "co_utils.h"
#include "prf_utils.h"

#include "ke_mem.h"
#include "co_utils.h"

/*
 * CCPS PROFILE ATTRIBUTES
 ****************************************************************************************
 */


const uint8_t BLE_CCPS_BASE_UUID_128[ATT_UUID_128_LEN] = {0xA6, 0x54, 0x23, 0xF9, 0x1A, 0xA2, 0x98, 0x8F, 0x07, 0x65, 0x22, 0x41, 0x08, 0xCC, 0xA2, 0x64}; /**< Used vendor specific UUID. */
#define ATT_DECL_PRIMARY_SERVICE_128  {0x00, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
#define ATT_DECL_CHARACTERISTIC_128   {0x03, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
#define ATT_CHAR_USER_DEFINED_CCPS_128 {0xA6, 0x54, 0x23, 0xF9, 0x1A, 0xA2, 0x98, 0x8F, 0x07, 0x65, 0x22, 0x41, 0x08, 0xCC, 0xA2, 0x64} 
#define ATT_DESC_CLIENT_CHAR_CFG_128  {0x02, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00} 

 
const struct attm_desc_128 ccps_att_db_128[CCPS_IDX_NB] =
{
    //  Service Declaration
    [CCPS_IDX_SVC]                   =   {ATT_DECL_PRIMARY_SERVICE_128, PERM(RD, ENABLE), 0, 0},

    // CCPS Report Characteristic Declaration
    [CCPS_IDX_REPORT_CHAR]              =   {ATT_DECL_CHARACTERISTIC_128, PERM(RD, ENABLE), 0, 0},
    // CCPS Report Characteristic Value
    [CCPS_IDX_REPORT_VAL]               =   {ATT_CHAR_USER_DEFINED_CCPS_128, PERM(NTF, ENABLE)|PERM(IND, ENABLE)|PERM(WRITE_REQ, ENABLE), PERM(RI, ENABLE)| PERM(UUID_LEN, UUID_128), CCPS_REPORT_MAX_LEN},
    // CCPS Report Characteristic - Client Characteristic Configuration Descriptor
    [CCPS_IDX_REPORT_NTF_CFG]           =   {ATT_DESC_CLIENT_CHAR_CFG_128, PERM(RD, ENABLE)|PERM(WRITE_REQ, ENABLE), 0, 0},
};


static uint16_t ccps_compute_att_table(uint16_t features);

/*
 * LOCAL FUNCTION DEFINITIONS
 ****************************************************************************************
 */

static uint8_t ccps_init(struct prf_task_env* env, uint16_t* start_hdl, uint16_t app_task,
                            uint8_t sec_lvl, struct ccps_db_cfg* params)
{
    // Service content flag
    uint16_t cfg_flag;
    // DB Creation Status
    uint8_t status = ATT_ERR_NO_ERROR;

    cfg_flag = ccps_compute_att_table(params->features);
    /*
    status = attm_svc_create_db_128(start_hdl, BLE_CCPS_BASE_UUID_128, (uint8_t *)&cfg_flag, 
               CCPS_IDX_NB, NULL, env->task,  &ccps_att_db_128[0],
               (sec_lvl & (PERM_MASK_SVC_DIS)) | PERM(SVC_MI, DISABLE) | PERM(SVC_MI, DISABLE)| PERM(SVC_UUID_LEN, UUID_128));
    */
     status = attm_svc_create_db_128(start_hdl, BLE_CCPS_BASE_UUID_128, (uint8_t *)&cfg_flag, 
               CCPS_IDX_NB, NULL, env->task,  &ccps_att_db_128[0],
               (sec_lvl & (PERM_MASK_SVC_DIS)) | PERM(SVC_MI, DISABLE) | PERM(SVC_UUID_LEN, UUID_128));

    
    if( status == ATT_ERR_NO_ERROR )
    {
        //-------------------- allocate memory required for the profile  ---------------------
        struct ccps_env_tag* ccps_env =
                (struct ccps_env_tag* ) ke_malloc(sizeof(struct ccps_env_tag), KE_MEM_ATT_DB);

        // allocate PROXR required environment variable
        env->env = (prf_env_t*) ccps_env;

        ccps_env->shdl     = *start_hdl;
        ccps_env->prf_env.app_task = app_task
                        | (PERM_GET(sec_lvl, SVC_MI) ? PERM(PRF_MI, ENABLE) : PERM(PRF_MI, DISABLE));
        ccps_env->prf_env.prf_task = env->task | PERM(PRF_MI, DISABLE);

        // initialize environment variable
        env->id                     = TASK_ID_CCPS;
        env->desc.idx_max           = CCPS_IDX_MAX;
        env->desc.state             = ccps_env->state;
        env->desc.default_handler   = &ccps_default_handler;

        //Save features on the environment
        ccps_env->features      = params->features;
//        ccps_env->meas_intv     = params->meas_intv;
//        ccps_env->meas_intv_min = params->valid_range_min;
//        ccps_env->meas_intv_max = params->valid_range_max;
//        ccps_env->temp_type     = params->temp_type;
        ccps_env->operation     = NULL;
        memset(ccps_env->ntf_ind_cfg, 0 , sizeof(ccps_env->ntf_ind_cfg));
#if 0 // remarked by Samuel
        // Update measurement interval permissions
        if (CCPS_IS_FEATURE_SUPPORTED(params->features, CCPS_MEAS_INTV_CHAR_SUP))
        {
            uint16_t perm = PERM(RD, ENABLE);

            //Check if Measurement Interval Char. supports indications
            if (CCPS_IS_FEATURE_SUPPORTED(params->features, CCPS_MEAS_INTV_IND_SUP))
            {
                perm |= PERM(IND, ENABLE);
            }

            //Check if Measurement Interval Char. is writable

            perm |= PERM(WRITE_REQ, ENABLE); // modified by Samuel
            /*
            if (CCPS_IS_FEATURE_SUPPORTED(params->features, CCPS_MEAS_INTV_WR_SUP))
            {
                perm |= PERM(WP, UNAUTH)|PERM(WRITE_REQ, ENABLE);
            }
            */
            attm_att_set_permission(CCPS_HANDLE(HTS_IDX_MEAS_INTV_VAL), perm, 0);
        }
#endif
        // service is ready, go into an Idle state
        ke_state_set(env->task, CCPS_IDLE);
    }
    return (status);
}

static void ccps_destroy(struct prf_task_env* env)
{
    struct ccps_env_tag* ccps_env = (struct ccps_env_tag*) env->env;

    // free profile environment variables
    if(ccps_env->operation != NULL)
    {
        ke_free(ccps_env->operation);
    }


    env->env = NULL;
    ke_free(ccps_env);
}

static void ccps_create(struct prf_task_env* env, uint8_t conidx)
{
    /* Clear configuration for this connection */
    struct ccps_env_tag* ccps_env = (struct ccps_env_tag*) env->env;
    ccps_env->ntf_ind_cfg[conidx] = 0;
}

static void ccps_cleanup(struct prf_task_env* env, uint8_t conidx, uint8_t reason)
{
    /* Clear configuration for this connection */
    struct ccps_env_tag* ccps_env = (struct ccps_env_tag*) env->env;
    ccps_env->ntf_ind_cfg[conidx] = 0;
}

/**
 ****************************************************************************************
 * @brief Compute a flag allowing to know attributes to add into the database
 *
 * @return a 16-bit flag whose each bit matches an attribute. If the bit is set to 1, the
 * attribute will be added into the database.
 ****************************************************************************************
 */
static uint16_t ccps_compute_att_table(uint16_t features)
{

    //CCPS Characteristic is mandatory
    uint16_t att_table = CCPS_REPORT_MASK;

    return att_table;
}


/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

/// CCPS Task interface required by profile manager
const struct prf_task_cbs ccps_itf =
{
    (prf_init_fnct) ccps_init,
    ccps_destroy,
    ccps_create,
    ccps_cleanup,
};


/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

uint8_t ccps_get_valid_rge_offset(uint16_t features)
{
    uint8_t offset = 0;
/*
    if (CCPS_IS_FEATURE_SUPPORTED(features, CCPS_MEAS_INTV_WR_SUP))
    {
        offset += 1;

        if (CCPS_IS_FEATURE_SUPPORTED(features, CCPS_MEAS_INTV_IND_SUP))
        {
            offset += 1;
        }
    }
*/
    return offset;
}
#if 0
uint8_t ccps_pack_temp_value(uint8_t *packed_temp, struct htp_temp_meas temp_meas)
{
    uint8_t cursor = 0;
/*
    *(packed_temp + cursor) = temp_meas.flags;
    cursor += 1;

    co_write32p(packed_temp + cursor, temp_meas.temp);
    cursor += 4;

    //Time Flag Set
    if ((temp_meas.flags & HTP_FLAG_TIME) == HTP_FLAG_TIME)
    {
        cursor += prf_pack_date_time(packed_temp + cursor, &(temp_meas.time_stamp));
    }

    //Type flag set
    if ((temp_meas.flags & HTP_FLAG_TYPE) == HTP_FLAG_TYPE)
    {
        *(packed_temp + cursor) = temp_meas.type;
        cursor += 1;
    }

    //Clear unused packet data
    if(cursor < CCPS_TEMP_MEAS_MAX_LEN)
    {
        memset(packed_temp + cursor, 0, (CCPS_TEMP_MEAS_MAX_LEN - cursor));
    }
*/
    return cursor;
}
#endif
void ccps_exe_operation(void)
{
    struct ccps_env_tag* ccps_env = PRF_ENV_GET(CCPS, ccps);

    ASSERT_ERR(ccps_env->operation != NULL);

    bool finished = true;

    while(ccps_env->operation->cursor < BLE_CONNECTION_MAX)
    {
        // check if this type of event is enabled
        if(((ccps_env->ntf_ind_cfg[ccps_env->operation->cursor] & ccps_env->operation->op) != 0)
            // and event not filtered on current connection
            && (ccps_env->operation->conidx != ccps_env->operation->cursor))
        {
            // trigger the event
            struct gattc_send_evt_cmd * evt = KE_MSG_ALLOC_DYN(GATTC_SEND_EVT_CMD,
                    KE_BUILD_ID(TASK_GATTC , ccps_env->operation->cursor), prf_src_task_get(&ccps_env->prf_env, 0),
                    gattc_send_evt_cmd, ccps_env->operation->length);

            evt->operation = (ccps_env->operation->op != CCPS_CFG_REPORT_NTF) ? GATTC_INDICATE : GATTC_NOTIFY;
            evt->length    = ccps_env->operation->length;
            evt->handle    = ccps_env->operation->handle;
            memcpy(evt->value, ccps_env->operation->data, evt->length);

            ke_msg_send(evt);

            finished = false;
            ccps_env->operation->cursor++;
            break;
        }
        ccps_env->operation->cursor++;
    }


    // check if operation is finished
    if(finished)
    {
   /*
        // do not send response if operation has been locally requested
        if(ccps_env->operation->dest_id != prf_src_task_get(&ccps_env->prf_env, 0))
        {
            // send response to requester
            struct ccps_meas_intv_upd_rsp * rsp =
                    KE_MSG_ALLOC(((ccps_env->operation->op == CCPS_CFG_REPORT_NTF) ? CCPS_MEAS_INTV_UPD_RSP : CCPS_TEMP_SEND_RSP),
                                 ccps_env->operation->dest_id, prf_src_task_get(&ccps_env->prf_env, 0),
                                 ccps_meas_intv_upd_rsp);
            rsp->status = GAP_ERR_NO_ERROR;
            ke_msg_send(rsp);
        }
*/
        // free operation
        ke_free(ccps_env->operation);
        ccps_env->operation = NULL;
        // go back to idle state
        ke_state_set(prf_src_task_get(&(ccps_env->prf_env), 0), CCPS_IDLE);
    }
}


uint8_t ccps_update_ntf_ind_cfg(uint8_t conidx, uint8_t cfg, uint16_t valid_val, uint16_t value)
{
    struct ccps_env_tag* ccps_env = PRF_ENV_GET(CCPS, ccps);
    uint8_t status = GAP_ERR_NO_ERROR;
/*
    if((value != valid_val) && (value != PRF_CLI_STOP_NTFIND))
    {
        status = PRF_APP_ERROR;

    }
    else 
*/    
    if (value == valid_val)
    {
        ccps_env->ntf_ind_cfg[conidx] |= cfg;
    }
    else
    {
        ccps_env->ntf_ind_cfg[conidx] &= ~cfg;
    }

    if(status == GAP_ERR_NO_ERROR)
    {
        // inform application that notification/indication configuration has changed
        struct ccps_cfg_indntf_ind * ind = KE_MSG_ALLOC(CCPS_CFG_INDNTF_IND,
                prf_dst_task_get(&ccps_env->prf_env, conidx), prf_src_task_get(&ccps_env->prf_env, conidx),
                ccps_cfg_indntf_ind);
        ind->conidx      = conidx;
        ind->ntf_ind_cfg = ccps_env->ntf_ind_cfg[conidx];
        ke_msg_send(ind);
    }

    return (status);
}


const struct prf_task_cbs* ccps_prf_itf_get(void)
{
   return &ccps_itf;
}


uint16_t ccps_att_hdl_get(struct ccps_env_tag* ccps_env, uint8_t att_idx)
{
    uint16_t handle = ccps_env->shdl;

    do
    {
        // Mandatory attribute handle

        if(att_idx > CCPS_IDX_REPORT_NTF_CFG)
        {
            handle += CCPS_REPORT_ATT_NB;
        }
        else
        {
            handle += att_idx;
            break;
        }
    
    } while (0);

    return handle;
}

uint8_t ccps_att_idx_get(struct ccps_env_tag* ccps_env, uint16_t handle)
{
    uint16_t handle_ref = ccps_env->shdl; // handle of start
    uint8_t att_idx = ATT_INVALID_IDX;

    do
    {
        // not valid hande
        if(handle < handle_ref)
        {
            break;
        }


   
        // Mandatory attribute handle, REPORT packet:
        handle_ref += CCPS_REPORT_ATT_NB;

        if(handle < handle_ref) 
        {
            att_idx = CCPS_IDX_NB - (handle_ref - handle);
            break;
        }
  
    } while (0);

    return att_idx;
}



#endif //BLE_HT_THERMOM

/// @} CCPS
