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

/******************************************************************************
*  Filename:
*  ---------
*  xxxx.c
*
*  Project:
*  --------
*  cc6801
*
*  Description:
*  ------------
*  xxxx,
*      (1) xxx
*
*  Author:
*  -------
*  CloudChip
*
*===========================================================================/
*  20180315 JASON initial version
******************************************************************************/

#ifdef EMWIN_ENABLE

/******************************************************************************
 * INCLUDE FILE
 ******************************************************************************/
#include "scheduler.h"
#include "app_win_global.h"
#include "app_win_utility.h"
#include "DIALOG.h"
#include "tracer.h"
#ifdef JDI_OLED_ENABLE_208x208
#include "JDI_LPM010M297B.h"
#endif
#ifdef JDI_OLED_ENABLE_176x176
#include "JDI_LPM013M126A.h"
#endif

/******************************************************************************
 * DEFINITION / CONSTANT / ENUM / TYPE
 ******************************************************************************/
#define APP_WIN_EVENT_CODE_TOUCH         ((uint8_t) 0x00)
#define APP_WIN_EVENT_CODE_PED_REPORT    ((uint8_t) 0x10)
#define APP_WIN_EVENT_CODE_DRAW_WINDOW   ((uint8_t) 0x20)


extern WM_HWIN APP_WIN_GetWinID(void);








/******************************************************************************
* [FUNCTION] _AppWinEventHandler
*     Redispatch events to emWin
* [ARGUMENT]
*     <in> ptAppEvent: a pointer to the struct where below info. specified,
*            .eModuleID    : ID of the dest. module to handle this event
*            .bEventID     : ID of the event, defined by each module
*            .wDataByteSize: Byte-size of allocated buffer for additional data
*            .vpData       : An pointer to allocated buf, where data is stored
* [RETURN  ] None.
******************************************************************************/
static void   _AppWinEventHandler(S_AppSchedEvent *tEvent)
{
    WM_MESSAGE    _tMsg;

    TracerInfo(  "[APP_WIN_EventHandler] event: %d, argv: 0x%08X\r\n"
               , tEvent->bEventID
               , ((uint32_t) tEvent->vpData)                         );

    switch (tEvent->bEventID)
    {
    case APP_WIN_EVENT_CODE_TOUCH:
         _tMsg.MsgId = WM_TOUCH;
         break;

    case APP_WIN_EVENT_CODE_PED_REPORT:
         _tMsg.MsgId  = WM_MSG_ID_CC_PED_REPORT;
         _tMsg.Data.d = ((uint32_t) tEvent->vpData);
         break;
    case APP_WIN_EVENT_CODE_DRAW_WINDOW:
         _tMsg.MsgId = 0x00; // Send NULL event
         JDI_End_Draw();
         break;
    }

    WM_HWIN _hActive = APP_WIN_GetWinID();
    TracerInfo(  "[APP_WIN_EventHandler] _hActive = %d\r\n", _hActive);
    WM_SendMessage(_hActive, &_tMsg);
}

/******************************************************************************
* [FUNCTION] APP_WIN_DefaultProcess
*     Cloudchip proprietary default handler for events not being processed
* [ARGUMENT] pMsg: a pointer where message is stored.
* [RETURN  ] None.
* [USAGE   ] For emWin internal use.
******************************************************************************/
void    APP_WIN_DefaultProcess(WM_MESSAGE * pMsg)
{
    switch (pMsg->MsgId)
    {
// enable below code to handle all events defined by cloudchip. (e.g. BLE notification)
#if 0
    case XXXXX:
        WM_SendMessage(WM_HBKWIN, pMsg);
        break;
#endif

    default:
        WM_DefaultProc(pMsg);
        break;
    }
}

/******************************************************************************
* [FUNCTION] APP_WIN_HookScheduler
*     Register event dispatch handler to scheduler of app framework
* [ARGUMENT] None.
* [RETURN  ] None.
* [USAGE   ] For external module use.
******************************************************************************/
void    APP_WIN_HookScheduler(void)
{
    APP_SCHED_RegEventHandler(E_APP_SCHED_MODID_WINDOW_MGR, _AppWinEventHandler);
}

/******************************************************************************
* [FUNCTION] APP_WIN_PostEvent_TOUCH
*     Post TOUCH event to notify emWin that touch event is detected.
* [ARGUMENT] None.
* [RETURN  ] None.
* [USAGE   ] For external module use.
 ******************************************************************************/
void    APP_WIN_PostEvent_TOUCH(void)
{
    S_AppSchedEvent     _tEvent;

    _tEvent.eModuleID     = E_APP_SCHED_MODID_WINDOW_MGR;
    _tEvent.bEventID      = APP_WIN_EVENT_CODE_TOUCH;
    _tEvent.wDataByteSize = 0;

    APP_SCHED_PostEvent(&_tEvent);
}

/******************************************************************************
* [FUNCTION] APP_WIN_PostEvent_PedReport
*     Post PED report to display updated steps.
* [ARGUMENT] dwStepCount: the latest steps.
* [RETURN  ] None.
* [USAGE   ] For external module use.
******************************************************************************/
void    APP_WIN_PostEvent_PedReport(uint32_t dwStepCount)
{
    S_AppSchedEvent     _tEvent;

    _tEvent.eModuleID     = E_APP_SCHED_MODID_WINDOW_MGR;
    _tEvent.bEventID      = APP_WIN_EVENT_CODE_PED_REPORT;
    _tEvent.wDataByteSize = 0;
    _tEvent.vpData        = ((void *) dwStepCount);

    APP_SCHED_PostEvent(&_tEvent);
}


/******************************************************************************
* [FUNCTION] APP_WIN_PostEvent_TOUCH
*     Post DRWA WINDOW event to notify OLED driver that draw event is detected.
* [ARGUMENT] None.
* [RETURN  ] None.
* [USAGE   ] For external module use.
 ******************************************************************************/
void    APP_WIN_PostEvent_DrawWindow(void)
{
    S_AppSchedEvent     _tEvent;

    _tEvent.eModuleID     = E_APP_SCHED_MODID_WINDOW_MGR;
    _tEvent.bEventID      = APP_WIN_EVENT_CODE_DRAW_WINDOW;
    _tEvent.wDataByteSize = 0;

    APP_SCHED_PostEvent(&_tEvent);
}

#endif

