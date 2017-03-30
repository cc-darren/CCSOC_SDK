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
#ifndef _DRVI_TIMER_H_
#define _DRVI_TIMER_H_
#include "wktm.h"


#define drvi_timer0_init(x)         cc6801_timer0_init(x)
#define drvi_timer0_counterGet(x)   cc6801_timer0_counterGet(x)
#define drvi_timer0_counterClear()  cc6801_timer0_counterClear()
#define drvi_timer0_counterLoad(x)  cc6801_timer0_counterLoad(x)
#define drvi_timer0_start()         cc6801_timer0_start()
#define drvi_timer0_stop()          cc6801_timer0_stop()


#define drvi_timer1_init(x)         cc6801_timer1_init(x)
#define drvi_timer1_counterGet(x)   cc6801_timer1_counterGet(x)




















#endif //_DRVI_TIMER_H_


