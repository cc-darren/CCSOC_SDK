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

#define WDT_INT_DISABLE_BIT                      (0)
#define WDT_INT_DISABLE_MASK                     (WDT_INT_DISABLE_BIT << 0)
#define WDT_INT_ENABLE_BIT                       (1)
#define WDT_INT_ENABLE_MASK                      (WDT_INT_ENABLE_BIT << 0)

#define WDT_INT_CLEAR_BIT                        (1)
#define WDT_INT_CLEAR_MASK                       (WDT_INT_CLEAR_BIT << 16)

#define WDT_ENABLE_BIT                           (0)
#define WDT_ENABLE_MASK                          (WDT_ENABLE_BIT << 0)
#define WDT_DISABLE_BIT                          (1)
#define WDT_DISABLE_MASK                         (WDT_DISABLE_BIT << 0)

#define WDT_SELECT_BIT                           (0)
#define WDT_SELECT_MASK                          (WDT_SELECT_BIT << 1)
#define TIMER_SELECT_BIT                         (1)
#define TIMER_SELECT_MASK                        (TIMER_SELECT_BIT << 1)

#define WDT_RESET_DISABLE_BIT                    (0)
#define WDT_RESET_DISABLE_MASK                   (WDT_RESET_DISABLE_BIT << 2)
#define WDT_RESET_ENABLE_BIT                     (1)
#define WDT_RESET_ENABLE_MASK                    (WDT_RESET_ENABLE_BIT << 2)

#define WDT_NO_PRESCALER_BIT                     (0)
#define WDT_NO_PRESCALER_MASK                    (WDT_NO_PRESCALER_BIT << 8)

void cc6801_WdtInit(uint32_t dwMSec);
void cc6801_WdtEnable(void);
void cc6801_WdtDisable(void);
void cc6801_WdtFeed(void);



















#endif //_WDT_H_
