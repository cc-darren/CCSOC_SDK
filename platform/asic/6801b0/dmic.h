/* Copyright (c) 2019 Cloudchip, Inc. All Rights Reserved.
 *
 * The information contained herein is property of Cloudchip, Inc.
 * Terms and conditions of usage are described in detail in CLOUDCHIP
 * STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information.
 * NO WARRANTY of ANY KIND is provided. This heading must NOT be removed 
 * from the file.
 */

#ifndef _DMIC_H_
#define _DMIC_H_
#include "global.h"

void cc6801_dmicInit(void*);
void cc6801_dmicLeftConfig(uint32_t, uint32_t);
void cc6801_dmicRightConfig(uint32_t, uint32_t);
void cc6801_dmicEnableBoth(uint32_t);
void cc6801_dmicDisableBoth(void);
void cc6801_dmicReadCurrentPos(uint32_t, uint32_t);

#endif //_DMIC_H_


