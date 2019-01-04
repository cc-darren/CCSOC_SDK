/* Copyright (c) 2019 Cloudchip, Inc. All Rights Reserved.
 *
 * The information contained herein is property of Cloudchip, Inc.
 * Terms and conditions of usage are described in detail in CLOUDCHIP
 * STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information.
 * NO WARRANTY of ANY KIND is provided. This heading must NOT be removed 
 * from the file.
 */

#include "CC_global_config.h"

#ifdef UTILITY_EN
#include "CC_Utility_Service.h"
#include "CC_Utility.h"

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
    }
    else
    {
    }
}

void CC_Utility_Srv_Disable(void)
{
    if (utility_is_opened())
    {
        utility_close();
        CC_Utility_ResetTimer();
        g_bUtilityEnCnt =0;
    }
    else
    {
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
    return state;
}

#endif
