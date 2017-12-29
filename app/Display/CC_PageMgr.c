
#include <stdint.h>
#include <stdbool.h>
#include "project.h"
//#include "CC_HRM_drv.h"
#include "CC_DisplayService.h"
#include "CC_PageMgr.h"
//#include "ssd1306.h"
//#include "ssd1306_128x32.h"
#include "appDisplay.h"
#include "CC_Vib_Service.h"
#include "Icon.h"

#include "CC_AppSrvc_HeartRate.h"

#include "appTime.h"
#include "cc_db.h"
#include "tracer.h"


#define GENERALTIME_TURN_ON_OFF_OLED             100
#define GENERALTIME_PAGESWITCH                        400
#define GENERALTIME_PAGEIDLE                             5000
#define GENERALTIME_HRMPROG                             25000
#define GENERALTIME_INCOMMING                          30000
#define GENERALTIME_TURN_OFF_OLED_NOW               10


extern void CC_VENUS_OLEDWakUpTimeOutTimerStart(uint16_t _wdata);
extern void CC_VENUS_OLEDWakUpTimeOutTimerStop(void);
extern void CC_VENUS_OLEDSleepTimeOutTimerStart(uint16_t _wdata);
extern void CC_VENUS_OLEDSleepTimeOutTimerStop(void);
extern void CC_VENUS_OLEDGeneralOutTimerStart(uint16_t _wdata);
extern void CC_VENUS_OLEDGeneralOutTimerReset(void);
extern void CC_VENUS_OLEDGeneralOutTimerStop(void);
extern eHrmOp_State CC_GetHrmStatus(void);
extern void CC_SetHrmClosePageEvent(void);
extern void CC_MainGet_CurrentTime(app_date_time_t *_stCurTime);
extern uint32_t CC_MainGet_PedCnt(void);
extern void CC_MainGet_Stride_LengthSetting(uint8_t *_pbStride, uint8_t *_pbUnit);
extern uint32_t CC_MainGet_Calorie(void);
extern void CC_MainGet_SwimmingInfo(eSWIM_LEN_SET_t *_bPoolSize, uint32_t *_dwSwimLen);
extern uint8_t CC_MainGet_BatteryCapacity(void);
extern uint16_t CC_MainGet_BatteryAdcLevel(void);
extern uint8_t CC_MainGet_HrmData(void);
extern uint8_t CC_MainGet_HrmDataFlag(void);
extern void CC_MainSet_HrmData(uint8_t _bHrmData);
extern void CC_SetToTurnOffHrm(eHrm_Close_EventID _bID);
extern eDEV_CHARGE_STATE_t CC_GetChargeStatus(void);
extern eDEV_CHARGEFULL_STATE_t CC_GetChargeFullStatus(void);
//extern void CC_Check_and_Set_Swim_Mode(void);
//extern eSystem_Battery_State_t CC_GetBatteryPwrState(void);
extern void CC_SwimOffUnlockHrm(void);
//extern uint8_t CC_Battery_GetPreLowPower(void);
extern uint8_t CC_BLE_Cmd_GetSwimmingEN(void);

typedef struct
{
    ePageMgrStep _ePageStep; 
    eMMI_Page_t eNowPage;
    eMMI_Page_t ePrePage;
    uint8_t _cIs_UpdatePage;
    
}s_page;

s_page s_tPage;

uint8_t _cIs_OLED_Blocking;
 uint8_t _cIs_OLED_Active;
 uint8_t _cIs_OLED_Draw_Page;
 uint8_t _cIs_OLED_Draw_Page_Data;
static uint8_t g_bIs_Notification_beResume = false;

static uint8_t _PageMgr_HRSPage_Resume(void)
{
    if (true == g_bIs_Notification_beResume)
    {
        CC_SwimOffUnlockHrm();
        g_bIs_Notification_beResume = false;

        
        if (true == CC_AppSrv_HR_IsHrsEnabled())
        {
            s_tPage.eNowPage = eMMI_HRM_HRS_ACTIVATED_PAGE;
            s_tPage.ePrePage = eMMI_HRM_HRS_ACTIVATED_PAGE;
        }
        else
        {
            s_tPage.eNowPage = eMMI_STANDBY_PAGE;
            s_tPage.ePrePage = eMMI_STANDBY_PAGE;
        }
        return (true);
     }

    return (false);

}
  
static bool _PageMgr_Srv_WakeUpOLED(void)
{
    _cIs_OLED_Blocking = true;
    oled_SetPwrCtrlHigh();
    CC_VENUS_OLEDWakUpTimeOutTimerStart(GENERALTIME_TURN_ON_OFF_OLED);
    return false;
}

static bool _PageMgr_Srv_SleepOLED(void)
{
    _cIs_OLED_Blocking = true;
    oled_Sleep();
    return false;
}

