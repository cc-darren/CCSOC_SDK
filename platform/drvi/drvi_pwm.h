/**
 ****************************************************************************************
 *
 * @file drvi_timer.h
 *
 * @brief Head file of drvi_timer.c
 *
 * Copyright (C) CloudChip 2017-2019
 *
 *
 ****************************************************************************************
 */
#ifndef _DRVI_PWM_H_
#define _DRVI_PWM_H_
#include "pwm.h"


#define drvi_pwmCounterGet(x,y)     	cc6801_pwmCounterGet((E_pwmTotal)x,y)
#define drvi_pwmCounterClear(x)    		cc6801_pwmCounterClear((E_pwmTotal)x)
#define drvi_pwmDuty(x,y)           	cc6801_pwmDuty((E_pwmTotal)x,y)
#define drvi_pwmStart(x)           		cc6801_pwmStart((E_pwmTotal)x)
#define drvi_pwmStop(x)            		cc6801_pwmStop((E_pwmTotal)x)
#define drvi_pwmLoadPrescaler(x,y)  	cc6801_pwmLoadPrescaler((E_pwmTotal)x,y)
#define drvi_pwmRegisterCallback(x,y)	cc6801_pwmRegisterCallback((E_pwmTotal)x,y)

void drvi_pwmInit(void);



















#endif //_DRVI_PWM_H_


