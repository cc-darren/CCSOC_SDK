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

#include "fota_flash.h"
#include "fota_types.h"
#include "drvi_eflash.h"
#include "tracer.h"
#include "fstorage.h"

#define FLASH_FLAG_NONE                 (0)
#define FLASH_FLAG_OPER                 (1<<0)
#define FLASH_FLAG_FAILURE_SINCE_LAST   (1<<1)
#define FLASH_FLAG_FSTORAGE_ENABLED     (1<<2)

static uint32_t m_flags;


// Function prototypes
static void fs_evt_handler(fs_evt_t const * const evt, fs_ret_t result);

FS_REGISTER_CFG(fs_config_t fs_dfu_config) =
{
    .callback       = fs_evt_handler,            // Function for event callbacks.
    .p_start_addr   = (uint32_t*)0x10000000,
    .p_end_addr     = (uint32_t*)BOOTLOADER_SETTINGS_ADDRESS + CODE_PAGE_SIZE
};


static void fs_evt_handler(fs_evt_t const * const evt, fs_ret_t result)
{
    // Clear the operation flag
    m_flags &= ~FLASH_FLAG_OPER;

    if (result == FS_SUCCESS)
    {
        // Clear flag for ongoing operation and failure since last
        m_flags &= ~FLASH_FLAG_FAILURE_SINCE_LAST;
    }
    else
    {
        TracerInfo("Generating failure\r\n");
        m_flags |= FLASH_FLAG_FAILURE_SINCE_LAST;
    }

    if (evt->p_context)
    {
        //lint -e611
        ((dfu_flash_callback_t)evt->p_context)(evt, result);
    }
}



uint32_t nrf_dfu_flash_init(bool fstorage_enable)
{
    uint32_t err_code = CC_SUCCESS;

    if(fstorage_enable)
    {
        TracerInfo("------- nrf_dfu_flash_init-------\r\n");
        if (fs_init() != FS_SUCCESS)
        {
            TracerInfo("Not initializing the thing\r\n");
            return CC_ERROR_INVALID_STATE;
        }

        // Setting flag to indicate that SD is enabled to ensure fstorage is use in calls
        // to do flash operations.
        m_flags = FLASH_FLAG_FSTORAGE_ENABLED;
    }
    else
    {
        m_flags = FLASH_FLAG_NONE;
        drvi_EflashInit();
    }

    return err_code;
}


fs_ret_t nrf_dfu_flash_store(uint32_t const * p_dest, uint32_t const * const p_src, uint32_t len_words, dfu_flash_callback_t callback)
{
    fs_ret_t ret_val = FS_SUCCESS;
    //TracerInfo("Storing: from 0x%08x to 0x%08x, num: %d\r\n", (uint32_t)p_src, (uint32_t)p_dest, len_words);

    if ((m_flags & FLASH_FLAG_FSTORAGE_ENABLED) != 0)
    {
        // Check if there is a pending error
        if ((m_flags & FLASH_FLAG_FAILURE_SINCE_LAST) != 0)
        {
            TracerInfo("Flash: Failure since last\r\n");
            return FS_ERR_FAILURE_SINCE_LAST;
        }

        // Set the flag to indicate ongoing operation
        m_flags |= FLASH_FLAG_OPER;
        //lint -e611
        ret_val = fs_store(&fs_dfu_config, p_dest, p_src, len_words, (void*)callback);

        if (ret_val != FS_SUCCESS)
        {
            TracerInfo("Flash: failed %d\r\n", ret_val);
            return ret_val;
        }

        // Set the flag to indicate ongoing operation
        m_flags |= FLASH_FLAG_OPER;
    }
    else
    {

        if ((p_src == NULL) || (p_dest == NULL))
        {
            return FS_ERR_NULL_ARG;
        }

        // Check that both pointers are word aligned.
        if (((uint32_t)p_src  & 0x03) ||
            ((uint32_t)p_dest & 0x03))
        {
            return FS_ERR_UNALIGNED_ADDR;
        }

        if (len_words == 0)
        {
            TracerInfo("Flash: Invallid length (NVMC)\r\n");
            return FS_ERR_INVALID_ARG;
        }

        drvi_EflashProgram((uint32_t)p_dest, (unsigned char *)p_src, len_words);

        #if (__LINT__ != 1)
        if (callback)
        {
            fs_evt_t evt =
            {
                .id = FS_EVT_STORE,
                .p_context = (void*)callback,
                .store =
                {
                    .length_words = len_words,
                    .p_data = p_dest
                }
            };
            callback(&evt, FS_SUCCESS);
        }
        #endif
    }

    return ret_val;
}


