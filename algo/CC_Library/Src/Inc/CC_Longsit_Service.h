#ifndef _CC_LONGSIT_SRV_
#define _CC_LONGSIT_SRV_

#include "project.h"
#include "CC_DB_Structure.h"

eStete_t CC_Longsit_Srv_Get_LongSitServiceState(void);


void CC_Longsit_Srv_TimerInit(void);
void CC_Longsit_Srv_Enable(void);
void CC_Longsit_Srv_Disable(void);
void CC_Longsit_Srv_LowPowerStateChange(eStete_t _bEnable , uint8_t _bIsLowPower);
void CC_Longsit_Srv_handle(uint32_t _dwPedStep);
void CC_Longsit_Srv_Reset_Notification(void);
void CC_LongSit_Srv_Register(void);
void CC_Longsit_Srv_UtilityAlgoStateChange(uint32_t _dwState);
void CC_LongSit_Srv_CheckNotifyProc(void);
void CC_LongSit_Srv_Idle_CheckWearProc(void);
void CC_LongSit_Srv_Idle_MonitorMotionHanlder(void);
void CC_LongSit_Srv_TimeSetting(db_sys_longsit_t *_stExecPeriod);
void CC_LongSit_Srv_PollingHandler(void);



#endif

