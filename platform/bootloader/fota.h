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

#ifndef _FOTA_H_
#define _FOTA_H_

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "bootloader.h"

uint32_t FotaInit(T_FotaConfig *tpConfig);

uint32_t FotaFlashInit(bool bFsEnable);

void BootloaderSettingInit(void);

#if defined(APPLICATION) && APPLICATION
void EnterBootloader(T_callback callback);
#endif

#endif // _FOTA_H_

/** @} */
