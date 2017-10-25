/**
 ****************************************************************************************
 *
 * @file otac_task.c
 *
 * @brief Health Thermometer Profile Collector Task implementation.
 *
 * Copyright (C) RivieraWaves 2009-2016
 *
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup OTACTASK
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "rwip_config.h"
#include "project.h"

#if (BLE_OTA_TARGET)
#include "gap.h"
#include "attm.h"
#include "otac_task.h"
#include "otac.h"
#include "gapc.h"
#include "gapc_task.h"

#include "ke_mem.h"

/// State machine used to retrieve Health Thermometer service characteristics information
const struct prf_char_def otac_hts_char[OTAC_CHAR_HTS_MAX] =
{
    /// Temperature Measurement
    [OTAC_CHAR_HTS_TEMP_MEAS]        = {ATT_CHAR_TEMPERATURE_MEAS,     ATT_MANDATORY, ATT_CHAR_PROP_IND},
    /// Temperature Type
    [OTAC_CHAR_HTS_TEMP_TYPE]        = {ATT_CHAR_TEMPERATURE_TYPE,     ATT_OPTIONAL,  ATT_CHAR_PROP_RD},
    /// Intermediate Temperature
    [OTAC_CHAR_HTS_INTM_TEMP]        = {ATT_CHAR_INTERMED_TEMPERATURE, ATT_OPTIONAL,  ATT_CHAR_PROP_NTF},
    /// Measurement Interval
    [OTAC_CHAR_HTS_MEAS_INTV]        = {ATT_CHAR_MEAS_INTERVAL,        ATT_OPTIONAL,  ATT_CHAR_PROP_RD},
};

/// State machine used to retrieve Health Thermometer service characteristics description information
const struct prf_char_desc_def otac_hts_char_desc[OTAC_DESC_HTS_MAX] =
{
    /// Temperature Measurement Client Config
    [OTAC_DESC_HTS_TEMP_MEAS_CLI_CFG]        = {ATT_DESC_CLIENT_CHAR_CFG, ATT_MANDATORY, OTAC_CHAR_HTS_TEMP_MEAS},
    /// Intermediate Temperature Client Config
    [OTAC_DESC_HTS_INTM_MEAS_CLI_CFG]        = {ATT_DESC_CLIENT_CHAR_CFG, ATT_OPTIONAL,  OTAC_CHAR_HTS_INTM_TEMP},
    /// Measurement Interval Client Config
    [OTAC_DESC_HTS_MEAS_INTV_CLI_CFG]        = {ATT_DESC_CLIENT_CHAR_CFG, ATT_OPTIONAL,  OTAC_CHAR_HTS_MEAS_INTV},
    /// Measurement Interval valid range
    [OTAC_DESC_HTS_MEAS_INTV_VAL_RGE]        = {ATT_DESC_VALID_RANGE,     ATT_OPTIONAL,  OTAC_CHAR_HTS_MEAS_INTV},
};

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Handles reception of the @ref OTAC_ENABLE_REQ message.
 * The handler enables the Health Thermometer Profile Collector Role.
 * @param[in] msgid Id of the message received (probably unused).
 * @param[in] param Pointer to the parameters of the message.
 * @param[in] dest_id ID of the receiving task instance (probably unused).
 * @param[in] src_id ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int otac_enable_req_handler(ke_msg_id_t const msgid,
                                   struct otac_enable_req const *param,
                                   ke_task_id_t const dest_id,
                                   ke_task_id_t const src_id)
{
    int msg_status = KE_MSG_CONSUMED;
    uint8_t status = GAP_ERR_NO_ERROR;
    uint8_t state = ke_state_get(dest_id);
    uint8_t conidx = KE_IDX_GET(dest_id);
    // Health Thermometer Profile Collector Role Task Environment
    struct otac_env_tag *otac_env = PRF_ENV_GET(OTAC, otac);

    if((state == OTAC_IDLE) && (otac_env->env[conidx] == NULL))
    {
        // allocate environment variable for task instance
        otac_env->env[conidx] = (struct otac_cnx_env*) ke_malloc(sizeof(struct otac_cnx_env),KE_MEM_ATT_DB);
        memset(otac_env->env[conidx], 0, sizeof(struct otac_cnx_env));

        //Config connection, start discovering
        if(param->con_type == PRF_CON_DISCOVERY)
        {
            //start discovering HTS on peer
            prf_disc_svc_send(&(otac_env->prf_env), conidx, ATT_SVC_HEALTH_THERMOM);

            // store context of request and go into busy state
            otac_env->env[conidx]->operation = ke_param2msg(param);
            msg_status = KE_MSG_NO_FREE;
            ke_state_set(dest_id, OTAC_BUSY);
        }
        //normal connection, get saved att details
        else
        {
            otac_env->env[conidx]->hts = param->hts;

            //send APP confirmation that can start normal connection to TH
            otac_enable_rsp_send(otac_env, conidx, GAP_ERR_NO_ERROR);
        }
    }
    else
    {
        status = PRF_ERR_REQ_DISALLOWED;
    }

    // send an error if request fails
    if(status != GAP_ERR_NO_ERROR)
    {
        otac_enable_rsp_send(otac_env, conidx, status);
    }

    return (msg_status);
}


/**
 ****************************************************************************************
 * @brief Handles reception of the @ref GATTC_SDP_SVC_IND_HANDLER message.
 * The handler stores the found service details for service discovery.
 * @param[in] msgid Id of the message received (probably unused).
 * @param[in] param Pointer to the parameters of the message.
 * @param[in] dest_id ID of the receiving task instance (probably unused).
 * @param[in] src_id ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int gattc_sdp_svc_ind_handler(ke_msg_id_t const msgid,
                                             struct gattc_sdp_svc_ind const *ind,
                                             ke_task_id_t const dest_id,
                                             ke_task_id_t const src_id)
{
    uint8_t state = ke_state_get(dest_id);

    if(state == OTAC_BUSY)
    {
        uint8_t conidx = KE_IDX_GET(dest_id);
        // Get the address of the environment
        struct otac_env_tag *otac_env = PRF_ENV_GET(OTAC, otac);

        ASSERT_INFO(otac_env != NULL, dest_id, src_id);
        ASSERT_INFO(otac_env->env[conidx] != NULL, dest_id, src_id);

        if(otac_env->env[conidx]->nb_svc == 0)
        {
            // Retrieve IAS characteristic
            prf_extract_svc_info(ind, OTAC_CHAR_HTS_MAX, &otac_hts_char[0],
                    otac_env->env[conidx]->hts.chars,
                    OTAC_DESC_HTS_MAX,
                    &otac_hts_char_desc[0],
                    &(otac_env->env[conidx]->hts.descs[0]) );

            //Even if we get multiple responses we only store 1 range
            otac_env->env[conidx]->hts.svc.shdl = ind->start_hdl;
            otac_env->env[conidx]->hts.svc.ehdl = ind->end_hdl;
        }
        otac_env->env[conidx]->nb_svc++;
    }
    return (KE_MSG_CONSUMED);
}



/**
 ****************************************************************************************
 * @brief Handles reception of the @ref OTAC_WR_MEAS_INTV_REQ message.
 * Check if the handle exists in profile(already discovered) and send request, otherwise
 * error to APP.
 * @param[in] msgid Id of the message received (probably unused).
 * @param[in] param Pointer to the parameters of the message.
 * @param[in] dest_id ID of the receiving task instance (probably unused).
 * @param[in] src_id ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int otac_wr_meas_intv_req_handler(ke_msg_id_t const msgid,
                                        struct otac_wr_meas_intv_req const *param,
                                        ke_task_id_t const dest_id,
                                        ke_task_id_t const src_id)
{
    int msg_status = KE_MSG_CONSUMED;
    // Status
    uint8_t status = GAP_ERR_NO_ERROR;

    uint8_t state = ke_state_get(dest_id);
    uint8_t conidx = KE_IDX_GET(dest_id);

    // Device Information Service Client Role Task Environment
    struct otac_env_tag *otac_env = PRF_ENV_GET(OTAC, otac);

    ASSERT_INFO(otac_env != NULL, dest_id, src_id);
    if((state == OTAC_IDLE) && (otac_env->env[conidx] != NULL))
    {
        uint16_t val_hdl  = ATT_INVALID_HANDLE;

        if ((otac_env->env[conidx]->hts.chars[OTAC_CHAR_HTS_MEAS_INTV].prop & ATT_CHAR_PROP_WR) != ATT_CHAR_PROP_WR)
        {
            status = PRF_ERR_NOT_WRITABLE;
        }
        else
        {
            val_hdl  = otac_env->env[conidx]->hts.chars[OTAC_CHAR_HTS_MEAS_INTV].val_hdl;

            if (val_hdl != ATT_INVALID_HANDLE)
            {
                // Send GATT Write Request
                prf_gatt_write(&(otac_env->prf_env), conidx, val_hdl,
                        (uint8_t *)&param->intv, sizeof(uint16_t), GATTC_WRITE);

                // store context of request and go into busy state
                otac_env->env[conidx]->operation = ke_param2msg(param);
                msg_status = KE_MSG_NO_FREE;
                ke_state_set(dest_id, OTAC_BUSY);
            }
            else
            {
                // invalid handle requested
                status = PRF_ERR_INEXISTENT_HDL;
            }
        }
    }
    else if(state != OTAC_FREE)
    {
        // request cannot be performed
        status = PRF_ERR_REQ_DISALLOWED;
    }

    // send error response if request fails
    if(status != GAP_ERR_NO_ERROR)
    {
        struct otac_wr_meas_intv_rsp *rsp = KE_MSG_ALLOC(OTAC_WR_MEAS_INTV_RSP, src_id, dest_id, otac_wr_meas_intv_rsp);
        rsp->status = status;
        ke_msg_send(rsp);
    }

    return (msg_status);
}




/**
 ****************************************************************************************
 * @brief Handles reception of the @ref OTAC_HEALTH_TEMP_NTF_CFG_REQ message.
 * It allows configuration of the peer ind/ntf/stop characteristic for a specified characteristic.
 * Will return an error code if that cfg char does not exist.
 * @param[in] msgid Id of the message received (probably unused).
 * @param[in] param Pointer to the parameters of the message.
 * @param[in] dest_id ID of the receiving task instance (probably unused).
 * @param[in] src_id ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int otac_health_temp_ntf_cfg_req_handler(ke_msg_id_t const msgid,
                                       struct otac_health_temp_ntf_cfg_req const *param,
                                       ke_task_id_t const dest_id,
                                       ke_task_id_t const src_id)
{
    // Client Characteristic Configuration Descriptor Handle
    uint16_t cfg_hdl = 0x0000;
    // Status
    uint8_t status = GAP_ERR_NO_ERROR;
    int msg_status = KE_MSG_CONSUMED;
    // Get the address of the environment
    struct otac_env_tag *otac_env = PRF_ENV_GET(OTAC, otac);
    uint8_t conidx = KE_IDX_GET(dest_id);

    switch(param->char_code)
    {
        case OTAC_CHAR_HTS_TEMP_MEAS://can only IND
            cfg_hdl = otac_env->env[conidx]->hts.descs[OTAC_DESC_HTS_TEMP_MEAS_CLI_CFG].desc_hdl;
            if(!((param->cfg_val == PRF_CLI_STOP_NTFIND)||
                 (param->cfg_val == PRF_CLI_START_IND)))
            {
                status = PRF_ERR_INVALID_PARAM;
            }
            break;

        case OTAC_CHAR_HTS_MEAS_INTV://can only IND
            cfg_hdl = otac_env->env[conidx]->hts.descs[OTAC_DESC_HTS_MEAS_INTV_CLI_CFG].desc_hdl;
            if(!((param->cfg_val == PRF_CLI_STOP_NTFIND)||
                 (param->cfg_val == PRF_CLI_START_IND)))
            {
                status = PRF_ERR_INVALID_PARAM;
            }
            break;

        case OTAC_CHAR_HTS_INTM_TEMP://can only NTF
            cfg_hdl = otac_env->env[conidx]->hts.descs[OTAC_DESC_HTS_INTM_MEAS_CLI_CFG].desc_hdl;
            if(!((param->cfg_val == PRF_CLI_STOP_NTFIND)||
                 (param->cfg_val == PRF_CLI_START_NTF)))
            {
                status = PRF_ERR_INVALID_PARAM;
            }
            break;

        default:
            //Let app know that one of the request params is incorrect
            status = PRF_ERR_INVALID_PARAM;
            break;
    }

    //Check if the handle value exists
    if ((cfg_hdl == ATT_INVALID_HANDLE))
    {
        status = PRF_ERR_INEXISTENT_HDL;
    }

    // no error detected
    if (status == GAP_ERR_NO_ERROR)
    {
        // Send GATT Write Request
        prf_gatt_write_ntf_ind(&otac_env->prf_env, conidx, cfg_hdl, param->cfg_val);
        // store context of request and go into busy state
        otac_env->env[conidx]->operation = ke_param2msg(param);
        ke_state_set(dest_id, OTAC_BUSY);
        msg_status = KE_MSG_NO_FREE;
    }
    // an error occurs send back response message
    else
    {
        struct otac_health_temp_ntf_cfg_rsp* rsp = KE_MSG_ALLOC(OTAC_HEALTH_TEMP_NTF_CFG_RSP,
                                                                src_id, dest_id,
                                                                otac_health_temp_ntf_cfg_rsp);
        rsp->status = status;
        ke_msg_send(rsp);
    }

    return (msg_status);
}


/**
 ****************************************************************************************
 * @brief Handles reception of the @ref OTAC_RD_REQ message.
 * Check if the handle exists in profile(already discovered) and send request, otherwise
 * error to APP.
 * @param[in] msgid Id of the message received (probably unused).
 * @param[in] param Pointer to the parameters of the message.
 * @param[in] dest_id ID of the receiving task instance (probably unused).
 * @param[in] src_id ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int otac_rd_char_req_handler(ke_msg_id_t const msgid,
                                    struct otac_rd_char_req const *param,
                                    ke_task_id_t const dest_id,
                                    ke_task_id_t const src_id)
{
    int msg_status = KE_MSG_CONSUMED;
    // Status
    uint8_t status = GAP_ERR_NO_ERROR;

    uint8_t state = ke_state_get(dest_id);
    uint8_t conidx = KE_IDX_GET(dest_id);
    // Device Information Service Client Role Task Environment
    struct otac_env_tag *otac_env = PRF_ENV_GET(OTAC, otac);

    ASSERT_INFO(otac_env != NULL, dest_id, src_id);
    if((state == OTAC_IDLE) && (otac_env->env[conidx] != NULL))
    {
        uint16_t search_hdl = ATT_INVALID_HDL;

        switch (param->char_code)
        {
            ///Read HTS Temp. Type
            case OTAC_RD_TEMP_TYPE:
            {
                search_hdl = otac_env->env[conidx]->hts.chars[OTAC_CHAR_HTS_TEMP_TYPE].val_hdl;
            }break;
            ///Read HTS Measurement Interval
            case OTAC_RD_MEAS_INTV:
            {
                search_hdl = otac_env->env[conidx]->hts.chars[OTAC_CHAR_HTS_MEAS_INTV].val_hdl;
            }break;

            ///Read HTS Temperature Measurement Client Cfg. Desc
            case OTAC_RD_TEMP_MEAS_CLI_CFG:
            {
                search_hdl = otac_env->env[conidx]->hts.descs[OTAC_DESC_HTS_TEMP_MEAS_CLI_CFG].desc_hdl;
            }break;
            ///Read HTS Intermediate Temperature Client Cfg. Desc
            case OTAC_RD_INTM_TEMP_CLI_CFG:
            {
                search_hdl = otac_env->env[conidx]->hts.descs[OTAC_DESC_HTS_INTM_MEAS_CLI_CFG].desc_hdl;
            }break;
            ///Read HTS Measurement Interval Client Cfg. Desc
            case OTAC_RD_MEAS_INTV_CLI_CFG:
            {
                search_hdl = otac_env->env[conidx]->hts.descs[OTAC_DESC_HTS_MEAS_INTV_CLI_CFG].desc_hdl;
            }break;
            ///Read HTS Measurement Interval Client Cfg. Desc
            case OTAC_RD_MEAS_INTV_VAL_RGE:
            {
                search_hdl = otac_env->env[conidx]->hts.descs[OTAC_DESC_HTS_MEAS_INTV_VAL_RGE].desc_hdl;
            }break;
            default:
            {
                status = GAP_ERR_INVALID_PARAM;
            }break;
        }

        //Check if handle is viable
        if (search_hdl != ATT_INVALID_HDL)
        {
            // perform read request
            //otac_env->env[conidx]->last_char_code = param->char_code;
            prf_read_char_send(&(otac_env->prf_env), conidx, otac_env->env[conidx]->hts.svc.shdl,
                    otac_env->env[conidx]->hts.svc.ehdl, search_hdl);

            // store context of request and go into busy state
            otac_env->env[conidx]->operation = ke_param2msg(param);
            msg_status = KE_MSG_NO_FREE;
            ke_state_set(dest_id, OTAC_BUSY);
        }
        else if(status == GAP_ERR_NO_ERROR)
        {
            // invalid handle requested
            status = PRF_ERR_INEXISTENT_HDL;
        }
    }
    else if(state != OTAC_FREE)
    {
        // request cannot be performed
        status = PRF_ERR_REQ_DISALLOWED;
    }

    // send error response if request fails
    if(status != GAP_ERR_NO_ERROR)
    {
        prf_client_att_info_rsp(&(otac_env->prf_env), conidx, OTAC_RD_CHAR_RSP,
                status, NULL);
    }

    return (msg_status);
}

/**
 ****************************************************************************************
 * @brief Handles reception of the @ref GATTC_READ_IND message.
 * Generic event received after every simple read command sent to peer server.
 * @param[in] msgid Id of the message received (probably unused).
 * @param[in] param Pointer to the parameters of the message.
 * @param[in] dest_id ID of the receiving task instance (probably unused).
 * @param[in] src_id ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int gattc_read_ind_handler(ke_msg_id_t const msgid,
                                    struct gattc_read_ind const *param,
                                    ke_task_id_t const dest_id,
                                    ke_task_id_t const src_id)
{
    // Get the address of the environment
    struct otac_env_tag *otac_env = PRF_ENV_GET(OTAC, otac);
    uint8_t conidx = KE_IDX_GET(dest_id);

    // send attribute information
    prf_client_att_info_rsp(&(otac_env->prf_env), conidx, OTAC_RD_CHAR_RSP, GAP_ERR_NO_ERROR, param);

    return (KE_MSG_CONSUMED);
}

/**
 ****************************************************************************************
 * @brief Handles reception of the @ref GATTC_CMP_EVT message.
 * This generic event is received for different requests, so need to keep track.
 * @param[in] msgid Id of the message received (probably unused).
 * @param[in] param Pointer to the parameters of the message.
 * @param[in] dest_id ID of the receiving task instance (probably unused).
 * @param[in] src_id ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int gattc_cmp_evt_handler(ke_msg_id_t const msgid,
                                struct gattc_cmp_evt const *param,
                                ke_task_id_t const dest_id,
                                ke_task_id_t const src_id)
{
    uint8_t state = ke_state_get(dest_id);
    uint8_t conidx = KE_IDX_GET(dest_id);

    // Get the address of the environment
    struct otac_env_tag *otac_env = PRF_ENV_GET(OTAC, otac);

    if(state == OTAC_BUSY)
    {
        switch(otac_env->env[conidx]->operation->id)
        {
            case OTAC_ENABLE_REQ:
            {
                uint8_t status;
                if(otac_env->env[conidx]->nb_svc == 1)
                {
                    status = prf_check_svc_char_validity(OTAC_CHAR_HTS_MAX,
                            otac_env->env[conidx]->hts.chars, otac_hts_char);

                    otac_env->env[conidx]->nb_svc =0;
                }
                // Too many services found only one such service should exist
                else if(otac_env->env[conidx]->nb_svc > 1)
                {
                    status = PRF_ERR_MULTIPLE_SVC;
                }
                else
                {
                    status = PRF_ERR_STOP_DISC_CHAR_MISSING;
                }

                otac_enable_rsp_send(otac_env, conidx, status);
            }break;


            case OTAC_HEALTH_TEMP_NTF_CFG_REQ:
            {
                // send response of the notification configuration request
                struct otac_health_temp_ntf_cfg_rsp * rsp = KE_MSG_ALLOC(OTAC_HEALTH_TEMP_NTF_CFG_RSP,
                        prf_dst_task_get(&(otac_env->prf_env), conidx), dest_id, otac_health_temp_ntf_cfg_rsp);
                // set error status
                rsp->status =param->status;
                ke_msg_send(rsp);
            }break;

            case OTAC_RD_CHAR_REQ:
            {
                if(param->status != GAP_ERR_NO_ERROR)
                {
                    // send attribute information
                    prf_client_att_info_rsp(&(otac_env->prf_env), conidx, OTAC_RD_CHAR_RSP, param->status, NULL);
                }
            }break;

            case OTAC_WR_MEAS_INTV_REQ:
            {
                struct otac_wr_meas_intv_rsp *rsp = KE_MSG_ALLOC(OTAC_WR_MEAS_INTV_RSP,
                        prf_dst_task_get(&(otac_env->prf_env), conidx), dest_id, otac_wr_meas_intv_rsp);
                rsp->status = param->status;
                ke_msg_send(rsp);
            }break;

            default: break;
        }


        //free operation
        if((otac_env->env[conidx] != NULL) && (otac_env->env[conidx]->operation != NULL))
        {
            ke_free(otac_env->env[conidx]->operation);
            otac_env->env[conidx]->operation = NULL;
        }
        // set state to IDLE
        ke_state_set(dest_id, OTAC_IDLE);
    }

    return (KE_MSG_CONSUMED);
}


/**
 ****************************************************************************************
 * @brief Handles reception of the @ref GATTC_EVENT_IND message.
 * @param[in] msgid Id of the message received (probably unused).
 * @param[in] param Pointer to the parameters of the message.
 * @param[in] dest_id ID of the receiving task instance (probably unused).
 * @param[in] src_id ID of the sending task instance.
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int gattc_event_ind_handler(ke_msg_id_t const msgid,
                                        struct gattc_event_ind const *param,
                                        ke_task_id_t const dest_id,
                                        ke_task_id_t const src_id)
{
    // Get the address of the environment
    struct otac_env_tag *otac_env = PRF_ENV_GET(OTAC, otac);
    uint8_t conidx = KE_IDX_GET(dest_id);

    // Temperature Measurement Char.
     if(param->handle == otac_env->env[conidx]->hts.chars[OTAC_CHAR_HTS_TEMP_MEAS].val_hdl)
    {
        // confirm that indication has been correctly received
        struct gattc_event_cfm * cfm = KE_MSG_ALLOC(GATTC_EVENT_CFM, src_id, dest_id, gattc_event_cfm);
        cfm->handle = param->handle;
        ke_msg_send(cfm);

        otac_unpack_temp(otac_env, (uint8_t *)&(param->value), param->length,
                ((param->type == GATTC_NOTIFY) ? HTP_TEMP_INTERM : HTP_TEMP_STABLE), conidx);
    }
    // Intermediate Temperature Measurement Char.
    else if(param->handle == otac_env->env[conidx]->hts.chars[OTAC_CHAR_HTS_INTM_TEMP].val_hdl)
    {
        otac_unpack_temp(otac_env, (uint8_t *)&(param->value), param->length,
                ((param->type == GATTC_NOTIFY) ? HTP_TEMP_INTERM : HTP_TEMP_STABLE), conidx);
    }
    // Measurement Interval Char.
    else if (param->handle == otac_env->env[conidx]->hts.chars[OTAC_CHAR_HTS_MEAS_INTV].val_hdl)
    {
        // confirm that indication has been correctly received
        struct gattc_event_cfm * cfm = KE_MSG_ALLOC(GATTC_EVENT_CFM, src_id, dest_id, gattc_event_cfm);
        cfm->handle = param->handle;
        ke_msg_send(cfm);

        struct otac_meas_intv_ind * ind = KE_MSG_ALLOC(OTAC_MEAS_INTV_IND,
                                            prf_dst_task_get(&otac_env->prf_env, conidx),
                                            prf_src_task_get(&otac_env->prf_env, conidx),
                                            otac_meas_intv_ind);

        memcpy(&ind->intv, &param->value[0], sizeof(uint16_t));

        ke_msg_send(ind);
    }
    return (KE_MSG_CONSUMED);
}

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */


