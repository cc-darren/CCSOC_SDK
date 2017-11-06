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
 * @defgroup nrf_dfu_settings DFU settings
 * @{
 * @ingroup  sdk_nrf_dfu
 */

#ifndef NRF_DFU_SETTINGS_H__
#define NRF_DFU_SETTINGS_H__

#include <stdint.h>
//#include "app_util_platform.h"
#include "fota_types.h"
#include "fota_flash.h"

#ifdef __cplusplus
extern "C" {
#endif

/**@brief Global DFU settings.
 *
 * @note Using this variable is not thread-safe.
 *
 */
extern nrf_dfu_settings_t s_dfu_settings;


/** @brief Function for writing DFU settings to flash.
 *
 * @param[in]   callback    Pointer to a function that is called after completing the write operation.
 *
 * @retval      CC_SUCCESS         If the write process was successfully initiated.
 * @retval      NRF_ERROR_INTERNAL  If a flash error occurred.
 */
int nrf_dfu_settings_write(dfu_flash_callback_t callback);


/** @brief Function for initializing the DFU settings module.
 */
void nrf_dfu_settings_init(void);

#if defined(APPLICATION) && APPLICATION
void enter_bootloader(dfu_flash_callback_t callback);
#endif

#ifdef __cplusplus
}
#endif

#endif // NRF_DFU_SETTINGS_H__

/**@} */
