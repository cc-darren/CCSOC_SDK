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

__forceinline void drvi_pwmCounterGet(T_IF_ID _pwm, uint32_t *data)
{
    cc6801_pwmCounterGet((E_pwmTotal) _pwm, data);
}

__forceinline void drvi_pwmCounterClear(T_IF_ID _pwm)
{
    cc6801_pwmCounterClear((E_pwmTotal) _pwm);
}

__forceinline void drvi_pwmLoadPrescaler(T_IF_ID _pwm, uint32_t data)
{
    cc6801_pwmLoadPrescaler((E_pwmTotal) _pwm, data);
}

__forceinline void drvi_pwmStart(T_IF_ID _pwm)
{
    cc6801_pwmStart((E_pwmTotal) _pwm);
}

__forceinline void drvi_pwmStop(T_IF_ID _pwm)
{
    cc6801_pwmStop((E_pwmTotal) _pwm);
}

__forceinline void drvi_pwmRegisterCallback(T_IF_ID _pwm, T_callback _fp)
{
    cc6801_pwmRegisterCallback((E_pwmTotal) _pwm, _fp);
}

__forceinline void drvi_pwmDuty(T_IF_ID _pwm, uint32_t data)
{
    cc6801_pwmDuty((E_pwmTotal) _pwm, data);
}

void drvi_pwmInit(void);

#endif //_DRVI_PWM_H_


