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

uint32_t dwTxPacketCounter = 0;
void sdk_ccps_test_send(void * p_context)
{
    
    char tx_data[100]; 
    uint8_t tx_len;
    
    dwTxPacketCounter++;
    
    TracerInfo("Tx Packet No. %d\r\n",dwTxPacketCounter);
				
	tx_len = sprintf (tx_data, "NTF packet No. %d\r\n", dwTxPacketCounter);
    
	app_ccps_notify_send((uint8_t*)tx_data, tx_len); 

	tx_len = sprintf (tx_data, "IND packet No. %d\r\n",dwTxPacketCounter);
    app_ccps_indicate_send((uint8_t*)tx_data, tx_len);
		   
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
        // send notify/indicate packets per 1sec while CCCD is enabled.
    sw_timer_create(&s_tCCPSTimerTime, SW_TIMER_MODE_REPEATED, sdk_ccps_test_send);      
    sw_timer_start(s_tCCPSTimerTime, 1000, NULL);

    TracerInfo("\nSDK EXAMPLE >> CCPS scheduler running ==========\r\n");

    while(1)
    {
        rwip_schedule();
    }
}

#endif

