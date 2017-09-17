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


#define drvi_wktmCounterGet(x,y)        cc6801_wktmCounterGet((E_wktmTotal)x,y)
#define drvi_wktmCounterClear(x)        cc6801_wktmCounterClear((E_wktmTotal)x)
#define drvi_wktmCounterLoad(x,y)       cc6801_wktmCounterLoad((E_wktmTotal)x,y)
#define drvi_wktmStart(x)               cc6801_wktmStart((E_wktmTotal)x)
#define drvi_wktmStop(x)                cc6801_wktmStop((E_wktmTotal)x)
#define drvi_wktmRegisterCallback(x,y)  cc6801_wktmRegisterCallback((E_wktmTotal)x,y)

void drvi_wktmInit(void);













#endif //_DRVI_TIMER_H_


