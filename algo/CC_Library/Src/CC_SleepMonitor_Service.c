#include "project_FPGA_Venus.h"


#include "CC_SleepMonitor_Service.h"
#include "CC_Longsit_Service.h"
#include "CC_Slpmtr.h"
#include "tracer.h"

extern uint8_t CC_Charge_Register(charge_cb_t cb);
extern void _sensor_algorithm_sleepmeter_proc(void);
extern void CC_MainGet_CurrentTime(app_date_time_t *_stCurTime);
extern eDEV_CHARGE_STATE_t CC_GetChargeStatus(void);
extern void CC_ResetSleep_StateCount(void);
extern float g_fSleepCalSeconds;
extern uint8_t g_bSleepEnCnt;


uint8_t m_bIsSleepAlgActive = false;
uint8_t m_bIsSleepAlwayon = false;
uint8_t m_bIsSleepStartService = false;
db_sys_sleep_monitor_t m_stSleepExecPeriod;
eSleep_State_t eAlgoProcState =eInit;


#if 0 //sleep algorithm for temporarily
uint8_t g_bSleepEnCnt;
float g_fSleepCalSeconds;
uint8_t g_bSleepEnCnt;

void _sensor_algorithm_sleepmeter_proc(void){}
void slpmtr_close(void){}
int8_t slpmtr_is_opened(void){return 0x01;}
void slpmtr_open_with_sensitivity(const slpmtr_sens_t sensitivity){}

#endif





