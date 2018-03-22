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

#ifdef APP_SERV_MGR_EN

/******************************************************************************
*  Filename:
*  ---------
*  CC_AppSrvc_Manager.c
*
*  Project:
*  --------
*  cc6801
*
*  Description:
*  ------------
*  Service Manager, taking care of,
*      (1) Manage clash among multiple services
*      (2) Service dispatch while service requests are upder different threads
*      (3) Provide running/suspended services
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
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "project.h"
#include "CC_Sensor_Manager.h"
#include "CC_AppSrvc_Manager.h"
#include "CC_AppSrvc_HeartRate.h"
#include "error.h"
#include "tracer.h"
#include "scheduler.h"
#include "clock.h"
#include "tracer.h"

/******************************************************************************
 * DEFINITION / CONSTANT / ENUM / TYPE
 ******************************************************************************/

#define IS_SRV_ACTIVED(type)    ((E_APP_SRV_ST_ACTIVE == b_app_srv_state[type]) ? true : false)
#define IS_SRV_SUSPENDED(type)  ((E_APP_SRV_ST_SUSPEND == b_app_srv_state[type]) ? true : false)

typedef enum
{
//    E_APP_SRV_ID_HRM = 0,
    E_APP_SRV_ID_PEDO = 0,
    E_APP_SRV_ID_SWIM,
    E_APP_SRV_ID_SINGLE_HR,
    E_APP_SRV_ID_24HR_HR,    
    E_APP_SRV_ID_HRS,    

    E_APP_SRV_ID_TOTAL,
}E_App_Srv_ID;
   

typedef enum
{
    E_APP_SRV_ST_IDLE = 0,            
    E_APP_SRV_ST_ACTIVE,
    E_APP_SRV_ST_SUSPEND,    
        
}E_App_Srv_State;

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

typedef struct
{
    uint8_t           bSwitch;
    uint8_t           baPadding[3];
}   S_AppSvcEvtPedoReq;

typedef struct
{
    uint8_t           bSwitch;
    uint8_t           baPadding[3];
}   S_AppSvcEvtSwimReq;

/******************************************************************************
 * EXTERNAL FUNCTION
 ******************************************************************************/

/******************************************************************************
 *  VARIABLE
 ******************************************************************************/
 static E_App_Srv_State b_app_srv_state[E_APP_SRV_ID_TOTAL];


 /******************************************************************************
 * [FUNCTION] AppSrv_Suspend
 *     Indicate one of the service to enter suspend mode.
 * [ARGUMENT] 
 *     <in> id: identifier for registered from service manager
 * [RETURN  ] E_App_Srv_Err_Code.
 ******************************************************************************/
#if 0    // TODO: comment out for compiler warning
 static E_App_Srv_Err_Code AppSrv_Suspend(E_App_Srv_ID id)
 {
 
     E_App_Srv_Err_Code ret_code = E_APP_SRV_ERR_NONE;
 
     b_app_srv_state[id] = E_APP_SRV_ST_SUSPEND;
 
     switch(id)
     {
#if 0        
         case E_APP_SRV_ID_HRM:
             CC_SenMgr_Stop_HRM();
             break;
#endif         
         case E_APP_SRV_ID_PEDO:            
             CC_SenMgr_Stop_Pedometer();
             break;
         case E_APP_SRV_ID_SWIM:            
             CC_SenMgr_Stop_Swim();
             break;
         default:
             ret_code = E_APP_SRV_ERR_TYPE;
             break;
     }
    
 
     return ret_code; 
 }
#endif

 /******************************************************************************
 * [FUNCTION] AppSrv_Resume
 *     Judge which suspended service sholud be resumed
 * [ARGUMENT] 
 *     <in> id: identifier for registered from service manager
 * [RETURN  ] E_App_Srv_Err_Code.
 ******************************************************************************/
 static E_App_Srv_Err_Code AppSrv_Resume(void)
 {
 
     E_App_Srv_Err_Code ret_code = E_APP_SRV_ERR_NONE;
 
#ifdef SRV_MGR_TEST_CASE_4
     if(IS_SRV_SUSPENDED(E_APP_SRV_ID_SWIM))
     {
         TracerInfo("CC_AppSrv_Sensor_Resume: %d\r\n", (E_APP_SRV_ID_SWIM));
         if(E_SEN_ERROR_NONE == CC_SenMgr_Start_Swim())            
             b_app_srv_state[E_APP_SRV_ID_HRM] = E_APP_SRV_ST_ACTIVE;        
         else
             ret_code = E_APP_SRV_ERR_RESUME_FAIL;
     }      
 
#endif
 
     return ret_code; 
 }



