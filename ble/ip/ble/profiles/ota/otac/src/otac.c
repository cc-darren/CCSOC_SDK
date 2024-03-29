/**
 ****************************************************************************************
 *
 * @file otac.c
 *
 * @brief Health Thermometer Profile Collector implementation.
 *
 * Copyright (C) RivieraWaves 2009-2016
 *
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup OTAC
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "rwip_config.h"

#if (BLE_OTA_TARGET)
#include "otac.h"
#include "otac_task.h"
#include "gap.h"

#include "ke_mem.h"
#include "co_utils.h"
#include "prf_utils.h"

/*
 * LOCAL FUNCTION DEFINITIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Initialization of the OTAC module.
 * This function performs all the initializations of the Profile module.
 *  - Creation of database (if it's a service)
 *  - Allocation of profile required memory
 *  - Initialization of task descriptor to register application
 *      - Task State array
 *      - Number of tasks
 *      - Default task handler
 *
 * @param[out]    env        Collector or Service allocated environment data.
 * @param[in|out] start_hdl  Service start handle (0 - dynamically allocated), only applies for services.
 * @param[in]     app_task   Application task number.
 * @param[in]     sec_lvl    Security level (AUTH, EKS and MI field of @see enum attm_value_perm_mask)
 * @param[in]     param      Configuration parameters of profile collector or service (32 bits aligned)
 *
 * @return status code to know if profile initialization succeed or not.
 ****************************************************************************************
 */
static uint8_t otac_init(struct prf_task_env* env, uint16_t* start_hdl, uint16_t app_task,
                            uint8_t sec_lvl,  void* params)
{
    uint8_t idx;
    //-------------------- allocate memory required for the profile  ---------------------

    struct otac_env_tag* otac_env =
            (struct otac_env_tag* ) ke_malloc(sizeof(struct otac_env_tag), KE_MEM_ATT_DB);

    // allocate OTAC required environment variable
    env->env = (prf_env_t*) otac_env;

    otac_env->prf_env.app_task = app_task
            | (PERM_GET(sec_lvl, SVC_MI) ? PERM(PRF_MI, ENABLE) : PERM(PRF_MI, DISABLE));
    otac_env->prf_env.prf_task = env->task | PERM(PRF_MI, ENABLE);

    // initialize environment variable
    env->id                     = TASK_ID_OTAC;
    env->desc.idx_max           = OTAC_IDX_MAX;
    env->desc.state             = otac_env->state;
    env->desc.default_handler   = &otac_default_handler;

    for(idx = 0; idx < OTAC_IDX_MAX ; idx++)
    {
        otac_env->env[idx] = NULL;
        // service is ready, go into an Idle state
        ke_state_set(KE_BUILD_ID(env->task, idx), OTAC_FREE);
    }

    return GAP_ERR_NO_ERROR;
}

/**
 ****************************************************************************************
 * @brief Destruction of the OTAC module - due to a reset for instance.
 * This function clean-up allocated memory (attribute database is destroyed by another
 * procedure)
 *
 * @param[in|out]    env        Collector or Service allocated environment data.
 ****************************************************************************************
 */
static void otac_destroy(struct prf_task_env* env)
{
    uint8_t idx;
    struct otac_env_tag* otac_env = (struct otac_env_tag*) env->env;

    // cleanup environment variable for each task instances
    for(idx = 0; idx < OTAC_IDX_MAX ; idx++)
    {
        if(otac_env->env[idx] != NULL)
        {
            if(otac_env->env[idx]->operation != NULL)
            {
                ke_msg_free(otac_env->env[idx]->operation);
            }

            ke_free(otac_env->env[idx]);
        }
    }

    // free profile environment variables
    env->env = NULL;
    ke_free(otac_env);
}

/**
 ****************************************************************************************
 * @brief Handles Connection creation
 *
 * @param[in|out]    env        Collector or Service allocated environment data.
 * @param[in]        conidx     Connection index
 ****************************************************************************************
 */
static void otac_create(struct prf_task_env* env, uint8_t conidx)
{
    /* Put HTP Client in Idle state */
    ke_state_set(KE_BUILD_ID(env->task, conidx), OTAC_IDLE);
}

/**
 ****************************************************************************************
 * @brief Handles Disconnection
 *
 * @param[in|out]    env        Collector or Service allocated environment data.
 * @param[in]        conidx     Connection index
 * @param[in]        reason     Detach reason
 ****************************************************************************************
 */
