/**
 ****************************************************************************************
 *
 * @file wktm.h
 *
 * @brief Head file of wktm.c
 *
 * Copyright (C) CloudChip 2017-2019
 *
 *
 ****************************************************************************************
 */
#ifndef _PWM_H_
#define _PWM_H_
#include "global.h"

void cc6801_pwm0_init(T_callback);
void cc6801_pwm0_counterGet(uint32_t*);
void cc6801_pwm0_counterClear(void);
void cc6801_pwm0_duty(uint32_t);
void cc6801_pwm0_start(void);
void cc6801_pwm0_stop(void);
void cc6801_pwm0_loadPrescaler(uint32_t);


void cc6801_pwm1_init(T_callback);


















#endif //_PWM_H_


