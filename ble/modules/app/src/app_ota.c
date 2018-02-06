/**
 ****************************************************************************************
 *
 * @file app_ota.c
 *
 * @brief Health Thermometer Application entry point
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

#if (BLE_APP_OTA)

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "app_ota.h"                  // Health Thermometer Application Definitions
#include "app.h"                     // Application Definitions
#include "app_task.h"                // application task definitions
#include "otat_task.h"               // health thermometer functions
#include "co_bt.h"
#include "prf_types.h"
#include "prf_utils.h"
#include "arch.h"                    // Platform Definitions

#include "co_math.h"
#include "ke_timer.h"
#ifdef BLE_OTA_BL_MODE_EN
#include "fota_ble_handler.h"
#endif
#include "fota.h"
#include "drvi_clock.h"

#if (DISPLAY_SUPPORT)
#include "app_display.h"
#include "display.h"
#endif //DISPLAY_SUPPORT

/*
 * DEFINES
 ****************************************************************************************
 */

/// Initial Temperature Value : 37c
#define APP_OTA_TEMP_VALUE_INIT       (3700)
/// Temperature Step
#define APP_OTA_TEMP_STEP_INIT        (10)
/// Measurement Interval Value Min
#define APP_OTA_MEAS_INTV_MIN         (1)
/// Measurement Interval Value Max
#define APP_OTA_MEAS_INTV_MAX         (30)

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */
extern void venus_ready_to_bootloader(void);

/// health thermometer application environment structure
struct app_ota_env_tag app_ota_env;

/*
 * LOCAL FUNCTION DEFINITIONS
 ****************************************************************************************
 */
/*
static void app_ota_temp_send(void)
{
    // Temperature Value
    int32_t value = (int32_t)(app_ota_env.temp_value);

    // The value is a float value, set the exponent
    value |= 0xFE000000;

    // Allocate the OTAT_TEMP_SEND_REQ message
    struct otat_temp_send_req * req = KE_MSG_ALLOC(OTAT_TEMP_SEND_REQ,
                                                    prf_get_task_from_id(TASK_ID_OTAT),
                                                    TASK_APP,
                                                    otat_temp_send_req);

    // Stable => Temperature Measurement Char.
    req->stable_meas = 0x01;

    // Temperature Measurement Value
    req->temp_meas.temp         = value;
//    req->temp_meas.time_stamp = 0;
    req->temp_meas.flags        = HTP_FLAG_CELSIUS | HTP_FLAG_TYPE;
    req->temp_meas.type         = app_ota_env.temp_meas_type;

    ke_msg_send(req);
}

void app_ota_period_meas_send(bool update_step, uint32_t step_data, bool update_hr, uint16_t hr_data, uint32_t calorie)
{
    // Allocate the OTAT_TEMP_SEND_REQ message
    struct otat_period_meas_send_req * req = KE_MSG_ALLOC(OTAT_TEMP_SEND_REQ,
                                                    prf_get_task_from_id(TASK_ID_OTAT),
                                                    TASK_APP,
                                                    otat_period_meas_send_req);
                                                    
    req->period_meas.vid = 0xCC;
    req->period_meas.type = 0xF1;   // step 
    req->period_meas.update_step_flag = update_step;
    req->period_meas.step = step_data;
    req->period_meas.type2 = 0xF2; // hr
    req->period_meas.update_hr_flag = update_hr;
    req->period_meas.hr = hr_data;
    req->period_meas.calorie = calorie;
                                              

    ke_msg_send(req);
}


void app_ota_swim_meas_send(uint8_t swim_en, uint8_t style_type, uint32_t dwSwimCnt, uint32_t dwSwimLap, uint32_t dwTimestamp)
{
    // Allocate the OTAT_TEMP_SEND_REQ message
    struct otat_swim_meas_send_req * req = KE_MSG_ALLOC(OTAT_TEMP_SWIM_MEAS_REQ,
                                                    prf_get_task_from_id(TASK_ID_OTAT),
                                                    TASK_APP,
                                                    otat_swim_meas_send_req);

    req->swim_meas.vid = 0xCC;
    req->swim_meas.type = 0xF4;
    req->swim_meas.is_Swim_En = swim_en;
    req->swim_meas.style_type = style_type;
    req->swim_meas.dwSwimCnt = dwSwimCnt;
    req->swim_meas.dwSwimLap = dwSwimLap;
    req->swim_meas.dwTimestamp = dwTimestamp;
                                                     
    ke_msg_send(req);
}



void app_ota_history_send(uint8_t id)
{
    // Allocate the OTAT_TEMP_SEND_REQ message
    struct otat_history_send_req * req = KE_MSG_ALLOC(OTAT_TEMP_SEND_REQ,
                                                    prf_get_task_from_id(TASK_ID_OTAT),
                                                    TASK_APP,
                                                    otat_history_send_req);

    req->history_meas.vid = 0xCC;
    req->history_meas.type = id;
    req->history_meas.total = 0;
    req->history_meas.rec_index = 0;    
                                          

    ke_msg_send(req);
}
*/

