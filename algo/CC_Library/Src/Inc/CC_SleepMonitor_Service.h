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

#ifndef _CC_SLEEPMONITOR_SRV_
#define _CC_SLEEPMONITOR_SRV_



#define   DB_COMMAND_START     0x01
#define   DB_COMMAND_DATA     0x02
#define   DB_COMMAND_END     0x04


void CC_SleepMonitor_Srv_Register(void);
eStete_t CC_SleepMonitor_GetSleepState(void);
void CC_SleepMonitor_Srv_Enable(void);
void CC_SleepMonitor_Srv_Disable(void);
void CC_SleepMonitor_Srv_Handle(void);
eStete_t CC_SleepMonitor_Srv_Get_SleepService(void);
void CC_SleepMonitor_Srv_Enable_SleepService(eStete_t eStete);
void CC_SleepMonitor_Srv_Disable_SleepService(eStete_t eStete);


typedef enum 
{
    eInit =0,
    eStartToService,
    eTimeSettingChange,
    eAlgoCheckStartAlgo,
    eAlgoCheckStopAlgo,
    eAlgoInvaild,
}eSleep_State_t;

#endif

