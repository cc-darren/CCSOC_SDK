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

/******************************************************************************
*  Filename:
*  ---------
*  task_table.h
*
*  Project:
*  --------
*  cc6801
*
*  Description:
*  ------------
*  Define RTOS task priority, stack size, name...
*  Create a table to store and list all tasks. User can easily add/remove/update tasks
*
*  Author:
*  -------
*  PPP      (cc-pattaya)
*
*===========================================================================

******************************************************************************/

#ifndef _TASK_TABLE_H
#define _TASK_TABLE_H

/******************************************************************************
Head Block of The File
******************************************************************************/
// Sec 0: Comment block of the file

// Sec 1: Include File
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous

//############################
// CC Task Priority constant #
//############################
#define TASK_PRI_TEST1          (2)
#define TASK_PRI_TEST2          (4)
#define TASK_PRI_TEST3          (6)
#define TASK_PRI_TEST4          (8)


//##############################
// CC Task Stack Size constant #
//##############################
#define TASK_STK_SIZE           (128)

#define TASK_STK_TEST1          TASK_STK_SIZE
#define TASK_STK_TEST2          TASK_STK_SIZE
#define TASK_STK_TEST3          TASK_STK_SIZE
#define TASK_STK_TEST4          TASK_STK_SIZE

//#################################################
// CC Queue Size constant (elements of the queue) #
//#################################################
#define QUEUE_DEPTH_TEST1        4
#define QUEUE_DEPTH_TEST2        4
#define QUEUE_DEPTH_TEST3        4
#define QUEUE_DEPTH_TEST4        4

//######################################################
//CC Message Size constant (each element size, bytes)  #
//######################################################
#define Q_ITEM_SIZE_TEST1        sizeof(void*)
#define Q_ITEM_SIZE_TEST2        sizeof(void*)
#define Q_ITEM_SIZE_TEST3        sizeof(void*)
#define Q_ITEM_SIZE_TEST4        sizeof(void*)

//######################################################
//Other System Definition and Checking                 #
//######################################################



/******************************************************************************
Declaration of External Variables & Functions
******************************************************************************/
// Sec 3: declaration of external variable

// Sec 4: declaration of external function prototype

//#####################
// TASK MAIN FUNCTION #
//#####################
extern void taskFuncTest1(void*);
extern void taskFuncTest2(void*);
extern void taskFuncTest3(void*);
extern void taskFuncTest4(void*);

/******************************************************************************
Declaration of data structure
******************************************************************************/
// Sec 5: structure, uniou, enum, linked list
typedef struct
{
    //parameters for creating tasks
    const char caName[configMAX_TASK_NAME_LEN]; //now, len=8

    TaskHandle_t *tTaskHandle;
    TaskFunction_t    fpEntry;
    const configSTACK_DEPTH_TYPE     wStackSize;
    UBaseType_t   dwPriority;

    //parameters for creating queue
    QueueHandle_t       *tQueueHandle;
    const UBaseType_t   tQueueDepth;
    const UBaseType_t   tQItemSize;
    
}T_S_CCTask;

/******************************************************************************
Declaration of Global Variables & Functions
******************************************************************************/
// Sec 6: declaration of global variable
TaskHandle_t T_handle_Test1;
TaskHandle_t T_handle_Test2;
TaskHandle_t T_handle_Test3;
TaskHandle_t T_handle_Test4;

QueueHandle_t Q_handle_Test1=NULL;
QueueHandle_t Q_handle_Test2=NULL;
QueueHandle_t Q_handle_Test3=NULL;
QueueHandle_t Q_handle_Test4=NULL;

// Sec 7: declaration of global function prototype

/******************************************************************************
Declaration of static Global Variables & Functions
******************************************************************************/
// Sec 8: declaration of static global variable

T_S_CCTask g_atCCTaskPool[]=
{
// TaskName        TaskHandle      TaskFuncEntry      TaskStack         TaskPriority    QueueHandle        QueueDepth           QItemSize
//----------------------------------------------------------------------------------------------------------------------------------------------------
  {"TK_TEST1", &T_handle_Test1,    taskFuncTest1,     TASK_STK_TEST1,   TASK_PRI_TEST1, &Q_handle_Test1,    QUEUE_DEPTH_TEST1,   Q_ITEM_SIZE_TEST1},
  {"TK_TEST2", &T_handle_Test2,    taskFuncTest2,     TASK_STK_TEST2,   TASK_PRI_TEST2, &Q_handle_Test2,    QUEUE_DEPTH_TEST2,   Q_ITEM_SIZE_TEST2},
  {"TK_TEST3", &T_handle_Test3,    taskFuncTest3,     TASK_STK_TEST3,   TASK_PRI_TEST3, &Q_handle_Test3,    QUEUE_DEPTH_TEST3,   Q_ITEM_SIZE_TEST3},
  {"TK_TEST4", &T_handle_Test4,    taskFuncTest4,     TASK_STK_TEST4,   TASK_PRI_TEST4, &Q_handle_Test4,    QUEUE_DEPTH_TEST4 ,  Q_ITEM_SIZE_TEST4},

//
//Follow above format to add your TASK here
//
//----------------------------------------------------------------------------------------------------------------------------------------------------
  {NULL,         NULL,               NULL,              0,                0,              NULL,              0,                   0                } //end of task, don't remove it
//----------------------------------------------------------------------------------------------------------------------------------------------------
};


// Sec 9: declaration of static function prototype

#endif // _TASK_TABLE_H

