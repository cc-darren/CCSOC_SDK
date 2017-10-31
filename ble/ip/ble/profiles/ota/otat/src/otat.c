/**
 ****************************************************************************************
 *
 * @file otat.c
 *
 * @brief Health Thermometer Profile Thermometer implementation.
 *
 * Copyright (C) RivieraWaves 2009-2016
 *
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup OTAT
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "rwip_config.h"

#if (BLE_OTA_TARGET)
#include "attm.h"
#include "otat.h"
#include "otat_task.h"
#include "co_utils.h"
#include "prf_utils.h"

#include "ke_mem.h"
#include "co_utils.h"

/*
 * OTAT PROFILE ATTRIBUTES
 ****************************************************************************************
 */
#if 0
/// Full OTA Database Description - Used to add attributes into the database
const struct attm_desc otat_att_db[OTAS_IDX_NB] =
{
    // Health Thermometer Service Declaration
    [OTAS_IDX_SVC]                   =   {ATT_DECL_PRIMARY_SERVICE, PERM(RD, ENABLE), 0, 0},

    // OTA Characteristic Declaration
    [OTAS_IDX_OTA_CHAR]        =   {ATT_DECL_CHARACTERISTIC, PERM(RD, ENABLE), 0, 0},
    // OTA Characteristic Value
    [OTAS_IDX_OTA_VAL]         =   {ATT_CHAR_TEMPERATURE_MEAS, PERM(NTF, ENABLE)|PERM(WRITE_REQ, ENABLE), PERM(RI, ENABLE), OTAT_OTA_MAX_LEN},
    //[OTAS_IDX_OTA_VAL]         =   {ATT_CHAR_TEMPERATURE_MEAS, PERM(NTF, ENABLE), PERM(RI, ENABLE), 0},
    // OTA Characteristic - Client Characteristic Configuration Descriptor
    [OTAS_IDX_OTA_NTF_CFG]     =   {ATT_DESC_CLIENT_CHAR_CFG, PERM(RD, ENABLE)|PERM(WRITE_REQ, ENABLE), 0, 0},
/*
    // Temperature Type Characteristic Declaration
    [HTS_IDX_TEMP_TYPE_CHAR]        =   {ATT_DECL_CHARACTERISTIC, PERM(RD, ENABLE), 0, 0},
    // Temperature Type Characteristic Value
    [HTS_IDX_TEMP_TYPE_VAL]         =   {ATT_CHAR_TEMPERATURE_TYPE, PERM(RD, ENABLE), PERM(RI, ENABLE), 0},

    // Intermediate Measurement Characteristic Declaration
    [HTS_IDX_INTERM_TEMP_CHAR]      =   {ATT_DECL_CHARACTERISTIC, PERM(RD, ENABLE), 0, 0},
    // Intermediate Measurement Characteristic Value
    [HTS_IDX_INTERM_TEMP_VAL]       =   {ATT_CHAR_INTERMED_TEMPERATURE, PERM(NTF, ENABLE), PERM(RI, ENABLE), 0},
    // Intermediate Measurement Characteristic - Client Characteristic Configuration Descriptor
    [HTS_IDX_INTERM_TEMP_CFG]       =   {ATT_DESC_CLIENT_CHAR_CFG, PERM(RD, ENABLE)|PERM(WRITE_REQ, ENABLE), 0, 0},

    // Measurement Interval Characteristic Declaration
    [HTS_IDX_MEAS_INTV_CHAR]        =   {ATT_DECL_CHARACTERISTIC, PERM(RD, ENABLE), 0, 0},
    // Measurement Interval Characteristic Value
    [HTS_IDX_MEAS_INTV_VAL]         =   {ATT_CHAR_MEAS_INTERVAL, PERM(RD, ENABLE), PERM(RI, ENABLE), OTAT_MEAS_INTV_MAX_LEN},
    // Measurement Interval Characteristic - Client Characteristic Configuration Descriptor
    [HTS_IDX_MEAS_INTV_CFG]         =   {ATT_DESC_CLIENT_CHAR_CFG, PERM(RD, ENABLE)|PERM(WRITE_REQ, ENABLE), 0, 0},
    // Measurement Interval Characteristic - Valid Range Descriptor
    [HTS_IDX_MEAS_INTV_VAL_RANGE]   =   {ATT_DESC_VALID_RANGE, PERM(RD, ENABLE), PERM(RI, ENABLE), 0},
*/    
};
#endif

