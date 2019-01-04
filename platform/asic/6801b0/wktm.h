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

#ifndef _WKTM_H_
#define _WKTM_H_
#include "global.h"
#include "cc6801_reg.h"
#include "cc6801_options.h"

typedef enum wktmPort
{
    WKTM_0 = WKTM_PORT_0,
    WKTM_1 = WKTM_PORT_1,
    WKTM_TOTAL = WKTM_PORT_TOTAL
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




















#endif //_WKTM_H_