static void _PageMgr_Srv_Init(void)
{
    oled_DisplayOn();    
    CC_Dsp_Srv_Init();
    _cIs_OLED_Blocking = false;
    _cIs_OLED_Active = true;

}

void _PageMgr_EventHandle(S_VenusEvent *_stEvent)
{
    s_tPage._ePageStep=eStep_MainPage;
}

void _PageMgr_DrawPath(eMMI_Page_t _PageIndex)
{
    switch (_PageIndex)
    {
        case eMMI_STANDBY_PAGE:
            break;
        case eMMI_CLOCK_PAGE:
        {    
            app_date_time_t _stCurTime;
            #ifdef RD_DEBUG
                uint8_t _bTimeFormat =0;
            #else
            uint8_t _bTimeFormat =1;
            #endif
            CC_MainGet_CurrentTime(&_stCurTime);
            CC_Dsp_Srv_PresentTime(_stCurTime, _bTimeFormat);  
        }    
            break;
        case eMMI_PEDO_PAGE:
        case eMMI_HRM_PAGE:
        case eMMI_HRM_HRS_INIT_PAGE:
        case eMMI_HRM_HRS_ACTIVATED_PAGE:
        case eMMI_BATTERY_PAGE:
        case eMMI_DISTANCE_PAGE:
        case eMMI_CAL_PAGE:
        case eMMI_SWINMING_PAGE:
                CC_Dsp_Draw_Icon(_PageIndex);
            break; 

        case eMMI_DEVICE_INFO_PAGE:             
                CC_Dsp_Srv_DeviceInfo();
            break;

        case eMMI_CHARGING_IN_PAGE:
                CC_Dsp_Draw_Icon(_PageIndex);
            break;
        case eMMI_CHARGING_IN_FULL:
                CC_Dsp_Draw_Icon(_PageIndex);          
                CC_Vib_Srv_Icon(_PageIndex);              
            break;

        case eMMI_HRM_HRS_DATA_PAGE:
        case eMMI_HRMDATA_PAGE:
             {    
                 uint16_t _wHrmData = CC_MainGet_HrmData();

                 CC_Dsp_Srv_HrmData(_wHrmData);
                 if (eMMI_HRM_HRS_DATA_PAGE != _PageIndex)
                     CC_Vib_Srv_Icon(_PageIndex);                  
             }    
             break;

        case eMMI_HRM_HRS_DEACTIVATED_PAGE:
             CC_Dsp_Draw_Icon(_PageIndex);
             break;

        case eMMI_SWIMMING_ON_PAGE:
        {
            CC_Dsp_Srv_SwimmingStatus(1);
            //CC_Vib_Srv_Icon(_PageIndex);
        }
            break;
        case eMMI_SWIMMING_OFF_PAGE:
        {
            CC_Dsp_Srv_SwimmingStatus(0);
            //CC_Vib_Srv_Icon(_PageIndex);
        }
            break;
        case eMMI_SWIMMING_CONFIRM_ON_PAGE:
        {
            CC_Dsp_Srv_SwimmingConfirm(1);
        }
            break;        
        case eMMI_SWIMMING_CONFIRM_OFF_PAGE:
        {
            CC_Dsp_Srv_SwimmingConfirm(0);
        
            break;
        }
        case eMMI_HRMTIMEOUT_PAGE:
        case eMMI_LONGSIT_PAGE:
        case eMMI_INCOMMING_CALL_PAGE:
        case eMMI_INCOMMING_SMS_PAGE:
        case eMMI_ALARM_PAGE:
        {
            CC_Dsp_Draw_Icon(_PageIndex);     
            CC_Vib_Srv_Icon(_PageIndex);      
        }    
            break;
        case eMMI_PRE_LOWPOWER:
        {
            CC_Dsp_Draw_Icon(_PageIndex);    
        }    
            break;
           
        case  eMMI_LOWPOWER:
        {
            CC_Dsp_Draw_Icon(_PageIndex);           
            CC_Vib_Srv_Icon(_PageIndex);         
        }  
        break;

        case eMMI_DB_FULL:
        {
#ifdef DB_EN            
            CC_Dsp_Set_DB_Full();
#endif
        }
        break;
        case eMMI_DUMMY_PAGE:
        case eMMI_DUMMY_END:    
        default:
            break;
    }

}

