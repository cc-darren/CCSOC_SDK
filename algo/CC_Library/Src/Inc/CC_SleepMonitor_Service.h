#ifndef _CC_SLEEPMONITOR_SRV_
#define _CC_SLEEPMONITOR_SRV_
//#include "CC_DB_Structure.h"


void CC_SleepMonitor_Srv_Register(void);
//void CC_SleepMonitor_InitSetTimePeriod(db_sys_sleep_monitor_t *_stExecPeriod);
uint8_t CC_SleepMonitor_GetSleepState(void);
void CC_SleepMonitor_Srv_Enable(void);
void CC_SleepMonitor_Srv_Disable(void);
void CC_SleepMonitor_Srv_PollingCheck(void);
void CC_SleepMonitor_Srv_Handle(void);
//void CC_SleepMonitor_Srv_SyncTimeSlot(db_sys_sleep_monitor_t *_stExecPeriod);


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

