/******************************************************************************
*  Copyright 2017, CloudChip, Inc.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of CloudChip, Inc. (C) 2017
******************************************************************************/

#ifdef APP_SERV_MGR_EN


#ifndef _CC_APP_SVC_MANAGER_H
#define _CC_APP_SVC_MANAGER_H


/******************************************************************************
 * INCLUDE FILE
 ******************************************************************************/
#include "scheduler.h"
#include <stdint.h>
#include "error.h"

 /******************************************************************************
 * DEFINITION / CONSTANT / ENUM / TYPE
 ******************************************************************************/

#ifdef SRV_MGR_TEST_EN
//    #define SRV_MGR_TEST_CASE_1     // pedo test    
    #define SRV_MGR_TEST_CASE_2     // hrm test    
//    #define SRV_MGR_TEST_CASE_3     // swimming test    
//    #define SRV_MGR_TEST_CASE_4     // suspend/resume test    

#endif

typedef enum
{
    // HEART RATE
    E_APP_SVC_EVENT_HRM_SERVICE_REQUEST,
    E_APP_SVC_EVENT_HRM_TIMEOUT,

    // PEDOMETER
    E_APP_SVC_EVENT_PEDO_SERVICE_REQEST,

    // SWIM
    E_APP_SVC_EVENT_SWIM_SERVICE_REQEST,

    // END OF EVENT
    E_APP_SVC_EVENT_DUMMY_END = 0xFF    
}   E_AppSvcEventID;

// HEART RATE
typedef enum
{
    E_APP_SVC_HR_MODE_SINGLE_SHOT,
    E_APP_SVC_HR_MODE_STRAP,
    E_APP_SVC_HR_MODE_24HR,
    E_APP_SVC_HR_MODE_DUMMY_END = 0xFF    
}   E_AppSvcHrMode;

typedef enum
{
    E_APP_SVC_HR_TIMER_24HR_ONE_MEASUREMENT,
    E_APP_SVC_HR_TIMER_24HR_PERIODIC_MEASUREMENT,
    E_APP_SVC_HR_TIMER_DUMMY_END = 0xFF    
}   E_AppSvcHrTimerID;


typedef enum
{
    E_APP_SRV_ERR_NONE = CC_SUCCESS,            
    E_APP_SRV_ERR_TYPE,
    E_APP_SRV_ERR_CONFLICT,  
    E_APP_SRV_ERR_START_FAIL,
    E_APP_SRV_ERR_STOP_FAIL, 
    //E_APP_SRV_ERR_GET_DATA_FAIL, 
    E_APP_SRV_ERR_NOT_WORK_IN_SUSPEND, 
    E_APP_SRV_ERR_CANT_RESUME_FROM_ACTIVE, 
    E_APP_SRV_ERR_RESUME_FAIL,
    
}   E_App_Srv_Err_Code;

/******************************************************************************
 * EXPORTED FUNCTION
 ******************************************************************************/

/******************************************************************************
 * [FUNCTION] APP_SVCMGR_Init
 *     Initialize Service Manager (SVC_MGR)
 * [ARGUMENT] None.
 * [RETURN  ] E_App_Srv_Err_Code.
 ******************************************************************************/
extern E_App_Srv_Err_Code    APP_SVCMGR_Init(void);

/******************************************************************************
 * [FUNCTION] APP_SVCMGR_PostEvent_HrRequest
 *     Post the event to request a Heart Rate service ON or OFF.
 * [ARGUMENT]
 *     <in> eMode: which HR service to be controlled. Refer to enum: E_AppSvcHrMode
 *            .E_APP_SVC_HR_MODE_SINGLE_SHOT: user triggered manually
 *            .E_APP_SVC_HR_MODE_STRAP      : for exercise, record each second
 *            .E_APP_SVC_HR_MODE_24HR       : 24HR monitoring
 *     <in> bSwitch: turn ON/OFF
 *            .zero to turn off this service
 *            .one to turn on this service
  * [RETURN  ] None.
 ******************************************************************************/
extern void    APP_SVCMGR_PostEvent_HrRequest(E_AppSvcHrMode eMode, uint8_t bSwitch);

/******************************************************************************
 * [FUNCTION] APP_SVCMGR_PostEvent_HrTimeout
 *     Post the event to indicate a timer expired.
 * [ARGUMENT]
 *     <in> eTimerID: specify which timer expires. Refer to enum: E_AppSvcHrTimerID
  * [RETURN  ] None.
 ******************************************************************************/
extern void    APP_SVCMGR_PostEvent_HrTimeout(E_AppSvcHrTimerID eTimerID);


/******************************************************************************
 * [FUNCTION] APP_SVCMGR_PostEvent_PedoRequest
 *     Post the event to request a pedometer service ON or OFF.
 * [ARGUMENT]
 *     <in> bSwitch: turn ON/OFF
 *            .zero to turn off this service
 *            .one to turn on this service
  * [RETURN  ] None.
 ******************************************************************************/
extern void    APP_SVCMGR_PostEvent_PedoRequest(uint8_t bSwitch);


/******************************************************************************
 * [FUNCTION] APP_SVCMGR_PostEvent_PedoRequest
 *     Post the event to request a swim service ON or OFF.
 * [ARGUMENT]
 *     <in> bSwitch: turn ON/OFF
 *            .zero to turn off this service
 *            .one to turn on this service
  * [RETURN  ] None.
 ******************************************************************************/
extern void    APP_SVCMGR_PostEvent_SwimRequest(uint8_t bSwitch);

/******************************************************************************
 * [FUNCTION] APP_SVCMGR_Test
 *     Test sensor manager
 * [ARGUMENT] None.
 * [RETURN  ] None.
 ******************************************************************************/
extern void APP_SVCMGR_Test(void);




#endif// end of _CC_APP_SVC_MANAGER_H


#endif
