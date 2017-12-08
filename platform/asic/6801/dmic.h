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
#ifndef _DMIC_H_
#define _DMIC_H_
#include "global.h"

void cc6801_dmicInit(void*);
void cc6801_dmicLeftConfig(uint32_t, uint32_t);
void cc6801_dmicRightConfig(uint32_t, uint32_t);
void cc6801_dmicEnableBoth(uint32_t);
void cc6801_dmicDisableBoth(void);
void cc6801_dmicReadCurrentPos(uint32_t, uint32_t);














#endif //_DMIC_H_


