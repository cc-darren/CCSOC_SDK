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

#ifndef _ALTITUDE_H
#define _ALTITUDE_H

#include <stdint.h>
#include <stdlib.h>
#include "project.h"

#if (defined PRESSURE_INT_PIN) && (PRESSURE_INT_PIN)
uint8_t ALT_StartDevice(fpGpiIrqHandler tCB);
#else
uint8_t ALT_StartDevice(void);
BOOL ALT_CheckFIFORdy(void);
#endif

void ALT_StopDevice(void);
uint32_t ALT_CalculateAltitdue(uint32_t dwInPressure);
uint32_t ALT_GetAtmosPressure(void);

void ALT_CalibrateWithSeaLevelPressure(uint32_t dwPressure);
void ALT_CalibrateWithCurrentPressure(int32_t dwCurPressure);
void ALT_CalibrateWithCurrentHeight(uint32_t dwCurHeight);
void ALT_CalibrateWithCurrentPressureAndHeight(uint32_t dwCurPressure,uint32_t dwCurHeight);

#endif

