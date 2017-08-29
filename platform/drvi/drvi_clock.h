/**
 ****************************************************************************************
 *
 * @file drvi_clock.h
 *
 * @brief Head file of drvi_clock.c
 *
 * Copyright (C) CloudChip 2017-2019
 *
 *
 ****************************************************************************************
 */
#ifndef _DRVI_CLOCK_H_
#define _DRVI_CLOCK_H_
#include "clock.h"

#define drvi_ClockSysClkAdjust(x)               cc6801_ClockSysClkAdjust(x)
#define drvi_ClockPeripheralClkAdjust()         cc6801_ClockPeripheralClkAdjust()
#define drvi_ClockDelayUs(x)                    cc6801_ClockDelayUs(x)
#define drvi_ClockDelayMs(x)                    cc6801_ClockDelayMs(x)



















#endif //_DRVI_CLOCK_H_


