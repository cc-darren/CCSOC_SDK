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

#include <stdint.h>
#include "fota_bootloader_app_start.h"
//#include "compiler_abstraction.h"
#include "tracer.h"
//#include "nrf_dfu_mbr.h"
//#include "nrf_sdm.h"

#define EF_ENTRY(addr)                (addr+4)
#define EF_ENTRY_PTR(addr)            (*(volatile uint32_t*)EF_ENTRY(addr))
#define gotoApplication(addr)         ((void (*)(void))(EF_ENTRY_PTR(addr)))()

void nrf_bootloader_app_start(uint32_t start_addr)
{
    TracerInfo("Running nrf_bootloader_app_start with address: 0x%08x\r\n", start_addr);

    // Run application
    gotoApplication(start_addr);
}

