/**
 ****************************************************************************************
 *
 * @file wdt.h
 *
 * @brief Head file of wdt.c
 *
 * Copyright (C) CloudChip 2017-2019
 *
 *
 ****************************************************************************************
 */
#ifndef _WDT_H_
#define _WDT_H_
#include "global.h"

void cc6801_wdt_init(uint32_t reload_value);
void cc6801_wdt_enable(void);
void cc6801_wdt_feed(void);



















#endif //_WDT_H_
