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
#include "drvi_gpio.h"
#include "tracer.h"
#include "project.h"
#include "FreeRTOS.h"
#include "timers.h"

#define LED0    GPIO_PIN_0
#define LED1    GPIO_PIN_1
#define LED2    GPIO_PIN_2
#define LED3    GPIO_PIN_3

extern void sys_InitMain(void);

TimerHandle_t timerA_handle;

uint8_t k=0;
static const uint8_t running[6][4]=
{
    {1,0,0,0},
    {0,1,0,0},
    {0,0,1,0},
    {0,0,0,1},
    {0,0,1,0},
    {0,1,0,0}
};

void vApplicationIdleHook(void)
{

}

void vApplicationTickHook(void)
{

}

void vApplicationStackOverflowHook(TaskHandle_t xTask, signed char *pcTaskName)
{

}

void vApplicationMallocFailedHook(void)
{

}    


static void timerA_callback(void *p)
{
    (void)p;
    
    if(k>5) k=0;
    drvi_GpioWrite(LED0, running[k][0]);
    drvi_GpioWrite(LED1, running[k][1]);
    drvi_GpioWrite(LED2, running[k][2]);
    drvi_GpioWrite(LED3, running[k][3]);
    k++;
}

/*
 * MAIN FUNCTION
 ****************************************************************************************
 */

int main(void)
{
    //Must be first in main()
    sys_InitMain();

    //start interrupt handling
    GLOBAL_INT_START(); 

    drvi_initialize();

    timerA_handle = xTimerCreate("timerA", (70/portTICK_PERIOD_MS), pdTRUE, 0, timerA_callback);
    if(timerA_handle != NULL) xTimerStart(timerA_handle, 0);

    vTaskStartScheduler();
    while(1);
}