#ifdef BLE_OTA_BL_MODE_EN
const uint8_t BLE_DFU_BASE_UUID_128[ATT_UUID_128_LEN] = {0x59, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; // Nordic Secure DFU Service
#define ATT_DECL_PRIMARY_SERVICE_128  {0x59, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
#define ATT_DECL_CHARACTERISTIC_128   {0x03, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00} 
#define ATT_CHAR_OTA_PACKET_128 {0x50, 0xEA, 0xDA, 0x30, 0x88, 0x83, 0xB8, 0x9F, 0x60, 0x4F, 0x15, 0xF3, 0x02, 0x00, 0xC9, 0x8E} 
#define ATT_CHAR_OTA_CTRL_PT_128 {0x50, 0xEA, 0xDA, 0x30, 0x88, 0x83, 0xB8, 0x9F, 0x60, 0x4F, 0x15, 0xF3, 0x01, 0x00, 0xC9, 0x8E} 
#define ATT_DESC_CLIENT_CHAR_CFG_128  {0x02, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00} 

 
const struct attm_desc_128 otat_att_db_128[OTAS_IDX_NB] =
{
    //  Service Declaration
    [OTAS_IDX_SVC]                   =   {ATT_DECL_PRIMARY_SERVICE_128, PERM(RD, ENABLE), 0, 0},

    // OTA DFU Packet Characteristic Declaration
    [OTAS_IDX_OTA_PKT_CHAR]              =   {ATT_DECL_CHARACTERISTIC_128, PERM(RD, ENABLE), 0, 0},
    // OTA Characteristic Value
    [OTAS_IDX_OTA_PKT_VAL]               =   {ATT_CHAR_OTA_PACKET_128, PERM(WRITE_COMMAND, ENABLE), PERM(RI, ENABLE)| PERM(UUID_LEN, UUID_128), OTAT_NOTIFY_SEND_MAX_LEN},


    // OTA Control Point Characteristic Declaration
    [OTAS_IDX_OTA_CTRL_PT_CHAR]              =   {ATT_DECL_CHARACTERISTIC_128, PERM(RD, ENABLE), 0, 0},
    // OTA Characteristic Value
    [OTAS_IDX_OTA_CTRL_PT_VAL]               =   {ATT_CHAR_OTA_CTRL_PT_128, PERM(NTF, ENABLE)|PERM(WRITE_REQ, ENABLE), PERM(RI, ENABLE)| PERM(UUID_LEN, UUID_128), OTAT_NOTIFY_SEND_MAX_LEN},    
    // OTA Characteristic - Client Characteristic Configuration Descriptor
    [OTAS_IDX_OTA_CTRL_PT_NTF_CFG]           =   {ATT_DESC_CLIENT_CHAR_CFG_128, PERM(RD, ENABLE)|PERM(WRITE_REQ, ENABLE), 0, 0},
};



#else //for app 
const uint8_t BLE_DFU_BASE_UUID_128[ATT_UUID_128_LEN] = {0x50, 0xEA, 0xDA, 0x30, 0x88, 0x83, 0xB8, 0x9F, 0x60, 0x4F, 0x15, 0xF3, 0x01, 0x00, 0x40, 0x8E}; /**< Used vendor specific UUID. */
#define ATT_DECL_PRIMARY_SERVICE_128  {0x00, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
#define ATT_DECL_CHARACTERISTIC_128   {0x03, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00} 
#define ATT_CHAR_USER_DEFINED_DFU_128 {0x50, 0xEA, 0xDA, 0x30, 0x88, 0x83, 0xB8, 0x9F, 0x60, 0x4F, 0x15, 0xF3, 0x01, 0x00, 0x40, 0x8E} 
#define ATT_DESC_CLIENT_CHAR_CFG_128  {0x02, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00} 

 
const struct attm_desc_128 otat_att_db_128[OTAS_IDX_NB] =
{
    //  Service Declaration
    [OTAS_IDX_SVC]                   =   {ATT_DECL_PRIMARY_SERVICE_128, PERM(RD, ENABLE), 0, 0},

    // OTA Characteristic Declaration
    [OTAS_IDX_OTA_CHAR]              =   {ATT_DECL_CHARACTERISTIC_128, PERM(RD, ENABLE), 0, 0},
    // OTA Characteristic Value
    [OTAS_IDX_OTA_VAL]               =   {ATT_CHAR_USER_DEFINED_DFU_128, PERM(NTF, ENABLE)|PERM(WRITE_REQ, ENABLE), PERM(RI, ENABLE)| PERM(UUID_LEN, UUID_128), OTAT_OTA_MAX_LEN},
    // OTA Characteristic - Client Characteristic Configuration Descriptor
    [OTAS_IDX_OTA_NTF_CFG]           =   {ATT_DESC_CLIENT_CHAR_CFG_128, PERM(RD, ENABLE)|PERM(WRITE_REQ, ENABLE), 0, 0},
};
#endif

static uint16_t otat_compute_att_table(uint16_t features);

/*
 * LOCAL FUNCTION DEFINITIONS
 ****************************************************************************************
 */

static uint8_t otat_init(struct prf_task_env* env, uint16_t* start_hdl, uint16_t app_task,
                            uint8_t sec_lvl, struct otat_db_cfg* params)
{
    // Service content flag
    uint16_t cfg_flag;
    // DB Creation Status
    uint8_t status = ATT_ERR_NO_ERROR;

    cfg_flag = otat_compute_att_table(params->features);


    // modified by Samuel
    /*
    status = attm_svc_create_db(start_hdl, ATT_SVC_HEALTH_THERMOM, (uint8_t *)&cfg_flag,
               OTAS_IDX_NB, NULL, env->task, &otat_att_db[0],
              (sec_lvl & (PERM_MASK_SVC_DIS)) | PERM(SVC_MI, DISABLE) );
    */

    if(1)//test
    {

        status = attm_svc_create_db_128(start_hdl, BLE_DFU_BASE_UUID_128, (uint8_t *)&cfg_flag, 
                   OTAS_IDX_NB, NULL, env->task,  &otat_att_db_128[0],
                   (sec_lvl & (PERM_MASK_SVC_DIS)) | PERM(SVC_MI, DISABLE));
    }
    
    if( status == ATT_ERR_NO_ERROR )
    {
        //-------------------- allocate memory required for the profile  ---------------------
        struct otat_env_tag* otat_env =
                (struct otat_env_tag* ) ke_malloc(sizeof(struct otat_env_tag), KE_MEM_ATT_DB);

        // allocate PROXR required environment variable
        env->env = (prf_env_t*) otat_env;

        otat_env->shdl     = *start_hdl;
        otat_env->prf_env.app_task = app_task
                        | (PERM_GET(sec_lvl, SVC_MI) ? PERM(PRF_MI, ENABLE) : PERM(PRF_MI, DISABLE));
        otat_env->prf_env.prf_task = env->task | PERM(PRF_MI, DISABLE);

        // initialize environment variable
        env->id                     = TASK_ID_OTAT;
        env->desc.idx_max           = OTAT_IDX_MAX;
        env->desc.state             = otat_env->state;
        env->desc.default_handler   = &otat_default_handler;

        //Save features on the environment
        otat_env->features      = params->features;
//        otat_env->meas_intv     = params->meas_intv;
//        otat_env->meas_intv_min = params->valid_range_min;
//        otat_env->meas_intv_max = params->valid_range_max;
//        otat_env->temp_type     = params->temp_type;
        otat_env->operation     = NULL;
        memset(otat_env->ntf_ind_cfg, 0 , sizeof(otat_env->ntf_ind_cfg));
#if 0 // remarked by Samuel
        // Update measurement interval permissions
        if (OTAT_IS_FEATURE_SUPPORTED(params->features, OTAT_MEAS_INTV_CHAR_SUP))
        {
            uint16_t perm = PERM(RD, ENABLE);

            //Check if Measurement Interval Char. supports indications
            if (OTAT_IS_FEATURE_SUPPORTED(params->features, OTAT_MEAS_INTV_IND_SUP))
            {
                perm |= PERM(IND, ENABLE);
            }

            //Check if Measurement Interval Char. is writable

            perm |= PERM(WRITE_REQ, ENABLE); // modified by Samuel
            /*
            if (OTAT_IS_FEATURE_SUPPORTED(params->features, OTAT_MEAS_INTV_WR_SUP))
            {
                perm |= PERM(WP, UNAUTH)|PERM(WRITE_REQ, ENABLE);
            }
            */
            attm_att_set_permission(OTAT_HANDLE(HTS_IDX_MEAS_INTV_VAL), perm, 0);
        }
#endif
        // service is ready, go into an Idle state
        ke_state_set(env->task, OTAT_IDLE);
    }
    return (status);
}

static void otat_destroy(struct prf_task_env* env)
{
    struct otat_env_tag* otat_env = (struct otat_env_tag*) env->env;

    // free profile environment variables
    if(otat_env->operation != NULL)
    {
        ke_free(otat_env->operation);
    }


    env->env = NULL;
    ke_free(otat_env);
}

static void otat_create(struct prf_task_env* env, uint8_t conidx)
{
    /* Clear configuration for this connection */
    struct otat_env_tag* otat_env = (struct otat_env_tag*) env->env;
    otat_env->ntf_ind_cfg[conidx] = 0;
}

static void otat_cleanup(struct prf_task_env* env, uint8_t conidx, uint8_t reason)
{
    /* Clear configuration for this connection */
    struct otat_env_tag* otat_env = (struct otat_env_tag*) env->env;
    otat_env->ntf_ind_cfg[conidx] = 0;
}

/**
 ****************************************************************************************
 * @brief Compute a flag allowing to know attributes to add into the database
 *
 * @return a 16-bit flag whose each bit matches an attribute. If the bit is set to 1, the
 * attribute will be added into the database.
 ****************************************************************************************
 */
static uint16_t otat_compute_att_table(uint16_t features)
{

#ifdef BLE_OTA_BL_MODE_EN
    //OTA Characteristic is mandatory
    uint16_t att_table = OTAT_OTA_PKT_MASK | OTAT_OTA_CTRL_PT_MASK;


#else
    //OTA Characteristic is mandatory
    uint16_t att_table = OTAT_OTA_MASK;
#endif
    /*

    //Temperature Measurement Characteristic is mandatory
    uint16_t att_table = OTAT_TEMP_MEAS_MASK;


    //Check if Temperature Type Char. is supported
    if (OTAT_IS_FEATURE_SUPPORTED(features, OTAT_TEMP_TYPE_CHAR_SUP))
    {
        att_table |= OTAT_TEMP_TYPE_MASK;
    }

    //Check if Intermediate Temperature Char. is supported
    if (OTAT_IS_FEATURE_SUPPORTED(features, OTAT_INTERM_TEMP_CHAR_SUP))
    {
        att_table |= OTAT_INTM_TEMP_MASK;
    }

    //Check if Measurement Interval Char. is supported
    if (OTAT_IS_FEATURE_SUPPORTED(features, OTAT_MEAS_INTV_CHAR_SUP))
    {
        att_table |= OTAT_MEAS_INTV_MASK;

        //Check if Measurement Interval Char. supports indications
        if (OTAT_IS_FEATURE_SUPPORTED(features, OTAT_MEAS_INTV_IND_SUP))
        {
            att_table |= OTAT_MEAS_INTV_CCC_MASK;
        }

        //Check if Measurement Interval Char. is writable
        if (OTAT_IS_FEATURE_SUPPORTED(features, OTAT_MEAS_INTV_WR_SUP))
        {
            att_table |= OTAT_MEAS_INTV_VALID_RGE_MASK;
        }
    }
*/
    return att_table;
}


/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

/// OTAT Task interface required by profile manager
const struct prf_task_cbs otat_itf =
{
    (prf_init_fnct) otat_init,
    otat_destroy,
    otat_create,
    otat_cleanup,
};


/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

uint8_t otat_get_valid_rge_offset(uint16_t features)
{
    uint8_t offset = 0;
/*
    if (OTAT_IS_FEATURE_SUPPORTED(features, OTAT_MEAS_INTV_WR_SUP))
    {
        offset += 1;

        if (OTAT_IS_FEATURE_SUPPORTED(features, OTAT_MEAS_INTV_IND_SUP))
        {
            offset += 1;
        }
    }
*/
    return offset;
}
#if 0
uint8_t otat_pack_temp_value(uint8_t *packed_temp, struct htp_temp_meas temp_meas)
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
    if(cursor < OTAT_TEMP_MEAS_MAX_LEN)
    {
        memset(packed_temp + cursor, 0, (OTAT_TEMP_MEAS_MAX_LEN - cursor));
    }
*/
    return cursor;
}
#endif
void otat_exe_operation(void)
{
    struct otat_env_tag* otat_env = PRF_ENV_GET(OTAT, otat);

    ASSERT_ERR(otat_env->operation != NULL);

    bool finished = true;

    while(otat_env->operation->cursor < BLE_CONNECTION_MAX)
    {
        // check if this type of event is enabled
        if(((otat_env->ntf_ind_cfg[otat_env->operation->cursor] & otat_env->operation->op) != 0)
            // and event not filtered on current connection
            && (otat_env->operation->conidx != otat_env->operation->cursor))
        {
            // trigger the event
            struct gattc_send_evt_cmd * evt = KE_MSG_ALLOC_DYN(GATTC_SEND_EVT_CMD,
                    KE_BUILD_ID(TASK_GATTC , otat_env->operation->cursor), prf_src_task_get(&otat_env->prf_env, 0),
                    gattc_send_evt_cmd, otat_env->operation->length);

            evt->operation = (otat_env->operation->op != OTAT_CFG_OTA_NTF) ? GATTC_INDICATE : GATTC_NOTIFY;
            evt->length    = otat_env->operation->length;
            evt->handle    = otat_env->operation->handle;
            memcpy(evt->value, otat_env->operation->data, evt->length);

            ke_msg_send(evt);

            finished = false;
            otat_env->operation->cursor++;
            break;
        }
        otat_env->operation->cursor++;
    }


    // check if operation is finished
    if(finished)
    {
   /*
        // do not send response if operation has been locally requested
        if(otat_env->operation->dest_id != prf_src_task_get(&otat_env->prf_env, 0))
        {
            // send response to requester
            struct otat_meas_intv_upd_rsp * rsp =
                    KE_MSG_ALLOC(((otat_env->operation->op == OTAT_CFG_OTA_NTF) ? OTAT_MEAS_INTV_UPD_RSP : OTAT_TEMP_SEND_RSP),
                                 otat_env->operation->dest_id, prf_src_task_get(&otat_env->prf_env, 0),
                                 otat_meas_intv_upd_rsp);
            rsp->status = GAP_ERR_NO_ERROR;
            ke_msg_send(rsp);
        }
*/
        // free operation
        ke_free(otat_env->operation);
        otat_env->operation = NULL;
        // go back to idle state
        ke_state_set(prf_src_task_get(&(otat_env->prf_env), 0), OTAT_IDLE);
    }
}


