/* Copyright (c) 2018 Cloudchip, Inc. All Rights Reserved.
 *
 * The information contained herein is property of Cloudchip, Inc.
 * Terms and conditions of usage are described in detail in CLOUDCHIP
 * STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information.
 * NO WARRANTY of ANY KIND is provided. This heading must NOT be removed 
 * from the file.
 */

/******************************************************************************
*  Filename:
*  ---------
*  arch_main.c
*
*  Project:
*  --------
*  cc6801
*
*  Description:
*  ------------
*  SDK example to demonstrate OTA & user data transmission over BLE
*
*  Author:
*  -------
*  CloudChip
*
*===========================================================================/
*  20180408 LOUIS initial version
******************************************************************************/

/******************************************************************************
 * INCLUDE FILE
 ******************************************************************************/
#include <string.h>
#include "project.h"

#include "ble_mgr.h"

#include "ll.h"
#include "drvi_init.h"
#include "drvi_gpio.h"
#include "tracer.h"

#include "sw_timer.h"
#include "appdisplay.h"

#include "rwip_config.h"     // SW configuration

#if (BLE_APP_CCPS)
#include "app_ccps.h"        // CloudChip Proprietary Services Application Definitions
#include "app.h"             // Application Definitions
#include "app_task.h"        // application task definitions
#include "ccps_task.h"       
#include "co_bt.h"
#include "prf_types.h"
#include "prf_utils.h"
#include "arch.h"            // Platform Definitions
#include "ccps.h"
#include "co_math.h"
#include "ke_timer.h"
#include "drvi_clock.h"
#include "sw_timer.h"
#include "tracer.h"
#include "scheduler.h"
#include "rwip.h"

/*
 * DEFINES
 ****************************************************************************************
 */



/*
 * LOCAL variables
 ****************************************************************************************
 */

uint8_t g_GyroEnable = 1;


/*
 * GLOBAL FUNCTION DEFINITIONS
 ****************************************************************************************
 */
extern void sys_InitMain(void);



/******************************************************************************
 * FUNCTION > _AppInit
 ******************************************************************************/
void    _AppInit(void)
{
  

    APP_SCHED_Init();
}

/******************************************************************************
 * _platform_init
 ******************************************************************************/
void    _PeripheralInit(void)
{
    
    app_displayoled_init();
    app_displayoled_start();

}

/******************************************************************************
 * _OledShowMsgDemo
 ******************************************************************************/
void    _OledShowMsgDemo(void)
{  
    uint8_t    _bPosX = 4;
    uint8_t    _bPosY = 3;
    uint8_t    _baString[] = { 'C', 'C', '-', '6', '8', '0', '1' };

    oled_DisplayOn();    
    oled_DrawPixel8x16(_bPosX, _bPosY, _baString);
}


/******************************************************************************
 * FUNCTION > _PlatformInit
 ******************************************************************************/
void    _PlatformInit(void)
{
    sys_InitMain();

    GLOBAL_INT_START();

    drvi_initialize();
}

int    main(void)
{
    _PlatformInit();
    _PeripheralInit();


    #ifdef CFG_BLE_APP    
    APP_BLEMGR_Init();
    
    APP_SW_Timer_Init();
    #endif

    _AppInit();

    TracerInfo("\r\n===== [CC6801] SDK %s =====\r\n", SDK_FW_VERSION);

    // Show welcome msg on LCM
    _OledShowMsgDemo();
        
    // SDK example: Cloudchip proprietary service profile
    sdk_ccps_test_code();    

    while(1)
    {
        #ifdef CFG_BLE_APP
            rwip_schedule();
            rwip_detect_disconnect_patch();
            rwip_ignore_ll_conn_param_update_patch();
        #endif    
    
        APP_SCHED_RunScheduler();
        
        __WFE();
    }
}

#endif

