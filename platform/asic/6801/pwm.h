/**
 ****************************************************************************************
 *
 * @file pwm.h
 *
 * @brief Head file of pwm.c
 *
 * Copyright (C) CloudChip 2017-2019
 *
 *
 ****************************************************************************************
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


