/* Copyright (c) 2019 Cloudchip, Inc. All Rights Reserved.
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
 * @file appm_task.c
 *
 * @brief RW APP Task implementation
 *
 * Copyright (C) RivieraWaves 2009-2015
 *
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup APPTASK
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwip_config.h"          // SW configuration

#if (BLE_APP_PRESENT)

#include "app_task.h"             // Application Manager Task API
#include "app.h"                  // Application Manager Definition
#include "gapc_task.h"            // GAP Controller Task API
#include "gapm_task.h"            // GAP Manager Task API
#include "arch.h"                 // Platform Definitions
#include <string.h>
#include "ke_timer.h"             // Kernel timer

#if (BLE_APP_SEC)
#include "app_sec.h"              // Security Module Definition
#endif //(BLE_APP_SEC)

#if (BLE_APP_HT)
#include "app_ht.h"               // Health Thermometer Module Definition
#include "htpt_task.h"
#endif //(BLE_APP_HT)

#if (BLE_APP_DIS)
#include "app_dis.h"              // Device Information Module Definition
#include "diss_task.h"
#endif //(BLE_APP_DIS)

#if (BLE_APP_BATT)
#include "app_batt.h"             // Battery Module Definition
#include "bass_task.h"
#endif //(BLE_APP_BATT)

#if (BLE_APP_HID)
#include "app_hid.h"              // HID Module Definition
#include "hogpd_task.h"
#endif //(BLE_APP_HID)

#ifdef BLE_APP_AM0
#include "am0_app.h"             // Audio Mode 0 Application
#endif //defined(BLE_APP_AM0)

#if (BLE_APP_OTA)
#include "app_ota.h"               // OTA Module Definition
#include "otat_task.h"
#endif //(BLE_APP_OTA)

#if (BLE_APP_CCPS)
#include "app_ccps.h"               // CCPS Module Definition
#include "ccps_task.h"
#endif //(BLE_APP_CCPS)

#if (BLE_APP_BLEUARTS)
#include "app_bleuarts.h"			// BLEUARTS Module Definition
#include "bleuarts_task.h"
#endif //(BLE_APP_BLEUARTS)

#if (DISPLAY_SUPPORT)
#include "app_display.h"          // Application Display Definition
#endif //(DISPLAY_SUPPORT)

#ifdef SW_TIMER_BY_KERNEL
#include "sw_timer.h"
#include "tracer.h"
#endif
/*
 * LOCAL FUNCTION DEFINITIONS
 ****************************************************************************************
 */

static uint8_t appm_get_handler(const struct ke_state_handler *handler_list,
                                ke_msg_id_t msgid,
                                void *param,
                                ke_task_id_t src_id)
{
    // Counter
    uint8_t counter;

    // Get the message handler function by parsing the message table
    for (counter = handler_list->msg_cnt; 0 < counter; counter--)
    {
//Louis, 2016.08.16, re-coding for compiler issue
//        struct ke_msg_handler handler = (struct ke_msg_handler)(*(handler_list->msg_table + counter - 1));
        struct ke_msg_handler handler = (*(handler_list->msg_table + counter - 1));

        if ((handler.id == msgid) ||
            (handler.id == KE_MSG_DEFAULT_HANDLER))
        {
            // If handler is NULL, message should not have been received in this state
            ASSERT_ERR(handler.func);

            return (uint8_t)(handler.func(msgid, param, TASK_APP, src_id));
        }
    }

    // If we are here no handler has been found, drop the message
    return (KE_MSG_CONSUMED);
}

