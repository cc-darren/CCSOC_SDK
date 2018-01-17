/**
 ****************************************************************************************
 *
 * @file app_ccps.c
 *
 * @brief CloudChip Proprietary Services Application entry point
 *
 * Copyright (C) RivieraWaves 2009-2015
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

#include "co_math.h"
#include "ke_timer.h"
#include "drvi_clock.h"

#if (DISPLAY_SUPPORT)
#include "app_display.h"
#include "display.h"
#endif //DISPLAY_SUPPORT

/*
 * DEFINES
 ****************************************************************************************
 */


/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */
extern void venus_ready_to_bootloader(void);

/// CCPS application environment structure
struct app_ccps_env_tag app_ccps_env;

/*
 * LOCAL FUNCTION DEFINITIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Notify user defined data
 *
 * @param[in] tx_data    Pointer to the data that would be sent by notifiction packets
 * @param[in] length     Length of the data
 * @return If the message was consumed or not.
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


/**
 ****************************************************************************************
 * @brief Indicate user defined data
 *
 * @param[in] tx_data    Pointer to the data that would be sent by indication packets
 * @param[in] length     Length of the data
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
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


#if (DISPLAY_SUPPORT)
static void app_ccps_update_type_string(uint8_t temp_type)
{
    switch (temp_type)
    {
        case 0:
            strcpy(app_ccps_env.temp_type_string, "NONE");
            break;
        case 1:
            strcpy(app_ccps_env.temp_type_string, "ARMPIT");
            break;
        case 2:
            strcpy(app_ccps_env.temp_type_string, "BODY");
            break;
        case 3:
            strcpy(app_ccps_env.temp_type_string, "EAR");
            break;
        case 4:
            strcpy(app_ccps_env.temp_type_string, "FINGER");
            break;
        case 5:
            strcpy(app_ccps_env.temp_type_string, "GASTRO-INT");
            break;
        case 6:
            strcpy(app_ccps_env.temp_type_string, "MOUTH");
            break;
        case 7:
            strcpy(app_ccps_env.temp_type_string, "RECTUM");
            break;
        case 8:
            strcpy(app_ccps_env.temp_type_string, "TOE");
            break;
        case 9:
            strcpy(app_ccps_env.temp_type_string, "TYMPANUM");
            break;
        default:
            strcpy(app_ccps_env.temp_type_string, "UNKNOWN");
            break;
    }
}
#endif

/*
 * GLOBAL FUNCTION DEFINITIONS
 ****************************************************************************************
 */

void app_ccps_init(void)
{
    // Reset the environment
    memset(&app_ccps_env, 0, sizeof(app_ccps_env));

}
/*
void app_ccps_stop_timer (void)
{
    // Stop the timer used for the measurement interval if enabled
    if (app_ccps_env.timer_enable)
    {
        ke_timer_clear(APP_REPORT_MEAS_INTV_TIMER, TASK_APP);
        app_ccps_env.timer_enable = false;
    }
}
*/
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


static int ccps_packet_send_cmd_handler(ke_msg_id_t const msgid,
                                        struct ccps_packet_send_cmd const *param,
                                        ke_task_id_t const dest_id,
                                        ke_task_id_t const src_id)
{
    // Do nothing

    return (KE_MSG_CONSUMED);
}


static int ccps_cfg_indntf_ind_handler(ke_msg_id_t const msgid,
                                        struct ccps_cfg_indntf_ind const *param,
                                        ke_task_id_t const dest_id,
                                        ke_task_id_t const src_id)
{
    // Do nothing

    return (KE_MSG_CONSUMED);
}
                                        
#if 0

/**
 ****************************************************************************************
 * @brief Handles health thermometer timer
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAP).
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */

static int app_ccps_meas_intv_timer_handler(ke_msg_id_t const msgid,
                                          void const *param,
                                          ke_task_id_t const dest_id,
                                          ke_task_id_t const src_id)
{
    // Random generation of a temperature value
    uint32_t rand_temp_step;
    // Sign used to know if the temperature will be increased or decreased
    int8_t sign;

    // Generate temperature step
    rand_temp_step = (uint32_t)(co_rand_word()%20);
    // Increase or decrease the temperature value
    sign = (int8_t)(rand_temp_step & 0x00000001);

    if (!sign)
    {
        sign = -1;
    }

    app_ccps_env.temp_value += sign*rand_temp_step;

    // Send the new temperature
    //app_ccps_temp_send();

    #if (DISPLAY_SUPPORT)
    app_display_update_temp_val_screen(app_ccps_env.temp_value);
    #endif //DISPLAY_SUPPORT

    // Reset the Timer (Measurement Interval is not 0 if we are here)
    ke_timer_set(APP_REPORT_MEAS_INTV_TIMER, TASK_APP, app_ccps_env.ccps_meas_intv*100);

    return (KE_MSG_CONSUMED);
}
#endif
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

    //{APP_OTA_MEAS_INTV_TIMER,        (ke_msg_func_t)app_ccps_meas_intv_timer_handler},
};

const struct ke_state_handler app_ccps_table_handler =
    {&app_ccps_msg_handler_list[0], (sizeof(app_ccps_msg_handler_list)/sizeof(struct ke_msg_handler))};

#endif //BLE_APP_REPORT

/// @} APP