/******************************************************************************
* [FUNCTION] CC_AppSrv_Manager_Start
*     Invoke the corresponding function to enable service, and return error code
*     if any services clash with each other. If not return error, 
*     use b_app_srv_state to record service state: idle, active, suspended.
* [ARGUMENT] 
*     <in> id: identifier for registered from service manager
* [RETURN  ] E_App_Srv_Err_Code.
******************************************************************************/ 
E_App_Srv_Err_Code   CC_AppSrv_Manager_Start(E_App_Srv_ID id)
{
    E_App_Srv_Err_Code ret_code = E_APP_SRV_ERR_NONE;
    
    switch(id)
    {
#if 0        
        case E_APP_SRV_ID_HRM:

            if(IS_SRV_ACTIVED(E_APP_SRV_ID_SWIM))
                return E_APP_SRV_ERR_CONFLICT;

            if(E_SEN_ERROR_NONE != CC_SenMgr_Start_HRM())
                return E_APP_SRV_ERR_START_FAIL;

            break;
#endif            
        case E_APP_SRV_ID_PEDO:            
            
#if 0 // not used resume here           
            if(IS_SRV_ACTIVED(E_APP_SRV_ID_SWIM))
                AppSrv_Suspend(E_APP_SRV_ID_SWIM);
#endif
            if(IS_SRV_ACTIVED(E_APP_SRV_ID_SWIM))
                return E_APP_SRV_ERR_CONFLICT;

            if(E_SEN_ERROR_NONE != CC_SenMgr_Start_Pedometer())
                return E_APP_SRV_ERR_START_FAIL;
            
            break;
            
        case E_APP_SRV_ID_SWIM:  // the highest priority!          
#if 0
            if(IS_SRV_ACTIVED(E_APP_SRV_ID_HRM))
                AppSrv_Suspend(E_APP_SRV_ID_HRM);

            if(IS_SRV_ACTIVED(E_APP_SRV_ID_PEDO))
                AppSrv_Suspend(E_APP_SRV_ID_PEDO);
#endif

            if(IS_SRV_ACTIVED(E_APP_SRV_ID_SINGLE_HR)
                || IS_SRV_ACTIVED(E_APP_SRV_ID_24HR_HR)
                || IS_SRV_ACTIVED(E_APP_SRV_ID_HRS)
                || IS_SRV_ACTIVED(E_APP_SRV_ID_PEDO))
                return E_APP_SRV_ERR_CONFLICT;

            if(E_SEN_ERROR_NONE != CC_SenMgr_Start_Swim())
                return E_APP_SRV_ERR_START_FAIL;
            
            break;

        case E_APP_SRV_ID_SINGLE_HR:            

            if(IS_SRV_ACTIVED(E_APP_SRV_ID_24HR_HR) 
                || IS_SRV_ACTIVED(E_APP_SRV_ID_HRS)
                || IS_SRV_ACTIVED(E_APP_SRV_ID_SWIM))
                return E_APP_SRV_ERR_CONFLICT;
            
            CC_AppSrv_HR_StartSingleHR();
            
            break;

        case E_APP_SRV_ID_24HR_HR:            
            
            if(IS_SRV_ACTIVED(E_APP_SRV_ID_SINGLE_HR) 
                || IS_SRV_ACTIVED(E_APP_SRV_ID_HRS)
                || IS_SRV_ACTIVED(E_APP_SRV_ID_SWIM))
                return E_APP_SRV_ERR_CONFLICT;
        
            CC_AppSrv_HR_Start24HR();
            
            break;

        case E_APP_SRV_ID_HRS:            
            
            if(IS_SRV_ACTIVED(E_APP_SRV_ID_SINGLE_HR) 
                || IS_SRV_ACTIVED(E_APP_SRV_ID_24HR_HR)
                || IS_SRV_ACTIVED(E_APP_SRV_ID_SWIM))
                return E_APP_SRV_ERR_CONFLICT;
        
            CC_AppSrv_HR_StartHRS();
            
            break;
        
        default:
            ret_code = E_APP_SRV_ERR_TYPE;
            break;
    }


    b_app_srv_state[id] = E_APP_SRV_ST_ACTIVE;


    return ret_code;
}



/******************************************************************************
* [FUNCTION] CC_AppSrv_Manager_Stop
*     Invoke the corresponding function to disable service. At the end of function,
*     use b_app_srv_state to reset service state to idle.
* [ARGUMENT] 
*     <in> id: identifier for registered from service manager
* [RETURN  ] E_App_Srv_Err_Code.
******************************************************************************/ 
E_App_Srv_Err_Code   CC_AppSrv_Manager_Stop(E_App_Srv_ID id)
{

    E_App_Srv_Err_Code ret_code = E_APP_SRV_ERR_NONE;

    //TracerInfo("CC_AppSrv_Manager_Stop: %d!\r\n", id);

    switch(id)
    {
#if 0        
        case E_APP_SRV_ID_HRM:            
            
            if(E_SEN_ERROR_NONE != CC_SenMgr_Stop_HRM())
               return E_APP_SRV_ERR_STOP_FAIL;             
            
            break;            
#endif            
        case E_APP_SRV_ID_PEDO:            
            
            if(E_SEN_ERROR_NONE != CC_SenMgr_Stop_Pedometer())
               return E_APP_SRV_ERR_STOP_FAIL;             
            
            break;
            
        case E_APP_SRV_ID_SWIM:            
            
            if(E_SEN_ERROR_NONE != CC_SenMgr_Stop_Swim())
               return E_APP_SRV_ERR_STOP_FAIL;           
            
            break;

        case E_APP_SRV_ID_SINGLE_HR:            
            CC_AppSrv_HR_StopSingleHR();
            break;

        case E_APP_SRV_ID_24HR_HR:            
            CC_AppSrv_HR_Stop24HR();
            break;

        case E_APP_SRV_ID_HRS:            
            CC_AppSrv_HR_StopHRS();
            break;
        
        default:
            
            ret_code = E_APP_SRV_ERR_TYPE;
            
            break;
    }



    b_app_srv_state[id] = E_APP_SRV_ST_IDLE;

    AppSrv_Resume();

    return ret_code;
}



