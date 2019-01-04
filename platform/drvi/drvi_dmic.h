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
 
#ifndef _DRVI_DMIC_H_
#define _DRVI_DMIC_H_
#include "dmic.h"

#define drvi_dmicInit(x)                     cc6801_dmicInit(x)
#define drvi_dmicLeftConfig(x, y)            cc6801_dmicLeftConfig(x, y)
#define drvi_dmicRightConfig(x, y)           cc6801_dmicRightConfig(x, y)
#define drvi_dmicEnableBoth(x)               cc6801_dmicEnableBoth(x)
#define drvi_dmicDisableBoth(x)              cc6801_dmicDisableBoth(x)
#define drvi_dmicReadCurrentPos(x, y)        cc6801_dmicReadCurrentPos(x, y)

#endif //_DRVI_DMIC_H_