void _PageMgr_DrawPathData(eMMI_Page_t _PageIndex)
{
    switch (_PageIndex)
    {
        case eMMI_STANDBY_PAGE:
            break;
        case eMMI_CLOCK_PAGE:
            break;
        case eMMI_PEDO_PAGE:
        {
            uint32_t _dwData;
            _dwData =CC_MainGet_PedCnt();                 
            CC_Dsp_Srv_PedCnt(_dwData);   
        }    
            break;
        case eMMI_HRM_PAGE:
            break;
        case eMMI_BATTERY_PAGE:
        {
            uint8_t _bBatCap = CC_MainGet_BatteryCapacity();
            CC_Dsp_Srv_BatteryLife(_bBatCap);  
            //uint16_t _wAdcLevel = CC_MainGet_BatteryAdcLevel();
            //CC_Dsp_Srv_BatteryLevel(_wAdcLevel);
        }    
            break;
        case eMMI_DISTANCE_PAGE:
        {    
            uint32_t _dwData = CC_MainGet_PedCnt();
            uint8_t _bStrideLen = 0;
            uint8_t _bUnitSetting = 0;
            CC_MainGet_Stride_LengthSetting(&_bStrideLen,&_bUnitSetting);					
            CC_Dsp_Srv_PedDistance(_dwData,_bStrideLen,_bUnitSetting);
        }    
            break;
        case eMMI_CAL_PAGE:
        {    
            uint32_t _dwCalo = CC_MainGet_Calorie();
     
            CC_Dsp_Srv_PedCalorie(_dwCalo);
        }    
            break;
        case eMMI_SWINMING_PAGE:
        {    
            eSWIM_LEN_SET_t _ePoolSize;
            uint32_t dwSwimLen;
            CC_MainGet_SwimmingInfo(&_ePoolSize,&dwSwimLen);
            CC_Dsp_Srv_SwimgDistance(_ePoolSize,dwSwimLen);
        }  
            break; 
        case eMMI_DEVICE_INFO_PAGE:  
            break;
        case eMMI_HRMDATA_PAGE:
        case eMMI_DUMMY_PAGE:
        case eMMI_CHARGING_IN_PAGE:
        case eMMI_HRMTIMEOUT_PAGE:
        case eMMI_LONGSIT_PAGE:
        case eMMI_INCOMMING_CALL_PAGE: 
        case eMMI_INCOMMING_SMS_PAGE:
        case eMMI_ALARM_PAGE:
        case eMMI_PRE_LOWPOWER:    
        case eMMI_LOWPOWER:
        case eMMI_DUMMY_END:
        default:
            break;
    }


}
static void _PageMgr_ResetGeneralTimer(void)
{
    CC_VENUS_OLEDGeneralOutTimerStop();
    CC_VENUS_OLEDGeneralOutTimerReset();
}

static void _PageMgr_PageDestroy(eMMI_Page_t _Page, S_VenusEvent *_stEvent)
{
    //CC_Vib_Srv_Reset(); // remarked for test 
    CC_Dsp_Srv_Set_Ind_Reset();
    _PageMgr_ResetGeneralTimer();
    
    if (eEvent_HRM_DATA != _stEvent->dwData)
    {
        if ((eMMI_HRM_PAGE == _Page) || (eMMI_HRMDATA_PAGE == _Page))
            CC_AppSrv_HR_StopSingleHR();
    }
    
    if (_Page == eMMI_DEVICE_INFO_PAGE)
        s_tPage.eNowPage = eMMI_STANDBY_PAGE;

    if (eMMI_SWIMMING_OFF_PAGE == _Page)
        CC_SwimOffUnlockHrm();
}