/** @brief Internal function to initialize DFU BLE transport
 */
fs_ret_t nrf_dfu_flash_erase(uint32_t const * p_dest, uint32_t num_pages, dfu_flash_callback_t callback)
{
    fs_ret_t ret_val = FS_SUCCESS;
    TracerInfo("Erasing: 0x%08x, num: %d\r\n", (uint32_t)p_dest, num_pages);


    if ((m_flags & FLASH_FLAG_FSTORAGE_ENABLED) != 0)
    {
        // Check if there is a pending error
        if ((m_flags & FLASH_FLAG_FAILURE_SINCE_LAST) != 0)
        {
            TracerInfo("Erase: Failure since last\r\n");
            return FS_ERR_FAILURE_SINCE_LAST;
        }

        m_flags |= FLASH_FLAG_OPER;
        ret_val = fs_erase(&fs_dfu_config, p_dest, num_pages, (void*)callback);

        if (ret_val != FS_SUCCESS)
        {
            TracerInfo("Erase failed: %d\r\n", ret_val);
            m_flags &= ~FLASH_FLAG_OPER;
            return ret_val;
        }

        // Set the flag to indicate ongoing operation
        m_flags |= FLASH_FLAG_OPER;
    }
    else
    {
        // Softdevice is not present or activated. Run the NVMC instead
        if (((uint32_t)p_dest & (CODE_PAGE_SIZE-1)) != 0)
        {
            TracerInfo("Invalid address\r\n");
            return FS_ERR_UNALIGNED_ADDR;
        }

        uint16_t first_page = ((uint32_t)p_dest / CODE_PAGE_SIZE);
        do
        {
            //nrf_nvmc_page_erase((uint32_t)p_dest);
            drvi_EflashErasePage((uint32_t)p_dest);
            p_dest += CODE_PAGE_SIZE/sizeof(uint32_t);
        }
        while(--num_pages > 0);


        if (callback)
        {
            #if (__LINT__ != 1)
            fs_evt_t evt =
            {
                .id = FS_EVT_ERASE,
                .p_context = (void*)callback,
                .erase =
                {
                    .first_page = first_page,
                    .last_page = ((uint32_t)p_dest / CODE_PAGE_SIZE)
                }
            };
            callback(&evt, FS_SUCCESS);
            #else
            (void)first_page;
            #endif
        }
    }

    return ret_val;
}


void nrf_dfu_flash_error_clear(void)
{
    m_flags &= ~FLASH_FLAG_FAILURE_SINCE_LAST;
}


fs_ret_t nrf_dfu_flash_wait(void)
{
    TracerInfo("Waiting for finished...\r\n");

    if ((m_flags & FLASH_FLAG_FSTORAGE_ENABLED) != 0)
    {
        while ((m_flags & FLASH_FLAG_OPER) != 0)
        {
            //(void)sd_app_evt_wait();
        }

        if ((m_flags & FLASH_FLAG_FAILURE_SINCE_LAST) != 0)
        {
            TracerInfo("Failure since last\r\n");
            return FS_ERR_FAILURE_SINCE_LAST;
        }
    }
    drvi_EflashFlush();

    TracerInfo("After wait!\r\n");
    return FS_SUCCESS;
}
