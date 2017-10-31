/* Copyright (c) 2013 Nordic Semiconductor. All Rights Reserved.
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


#ifndef _LOW_POWER_H
#define _LOW_POWER_H

#include <stdint.h>
#include <stdlib.h>

void cc6801_LowPower_Sleep(void);
void cc6801_LowPower_DeepSleep(void);
void cc6801_LowPower_Hibernate(void);

void cc6801_LowPower_SleepMs(uint16_t dTimeMs);

void cc6801_LowPower_SetPeripheralOn(uint32_t dwPeripheral);
void cc6801_LowPower_SetPeripheralOff(uint32_t dwPeripheral);


void cc6801_LowPower_SetPowerOn(uint32_t dwPeripheral);
void cc6801_LowPower_SetPowerOff(uint32_t dwPeripheral);
#endif
