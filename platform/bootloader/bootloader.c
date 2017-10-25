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

#include "bootloader.h"

//#include "compiler_abstraction.h"
//#include "nrf.h"
#include "fota_bootloader_app_start.h"
#include "bootloader_info.h"
#include "tracer.h"
#include "fota.h"
#include "error.h"

uint32_t nrf_bootloader_init(void)
{
    TracerInfo("In nrf_bootloader_init\r\n");

    uint32_t ret_val = CC_SUCCESS;

    // Call DFU init function if implemented
    ret_val = nrf_dfu_init();
    if (ret_val != CC_SUCCESS)
    {
        return ret_val;
    }

    TracerInfo("After nrf_bootloader_init\r\n");
    return ret_val;
}

void bootloader_main(void)
{
    uint32_t ret_val;

    TracerInfo("Inside main\r\n");

    ret_val = nrf_bootloader_init();
    if (ret_val != CC_SUCCESS)
    {
        TracerInfo("bootloader init fail!!\r\n");
        return;
    }

    // Either there was no DFU functionality enabled in this project or the DFU module detected
    // no ongoing DFU operation and found a valid main application.
    // Boot the main application.
    nrf_bootloader_app_start(MAIN_APPLICATION_START_ADDR);

    // Should never be reached.
    TracerInfo("After main\r\n");
}
