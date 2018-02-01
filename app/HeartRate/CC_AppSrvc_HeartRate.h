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

#ifndef _CC_APP_SVC_HR_
    #define _CC_APP_SVC_HR_


#define APPSRV_HRM_DEFAULT_RESTING_MAX          (150)
#define APPSRV_HRM_DEFAULT_RESTING_MIN          ( 40)
#define APPSRV_HRM_DEFAULT_EXERCISE_MAX(age)    ((220 - age) * 85 / 100)
#define APPSRV_HRM_DEFAULT_EXERCISE_MIN(age)    ((220 - age) * 65 / 100)

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
extern void CC_AppSrv_HR_SetLimited(uint8_t _bRestingHrMax,uint8_t bRestingHrMin,uint8_t bExerciseHrMax,uint8_t bExerciseHrMin);
extern void CC_AppSrv_HR_ResetLimited(uint8_t _bAge);

#endif    // END of _CC_APP_SVC_HR_

