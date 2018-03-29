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

#include "bootloader.h"

#include "tracer.h"
#include "ble_mgr.h"
#include "ota_handling.h"
#include "error.h"
#include "app.h"


#define EF_ENTRY(addr)                (addr+4)
#define EF_ENTRY_PTR(addr)            (*(volatile uint32_t*)EF_ENTRY(addr))
#define gotoApplication(addr)         ((void (*)(void))(EF_ENTRY_PTR(addr)))()

void JumpToApplication(uint32_t dwStartAddr)
{
    TracerInfo("Jump to application address: 0x%08x\r\n", dwStartAddr);

    // Jump to application
    gotoApplication(dwStartAddr);
}

//uint32_t BootloaderInit(T_FotaConfig *tpConfig)
uint32_t BootloaderInit(uint32_t *app_start_address)
{
    TracerInfo("Bootloader init start...\r\n");

    uint32_t ret_val = CC_SUCCESS;

    if(false == ota_verify_and_get_app_address(app_start_address))
        ret_val = CC_ERROR_INVALID_DATA;

#if 0
    ret_val = FotaInit(tpConfig);
    if (ret_val != CC_SUCCESS)
    {
        return ret_val;
    }
#endif
    TracerInfo("Bootloader init done...\r\n");
    return ret_val;
}

void BootloaderMain(void)
{
    uint32_t ret_val;
    uint32_t app_start_address;
#if 0    
    T_FotaConfig tBlConfig;
#endif
    TracerInfo("Bootloader main start...\r\n");
#if 0
    tBlConfig.dwEflashStartAddr = EFLASH_START_ADDRESS;
    tBlConfig.dwEflashTotalSize = EFLASH_TOTAL_SIZE;
    tBlConfig.dwAppStartAddr    = APPLICATION_START_ADDRESS;
    tBlConfig.dwUserDataSize    = USER_DATA_SIZE;

    ret_val = BootloaderInit(&tBlConfig);
    if (ret_val != CC_SUCCESS)
    {
        TracerInfo("bootloader init fail!!\r\n");
        return;
    }

    // Boot the application
    JumpToApplication(tBlConfig.dwAppStartAddr);
#endif

    ret_val = BootloaderInit(&app_start_address);

    if (ret_val != CC_SUCCESS)
    {
        TracerInfo("bootloader init fail!!\r\n");
        TracerInfo("Jump to OTA Mode!\r\n");

        APP_BLEMGR_Init();

        ota_set_mode_disabled();

        while(1)
        {
            rwip_schedule();
            rwip_detect_disconnect_patch();
            rwip_ignore_ll_conn_param_update_patch();

            __WFE();
        }
        
    }

    // Boot the application
    JumpToApplication(app_start_address);


    TracerInfo("Bootloader main done...\r\n");
}