/*
 * MESSAGE HANDLERS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAP).
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int app_adv_timeout_handler(ke_msg_id_t const msgid,
                                   void const *param,
                                   ke_task_id_t const dest_id,
                                   ke_task_id_t const src_id)
{
    #if (BLE_APP_HID)
    #else
    // Stop advertising
    appm_stop_advertising();
    #endif

    return (KE_MSG_CONSUMED);
}

/**
 ****************************************************************************************
 * @brief Handles ready indication from the GAP. - Reset the stack
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAP).
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int gapm_device_ready_ind_handler(ke_msg_id_t const msgid,
                                         void const *param,
                                         ke_task_id_t const dest_id,
                                         ke_task_id_t const src_id)
{
    // Application has not been initialized
    ASSERT_ERR(ke_state_get(dest_id) == APPM_INIT);

    // Reset the stack
    struct gapm_reset_cmd* cmd = KE_MSG_ALLOC(GAPM_RESET_CMD,
                                              TASK_GAPM, TASK_APP,
                                              gapm_reset_cmd);

    cmd->operation = GAPM_RESET;

    ke_msg_send(cmd);

    return (KE_MSG_CONSUMED);
}

/**
 ****************************************************************************************
 * @brief Handles GAP manager command complete events.
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAP).
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int gapm_cmp_evt_handler(ke_msg_id_t const msgid,
                                struct gapm_cmp_evt const *param,
                                ke_task_id_t const dest_id,
                                ke_task_id_t const src_id)
{
    switch(param->operation)
    {
        // Reset completed
        case (GAPM_RESET):
        {
            if(param->status == GAP_ERR_NO_ERROR)
            {
                #if (BLE_APP_HID)
                app_hid_start_mouse();
                #endif //(BLE_APP_HID)

                // Set Device configuration
                struct gapm_set_dev_config_cmd* cmd = KE_MSG_ALLOC(GAPM_SET_DEV_CONFIG_CMD,
                                                                   TASK_GAPM, TASK_APP,
                                                                   gapm_set_dev_config_cmd);
                // Set the operation
                cmd->operation = GAPM_SET_DEV_CONFIG;
                // Set the device role - Peripheral
                cmd->role      = GAP_ROLE_PERIPHERAL;
                // Set Data length parameters
                cmd->sugg_max_tx_octets = BLE_MAX_OCTETS;
                cmd->sugg_max_tx_time   = BLE_MAX_TIME;
                cmd->max_mtu = BLE_MAX_OCTETS; 
                // Do not support secure connections
                cmd->pairing_mode = GAPM_PAIRING_LEGACY;
                #if (BLE_APP_HID)
                // Enable Slave Preferred Connection Parameters present 
                cmd->att_cfg = GAPM_MASK_ATT_SLV_PREF_CON_PAR_EN;
                #endif //(BLE_APP_HID)

                #ifdef BLE_APP_AM0
                cmd->addr_type   = GAPM_CFG_ADDR_HOST_PRIVACY;
                cmd->audio_cfg   = GAPM_MASK_AUDIO_AM0_SUP;
                #endif // BLE_APP_AM0

                // load IRK
                memcpy(cmd->irk.key, app_env.loc_irk, KEY_LEN);

                // Send message
                ke_msg_send(cmd);
            }
            else
            {
                ASSERT_ERR(0);
            }
        }
        break;
        case (GAPM_PROFILE_TASK_ADD):
        {
           // ASSERT_INFO(param->status == GAP_ERR_NO_ERROR, param->operation, param->status);
            // Add the next requested service
            if (!appm_add_svc())
            {
                // Go to the ready state
                ke_state_set(TASK_APP, APPM_READY);

                // No more service to add, start advertising
                appm_start_advertising();
                
            }
        }
        break;
        // Device Configuration updated
        case (GAPM_SET_DEV_CONFIG):
        {
            ASSERT_INFO(param->status == GAP_ERR_NO_ERROR, param->operation, param->status);

            // Go to the create db state
            ke_state_set(TASK_APP, APPM_CREATE_DB);

            // Add the first required service in the database
            // and wait for the PROFILE_ADDED_IND
            appm_add_svc();
        }
        break;

        case (GAPM_ADV_NON_CONN):
        case (GAPM_ADV_UNDIRECT):
        #if !(BLE_APP_HID)
        case (GAPM_ADV_DIRECT):
        #endif// !(BLE_APP_HID)
        case (GAPM_ADV_DIRECT_LDC):
        case (GAPM_UPDATE_ADVERTISE_DATA):
        {
            ASSERT_INFO(param->status == GAP_ERR_NO_ERROR, param->operation, param->status);
        } break;

        #if (BLE_APP_HID)
        case (GAPM_ADV_DIRECT):
        {
            if (param->status == GAP_ERR_TIMEOUT)
            {
                ke_state_set(TASK_APP, APPM_READY);
                appm_start_advertising();
            }
        } break;
        #endif //(BLE_APP_HID)

        default:
        {
            // Drop the message
        }
        break;
    }

    return (KE_MSG_CONSUMED);
}

static int gapc_get_dev_info_req_ind_handler(ke_msg_id_t const msgid,
        struct gapc_get_dev_info_req_ind const *param,
        ke_task_id_t const dest_id,
        ke_task_id_t const src_id)
{
    switch(param->req)
    {
        case GAPC_DEV_NAME:
        {
            struct gapc_get_dev_info_cfm * cfm = KE_MSG_ALLOC_DYN(GAPC_GET_DEV_INFO_CFM,
                                                    src_id, dest_id,
                                                    gapc_get_dev_info_cfm, APP_DEVICE_NAME_MAX_LEN);
            cfm->req = param->req;
            cfm->info.name.length = appm_get_dev_name(cfm->info.name.value);

            // Send message
            ke_msg_send(cfm);
        } break;

        case GAPC_DEV_APPEARANCE:
        {
            // Allocate message
            struct gapc_get_dev_info_cfm *cfm = KE_MSG_ALLOC(GAPC_GET_DEV_INFO_CFM,
                                                             src_id, dest_id,
                                                             gapc_get_dev_info_cfm);
            cfm->req = param->req;
            // Set the device appearance
            cfm->info.appearance = 192; //Generic Watch
            /*
            #if (BLE_APP_HT)
            // Generic Thermometer - TODO: Use a flag
            cfm->info.appearance = 728;
            #elif (BLE_APP_HID)
            // HID Mouse
            cfm->info.appearance = 962;
            #else
            // No appearance
            cfm->info.appearance = 0;
            #endif
            */
            // Send message
            ke_msg_send(cfm);
        } break;

        case GAPC_DEV_SLV_PREF_PARAMS:
        {
            // Allocate message
            struct gapc_get_dev_info_cfm *cfm = KE_MSG_ALLOC(GAPC_GET_DEV_INFO_CFM,
                    src_id, dest_id,
                                                            gapc_get_dev_info_cfm);
            cfm->req = param->req;
            // Slave preferred Connection interval Min
            cfm->info.slv_params.con_intv_min = 8;
            // Slave preferred Connection interval Max
            cfm->info.slv_params.con_intv_max = 10;
            // Slave preferred Connection latency
            cfm->info.slv_params.slave_latency  = 0;
            // Slave preferred Link supervision timeout
            cfm->info.slv_params.conn_timeout    = 200;  // 2s (500*10ms)

            // Send message
            ke_msg_send(cfm);
        } break;

        default: /* Do Nothing */ break;
    }


    return (KE_MSG_CONSUMED);
}
/**
 ****************************************************************************************
 * @brief Handles GAPC_SET_DEV_INFO_REQ_IND message.
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAP).
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int gapc_set_dev_info_req_ind_handler(ke_msg_id_t const msgid,
        struct gapc_set_dev_info_req_ind const *param,
        ke_task_id_t const dest_id,
        ke_task_id_t const src_id)
{
    // Set Device configuration
    struct gapc_set_dev_info_cfm* cfm = KE_MSG_ALLOC(GAPC_SET_DEV_INFO_CFM, src_id, dest_id,
                                                     gapc_set_dev_info_cfm);
    // Reject to change parameters
    cfm->status = GAP_ERR_REJECTED;
    cfm->req = param->req;
    // Send message
    ke_msg_send(cfm);

    return (KE_MSG_CONSUMED);
}

/**
 ****************************************************************************************
 * @brief Handles connection complete event from the GAP. Enable all required profiles
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAP).
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int gapc_connection_req_ind_handler(ke_msg_id_t const msgid,
                                           struct gapc_connection_req_ind const *param,
                                           ke_task_id_t const dest_id,
                                           ke_task_id_t const src_id)
{
    app_env.conidx = KE_IDX_GET(src_id);

    // Check if the received Connection Handle was valid
    if (app_env.conidx != GAP_INVALID_CONIDX)
    {
        // Retrieve the connection info from the parameters
        app_env.conhdl = param->conhdl;

        // Clear the advertising timeout timer
        if (ke_timer_active(APP_ADV_TIMEOUT_TIMER, TASK_APP))
        {
            ke_timer_clear(APP_ADV_TIMEOUT_TIMER, TASK_APP);
        }

        // Send connection confirmation
        struct gapc_connection_cfm *cfm = KE_MSG_ALLOC(GAPC_CONNECTION_CFM,
                KE_BUILD_ID(TASK_GAPC, app_env.conidx), TASK_APP,
                gapc_connection_cfm);

        #if(BLE_APP_SEC)
        cfm->auth      = app_sec_get_bond_status() ? GAP_AUTH_REQ_NO_MITM_BOND : GAP_AUTH_REQ_NO_MITM_NO_BOND; // TODO [FBE] restore valid data
        #else // !(BLE_APP_SEC)
        cfm->auth      = GAP_AUTH_REQ_NO_MITM_NO_BOND;
        #endif // (BLE_APP_SEC)
        // Send the message
        ke_msg_send(cfm);

        #if DISPLAY_SUPPORT
        // Update displayed information
        app_display_set_adv(false);
        app_display_set_con(true);
        #endif //(DISPLAY_SUPPORT)

        /*--------------------------------------------------------------
         * ENABLE REQUIRED PROFILES
         *--------------------------------------------------------------*/

        #if (BLE_APP_BATT)
        // Enable Battery Service
        app_batt_enable_prf(app_env.conhdl);
        #endif //(BLE_APP_BATT)

        #if (BLE_APP_HID)
        // Enable HID Service
        app_hid_enable_prf(app_env.conhdl);
        #endif //(BLE_APP_HID)

        // We are now in connected State
        ke_state_set(dest_id, APPM_CONNECTED);


        #if (BLE_APP_SEC && !defined(BLE_APP_AM0))
        if (app_sec_get_bond_status())
        {
            // Ask for the peer device to either start encryption
            app_sec_send_security_req(app_env.conidx);
        }
        #endif // (BLE_APP_SEC && !defined(BLE_APP_AM0))
    }
    else
    {
        // No connection has been establish, restart advertising
        appm_start_advertising();
    }

    return (KE_MSG_CONSUMED);
}

