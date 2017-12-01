#ifndef _CC_LONGSIT_SRV_
#define _CC_LONGSIT_SRV_

#include "project.h"

void CC_Longsit_Srv_Enable(void);
void CC_Longsit_Srv_Disable(void);
void CC_Longsit_Srv_NotifySleepState(eStete_t _eState);
void CC_Longsit_Srv_LowPowerStateChange(eStete_t _bEnable , uint8_t _bIsLowPower);
void CC_Longsit_Srv_SleepAlgoStateChange(uint32_t _dwState);
void CC_Longsit_Srv_handle(uint32_t _dwPedStep);
void CC_Longsit_Srv_Reset_Notification(void);
void CC_LongSit_Srv_Register(void);
void CC_Longsit_Srv_NotifySleepState(eStete_t _eSleepAlgoState);



#endif

