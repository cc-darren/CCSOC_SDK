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
 
#ifndef _DRVI_WDT_H_
#define _DRVI_WDT_H_
#include "wdt.h"

__forceinline void drvi_WdtInit(void)
{
    cc6801_WdtInit();
}

__forceinline void drvi_WdtRegisterCallback(uint32_t dwMSec, T_callback tCB)
{
    cc6801_WdtRegisterCallback(dwMSec, tCB);
}

__forceinline void drvi_WdtRstSet(uint32_t dwMSec)
{
    cc6801_WdtRstSet(dwMSec);
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
