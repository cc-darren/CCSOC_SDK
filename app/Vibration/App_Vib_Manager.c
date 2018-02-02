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
*  App_Vib_Manager.c
*
*  Project:
*  --------
*
*
*  Description:
*  ------------
*  Vibration Manager of APPLICATION
*
*  Author:
*  -------
*  CloudChip
*
*===========================================================================/
*  20180115 JASON initial version
******************************************************************************/

/******************************************************************************
 * INCLUDE FILE
 ******************************************************************************/
#ifdef CC_REFINE_VIB
#include <string.h>

#include "App_Vib_Manager.h"
#include "tracer.h"
#include "drvi_pwm.h"




/******************************************************************************
 * DEFINITION / CONSTANT / ENUM / TYPE
 ******************************************************************************/


/******************************************************************************
 * EXTERNAL FUNCTION
 ******************************************************************************/
extern void CC_VENUS_VIB_TIMER_OPT_Start(uint16_t _wdata);
extern void CC_VENUS_VIB_TIMER_OPT_Stop(void);
extern void CC_VENUS_VIB_TIMER_OPT_Clear(void);
extern void CC_VENUS_VIB_TIMER_EXECINTERVAL_Start(uint16_t _wdata);
extern void CC_VENUS_VIB_TIMER_EXECINTERVAL_Stop(void);
extern void CC_VENUS_VIB_TIMER_EXECINTERVAL_Clear(void);
extern void CC_VENUS_VIB_TIMER_DUTYCYCLECHANGE_Start(uint16_t _wdata);
extern void CC_VENUS_VIB_TIMER_DUTYCYCLECHANGE_Stop(void);
extern void CC_VENUS_VIB_TIMER_DUTYCYCLECHANGE_Clear(void);



/******************************************************************************
 *  VARIABLE
 ******************************************************************************/
S_APP_VIB_PATTERN_CFG s_tVibPara;
uint8_t g_bVibExecActive = false;
uint8_t g_bVibExecPatterRepeatCnt = 0;
uint8_t g_bVibExecPatterNum = 0;
int16_t g_bVibPwmDutyCycleLevel = VIB_VAR_LEVEL_LMT_MAX;

/******************************************************************************
 * LOCAL FUNCTION > _Vib_Mgr_Start
 ******************************************************************************/
static void APP_DRV_PWM_Start(uint16_t _wDutyCycle)
{

    drvi_pwmStop(PWM_0);
    drvi_pwmLoadPrescaler(PWM_0,100);
    drvi_pwmDuty(PWM_0,_wDutyCycle);
    drvi_pwmStart(PWM_0);

}
static void APP_DRV_PWM_Stop(void)
{
    drvi_pwmStop(PWM_0);

}

static void _Vib_Mgr_Start(void)
{

    APP_DRV_PWM_Stop();
    g_bVibExecActive = true;

    if (E_APP_VIB_OPT_NORMAL == s_tVibPara.eExecMode)
    {
        g_bVibPwmDutyCycleLevel = VIB_VAR_LEVEL_LMT_MAX;
    }
    else if ((E_APP_VIB_OPT_ASCENDING == s_tVibPara.eExecMode) || (E_APP_VIB_OPT_DESCENDING == s_tVibPara.eExecMode))
    {
        g_bVibPwmDutyCycleLevel = s_tVibPara.stVarAttr.bVar_StartLevel;
        CC_VENUS_VIB_TIMER_DUTYCYCLECHANGE_Start(s_tVibPara.stVarAttr.wVar_TimeInterval);
    }


    // set exectime on interval.
    CC_VENUS_VIB_TIMER_EXECINTERVAL_Start(s_tVibPara.stAttr.eVibPattern[g_bVibExecPatterNum].eVibOnOrOff.wVibON_ms);
    // enable OPT ms Timer
    CC_VENUS_VIB_TIMER_OPT_Start(s_tVibPara.wOpt_Ms);

    APP_DRV_PWM_Start(g_bVibPwmDutyCycleLevel);

    TracerInfo("_Vib_Mgr_Start \r\n");
    TracerInfo("g_bVibPwmDutyCycleLevel %d !!!!!!!!!!!!!!!!!!!!!!!!!!!!!1\r\n",g_bVibPwmDutyCycleLevel);
    TracerInfo("g_bVibExecActive = %d \r\n",g_bVibExecActive);

}