static void _SleepMonitor_Charge_Evt_CB(eDEV_CHARGE_STATE_t eState)
{
    if (eDEVICE_CHARGE_IN == eState)
    {
        CC_SleepMonitor_Srv_Disable();
    }
    else if (eDEVICE_CHARGE_OUT== eState)
    {
        eAlgoProcState = eStartToService;
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


static uint8_t _SleepMonitor_CheckAlwayON(void)
{
    if ((m_stSleepExecPeriod.start_time_hour == m_stSleepExecPeriod.end_time_hour) &&
    (m_stSleepExecPeriod.start_time_min == m_stSleepExecPeriod.end_time_min))
    {
        m_bIsSleepAlwayon = true;
        TracerInfo("_SleepMonitor_CheckAlwayON Alway on  !!!!!!\r\n");
        CC_SleepMonitor_Srv_Enable();
        return true;
    }
    else
    {    
        TracerInfo("_SleepMonitor_CheckAlwayON Time different   !!!!!!\r\n");
        m_bIsSleepAlwayon = false;
        return false;
    }
}

static uint8_t _SleepMonitor_CheckStartAlgo(void)
{
    app_date_time_t _stCurTime;
    CC_MainGet_CurrentTime(&_stCurTime);
    uint16_t _wStartTime = m_stSleepExecPeriod.start_time_hour *60 + m_stSleepExecPeriod.start_time_min;
    uint16_t _wEndTime = m_stSleepExecPeriod.end_time_hour *60 + m_stSleepExecPeriod.end_time_min;
    uint16_t _wCurTime = _stCurTime.hours*60 + _stCurTime.minutes;
    
    if (m_bIsSleepAlgActive == true)
        return false;

    if (_wStartTime < _wEndTime)
    {
        if ((_wCurTime>= _wStartTime) && (_wCurTime <= _wEndTime))
        {
            TracerInfo("CC_SleepMonitor_Srv_PollingCheck Enable  1  !!!!!!\r\n");
            CC_SleepMonitor_Srv_Enable();    
            
            return true;

        }
        else
        {
            //CC_SleepMonitor_Srv_Disable();
        }
    }
    else
    {
        if ((_wCurTime < _wStartTime) && (_wCurTime > _wEndTime))
        {
            
            //CC_SleepMonitor_Srv_Disable();    

        }
        else
        {
            TracerInfo("CC_SleepMonitor_Srv_PollingCheck Enable  2  !!!!!!\r\n");
            CC_SleepMonitor_Srv_Enable();
            return true;
        }

        
    }
    
    return false;
}

static uint8_t _SleepMonitor_CheckStopAlgo(void)
{
    app_date_time_t _stCurTime;
    CC_MainGet_CurrentTime(&_stCurTime);
    uint16_t _wStartTime = m_stSleepExecPeriod.start_time_hour *60 + m_stSleepExecPeriod.start_time_min;
    uint16_t _wEndTime = m_stSleepExecPeriod.end_time_hour *60 + m_stSleepExecPeriod.end_time_min;
    uint16_t _wCurTime = _stCurTime.hours*60 + _stCurTime.minutes;


    if (m_bIsSleepAlgActive == false)
        return false;

    if (_wStartTime < _wEndTime)
    {
        if ((_wCurTime>= _wStartTime) && (_wCurTime <= _wEndTime))
        {
            //CC_SleepMonitor_Srv_Enable();    

        }
        else
        {
            TracerInfo("CC_SleepMonitor_Srv_PollingCheck Disable  1 !!!!!!\r\n");
    
            CC_SleepMonitor_Srv_Disable();
            return true;

        }
    }
    else
    {
        if ((_wCurTime < _wStartTime) && (_wCurTime > _wEndTime))
        {
            TracerInfo("CC_SleepMonitor_Srv_PollingCheck Disable  2 !!!!!!\r\n");

            CC_SleepMonitor_Srv_Disable();    
            return true;
        }
        else
        {
            //CC_SleepMonitor_Srv_Enable();

        }

    }    

    return false;
}

void _SleepMonitor_SwitchAlgo(void)
{
    TracerInfo("_SleepMonitor_SwitchAlgo \r\n");
    app_date_time_t _stCurTime;
    CC_MainGet_CurrentTime(&_stCurTime);
    uint16_t _wStartTime = m_stSleepExecPeriod.start_time_hour *60 + m_stSleepExecPeriod.start_time_min;
    uint16_t _wEndTime = m_stSleepExecPeriod.end_time_hour *60 + m_stSleepExecPeriod.end_time_min;
    uint16_t _wCurTime = _stCurTime.hours*60 + _stCurTime.minutes;


    if (_wStartTime < _wEndTime)
    {
        if ((_wCurTime>= _wStartTime) && (_wCurTime <= _wEndTime))
        {
            TracerInfo("_SleepMonitor_SwitchAlgo Enable  1 !!!!!!\r\n");
           
            CC_SleepMonitor_Srv_Enable();   
            eAlgoProcState = eAlgoCheckStopAlgo;
        }
        else
        {
            TracerInfo("_SleepMonitor_SwitchAlgo Disable  2 !!!!!!\r\n");
        
            CC_SleepMonitor_Srv_Disable();
            eAlgoProcState = eAlgoCheckStartAlgo;
        }
    }
    else
    {
        if ((_wCurTime < _wStartTime) && (_wCurTime > _wEndTime))
        {
            TracerInfo("_SleepMonitor_SwitchAlgo Disable  3 !!!!!!\r\n");

            CC_SleepMonitor_Srv_Disable();    
            eAlgoProcState = eAlgoCheckStartAlgo;
        }
        else
        {
            TracerInfo("_SleepMonitor_SwitchAlgo enable  4 !!!!!!\r\n");
        
            CC_SleepMonitor_Srv_Enable();
            eAlgoProcState = eAlgoCheckStopAlgo;
        }        
    }
}

void CC_SleepMonitor_InitSetTimePeriod(db_sys_sleep_monitor_t *_stExecPeriod)
{
     m_stSleepExecPeriod.start_time_hour= _stExecPeriod->start_time_hour;
     m_stSleepExecPeriod.start_time_min = _stExecPeriod->start_time_min;
     m_stSleepExecPeriod.end_time_hour = _stExecPeriod->end_time_hour;
     m_stSleepExecPeriod.end_time_min  = _stExecPeriod->end_time_min;
     eAlgoProcState = eInit;
}

uint8_t CC_SleepMonitor_GetSleepState(void)
{
    return m_bIsSleepAlgActive;
}

void CC_SleepMonitor_Srv_Enable(void)
{
    if (!slpmtr_is_opened())
    {
        slpmtr_open_with_sensitivity(SLPMTR_SENS_LOW);
        m_bIsSleepAlgActive= true;
#ifdef LONGSIT_EN        
        CC_Longsit_Srv_NotifySleepState(eEnable);
#endif        
        CC_ResetSleep_StateCount();
        TracerInfo("CC_SleepMonitor_Srv_Enable \r\n");
    } 
}

void CC_SleepMonitor_Srv_Disable(void)
{
    if (slpmtr_is_opened())
    {
        slpmtr_close();
        m_bIsSleepAlgActive= false;
        g_fSleepCalSeconds = 0;
        g_bSleepEnCnt=0;
#ifdef LONGSIT_EN        
        CC_Longsit_Srv_NotifySleepState(eDisable);
#endif        
        TracerInfo("CC_SleepMonitor_Srv_Disable \r\n");
    }        
}

void CC_SleepMonitor_Srv_SyncTimeSlot(db_sys_sleep_monitor_t *_stExecPeriod)
{
    //  1 first time service 
    if (m_bIsSleepStartService == false)
    {
        m_stSleepExecPeriod.start_time_hour= _stExecPeriod->start_time_hour;
        m_stSleepExecPeriod.start_time_min = _stExecPeriod->start_time_min;
        m_stSleepExecPeriod.end_time_hour = _stExecPeriod->end_time_hour;
        m_stSleepExecPeriod.end_time_min  = _stExecPeriod->end_time_min;        

        eAlgoProcState = eStartToService;
        m_bIsSleepStartService = true;
    
    }
    else
    {
        // 2 time change or not
        if (( m_stSleepExecPeriod.start_time_hour != _stExecPeriod->start_time_hour) ||
        ( m_stSleepExecPeriod.start_time_min != _stExecPeriod->start_time_min) ||
        ( m_stSleepExecPeriod.end_time_hour != _stExecPeriod->end_time_hour) ||
        ( m_stSleepExecPeriod.end_time_min != _stExecPeriod->end_time_min))
        {
            m_stSleepExecPeriod.start_time_hour= _stExecPeriod->start_time_hour;
            m_stSleepExecPeriod.start_time_min = _stExecPeriod->start_time_min;
            m_stSleepExecPeriod.end_time_hour = _stExecPeriod->end_time_hour;
            m_stSleepExecPeriod.end_time_min  = _stExecPeriod->end_time_min;        

            
            eAlgoProcState = eTimeSettingChange;
        }
        else
        {
            // to do 
            // no time change
        }    
    
    }    
}

void CC_SleepMonitor_Srv_PollingCheck(void)
{

    
    if (m_bIsSleepStartService == false)
        return;

    if (eDEVICE_CHARGE_IN == CC_GetChargeStatus())
        return;
    
    switch (eAlgoProcState)
    {
        case eInit:
            break;
        case eStartToService:
                if (true == _SleepMonitor_CheckAlwayON())
                {
                    eAlgoProcState = eAlgoCheckStopAlgo;
                    TracerInfo("CC_SleepMonitor_Srv_PollingCheck FIRST TIME ALWAYS ON \r\n");
                }                   
                else
                {
                    _SleepMonitor_SwitchAlgo();                    
                }
            break;
        case eTimeSettingChange:            
            {
                // 1, check time same
                if (true == _SleepMonitor_CheckAlwayON())
                {
                    eAlgoProcState = eAlgoCheckStopAlgo;
                    return;
                }
                else
                {
                    _SleepMonitor_SwitchAlgo();        
                }    
            }
            break;

        case eAlgoCheckStartAlgo:
            {
                if (true == _SleepMonitor_CheckStartAlgo())
                    eAlgoProcState = eAlgoCheckStopAlgo;
            }
            break;
        case eAlgoCheckStopAlgo:
            {
                if (m_bIsSleepAlwayon == true)
                    break;
                if (true == _SleepMonitor_CheckStopAlgo())
                    eAlgoProcState = eAlgoCheckStartAlgo;

            }
            break;
            
        case eAlgoInvaild:
        default:
            break;

    }

}

void CC_SleepMonitor_Srv_Handle(void)
{
    if ( false ==m_bIsSleepAlgActive )
        return;
    if  ( eDEVICE_CHARGE_IN == CC_GetChargeStatus() )
        return;
    
        _sensor_algorithm_sleepmeter_proc();
}