/**
 ****************************************************************************************
 * @brief Handles GAP controller command complete events.
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAP).
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int gapc_cmp_evt_handler(ke_msg_id_t const msgid,
                                struct gapc_cmp_evt const *param,
                                ke_task_id_t const dest_id,
                                ke_task_id_t const src_id)
{
    switch(param->operation)
    {
        case (GAPC_UPDATE_PARAMS):
        {
            if (param->status != GAP_ERR_NO_ERROR)
            {
//                appm_disconnect();
            }
        } break;

        default:
        {
        } break;
    }

    return (KE_MSG_CONSUMED);
}

/**
 ****************************************************************************************
 * @brief Handles disconnection complete event from the GAP.
 *
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance (TASK_GAP).
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */

// workaround for OTA of BL
#define EF_ENTRY(addr)               (addr+4)
#define EF_ENTRY_PTR(addr)           (*(volatile uint32_t*)EF_ENTRY(addr))
#define gotoBootloader(addr)         ((void (*)(void))(EF_ENTRY_PTR(addr)))()
static int gapc_disconnect_ind_handler(ke_msg_id_t const msgid,
                                      struct gapc_disconnect_ind const *param,
                                      ke_task_id_t const dest_id,
                                      ke_task_id_t const src_id)
{


    if(APPM_CONNECTED != ke_state_get(TASK_APP))
        return (KE_MSG_CONSUMED);

	#if (APP_OTA)
    if(true == APP_OTA_RebootIsReady())
    {
        //appm_system_reset();

        (*((volatile uint32_t *) 0xe000e010)) &= ~(1UL << 0UL); // close system tick timer
        
        gotoBootloader(0x1003C000); // workaround for OTA of BL

        while(1)
            ;
    }
	#endif

    // Go to the ready state
    ke_state_set(TASK_APP, APPM_READY);

    #if (BLE_APP_HT)
    // Stop interval timer
    app_stop_timer();
    #endif //(BLE_APP_HT)

    #if (DISPLAY_SUPPORT)
    // Update Connection State screen
    app_display_set_con(false);
    #endif //(DISPLAY_SUPPORT)

    #if (!BLE_APP_HID)
    // Restart Advertising
    appm_start_advertising();
    #endif //(!BLE_APP_HID)


    return (KE_MSG_CONSUMED);
}