uint8_t otat_update_ntf_ind_cfg(uint8_t conidx, uint8_t cfg, uint16_t valid_val, uint16_t value)
{
    struct otat_env_tag* otat_env = PRF_ENV_GET(OTAT, otat);
    uint8_t status = GAP_ERR_NO_ERROR;

    if((value != valid_val) && (value != PRF_CLI_STOP_NTFIND))
    {
        status = PRF_APP_ERROR;

    }
    else if (value == valid_val)
    {
        otat_env->ntf_ind_cfg[conidx] |= cfg;
    }
    else
    {
        otat_env->ntf_ind_cfg[conidx] &= ~cfg;
    }

    if(status == GAP_ERR_NO_ERROR)
    {
        // inform application that notification/indication configuration has changed
        struct otat_cfg_indntf_ind * ind = KE_MSG_ALLOC(OTAT_CFG_INDNTF_IND,
                prf_dst_task_get(&otat_env->prf_env, conidx), prf_src_task_get(&otat_env->prf_env, conidx),
                otat_cfg_indntf_ind);
        ind->conidx      = conidx;
        ind->ntf_ind_cfg = otat_env->ntf_ind_cfg[conidx];
        ke_msg_send(ind);
    }

    return (status);
}


const struct prf_task_cbs* otat_prf_itf_get(void)
{
   return &otat_itf;
}