static void _PageMgr_PageSwitch(S_VenusEvent *_stEvent)
{
    TracerInfo("_PageMgr_PageSwitch  %d %d\r\n",_stEvent->bCount,_stEvent->dwData);

    switch (_stEvent->dwData)
    {
        case eEvent_LOWPOWER:
            if (eDEVICE_CHARGE_IN ==CC_GetChargeStatus())
            {   
                if ( eDEVICE_CHARGE_FULL==CC_GetChargeFullStatus())
                    s_tPage.eNowPage = eMMI_CHARGING_IN_FULL; 
                else
                    s_tPage.eNowPage = eMMI_CHARGING_IN_PAGE;
            }
            else
                s_tPage.eNowPage= eMMI_LOWPOWER; 
            break;
        case eEvent_TOUCH:

            if (eMMI_CHARGING_IN_PAGE == s_tPage.ePrePage)
                s_tPage.eNowPage = eMMI_CHARGING_IN_PAGE;
            else if (eMMI_CHARGING_IN_FULL== s_tPage.ePrePage)
                s_tPage.eNowPage = eMMI_CHARGING_IN_FULL;
            else if (eMMI_HRMDATA_PAGE == s_tPage.ePrePage)
                s_tPage.eNowPage = eMMI_BATTERY_PAGE;
            else if (eMMI_HRMTIMEOUT_PAGE == s_tPage.ePrePage)
                s_tPage.eNowPage = eMMI_BATTERY_PAGE;
            else if (eMMI_LONGSIT_PAGE== s_tPage.ePrePage)
                if (true==CC_AppSrv_HR_IsHrsEnabled())
                {
                    if (false == _PageMgr_HRSPage_Resume())
                    {
                        s_tPage.eNowPage = eMMI_STANDBY_PAGE;
                        s_tPage.ePrePage = eMMI_STANDBY_PAGE; 
                    } 
                }
                else 
                    s_tPage.eNowPage = eMMI_CLOCK_PAGE;
            else if (eMMI_INCOMMING_CALL_PAGE == s_tPage.ePrePage)
                if (true==CC_AppSrv_HR_IsHrsEnabled())
                {
                    if (false == _PageMgr_HRSPage_Resume())
                    {
                        s_tPage.eNowPage = eMMI_STANDBY_PAGE;
                        s_tPage.ePrePage = eMMI_STANDBY_PAGE; 
                    } 
                }
                else 
                s_tPage.eNowPage = eMMI_CLOCK_PAGE;
            else if (eMMI_INCOMMING_SMS_PAGE == s_tPage.ePrePage)
                if (true==CC_AppSrv_HR_IsHrsEnabled())
                {
                    if (false == _PageMgr_HRSPage_Resume())
                    {
                        s_tPage.eNowPage = eMMI_STANDBY_PAGE;
                        s_tPage.ePrePage = eMMI_STANDBY_PAGE; 
                    } 
                }
                else 
                s_tPage.eNowPage = eMMI_CLOCK_PAGE;
            else if (eMMI_ALARM_PAGE == s_tPage.ePrePage)
                if (true==CC_AppSrv_HR_IsHrsEnabled())
                {
                    if (false == _PageMgr_HRSPage_Resume())
                    {
                        s_tPage.eNowPage = eMMI_STANDBY_PAGE;
                        s_tPage.ePrePage = eMMI_STANDBY_PAGE; 
                    } 
                }
                else 
                s_tPage.eNowPage = eMMI_CLOCK_PAGE;            
            else if (eMMI_SWIMMING_ON_PAGE == s_tPage.ePrePage) 
                s_tPage.eNowPage = eMMI_CLOCK_PAGE; 
            else if (eMMI_SWIMMING_OFF_PAGE == s_tPage.ePrePage)
                s_tPage.eNowPage = eMMI_CLOCK_PAGE; 
            else if (eMMI_SWIMMING_CONFIRM_ON_PAGE == s_tPage.ePrePage)
                s_tPage.eNowPage = eMMI_SWIMMING_CONFIRM_OFF_PAGE; 
            else if (eMMI_SWIMMING_CONFIRM_OFF_PAGE == s_tPage.ePrePage)
                s_tPage.eNowPage = eMMI_SWIMMING_CONFIRM_ON_PAGE;             
            else  if (eMMI_PRE_LOWPOWER== s_tPage.ePrePage)
            {//START:[VNS-97]
                if((eMMI_CLOCK_PAGE != s_tPage.eNowPage) && (eMMI_DB_FULL != s_tPage.ePrePage)
#ifdef DB_EN                    
                    && (0x01 == CC_DB_Check_DB_Full()))
#else
                )
#endif
                {
                    s_tPage.eNowPage = eMMI_DB_FULL;
                }
                else
                {
                    s_tPage.eNowPage = eMMI_CLOCK_PAGE; 
                }
            }//END:[VNS-97]
            else if (eMMI_DB_FULL == s_tPage.ePrePage)
                s_tPage.eNowPage = eMMI_CLOCK_PAGE; 
            else
            {
                if (eDEVICE_CHARGE_IN ==CC_GetChargeStatus())
                {   
                    if ( eDEVICE_CHARGE_FULL==CC_GetChargeFullStatus())
                        s_tPage.eNowPage = eMMI_CHARGING_IN_FULL; 
                    else
                        s_tPage.eNowPage = eMMI_CHARGING_IN_PAGE;
                }    
                else if ((eMMI_HRM_HRS_INIT_PAGE == s_tPage.eNowPage) ||(eMMI_HRM_HRS_ACTIVATED_PAGE == s_tPage.eNowPage) || (eMMI_HRM_HRS_DATA_PAGE == s_tPage.eNowPage))
                    break;
                else if (eMMI_HRM_HRS_DEACTIVATED_PAGE == s_tPage.eNowPage)
                    s_tPage.eNowPage = eMMI_CLOCK_PAGE;
                else    
                {   
                    if ( eSysStateLowPwr == CC_GetBatteryPwrState())
                        s_tPage.eNowPage = eMMI_LOWPOWER;
                    else
                    {
                        if ((true == CC_Battery_GetPreLowPower() && 
                            (eMMI_PRE_LOWPOWER != s_tPage.ePrePage)&&
                            eMMI_STANDBY_PAGE == s_tPage.eNowPage))
                            s_tPage.eNowPage = eMMI_PRE_LOWPOWER;
                        else if ((eMMI_STANDBY_PAGE == s_tPage.eNowPage) && 
                            (eMMI_DB_FULL != s_tPage.ePrePage)  
#ifdef DB_EN                            
                            && (0x01 == CC_DB_Check_DB_Full()))
#else
                            )
#endif                            
                            s_tPage.eNowPage = eMMI_DB_FULL;
                        else if ((eMMI_STANDBY_PAGE == s_tPage.eNowPage) && 
                            (eEvent_SWIM_ON != s_tPage.ePrePage) && 
                            (0x01 == CC_BLE_Cmd_GetSwimmingEN()))
                            s_tPage.eNowPage = eMMI_SWIMMING_ON_PAGE;
                        else
                        {
                                s_tPage.eNowPage++;

                            if(1 == CC_BLE_Cmd_GetSwimmingEN())
                            {
                                // skip:
                                while(1)
                                {
                                    if((eMMI_PEDO_PAGE == s_tPage.eNowPage)
                                     || (eMMI_HRM_PAGE == s_tPage.eNowPage)
                                     || (eMMI_DISTANCE_PAGE == s_tPage.eNowPage)
                                     || (eMMI_CAL_PAGE == s_tPage.eNowPage))
                                        s_tPage.eNowPage++;
                                    else
                                        break;
                                }
                            }
                        }

                        //if(eMMI_HRM_PAGE == s_tPage.eNowPage)
                          //  CC_SetHrmStatus(eHRM_On);
                    }    
                }    
            }
            break;

        case eEvent_SWIM_ON:
                s_tPage.eNowPage= eMMI_SWIMMING_ON_PAGE; 
            break;
        case eEvent_SWIM_OFF:
                s_tPage.eNowPage= eMMI_SWIMMING_OFF_PAGE;                 
            break;
        case eEvent_SWIM_CONFIRM_ON:
                s_tPage.eNowPage= eMMI_SWIMMING_CONFIRM_ON_PAGE;                
            break;
        case eEvent_SWIM_CONFIRM_OFF:
                s_tPage.eNowPage= eMMI_SWIMMING_CONFIRM_OFF_PAGE;                            
            break;

        case eEvent_HEARTRATESTRAPMODE_ON:
             s_tPage.eNowPage = eMMI_HRM_HRS_INIT_PAGE; 
             break;

        case eEvent_HEARTRATESTRAPMODE_OFF:
             s_tPage.eNowPage = eMMI_HRM_HRS_DEACTIVATED_PAGE;
             break;

        case eEvent_CHARGINGIN:
                s_tPage.eNowPage= eMMI_CHARGING_IN_PAGE;
            break;
        case eEvent_CHARGINGOUT:
                //START:[VNS-97]
                if (true == CC_Battery_GetPreLowPower())
                    s_tPage.eNowPage= eMMI_PRE_LOWPOWER; 
                else
                {  
#ifdef DB_EN                    
                    if (0x01 == CC_DB_Check_DB_Full())
                        s_tPage.eNowPage= eMMI_DB_FULL; 
                    else 
#endif                        
                        s_tPage.eNowPage= eMMI_CLOCK_PAGE;
                }//End    
            break;
        case eEvent_CHARGINGFULL:
                s_tPage.eNowPage= eMMI_CHARGING_IN_FULL;
            break;
        case eEvent_HRM_DATA:
            if ((eMMI_HRM_HRS_ACTIVATED_PAGE == s_tPage.eNowPage) || (eMMI_HRM_HRS_DATA_PAGE == s_tPage.eNowPage))
                s_tPage.eNowPage = eMMI_HRM_HRS_DATA_PAGE;
            else if (eMMI_HRM_HRS_DEACTIVATED_PAGE == s_tPage.eNowPage)
                break;
            else
                if ( true==CC_AppSrv_HR_IsHrsEnabled())
                s_tPage.eNowPage = eMMI_HRM_HRS_DATA_PAGE; // workaround for alarm incomming and touch to disable it,
                    else                                                                    //  set page to hrs)
                s_tPage.eNowPage= eMMI_HRMDATA_PAGE;
            break;
        case eEvent_HRM_TIMEOUT:
                s_tPage.eNowPage= eMMI_HRMTIMEOUT_PAGE;
            break;
        case eEvent_LIFTARM_UP:
                s_tPage.eNowPage= eMMI_CLOCK_PAGE;
            break;
        case eEvent_LIFTARM_DOWN:
                s_tPage.eNowPage= eMMI_LIFTARM_DOWN_OLEDOFF;
            break;
        case eEvent_LONGSIT:
                s_tPage.eNowPage= eMMI_LONGSIT_PAGE;
            break;
        case eEvent_INCOMMINGCALL_ON:
                s_tPage.eNowPage= eMMI_INCOMMING_CALL_PAGE;
            break;
        case eEvent_INCOMMINGCALL_OFF:
                if (true==CC_AppSrv_HR_IsHrsEnabled())
                {
                    if (false == _PageMgr_HRSPage_Resume())
                    {
                        s_tPage.eNowPage = eMMI_STANDBY_PAGE;
                        s_tPage.ePrePage = eMMI_STANDBY_PAGE; 
                    } 
                }
                else 
                s_tPage.eNowPage= eMMI_CLOCK_PAGE;
            break;   
        case eEvent_INCOMMINGSMS:
                s_tPage.eNowPage= eMMI_INCOMMING_SMS_PAGE;
            break;   
        case eEvent_ALARM:
                s_tPage.eNowPage= eMMI_ALARM_PAGE;
            break;
        case eEvent_None:
        case eEvent_Dummy:
        default:
            // no todo
            break;
    }
    
    if (s_tPage.eNowPage != s_tPage.ePrePage)
        s_tPage.ePrePage = s_tPage.eNowPage;    
}