static int gapm_profile_added_ind_handler(ke_msg_id_t const msgid,
                                          struct gapm_profile_added_ind *param,
                                          ke_task_id_t const dest_id,
                                          ke_task_id_t const src_id)
{
    // Current State
    uint8_t state            = ke_state_get(dest_id);

    if (state == APPM_CREATE_DB)
    {
        switch (param->prf_task_id)
        {
            #if defined (BLE_APP_AM0)
            case (TASK_ID_AM0_HAS):
            {
                am0_app_set_prf_task(param->prf_task_nb);
            } break;
            #endif // defined (BLE_APP_AM0)

            default: /* Nothing to do */ break;
        }
    }
    else
    {
        ASSERT_INFO(0, state, src_id);
    }

    return KE_MSG_CONSUMED;
}

/**
 ****************************************************************************************
 * @brief Handles reception of all messages sent from the lower layers to the application
 * @param[in] msgid     Id of the message received.
 * @param[in] param     Pointer to the parameters of the message.
 * @param[in] dest_id   ID of the receiving task instance
 * @param[in] src_id    ID of the sending task instance.
 *
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
static int appm_msg_handler(ke_msg_id_t const msgid,
                            void *param,
                            ke_task_id_t const dest_id,
                            ke_task_id_t const src_id)
{
    // Retrieve identifier of the task from received message
    ke_task_id_t src_task_id = MSG_T(msgid);
    // Message policy
    uint8_t msg_pol          = KE_MSG_CONSUMED;


    switch (src_task_id)
    {
        case (TASK_ID_GAPC):
        {
            #if (BLE_APP_SEC)
            if ((msgid >= GAPC_BOND_CMD) &&
                (msgid <= GAPC_SECURITY_IND))
            {
                // Call the Security Module
                msg_pol = appm_get_handler(&app_sec_table_handler, msgid, param, src_id);
            }
            #endif //(BLE_APP_SEC)
            // else drop the message
        } break;

        case (TASK_ID_GATTC):
        {
            // Service Changed - Drop
        } break;

        #if (BLE_APP_HT)
        case (TASK_ID_HTPT):
        {
            // Call the Health Thermometer Module
            msg_pol = appm_get_handler(&app_ht_table_handler, msgid, param, src_id);
        } break;
        #endif //(BLE_APP_HT)

        #if (BLE_APP_DIS)
        case (TASK_ID_DISS):
        {
            // Call the Device Information Module
            msg_pol = appm_get_handler(&app_dis_table_handler, msgid, param, src_id);
        } break;
        #endif //(BLE_APP_DIS)

        #if (BLE_APP_HID)
        case (TASK_ID_HOGPD):
        {
            // Call the HID Module
            msg_pol = appm_get_handler(&app_hid_table_handler, msgid, param, src_id);
        } break;
        #endif //(BLE_APP_HID)

        #if (BLE_APP_BATT)
        case (TASK_ID_BASS):
        {
            // Call the Battery Module
            msg_pol = appm_get_handler(&app_batt_table_handler, msgid, param, src_id);
        } break;
        #endif //(BLE_APP_BATT)

        #if defined(BLE_APP_AM0)
        case (TASK_ID_AM0):
        {
            // Call the Audio Mode 0 Module
            msg_pol = appm_get_handler(&am0_app_table_handler, msgid, param, src_id);
        } break;
        case (TASK_ID_AM0_HAS):
        {
            // Call the Audio Mode 0 Module
            msg_pol = appm_get_handler(&am0_app_has_table_handler, msgid, param, src_id);
        } break;
        #endif // defined(BLE_APP_AM0)

		#if (BLE_APP_OTA)
        case (TASK_ID_OTAT):
        {
            // Call the OTA Module
            //msg_pol = appm_get_handler(&app_ota_table_handler, msgid, param, src_id); // not used now
        } break;
		#endif //(BLE_APP_OTA)

		#if (BLE_APP_CCPS)
        case (TASK_ID_CCPS):
        {
            // Call the CCPS Module
            msg_pol = appm_get_handler(&app_ccps_table_handler, msgid, param, src_id);
        } break;
		#endif //(BLE_APP_CCPS)

		#if (BLE_APP_BLEUARTS)
        case (TASK_ID_BLEUARTS):
        {
            // Call the BLEUARTS Module
            msg_pol = appm_get_handler(&app_bleuarts_table_handler, msgid, param, src_id);
        } break;
		#endif //(BLE_APP_BLEUARTS)

        default:
        {
            #if (BLE_APP_HT)
            if (msgid == APP_HT_MEAS_INTV_TIMER)
            {
                msg_pol = appm_get_handler(&app_ht_table_handler, msgid, param, src_id);
            }
            #endif //(BLE_APP_HT)

            #if (BLE_APP_HID)
            if (msgid == APP_HID_MOUSE_TIMEOUT_TIMER)
            {
                msg_pol = appm_get_handler(&app_hid_table_handler, msgid, param, src_id);
            }
            #endif //(BLE_APP_HID)
        } break;
    }

    return (msg_pol);
}




#ifdef SW_TIMER_BY_KERNEL

static int app_timer_handler(ke_msg_id_t const msgid,
                                   void const *param,
                                   ke_task_id_t const dest_id,
                                   ke_task_id_t const src_id)
{

    uint8_t id = msgid & 0xFF;

    if(id >= (APP_SW_TIMER_0_TIMER & 0xFF))
        id -= (APP_SW_TIMER_0_TIMER & 0xFF);
    else
    {
        TracerInfo("app_timer_0_handler => ERR ID: 0x%x\r\n", msgid);
        return (KE_MSG_CONSUMED);
    }


    //drvi_GpioWrite(GPIO_PIN_38, 1);

    if(NULL != m_timer_node[id].p_timeout_handler)
    {
        void * p_context;
        m_timer_node[id].p_timeout_handler(p_context);
    }

    if(SW_TIMER_MODE_REPEATED == m_timer_node[id].mode)
    {

        ke_timer_clear((APP_SW_TIMER_0_TIMER + id), TASK_APP);

        ke_timer_set((APP_SW_TIMER_0_TIMER + id), TASK_APP, m_timer_node[id].periodic_interval); // 10 ms
    }

    //drvi_GpioWrite(GPIO_PIN_38, 0);

    return (KE_MSG_CONSUMED);
}
                            

                                   
#endif                             

/*
 * GLOBAL VARIABLES DEFINITION
 ****************************************************************************************
 */