/******************************************************************************
* [FUNCTION] APP_SVCMGR_EventHandler
*     Provide a space to handle all the service requests that need to execute 
*     in the main thread
* [ARGUMENT] 
*     <in> ptAppEvent: a pointer to the struct where below info. specified,
*            .eModuleID    : ID of the dest. module to handle this event
*            .bEventID     : ID of the event, defined by each module
*            .wDataByteSize: Byte-size of allocated buffer for additional data
*            .vpData       : An pointer to allocated buf, where data is stored
* [RETURN  ] None.
******************************************************************************/ 

void   APP_SVCMGR_EventHandler(S_AppSchedEvent *tEvent)
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
                    CC_AppSrv_Manager_Stop(E_APP_SRV_ID_SINGLE_HR);
                 else
                    CC_AppSrv_Manager_Start(E_APP_SRV_ID_SINGLE_HR);
             }
             else if (E_APP_SVC_HR_MODE_STRAP == _ptReq->eHrMode)
             {
                 if (0 == _ptReq->bSwitch)
                    CC_AppSrv_Manager_Stop(E_APP_SRV_ID_HRS);
                 else
                    CC_AppSrv_Manager_Start(E_APP_SRV_ID_HRS);
             }
             else //if (E_APP_SVC_HR_MODE_24HR == _ptReq->eHrMode)
             {
                 if (0 == _ptReq->bSwitch)
                    CC_AppSrv_Manager_Stop(E_APP_SRV_ID_24HR_HR);
                 else
                    CC_AppSrv_Manager_Start(E_APP_SRV_ID_24HR_HR);
             }

             break;
         }
         
         case E_APP_SVC_EVENT_HRM_TIMEOUT:
         {
             S_AppSvcEvtTimeout   *_peTimeout = ((S_AppSvcEvtTimeout *) &tEvent->vpData);

             if (E_APP_SVC_HR_TIMER_24HR_ONE_MEASUREMENT == _peTimeout->eTimerID)
                 CC_AppSrv_24HR_Handler_ToOneMeasurement();
             else //if (E_APP_SVC_HR_TIMER_24HR_ONE_MEASUREMENT == *_peTimerID)
                 CC_AppSrv_24HR_Handler_ToPeriodicMeasurement();

             break;
         }
         
         case E_APP_SVC_EVENT_PEDO_SERVICE_REQEST:
         {
            S_AppSvcEvtPedoReq  *_ptReq = ((S_AppSvcEvtPedoReq *) &tEvent->vpData);
    
            if (0 == _ptReq->bSwitch)
                CC_AppSrv_Manager_Stop(E_APP_SRV_ID_PEDO);
            else
                CC_AppSrv_Manager_Start(E_APP_SRV_ID_PEDO);
            
            break;
         }
         
         case E_APP_SVC_EVENT_SWIM_SERVICE_REQEST:
         {
            S_AppSvcEvtSwimReq  *_ptReq = ((S_AppSvcEvtSwimReq *) &tEvent->vpData);
    
            if (0 == _ptReq->bSwitch)
                CC_AppSrv_Manager_Stop(E_APP_SRV_ID_SWIM);
            else
                CC_AppSrv_Manager_Start(E_APP_SRV_ID_SWIM);
            
            break;            
         }
    }
}



/******************************************************************************
 * FUNCTION > APP_SVCMGR_Init
 ******************************************************************************/
E_App_Srv_Err_Code   APP_SVCMGR_Init(void)
{
    E_App_Srv_Err_Code ret_code = E_APP_SRV_ERR_NONE;
    
    for(uint8_t i = 0; i < E_APP_SRV_ID_TOTAL; i++)
    {
        b_app_srv_state[i] = E_APP_SRV_ST_IDLE;
    
    }

    APP_SCHED_RegEventHandler(E_APP_SCHED_MODID_SERVICE_MGR, APP_SVCMGR_EventHandler);

    return ret_code;
}

/******************************************************************************
 * FUNCTION > APP_SVCMGR_PostEvent_HrRequest
 ******************************************************************************/
void    APP_SVCMGR_PostEvent_HrRequest(E_AppSvcHrMode eMode, uint8_t bSwitch)
{
    S_AppSchedEvent     _tEvent;
    S_AppSvcEvtHrReq   *_ptReq;
    
    _tEvent.eModuleID     = E_APP_SCHED_MODID_SERVICE_MGR;
    _tEvent.bEventID      = E_APP_SVC_EVENT_HRM_SERVICE_REQUEST;
    _tEvent.wDataByteSize = sizeof(S_AppSvcEvtHrReq);
    _tEvent.vpData = ((void *) malloc(_tEvent.wDataByteSize));

    _ptReq = (S_AppSvcEvtHrReq*) _tEvent.vpData;
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
    S_AppSvcEvtTimeout   *_ptTimerout;
    

    _tEvent.eModuleID     = E_APP_SCHED_MODID_SERVICE_MGR;
    _tEvent.bEventID      = E_APP_SVC_EVENT_HRM_TIMEOUT;
    _tEvent.wDataByteSize = sizeof(S_AppSvcEvtTimeout);
    _tEvent.vpData = ((void *) malloc(_tEvent.wDataByteSize));  

    _ptTimerout = (S_AppSvcEvtTimeout*) _tEvent.vpData;
    _ptTimerout->eTimerID = eTimerID;
    
    APP_SCHED_PostEvent(&_tEvent);
}


/******************************************************************************
 * FUNCTION > APP_SVCMGR_PostEvent_PedoRequest
 ******************************************************************************/