static void _PageMgr_PageDraw(void)
{
    _PageMgr_DrawPath(s_tPage.eNowPage);
}

static void _PageMgr_PageRelatedAct(void)
{
    switch (s_tPage.eNowPage)
    {
    case eMMI_HRM_PAGE:
         CC_AppSrv_HR_StartSingleHR();
         break;

    case eMMI_HRMDATA_PAGE:
         CC_AppSrv_HR_StopSingleHR();
         break;

    case eMMI_HRM_HRS_INIT_PAGE:
         CC_AppSrv_HR_StartHRS();

         s_tPage.eNowPage = eMMI_HRM_HRS_ACTIVATED_PAGE;
         s_tPage.ePrePage = eMMI_HRM_HRS_ACTIVATED_PAGE;
         break;

    case eMMI_HRM_HRS_DEACTIVATED_PAGE:
         CC_AppSrv_HR_StopHRS();
         break;

    case eMMI_CHARGING_IN_PAGE:
    case eMMI_LOWPOWER:
         CC_AppSrv_HR_StopSingleHR();
         CC_AppSrv_HR_StopHRS();
         CC_AppSrv_HR_Stop24HR();
         break;
    }

    if (eMMI_LIFTARM_DOWN_OLEDOFF == s_tPage.eNowPage)
        CC_VENUS_OLEDGeneralOutTimerStart(GENERALTIME_TURN_OFF_OLED_NOW);
    else
        CC_VENUS_OLEDGeneralOutTimerStart(GENERALTIME_PAGESWITCH);             
}