/* Default State handlers definition. */
const struct ke_msg_handler appm_default_state[] =
{
    // Note: first message is latest message checked by kernel so default is put on top.
    {KE_MSG_DEFAULT_HANDLER,    (ke_msg_func_t)appm_msg_handler},

    {APP_ADV_TIMEOUT_TIMER,     (ke_msg_func_t)app_adv_timeout_handler},

    {GAPM_DEVICE_READY_IND,     (ke_msg_func_t)gapm_device_ready_ind_handler},
    {GAPM_CMP_EVT,              (ke_msg_func_t)gapm_cmp_evt_handler},
    {GAPC_GET_DEV_INFO_REQ_IND, (ke_msg_func_t)gapc_get_dev_info_req_ind_handler},
    {GAPC_SET_DEV_INFO_REQ_IND, (ke_msg_func_t)gapc_set_dev_info_req_ind_handler},
    {GAPC_CONNECTION_REQ_IND,   (ke_msg_func_t)gapc_connection_req_ind_handler},
    {GAPC_CMP_EVT,              (ke_msg_func_t)gapc_cmp_evt_handler},
    {GAPC_DISCONNECT_IND,       (ke_msg_func_t)gapc_disconnect_ind_handler},
    {GAPM_PROFILE_ADDED_IND,    (ke_msg_func_t)gapm_profile_added_ind_handler},

#ifdef SW_TIMER_BY_KERNEL
    {APP_SW_TIMER_0_TIMER,      (ke_msg_func_t)app_timer_handler},        
    {APP_SW_TIMER_1_TIMER,      (ke_msg_func_t)app_timer_handler},
    {APP_SW_TIMER_2_TIMER,      (ke_msg_func_t)app_timer_handler},
    {APP_SW_TIMER_3_TIMER,      (ke_msg_func_t)app_timer_handler},
    {APP_SW_TIMER_4_TIMER,      (ke_msg_func_t)app_timer_handler},
    {APP_SW_TIMER_5_TIMER,      (ke_msg_func_t)app_timer_handler},
    {APP_SW_TIMER_6_TIMER,      (ke_msg_func_t)app_timer_handler},
    {APP_SW_TIMER_7_TIMER,      (ke_msg_func_t)app_timer_handler},
    {APP_SW_TIMER_8_TIMER,      (ke_msg_func_t)app_timer_handler},
    {APP_SW_TIMER_9_TIMER,      (ke_msg_func_t)app_timer_handler},
    {APP_SW_TIMER_10_TIMER,      (ke_msg_func_t)app_timer_handler},
    {APP_SW_TIMER_11_TIMER,      (ke_msg_func_t)app_timer_handler},
    {APP_SW_TIMER_12_TIMER,      (ke_msg_func_t)app_timer_handler},
    {APP_SW_TIMER_13_TIMER,      (ke_msg_func_t)app_timer_handler},
    {APP_SW_TIMER_14_TIMER,      (ke_msg_func_t)app_timer_handler},
    {APP_SW_TIMER_15_TIMER,      (ke_msg_func_t)app_timer_handler},    
    {APP_SW_TIMER_16_TIMER,      (ke_msg_func_t)app_timer_handler},               
    {APP_SW_TIMER_17_TIMER,      (ke_msg_func_t)app_timer_handler},
    {APP_SW_TIMER_18_TIMER,      (ke_msg_func_t)app_timer_handler},
    {APP_SW_TIMER_19_TIMER,      (ke_msg_func_t)app_timer_handler},
    {APP_SW_TIMER_20_TIMER,      (ke_msg_func_t)app_timer_handler},
    {APP_SW_TIMER_21_TIMER,      (ke_msg_func_t)app_timer_handler},
    {APP_SW_TIMER_22_TIMER,      (ke_msg_func_t)app_timer_handler},
    {APP_SW_TIMER_23_TIMER,      (ke_msg_func_t)app_timer_handler},
    {APP_SW_TIMER_24_TIMER,      (ke_msg_func_t)app_timer_handler},
    {APP_SW_TIMER_25_TIMER,      (ke_msg_func_t)app_timer_handler},
    {APP_SW_TIMER_26_TIMER,      (ke_msg_func_t)app_timer_handler},
    {APP_SW_TIMER_27_TIMER,      (ke_msg_func_t)app_timer_handler},
    {APP_SW_TIMER_28_TIMER,      (ke_msg_func_t)app_timer_handler},
    {APP_SW_TIMER_29_TIMER,      (ke_msg_func_t)app_timer_handler},
    {APP_SW_TIMER_30_TIMER,      (ke_msg_func_t)app_timer_handler},
    {APP_SW_TIMER_31_TIMER,      (ke_msg_func_t)app_timer_handler},             

#endif
};

/* Specifies the message handlers that are common to all states. */
const struct ke_state_handler appm_default_handler = KE_STATE_HANDLER(appm_default_state);

/* Defines the place holder for the states of all the task instances. */
ke_state_t appm_state[APP_IDX_MAX];

#endif //(BLE_APP_PRESENT)

/// @} APPTASK