/******************************************************************************
 * LOCAL FUNCTION > _Vib_Mgr_Stop
 ******************************************************************************/
static void _Vib_Mgr_Stop(void)
{
    APP_DRV_PWM_Stop();
}


/******************************************************************************
 * LOCAL FUNCTION > _Vib_Mgr_Reset
 ******************************************************************************/
static void _Vib_Mgr_Reset(void)
{

    _Vib_Mgr_Stop();
    CC_VENUS_VIB_TIMER_DUTYCYCLECHANGE_Stop();
    CC_VENUS_VIB_TIMER_DUTYCYCLECHANGE_Clear();

    CC_VENUS_VIB_TIMER_EXECINTERVAL_Stop();
    CC_VENUS_VIB_TIMER_EXECINTERVAL_Clear();

    CC_VENUS_VIB_TIMER_OPT_Stop();
    CC_VENUS_VIB_TIMER_OPT_Clear();

    g_bVibExecActive = false;
    TracerInfo("_Vib_Mgr_Reset g_bVibExecActive = %d \r\n",g_bVibExecActive);

    g_bVibExecPatterRepeatCnt = 0;
    g_bVibExecPatterNum = 0;
    g_bVibPwmDutyCycleLevel = VIB_VAR_LEVEL_LMT_MAX;


}


/******************************************************************************
 * LOCAL FUNCTION > _Vib_Mgr_DutyCycleChangeHandler
 ******************************************************************************/
static void _Vib_Mgr_DutyCycleChangeHandler (void)
{
    if (E_APP_VIB_OPT_ASCENDING == s_tVibPara.eExecMode)
    {
        g_bVibPwmDutyCycleLevel = g_bVibPwmDutyCycleLevel + s_tVibPara.stVarAttr.bVar_LevelInterval;

        if (g_bVibPwmDutyCycleLevel > VIB_VAR_LEVEL_LMT_MAX )
        {
            if ( E_APP_VIB_VAR_REPEAT== s_tVibPara.stVarAttr.eVar_Type)
            {
                TracerInfo("_Vib_Mgr_DutyCycleChangeHandler E_APP_VIB_OPT_ASCENDING REPEAT \r\n");
                g_bVibPwmDutyCycleLevel = s_tVibPara.stVarAttr.bVar_StartLevel;
            }
            else
            {
                TracerInfo("_Vib_Mgr_DutyCycleChangeHandler E_APP_VIB_OPT_ASCENDING INFINITI \r\n");
                g_bVibPwmDutyCycleLevel = VIB_VAR_LEVEL_LMT_MAX;
            }
        }

        CC_VENUS_VIB_TIMER_DUTYCYCLECHANGE_Start(s_tVibPara.stVarAttr.wVar_TimeInterval);
    }


    if (E_APP_VIB_OPT_DESCENDING == s_tVibPara.eExecMode)
    {
        g_bVibPwmDutyCycleLevel = g_bVibPwmDutyCycleLevel - s_tVibPara.stVarAttr.bVar_LevelInterval;

        if (g_bVibPwmDutyCycleLevel < VIB_VAR_LEVEL_LMT_MIN )
        {
            if ( E_APP_VIB_VAR_REPEAT== s_tVibPara.stVarAttr.eVar_Type)
            {
                TracerInfo("_Vib_Mgr_DutyCycleChangeHandler E_APP_VIB_OPT_DESCENDING REPEAT \r\n");
                g_bVibPwmDutyCycleLevel = s_tVibPara.stVarAttr.bVar_StartLevel;
            }
            else
            {
                TracerInfo("_Vib_Mgr_DutyCycleChangeHandler E_APP_VIB_OPT_DESCENDING INFINITI \r\n");
                g_bVibPwmDutyCycleLevel = VIB_VAR_LEVEL_LMT_MIN;
            }
        }

        CC_VENUS_VIB_TIMER_DUTYCYCLECHANGE_Start(s_tVibPara.stVarAttr.wVar_TimeInterval);
    }

    TracerInfo("g_bVibPwmDutyCycleLevel %d !!!!!!!!!!!!!!!!!!!!!!!!!!!!!1\r\n",g_bVibPwmDutyCycleLevel);

}

