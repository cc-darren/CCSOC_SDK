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

void lowpower_mode_Sleep(void);
void lowpower_mode_DeepSleep(void);
void lowpower_mode_Hibernater(void);



#endif
