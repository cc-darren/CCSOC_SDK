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

typedef int (*cc6801_AdcCB)(uint32_t *pdwBatVol);

void cc6801_AdcInit(cc6801_AdcCB fpAdcCB);
int cc6801_AdcSample(uint32_t *dwValue);
uint32_t cc6801_Adc2BatVol(uint32_t dwAdcVol);


















#endif //_ADC_H_
