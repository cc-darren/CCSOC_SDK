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
#define TASK_PRI_BLE            (2) /*ePrioTaskBLE*/
#define TASK_PRI_SEN            (1) /*ePrioTaskBLE*/


//##############################
// CC Task Stack Size constant #
//##############################
#define TASK_STK_BLE            (1580) /*STACK_SIZE_TASK_BLE*/
#define TASK_STK_SEN            (200)  /*STACK_SIZE_TASK_SEN*/

//#################################################
// CC Queue Size constant (elements of the queue) #
//#################################################
#define QUEUE_DEPTH_BLE          20


//######################################################
//CC Message Size constant (each element size, bytes)  #
//######################################################
#define Q_ITEM_SIZE_BLE          sizeof(uint8_t)


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
extern void vTaskBleScheduleManger(void*);
extern void vTaskSensorTask(void*);

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
TaskHandle_t task_ble_handle;       //BLE
TaskHandle_t task_sen_handle;       //Sensor

QueueHandle_t qBleMgrSched=NULL;


// Sec 7: declaration of global function prototype

/******************************************************************************
Declaration of static Global Variables & Functions
******************************************************************************/
// Sec 8: declaration of static global variable

T_S_CCTask g_atCCTaskPool[]=
{
// TaskName     TaskHandle           TaskFuncEntry                TaskStack         TaskPriority    QueueHandle         QueueDepth           QItemSize
//----------------------------------------------------------------------------------------------------------------------------------------------------
  {"BleTask" , &task_ble_handle,     vTaskBleScheduleManger,      TASK_STK_BLE,     TASK_PRI_BLE,   &qBleMgrSched,      QUEUE_DEPTH_BLE,     Q_ITEM_SIZE_BLE},
  {"SenTask" , &task_sen_handle,     vTaskSensorTask,             TASK_STK_SEN,     TASK_PRI_SEN,   NULL,               0,                   0},  
//
//Follow above format to add your TASK here
//
//----------------------------------------------------------------------------------------------------------------------------------------------------
  {NULL,         NULL,               NULL,              0,                0,              NULL,              0,                   0                } //end of task, don't remove it
//----------------------------------------------------------------------------------------------------------------------------------------------------
};


// Sec 9: declaration of static function prototype

#endif // _TASK_TABLE_H