/* Default State handlers definition. */
const struct ke_msg_handler otac_default_state[] =
{
    {OTAC_ENABLE_REQ,               (ke_msg_func_t) otac_enable_req_handler},
    {GATTC_SDP_SVC_IND,             (ke_msg_func_t) gattc_sdp_svc_ind_handler},
    {OTAC_HEALTH_TEMP_NTF_CFG_REQ,  (ke_msg_func_t) otac_health_temp_ntf_cfg_req_handler},
    {OTAC_WR_MEAS_INTV_REQ,         (ke_msg_func_t) otac_wr_meas_intv_req_handler},
    {OTAC_RD_CHAR_REQ,              (ke_msg_func_t) otac_rd_char_req_handler},
    {GATTC_READ_IND,                (ke_msg_func_t) gattc_read_ind_handler},
    {GATTC_CMP_EVT,                 (ke_msg_func_t) gattc_cmp_evt_handler},
    {GATTC_EVENT_IND,               (ke_msg_func_t) gattc_event_ind_handler},
    {GATTC_EVENT_REQ_IND,           (ke_msg_func_t) gattc_event_ind_handler},
};

// Specifies the message handlers that are common to all states.
const struct ke_state_handler otac_default_handler = KE_STATE_HANDLER(otac_default_state);

#endif //BLE_HT_COLLECTOR

/// @} OTACTASK
