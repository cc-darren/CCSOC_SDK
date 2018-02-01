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
*  sdk_scheduler.c
*
*  Project:
*  --------
*  cc6801
*
*  Description:
*  ------------
*  Emulator of Scheduler
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
#include <stdlib.h>
#include "sdk_scheduler.h"
#include "tracer.h"

#include "sw_timer.h"

/******************************************************************************
 * DEFINITION / CONSTANT / ENUM / TYPE
 ******************************************************************************/


/******************************************************************************
 * EXTERNAL FUNCTION
 ******************************************************************************/

/******************************************************************************
 *  VARIABLE
 ******************************************************************************/

/******************************************************************************
 * LOCAL FUNCTION > _SDK_WM_EventHandler
 ******************************************************************************/
void    _SDK_WM_EventHandler(S_AppSchedEvent *tEvent)
{
    TracerInfo(  "    [TC-04][_SDK_WM_EventHandler] EVENT(%d), D-SIZE(%d), @ 0x%08X\r\n"
               , tEvent->bEventID
               , tEvent->wDataByteSize
               , ((uint32_t) tEvent->vpData)                                            );

    if (tEvent->wDataByteSize)
    {
        uint8_t    _bIdx = 0;
        uint8_t   *_pbData = (uint8_t *)tEvent->vpData;

        for ( ; ; )
        {
            TracerInfo(  "        %d %d %d %d\r\n"
                       , _pbData[_bIdx+0]
                       , _pbData[_bIdx+1]
                       , _pbData[_bIdx+2]
                       , _pbData[_bIdx+3]         );

            _bIdx += 4;

            if (_bIdx > tEvent->wDataByteSize)
                return;
        }
    }
}

/******************************************************************************
 * LOCAL FUNCTION > _SDK_SVCMGR_EventHandler
 ******************************************************************************/
void    _SDK_SVCMGR_EventHandler(S_AppSchedEvent *tEvent)
{
    TracerInfo(  "    [TC-04][_SDK_SVCMGR_EventHandler] EVENT(%d), D-SIZE(%d), @ 0x%08X\r\n"
               , tEvent->bEventID
               , tEvent->wDataByteSize
               , ((uint32_t) tEvent->vpData)                                                );

    if (tEvent->wDataByteSize)
    {
        uint8_t    _bIdx = 0;
        uint8_t   *_pbData = (uint8_t *)tEvent->vpData;

        for ( ; ; )
        {
            TracerInfo(  "        %d %d %d %d\r\n"
                       , _pbData[_bIdx+0]
                       , _pbData[_bIdx+1]
                       , _pbData[_bIdx+2]
                       , _pbData[_bIdx+3]         );

            _bIdx += 4;

            if (_bIdx > tEvent->wDataByteSize)
                return;
        }
    }
}

/******************************************************************************
 * LOCAL FUNCTION > _SDK_SENSORMGR_EventHandler
 ******************************************************************************/
void    _SDK_SENSORMGR_EventHandler(S_AppSchedEvent *tEvent)
{
    TracerInfo(  "    [TC-04][_SDK_SENSORMGR_EventHandler] EVENT(%d), D-SIZE(%d), @ 0x%08X\r\n"
               , tEvent->bEventID
               , tEvent->wDataByteSize
               , ((uint32_t) tEvent->vpData)                                                   );

    if (tEvent->wDataByteSize)
    {
        uint8_t    _bIdx = 0;
        uint8_t   *_pbData = (uint8_t *)tEvent->vpData;

        for ( ; ; )
        {
            TracerInfo(  "        %d %d %d %d\r\n"
                       , _pbData[_bIdx+0]
                       , _pbData[_bIdx+1]
                       , _pbData[_bIdx+2]
                       , _pbData[_bIdx+3]         );

            _bIdx += 4;

            if (_bIdx > tEvent->wDataByteSize)
                return;
        }
    }
}

/******************************************************************************
 * LOCAL FUNCTION > _SDK_VTHREAD_Handler
 ******************************************************************************/
SW_TIMER_DEF(s_tAppSched_Timer_VThread);

void    _SDK_VTHREAD_Handler(void * pvContext)
{
    //int                _nReturnCode = 0;
    uint8_t           *_pbData      = NULL;
    S_AppSchedEvent    _tEvent;
    
    UNUSED_PARAMETER(pvContext);
    
    _tEvent.eModuleID     = E_APP_SCHED_MODID_SENSOR_MGR;
    _tEvent.bEventID      = 35;
    _tEvent.wDataByteSize = 7;

    _pbData = ((uint8_t *) malloc(_tEvent.wDataByteSize));

    // APP_SCHED_PostEvent() checks null-pointer case. However, better for users
    // to take care the exception when allocation fails...
    if (_pbData != NULL)
    {
        _pbData[0] = 41;
        _pbData[1] = 42;
        _pbData[2] = 43;
        _pbData[3] = 44;
        _pbData[4] = 45;
        _pbData[5] = 46;
        _pbData[6] = 47;
    }

    _tEvent.vpData = ((void *) _pbData);
    
    //TracerInfo("    [TC-05] POSTING by VTHREAD\r\n");
    //_nReturnCode = APP_SCHED_PostEvent(&_tEvent);
    //TracerInfo("    [TC-05] POST DONE (%d) by VTHREAD\r\n", _nReturnCode);
    APP_SCHED_PostEvent(&_tEvent);
}