/******************************************************************************
 * LOCAL FUNCTION > _Vib_Mgr_ExecIntervalHandler
 ******************************************************************************/
static void _Vib_Mgr_ExecIntervalHandler(void)
{

    //TracerInfo("_Vib_Mgr_ExecIntervalHandler \r\n");
    if (g_bVibExecActive == true)
    {
        APP_DRV_PWM_Stop();
        g_bVibExecActive = false;
        g_bVibExecPatterRepeatCnt++;
        CC_VENUS_VIB_TIMER_EXECINTERVAL_Start(s_tVibPara.stAttr.eVibPattern[g_bVibExecPatterNum].eVibOnOrOff.wVibOFF_ms);
        TracerInfo("g_bVibExecActive = %d \r\n",g_bVibExecActive);
    }
    else
    {


        if (g_bVibExecPatterRepeatCnt >=
            ((s_tVibPara.stAttr.eVibPattern[g_bVibExecPatterNum].bRepeatCount)))
        {
            // check next patten
            ++g_bVibExecPatterNum;


            if (g_bVibExecPatterNum <= s_tVibPara.stAttr.bNumOfPattern)
            {

                // do next pattern
                CC_VENUS_VIB_TIMER_EXECINTERVAL_Start(s_tVibPara.stAttr.eVibPattern[g_bVibExecPatterNum].eVibOnOrOff.wVibON_ms);
                g_bVibExecActive = true;
                TracerInfo("g_bVibExecActive = %d \r\n",g_bVibExecActive);
                g_bVibExecPatterRepeatCnt = 0 ;
                APP_DRV_PWM_Start(g_bVibPwmDutyCycleLevel);

            }
            else
            {
                //done
                //APP_DRV_PWM_Stop();
                TracerInfo("g_bVibExecActive error case = %d \r\n",g_bVibExecActive);

            }


        }
        else
        {
            // repeat
            CC_VENUS_VIB_TIMER_EXECINTERVAL_Start(s_tVibPara.stAttr.eVibPattern[g_bVibExecPatterNum].eVibOnOrOff.wVibON_ms);
            g_bVibExecActive = true;
            TracerInfo("g_bVibExecActive = %d \r\n",g_bVibExecActive);
            TracerInfo("g_bVibExecPatterRepeatCnt %d \r\n",g_bVibExecPatterRepeatCnt);
            TracerInfo("g_bVibExecPatterNum %d \r\n",g_bVibExecPatterNum);
            APP_DRV_PWM_Start(g_bVibPwmDutyCycleLevel);
        }


    }


}

/******************************************************************************
 * LOCAL FUNCTION > _Vib_Mgr_OptHandler
 ******************************************************************************/
static void _Vib_Mgr_OptHandler(void)
{
    if (E_APP_VIB_EXEC_REPEAT == s_tVibPara.eBehavior  )
    {
        _Vib_Mgr_Reset();
        _Vib_Mgr_Start();

    }
    else //if (E_APP_VIB_EXEC_ONESHOT == s_tVibPara.eBehavior  );
        _Vib_Mgr_Reset();
}

/******************************************************************************
 * LOCAL FUNCTION > _Vib_Mgr_EventSort
 ******************************************************************************/