//void app_ota_notify_send(uint8_t *tx_data)
void app_ota_notify_send(uint8_t *tx_data, uint8_t length)
{
    // Allocate the OTAT_TEMP_SEND_REQ message
    struct otat_notify_send_req * req = KE_MSG_ALLOC(OTAT_CTRL_PT_SEND_NOTIFY,
                                                    prf_get_task_from_id(TASK_ID_OTAT),
                                                    TASK_APP,
                                                    otat_notify_send_req);


    req->lenth = length;
    memcpy(req->eArray, tx_data, length);
                                             

    ke_msg_send(req);
}

#if (DISPLAY_SUPPORT)
static void app_ota_update_type_string(uint8_t temp_type)
{
    switch (temp_type)
    {
        case 0:
            strcpy(app_ota_env.temp_type_string, "NONE");
            break;
        case 1:
            strcpy(app_ota_env.temp_type_string, "ARMPIT");
            break;
        case 2:
            strcpy(app_ota_env.temp_type_string, "BODY");
            break;
        case 3:
            strcpy(app_ota_env.temp_type_string, "EAR");
            break;
        case 4:
            strcpy(app_ota_env.temp_type_string, "FINGER");
            break;
        case 5:
            strcpy(app_ota_env.temp_type_string, "GASTRO-INT");
            break;
        case 6:
            strcpy(app_ota_env.temp_type_string, "MOUTH");
            break;
        case 7:
            strcpy(app_ota_env.temp_type_string, "RECTUM");
            break;
        case 8:
            strcpy(app_ota_env.temp_type_string, "TOE");
            break;
        case 9:
            strcpy(app_ota_env.temp_type_string, "TYMPANUM");
            break;
        default:
            strcpy(app_ota_env.temp_type_string, "UNKNOWN");
            break;
    }
}
#endif

/*
 * GLOBAL FUNCTION DEFINITIONS
 ****************************************************************************************
 */

void app_ota_init(void)
{
    // Reset the environment
    memset(&app_ota_env, 0, sizeof(app_ota_env));
/*
    // Initial measurement interval : 0s
    app_ota_env.otat_meas_intv   = 0;
    // Initial temperature value : 37.00 C
    app_ota_env.temp_value       = APP_OTA_TEMP_VALUE_INIT;
    // Initial temperature step : 0.20 C
    app_ota_env.temp_step        = APP_OTA_TEMP_STEP_INIT;
    // Initial temperature type : ARMPIT
    app_ota_env.temp_meas_type   = 1;
*/
    //TODO: Add a state for the module
}

void app_ota_stop_timer (void)
{
    // Stop the timer used for the measurement interval if enabled
    if (app_ota_env.timer_enable)
    {
        ke_timer_clear(APP_OTA_MEAS_INTV_TIMER, TASK_APP);
        app_ota_env.timer_enable = false;
    }
}

void app_ota_add_otas(void)
{
    struct otat_db_cfg* db_cfg;
    // Allocate the OTAT_CREATE_DB_REQ
    struct gapm_profile_task_add_cmd *req = KE_MSG_ALLOC_DYN(GAPM_PROFILE_TASK_ADD_CMD,
                                                  TASK_GAPM, TASK_APP,
                                                  gapm_profile_task_add_cmd, sizeof(struct otat_db_cfg));
    // Fill message
    req->operation = GAPM_PROFILE_TASK_ADD;
    req->sec_lvl = PERM(SVC_AUTH, ENABLE);
    req->prf_task_id = TASK_ID_OTAT;
    req->app_task = TASK_APP;
    req->start_hdl = 0;

    // Set parameters
    db_cfg = (struct otat_db_cfg* ) req->param;
    // All features are supported
    db_cfg->features = OTAT_ALL_FEAT_SUP;

    // Send the message
    ke_msg_send(req);
}

