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


#define drvi_pwm0_init(x)           cc6801_pwm0_init(x)
#define drvi_pwm0_counterGet(x)     cc6801_pwm0_counterGet(x)
#define drvi_pwm0_counterClear()    cc6801_pwm0_counterClear()
#define drvi_pwm0_duty(x)           cc6801_pwm0_duty(x)
#define drvi_pwm0_start()           cc6801_pwm0_start()
#define drvi_pwm0_stop()            cc6801_pwm0_stop()
#define drvi_pwm0_loadPrescaler(x)  cc6801_pwm0_loadPrescaler(x)


#define drvi_pwm1_init(x)         cc6801_pwm1_init(x)
#define drvi_pwm1_counterGet(x)   cc6801_pwm1_counterGet(x)




















#endif //_DRVI_PWM_H_


