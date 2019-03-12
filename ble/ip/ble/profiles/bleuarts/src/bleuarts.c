/**
 ****************************************************************************************
 *
 * @file bleuarts.c
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
 * @addtogroup BLEUARTS
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "rwip_config.h"

#if (CFG_APP_BLEUARTS)
#include "attm.h"
#include "bleuarts.h"
#include "bleuarts_task.h"
#include "co_utils.h"
#include "prf_utils.h"

#include "ke_mem.h"
#include "co_utils.h"
#include "tracer.h"
/*
 * BLEUARTS PROFILE ATTRIBUTES
 ****************************************************************************************
 */
const uint8_t BLE_BLEUARTS_BASE_UUID_128[ATT_UUID_128_LEN] = {0x9E, 0xCA, 0xDC, 0x24, 0x0E, 0xE5, 0xA9, 0xE0, 0x93, 0xF3, 0xA3, 0xB5, 0x01, 0x00, 0x40, 0x6E}; /**< Used vendor specific UUID. */

#define ATT_DECL_PRIMARY_SERVICE_128  {0x00, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
#define ATT_DECL_CHARACTERISTIC_128   {0x03, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
#define ATT_UUID_BLEUARTS_CHAR_RX_128  {0x9E, 0xCA, 0xDC, 0x24, 0x0E, 0xE5, 0xA9, 0xE0, 0x93, 0xF3, 0xA3, 0xB5, 0x02, 0x00, 0x40, 0x6E} 
#define ATT_UUID_BLEUARTS_CHAR_TX_128  {0x9E, 0xCA, 0xDC, 0x24, 0x0E, 0xE5, 0xA9, 0xE0, 0x93, 0xF3, 0xA3, 0xB5, 0x03, 0x00, 0x40, 0x6E} 
#define ATT_DESC_CLIENT_CHAR_CFG_128  {0x02, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00} 

const struct attm_desc_128 bleuarts_att_db_128[BLEUARTS_IDX_NB] =
{
	//----  Service  ----
    // = Service Declaration =
    [BLEUARTS_IDX_SVC]				=   {ATT_DECL_PRIMARY_SERVICE_128,	PERM(RD, ENABLE), 0, 0},

	//-------  RX  -------
    // = RX Declaration =
    [BLEUARTS_IDX_RX_CHAR]			=   {ATT_DECL_CHARACTERISTIC_128,	PERM(RD, ENABLE), 0, 0},
    // = RX Value =
    [BLEUARTS_IDX_RX_VAL]			=   {ATT_UUID_BLEUARTS_CHAR_RX_128,	PERM(WRITE_REQ, ENABLE) | PERM(WRITE_COMMAND, ENABLE), PERM(RI, ENABLE) | PERM(UUID_LEN, UUID_128), BLEUARTS_REPORT_MAX_LEN},

	//-------  TX  -------
	// = TX Declaration =
    [BLEUARTS_IDX_TX_CHAR]			=   {ATT_DECL_CHARACTERISTIC_128,	PERM(RD, ENABLE), 0, 0},
	// = TX Value =
    [BLEUARTS_IDX_TX_VAL]			=   {ATT_UUID_BLEUARTS_CHAR_TX_128,	PERM(NTF,ENABLE), PERM(RI, ENABLE) | PERM(UUID_LEN, UUID_128), BLEUARTS_REPORT_MAX_LEN},
    // = Client Characteristic Configuration Descriptor =
    [BLEUARTS_IDX_TX_NTF_CFG]		=   {ATT_DESC_CLIENT_CHAR_CFG_128, PERM(RD, ENABLE) | PERM(WRITE_REQ, ENABLE), 0, 0},
};

static uint16_t bleuarts_compute_att_table(uint16_t features);

/*
 * LOCAL FUNCTION DEFINITIONS
 ****************************************************************************************
 */