static void otac_cleanup(struct prf_task_env* env, uint8_t conidx, uint8_t reason)
{
    struct otac_env_tag* otac_env = (struct otac_env_tag*) env->env;

    // clean-up environment variable allocated for task instance
    if(otac_env->env[conidx] != NULL)
    {
        if(otac_env->env[conidx]->operation != NULL)
        {
            ke_msg_free(otac_env->env[conidx]->operation);
        }

        ke_free(otac_env->env[conidx]);
        otac_env->env[conidx] = NULL;
    }

    /* Put HTP Client in Free state */
    ke_state_set(KE_BUILD_ID(env->task, conidx), OTAC_FREE);
}


void otac_unpack_temp(struct otac_env_tag *otac_env, uint8_t *packed_temp,
                      uint8_t length, uint8_t flag_stable_meas, uint8_t conidx)
{
    // Cursor used to read parameter in the packed temperature value
    uint8_t cursor = 0;


    // Checked the length of the received value
    if (length >= OTAC_PACKED_TEMP_MIN_LEN)
    {
        // Prepare the message to send to the application
        struct otac_temp_ind *ind = KE_MSG_ALLOC(OTAC_TEMP_IND,
                                                 prf_dst_task_get(&otac_env->prf_env, conidx),
                                                 prf_src_task_get(&otac_env->prf_env, conidx),
                                                 otac_temp_ind);

        // Unpack Temp Measurement
        ind->temp_meas.flags = *(packed_temp + cursor);
        cursor += 1;

        ind->temp_meas.temp = co_read32p(packed_temp + cursor);
        cursor += 4;

        // Time Flag Set
        if ((ind->temp_meas.flags & HTP_FLAG_TIME) == HTP_FLAG_TIME)
        {
            cursor += prf_unpack_date_time(packed_temp+cursor, &(ind->temp_meas.time_stamp));
        }

        // Type Flag set
        if ((ind->temp_meas.flags & HTP_FLAG_TYPE) == HTP_FLAG_TYPE)
        {
            ind->temp_meas.type    = *(packed_temp + cursor);
        }

        // Stable or intermediary type of temperature
        ind->stable_meas = flag_stable_meas;

        //Send request to ATT
        ke_msg_send(ind);
    }
}
#if 0
uint8_t otac_get_next_desc_char_code(struct otac_env_tag *otac_env,
                                     const struct prf_char_desc_def *desc_def)
{
    // Counter
    uint8_t i;
    uint8_t next_char_code;

    for (i=0; i<OTAC_DESC_HTS_MAX; i++)
    {
        next_char_code = desc_def[i].char_code;

        if (next_char_code > otac_env->last_char_code)
        {
            //If Char. has been found and number of attributes is upper than 2
            if ((otac_env->hts.chars[next_char_code].char_hdl != 0)
                    & (otac_env->hts.chars[next_char_code].char_ehdl_off > 2))
            {
                return next_char_code;
            }
        }
    }

    return i;
}
#endif

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

/// OTAC Task interface required by profile manager
const struct prf_task_cbs otac_itf =
{
        (prf_init_fnct) otac_init,
        otac_destroy,
        otac_create,
        otac_cleanup,
};

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

const struct prf_task_cbs* otac_prf_itf_get(void)
{
   return &otac_itf;
}


void otac_enable_rsp_send(struct otac_env_tag *otac_env, uint8_t conidx, uint8_t status)
{
    // Inform the APP about the status of the enabling of the Health Thermometer Profile Collector role 5awk
    struct otac_enable_rsp *cfm = KE_MSG_ALLOC(OTAC_ENABLE_RSP,
                                                prf_dst_task_get(&(otac_env->prf_env), conidx),
                                                prf_src_task_get(&(otac_env->prf_env), conidx),
                                                otac_enable_rsp);

    // Status
    cfm->status = status;

    if (status == GAP_ERR_NO_ERROR)
    {
        cfm->hts    = otac_env->env[conidx]->hts;
        // Go to IDLE state
        ke_state_set(prf_src_task_get(&(otac_env->prf_env), conidx), OTAC_IDLE);

        // Register profile handle to catch gatt indications
        prf_register_atthdl2gatt(&(otac_env->prf_env), conidx, &otac_env->env[conidx]->hts.svc );
    }
    else
    {
        if(otac_env->env[conidx] != NULL)
        {
            // clean-up environment variable allocated for task instance
            if(otac_env->env[conidx]->operation != NULL)
            {
                ke_free(otac_env->env[conidx]->operation);
            }
            ke_free(otac_env->env[conidx]);
            otac_env->env[conidx] = NULL;
        }
    }

    // Send the confirmation to the application
    ke_msg_send(cfm);
}

#endif //BLE_HT_COLLECTOR
/// @} OTAC