static void _PageMgr_UpdateEvent(void)
{
       s_tPage._cIs_UpdatePage = true;
       s_tPage._ePageStep = eStep_MainPage;
}

eMMI_Page_t _PageMgr_GetPrePage(void)
{
    return s_tPage.ePrePage;
}


void CC_PageMgr_Init(void)
{
    _cIs_OLED_Blocking = false;
    _cIs_OLED_Active = false;
    _cIs_OLED_Draw_Page = false;
    s_tPage._ePageStep = eStep_MainPage;
    s_tPage.eNowPage = eMMI_STANDBY_PAGE;
    s_tPage.ePrePage = eMMI_STANDBY_PAGE;
    s_tPage._cIs_UpdatePage = false;
    _PageMgr_Srv_Init();

     s_tPage._ePageStep = eStep_CloseOLED; // test by Samuel for close "VENUS" title
}

bool _PageMgr_IgnoreEvent(uint32_t dwEventCode)
{    

    if(eEvent_None == dwEventCode)
        return false;

    if ( eSysStateLowPwr == CC_GetBatteryPwrState())
    {
        
        if((eEvent_TOUCH== dwEventCode) || 
            (eEvent_LOWPOWER == dwEventCode)||
           (eEvent_CHARGINGIN== dwEventCode) ||
        //Jason, add
           (eEvent_HR_WARNING_HIHG== dwEventCode) ||
           (eEvent_HR_WARNING_LOW== dwEventCode) ||
           (eEvent_FACTORY_RESET_START== dwEventCode) ||
           (eEvent_FACTORY_RESET_DONE== dwEventCode) ||
           (eEvent_FACTORY_RESET_STOP == dwEventCode)||
           (eEvent_LIFTARM_DOWN== dwEventCode))
            return (false);
        else
            return (true);
    }

   
    // OLED is active, so check these event to ignore page.
    switch (s_tPage.eNowPage)
    {
    case eMMI_CHARGING_IN_PAGE:
    case eMMI_CHARGING_IN_FULL:
         if (   (eEvent_TOUCH                 != dwEventCode)
             && (eEvent_HEARTRATESTRAPMODE_ON != dwEventCode)
             && (eEvent_SWIM_ON               != dwEventCode)
             && (eEvent_CHARGINGIN            != dwEventCode)
             && (eEvent_CHARGINGOUT           != dwEventCode)
             && (eEvent_CHARGINGFULL        != dwEventCode)
             && (eEvent_ALARM                   != dwEventCode)
             && (eEvent_PAIR_PASSKEY          != dwEventCode)
             && (eEvent_PAIR_SUCCESS          != dwEventCode)
             && (eEvent_PAIR_FAIL             != dwEventCode)
             && (eEvent_FACTORY_RESET_START!= dwEventCode)
             &&(eEvent_FACTORY_RESET_DONE!= dwEventCode)
             &&(eEvent_FACTORY_RESET_STOP != dwEventCode)) // Jason, [VNS-91] define muilt event on Charging in or Charging Full
         {
             return (true);
         }
         break;

    case eMMI_HRM_HRS_INIT_PAGE:
         if (eEvent_HEARTRATESTRAPMODE_ON == dwEventCode) 
            return true;        
    case eMMI_HRM_HRS_ACTIVATED_PAGE:
    case eMMI_HRM_HRS_DATA_PAGE:
         if (   (eEvent_TOUCH            == dwEventCode) 
             || (eEvent_SWIM_CONFIRM_OFF == dwEventCode)
             || (eEvent_LIFTARM_DOWN == dwEventCode))
             return (true);

         if ((eEvent_PAIR_PASSKEY== dwEventCode) || 
             (eEvent_PAIR_SUCCESS== dwEventCode) ||
             (eEvent_PAIR_FAIL== dwEventCode))         
             return (false);

         if ((eEvent_ALARM == dwEventCode) || 
            (eEvent_INCOMMINGCALL_ON == dwEventCode) ||
            (eEvent_INCOMMINGSMS == dwEventCode)||
            (eEvent_LONGSIT== dwEventCode)||
#ifdef PED_GOAL
            (eEvent_PED_GOAL_ACHIEVE== dwEventCode)||
            (eEvent_SWIMMING_GOAL_ACHIEVE== dwEventCode)||
#endif
            //Jason, add
            (eEvent_HR_WARNING_HIHG== dwEventCode)||
            (eEvent_HR_WARNING_LOW== dwEventCode))
         {
            TracerInfo("_PageMgr_IgnoreEvent Handle Notification Event\r\n");
            g_bIs_Notification_beResume = true;
            return (false);
         }
         break;
         
     case eMMI_ALARM_PAGE:
     case eMMI_INCOMMING_CALL_PAGE:
     case eMMI_INCOMMING_SMS_PAGE:
          if (eEvent_HRM_DATA== dwEventCode) 
             return (true);
            break;
        //Jason, add
        case eMMI_HR_WARNING_HIGH:
        case eMMI_HR_WARNING_LOW:
            if ((eEvent_TOUCH != dwEventCode)
             && (eEvent_CHARGINGIN != dwEventCode)
             && (eEvent_CHARGINGOUT != dwEventCode)
             && (eEvent_CHARGINGFULL != dwEventCode)
            && (eEvent_HEARTRATESTRAPMODE_OFF != dwEventCode)
            && (eEvent_FACTORY_RESET_START!= dwEventCode)
            && (eEvent_FACTORY_RESET_STOP!= dwEventCode)
            && (eEvent_FACTORY_RESET_DONE!= dwEventCode)
            && (eEvent_PAIR_PASSKEY!= dwEventCode)
            && (eEvent_PAIR_SUCCESS!= dwEventCode)
            && (eEvent_PAIR_FAIL!= dwEventCode))
                return (true);
         break;
     case eMMI_PAIR_PASSKEY:
            if((eEvent_TOUCH == dwEventCode)
              || (eEvent_LOWPOWER == dwEventCode)
              || (eEvent_SWIM_ON == dwEventCode)
              || (eEvent_SWIM_OFF == dwEventCode)
              || (eEvent_CHARGINGIN == dwEventCode)
              || (eEvent_CHARGINGOUT== dwEventCode)
              || (eEvent_CHARGINGFULL== dwEventCode)
              || (eEvent_HRM_DATA == dwEventCode)
              || (eEvent_HRM_TIMEOUT== dwEventCode)
              || (eEvent_HEARTRATESTRAPMODE_ON== dwEventCode)
              || (eEvent_HEARTRATESTRAPMODE_OFF== dwEventCode)
              || (eEvent_LIFTARM_UP== dwEventCode)
              || (eEvent_LONGSIT== dwEventCode)
              || (eEvent_INCOMMINGCALL_ON== dwEventCode)
              || (eEvent_INCOMMINGCALL_OFF== dwEventCode)
              || (eEvent_INCOMMINGSMS== dwEventCode)
              || (eEvent_ALARM== dwEventCode)
              || (eEvent_HR_WARNING_HIHG== dwEventCode)
              || (eEvent_HR_WARNING_LOW== dwEventCode)
#ifdef PED_GOAL
              || (eEvent_PED_GOAL_ACHIEVE== dwEventCode)
              || (eEvent_SWIMMING_GOAL_ACHIEVE== dwEventCode)
#endif
              || (eEvent_FACTORY_RESET_START == dwEventCode)
              || (eEvent_FACTORY_RESET_DONE == dwEventCode)
              || (eEvent_FACTORY_RESET_STOP == dwEventCode)
              || (eEvent_LIFTARM_DOWN== dwEventCode))

                return (true);
         break;
      case eMMI_FACTORY_RESET_DONE:
      case eMMI_FACTORY_RESET_STOP:
      {
        if( (eEvent_TOUCH == dwEventCode)
            ||(eEvent_LIFTARM_DOWN== dwEventCode))
            return (true);
        break;
      }
    }

    return (false);

}

