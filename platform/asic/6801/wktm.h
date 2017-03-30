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
#ifndef _WKTM_H_
#define _WKTM_H_
#include "global.h"

void cc6801_timer0_init(T_callback);
void cc6801_timer0_counterGet(uint32_t*);
void cc6801_timer0_counterClear(void);
void cc6801_timer0_counterLoad(uint32_t);
void cc6801_timer0_start(void);
void cc6801_timer0_stop(void);


void cc6801_timer1_init(T_callback);


















#endif //_WKTM_H_


