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

#ifndef _WDT_H_
#define _WDT_H_
#include "global.h"

#define WDT_CLK_DISABLE_BIT                      (0)
#define WDT_CLK_DISABLE_MASK                     (WDT_CLK_DISABLE_BIT << 5)
#define WDT_CLK_ENABLE_BIT                       (1)
#define WDT_CLK_ENABLE_MASK                      (WDT_CLK_ENABLE_BIT << 5)

#define WDT_RST_ENABLE_BIT                       (0)
#define WDT_RST_ENABLE_MASK                      (WDT_RST_ENABLE_BIT << 5)
#define WDT_RST_DISABLE_BIT                      (1)
#define WDT_RST_DISABLE_MASK                     (WDT_RST_DISABLE_BIT << 5)

#define WDT_INT_ENABLE_BIT                       (1)
#define WDT_INT_ENABLE_MASK                      (WDT_INT_ENABLE_BIT << 0)

#define WDT_INT_CLEAR_BIT                        (1)
#define WDT_INT_CLEAR_MASK                       (WDT_INT_CLEAR_BIT << 16)

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

#define WDT_NO_PRESCALER_BIT                     (1)
#define WDT_NO_PRESCALER_MASK                    (WDT_NO_PRESCALER_BIT << 8)

void cc6801_WdtInit(void);
void cc6801_WdtRegisterCallback(uint32_t dwMSec, T_callback tCB);
void cc6801_WdtRstSet(uint32_t dwMSec);
void cc6801_WdtEnable(void);
void cc6801_WdtDisable(void);
void cc6801_WdtFeed(void);



















#endif //_WDT_H_
