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

#ifndef _DRVI_ADC_H_
#define _DRVI_ADC_H_
#include "adc.h"

typedef enum
{
    ADC_CAL_DEFAULT = 0,
    ADC_CAL_HIGHER,
    ADC_CAL_LOWER,
}E_ADC_CAL;
    

__forceinline void drvi_AdcInit(void)
{
    cc6801_AdcInit();
}

__forceinline void drvi_AdcEnable(void)
{
    cc6801_AdcEnable();
}

__forceinline void drvi_AdcDisable(void)
{
    cc6801_AdcDisable();
}

__forceinline void drvi_AdcVbatt(uint32_t *dwValue)
{
    cc6801_AdcVbatt(dwValue);
}

__forceinline void drvi_AdcCalibrate(E_ADC_CAL eOpt)
{
    E_AdcBGRSelect bCurrentBGR;
    
    bCurrentBGR = cc6801_AdcBGRSelGet();
    if(eOpt == ADC_CAL_HIGHER)
    {
        if(bCurrentBGR == 7) return;
        cc6801_AdcBGRSelSet((E_AdcBGRSelect)(bCurrentBGR+1));
    }
    else if(eOpt == ADC_CAL_LOWER)
    {
        if(bCurrentBGR == 0) return;
        cc6801_AdcBGRSelSet((E_AdcBGRSelect)(bCurrentBGR-1));
    }
    else
    {
        cc6801_AdcBGRSelSet((E_AdcBGRSelect)0);
    }
}




















#endif //_DRVI_WDT_H_
