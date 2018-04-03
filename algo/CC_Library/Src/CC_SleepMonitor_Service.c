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

#ifdef SLEEP_EN
#include "CC_SleepMonitor_Service.h"
#include "CC_Longsit_Service.h"
#include "CC_Slpmtr.h"
#include "htpt_task.h"
#include "tracer.h"



extern uint8_t CC_Charge_Register(charge_cb_t cb);
extern void _sensor_algorithm_sleepmeter_proc(void);
extern void CC_MainGet_CurrentTime(app_date_time_t *_stCurTime);
extern eDEV_CHARGE_STATE_t CC_GetChargeStatus(void);
extern void CC_ResetSleep_StateCount(void);
extern uint8_t CC_MainGet_SwimmingEn(void);
extern void CC_DB_Save_StartSleepService_Info(void);
extern void CC_DB_Save_EndSleepService_Info(void);


extern float g_fSleepCalSeconds;
extern float g_bSleepEnCnt;


eStete_t m_bIsSleepAlgActive = eDisable;
uint8_t m_bIsSleepAlwayon = false;
eStete_t m_bIsSleepStartService = eDisable;

static void _SleepMonitor_Charge_Evt_CB(eDEV_CHARGE_STATE_t eState)
{
    if (eDEVICE_CHARGE_IN == eState)
    {
        CC_SleepMonitor_Srv_Disable();
    }
    else if (eDEVICE_CHARGE_OUT== eState)
    {
        CC_SleepMonitor_Srv_Enable();
    }
    else
    {
        //no this case;
    }
}

void CC_SleepMonitor_Srv_Register(void)
{
      CC_Charge_Register(_SleepMonitor_Charge_Evt_CB);
}

eStete_t CC_SleepMonitor_GetSleepState(void)
{
    return m_bIsSleepAlgActive;
}

void CC_SleepMonitor_Srv_Enable(void)
{
    if ( false == m_bIsSleepStartService )
        return;
    
    if (eDEVICE_CHARGE_IN == CC_GetChargeStatus())
        return;

    if (true == CC_MainGet_SwimmingEn())
        return;

    if (!slpmtr_is_opened())
    {
        slpmtr_open_with_sensitivity(SLPMTR_SENS_LOW);
        m_bIsSleepAlgActive= eEnable;
        CC_ResetSleep_StateCount();
        CC_DB_Save_StartSleepService_Info();

        TracerInfo("CC_SleepMonitor_Srv_Enable Sleep Algo Open\r\n");
    } 
}

void CC_SleepMonitor_Srv_Disable(void)
{
    if (slpmtr_is_opened())
    {
        slpmtr_close();
        CC_DB_Save_EndSleepService_Info();
        m_bIsSleepAlgActive= eDisable;
        g_fSleepCalSeconds = 0;
        g_bSleepEnCnt=0;
        TracerInfo("CC_SleepMonitor_Srv_Disable Sleep Algo Close \r\n");
    }        
}

eStete_t CC_SleepMonitor_Srv_Get_SleepService(void)
{
    //return m_bIsSleepStartService;
    return m_bIsSleepAlgActive;
}

void CC_SleepMonitor_Srv_Enable_SleepService(eStete_t eStete)
{
    m_bIsSleepStartService = eStete;
    CC_SleepMonitor_Srv_Enable();
    }

void CC_SleepMonitor_Srv_Disable_SleepService(eStete_t eStete)
        {
    m_bIsSleepStartService = eStete;
}

void CC_SleepMonitor_Srv_Handle(void)
{

    if ( false == m_bIsSleepStartService)
        return;
    
    if ( false ==m_bIsSleepAlgActive )
        return;
        
    if  ( eDEVICE_CHARGE_IN == CC_GetChargeStatus() )
        return;
    
    if (true == CC_MainGet_SwimmingEn())
        return;

        _sensor_algorithm_sleepmeter_proc();
}

#endif

