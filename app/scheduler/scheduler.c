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
*  scheduler.c
*
*  Project:
*  --------
*  cc6801
*
*  Description:
*  ------------
*  Scheduler of APPLICATION
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
#include "scheduler.h"
#include "tracer.h"

/******************************************************************************
 * DEFINITION / CONSTANT / ENUM / TYPE
 ******************************************************************************/
#define _APP_SCHED_QUEUE_SIZE    (32)

typedef struct
{
    S_AppSchedEvent    taEvent[_APP_SCHED_QUEUE_SIZE];
    uint16_t           wIdxWrite;
    uint16_t           wIdxRead;
}   S_AppSchedEventQueue;

/******************************************************************************
 * EXTERNAL FUNCTION
 ******************************************************************************/

/******************************************************************************
 *  VARIABLE
 ******************************************************************************/
S_AppSchedEventQueue      s_tQueue;
fpAppSchedEventHandler    s_fpaEventHandler[E_APP_SCHED_MODID_DUMMY_END];

#ifdef _APP_SCHED_DEBUG_ON
    E_AppSchedDbg_TestCaseID    s_eDbgTestID;
    volatile uint8_t            s_bDbgEntranceCount;
#endif

#ifdef _APP_SCHED_PROFILE_ON
    volatile uint16_t    s_wDbgMaxQueueInUse;
#endif

/******************************************************************************
 * LOCAL FUNCTION > _FreeDataBuffer
 ******************************************************************************/
static void    _FreeDataBuffer(S_AppSchedEvent *ptAppEvent)
{
    #ifdef _APP_SCHED_DEBUG_ON
        TracerInfo(  "        [_FreeDataBuffer] wDataByteSize: %d, vpData: 0x%08X\r\n"
                   , ptAppEvent->wDataByteSize
                   , ((uint32_t) ptAppEvent->vpData)                                  );
    #endif

    if (ptAppEvent->wDataByteSize > 0)
    {
        if (ptAppEvent->vpData != NULL)
        {
            free(ptAppEvent->vpData);
        }
    }
}

/******************************************************************************
 * DEBUG FUNCTION > APP_SCHED_Init
 ******************************************************************************/
#ifdef _APP_SCHED_DEBUG_ON
void    APP_SCHED_DbgSetTestCase(E_AppSchedDbg_TestCaseID eTestID)
{
    s_eDbgTestID        = eTestID;
    s_bDbgEntranceCount = 0;
}
#endif

/******************************************************************************
 * FUNCTION > APP_SCHED_Init
 ******************************************************************************/
void    APP_SCHED_Init(void)
{
    memset(&s_tQueue         , 0, sizeof(s_tQueue)         );
    memset(&s_fpaEventHandler, 0, sizeof(s_fpaEventHandler));

    #ifdef _APP_SCHED_DEBUG_ON
        s_eDbgTestID        = E_APP_SCHED_DBG_TCXX_DUMMY_END;
        s_bDbgEntranceCount = 0;
    #endif

    #ifdef _APP_SCHED_PROFILE_ON
        s_wDbgMaxQueueInUse = 0;
    #endif
}

/******************************************************************************
 * FUNCTION > APP_SCHED_RegEventHandler
 ******************************************************************************/
void    APP_SCHED_RegEventHandler(E_AppSched_ModuleID    eModuleID,
                                  fpAppSchedEventHandler fpEventHandler)
{
    if (eModuleID >= E_APP_SCHED_MODID_DUMMY_END)
        return;

    s_fpaEventHandler[eModuleID] = fpEventHandler;
}

/******************************************************************************
 * FUNCTION > APP_SCHED_PostEvent
 ******************************************************************************/
