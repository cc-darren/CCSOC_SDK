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
#include "APP_Win_Global.h"
#include "APP_Win_Utility.h"
#include "DIALOG.h"
#include "tracer.h"

/******************************************************************************
 * DEFINITION / CONSTANT / ENUM / TYPE
 ******************************************************************************/
#define APP_WIN_EVENT_CODE_TOUCH    ((uint8_t) 0x01)



extern WM_HWIN APP_WIN_GetWinID(void);



/******************************************************************************
* [FUNCTION] APP_SendMessage
*     xxxx
* [ARGUMENT] 
*     <in> .nMsgID: xxx
           .vpData: An pointer to allocated buf, where data is stored
* [RETURN  ] None.
******************************************************************************/ 
void    APP_SendMessage(int nMsgID, void *vpData)
{
    WM_MESSAGE    _tMsg;

    _tMsg.MsgId  = nMsgID;
    _tMsg.Data.p = vpData;

    WM_SendMessage(WM_GetActiveWindow(), &_tMsg);
}








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
    }
    
    
    WM_HWIN _hActive = APP_WIN_GetWinID();
    TracerInfo(  "[APP_WIN_EventHandler] _hActive = %d\r\n", _hActive);
    //if (_hActive != 0)
        WM_SendMessage(_hActive, &_tMsg);
    //else
    //    WM_SendMessage(WM_HBKWIN, &_tMsg);
    
    
    
}

/******************************************************************************
* [FUNCTION] APP_WIN_HookScheduler
*     Register event dispatch handler to scheduler of app framework
* [ARGUMENT] None.
* [RETURN  ] None.
******************************************************************************/ 
void    APP_WIN_HookScheduler(void)
{
    APP_SCHED_RegEventHandler(E_APP_SCHED_MODID_WINDOW_MGR, _AppWinEventHandler);
}

/******************************************************************************
 * FUNCTION > APP_WIN_PostEvent_TOUCH
 ******************************************************************************/
void    APP_WIN_PostEvent_TOUCH(void)
{
    S_AppSchedEvent     _tEvent;
        
    _tEvent.eModuleID     = E_APP_SCHED_MODID_WINDOW_MGR;
    _tEvent.bEventID      = APP_WIN_EVENT_CODE_TOUCH;
    _tEvent.wDataByteSize = 0;

    APP_SCHED_PostEvent(&_tEvent);
}


#endif