void    APP_SVCMGR_PostEvent_PedoRequest(uint8_t bSwitch)
{
    S_AppSchedEvent     _tEvent;
    S_AppSvcEvtPedoReq   *_ptReq;
    
    _tEvent.eModuleID     = E_APP_SCHED_MODID_SERVICE_MGR;
    _tEvent.bEventID      = E_APP_SVC_EVENT_PEDO_SERVICE_REQEST;
    _tEvent.wDataByteSize = sizeof(S_AppSvcEvtPedoReq);
    _tEvent.vpData = ((void *) malloc(_tEvent.wDataByteSize));

    _ptReq = (S_AppSvcEvtPedoReq*) _tEvent.vpData;
    _ptReq->bSwitch = bSwitch;
    
    APP_SCHED_PostEvent(&_tEvent);
}

/******************************************************************************
 * FUNCTION > APP_SVCMGR_PostEvent_SwimRequest
 ******************************************************************************/
void    APP_SVCMGR_PostEvent_SwimRequest(uint8_t bSwitch)
{
    S_AppSchedEvent     _tEvent;
    S_AppSvcEvtSwimReq   *_ptReq;
    
    _tEvent.eModuleID     = E_APP_SCHED_MODID_SERVICE_MGR;
    _tEvent.bEventID      = E_APP_SVC_EVENT_SWIM_SERVICE_REQEST;
    _tEvent.wDataByteSize = sizeof(S_AppSvcEvtSwimReq);
    _tEvent.vpData = ((void *) malloc(_tEvent.wDataByteSize));

    _ptReq = (S_AppSvcEvtSwimReq*) _tEvent.vpData;
    _ptReq->bSwitch = bSwitch;
    
    APP_SCHED_PostEvent(&_tEvent);
}




//////////////  Test Code /////////////////////////

#ifdef SRV_MGR_TEST_EN



