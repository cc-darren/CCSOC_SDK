
#ifdef EMWIN_ENABLE

#include "dialog.h"
#include "GUI.h"
#include "WM.h"

#include "tracer.h"
WM_HWIN hPage[5];


#define ID_BUTTON_0		(GUI_ID_USER + 0x01)
#define ID_BUTTON_1		(GUI_ID_USER + 0x02)



#define PAGE_CLOCK					(GUI_ID_USER + 0xD0)
#define PAGE_PEDO					(GUI_ID_USER + 0xD1)
#define PAGE_HR						(GUI_ID_USER + 0xD2)
#define PAGE_BATTERY				(GUI_ID_USER + 0xD3)
#define PAGE_PEDO_DISTANCE			(GUI_ID_USER + 0xD4)

#define ID_USER_SLIDE_RETURE		(GUI_ID_USER + 0xE1)
#define ID_USER_SLIDE_UP			(GUI_ID_USER + 0xE2)
#define ID_USER_SLIDE_DOWN			(GUI_ID_USER + 0xE3)
#define ID_USER_SLIDE_NEXT			(GUI_ID_USER + 0xE4)
#define ID_USER_TIMER_TIMEOUT		(GUI_ID_USER + 0xE5)
#define ID_USER_TOUCH				(GUI_ID_USER + 0xE6)

void APP_Win_Switch_Handler(unsigned int _dwEvenID ,WM_HWIN hItem,unsigned int pBuf)
{
	switch (_dwEvenID)
	{
		case ID_USER_SLIDE_RETURE:
		if (pBuf == PAGE_PEDO)
		{

			WM_DeleteWindow(hPage[0]);
			hPage[0] = CreateWindow1(WM_HBKWIN);

		}

		if (pBuf == PAGE_HR)
		{

			WM_DeleteWindow(hPage[0]);
			hPage[0] = CreateWindow2(WM_HBKWIN);

		}
		if (pBuf == PAGE_BATTERY)
		{

			WM_DeleteWindow(hPage[0]);
			hPage[0] = CreateWindow3(WM_HBKWIN);

		}
		if (pBuf == PAGE_PEDO_DISTANCE)
		{

			WM_DeleteWindow(hPage[0]);
			hPage[0] = CreateWindow4(WM_HBKWIN);

		}

		break;
		case ID_USER_SLIDE_UP:

		break;
		case ID_USER_SLIDE_DOWN:

		break;
		case ID_USER_SLIDE_NEXT:
		case ID_USER_TOUCH:
		if (pBuf == PAGE_CLOCK)
		{

			WM_DeleteWindow(hPage[0]);
			hPage[0] = CreateWindow2(WM_HBKWIN);

		}

		if (pBuf == PAGE_PEDO)
		{
			WM_DeleteWindow(hPage[0]);
			hPage[0] = CreateWindow3(WM_HBKWIN);
		}

		if (pBuf == PAGE_HR)
		{
			WM_DeleteWindow(hPage[0]);
			hPage[0] = CreateWindow4(WM_HBKWIN);
		}

		if (pBuf == PAGE_BATTERY)
		{
			WM_DeleteWindow(hPage[0]);
			hPage[0] = CreateWindow5(WM_HBKWIN);
		}

		if (pBuf == PAGE_PEDO_DISTANCE)
		{
			WM_DeleteWindow(hPage[0]);
			hPage[0] = CreateWindow1(WM_HBKWIN);
		}
		break;
		default:
		break;

	}

}

static void _cbBackGround(WM_MESSAGE* pMsg) {	//?????????

	switch (pMsg->MsgId)
	{
	case WM_PAINT:
		JDI_Start_Draw();
		GUI_SetBkColor(GUI_WHITE);				//??????
		GUI_Clear();							//??
		JDI_End_Draw();

        TracerInfo("== BK WINDOW 1 WM_PAINT ==\r\n");
		break;

	case ID_USER_SLIDE_RETURE:
		APP_Win_Switch_Handler(ID_USER_SLIDE_RETURE, pMsg->hWinSrc, pMsg->Data.v);
		break;
    case ID_USER_SLIDE_UP:
		break;
	case ID_USER_SLIDE_DOWN:
		break;
	case ID_USER_SLIDE_NEXT:
		APP_Win_Switch_Handler(ID_USER_SLIDE_NEXT,pMsg->hWinSrc,pMsg->Data.v);
		break;
	case ID_USER_TOUCH:
		APP_Win_Switch_Handler(ID_USER_TOUCH, pMsg->hWinSrc, pMsg->Data.v);
		break;
    case ID_USER_TIMER_TIMEOUT:
		GUI_SetBkColor(GUI_BLACK);				//??????
		GUI_Clear();
		WM_DeleteWindow(hPage[0]);
		hPage[0] = 0;

		break;
	case WM_TOUCH:
		if (hPage[0] == 0)
			hPage[0] = CreateWindow1(WM_HBKWIN);
		break;
	default: WM_DefaultProc(pMsg); break;
	}
}

void MainTask(void)
{
    //WM_SetCreateFlags(WM_CF_MEMDEV);
    //WM_SetCreateFlags(WM_CF_SHOW);
    //WM_SetCreateFlags(WM_CF_LATE_CLIP);
    //WM_MULTIBUF_Enable(1);
	WM_SetCallback(WM_HBKWIN, _cbBackGround);	//???????????
    hPage[0] = CreateWindow1(WM_HBKWIN);		//????,????????

#if 0
    hPage[1] = CreateWindow2(WM_HBKWIN);		//????,????????
    hPage[2] = CreateWindow3(WM_HBKWIN);		//????,????Page 1
    hPage[3] = CreateWindow4(WM_HBKWIN);		//????,????????
	hPage[4] = CreateWindow5(WM_HBKWIN);		//????,????????
#endif
}

#endif


