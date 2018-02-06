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
#include "fota.h"
#include "error.h"


#define EF_ENTRY(addr)                (addr+4)
#define EF_ENTRY_PTR(addr)            (*(volatile uint32_t*)EF_ENTRY(addr))
#define gotoApplication(addr)         ((void (*)(void))(EF_ENTRY_PTR(addr)))()

void JumpToApplication(uint32_t dwStartAddr)
{
    TracerInfo("Jump to application address: 0x%08x\r\n", dwStartAddr);

    // Jump to application
    gotoApplication(dwStartAddr);
}

uint32_t BootloaderInit(T_FotaConfig *tpConfig)
{
    TracerInfo("Bootloader init start...\r\n");

    uint32_t ret_val = CC_SUCCESS;

    ret_val = FotaInit(tpConfig);
    if (ret_val != CC_SUCCESS)
    {
        return ret_val;
    }

    TracerInfo("Bootloader init done...\r\n");
    return ret_val;
}

void BootloaderMain(void)
{
    uint32_t ret_val;
    T_FotaConfig tBlConfig;

    TracerInfo("Bootloader main start...\r\n");

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

    TracerInfo("Bootloader main done...\r\n");
}
