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
 
#ifndef _DRVI_TIMER_H_
#define _DRVI_TIMER_H_

#include "wktm.h"

__forceinline void drvi_wktmCounterGet(T_IF_ID _wktm, uint32_t *_data)
{
    cc6801_wktmCounterGet((E_wktmTotal)_wktm, _data);
}

__forceinline void drvi_wktmCounterClear(T_IF_ID _wktm)
{
    cc6801_wktmCounterClear((E_wktmTotal)_wktm);
}

__forceinline void drvi_wktmCounterLoad(T_IF_ID _wktm, uint32_t _data)
{
    cc6801_wktmCounterLoad((E_wktmTotal)_wktm, _data);
}

__forceinline void drvi_wktmStart(T_IF_ID _wktm)
{
    cc6801_wktmStart((E_wktmTotal)_wktm);
}

__forceinline void drvi_wktmStop(T_IF_ID _wktm)
{
    cc6801_wktmStop((E_wktmTotal)_wktm);
}

__forceinline void drvi_wktmRegisterCallback(T_IF_ID _wktm, T_callback _fp)
{
    cc6801_wktmRegisterCallback((E_wktmTotal)_wktm, _fp);
}

void drvi_wktmInit(void);

#endif //_DRVI_TIMER_H_
