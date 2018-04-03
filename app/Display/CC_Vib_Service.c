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

#include "project.h"
#include "drvi_pwm.h"
#include "cc_Vib_Service.h"
#include "drvi_pwm.h"



#define TIMER_START_TIMEOUT 10 
#define RESTART_TIMER_100    100
#define RESTART_TIMER_200    200
#define RESTART_TIMER_300    300
#define RESTART_TIMER_400    400
#define RESTART_TIMER_500    500

#define VIT_STOP                        0
#define VIB_ONETIME                  1
#define VIB_TWOTIME                 2
#define VIB_THREETIME               3
#define VIB_CONTINUES               4


extern void CC_VENUS_PWMVibrationServiceTimerStart(uint16_t _wdata);
extern void CC_VENUS_PWMVibrationServiceTimerReset(void);
extern void CC_VENUS_PWMVibrationServiceTimerStop(void);
extern void CC_VENUS_PWMVibrationServiceEventClear(void);

typedef struct 
{
    eMMI_Page_t _eVibInd;
    eCommon_State _bAction;
    uint8_t _bIndex;
    uint8_t _bPeriod;
    uint32_t _dwReStartTimerTime;
}S_TVibVal;


static S_TVibVal s_tVibVal;

void _CC_Vib_Action_CountingTime(void)
{
    //TracerInfo("_CC_Vib_Action_CountingTime %d \r\n", s_tVibVal._bPeriod);
    if ( eStateOn == s_tVibVal._bAction )
    {
        if ( s_tVibVal._bIndex == 0)
        {
            drvi_pwmStart(PWM_0);
            s_tVibVal._bPeriod--;
        }
        else 
        {
            drvi_pwmStop(PWM_0);
            if (s_tVibVal._bPeriod == VIT_STOP)
                CC_Vib_Srv_Reset();
        }    
        s_tVibVal._bIndex ^=1;
        CC_VENUS_PWMVibrationServiceTimerStart(s_tVibVal._dwReStartTimerTime);

    }

}


void _CC_Vib_Action_Continues(void)
{
    if ( eStateOn == s_tVibVal._bAction)
    {
        if ( s_tVibVal._bIndex == 0)
            drvi_pwmStart(PWM_0);
        else 
            drvi_pwmStop(PWM_0);
        
        s_tVibVal._bIndex ^=1;
        s_tVibVal._bPeriod= VIB_CONTINUES;
        CC_VENUS_PWMVibrationServiceTimerStart(s_tVibVal._dwReStartTimerTime);
    }
}

void _CC_Set_VibParameter(uint8_t _bTimerTime,eMMI_Page_t _PageIndex, uint8_t _bPeriodData, uint32_t _dwTime)
{
    CC_VENUS_PWMVibrationServiceTimerStart(_bTimerTime);
    s_tVibVal._eVibInd = _PageIndex;
    s_tVibVal._bIndex = 0;
    s_tVibVal._bAction = eStateOn;
    s_tVibVal._bPeriod = _bPeriodData;
    s_tVibVal._dwReStartTimerTime = _dwTime;
}



void CC_Vib_Srv_Reset(void)
{
    drvi_pwmStop(PWM_0);
    CC_VENUS_PWMVibrationServiceEventClear();
    CC_VENUS_PWMVibrationServiceTimerStop();
    CC_VENUS_PWMVibrationServiceTimerReset();
    s_tVibVal._eVibInd = eMMI_DUMMY_PAGE;
    s_tVibVal._bIndex = 0;
    s_tVibVal._bAction = eStateOff;
    s_tVibVal._dwReStartTimerTime =0;
}

void CC_Vib_Srv_Icon(eMMI_Page_t _PageIndex)
{
    switch (_PageIndex)
    {
        case eMMI_CHARGING_IN_FULL:
            _CC_Set_VibParameter(TIMER_START_TIMEOUT, _PageIndex, VIB_ONETIME,RESTART_TIMER_200);
            break;
        case eMMI_HRMDATA_PAGE:
            _CC_Set_VibParameter(TIMER_START_TIMEOUT, _PageIndex, VIB_ONETIME,RESTART_TIMER_100);
            break;
        case eMMI_HRMTIMEOUT_PAGE:
            _CC_Set_VibParameter(TIMER_START_TIMEOUT, _PageIndex, VIB_ONETIME,RESTART_TIMER_100);       
            break;
        case eMMI_LONGSIT_PAGE:
            _CC_Set_VibParameter(TIMER_START_TIMEOUT, _PageIndex, VIB_TWOTIME,RESTART_TIMER_200);
            break;
        case eMMI_INCOMMING_CALL_PAGE:
            _CC_Set_VibParameter(TIMER_START_TIMEOUT, _PageIndex, VIB_THREETIME,RESTART_TIMER_300);
            break;
        case eMMI_INCOMMING_SMS_PAGE:
            _CC_Set_VibParameter(TIMER_START_TIMEOUT, _PageIndex, VIB_THREETIME,RESTART_TIMER_300);
            break;          
        case eMMI_ALARM_PAGE:
            _CC_Set_VibParameter(TIMER_START_TIMEOUT, _PageIndex, VIB_CONTINUES,RESTART_TIMER_100);
            break;
        case eMMI_SWIMMING_ON_PAGE:
        case eMMI_SWIMMING_OFF_PAGE:
            _CC_Set_VibParameter(TIMER_START_TIMEOUT, _PageIndex, VIB_ONETIME,RESTART_TIMER_100);
            break;
        case eMMI_LOWPOWER:
            _CC_Set_VibParameter(TIMER_START_TIMEOUT, _PageIndex, VIB_THREETIME,RESTART_TIMER_100);
            break;
        case eMMI_DUMMY_PAGE:
        case eMMI_DUMMY_END:    
        default:
            break;
    }


}

void CC_Vib_Srv_Handler(void)
{
   // TracerInfo("CC_Vib_Srv_Handler \r\n");

    switch (s_tVibVal._eVibInd) 
    {
        case eMMI_CHARGING_IN_FULL:
        case eMMI_HRMDATA_PAGE:
        case eMMI_HRMTIMEOUT_PAGE:
        case eMMI_LONGSIT_PAGE:
        case eMMI_INCOMMING_CALL_PAGE:
        case eMMI_INCOMMING_SMS_PAGE:
        case eMMI_SWIMMING_ON_PAGE:
        case eMMI_SWIMMING_OFF_PAGE:
        case eMMI_ALARM_PAGE:
        case eMMI_LOWPOWER:
        if (s_tVibVal._bPeriod <=VIB_THREETIME )    
            _CC_Vib_Action_CountingTime();
        else
            _CC_Vib_Action_Continues();
            break;
        default:
            break;
    }

}

