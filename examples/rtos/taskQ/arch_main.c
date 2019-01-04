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

/*
 * INCLUDES
 ****************************************************************************************
 */
#include <string.h>
#include "ll.h"
#include "drvi_init.h"
#include "tracer.h"
#include "project.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "queue.h"
#include "semphr.h"

extern void sys_InitMain(void);
extern void TaskInit(void);

/*
 * MAIN FUNCTION
 ****************************************************************************************
 */

volatile uint32_t task1cnt = 0;
volatile uint32_t task2cnt = 0;
volatile uint32_t task3cnt = 0;
volatile uint32_t task4cnt = 0;
volatile uint32_t ostick = 0;
volatile uint32_t idlecnt = 0;
volatile uint32_t timerAcnt = 0;
volatile uint32_t timerBcnt = 0;
volatile uint32_t overflow = 0;
volatile uint32_t mallocfailure = 0;

volatile BaseType_t tA1Stack, tB1Stack, t3Stack;

extern TaskHandle_t T_handle_Test1;
extern TaskHandle_t T_handle_Test2;
extern TaskHandle_t T_handle_Test3;
extern TaskHandle_t T_handle_Test4;

TimerHandle_t timerA_handle, timerB_handle;

extern QueueHandle_t Q_handle_Test1;
extern QueueHandle_t Q_handle_Test2;
extern QueueHandle_t Q_handle_Test3;
extern QueueHandle_t Q_handle_Test4;

SemaphoreHandle_t sema1;

uint32_t messA1 = 0x10;
uint32_t messB1 = 0x20;

void vApplicationIdleHook(void)
{
    idlecnt++;
}

void vApplicationTickHook(void)
{
    ostick++;
}

void vApplicationStackOverflowHook(TaskHandle_t xTask, signed char *pcTaskName)
{
    char name[10];
    strcpy(name, (const char *)pcTaskName);
    overflow++;
}

void vApplicationMallocFailedHook(void)
{
    mallocfailure++;
}    

void taskFuncTest1(void *p)
{
    (void)p;
    uint32_t qdata;
    
    xSemaphoreTake(sema1, 0);
    while(1)    
    {        
        //vTaskSuspend(NULL);
        //vTaskDelay(1000/portTICK_PERIOD_MS);
        xQueueReceive(Q_handle_Test1, &qdata, portMAX_DELAY);

        if(qdata == 0x10)
        {
            task1cnt++;    
        }
        
        TracerInfo("task1cnt = %d\r\n", task1cnt);

        tA1Stack = uxTaskGetStackHighWaterMark(NULL);
        t3Stack = uxTaskGetStackHighWaterMark(T_handle_Test3);
        
    }
}

void taskFuncTest2(void *p)
{
    (void)p;
    uint32_t qdata;
    
    xSemaphoreTake(sema1, 1);
    while(1)
    {        
        //vTaskSuspend(NULL);
        xQueueReceive(Q_handle_Test2, &qdata, portMAX_DELAY);
        
        if(qdata == 0x20)
        {
            task2cnt++;
        }

        
        if((task2cnt%10) == 0)
        {
            xSemaphoreGive(sema1);
        }
        else
        {
            xSemaphoreTake(sema1, 1);
        }
        
        TracerInfo("task2cnt = %d\r\n", task2cnt);
        tB1Stack = uxTaskGetStackHighWaterMark(NULL);
    }
}

void taskFuncTest3(void *p)
{
    (void)p;
    
    while(1)
    {
        xSemaphoreTake(sema1, portMAX_DELAY);
        task3cnt++;
        xSemaphoreGive(sema1);
        vTaskDelay(1000/portTICK_PERIOD_MS);
        TracerInfo("task3cnt = %d\r\n", task3cnt);
    }
    
}

void taskFuncTest4(void *p)
{
    (void)p;
    
    while(1)
    {
        vTaskDelay(6000/portTICK_PERIOD_MS);
        TracerInfo("task4cnt = %d\r\n", task4cnt++);
    }
    
}

static void timerA_callback(void *p)
{
    (void)p;
    timerAcnt++;
    xQueueSend(Q_handle_Test1, &messA1, 0); //don't wait if Q is full
    //vTaskResume(tA1_handle);
}

static void timerB_callback(void *p)
{
    (void)p;
    timerBcnt++;
    xQueueSend(Q_handle_Test2, &messB1, 0); //don't wait if Q is full
    //vTaskResume(tB1_handle);
}

int main(void)
{
    //Must be first in main()
    sys_InitMain();

    //start interrupt handling
    //GLOBAL_INT_START();
    //NVIC_SetPriorityGrouping( 0 );

    drvi_initialize();

    //TracerInfo("== CC6801 Start ==\r\n");

    TaskInit();
    
    /*******************************/
    /****** Application Start ******/
    /*******************************/
/*    
    xTaskCreate(taskFuncTest1, "TK_TEST1", 80, NULL, 2, &T_handle_Test1);
    xTaskCreate(taskFuncTest2, "TK_TEST2", 80, NULL, 4, &T_handle_Test2);
    xTaskCreate(taskFuncTest3, "TK_TEST3", 80, NULL, 6, &T_handle_Test3);
    xTaskCreate(taskFuncTest4, "TK_TEST4", 80, NULL, 8, &T_handle_Test4);
    
    Q_handle_Test1 = xQueueCreate(4, sizeof(uint32_t));
    Q_handle_Test2 = xQueueCreate(4, sizeof(uint32_t));
    Q_handle_Test3 = xQueueCreate(4, sizeof(uint32_t));
    Q_handle_Test4 = xQueueCreate(4, sizeof(uint32_t));
*/

    timerA_handle = xTimerCreate("timerA", (2000/portTICK_PERIOD_MS), pdTRUE, 0, timerA_callback);
    if(timerA_handle != NULL) xTimerStart(timerA_handle, 0);
    timerB_handle = xTimerCreate("timerB", (4000/portTICK_PERIOD_MS), pdTRUE, 0, timerB_callback);
    if(timerB_handle != NULL) xTimerStart(timerB_handle, 0);

    sema1 = xSemaphoreCreateCounting(2, 0);

    vTaskStartScheduler();
    
    while(1)
    {

    }
    //return(0);
}