uint16_t otat_att_hdl_get(struct otat_env_tag* otat_env, uint8_t att_idx)
{
    uint16_t handle = otat_env->shdl;

    do
    {
        // Mandatory attribute handle
#ifdef BLE_OTA_BL_MODE_EN
        //if(att_idx > OTAS_IDX_OTA_PKT_VAL)
        //{
        //    handle += OTAT_OTA_ATT_NB;
        //}
        //else
        //{
            handle += att_idx;
            break;
        //}

#else
        if(att_idx > OTAS_IDX_OTA_NTF_CFG)
        {
            handle += OTAT_OTA_ATT_NB;
        }
        else
        {
            handle += att_idx;
            break;
        }
#endif        
/*
        // Temperature Type
        if((OTAT_IS_FEATURE_SUPPORTED(otat_env->features, OTAT_TEMP_TYPE_CHAR_SUP)) && (att_idx > HTS_IDX_TEMP_TYPE_VAL))
        {
            handle += OTAT_TEMP_TYPE_ATT_NB;
        }
        else if(!OTAT_IS_FEATURE_SUPPORTED(otat_env->features, OTAT_TEMP_TYPE_CHAR_SUP))
        {
            handle = ATT_INVALID_HANDLE;
            break;
        }
        else
        {
            handle += att_idx - HTS_IDX_TEMP_TYPE_CHAR;
            break;
        }

        // Intermediate Temperature Measurement
        if((OTAT_IS_FEATURE_SUPPORTED(otat_env->features, OTAT_INTERM_TEMP_CHAR_SUP)) && (att_idx > HTS_IDX_INTERM_TEMP_CFG))
        {
            handle += OTAT_INTERM_MEAS_ATT_NB;
        }
        else if(!OTAT_IS_FEATURE_SUPPORTED(otat_env->features, OTAT_INTERM_TEMP_CHAR_SUP))
        {
            handle = ATT_INVALID_HANDLE;
            break;
        }
        else
        {
            handle += att_idx - HTS_IDX_INTERM_TEMP_CHAR;
            break;
        }

        // Measurement Interval
        if(!OTAT_IS_FEATURE_SUPPORTED(otat_env->features, OTAT_MEAS_INTV_CHAR_SUP) || (att_idx >= HTS_IDX_NB))
        {
            handle = ATT_INVALID_HANDLE;
            break;
        }

        if(att_idx <= HTS_IDX_MEAS_INTV_VAL)
        {
            handle += att_idx - HTS_IDX_MEAS_INTV_CHAR;
            break;
        }
        else
        {
            handle += OTAT_MEAS_INTV_ATT_NB;
        }

        // Measurement Interval Indication
        if(att_idx == HTS_IDX_MEAS_INTV_CFG)
        {
            if(!OTAT_IS_FEATURE_SUPPORTED(otat_env->features, OTAT_MEAS_INTV_IND_SUP))
            {
                handle = ATT_INVALID_HANDLE;
                break;
            }
        }
        // Measurement Interval Write permission
        else if(OTAT_IS_FEATURE_SUPPORTED(otat_env->features, OTAT_MEAS_INTV_WR_SUP))
        {
            handle += OTAT_MEAS_INTV_CCC_ATT_NB;

            if(!OTAT_IS_FEATURE_SUPPORTED(otat_env->features, OTAT_MEAS_INTV_WR_SUP))
            {
                handle = ATT_INVALID_HANDLE;
                break;
            }
        }
 */       
    } while (0);

    return handle;
}