void app_ota_enable_prf(uint8_t conidx)
{
    // Allocate the message
    struct otat_enable_req * req = KE_MSG_ALLOC(OTAT_ENABLE_REQ,
                                                prf_get_task_from_id(TASK_ID_OTAT),
                                                TASK_APP,
                                                otat_enable_req);

    // Fill in the parameter structure
    req->conidx        = conidx;
    // NTF/IND initial status - Disabled
    req->ntf_ind_cfg   = PRF_CLI_STOP_NTFIND;

    // Send the message
    ke_msg_send(req);
}
/*

void app_ota_ctrl_pt_write(struct gattc_write_req_ind const * param)
{
#ifdef BLE_OTA_BL_MODE_EN    
    fota_on_ctrl_pt_write(param);
#endif    
}

void app_ota_pkt_write_cmd(struct gattc_write_req_ind const * param)
{
#ifdef BLE_OTA_BL_MODE_EN    
    fota_on_write(param);
#endif    
}
*/

/**
 ****************************************************************************************
 * OTA Application Functions
 ****************************************************************************************
 */
/*
void app_ota_temp_inc(void)
{
    app_ota_env.temp_value += app_ota_env.temp_step;

    #if (DISPLAY_SUPPORT)
    app_display_update_temp_val_screen(app_ota_env.temp_value);
    #endif //DISPLAY_SUPPORT

    app_ota_temp_send();
}

void app_ota_temp_dec(void)
{
    app_ota_env.temp_value -= app_ota_env.temp_step;

    #if (DISPLAY_SUPPORT)
    app_display_update_temp_val_screen(app_ota_env.temp_value);
    #endif //DISPLAY_SUPPORT

    app_ota_temp_send();
}

void app_ota_temp_type_inc(void)
{
    app_ota_env.temp_meas_type = (uint8_t)(((int)app_ota_env.temp_meas_type + 1)%10);

    #if (DISPLAY_SUPPORT)
    app_ota_update_type_string(app_ota_env.temp_meas_type);
    app_display_update_temp_type_screen(app_ota_env.temp_type_string);
    #endif //DISPLAY_SUPPORT
}

void app_ota_temp_type_dec(void)
{
    if (((int)app_ota_env.temp_meas_type-1) < 0)
    {
        app_ota_env.temp_meas_type = 0x09;
    }
    else
    {
        app_ota_env.temp_meas_type = app_ota_env.temp_meas_type - 1;
    }

    #if DISPLAY_SUPPORT
    app_ota_update_type_string(app_ota_env.temp_meas_type);
    app_display_update_temp_type_screen(app_ota_env.temp_type_string);
    #endif //DISPLAY_SUPPORT
}
*/
/****************************************************************************************
 * MESSAGE HANDLERS
 ****************************************************************************************/

/**
 ****************************************************************************************
 * @brief Handles measurement interval start indication from the Health Thermometer
 *        profile.
 *        Start or stop a timer following the value of the param intv.
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAP).
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
 /*
static int otat_meas_intv_chg_req_ind_handler(ke_msg_id_t const msgid,
                                          struct otat_meas_intv_chg_req_ind const *param,
                                          ke_task_id_t const dest_id,
                                          ke_task_id_t const src_id)
{
    // Store the received Measurement Interval value
    app_ota_env.otat_meas_intv = param->intv;

    // Check the new Measurement Interval Value
    if (app_ota_env.otat_meas_intv != 0)
    {
        // Check if a Timer already exists
        if (!app_ota_env.timer_enable)
        {
            // Set a Timer
            ke_timer_set(APP_OTA_MEAS_INTV_TIMER, TASK_APP, app_ota_env.otat_meas_intv*100);
            app_ota_env.timer_enable = true;
        }
        else
        {
            // Clear the previous timer
            ke_timer_clear(APP_OTA_MEAS_INTV_TIMER, TASK_APP);
            // Create a new timer with the received measurement interval
            ke_timer_set(APP_OTA_MEAS_INTV_TIMER, TASK_APP, app_ota_env.otat_meas_intv*100);
        }
    }
    else
    {
        // Check if a Timer exists
        if (app_ota_env.timer_enable)
        {
            // Measurement Interval is 0, clear the timer
            ke_timer_clear(APP_OTA_MEAS_INTV_TIMER, TASK_APP);
            app_ota_env.timer_enable = false;
        }
    }

    // Allocate the message
    struct otat_meas_intv_chg_cfm * cfm = KE_MSG_ALLOC(OTAT_MEAS_INTV_CHG_CFM,
                                                prf_get_task_from_id(TASK_ID_OTAT),
                                                TASK_APP,
                                                otat_meas_intv_chg_cfm);

    // Set data
    cfm->conidx = KE_IDX_GET(dest_id);
    cfm->status = 0;

    // Send the message
    ke_msg_send(cfm);


    return (KE_MSG_CONSUMED);
}
*/
/*
static int otat_temp_send_rsp_handler(ke_msg_id_t const msgid,
                                        struct otat_temp_send_rsp const *param,
                                        ke_task_id_t const dest_id,
                                        ke_task_id_t const src_id)
{
    // Do nothing
    return (KE_MSG_CONSUMED);
}
*/