static E_APP_VIB_RESULT _Vib_Mgr_EventSort(S_APP_VIB_PATTERN_CFG *_EventData)
{
    E_APP_VIB_RESULT eRet = E_APP_VIB_RET_SUCESS;

    if ( 0 >= _EventData->stAttr.bNumOfPattern)
    {
        eRet =  E_APP_VIB_RET_PATTERN_NUM_ERROR;
        return eRet;
    }

    s_tVibPara.stAttr.bNumOfPattern = _EventData->stAttr.bNumOfPattern;

    for (int i=0 ; i < s_tVibPara.stAttr.bNumOfPattern ; i++)
    {

        if (( VIB_SWITCH_ON_INTERVAL_MAX < _EventData->stAttr.eVibPattern[i].eVibOnOrOff.wVibON_ms)
            ||( VIB_SWITCH_ON_INTERVAL_MIN > _EventData->stAttr.eVibPattern[i].eVibOnOrOff.wVibON_ms))
        {

            eRet =  E_APP_VIB_RET_SWITCH_LIMITED_ERROR;
            return eRet;

        }
        if (( VIB_SWITCH_OFF_INTERVAL_MAX < _EventData->stAttr.eVibPattern[i].eVibOnOrOff.wVibOFF_ms)
            ||( VIB_SWITCH_OFF_INTERVAL_MIN > _EventData->stAttr.eVibPattern[i].eVibOnOrOff.wVibOFF_ms))
        {
            eRet =  E_APP_VIB_RET_SWITCH_LIMITED_ERROR;
            return eRet;

        }
        if ( 0 >= _EventData->stAttr.eVibPattern[i].bRepeatCount)
        {
            eRet =  E_APP_VIB_RET_EXEC_COUNT_ERROR;
            return eRet;
        }

        uint16_t _wTempOptMs = 0;
        for (int j = 1 ; j <= _EventData->stAttr.eVibPattern[i].bRepeatCount ;j++)

        {
            _wTempOptMs +=_EventData->stAttr.eVibPattern[i].eVibOnOrOff.wVibON_ms +
                          _EventData->stAttr.eVibPattern[i].eVibOnOrOff.wVibOFF_ms;
        }

        s_tVibPara.wOpt_Ms += _wTempOptMs;


    }

    TracerInfo("OPT MS = %d \r\n",s_tVibPara.wOpt_Ms );
    memcpy(&s_tVibPara.stAttr.eVibPattern, _EventData->stAttr.eVibPattern,
                (sizeof(S_APP_VIB_PATTERN_CFG_UNIT) * (s_tVibPara.stAttr.bNumOfPattern)));

    if ((E_APP_VIB_OPT_ASCENDING == _EventData->eExecMode)||( E_APP_VIB_OPT_DESCENDING == _EventData->eExecMode))
    {
        if ((VIB_VARIABLE_INTERVAL_MAX < _EventData->stVarAttr.wVar_TimeInterval)
            ||(VIB_VARIABLE_INTERVAL_MIN > _EventData->stVarAttr.wVar_TimeInterval))
        {
            eRet = E_APP_VIB_RET_INTERVAL_ERROR;
            return eRet;
        }
        else
        {
            s_tVibPara.stVarAttr.wVar_TimeInterval = _EventData->stVarAttr.wVar_TimeInterval;
        }

        if (VIB_VAR_LEVEL_INTERVAL_MAX < _EventData->stVarAttr.bVar_LevelInterval)
        {
            eRet = E_APP_VIB_RET_VAR_LEVEL_INTERVAL_ERROR;
            return eRet;
        }
        else
        {
            s_tVibPara.stVarAttr.bVar_LevelInterval = _EventData->stVarAttr.bVar_LevelInterval;
        }


        if (E_APP_VIB_VAR_INVAILD == _EventData->stVarAttr.eVar_Type)

        {
            eRet = E_APP_VIB_RET_VAR_TYPE_ERROR;
            return eRet;
        }
        else
        {
            s_tVibPara.stVarAttr.eVar_Type = _EventData->stVarAttr.eVar_Type;
        }

        if ((VIB_VAR_LEVEL_LMT_MAX < _EventData->stVarAttr.bVar_StartLevel)
            ||(VIB_VAR_LEVEL_LMT_MIN > _EventData->stVarAttr.bVar_StartLevel))
        {
            eRet = E_APP_VIB_RET_VAR_LEVEL_LIMIT_ERROR;
            return eRet;
        }
        else
        {
            s_tVibPara.stVarAttr.bVar_StartLevel = _EventData->stVarAttr.bVar_StartLevel;
        }

        if (VIB_VAR_LEVEL_INTERVAL_MAX < _EventData->stVarAttr.bVar_LevelInterval)
        {
            eRet = E_APP_VIB_RET_VAR_LEVEL_INTERVAL_ERROR;
            return eRet;
        }
        else
        {
            s_tVibPara.stVarAttr.bVar_LevelInterval = _EventData->stVarAttr.bVar_LevelInterval;
        }

    }

    return eRet;
}