void CC_PageMgr_Proc(S_VenusEvent *_stEvent)
{
    TracerInfo("CC_PageMgr_Proc \r\n");
    //TracerInfo("CC_PageMgr_Proc  %d %d\r\n",_stEvent->bCount,_stEvent->dwData);

    if (true == _PageMgr_IgnoreEvent(_stEvent->dwData))
        return;

    if (_stEvent->dwData != eEvent_None)
        _PageMgr_UpdateEvent();
  
    switch (s_tPage._ePageStep)
    {
    case eStep_MainPage:
         {
             if ( true==s_tPage._cIs_UpdatePage )
             {
                 TracerInfo("CC_PageMgr_Proc Draw Icon 1 \r\n");     
                 _PageMgr_PageDestroy(s_tPage.ePrePage,_stEvent);
                 
                 _PageMgr_PageSwitch(_stEvent);
                 
                 _PageMgr_PageDraw();
                 
                 _PageMgr_PageRelatedAct();
                 s_tPage._ePageStep=eStep_SubPage;
                 s_tPage._cIs_UpdatePage = false;
             }    
         }    
         break;

    case eStep_SubPage:
         {
             TracerInfo("CC_PageMgr_Proc Draw Icon Data 2\r\n"); 
             _PageMgr_DrawPathData(s_tPage.eNowPage);
             
             CC_VENUS_OLEDGeneralOutTimerStop();
             CC_VENUS_OLEDGeneralOutTimerReset();
             
             if ((eMMI_HRM_HRS_ACTIVATED_PAGE == s_tPage.eNowPage) || (eMMI_HRM_HRS_DATA_PAGE == s_tPage.eNowPage))
                 break;
             
             if ( eMMI_HRM_PAGE==s_tPage.eNowPage)
                 CC_VENUS_OLEDGeneralOutTimerStart(GENERALTIME_HRMPROG);
             else if (eMMI_INCOMMING_CALL_PAGE==s_tPage.eNowPage)
                 CC_VENUS_OLEDGeneralOutTimerStart(GENERALTIME_INCOMMING);
             else if (eMMI_LIFTARM_DOWN_OLEDOFF==s_tPage.eNowPage)
                 CC_VENUS_OLEDGeneralOutTimerStart(GENERALTIME_TURN_OFF_OLED_NOW);             
             else
                 CC_VENUS_OLEDGeneralOutTimerStart(GENERALTIME_PAGEIDLE);

             s_tPage._ePageStep = eStep_CloseOLED;
         }
         break;

    case eStep_CloseOLED:
         {     
#ifdef HRM_EN             
              if (eMMI_HRM_PAGE == s_tPage.ePrePage && CC_GetHrmStatus())
                  CC_SetToTurnOffHrm(eHRMCLOSE_ID_GENERALTIMEOUT);  // hrm timeout set event hrm close page.
              else
#else
              if(1)
#endif                  
              {
                  TracerInfo("CC_PageMgr_Proc Draw Icon Data 3\r\n"); 
                  
                  CC_Check_and_Set_Swim_Mode();
                  
                  _PageMgr_PageDestroy(s_tPage.ePrePage,_stEvent);
                  
                  if (false ==_PageMgr_Srv_SleepOLED())
                        CC_VENUS_OLEDSleepTimeOutTimerStart(GENERALTIME_TURN_ON_OFF_OLED);   
                        
                  s_tPage._ePageStep=eStep_MainPage;
              }   
         }    
         break;    

    case eStep_DummyStep:
    default:
         s_tPage._ePageStep = eStep_MainPage;
         break;
    }    // END of switch (s_tPage._ePageStep)
}

void CC_PageMgr_ExecWakeup(void)
{
    oled_WakeUp();
    _cIs_OLED_Blocking = false;
    _cIs_OLED_Active = true;
    oled_DrawBlack();
}
void CC_PageMgr_ExecSleep(void)
{
    oled_DrawBlack();
    oled_SetPwrCtrlLow();
    _cIs_OLED_Blocking = false;
    _cIs_OLED_Active = false;
        
    if (false == _PageMgr_HRSPage_Resume())
    {
        s_tPage.eNowPage = eMMI_STANDBY_PAGE;
        s_tPage.ePrePage = eMMI_STANDBY_PAGE;    
    }
}

uint8_t CC_PageMgr_ActiveOLED(void)
{   
    bool _state = true;
    
    if(false == _cIs_OLED_Active)
    {
        _state = _PageMgr_Srv_WakeUpOLED();
    }
    
    return _state;
}

uint8_t CC_PageMgr_IsBlockingOLED(void)
{
    return _cIs_OLED_Blocking;
}

uint8_t CC_PageMgr_IsOLEDActive(void)
{
    return _cIs_OLED_Active;
}

