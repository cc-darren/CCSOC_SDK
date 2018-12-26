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
#include "task.h"
#include "queue.h"
#include "timers.h"
#include "ble_mgr.h"
#include "rwble.h"
#include "app.h"
#include "ff_stdio.h"
#include "ff_spidisk.h"

/* The number and size of sectors that will make up the flash disk. */
#define mainSPI_DISK_SECTOR_SIZE		512UL /* Currently fixed! */
#define mainSPI_DISK_SECTORS			( ( 32UL * 1024UL * 1024UL ) / mainSPI_DISK_SECTOR_SIZE ) /* 5M bytes. */
#define mainIO_MANAGER_CACHE_SIZE		( 15UL * mainSPI_DISK_SECTOR_SIZE )

/* Where the flash disks are mounted.  As set here the flash disk is the
root directory, and the flash disk appears as a directory off the root. */
#define mainSPI_DISK_NAME				"/spi"

static FF_Disk_t *pxSPIDisk;

extern void sys_InitMain(void);
extern void TaskInit(void);
extern QueueHandle_t qBleMgrSched;

static FF_Disk_t *pxSPIDisk;


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

void vTaskBleScheduleManger( void *pvParameters )
{
    uint8_t qdata;   

    #ifdef CFG_NVDS
    nvds_init((uint8_t *)0x1003F000, 2048);
    #endif

    APP_BLEMGR_Init();

    APP_SW_Timer_Init();

    NVIC_EnableIRQ(BLE_IRQn);
    
    while(1)
    {
        if ( pdTRUE == xQueueReceive(qBleMgrSched, &qdata, portMAX_DELAY))
        {
            rwip_schedule();            
        }
    }

}


void vTaskSensorTask(void *p)
{
       
    int16_t _wAccelData[3]  = { 0 };
    bool    bSensorInitDone = false;
   
    if(LSM6DS3_X_Init() == MEMS_ERROR)
    {
        bSensorInitDone = false;
        TracerInfo("Accelerometer initialize failed\r\n");        
    } 
    else 
    {
        bSensorInitDone = true;
        TracerInfo("Accelerometer initialize OK\r\n");        
        
        CC_LSM6DSX_AccelPowerDown();
        CC_LSM6DSX_GyroPowerDown();
        
        CC_LSM6DSX_AccelPowerON(LSM6DS3_ACC_GYRO_ODR_XL_52Hz);
        CC_LSM6DSX_GyroPowerON (LSM6DS3_ACC_GYRO_ODR_G_52Hz );        
    }
   
    #if  defined(EMWIN_LOAD_EXT_FLASH) && EMWIN_LOAD_EXT_FLASH
	/* Create the SPI flash disk. */
	pxSPIDisk = FF_SPIDiskInit( mainSPI_DISK_NAME, mainSPI_DISK_SECTORS );
	configASSERT( pxSPIDisk );

	/* Print out information on the SPI flash disk. */
	FF_SPIDiskShowPartition( pxSPIDisk );
    #endif
    
    
    while(1)
    {
        
        if(bSensorInitDone == true)
        {
            LSM6DS3_ACC_GYRO_GetRawAccData(NULL, (i16_t *) _wAccelData); 
            TracerInfo("Acc_X: %d, Acc_Y: %d, Acc_Z: %d\r\n", _wAccelData[0], _wAccelData[1], _wAccelData[2]);
        }
        
        vTaskDelay(pdMS_TO_TICKS(1000));
    }    
    
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
    //GLOBAL_INT_START(); 

    drvi_initialize();   
            
    //timerA_handle = xTimerCreate("timerA", (70/portTICK_PERIOD_MS), pdTRUE, 0, timerA_callback);
    //if(timerA_handle != NULL) xTimerStart(timerA_handle, 0);

    TaskInit();
    
    vTaskStartScheduler();
    
    while(1);
}