/******************************************************************************
 * FUNCTION > SDK_APP_SCHED_EmulatorRun
 ******************************************************************************/
void    SDK_APP_SCHED_EmulatorRun(void)
{
    int                _nReturnCode = 0;
    uint8_t           *_pbData      = NULL;
    S_AppSchedEvent    _tEvent;

    /********************************************************/
    /* [TC-01] Initialization Check                         */
    /********************************************************/
    TracerInfo("[TC-01] INIT CHECK ================================\r\n");
    APP_SCHED_Init();
    
    /********************************************************/
    /* [TC-02] Event Handler Registration Check             */
    /********************************************************/
    TracerInfo("[TC-02] EVENT HANDLER REGISTRATION ================\r\n");
    APP_SCHED_RegEventHandler(E_APP_SCHED_MODID_WINDOW_MGR, _SDK_WM_EventHandler       );
    APP_SCHED_RegEventHandler(E_APP_SCHED_MODID_SENSOR_MGR, _SDK_SENSORMGR_EventHandler);
    
    /********************************************************/
    /* [TC-03] Event Post Check                             */
    /********************************************************/
    TracerInfo("[TC-03] POST TO MAKE BUFFER OVERFLOW ==============\r\n");
    {
        uint8_t    _bLoopIdx = 0;
    
        for (_bLoopIdx = 0; _bLoopIdx < 36; _bLoopIdx++)
        {
            _tEvent.eModuleID     = (E_AppSched_ModuleID)(_bLoopIdx % 3);
            _tEvent.bEventID      = _bLoopIdx;
            _tEvent.wDataByteSize = _bLoopIdx;
        
            if (0 == _tEvent.wDataByteSize)
            {
                _tEvent.vpData = NULL;
            }
            else
            {
                _pbData = ((uint8_t *) malloc(_bLoopIdx));

                if (_pbData != NULL)
                    memset(_pbData, _bLoopIdx, _bLoopIdx);

                _tEvent.vpData = (void *)_pbData;
            }
    
            _nReturnCode = APP_SCHED_PostEvent(&_tEvent);
            
            TracerInfo("    [TC-03] POST(%d), RET(%d)\r\n", _bLoopIdx, _nReturnCode);
        }
    }

    /********************************************************/
    /* [TC-04] Scheduler Run                                */
    /********************************************************/
    TracerInfo("[TC-04] RUN SCHEDULER =============================\r\n");
    APP_SCHED_RunScheduler();
    
    /********************************************************/
    /* [TC-05] Event Post by Multiple Access Simultaneously */
    /*     /CASE-1/ before critical section                 */
    /********************************************************/
    sw_timer_create(&s_tAppSched_Timer_VThread, SW_TIMER_MODE_SINGLE_SHOT, _SDK_VTHREAD_Handler);

    #ifdef _APP_SCHED_DEBUG_ON
        #if 1
            APP_SCHED_DbgSetTestCase(E_APP_SCHED_DBG_TC05_MULTI_USERS_POST_SIMUTANEOUSLY_C1);
            TracerInfo("[TC-05] RACE CONDITION - CASE 1\r\n");
        #else
            APP_SCHED_DbgSetTestCase(E_APP_SCHED_DBG_TC05_MULTI_USERS_POST_SIMUTANEOUSLY_C2);
            TracerInfo("[TC-05] RACE CONDITION - CASE 2\r\n");
        #endif
    #endif

    _tEvent.eModuleID     = E_APP_SCHED_MODID_WINDOW_MGR;
    _tEvent.bEventID      = 18;
    _tEvent.wDataByteSize = 3;

    _pbData = ((uint8_t *) malloc(_tEvent.wDataByteSize));

    // APP_SCHED_PostEvent() checks null pointer case. However, better for users
    // to take care the exception when allocation fails...
    if (_pbData != NULL)
    {
        _pbData[0] = 17;
        _pbData[1] = 18;
        _pbData[2] = 19;
    }

    _tEvent.vpData = ((void *) _pbData);
    
    sw_timer_start(s_tAppSched_Timer_VThread, SW_TIMER_TICKS(500, SW_TIMER_PRESCALER), NULL);
    
    //TracerInfo("    [TC-05] POSTING by SCHEDULER\r\n");

    _nReturnCode = APP_SCHED_PostEvent(&_tEvent);
    
    //TracerInfo("    [TC-05] POST DONE (%d) by SCHEDULER\r\n", _nReturnCode);

    APP_SCHED_RunScheduler();
    
    /********************************************************/
    /* [TC-06] Memory Release                               */
    /********************************************************/
    // already included in previous tests
    
    /********************************************************/
    /* [TC-07] Pressure Test                                */
    /********************************************************/
    // TBD
}

