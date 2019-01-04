/* Copyright (c) 2019 Cloudchip, Inc. All Rights Reserved.
 *
 * The information contained herein is property of Cloudchip, Inc.
 * Terms and conditions of usage are described in detail in CLOUDCHIP
 * STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information.
 * NO WARRANTY of ANY KIND is provided. This heading must NOT be removed 
 * from the file.
 */

#ifndef _APP_SCHED_H_
#define _APP_SCHED_H_

/******************************************************************************
 * INCLUDE FILE
 ******************************************************************************/
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

 /******************************************************************************
 * DEFINITION / CONSTANT / ENUM / TYPE
 ******************************************************************************/
typedef enum
{
    E_APP_SCHED_MODID_WINDOW_MGR = 0,
    E_APP_SCHED_MODID_SERVICE_MGR,
    E_APP_SCHED_MODID_SENSOR_MGR,
    E_APP_SCHED_MODID_DUMMY_END
}   E_AppSched_ModuleID;

typedef struct
{
    E_AppSched_ModuleID    eModuleID;
    uint8_t                bEventID;
    uint16_t               wDataByteSize;
    void                  *vpData;
}   S_AppSchedEvent;

typedef void (*fpAppSchedEventHandler)(S_AppSchedEvent *tEvent);

/******************************************************************************
 * EXPORTED FUNCTION
 ******************************************************************************/

/******************************************************************************
 * [FUNCTION] APP_SCHED_Init
 *     Initialize APP Scheduler
 * [ARGUMENT] None.
 * [RETURN  ] None.
 ******************************************************************************/
extern void    APP_SCHED_Init(void);

/******************************************************************************
 * [FUNCTION] APP_SCHED_RegEventHandler
 *     Register the event handler of a module.
 * [ARGUMENT]
 *     <in> ptAppEvent: a pointer to the struct where below info. specified,
 *            .eModuleID     : ID of a module to handle events
 *            .fpEventHandler: A function pointer of the handler
 * [RETURN  ] None
 ******************************************************************************/
extern void    APP_SCHED_RegEventHandler(E_AppSched_ModuleID    eModuleID, 
                                         fpAppSchedEventHandler fpEventHandler);

/******************************************************************************
 * [FUNCTION] APP_SCHED_PostEvent
 *     Post an event to app framework. App scheduler will dispatch events to
 *     corresponding modules to handle the event.
 * [ARGUMENT]
 *     <in> ptAppEvent: a pointer to the struct where below info. specified,
 *            .eModuleID    : ID of the dest. module to handle this event
 *            .bEventID     : ID of the event, defined by each module
 *            .wDataByteSize: Byte-size of allocated buffer for additional data
 *            .vpData       : An pointer to allocated buf, where data is stored
 * [HIGHLIGHT]
 *     1. If no addtional data, wDataByteSize must be zero.
 *        And vpData can be used as 1/2/4-byte additional data.
 *     2. The allocated memory can be released by APP SCHEDULER ONLY!
 * [RETURN  ]
 *     The return code to specify the result of event post, listed below.
 ******************************************************************************/
#define APP_SCHED_POST_RESULT_OK              (0)
#define APP_SCHED_POST_RESULT_NULL_POINTER    (-1)
#define APP_SCHED_POST_RESULT_BUFFER_FULL     (-2)

extern int    APP_SCHED_PostEvent(S_AppSchedEvent *ptAppEvent);

/******************************************************************************
 * [FUNCTION] APP_SCHED_RunScheduler
 *     Call this scheduler to process all queued events. Allocated memory will
 *     be released by the scheduler automatically.
 * [ARGUMENT] None.
 * [RETURN  ] None.
 ******************************************************************************/
extern void    APP_SCHED_RunScheduler(void);






/******************************************************************************
 * [DEBUG CONFIGURATION]
 *     Config debug functiond
 ******************************************************************************/
//#define _APP_SCHED_PROFILE_ON
//#define _APP_SCHED_DEBUG_ON

#ifdef _APP_SCHED_DEBUG_ON
/******************************************************************************
 * [DBG FUNC] APP_SCHED_DbgSetTestCase
 *     Set test case to simulate conditions for validation.
 * [ARGUMENT] None.
 * [RETURN  ] None.
 ******************************************************************************/
typedef enum
{
    E_APP_SCHED_DBG_TC01_INIT,
    E_APP_SCHED_DBG_TC02_EVENT_HANDLER_REGISTRATION,
    E_APP_SCHED_DBG_TC03_EVENT_BUFFER_OVERFLOW,
    E_APP_SCHED_DBG_TC04_SCHEDULER_RUN,
    E_APP_SCHED_DBG_TC05_MULTI_USERS_POST_SIMUTANEOUSLY_C1,
    E_APP_SCHED_DBG_TC05_MULTI_USERS_POST_SIMUTANEOUSLY_C2,
    E_APP_SCHED_DBG_TCXX_DUMMY_END,
}   E_AppSchedDbg_TestCaseID;

extern void    APP_SCHED_DbgSetTestCase(E_AppSchedDbg_TestCaseID eTestID);

#endif    //(_APP_SCHED_DEBUG_ON)






#endif    //(_APP_SCHED_H_)
