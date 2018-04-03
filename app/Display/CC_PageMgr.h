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

#ifndef _CC_PAGE_MGR__
#define _CC_PAGE_MGR__

// DB for temporarily
//#define CC_DB_Check_DB_Full()     0x00

// HRM for temporarily
/*
#define CC_AppSrv_HR_IsHrsEnabled()   0x00
#define CC_AppSrv_HR_StopSingleHR()     ;
#define CC_AppSrv_HR_StartSingleHR()    ;
#define CC_AppSrv_HR_StopSingleHR()     ;
#define CC_AppSrv_HR_StopHRS()          ;
#define CC_AppSrv_HR_Stop24HR()         ;
#define CC_AppSrv_HR_StartHRS()         ;
//#define CC_GetHrmStatus()           eHRM_Off
#define CC_MainGet_HrmData()         10
*/  

// Low power for temporarily
#define CC_Battery_GetPreLowPower()   0x00
#define CC_GetBatteryPwrState()       eSysStateNormal  
//#define CC_GetChargeFullStatus()      eDEVICE_CHARGE_NOFULL
//#define CC_MainGet_BatteryCapacity()    100

// Swim for temporarily
#define CC_Check_and_Set_Swim_Mode()    ;


// Calorie for temporarily
//#define CC_MainGet_Calorie()        0x00

// Pedo for temporarily
//#define CC_MainGet_PedCnt()         100



typedef enum
{
    eStep_MainPage =1,
    eStep_SubPage,
    eStep_CloseOLED,
    eStep_DummyStep
}ePageMgrStep;


void CC_PageMgr_Init(void);
void CC_PageMgr_Proc(S_VenusEvent *_stEvent);
void CC_PageMgr_ExecWakeup(void);
void CC_PageMgr_ExecSleep(void);
uint8_t CC_PageMgr_ActiveOLED(void);
uint8_t CC_PageMgr_IsBlockingOLED(void);
uint8_t CC_PageMgr_IsOLEDActive(void);
eMMI_Page_t _PageMgr_GetPrePage(void);

#endif
