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

#ifndef _APP_SVC_MGR_H_
#define _APP_SVC_MGR_H_

/******************************************************************************
 * INCLUDE FILE
 ******************************************************************************/
#include "scheduler.h"

 /******************************************************************************
 * DEFINITION / CONSTANT / ENUM / TYPE
 ******************************************************************************/

typedef enum
{
    // HEART RATE
    E_APP_SVC_EVENT_HRM_SERVICE_REQUEST,
    E_APP_SVC_EVENT_HRM_TIMEOUT,

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


/******************************************************************************
 * EXPORTED FUNCTION
 ******************************************************************************/

/******************************************************************************
 * [FUNCTION] APP_SVCMGR_Init
 *     Initialize Service Manager (SVC_MGR)
 * [ARGUMENT] None.
 * [RETURN  ] None.
 ******************************************************************************/
extern void    APP_SVCMGR_Init(void);

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

#endif    //(_APP_SVC_MGR_H_)

