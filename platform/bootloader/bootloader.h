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


#ifndef _BOOTLOADER_H__
#define _BOOTLOADER_H__

#include <stdint.h>

typedef struct S_FotaConfig
{
    uint32_t dwEflashStartAddr;
    uint32_t dwEflashTotalSize;
    uint32_t dwAppStartAddr;
    uint32_t dwUserDataSize;
} T_FotaConfig;

uint32_t BootloaderInit(T_FotaConfig *tpConfig);
void BootloaderMain(void);

#endif // _BOOTLOADER_H__
