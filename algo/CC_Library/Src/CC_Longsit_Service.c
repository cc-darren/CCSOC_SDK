#include "project_FPGA_Venus.h"


#include "CC_Longsit_Service.h"
#include "CC_SleepMonitor_Service.h"
#include "CC_Slpmtr.h"
#include "CC_Longsit.h"

//#define NRF_LOG_MODULE_NAME "APP"
//#include "nrf_log.h"
//#include "nrf_log_ctrl.h"
#include "tracer.h"
#define LONGSIT_TIME_TH     60


extern uint8_t CC_Charge_Register(charge_cb_t cb);
extern eStete_t CC_Get_LongsitEnState(void );
extern uint8_t CC_GetLowPowerState(void);
extern eDEV_CHARGE_STATE_t CC_GetChargeStatus(void);
static uint32_t g_dwUserSleepSate = 0x00000000;


#if 0 // longsit algo. for temporarily
uint8_t LongSit_Process(uint32_t steps){}
void LongSit_close(void){}
uint8_t LongSit_is_open(void){return 0x01;}
void LongSit_reset_notification(void){}
void LongSit_set_time_threshold(uint16_t threshold){}
uint8_t LongSit_open(void){}

#endif


void CC_Longsit_Srv_Enable(void)
{

    if (eDisable==CC_Get_LongsitEnState())
        return;
    
    if ( eDEVICE_CHARGE_IN == CC_GetChargeStatus()) 
        return;
    
    if  (true == CC_GetLowPowerState())
        return;
            
    if (true == CC_SleepMonitor_GetSleepState())
        return;
            
    if (!LongSit_is_open())
    {
        LongSit_open();
        LongSit_set_time_threshold(LONGSIT_TIME_TH);
        LongSit_reset_notification();
        TracerInfo("CC_Longsit_Srv_Enable Init LongSit_open \r\n");
        }
        else
        {
        TracerInfo("CC_Longsit_Srv_Enable Init LongSit already open!!!! \r\n");
        }
    }

void CC_Longsit_Srv_Disable(void)
        {
    if (LongSit_is_open())
        {
        LongSit_close();
        TracerInfo("CC_Longsit_Srv_Disable  LongSit_close \r\n");
    }
}

static void _LongSit_Charge_Evt_CB(eDEV_CHARGE_STATE_t eState)
{
    if (eDEVICE_CHARGE_IN == eState)
    {
         // Jason, 20170425, Disable longsit algorithm when plug in charging.
            CC_Longsit_Srv_Disable();
        
    }
    else if (eDEVICE_CHARGE_OUT== eState)
    {
            // Jason, 20170425, // plug out charging, reset longsit algorithm
        if ( eEnable ==CC_Get_LongsitEnState())
        {
            CC_Longsit_Srv_Enable();
        }   
    }
    else
    {
        //no this case;
    }
}
void CC_LongSit_Srv_Register(void)
{
    CC_Charge_Register(_LongSit_Charge_Evt_CB);
}

void CC_Longsit_Srv_NotifySleepState(eStete_t _eSleepAlgoState)
{
    if ( eDisable ==_eSleepAlgoState )
        CC_Longsit_Srv_Enable();
    else if ( eEnable ==_eSleepAlgoState )
        CC_Longsit_Srv_Disable();
    
    // else sleep algorithm enable nothing to do.
}

void CC_Longsit_Srv_LowPowerStateChange(eStete_t _bEnable , uint8_t _bIsLowPower)
{
    if ((eEnable==_bEnable) && (false == _bIsLowPower))
        CC_Longsit_Srv_Enable();
    else
        CC_Longsit_Srv_Disable();
   
}

void CC_Longsit_Srv_SleepAlgoStateChange(uint32_t _dwState)
{
    g_dwUserSleepSate = _dwState;
    
    switch (g_dwUserSleepSate)
    {
        case CC_SLEEPMETER_AWAKE:
                CC_Longsit_Srv_Enable();
            break;
        case CC_SLEEPMETER_DEEPSLEEP:
        case CC_SLEEPMETER_LIGHTSLEEP:
        case CC_SLEEPMETER_ROLLOVER:
        case CC_SLEEPMETER_IDLE:
                CC_Longsit_Srv_Disable();
            break;
        default:
            break;
    }
}

void CC_Longsit_Srv_handle(uint32_t _dwPedStep)
{
    LongSit_Process(_dwPedStep); 
}


void CC_Longsit_Srv_Reset_Notification(void)
{
    LongSit_reset_notification(); 
}

