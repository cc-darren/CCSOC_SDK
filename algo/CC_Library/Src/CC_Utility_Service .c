#include "CC_global_config.h"

#ifdef UTILITY_EN
#include "CC_Utility_Service.h"
#include "CC_Utility.h"

#define NRF_LOG_MODULE_NAME "APP"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"

extern uint8_t CC_Charge_Register(charge_cb_t cb);
extern uint8_t CC_GetLowPowerState(void);
extern eDEV_CHARGE_STATE_t CC_GetChargeStatus(void);
extern void _sensor_algorithm_utility_proc(void);
extern void CC_Utility_ResetTimer(void);
extern float g_bUtilityEnCnt;



static void _Utiltiy_Charge_Evt_CB(eDEV_CHARGE_STATE_t eState)
{
    if (eDEVICE_CHARGE_IN == eState)
    {
         // Jason, 20170425, Disable longsit algorithm when plug in charging.
            CC_Utility_Srv_Disable();
        
    }
    else if (eDEVICE_CHARGE_OUT== eState)
    {
        // Jason, 20170425, // plug out charging, reset longsit algorithm
        CC_Utility_Srv_Enable();

    }
    else
    {
        //no this case;
    }
}

void CC_Utility_Srv_Enable(void)
{

    if ( eDEVICE_CHARGE_IN == CC_GetChargeStatus()) 
        return;
    
    if  (true == CC_GetLowPowerState())
        return;
            
    if (!utility_is_opened())
    {
        utility_open();
        NRF_LOG_INFO("CC_Utility_Srv_Enable Init utility_open \r\n");
    }
    else
    {
        NRF_LOG_INFO("CC_Utility_Srv_Enable Init utility_open already open!!!! \r\n");
    }
}

void CC_Utility_Srv_Disable(void)
{
    if (utility_is_opened())
    {
        utility_close();
        CC_Utility_ResetTimer();
        g_bUtilityEnCnt =0;
        NRF_LOG_INFO("CC_Utility_Srv_Disable  Utility_close \r\n");
    }
    else
    {
        NRF_LOG_INFO("CC_Utility_Srv_Enable Already close  Utility algorithm!!!! \r\n");
    }
}

void CC_Utility_Srv_Register(void)
{
    CC_Charge_Register(_Utiltiy_Charge_Evt_CB);
}

void CC_Utility_Srv_Handle(void)
{
    _sensor_algorithm_utility_proc(); 
}

uint8_t CC_Utility_Srv_Get_Energy_state(void)
{
    uint8_t state = utility_get_energy_status();
    NRF_LOG_INFO("CC_Utility_Srv_Get_Energy_state  =%d \r\n",state);
    return state;
}

#endif
