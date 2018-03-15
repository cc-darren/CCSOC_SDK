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
#include "APP_Win_Global.h"
#include "App_Win_Utility.h"

extern void APP_WIM_CreateTimer(void);

const fpApp_Window APP_WM_TABLE[APP_WIMODW_INDEX_MAX] = {
													APP_WM_CLOCK,
													APP_WM_PED,
													APP_WM_HEARTRATE,
													APP_WM_BATTERY,
													APP_WM_DEVICEINFO,
													NULL,
													APP_WM_ENDOFWIN
													 };
WM_HWIN hActiveWindow[5];


#define ID_BUTTON_0		(GUI_ID_USER + 0x01)
#define ID_BUTTON_1		(GUI_ID_USER + 0x02)


#define ID_USER_SLIDE_RETURE		(GUI_ID_USER + 0xE1)
#define ID_USER_SLIDE_UP			(GUI_ID_USER + 0xE2)
#define ID_USER_SLIDE_DOWN			(GUI_ID_USER + 0xE3)
#define ID_USER_SLIDE_NEXT			(GUI_ID_USER + 0xE4)
#define ID_USER_TIMER_TIMEOUT		(GUI_ID_USER + 0xE5)
#define ID_USER_TOUCH				(GUI_ID_USER + 0xE6)

static int dwWin_Index = 0;
static int dwWin_PreIndex = 0;

static int APP_Launch_NewWindow(void)
{
	int bRet = 0;
    if (hActiveWindow[0] != WM_HBKWIN)
    {
        WM_DeleteWindow(hActiveWindow[0]);
        GUI_SetBkColor(GUI_BLACK);
        GUI_Clear();
    }

	hActiveWindow[0] = APP_WM_TABLE[dwWin_Index]();
	if (0 == hActiveWindow[0])
		return -1;
	return bRet;
}


void APP_Win_Switch_Handler(unsigned int _dwEvenID ,WM_HWIN hItem,unsigned int pBuf)
{
	dwWin_PreIndex = dwWin_Index;
	switch (_dwEvenID)
	{
	case ID_USER_SLIDE_RETURE:
    {
		int i = 0;
		for (i = (dwWin_Index -1 ) ; i < (APP_WIMODW_INDEX_MAX -1 ); i--)
		{
			if (i <= 0)
			{
				i = 0;
				break;
			}
			if (NULL != APP_WM_TABLE[i])
				break;
		}
		dwWin_Index = i;

		if (dwWin_Index != dwWin_PreIndex)
		{
			APP_Launch_NewWindow();
		}
    }
		break;
	case ID_USER_SLIDE_NEXT:
	case ID_USER_TOUCH:
    case WM_TOUCH:
    {
		int i = 0;
		for (i = (dwWin_Index + 1); i < (APP_WIMODW_INDEX_MAX -1); i++)
		{
			if (APP_WM_ENDOFWIN == APP_WM_TABLE[i])
            {
                i = 0;
            }

			if (NULL != APP_WM_TABLE[i])
				break;
		}
		dwWin_Index = i;

		if (dwWin_Index != dwWin_PreIndex)
		{
			APP_Launch_NewWindow();
		}
    }
		break;
	case ID_USER_TIMER_TIMEOUT:
		GUI_SetBkColor(GUI_BLACK);
		GUI_Clear();
		WM_DeleteWindow(hActiveWindow[0]);
        hActiveWindow[0] = WM_HBKWIN;
		//hActiveWindow[0] = 0;
		dwWin_Index = -1;
		dwWin_PreIndex = -1;
		break;
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

	case ID_USER_SLIDE_RETURE:
    case ID_USER_SLIDE_UP:
	case ID_USER_SLIDE_DOWN:
	case ID_USER_SLIDE_NEXT:
	case ID_USER_TOUCH:
	case ID_USER_TIMER_TIMEOUT:
    case WM_TOUCH:
		APP_Win_Switch_Handler(pMsg->MsgId, pMsg->hWinSrc, pMsg->Data.v);
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

    APP_WIM_CreateTimer();
	WM_SetCallback(WM_HBKWIN, _cbBackGround);
	dwWin_Index = 0;
	if (NULL == APP_WM_TABLE[dwWin_Index])
	{
		while (++dwWin_Index)
		{
			if (dwWin_Index > (APP_WIMODW_INDEX_MAX - 1))
				dwWin_Index = 0;

			if (NULL != APP_WM_TABLE[dwWin_Index])
				break;
		}
	}

	hActiveWindow[0]  = APP_WM_TABLE[dwWin_Index]();

}
#endif