static int otat_packet_send_cmd_handler(ke_msg_id_t const msgid,
                                        struct otat_packet_send_cmd const *param,
                                        ke_task_id_t const dest_id,
                                        ke_task_id_t const src_id)
{
#ifdef BLE_OTA_BL_MODE_EN    
    fota_on_write(param);
    cc6801_ClockDelayUs(400);
    
/*
    uint8_t test_buff[15] = {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};
    app_ota_notify_send(test_buff, 16);
    app_ota_notify_send((uint8_t*)param->value, param->length);
*/    
#else
        uint8_t  buffer[3]; 
        uint8_t  index = 0;
    
        buffer[index++] = 0x20;//DFU_OP_RESPONSE_CODE;
        
        // Encode the Request Op code
        buffer[index++] = 0x01;//BLE_DFU_OP_CODE_CREATE_OBJECT;
        
        // Encode the Response Value.
        buffer[index++] = 0x01;//NRF_DFU_RES_CODE_SUCCESS;
    
        app_ota_notify_send(buffer, index);

        // Ready to enter bootloader mode:
#if FSTORAGE_ENABLED == 1
        FotaFlashInit(true);
#else
        FotaFlashInit(false);
#endif        
        BootloaderSettingInit();
        EnterBootloader(venus_ready_to_bootloader);
#endif 
    return (KE_MSG_CONSUMED);
}

#ifdef BLE_OTA_BL_MODE_EN  
static int otat_ctrl_pt_send_req_handler(ke_msg_id_t const msgid,
                                        struct otat_packet_send_cmd const *param,
                                        ke_task_id_t const dest_id,
                                        ke_task_id_t const src_id)
{

    fota_on_ctrl_pt_write(param);


    return (KE_MSG_CONSUMED);
}
#endif

static int otat_cfg_indntf_ind_handler(ke_msg_id_t const msgid,
                                        struct otat_cfg_indntf_ind const *param,
                                        ke_task_id_t const dest_id,
                                        ke_task_id_t const src_id)
{
    // Do nothing

    return (KE_MSG_CONSUMED);
}

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
static int app_ota_meas_intv_timer_handler(ke_msg_id_t const msgid,
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

    app_ota_env.temp_value += sign*rand_temp_step;

    // Send the new temperature
    //app_ota_temp_send();

    #if (DISPLAY_SUPPORT)
    app_display_update_temp_val_screen(app_ota_env.temp_value);
    #endif //DISPLAY_SUPPORT

    // Reset the Timer (Measurement Interval is not 0 if we are here)
    ke_timer_set(APP_OTA_MEAS_INTV_TIMER, TASK_APP, app_ota_env.otat_meas_intv*100);

    return (KE_MSG_CONSUMED);
}

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
static int app_ota_msg_handler(ke_msg_id_t const msgid,
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
const struct ke_msg_handler app_ota_msg_handler_list[] =
{
    // Note: first message is latest message checked by kernel so default is put on top.
    {KE_MSG_DEFAULT_HANDLER,        (ke_msg_func_t)app_ota_msg_handler},

//  {OTAT_ENABLE_RSP,               (ke_msg_func_t)otat_enable_rsp_handler},
//    {OTAT_TEMP_SEND_RSP,            (ke_msg_func_t)otat_temp_send_rsp_handler},
//    {OTAT_MEAS_INTV_CHG_REQ_IND,    (ke_msg_func_t)otat_meas_intv_chg_req_ind_handler},
    {OTAT_PACKET_SEND_CMD,          (ke_msg_func_t)otat_packet_send_cmd_handler},
#ifdef BLE_OTA_BL_MODE_EN        
    {OTAT_CTRL_PT_SEND_REQ,          (ke_msg_func_t)otat_ctrl_pt_send_req_handler},
#endif        
    {OTAT_CFG_INDNTF_IND,           (ke_msg_func_t)otat_cfg_indntf_ind_handler},

    {APP_OTA_MEAS_INTV_TIMER,        (ke_msg_func_t)app_ota_meas_intv_timer_handler},
};

const struct ke_state_handler app_ota_table_handler =
    {&app_ota_msg_handler_list[0], (sizeof(app_ota_msg_handler_list)/sizeof(struct ke_msg_handler))};

#endif //BLE_APP_OTA

/// @} APP
