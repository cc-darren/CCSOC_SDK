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


// NORMAL WINDOW DEFEINE
extern WM_HWIN CreateWindow_DeviceInfo(void);
extern WM_HWIN CreateWindow_Clock(void);
extern WM_HWIN CreateWindow_Ped(void);
extern WM_HWIN CreateWindow_HeartRate(void);
extern WM_HWIN CreateWindow_Battery(void);
extern WM_HWIN CreateWindow_Distance(void);
extern WM_HWIN CreateWindow_Caroile(void);
extern WM_HWIN CreateWindow_Swimming(void);

#define APP_WM_CLOCK				CreateWindow_Clock
#define APP_WM_PED					CreateWindow_Ped
#define APP_WM_HEARTRATE			CreateWindow_HeartRate
#define APP_WM_BATTERY				CreateWindow_Battery
#define APP_WM_DISTANCE				CreateWindow_Distance
#define APP_WM_CAROILE              CreateWindow_Caroile
#define APP_WM_SWIMMING             CreateWindow_Swimming
#define APP_WM_DEVICEINFO			CreateWindow_DeviceInfo
#define APP_WM_ENDOFWIN				((fpApp_Window) 0xFFFFFFFF)
#define APP_WM_NULL  				0


// NOTIFICATION WINDOW DEFINE
extern WM_HWIN CreateWindow_BleBonding(void);
extern WM_HWIN CreateWindow_Factoryreset(void);
extern WM_HWIN CreateWindow_Lowpower(void);
extern WM_HWIN CreateWindow_Dbfull(void);
extern WM_HWIN CreateWindow_Chargestate(void);
extern WM_HWIN CreateWindow_Swimmingswitch(void);
extern WM_HWIN CreateWindow_Heartratewarning(void);
extern WM_HWIN CreateWindow_Alarm(void);
extern WM_HWIN CreateWindow_Longsit(void);
extern WM_HWIN CreateWindow_Pedgoal(void);
extern WM_HWIN CreateWindow_Incommingcall(void);
extern WM_HWIN CreateWindow_Incommingsms(void);

#define APP_WM_BLEBONDING		    CreateWindow_BleBonding
#define APP_WM_FACTORYRESET		    CreateWindow_Factoryreset
#define APP_WM_LOWPOWER	    	    CreateWindow_Lowpower
#define APP_WM_DBFULL	    	    CreateWindow_Dbfull
#define APP_WM_CHARGESTATE          CreateWindow_Chargestate
#define APP_WM_SWIMMINGSWITCH       CreateWindow_Swimmingswitch
#define APP_WM_HEARTRATEWARNING     CreateWindow_Heartratewarning
#define APP_WM_ALARM                CreateWindow_Alarm
#define APP_WM_LONGSIT              CreateWindow_Longsit
#define APP_WM_PEDGOAL              CreateWindow_Pedgoal
#define APP_WM_INCOMMINGCALL        CreateWindow_Incommingcall
#define APP_WM_INCOMMINGSMS         CreateWindow_Incommingsms


typedef enum
{
    E_WIN_NOTIFY_BONDING = 0,
    E_WIN_NOTIFY_FACTORY,
    E_WIN_NOTIFY_LOWPOWER,
    E_WIN_NOTIFY_DBFULL,
    E_WIN_NOTIFY_CHARGE_STATE,
    E_WIN_NOTIFY_SWIMMING_SW,
    E_WIN_NOTIFY_HRWARM,
    E_WIN_NOTIFY_ALARM,
    E_WIN_NOTIFY_LONGSIT,
    E_WIN_NOTIFY_INCALL,
    E_WIN_NOTIFY_INSMS,
    E_WIN_NOTIFY_PEDGOAL,
    E_WIN_NOTIFY_MAX = 12,
}E_WIN_NOTIFY;

#endif // __APP_WM_GLOBAL_H__

#endif

