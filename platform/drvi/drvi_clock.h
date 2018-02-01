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
 
#ifndef _DRVI_CLOCK_H_
#define _DRVI_CLOCK_H_
#include "clock.h"

#define drvi_ClockSysClkAdjust(x)               cc6801_ClockSysClkAdjust(x)
#define drvi_ClockPeripheralClkAdjust()         cc6801_ClockPeripheralClkAdjust()
#define drvi_ClockDelayUs(x)                    cc6801_ClockDelayUs(x)
#define drvi_ClockDelayMs(x)                    cc6801_ClockDelayMs(x)

#endif //_DRVI_CLOCK_H_


