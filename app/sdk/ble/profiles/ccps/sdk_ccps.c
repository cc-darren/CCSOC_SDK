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
*  sdk_ccps.c
*
*  Project:
*  --------
*  cc6801
*
*  Description:
*  ------------
*  Emulator of CloudChip Proprietary Serivce
*
*  Author:
*  -------
*  CloudChip
*
*===========================================================================/
*  20180125 SAMUEL initial version
******************************************************************************/

/******************************************************************************
 * INCLUDE FILE
 ******************************************************************************/

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
#include "sdk_ccps.h"

/*
 * DEFINES
 ****************************************************************************************
 */

SW_TIMER_DEF(s_tCCPSTimerTime);


/*
 * LOCAL FUNCTION DEFINITIONS
 ****************************************************************************************
 */

void sdk_ccps_test_send(void * p_context)
{
    
    char tx_data[100]; 
    int16_t _wAccelData[3]  = { 0 };
    uint8_t tx_len;

    LSM6DS3_ACC_GYRO_GetRawAccData(NULL, (i16_t *) _wAccelData);

    TracerInfo("Acc_X: %d, Acc_Y: %d, Acc_Z: %d\r\n", _wAccelData[0], _wAccelData[1], _wAccelData[2]);
				
	tx_len = sprintf (tx_data, "NTF: %d, %d, %d\r\n", _wAccelData[0], _wAccelData[1], _wAccelData[2]);
    
	app_ccps_notify_send((uint8_t*)tx_data, tx_len); 

	tx_len = sprintf (tx_data, "IND: %d, %d, %d\r\n", _wAccelData[0], _wAccelData[1], _wAccelData[2]);
    app_ccps_indicate_send((uint8_t*)tx_data, tx_len);
		
#if 0

    static uint8_t tx_data[CCPS_REPORT_MAX_LEN];    // Note: It should exchange MTU that can extend max length > 20 Bytes

    // send Notification: 0 ~ 9
    for(uint16_t i = 0; i < CCPS_REPORT_MAX_LEN; i++)
        tx_data[i] = (i%10) + '0';

    app_ccps_notify_send(tx_data, CCPS_REPORT_MAX_LEN); 


    // send Indication: 9 ~ 0
    for(uint16_t i = 0; i < CCPS_REPORT_MAX_LEN; i++)
        tx_data[i] = ((CCPS_REPORT_MAX_LEN-i)%10) + '0';

    app_ccps_indicate_send(tx_data, CCPS_REPORT_MAX_LEN);
#endif    
}



/*
 * GLOBAL FUNCTION DEFINITIONS
 ****************************************************************************************
 */

void sdk_ccps_rx_req_data(const uint8_t *rx_data, uint16_t length)
{
    
    TracerInfo("Receive %d Bytes Data:", length);

    for(uint16_t i = 0; i < length; i++)
    {
        TracerInfo("0x%0x, ", rx_data[i]);
    }

    TracerInfo("\r\n");
}


void sdk_ccps_test_code(void)
{
    TracerInfo("\nSDK EXAMPLE >> CCPS starts ==========\r\n");
    
    // Acc Init:
    if(LSM6DS3_X_Init() == MEMS_ERROR)
    {
        TracerInfo("Accelerometer initialize failed\r\n");
        cc6801_ClockDelayMs(10);
    } 
    else 
    {
        TracerInfo("Accelerometer initialize OK\r\n");
        cc6801_ClockDelayMs(10);
    }

    CC_LSM6DSX_AccelPowerDown();
    CC_LSM6DSX_GyroPowerDown();
    
    CC_LSM6DSX_AccelPowerON(LSM6DS3_ACC_GYRO_ODR_XL_52Hz);
    CC_LSM6DSX_GyroPowerON (LSM6DS3_ACC_GYRO_ODR_G_52Hz );


    // send notify/indicate packets per 1sec while CCCD is enabled.
    sw_timer_create(&s_tCCPSTimerTime, SW_TIMER_MODE_REPEATED, sdk_ccps_test_send);      
    sw_timer_start(s_tCCPSTimerTime, 1000, NULL);

    TracerInfo("\nSDK EXAMPLE >> CCPS scheduler running ==========\r\n");

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

