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

#include "fota_settings.h"
#include "fota_flash.h"
#include "crc32.h"
#include <string.h>
#include "app_scheduler.h"
#include "drvi_clock.h"
#include "error.h"
#include "tracer.h"

/** @brief  This variable reserves a codepage for bootloader specific settings,
 *          to ensure the compiler doesn't locate any code or variables at his location.
 */
uint8_t  m_dfu_settings_buffer[CODE_PAGE_SIZE] __attribute__((at(BOOTLOADER_SETTINGS_ADDRESS)))
                                                   __attribute__((used));


nrf_dfu_settings_t s_dfu_settings;

//lint -save -esym(551, flash_operation_pending)
static bool flash_operation_pending; // barrier for reading flash
//lint -restore

static dfu_flash_callback_t m_callback;


static void dfu_settings_write_callback(fs_evt_t const * const evt, fs_ret_t result)
{
    if (result == FS_SUCCESS)
    {
        flash_operation_pending = false;
    }
    if (m_callback != NULL)
    {
        m_callback(evt, result);
    }
}

static void delay_operation(void)
{
   drvi_ClockDelayMs(100);
   app_sched_execute();
}

static void wait_for_pending(void)
{
    while (flash_operation_pending == true)
    {
        TracerInfo("Waiting for other flash operation to finish.\r\n");
        delay_operation();
    }
}

static void wait_for_queue(void)
{
    while (fs_queue_is_full())
    {
        TracerInfo("Waiting for available space on flash queue.\r\n");
        delay_operation();
    }
}


uint32_t nrf_dfu_settings_calculate_crc(void)
{
    // the crc is calculated from the s_dfu_settings struct, except the crc itself and the init command
    return crc32_compute((uint8_t*)&s_dfu_settings + 4, sizeof(nrf_dfu_settings_t) - 4 - sizeof(s_dfu_settings.init_command), NULL);
}


void nrf_dfu_settings_init(void)
{
    TracerInfo("running nrf_dfu_settings_init\r\n");

    uint32_t crc;

    flash_operation_pending = false;

    // Copy the DFU settings out of flash and into a buffer in RAM.
    memcpy((void*)&s_dfu_settings, &m_dfu_settings_buffer[0], sizeof(nrf_dfu_settings_t));

    if(s_dfu_settings.crc != 0xFFFFFFFF)
    {
        // CRC is set. Content must be valid
        crc = nrf_dfu_settings_calculate_crc();
        if(crc == s_dfu_settings.crc)
        {
            return;
        }
    }

    // Reached if nothing is configured or if CRC was wrong
    TracerInfo("!!!!!!!!!!!!!!! Resetting bootloader settings !!!!!!!!!!!\r\n");
    memset(&s_dfu_settings, 0x00, sizeof(nrf_dfu_settings_t));
    s_dfu_settings.settings_version = NRF_DFU_SETTINGS_VERSION;
    //Blake, remove APP_ERROR_CHECK
    //APP_ERROR_CHECK(nrf_dfu_settings_write(NULL));
    nrf_dfu_settings_write(NULL);
}


int nrf_dfu_settings_write(dfu_flash_callback_t callback)
{
    int err_code = FS_SUCCESS;
    TracerInfo("Erasing old settings at: 0x%08x\r\n", (uint32_t)&m_dfu_settings_buffer[0]);

    // Wait for any ongoing operation (because of multiple calls to nrf_dfu_settings_write)
    wait_for_pending();

    flash_operation_pending = true;
    m_callback = callback;

    do
    {
        wait_for_queue();

        // Not setting the callback function because ERASE is required before STORE
        // Only report completion on successful STORE.
        err_code = nrf_dfu_flash_erase((uint32_t*)&m_dfu_settings_buffer[0], 1, NULL);

    } while (err_code == FS_ERR_QUEUE_FULL);


    if (err_code != FS_SUCCESS)
    {
        TracerErr("Erasing from flash memory failed.\r\n");
        flash_operation_pending = false;
        return CC_ERROR_INTERNAL;
    }

    s_dfu_settings.crc = nrf_dfu_settings_calculate_crc();

    TracerInfo("Writing 0x%08x words\r\n", sizeof(nrf_dfu_settings_t)/4);

    static nrf_dfu_settings_t temp_dfu_settings;
    memcpy(&temp_dfu_settings, &s_dfu_settings, sizeof(nrf_dfu_settings_t));

    do
    {
        wait_for_queue();

        err_code = nrf_dfu_flash_store((uint32_t*)&m_dfu_settings_buffer[0],
                                       (uint32_t*)&temp_dfu_settings,
                                       sizeof(nrf_dfu_settings_t)/4,
                                       dfu_settings_write_callback);

    } while (err_code == FS_ERR_QUEUE_FULL);

    if (err_code != FS_SUCCESS)
    {
        TracerErr("Storing to flash memory failed.\r\n");
        flash_operation_pending = false;
        return CC_ERROR_INTERNAL;
    }

    TracerInfo("Writing settings...\r\n");
    return CC_SUCCESS;
}

