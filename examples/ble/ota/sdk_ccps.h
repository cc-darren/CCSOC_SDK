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

#ifndef SDK_CCPS_H_
#define SDK_CCPS_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwip_config.h"     // SW configuration 

#if (BLE_APP_CCPS)

#include <stdint.h>          // Standard Integer Definition
#include "ke_task.h"
#include "gattc_task.h"

/*
 * FUNCTIONS DECLARATION
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief SDK for CCPS Read Request Data
 ****************************************************************************************
 */
void sdk_ccps_rx_req_data(const uint8_t *rx_data, uint16_t length);

/**
 ****************************************************************************************
 * @brief Sample Code for CCPS
 ****************************************************************************************
 */
void sdk_ccps_test_code(void);

#endif
#endif

