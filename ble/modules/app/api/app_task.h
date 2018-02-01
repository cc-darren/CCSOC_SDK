/**
 ****************************************************************************************
 *
 * @file app_task.h
 *
 * @brief Header file - APPTASK.
 *
 * Copyright (C) RivieraWaves 2009-2015
 *
 *
 ****************************************************************************************
 */

#ifndef APP_TASK_H_
#define APP_TASK_H_

/**
 ****************************************************************************************
 * @addtogroup APPTASK Task
 * @ingroup APP
 * @brief Routes ALL messages to/from APP block.
 *
 * The APPTASK is the block responsible for bridging the final application with the
 * RWBLE software host stack. It communicates with the different modules of the BLE host,
 * i.e. @ref SMP, @ref GAP and @ref GATT.
 *
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwip_config.h"             // SW configuration

#if (BLE_APP_PRESENT)

#include <stdint.h>         // Standard Integer
#include "rwip_task.h"      // Task definitions
#include "ke_task.h"        // Kernel Task

/*
 * DEFINES
 ****************************************************************************************
 */

/// Number of APP Task Instances
#define APP_IDX_MAX                 (1)

/*
 * ENUMERATIONS
 ****************************************************************************************
 */

/// States of APP task
enum appm_state
{
    /// Initialization state
    APPM_INIT,
    /// Database create state
    APPM_CREATE_DB,
    /// Ready State
    APPM_READY,
    /// Advertising state
    APPM_ADVERTISING,
    /// Connected state
    APPM_CONNECTED,

    /// Number of defined states.
    APPM_STATE_MAX
};


/// APP Task messages
enum appm_msg
{
    APPM_DUMMY_MSG = TASK_FIRST_MSG(TASK_ID_APP),

    /// Timer used to automatically stop advertising
    APP_ADV_TIMEOUT_TIMER,

    #if (BLE_APP_HT)
    /// Timer used to refresh the temperature measurement value
    APP_HT_MEAS_INTV_TIMER,
    #endif //(BLE_APP_HT)

    #if (BLE_APP_HID)
    /// Timer used to disconnect the moue if no activity is detecter
    APP_HID_MOUSE_TIMEOUT_TIMER,
    #endif //(BLE_APP_HID)

    #if (BLE_APP_OTA)
    /// Timer used to refresh the OTA measurement value
    APP_OTA_MEAS_INTV_TIMER,
    #endif //(BLE_APP_OTA)  

#ifdef SW_TIMER_BY_KERNEL
    APP_SW_TIMER_0_TIMER  =  20, 
    APP_SW_TIMER_1_TIMER,    
    APP_SW_TIMER_2_TIMER,    
    APP_SW_TIMER_3_TIMER,    
    APP_SW_TIMER_4_TIMER,    
    APP_SW_TIMER_5_TIMER,
    APP_SW_TIMER_6_TIMER,    
    APP_SW_TIMER_7_TIMER,    
    APP_SW_TIMER_8_TIMER,    
    APP_SW_TIMER_9_TIMER,    
    APP_SW_TIMER_10_TIMER,
    APP_SW_TIMER_11_TIMER,    
    APP_SW_TIMER_12_TIMER,    
    APP_SW_TIMER_13_TIMER,    
    APP_SW_TIMER_14_TIMER,    
    APP_SW_TIMER_15_TIMER,
    APP_SW_TIMER_16_TIMER,
    APP_SW_TIMER_17_TIMER,    
    APP_SW_TIMER_18_TIMER,    
    APP_SW_TIMER_19_TIMER,    
    APP_SW_TIMER_20_TIMER,    
    APP_SW_TIMER_21_TIMER,
    APP_SW_TIMER_22_TIMER,    
    APP_SW_TIMER_23_TIMER,    
    APP_SW_TIMER_24_TIMER,    
    APP_SW_TIMER_25_TIMER,    
    APP_SW_TIMER_26_TIMER,
    APP_SW_TIMER_27_TIMER,    
    APP_SW_TIMER_28_TIMER,    
    APP_SW_TIMER_29_TIMER,    
    APP_SW_TIMER_30_TIMER,    
    APP_SW_TIMER_31_TIMER,    
#endif

};


/*
 * GLOBAL VARIABLE DECLARATIONS
 ****************************************************************************************
 */

extern const struct ke_state_handler appm_default_handler;
extern ke_state_t appm_state[APP_IDX_MAX];

/// @} APPTASK

#endif //(BLE_APP_PRESENT)

#endif // APP_TASK_H_