int    APP_SCHED_PostEvent(S_AppSchedEvent *ptAppEvent)
{
    #define _POST_NO_AVAILABLE_BUFFER    (0xFF)

    volatile uint8_t    _bIdxNext = _POST_NO_AVAILABLE_BUFFER;

    if (ptAppEvent->wDataByteSize > 0)
    {
        if (NULL == ptAppEvent->vpData)
        {
            #ifdef _APP_SCHED_DEBUG_ON
                TracerErr("[APP_SCHED_PostEvent] ERR: Fail to alloc mem\r\n");
            #endif

            return (APP_SCHED_POST_RESULT_NULL_POINTER);
        }
    }

    #ifdef _APP_SCHED_DEBUG_ON
        s_bDbgEntranceCount++;

        if (E_APP_SCHED_DBG_TC05_MULTI_USERS_POST_SIMUTANEOUSLY_C1 == s_eDbgTestID)
            while (2 > s_bDbgEntranceCount)    { /* wait until condition unlocked */ }

        //if (   (E_APP_SCHED_DBG_TC05_MULTI_USERS_POST_SIMUTANEOUSLY_C1 == s_eDbgTestID)
        //    || (E_APP_SCHED_DBG_TC05_MULTI_USERS_POST_SIMUTANEOUSLY_C2 == s_eDbgTestID)
        //   )
        //{
        //    TracerInfo(  "[APP_SCHED_PostEvent-1] EntraceCount = %d, ModID = %d\r\n"
        //               , s_bDbgEntranceCount
        //               , ptAppEvent->eModuleID                                      );
        //}
    #endif

    GLOBAL_INT_DISABLE();

    #ifdef _APP_SCHED_DEBUG_ON
        if (E_APP_SCHED_DBG_TC05_MULTI_USERS_POST_SIMUTANEOUSLY_C2 == s_eDbgTestID)
            while (2 > s_bDbgEntranceCount)    { /* wait until condition unlocked */ }

        //if (   (E_APP_SCHED_DBG_TC05_MULTI_USERS_POST_SIMUTANEOUSLY_C1 == s_eDbgTestID)
        //    || (E_APP_SCHED_DBG_TC05_MULTI_USERS_POST_SIMUTANEOUSLY_C2 == s_eDbgTestID)
        //   )
        //{
        //    TracerInfo(  "[APP_SCHED_PostEvent-2] EntraceCount = %d, ModID = %d\r\n"
        //               , s_bDbgEntranceCount
        //               , ptAppEvent->eModuleID                                      );
        //}
    #endif

    _bIdxNext = ((s_tQueue.wIdxWrite + 1) % _APP_SCHED_QUEUE_SIZE);

    if (_bIdxNext == s_tQueue.wIdxRead)
    {
        _bIdxNext = _POST_NO_AVAILABLE_BUFFER;

        goto _PostExit;
    }
    else
    {
        s_tQueue.wIdxWrite = _bIdxNext;

        #ifdef _APP_SCHED_PROFILE_ON
        {
            volatile uint16_t    _wDistance = 0;

            if (s_tQueue.wIdxWrite > s_tQueue.wIdxRead)
                _wDistance = (s_tQueue.wIdxWrite - s_tQueue.wIdxRead);
            else
                _wDistance = ((s_tQueue.wIdxWrite + _APP_SCHED_QUEUE_SIZE) - s_tQueue.wIdxRead);

            if (_wDistance > s_wDbgMaxQueueInUse)
            {
                s_wDbgMaxQueueInUse = _wDistance;

                //TracerInfo(  "[APP_SCHED_PostEvent] s_wDbgMaxQueueInUse =  %d\r\n"
                //           , s_wDbgMaxQueueInUse                                  );
            }
        }
        #endif
    }

_PostExit:
    GLOBAL_INT_RESTORE();

    if (_POST_NO_AVAILABLE_BUFFER == _bIdxNext)
    {
        _FreeDataBuffer(ptAppEvent);
        return (APP_SCHED_POST_RESULT_BUFFER_FULL);
    }

    if (0 == _bIdxNext)
    {
        s_tQueue.taEvent[(_APP_SCHED_QUEUE_SIZE - 1)] = *ptAppEvent;

        //#ifdef _APP_SCHED_DEBUG_ON
        //    TracerInfo(  "[APP_SCHED_PostEvent-F] WriteIdx = %d <<< 0x%08X\r\n"
        //               , (_APP_SCHED_QUEUE_SIZE - 1)
        //               , ((uint32_t) ptAppEvent->vpData)                       );
        //#endif
    }
    else
    {
        s_tQueue.taEvent[(_bIdxNext - 1)] = *ptAppEvent;

        //#ifdef _APP_SCHED_DEBUG_ON
        //    TracerInfo(  "[APP_SCHED_PostEvent-F] WriteIdx = %d <<< 0x%08X\r\n"
        //               , (_bIdxNext - 1)
        //               , ((uint32_t) ptAppEvent->vpData)                       );
        //#endif
    }

    return (APP_SCHED_POST_RESULT_OK);
}

/******************************************************************************
 * FUNCTION > APP_SCHED_RunScheduler
 ******************************************************************************/
void    APP_SCHED_RunScheduler(void)
{
    while (1)
    {
        if (s_tQueue.wIdxRead == s_tQueue.wIdxWrite)
            return;

        if (s_fpaEventHandler[s_tQueue.taEvent[s_tQueue.wIdxRead].eModuleID])
            s_fpaEventHandler[s_tQueue.taEvent[s_tQueue.wIdxRead].eModuleID](&s_tQueue.taEvent[s_tQueue.wIdxRead]);

        _FreeDataBuffer(&s_tQueue.taEvent[s_tQueue.wIdxRead]);

        s_tQueue.wIdxRead = ((s_tQueue.wIdxRead + 1) % _APP_SCHED_QUEUE_SIZE);
    }
}

