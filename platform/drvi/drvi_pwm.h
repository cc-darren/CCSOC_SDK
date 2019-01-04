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
 
#ifndef _DRVI_PWM_H_
#define _DRVI_PWM_H_
#include "pwm.h"


#define drvi_pwmCounterGet(x,y)         cc6801_pwmCounterGet((E_pwmTotal)x,y)
#define drvi_pwmCounterClear(x)            cc6801_pwmCounterClear((E_pwmTotal)x)
#define drvi_pwmDuty(x,y)               cc6801_pwmDuty((E_pwmTotal)x,y)
#define drvi_pwmStart(x)                   cc6801_pwmStart((E_pwmTotal)x)
#define drvi_pwmStop(x)                    cc6801_pwmStop((E_pwmTotal)x)
#define drvi_pwmLoadPrescaler(x,y)      cc6801_pwmLoadPrescaler((E_pwmTotal)x,y)
#define drvi_pwmRegisterCallback(x,y)    cc6801_pwmRegisterCallback((E_pwmTotal)x,y)

void drvi_pwmInit(void);



















#endif //_DRVI_PWM_H_