uint8_t otat_att_idx_get(struct otat_env_tag* otat_env, uint16_t handle)
{
    uint16_t handle_ref = otat_env->shdl; // hangle of start
    uint8_t att_idx = ATT_INVALID_IDX;

    do
    {
        // not valid hande
        if(handle < handle_ref)
        {
            break;
        }


        
#ifdef BLE_OTA_BL_MODE_EN

        // Mandatory attribute handle, OTA packet:
        handle_ref += OTAT_OTA_PKT_ATT_NB;

        if(handle < handle_ref) 
        {
            att_idx = OTAS_IDX_OTA_CTRL_PT_CHAR - (handle_ref - handle);
            break;
        }


        // OTA control point:
        handle_ref += OTAT_OTA_CTRL_PT_ATT_NB;
        
        if(handle < handle_ref)
        {
            att_idx = OTAS_IDX_NB - (handle_ref - handle);
            break;
        }
#else
        
        // Mandatory attribute handle, OTA packet:
        handle_ref += OTAT_OTA_ATT_NB;

        if(handle < handle_ref) 
        {
            att_idx = OTAS_IDX_NB - (handle_ref - handle);
            break;
        }

        
#endif
/*

        if(handle < handle_ref)
        {
            att_idx = HTS_IDX_TEMP_TYPE_CHAR - (handle_ref - handle);
            break;
        }

        // Temperature Type
        if(OTAT_IS_FEATURE_SUPPORTED(otat_env->features, OTAT_TEMP_TYPE_CHAR_SUP))
        {
            handle_ref += OTAT_TEMP_TYPE_ATT_NB;

            if(handle < handle_ref)
            {
                att_idx = HTS_IDX_INTERM_TEMP_CHAR - (handle_ref - handle);
                break;
            }
        }

        // Intermediate Temperature Measurement
        if(OTAT_IS_FEATURE_SUPPORTED(otat_env->features, OTAT_INTERM_TEMP_CHAR_SUP))
        {
            handle_ref += OTAT_INTERM_MEAS_ATT_NB;

            if(handle < handle_ref)
            {
                att_idx = HTS_IDX_MEAS_INTV_CHAR - (handle_ref - handle);
                break;
            }
        }

        // Measurement Interval
        if(OTAT_IS_FEATURE_SUPPORTED(otat_env->features, OTAT_MEAS_INTV_CHAR_SUP))
        {
            handle_ref += OTAT_MEAS_INTV_ATT_NB;

            if(handle < handle_ref)
            {
                att_idx = HTS_IDX_MEAS_INTV_CFG - (handle_ref - handle);
                break;
            }

            if(OTAT_IS_FEATURE_SUPPORTED(otat_env->features, OTAT_MEAS_INTV_IND_SUP))
            {
                if(handle == handle_ref)
                {
                    att_idx = HTS_IDX_MEAS_INTV_CFG;
                    break;
                }

                handle_ref += OTAT_MEAS_INTV_CCC_ATT_NB;
            }

            if(OTAT_IS_FEATURE_SUPPORTED(otat_env->features, OTAT_MEAS_INTV_WR_SUP))
            {
                if(handle == handle_ref)
                {
                    att_idx = HTS_IDX_MEAS_INTV_VAL_RANGE;
                    break;
                }
            }
        }
 */       
    } while (0);

    return att_idx;
}



#endif //BLE_HT_THERMOM

/// @} OTAT
