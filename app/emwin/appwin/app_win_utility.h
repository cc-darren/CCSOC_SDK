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

#include "app_win_global.h"
#include "DIALOG.h"


/******************************************************************************
* [FUNCTION] APP_WIN_DefaultProcess
*     Cloudchip proprietary default handler for events not being processed
* [ARGUMENT] pMsg: a pointer where message is stored.
* [RETURN  ] None.
* [USAGE   ] For emWin internal use.
******************************************************************************/
void    APP_WIN_DefaultProcess(WM_MESSAGE * pMsg);

/******************************************************************************
* [FUNCTION] APP_WIN_HookScheduler
*     Register event dispatch handler to scheduler of app framework
* [ARGUMENT] None.
* [RETURN  ] None.
* [USAGE   ] For external module use.
******************************************************************************/
extern void    APP_WIN_HookScheduler(void);

/******************************************************************************
* [FUNCTION] APP_WIN_PostEvent_TOUCH
*     Post TOUCH event to notify emWin that touch event is detected.
* [ARGUMENT] None.
* [RETURN  ] None.
* [USAGE   ] For external module use.
******************************************************************************/
extern void    APP_WIN_PostEvent_TOUCH(void);

/******************************************************************************
* [FUNCTION] APP_WIN_PostEvent_PedReport
*     Post PED report to display updated steps.
* [ARGUMENT] dwStepCount: the latest steps.
* [RETURN  ] None.
* [USAGE   ] For external module use.
******************************************************************************/
void    APP_WIN_PostEvent_PedReport(uint32_t dwStepCount);

/******************************************************************************
* [FUNCTION] APP_WIN_PostEvent_TOUCH
*     Post DRWA WINDOW event to notify OLED driver that draw event is detected.
* [ARGUMENT] None.
* [RETURN  ] None.
* [USAGE   ] For external module use.
 ******************************************************************************/
void    APP_WIN_PostEvent_DrawWindow(void);

#endif