void APP_SVCMGR_Test(void)
{
    int16_t hrm_acc_data[MEMS_FIFO_SIZE];
    uint32_t hrm_acc_size = 0;
    //uint8_t hrm_ppg_data[HEART_RATE_MODE_SAMPLES_PER_READ * 4];
    uint32_t hrm_ppg_data[HEART_RATE_MODE_SAMPLES_PER_READ];
    uint32_t hrm_ppg_size = 0;   

    int16_t pedo_acc_data[MEMS_FIFO_SIZE];
    uint32_t pedo_acc_size = 0;
    
    int16_t swim_acc_data[MEMS_FIFO_SIZE];
    int16_t swim_gyro_data[MEMS_FIFO_SIZE];    
    uint32_t swim_acc_size = 0;
    uint32_t swim_gyro_size = 0;

    AxesRaw_t mag_data;
    uint32_t mag_size = 0;  

    uint32_t get_data_size = 0;
    uint32_t get_data2_size = 0;    

    TracerInfo("[SRV_TC-1]: CC_AppSrv_Manager_Init()\r\n");

    CC_AppSrv_Manager_Init();


#ifdef SRV_MGR_TEST_CASE_1

    #define TEST_CASE_ROUND_NB  10


    for(uint8_t round = 0; round < TEST_CASE_ROUND_NB; round++)
    {

        TracerInfo("[SRV_TC-1]: Test Round: %d\r\n", round);
        

        if(E_APP_SRV_ERR_NONE != CC_AppSrv_Manager_Start(E_APP_SRV_ID_PEDO))
        {
            TracerInfo("[SRV_TC-1]: CC_AppSrv_Manager_Start fail!\r\n");
            
            while(1)
                ;
        }

        get_data_size = 0;

        while(get_data_size < MEMS_FIFO_SIZE) // get data until fifo full!
        {

            if(E_APP_SRV_ERR_NONE == CC_SenMgr_Acc_GetData(E_APP_SRV_ID_PEDO, pedo_acc_data, &pedo_acc_size))
            {
                if(0x00 != pedo_acc_size)
                {
                    TracerInfo("[SRV_TC-1]: pedo_accel_size: %d\r\n", pedo_acc_size);
                
                    for(uint16_t i = 0; i < pedo_acc_size/sizeof(int16_t); i+=3)
                    {                                       
                        TracerInfo("pedo_accel_X: %d\r\n", pedo_acc_data[i]);
                        TracerInfo("pedo_accel_Y: %d\r\n", pedo_acc_data[i+1]);
                        TracerInfo("pedo_accel_Z: %d\r\n", pedo_acc_data[i+2]);        
                    }
                        
                    TracerInfo("\r\n");            

                    
                                        
                }            

                get_data_size += pedo_acc_size;            
           }

        }


        TracerInfo("[SRV_TC-1]: Pedo get data done.\r\n");

        TracerInfo("[SRV_TC-1]: CC_AppSrv_Manager_Stop()\r\n");

        if(E_APP_SRV_ERR_NONE != CC_AppSrv_Manager_Stop(E_APP_SRV_ID_PEDO))
        {
            TracerInfo("[SRV_TC-1]: CC_AppSrv_Manager_Stop fail!\r\n");
        
            while(1)
                ;
        }

        if(E_APP_SRV_ERR_NONE != CC_SenMgr_Acc_GetData(E_APP_SRV_ID_PEDO, pedo_acc_data, &pedo_acc_size))
        {
            TracerInfo("[SRV_TC-1]: CC_SenMgr_Acc_GetData() don't get any data after stopping.\r\n");
        }
        else
        {
            TracerInfo("[SRV_TC-1]: Acc Data should not get here!! \r\n");
        }

        
        
    }


    TracerInfo("[SRV_TC-1]: SRV Test Case 1 pass!\r\n");

    
    

    while(1)
        ;


#elif defined(SRV_MGR_TEST_CASE_2)

#define TEST_CASE_ROUND_NB  10


    for(uint8_t round = 0; round < TEST_CASE_ROUND_NB; round++)
    {

        TracerInfo("[SRV_TC-2]: Test Round: %d\r\n", round);
        

        if(E_APP_SRV_ERR_NONE != CC_AppSrv_Manager_Start(E_APP_SRV_ID_HRM))
        {
            TracerInfo("[SRV_TC-2]: CC_AppSrv_Manager_Start fail!\r\n");
            
            while(1)
                ;
        }

        get_data_size = 0;
        get_data2_size = 0;

        while((get_data_size < MEMS_FIFO_SIZE)
            || (get_data2_size < MEMS_FIFO_SIZE))// get data until fifo full!
        {

            if(E_APP_SRV_ERR_NONE == CC_SenMgr_Acc_GetData(E_APP_SRV_ID_HRM, hrm_acc_data, &hrm_acc_size))
            {
                if(0x00 != hrm_acc_size)
                {                
                    for(uint16_t i = 0; i < hrm_acc_size/sizeof(int16_t); i+=3)
                    {                                       
                        TracerInfo("hrm_accel_X: %d\r\n", hrm_acc_data[i]);
                        TracerInfo("hrm_accel_Y: %d\r\n", hrm_acc_data[i+1]);
                        TracerInfo("hrm_accel_Z: %d\r\n", hrm_acc_data[i+2]);        
                    }
                        
                    TracerInfo("\r\n");            

                    
                                        
                }            

                get_data_size += hrm_acc_size;            
           }

            
             if(E_APP_SRV_ERR_NONE == CC_SenMgr_PPG_GetData(E_APP_SRV_ID_HRM, hrm_ppg_data, &hrm_ppg_size))
             {
                 if(0x00 != hrm_ppg_size)
                 {
                     TracerInfo("[SRV_TC-2]: hrm_ppg_size: %d\r\n", hrm_ppg_size);
                 
                     for(uint16_t i = 0; i < hrm_ppg_size/sizeof(uint32_t); i+=3)
                     {                                       
                         TracerInfo("hrm_ppg_X: %d\r\n", hrm_ppg_data[i]);
                         TracerInfo("hrm_ppg_Y: %d\r\n", hrm_ppg_data[i+1]);
                         TracerInfo("hrm_ppg_Z: %d\r\n", hrm_ppg_data[i+2]);        

                         
                                                           
                     }
                         
                     TracerInfo("\r\n");                                   
                 }            
            
                 get_data2_size += hrm_ppg_size;            
            }

        }


        TracerInfo("[SRV_TC-2]: Pedo get data done.\r\n");

        TracerInfo("[SRV_TC-2]: CC_AppSrv_Manager_Stop()\r\n");

        if(E_APP_SRV_ERR_NONE != CC_AppSrv_Manager_Stop(E_APP_SRV_ID_HRM))
        {
            TracerInfo("[SRV_TC-2]: CC_AppSrv_Manager_Stop fail!\r\n");
        
            while(1)
                ;
        }

        if(E_APP_SRV_ERR_NONE != CC_SenMgr_Acc_GetData(E_APP_SRV_ID_HRM, pedo_acc_data, &pedo_acc_size))
        {
            TracerInfo("[SRV_TC-2]: CC_SenMgr_Acc_GetData() don't get any data after stopping.\r\n");
        }
        else
        {
            TracerInfo("[SRV_TC-2]: Acc Data should not get here!! size\r\n");
        }

        if(E_APP_SRV_ERR_NONE != CC_SenMgr_PPG_GetData(E_APP_SRV_ID_HRM, hrm_ppg_data, &hrm_ppg_size))
        {
            TracerInfo("[SRV_TC-2]: CC_SenMgr_PPG_GetData() don't get any data after stopping.\r\n");
        }
        else
        {
            TracerInfo("[SRV_TC-2]: PPG Data should not get here!! \r\n");
        }
        

        
        
    }


    TracerInfo("[SRV_TC-2]: SRV Test Case 2 pass!\r\n");

    
    

    while(1)
        ;



#elif defined(SRV_MGR_TEST_CASE_3)

    #define TEST_CASE_ROUND_NB  10


    for(uint8_t round = 0; round < TEST_CASE_ROUND_NB; round++)
    {

        TracerInfo("[SRV_TC-3]: Test Round: %d\r\n", round);
        

        if(E_APP_SRV_ERR_NONE != CC_AppSrv_Manager_Start(E_APP_SRV_ID_SWIM))
        {
            TracerInfo("[SRV_TC-3]: CC_AppSrv_Manager_Start fail!\r\n");
            
            while(1)
                ;
        }

        get_data_size = 0;
        get_data2_size = 0;

        while((get_data_size < MEMS_FIFO_SIZE) 
            || (get_data2_size < MEMS_FIFO_SIZE))// get data until fifo full!
        {

            if(E_APP_SRV_ERR_NONE == CC_SenMgr_Acc_GetData(E_APP_SRV_ID_SWIM, swim_acc_data, &swim_acc_size))
            {
                if(0x00 != swim_acc_size)
                {
                    TracerInfo("[SRV_TC-3]: swim_acc_size: %d\r\n", swim_acc_size);
                
                    for(uint16_t i = 0; i < swim_acc_size/sizeof(int16_t); i+=3)
                    {                                       
                        TracerInfo("swim_accel_X: %d\r\n", swim_acc_data[i]);
                        TracerInfo("swim_accel_Y: %d\r\n", swim_acc_data[i+1]);
                        TracerInfo("swim_accel_Z: %d\r\n", swim_acc_data[i+2]);        
                    }
                        
                    TracerInfo("\r\n");            

                    
                                        
                }            

                get_data_size += swim_acc_size;            
           }


           if(E_APP_SRV_ERR_NONE == CC_SenMgr_Gyro_GetData(E_APP_SRV_ID_SWIM, swim_gyro_data, &swim_gyro_size))
           {
               if(0x00 != swim_gyro_size)
               {
                   TracerInfo("[SRV_TC-3]: swim_gyro_size: %d\r\n", swim_gyro_size);
                 
                   for(uint16_t i = 0; i < swim_gyro_size/sizeof(int16_t); i+=3)
                   {                                       
                       TracerInfo("swim_gyro_X: %d\r\n", swim_gyro_data[i]);
                       TracerInfo("swim_gyro_Y: %d\r\n", swim_gyro_data[i+1]);
                       TracerInfo("swim_gyro_Z: %d\r\n", swim_gyro_data[i+2]);        
                   }
                         
                   TracerInfo("\r\n");            
            
                   
                                       
               }            
            
               get_data2_size += swim_gyro_size;            
           }


           if(E_APP_SRV_ERR_NONE == CC_SenMgr_Mag_GetData(E_APP_SRV_ID_SWIM, (int16_t*)&mag_data, &mag_size))
           {
               if(0x00 != mag_size)
               {
                   TracerInfo("[SRV_TC-3]: swim_mag_size: %d\r\n", mag_size);
                 
                   TracerInfo("swim_mag_X: %d\r\n", mag_data.AXIS_X);
                   TracerInfo("swim_mag_Y: %d\r\n", mag_data.AXIS_Y);
                   TracerInfo("swim_mag_Z: %d\r\n", mag_data.AXIS_Z);        
                         
                   TracerInfo("\r\n");            
            
                   
                                       
               }            
            
           }

        }


        TracerInfo("[SRV_TC-3]: Pedo/Gyro get data done.\r\n");

        TracerInfo("[SRV_TC-3]: CC_AppSrv_Manager_Stop()\r\n");

        if(E_APP_SRV_ERR_NONE != CC_AppSrv_Manager_Stop(E_APP_SRV_ID_SWIM))
        {
            TracerInfo("[SRV_TC-3]: CC_AppSrv_Manager_Stop fail!\r\n");
        
            while(1)
                ;
        }

        if(E_APP_SRV_ERR_NONE != CC_SenMgr_Acc_GetData(E_APP_SRV_ID_SWIM, swim_acc_data, &swim_acc_size))
        {
            TracerInfo("[SRV_TC-3]: CC_SenMgr_Acc_GetData() don't get any data after stopping.\r\n");
        }
        else
        {
            TracerInfo("[SRV_TC-3]: Acc Data should not get here!! size\r\n");
        }

        if(E_APP_SRV_ERR_NONE != CC_SenMgr_Gyro_GetData(E_APP_SRV_ID_SWIM, swim_gyro_data, &swim_gyro_size))
        {
            TracerInfo("[SRV_TC-3]: CC_SenMgr_Gyro_GetData() don't get any data after stopping.\r\n");
        }
        else
        {
            TracerInfo("[SRV_TC-3]: Gyro Data should not get here!! size\r\n");
        }

        if(E_APP_SRV_ERR_NONE != CC_SenMgr_Mag_GetData(E_APP_SRV_ID_SWIM, (int16_t*)&mag_data, &mag_size))
        {
            TracerInfo("[SRV_TC-3]: CC_SenMgr_Mag_GetData() don't get any data after stopping.\r\n");
        }
        else
        {
            TracerInfo("[SRV_TC-3]: Mag Data should not get here!! \r\n");
        }

        
        
    }


    TracerInfo("[SRV_TC-3]: SRV Test Case 3 pass!\r\n");

    
    

    while(1)
        ;



#elif defined(SRV_MGR_TEST_CASE_4)


    TracerInfo("[SRV_TC-4]: CC_AppSrv_Manager_Start\r\n");

    if(E_APP_SRV_ERR_NONE != CC_AppSrv_Manager_Start(E_APP_SRV_ID_SWIM))
    {
        TracerInfo("[SRV_TC-3]: CC_AppSrv_Manager_Start fail!\r\n");
        
        while(1)
            ;
    }


    cc6801_ClockDelayMs(100);

    TracerInfo("[SRV_TC-4]: Get Mag:\r\n");
    
    

    if(E_APP_SRV_ERR_NONE == CC_SenMgr_Mag_GetData(E_APP_SRV_ID_SWIM, (int16_t*)&mag_data, &mag_size))
    {
         if(0x00 != mag_size)
         {
              TracerInfo("[SRV_TC-4]: swim_mag_size: %d\r\n", mag_size);
              
              TracerInfo("swim_mag_X: %d\r\n", mag_data.AXIS_X);
              TracerInfo("swim_mag_Y: %d\r\n", mag_data.AXIS_Y);
              TracerInfo("swim_mag_Z: %d\r\n", mag_data.AXIS_Z);                              
              TracerInfo("\r\n");            
         
              
                                  
         }            
         
    }


    TracerInfo("[SRV_TC-4]: Suspend SWIM\r\n");
    AppSrv_Suspend(E_APP_SRV_ID_SWIM);


    TracerInfo("[SRV_TC-4]: Get Mag\r\n");

    if(E_APP_SRV_ERR_NONE != CC_SenMgr_Mag_GetData(E_APP_SRV_ID_SWIM, (int16_t*)&mag_data, &mag_size))
    {
         TracerInfo("[SRV_TC-4]: CC_SenMgr_Mag_GetData() don't get any data after suspend.\r\n");
    }
    else
    {
         TracerInfo("[SRV_TC-4]: Mag Data should not get here!! \r\n");
    }
     

    TracerInfo("[SRV_TC-4]: Resume SWIM\r\n");
    if(E_APP_SRV_ERR_NONE != AppSrv_Resume())
    {
        TracerInfo("[SRV_TC-4]: Resume Fail!\r\n");
        
                

        while(1)
            ;
    }


    cc6801_ClockDelayMs(100);

    TracerInfo("[SRV_TC-4]: Get Mag:\r\n");
    
    
    

    if(E_APP_SRV_ERR_NONE == CC_SenMgr_Mag_GetData(E_APP_SRV_ID_SWIM, (int16_t*)&mag_data, &mag_size))
    {
        if(0x00 != mag_size)
        {
            TracerInfo("[SRV_TC-4]: swim_mag_size: %d\r\n", mag_size);
          
            TracerInfo("swim_mag_X: %d\r\n", mag_data.AXIS_X);
            TracerInfo("swim_mag_Y: %d\r\n", mag_data.AXIS_Y);
            TracerInfo("swim_mag_Z: %d\r\n", mag_data.AXIS_Z);        
                  
            TracerInfo("\r\n");            
     
            
                                
        }            
     
    }
    else
    {
        TracerInfo("[SRV_TC-4]: Can't get SWIM data!!\r\n");
        
                  
    }



    if(E_APP_SRV_ERR_NONE != CC_AppSrv_Manager_Stop(E_APP_SRV_ID_SWIM))
    {
          TracerInfo("[SRV_TC-2]: CC_AppSrv_Manager_Stop fail!\r\n");
      
          while(1)
              ;
    }


    TracerInfo("[SRV_TC-4]: SRV Test Case 4 pass!\r\n");

    
    

    while(1)
        ;      

#else


#define CC_APPSRV_SENSOR_TEST_MODE     1


#if defined (CC_APPSRV_SENSOR_TEST_MODE) && (CC_APPSRV_SENSOR_TEST_MODE == 2U) 
    // TEST1,  for Swim
    if(E_APP_SRV_ERR_NONE != CC_AppSrv_Manager_Start(E_APP_SRV_ID_SWIM))
    {
        TracerInfo("CC_AppSrv_Manager_Start fail!\r\n");
            
        while(1)
           ;
    }
#endif


#if 0 // for PEDO    
    if(E_APP_SRV_ERR_NONE != CC_AppSrv_Manager_Start(E_APP_SRV_ID_PEDO))
    {
        TracerInfo("CC_AppSrv_Manager_Start fail!\r\n");
    
        while(1)
            ;
    }
#endif

#if 1 // for HRM
    if(E_APP_SRV_ERR_NONE != CC_AppSrv_Manager_Start(E_APP_SRV_ID_HRM))
    {
        TracerInfo("CC_AppSrv_Manager_Start fail!\r\n");
        
        while(1)
            ;
    }
#endif

#if defined (CC_APPSRV_SENSOR_TEST_MODE) && (CC_APPSRV_SENSOR_TEST_MODE == 1U)
        // TEST2,  for Swim
        if(E_APP_SRV_ERR_NONE != CC_AppSrv_Manager_Start(E_APP_SRV_ID_SWIM))
        {
            TracerInfo("CC_AppSrv_Manager_Start fail!\r\n");
        
            while(1)
                ;
        }
#endif


//    CC_AppSrv_Manager_Stop(E_APP_SRV_ID_SWIM);  // test!!!!!!!!!!!!!!


    Acc_Gyro_Dump_All_Settings();


    while(1)
    {
        static uint32_t test_mode_change_cnt = 0;
        static uint8_t test_mode = 0;

        TracerInfo("test mode: %d\r\n", test_mode);     

        if(test_mode_change_cnt == 1000) // about 10s
        {     
            test_mode = 1;
            
            CC_AppSrv_Manager_Stop(E_APP_SRV_ID_SWIM); 

            test_mode_change_cnt++;

            // only Swim, hrm/pedo are suspend
        }
        else if(test_mode_change_cnt == 2000) // about 20s
        {

            test_mode = 2;

            CC_AppSrv_Manager_Stop(E_APP_SRV_ID_HRM); 

            test_mode_change_cnt++;

            // Only Pedo
        }        
        else if(test_mode_change_cnt == 3000) // about 30s
        {

            test_mode = 3;

            CC_AppSrv_Manager_Stop(E_APP_SRV_ID_PEDO);

            test_mode_change_cnt++;

            test_mode_change_cnt = 0xffffffff; // stop mode change

            // close all
        }                
        else if(test_mode_change_cnt == 0xffffffff)
        {
            test_mode = 100;
            // do nothing;
        }
        else
            test_mode_change_cnt++;


#if 1 // for HRM    
        if(E_APP_SRV_ERR_NONE == CC_SenMgr_PPG_GetData(E_APP_SRV_ID_HRM, hrm_ppg_data, &hrm_ppg_size))
        {
            //TracerInfo("CC_SenMgr_Acc_GetData fail!\r\n");
        
            //while(1)
            //    ;

            if(0x00 != hrm_ppg_size)
            {
                TracerInfo("hrm_acc_data:\r\n");        

                for(uint16_t i = 0; i < hrm_ppg_size/sizeof(uint32_t); i+=3)
                {
                    TracerInfo("hrm_ppg_ch1: %d\r\n", hrm_ppg_data[i]);        
                    TracerInfo("hrm_ppg_ch2: %d\r\n", hrm_ppg_data[i+1]);
                    TracerInfo("hrm_ppg_ch3: %d\r\n", hrm_ppg_data[i+2]);                    
                }
                

                TracerInfo("\r\n");            
            }            
        }


#endif



        
#if 1 // for PEDO    
        if(E_APP_SRV_ERR_NONE == CC_SenMgr_Acc_GetData(E_APP_SRV_ID_PEDO, pedo_acc_data, &pedo_acc_size))
        {
            //TracerInfo("CC_SenMgr_Acc_GetData fail!\r\n");
        
            //while(1)
            //    ;

            if(0x00 != pedo_acc_size)
            {
                TracerInfo("pedo_acc_data:\r\n");        

                for(uint16_t i = 0; i < pedo_acc_size/sizeof(int16_t); i+=3)
                {
                    if( (pedo_acc_size < 6) 
                        || (0 != (pedo_acc_size % 6)))
                         TracerInfo("Error Size: %d\r\n", pedo_acc_size);
                
                
                    TracerInfo("pedo_accel_X: %d\r\n", pedo_acc_data[i]);
                    TracerInfo("pedo_accel_Y: %d\r\n", pedo_acc_data[i+1]);
                    TracerInfo("pedo_accel_Z: %d\r\n", pedo_acc_data[i+2]);        
                }
                

                TracerInfo("\r\n");            
            }            
        }


#endif
        

#if 1 // for Swim
        // ACCEL:
        if(E_APP_SRV_ERR_NONE == CC_SenMgr_Acc_GetData(E_APP_SRV_ID_SWIM, swim_acc_data, &swim_acc_size))
        {
            //TracerInfo("CC_SenMgr_Acc_GetData fail!\r\n");

            //while(1)
            //    ;

            if(0x00 != swim_acc_size)
            {
                TracerInfo("swim_acc_data:\r\n");        
            
                for(uint16_t i = 0; i < swim_acc_size/sizeof(int16_t); i+=3)
                {
                    if( (swim_acc_size < 6) 
                        || (0 != (swim_acc_size % 6)))
                         TracerInfo("Error Size: %d\r\n", swim_acc_size);
                
                
                    TracerInfo("swim_accel_X: %d\r\n", swim_acc_data[i]);
                    TracerInfo("swim_accel_Y: %d\r\n", swim_acc_data[i+1]);
                    TracerInfo("swim_accel_Z: %d\r\n", swim_acc_data[i+2]);        
                }
                
            
                TracerInfo("\r\n");            
            }
            
        }




        // Gyro:
        if(E_APP_SRV_ERR_NONE == CC_SenMgr_Gyro_GetData(E_APP_SRV_ID_SWIM, swim_gyro_data, &swim_gyro_size))
        {
            //TracerInfo("CC_SenMgr_Acc_GetData fail!\r\n");

            //while(1)
             //   ;

            if(0x00 != swim_gyro_size)
            {
                TracerInfo("swim_gyro_data:\r\n");        

                for(uint16_t i = 0; i < swim_gyro_size/sizeof(int16_t); i+=3)
                {
                    if( (swim_gyro_size < 6) 
                        || (0 != (swim_gyro_size % 6)))
                         TracerInfo("Error Size: %d\r\n", swim_gyro_size);
                
                
                    TracerInfo("swim_gyro_X: %d\r\n", swim_gyro_data[i]);
                    TracerInfo("swim_gyro_Y: %d\r\n", swim_gyro_data[i+1]);
                    TracerInfo("swim_gyro_Z: %d\r\n", swim_gyro_data[i+2]);        
                }
                

                TracerInfo("\r\n");            
            }            
        }




        // Mag:
        if(E_APP_SRV_ERR_NONE == CC_SenMgr_Mag_GetData(E_APP_SRV_ID_SWIM, (int16_t*)&mag_data, &mag_size))
        {
            //TracerInfo("CC_SenMgr_Acc_GetData fail!\r\n");

            //while(1)
              //  ;
              
            if(0x00 != mag_size)
            {
                TracerInfo("swim_mag_data:\r\n");        
               
                TracerInfo("mag_data_X: %d\r\n", mag_data.AXIS_X);
                TracerInfo("mag_data_Y: %d\r\n", mag_data.AXIS_Y);
                TracerInfo("mag_data_Z: %d\r\n", mag_data.AXIS_Z);                  

                TracerInfo("\r\n");            
            }                 
        }

     

#endif
        cc6801_ClockDelayMs(10);
       

    }
    
    

    if(E_APP_SRV_ERR_NONE != CC_AppSrv_Manager_Stop(E_APP_SRV_ID_PEDO))
    {
        TracerInfo("CC_AppSrv_Manager_Stop fail!\r\n");
    
        while(1)
            ;
    }



#endif
    
}


#endif

#endif
