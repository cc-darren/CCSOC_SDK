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

/******************************************************************************
*  Filename:
*  ---------
*  svc_mgr.c
*
*  Project:
*  --------
*  cc6801
*
*  Description:
*  ------------
*  Service Manager, taking care of,
*      (1) Event Dispatch,
*      (2) Decide running/suspended services
*      (3) Configure corresponding peripherals
*      (4) Report/notify status to applications
*
*  Author:
*  -------
*  CloudChip
*
*===========================================================================/
*  20171129 LOUIS initial version
******************************************************************************/

/******************************************************************************
 * INCLUDE FILE
 ******************************************************************************/
#include "svc_mgr.h"
#include "CC_AppSrvc_HeartRate.h"
#include "tracer.h"

/******************************************************************************
 * DEFINITION / CONSTANT / ENUM / TYPE
 ******************************************************************************/
typedef struct
{
    E_AppSvcHrMode    eHrMode;
    uint8_t           bSwitch;
    uint8_t           baPadding[2];
}   S_AppSvcEvtHrReq;

typedef struct
{
    E_AppSvcHrTimerID    eTimerID;
    uint8_t              baPadding[3];
}   S_AppSvcEvtTimeout;


/******************************************************************************
 * EXTERNAL FUNCTION
 ******************************************************************************/

/******************************************************************************
 *  VARIABLE
 ******************************************************************************/


/******************************************************************************
 * FUNCTION > APP_SVCMGR_EventHandler
 ******************************************************************************/
void    APP_SVCMGR_EventHandler(S_AppSchedEvent *tEvent)
{
    TracerInfo(  "[APP_SVCMGR_EventHandler] event: %d, argv: 0x%08X\r\n"
               , tEvent->bEventID
               , ((uint32_t) tEvent->vpData)                            );

    switch (tEvent->bEventID)
    {
    case E_APP_SVC_EVENT_HRM_SERVICE_REQUEST:
         {
             S_AppSvcEvtHrReq   *_ptReq = ((S_AppSvcEvtHrReq *) &tEvent->vpData);

             if (E_APP_SVC_HR_MODE_SINGLE_SHOT == _ptReq->eHrMode)
             {
                 if (0 == _ptReq->bSwitch)
                     CC_AppSrv_HR_StopSingleHR();
                 else
                     CC_AppSrv_HR_StartSingleHR();
             }
             else if (E_APP_SVC_HR_MODE_STRAP == _ptReq->eHrMode)
             {
                 if (0 == _ptReq->bSwitch)
                     CC_AppSrv_HR_StopHRS();
                 else
                     CC_AppSrv_HR_StartHRS();
             }
             else //if (E_APP_SVC_HR_MODE_24HR == _ptReq->eHrMode)
             {
                 if (0 == _ptReq->bSwitch)
                     CC_AppSrv_HR_Stop24HR();
                 else
                     CC_AppSrv_HR_Start24HR();
             }
         }
         break;

    case E_APP_SVC_EVENT_HRM_TIMEOUT:
         {
             S_AppSvcEvtTimeout   *_peTimeout = ((S_AppSvcEvtTimeout *) &tEvent->vpData);

             if (E_APP_SVC_HR_TIMER_24HR_ONE_MEASUREMENT == _peTimeout->eTimerID)
                 CC_AppSrv_24HR_Handler_ToOneMeasurement();
             else //if (E_APP_SVC_HR_TIMER_24HR_ONE_MEASUREMENT == *_peTimerID)
                 CC_AppSrv_24HR_Handler_ToPeriodicMeasurement();
         }
         break;
    }
}

/******************************************************************************
 * FUNCTION > APP_SVCMGR_Init
 ******************************************************************************/
void    APP_SVCMGR_Init(void)
{
    APP_SCHED_RegEventHandler(E_APP_SCHED_MODID_SERVICE_MGR, APP_SVCMGR_EventHandler);
}

/******************************************************************************
 * FUNCTION > APP_SVCMGR_PostEvent_HrRequest
 ******************************************************************************/
void    APP_SVCMGR_PostEvent_HrRequest(E_AppSvcHrMode eMode, uint8_t bSwitch)
{
    S_AppSchedEvent     _tEvent;
    S_AppSvcEvtHrReq   *_ptReq = ((S_AppSvcEvtHrReq *) &_tEvent.vpData);
        
    _tEvent.eModuleID     = E_APP_SCHED_MODID_SERVICE_MGR;
    _tEvent.bEventID      = E_APP_SVC_EVENT_HRM_SERVICE_REQUEST;
    _tEvent.wDataByteSize = 0;

    _ptReq->eHrMode = eMode;
    _ptReq->bSwitch = bSwitch;
    
    APP_SCHED_PostEvent(&_tEvent);
}

/******************************************************************************
 * FUNCTION > APP_SVCMGR_PostEvent_HrTimeout
 ******************************************************************************/
void    APP_SVCMGR_PostEvent_HrTimeout(E_AppSvcHrTimerID eTimerID)
{
    S_AppSchedEvent       _tEvent;
    S_AppSvcEvtTimeout   *_ptTimerout = ((S_AppSvcEvtTimeout *) &_tEvent.vpData);

    _tEvent.eModuleID     = E_APP_SCHED_MODID_SERVICE_MGR;
    _tEvent.bEventID      = E_APP_SVC_EVENT_HRM_TIMEOUT;
    _tEvent.wDataByteSize = 0;

    _ptTimerout->eTimerID = eTimerID;
    
    APP_SCHED_PostEvent(&_tEvent);
}

