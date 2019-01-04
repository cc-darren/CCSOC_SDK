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
*  task_table.c
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
*===========================================================================*/


/******************************************************************************
Head Block of The File
******************************************************************************/
// Sec 0: Comment block of the file

// Sec 1: Include File
#include "task_table.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous


/******************************************************************************
Declaration of External Variables & Functions
******************************************************************************/
// Sec 3: declaration of external variable


// Sec 4: declaration of external function prototype


/******************************************************************************
Declaration of data structure
******************************************************************************/
// Sec 5: structure, uniou, enum, linked list

/******************************************************************************
Declaration of Global Variables & Functions
******************************************************************************/
// Sec 6: declaration of global variable


// Sec 7: declaration of global function prototype

/******************************************************************************
Declaration of static Global Variables & Functions
******************************************************************************/
// Sec 8: declaration of static global variable


// Sec 9: declaration of static function prototype

/******************************************************************************
C Functions
******************************************************************************/
// Sec 10: C Functions


/******************************************************************************
* FUNCTION:
*   TaskInit
*
* DESCRIPTION:
*
* CALLS
*
* PARAMETERS
*
* RETURNS
*
* GLOBALS AFFECTED
*
******************************************************************************/
void TaskInit(void)
{
    uint32_t   index = 0;
    BaseType_t xReturned;

    while (NULL != g_atCCTaskPool[index].fpEntry)
    {
        xReturned = xTaskCreate(g_atCCTaskPool[index].fpEntry,
                                g_atCCTaskPool[index].caName,
                                g_atCCTaskPool[index].wStackSize,
                                NULL,
                                g_atCCTaskPool[index].dwPriority,
                                g_atCCTaskPool[index].tTaskHandle);
        
        
        
        if((pdPASS == xReturned) && (0 != g_atCCTaskPool[index].tQueueDepth) && (0 != g_atCCTaskPool[index].tQItemSize))
        {
            //create Queue
            *(g_atCCTaskPool[index].tQueueHandle) = xQueueCreate(g_atCCTaskPool[index].tQueueDepth,
                                                                 g_atCCTaskPool[index].tQItemSize);
        }

        index ++;
    }
}