static uint8_t bleuarts_init(struct prf_task_env* env, uint16_t* start_hdl, uint16_t app_task,
                            uint8_t sec_lvl, struct bleuarts_db_cfg* params)
{
    // Service content flag
    uint16_t cfg_flag;
    // DB Creation Status
    uint8_t status = ATT_ERR_NO_ERROR;

	cfg_flag = bleuarts_compute_att_table(params->features);

	status = attm_svc_create_db_128(start_hdl, BLE_BLEUARTS_BASE_UUID_128, (uint8_t *)&cfg_flag, 
               BLEUARTS_IDX_NB, NULL, env->task,  &bleuarts_att_db_128[0],
               (sec_lvl & (PERM_MASK_SVC_DIS)) | PERM(SVC_MI, DISABLE) | PERM(SVC_UUID_LEN, UUID_128));


    if( status == ATT_ERR_NO_ERROR )
    {
        //-------------------- allocate memory required for the profile  ---------------------
        struct bleuarts_env_tag* bleuarts_env =
                (struct bleuarts_env_tag* ) ke_malloc(sizeof(struct bleuarts_env_tag), KE_MEM_ATT_DB);

        // allocate PROXR required environment variable
        env->env = (prf_env_t*) bleuarts_env;

        bleuarts_env->shdl     = *start_hdl;
        bleuarts_env->prf_env.app_task = app_task
                        | (PERM_GET(sec_lvl, SVC_MI) ? PERM(PRF_MI, ENABLE) : PERM(PRF_MI, DISABLE));
        bleuarts_env->prf_env.prf_task = env->task | PERM(PRF_MI, DISABLE);

        // initialize environment variable
        env->id                     = TASK_ID_BLEUARTS;
        env->desc.idx_max           = BLEUARTS_IDX_MAX;
        env->desc.state             = bleuarts_env->state;
        env->desc.default_handler   = &bleuarts_default_handler;

        // save features on the environment
        bleuarts_env->features      = params->features;

        bleuarts_env->operation     = NULL;
        memset(bleuarts_env->ntf_ind_cfg, 0 , sizeof(bleuarts_env->ntf_ind_cfg));

        // packet control block
        memset(&(bleuarts_env->tRcvReportPktCB), 0, sizeof(bleuarts_env->tRcvReportPktCB));

        // service is ready, go into an Idle state
        ke_state_set(env->task, BLEUARTS_IDLE);
    }
    return (status);
}

static void bleuarts_destroy(struct prf_task_env* env)
{
    struct bleuarts_env_tag* bleuarts_env = (struct bleuarts_env_tag*) env->env;

    // free profile environment variables
    if(bleuarts_env->operation != NULL)
    {
        ke_free(bleuarts_env->operation);
    }


    env->env = NULL;
    ke_free(bleuarts_env);
}

static void bleuarts_create(struct prf_task_env* env, uint8_t conidx)
{
    /* Clear configuration for this connection */
    struct bleuarts_env_tag* bleuarts_env = (struct bleuarts_env_tag*) env->env;
    bleuarts_env->ntf_ind_cfg[conidx] = 0;
}

static void bleuarts_cleanup(struct prf_task_env* env, uint8_t conidx, uint8_t reason)
{
    /* Clear configuration for this connection */
    struct bleuarts_env_tag* bleuarts_env = (struct bleuarts_env_tag*) env->env;
    bleuarts_env->ntf_ind_cfg[conidx] = 0;
}

/**
 ****************************************************************************************
 * @brief Compute a flag allowing to know attributes to add into the database
 *
 * @return a 16-bit flag whose each bit matches an attribute. If the bit is set to 1, the
 * attribute will be added into the database.
 ****************************************************************************************
 */
static uint16_t bleuarts_compute_att_table(uint16_t features)
{
    uint16_t att_table = 0;

    for(uint16_t bit = 0 ; bit < BLEUARTS_REPORT_ATT_NB; bit++)
    {
        att_table |= (1 << bit);
    }

    return att_table;
}


/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

/// BLEUARTS Task interface required by profile manager
const struct prf_task_cbs bleuarts_itf =
{
    (prf_init_fnct) bleuarts_init,
    bleuarts_destroy,
    bleuarts_create,
    bleuarts_cleanup,
};


/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

uint8_t bleuarts_get_valid_rge_offset(uint16_t features)
{
    uint8_t offset = 0;

    return offset;
}


