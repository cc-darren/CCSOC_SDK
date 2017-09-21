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

__forceinline void drvi_WdtInit(uint32_t dwMSec)
{
    cc6801_WdtInit(dwMSec);
}

__forceinline void drvi_WdtEnable(void)
{
    cc6801_WdtEnable();
}

__forceinline void drvi_WdtDisable(void)
{
    cc6801_WdtDisable();
}

__forceinline void drvi_WdtFeed(void)
{
    cc6801_WdtFeed();
}



















#endif //_DRVI_WDT_H_
