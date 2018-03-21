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

#include "dialog.h"
#include "app_win_global.h"
#include "app_win_utility.h"
#include "CC_AppSrvc_Manager.h"

extern void APP_WIM_CreateTimer(void);

const fpApp_Window APP_WM_TABLE[APP_WIMODW_INDEX_MAX] = {
                                                    NULL,                   // Set NULL for Default is Background.
													APP_WM_CLOCK,           // User start window is start on here.
													APP_WM_PED,
													APP_WM_HEARTRATE,
													APP_WM_BATTERY,
													APP_WM_DEVICEINFO,
													NULL,
													APP_WM_ENDOFWIN
													 };


typedef struct S_WindowIdx{
    int32_t dwCurWinIdx;
    int32_t dwPreWinIdx;
}S_WinIdx;

#define APP_WIN_HANDLE_MAX            1
WM_HWIN hActiveWindow[APP_WIN_HANDLE_MAX];
S_WinIdx sWinData;

static int _APP_Win_Launch_NewWindow(void)
{
	int bRet = 0;
     if (hActiveWindow[0] != WM_HBKWIN)
    {
        WM_DeleteWindow(hActiveWindow[0]);
        //GUI_SetBkColor(GUI_BLACK);
        //GUI_Clear();
    }

	hActiveWindow[0] = APP_WM_TABLE[sWinData.dwCurWinIdx]();
	if (0 == hActiveWindow[0])
		return -1;
	return bRet;
}

static void _APP_Win_SearchNextWindow(void)
{
    int i = 0 ;
    for (i = (sWinData.dwCurWinIdx+1); i < (sizeof(APP_WM_TABLE)/sizeof(fpApp_Window) -1);i++)
    {
        if (APP_WM_ENDOFWIN == APP_WM_TABLE[i])
        {
            i = 0;
        }

        if (NULL != APP_WM_TABLE[i])
            break;
    }

    sWinData.dwCurWinIdx = i;

    if (sWinData.dwCurWinIdx != sWinData.dwPreWinIdx)
    {
        _APP_Win_Launch_NewWindow();
    }
}

static void _APP_Win_SearchPreviousWindow(void)
{
    int i = 0 ;
    for (i = (sWinData.dwCurWinIdx-1) ; i < (sizeof(APP_WM_TABLE)/sizeof(fpApp_Window) -1); i--)
    {
        if (i <= 0)
        {
            i = sWinData.dwPreWinIdx;
            break;
        }

        if (NULL != APP_WM_TABLE[i])
            break;
    }
    sWinData.dwCurWinIdx = i;

    if (sWinData.dwCurWinIdx != sWinData.dwPreWinIdx)
    {
        _APP_Win_Launch_NewWindow();
    }
}

static void _APP_Win_DeleteWindow(void)
{
    GUI_SetBkColor(GUI_BLACK);
    GUI_Clear();
    WM_DeleteWindow(hActiveWindow[0]);
    hActiveWindow[0] = WM_HBKWIN;
    sWinData.dwCurWinIdx = 0;
    sWinData.dwPreWinIdx = 0;
}

static void _APP_Win_SwitchWindow(WM_MESSAGE* pMsg)
{
	sWinData.dwPreWinIdx= sWinData.dwCurWinIdx;
	switch (pMsg->MsgId)
	{
        case WM_MSG_ID_CC_WINDOW_RETURN:
            _APP_Win_SearchPreviousWindow();
        break;
        case WM_MSG_ID_CC_WINDOW_NEXT:
        case WM_TOUCH:
            _APP_Win_SearchNextWindow();
        break;
        case WM_MSG_ID_CC_TIMER_TIMEOUT:
            _APP_Win_DeleteWindow();
        break;

        case WM_MSG_ID_CC_WINDOW_UP:
        case WM_MSG_ID_CC_WINDOW_DOWN:
        default:
        break;
	}

}

static void _cbBackGround(WM_MESSAGE* pMsg) {

	switch (pMsg->MsgId)
	{
    	case WM_PAINT:

    		GUI_SetBkColor(GUI_BLACK);
    		GUI_Clear();
    		break;
    	case WM_MSG_ID_CC_WINDOW_RETURN:
        case WM_MSG_ID_CC_WINDOW_UP:
    	case WM_MSG_ID_CC_WINDOW_DOWN:
    	case WM_MSG_ID_CC_WINDOW_NEXT:
    	case WM_MSG_ID_CC_TIMER_TIMEOUT:
        case WM_TOUCH:
    		_APP_Win_SwitchWindow(pMsg);
		break;
		default: WM_DefaultProc(pMsg); break;
	}
}


WM_HWIN APP_WIN_GetWinID(void)
{
    return (hActiveWindow[0]);
}

void MainTask(void)
{
    APP_WIN_HookScheduler();

    APP_SVCMGR_Init();
    APP_SVCMGR_PostEvent_PedoRequest(ENABLE);


    APP_WIM_CreateTimer();
	WM_SetCallback(WM_HBKWIN, _cbBackGround);
    hActiveWindow[0] = WM_HBKWIN;

    WM_MESSAGE    _tMsg;
    _tMsg.MsgId   = WM_MSG_ID_CC_WINDOW_NEXT;

    _APP_Win_SwitchWindow(&_tMsg);
}
#endif
