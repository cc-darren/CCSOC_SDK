/**
 ****************************************************************************************
 *
 * @file drvi_wdt.h
 *
 * @brief Head file of drvi_wdt.c
 *
 * Copyright (C) CloudChip 2017-2019
 *
 *
 ****************************************************************************************
 */
#ifndef _DRVI_WDT_H_
#define _DRVI_WDT_H_
#include "wdt.h"


#define drvi_wdt_init(x)         cc6801_wdt_init(x)
#define drvi_wdt_enable()        cc6801_wdt_enable()
#define drvi_wdt_feed()          cc6801_wdt_feed()






















#endif //_DRVI_WDT_H_
