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

#ifndef _PWM_H_
#define _PWM_H_
#include "global.h"
#include "cc6801_reg.h"

typedef enum
{
    PWM_0 = 0,
    PWM_1,
    PWM_TOTAL
} E_pwmTotal;

typedef struct
{
    U_regPWMWKTM *pReg;
    T_callback fpCallback;
} T_pwmPort;


void cc6801_pwmInit(E_pwmTotal);
void cc6801_pwmCounterGet(E_pwmTotal, uint32_t*);
void cc6801_pwmCounterClear(E_pwmTotal);
void cc6801_pwmDuty(E_pwmTotal, uint32_t);
void cc6801_pwmStart(E_pwmTotal);
void cc6801_pwmStop(E_pwmTotal);
void cc6801_pwmLoadPrescaler(E_pwmTotal, uint32_t);
void cc6801_pwmRegisterCallback(E_pwmTotal, T_callback);




















#endif //_PWM_H_


