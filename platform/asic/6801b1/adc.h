/**
 ****************************************************************************************
 *
 * @file adc.h
 *
 * @brief Head file of adc.c
 *
 * Copyright (C) CloudChip 2017-2019
 *
 *
 ****************************************************************************************
 */
#ifndef _ADC_H_
#define _ADC_H_
#include "global.h"
#include "project.h"

#define ADC_INTERNAL_R2_OHM     1000000     //default internal resistor divider in CC6801 ADC input


typedef enum
{
    ADC_VBR12_OUTPUT_1V208 = 0,
    ADC_VBR12_OUTPUT_1V222,
    ADC_VBR12_OUTPUT_1V237,
    ADC_VBR12_OUTPUT_1V252,
    ADC_VBR12_OUTPUT_1V150,
    ADC_VBR12_OUTPUT_1V165,
    ADC_VBR12_OUTPUT_1V180,
    ADC_VBR12_OUTPUT_1V193,
}E_AdcBGRSelect;


void cc6801_AdcEnable(void);
void cc6801_AdcDisable(void);
void cc6801_AdcInit(void);
int cc6801_AdcVbatt(uint32_t*);
void cc6801_AdcBGRSelSet(E_AdcBGRSelect);
E_AdcBGRSelect cc6801_AdcBGRSelGet(void);



















#endif //_ADC_H_
