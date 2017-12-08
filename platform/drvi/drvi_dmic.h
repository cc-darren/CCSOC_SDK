/**
 ****************************************************************************************
 *
 * @file drvi_dmic.h
 *
 * @brief Head file of drvi_dmic.c
 *
 * Copyright (C) CloudChip 2017-2019
 *
 *
 ****************************************************************************************
 */
#ifndef _DRVI_DMIC_H_
#define _DRVI_DMIC_H_
#include "dmic.h"

#define drvi_dmicInit(x)                     cc6801_dmicInit(x)
#define drvi_dmicLeftConfig(x, y)            cc6801_dmicLeftConfig(x, y)
#define drvi_dmicRightConfig(x, y)           cc6801_dmicRightConfig(x, y)
#define drvi_dmicEnableBoth(x)               cc6801_dmicEnableBoth(x)
#define drvi_dmicDisableBoth(x)              cc6801_dmicDisableBoth(x)
#define drvi_dmicReadCurrentPos(x, y)        cc6801_dmicReadCurrentPos(x, y)














#endif //_DRVI_DMIC_H_


