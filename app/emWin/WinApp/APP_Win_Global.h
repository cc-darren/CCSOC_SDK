/* Copyright (c) 2018 Cloudchip, Inc. All Rights Reserved.
 *
 * The information contained herein is property of Cloudchip, Inc.
 * Terms and conditions of usage are described in detail in CLOUDCHIP
 * STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information.
 * NO WARRANTY of ANY KIND is provided. This heading must NOT be removed
 * from the file.
 */

#ifdef EMWIN_ENABLE

#ifndef __APP_WM_GLOBAL_H__
#define __APP_WM_GLOBAL_H__
#include "DIALOG.h"

#define APP_WIM_TIMER_TICK          10
#define APP_WIM_TIMER_TIMEOUT       APP_WIM_TIMER_TICK*5

#define APP_WIMODW_INDEX_MAX 255
typedef WM_HWIN (*fpApp_Window)(void);

extern WM_HWIN CreateWindow_DeviceInfo(void);
extern WM_HWIN CreateWindow_Clock(void);
extern WM_HWIN CreateWindow_Ped(void);
extern WM_HWIN CreateWindow_HeartRate(void);
extern WM_HWIN CreateWindow_Battery(void);


#define APP_WM_CLOCK				CreateWindow_Clock
#define APP_WM_PED					CreateWindow_Ped
#define APP_WM_HEARTRATE			CreateWindow_HeartRate
#define APP_WM_BATTERY				CreateWindow_Battery
#define APP_WM_DEVICEINFO			CreateWindow_DeviceInfo
#define APP_WM_ENDOFWIN				((const fpApp_Window) 0xFFFFFFFF)
#define APP_WM_NULL  				0




#endif // __APP_WM_GLOBAL_H__

#endif