void bleuarts_exe_operation(void)
{
    struct bleuarts_env_tag* bleuarts_env = PRF_ENV_GET(BLEUARTS, bleuarts);

    ASSERT_ERR(bleuarts_env->operation != NULL);

    bool finished = true;

    while(bleuarts_env->operation->cursor < BLE_CONNECTION_MAX)
    {
        // check if this type of event is enabled
        if(((bleuarts_env->ntf_ind_cfg[bleuarts_env->operation->cursor] & bleuarts_env->operation->op) != 0)
            // and event not filtered on current connection
            && (bleuarts_env->operation->conidx != bleuarts_env->operation->cursor))
        {
            // trigger the event
            struct gattc_send_evt_cmd * evt = KE_MSG_ALLOC_DYN(GATTC_SEND_EVT_CMD,
                    KE_BUILD_ID(TASK_GATTC , bleuarts_env->operation->cursor), prf_src_task_get(&bleuarts_env->prf_env, 0),
                    gattc_send_evt_cmd, bleuarts_env->operation->length);

            evt->operation = (bleuarts_env->operation->op != BLEUARTS_CFG_TX_NTF) ? GATTC_INDICATE : GATTC_NOTIFY;
            evt->length    = bleuarts_env->operation->length;
            evt->handle    = bleuarts_env->operation->handle;
            memcpy(evt->value, bleuarts_env->operation->data, evt->length);
	
            ke_msg_send(evt);

            finished = false;
            bleuarts_env->operation->cursor++;
            break;
        }
        bleuarts_env->operation->cursor++;
    }

    // check if operation is finished
    if(finished)
    {
        // free operation
        ke_free(bleuarts_env->operation);
        bleuarts_env->operation = NULL;
        // go back to idle state
        ke_state_set(prf_src_task_get(&(bleuarts_env->prf_env), 0), BLEUARTS_IDLE);
    }
}

uint8_t bleuarts_update_ntf_ind_cfg(uint8_t conidx, uint8_t cfg, uint16_t valid_val, uint16_t value)
{
    struct bleuarts_env_tag* bleuarts_env = PRF_ENV_GET(BLEUARTS, bleuarts);
    uint8_t status = GAP_ERR_NO_ERROR;

    if (value == valid_val)
    {
        bleuarts_env->ntf_ind_cfg[conidx] |= cfg;
    }
    else
    {
        bleuarts_env->ntf_ind_cfg[conidx] &= ~cfg;
    }

    if(status == GAP_ERR_NO_ERROR)
    {
        // inform application that notification/indication configuration has changed
        struct bleuarts_cfg_indntf_ind * ind = KE_MSG_ALLOC(BLEUARTS_CFG_INDNTF_IND,
                prf_dst_task_get(&bleuarts_env->prf_env, conidx), prf_src_task_get(&bleuarts_env->prf_env, conidx),
                bleuarts_cfg_indntf_ind);
        ind->conidx      = conidx;
        ind->ntf_ind_cfg = bleuarts_env->ntf_ind_cfg[conidx];
        ke_msg_send(ind);
    }

    return (status);
}


const struct prf_task_cbs* bleuarts_prf_itf_get(void)
{
   return &bleuarts_itf;
}


uint16_t bleuarts_att_hdl_get(struct bleuarts_env_tag* bleuarts_env, uint8_t att_idx)
{
    uint16_t handle = bleuarts_env->shdl;

    do
    {
        // Mandatory attribute handle

        if(att_idx > (BLEUARTS_IDX_NB-1))
        {
            handle += BLEUARTS_REPORT_ATT_NB;
        }
        else
        {
            handle += att_idx;
            break;
        }
    
    } while (0);

    return handle;
}

uint8_t bleuarts_att_idx_get(struct bleuarts_env_tag* bleuarts_env, uint16_t handle)
{
    uint16_t handle_ref = bleuarts_env->shdl; // handle of start
    uint8_t att_idx = ATT_INVALID_IDX;

    do
    {
        // not valid hande
        if(handle < handle_ref)
        {
            break;
        }
   
        // Mandatory attribute handle, REPORT packet:
        handle_ref += BLEUARTS_REPORT_ATT_NB;

        if(handle < handle_ref) 
        {
            att_idx = BLEUARTS_IDX_NB - (handle_ref - handle);
            break;
        }
  
    } while (0);

    return att_idx;
}

#endif //BLE_HT_THERMOM

/// @} BLEUARTS
