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

/**@file
 *
 * @defgroup sdk_nrf_dfu_transport DFU transport
 * @{
 * @ingroup  sdk_nrf_bootloader
 * @brief Generic Device Firmware Update (DFU) transport interface.
 *
 * @details The DFU transport module defines a generic interface that must
 *          be implemented for each transport layer.
 */

#ifndef FOTA_TRANSPORT_H__
#define FOTA_TRANSPORT_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif






uint32_t fota_transports_init(void);
uint32_t fota_transports_close(void);





#ifdef __cplusplus
}
#endif

#endif // NRF_DFU_TRANSPORT_H__