/******************************************************************************
 * FUNCTION > APP_VibMgr_Init
 ******************************************************************************/
void APP_VibMgr_Init(void)
{
    g_bVibExecActive = false;
    g_bVibExecPatterRepeatCnt = 0;
    g_bVibExecPatterNum = 0;
    g_bVibPwmDutyCycleLevel = VIB_VAR_LEVEL_LMT_MAX;
}

/******************************************************************************
 * FUNCTION > APP_VibMgr_RegEventHandler
 ******************************************************************************/
void APP_VibMgr_RegEventHandler(S_VenusEvent *_stEvent)
{

    E_APP_SCHED_VIB_TIMER_TIMEOUT eEventTimeoutID =(E_APP_SCHED_VIB_TIMER_TIMEOUT)_stEvent->dwData;

    TracerInfo("\r\n APP_VibMgr_RegEventHandler %d \r\n",eEventTimeoutID);


    if ( E_APP_SCHED_VIB_TIMER_EXECINTERVAL_TIMEOUT == eEventTimeoutID)
    {
        _Vib_Mgr_ExecIntervalHandler();
    }

    if ( E_APP_SCHED_VIB_TIMER_OPT_TIMEOUT == eEventTimeoutID)
    {
        _Vib_Mgr_OptHandler();
    }

    if (E_APP_SCHED_VIB_TIMER_DUTYCYCLECHANGE_TIMEOUT == eEventTimeoutID)
    {
        _Vib_Mgr_DutyCycleChangeHandler();
    }

}

/******************************************************************************
 * FUNCTION > APP_VibMgr_StartService
 ******************************************************************************/
E_APP_VIB_RESULT APP_VibMgr_StartService(S_APP_VIB_PATTERN_CFG *_EventData)
{

    E_APP_VIB_RESULT eRet = E_APP_VIB_RET_SUCESS;
    _Vib_Mgr_Reset();
    memset(&s_tVibPara,0x00,sizeof(S_APP_VIB_PATTERN_CFG));

    if (( 0x00 == _EventData->eExecMode )
        ||(E_APP_VIB_OPT_DESCENDING <  _EventData->eExecMode ))
    {
        eRet = E_APP_VIB_RET_MODE_ERROR;
        return  eRet;
    }
    s_tVibPara.eExecMode = _EventData->eExecMode;


    if ((0x00 == _EventData->eBehavior )
        ||(E_APP_VIB_EXEC_REPEAT <  _EventData->eBehavior ))

    {
        eRet = E_APP_VIB_RET_EXEC_BEHAVIOR_ERROR;
        return  eRet;
    }
    s_tVibPara.eBehavior = _EventData->eBehavior;



    eRet = _Vib_Mgr_EventSort(_EventData);
    if ( E_APP_VIB_RET_SUCESS !=eRet )
    {
        return eRet;
    }

    _Vib_Mgr_Start();

    return eRet;


}


/******************************************************************************
 * FUNCTION > APP_VibMgr_StopService
 ******************************************************************************/
void APP_VibMgr_StopService(void)
{
    _Vib_Mgr_Reset();
}


#endif

