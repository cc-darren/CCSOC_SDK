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
#include <string.h>
#include "tracer.h"


 uint32_t fota_transports_init(void)
 {
     uint32_t ret_val = SUCCESS;
 
     TracerInfo("In nrf_dfu_transports_init\r\n");
#if 0 
#ifdef CFG_BLE_APP
         memset (((void *) 0x40006000), 0, 8192);
     
         *((uint32_t *) 0x40000010) = 0x00000B40;
         *((uint32_t *) 0x4000011C) = 0x00000008;
     
         // Initialize RW SW stack
         rwip_init(error);
#endif
#endif
 
     TracerInfo("After nrf_dfu_transports_init\r\n");
 
     return ret_val;
 }
 
 
 uint32_t fota_transports_close(void)
 {
     uint32_t ret_val = SUCCESS;
 
     TracerInfo("In nrf_dfu_transports_close\r\n");
 

 
     TracerInfo("After nrf_dfu_transports_close\r\n");
 
     return ret_val;
 }


