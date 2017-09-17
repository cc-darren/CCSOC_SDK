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
#include "cc6801_reg.h"

typedef enum
{
    WKTM_0 = 0,
    WKTM_1,
    WKTM_TOTAL
} E_wktmTotal;

typedef struct
{
    U_regPWMWKTM *pReg;
    T_callback fpCallback;
} T_wktmPort;


void cc6801_wktmInit(E_wktmTotal);
void cc6801_wktmCounterGet(E_wktmTotal, uint32_t*);
void cc6801_wktmCounterClear(E_wktmTotal);
void cc6801_wktmCounterLoad(E_wktmTotal,uint32_t);
void cc6801_wktmStart(E_wktmTotal);
void cc6801_wktmStop(E_wktmTotal);
void cc6801_wktmRegisterCallback(E_wktmTotal, T_callback);


void cc6801_timer1_init(T_callback);


















#endif //_WKTM_H_


