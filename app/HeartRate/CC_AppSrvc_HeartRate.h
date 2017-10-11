
#ifndef _CC_APP_SVC_HR_
    #define _CC_APP_SVC_HR_

extern void CC_AppSrv_HR_Register(void);
extern void CC_AppSrv_HR_Init(void);

extern void CC_AppSrv_HR_Set24HrPeriod(uint32_t dwPeriodicMeasurementTime_ms, uint32_t dwOneMeasurementMaxTime_ms);
extern void CC_AppSrv_HR_Get24HrPeriod(uint32_t *pdwPeriodicMeasurementTime_ms, uint32_t *pdwOneMeasurementMaxTime_ms);

extern void CC_AppSrv_HR_StartSingleHR(void);
extern void CC_AppSrv_HR_StopSingleHR(void);

extern void CC_AppSrv_HR_StartHRS(void);
extern void CC_AppSrv_HR_StopHRS(void);

extern void CC_AppSrv_HR_Start24HR(void);
extern void CC_AppSrv_HR_Stop24HR(void);
extern void CC_AppSrv_HR_DataReport(int16_t nHrData, int16_t nTrustLevel);
extern void CC_AppSrv_24HR_Handler_ToOneMeasurement(void);
extern void CC_AppSrv_24HR_Handler_ToPeriodicMeasurement(void);

extern bool CC_AppSrv_HR_IsSingleHrEnabled(void);
extern bool CC_AppSrv_HR_IsHrsEnabled(void);
extern bool CC_AppSrv_HR_Is24HrEnabled(void);

extern bool CC_AppSrv_HR_IsHrmWorking(void);

#define CC_APPSRV_HR_APP_LOCK      ((bool) true )
#define CC_APPSRV_HR_APP_UNLOCK    ((bool) false)
extern void CC_AppSrv_HR_SetAppLockHrm(bool bIsAppLocked);
extern void CC_AppSrv_HR_StartSystemTick(void);
extern void CC_AppSrv_HR_StopSystemTick(void);
extern uint32_t Hrm_get_sys_tick(void);
extern void CC_AppSrv_HR_StatusReport(uint8_t _bResult);
extern void CC_AppSrv_HR_ClrHrmStatus(void);
extern uint8_t CC_AppSrv_HR_GetHrmStatus(void);

#endif    // END of _CC_APP_SVC_HR_

