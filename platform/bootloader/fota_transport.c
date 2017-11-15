/* Copyright (c) 2016 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

#include "fota_transport.h"
#include "app_scheduler.h"
#include <string.h>
#include "tracer.h"
#include "rwip.h"
#include "error.h"

#include "jump_table.h"
#include "app.h"

 uint32_t fota_transports_init(void)
 {
     uint32_t ret_val = CC_SUCCESS;
 
     TracerInfo("In nrf_dfu_transports_init\r\n");

#ifdef CFG_BLE_APP
     uint32_t error;
	 
     GLOBAL_INT_STOP()

#ifndef CFG_JUMP_TABLE_2
     memset(((void *) JUMP_TABLE_2_BASE_ADDR), 0, 1024);
#endif
     memset (((void *) 0x40006000), 0, 8192);
     memset (((void *) 0x20000048), 0, 0x820);
     *((uint32_t *) 0x4000011C) = 0x00000008;
     *((uint32_t *) 0x40000104) = (*((uint32_t *) 0x40000104) & 0xFFFFFE0) | 0x04;
     
     //*((uint32_t *) 0x20000648) = 0x00;
      
     //regCKGEN->bf.bleClkDiv = 0x04;

     // Initialize RW SW stack
     rwip_init(error);

     GLOBAL_INT_START();
#endif

 
     TracerInfo("After nrf_dfu_transports_init\r\n");
 
     return ret_val;
 }


 uint32_t fota_transports_close(void)
 {
     uint32_t ret_val = CC_SUCCESS;
 
     //TracerInfo("In nrf_dfu_transports_close\r\n");
     app_start_reset_ble();

#if 0
#ifdef CFG_BLE_APP 

     for(uint32_t i = 0; i < 500000; i++)
     {
          app_sched_execute();
          rwip_schedule();
          rwip_ignore_ll_conn_param_update_patch();   
     }
    
     //appm_disconnect();
     appm_reset_ble();
/*
     for(uint32_t i = 0; i < 100; i++)
     {
          app_sched_execute();
          rwip_schedule();
          rwip_ignore_ll_conn_param_update_patch();   
     }  
*/     
#endif
#endif 
    // TracerInfo("After nrf_dfu_transports_close\r\n");
 
     return ret_val;
 }


