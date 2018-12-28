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
*  ble_mgr.c
*
*  Project:
*  --------
*  cc6801
*
*  Description:
*  ------------
*  BLE Manager, taking care of,
*      (1) Handle CMD & RES between APK and self
*      (2) Interface between firmware and BLE stack
*
*  Author:
*  -------
*  CloudChip
*
*===========================================================================/
*  20180124 LOUIS initial version
******************************************************************************/

/******************************************************************************
 * INCLUDE FILE
 ******************************************************************************/
#include "ble_mgr.h"

#ifdef CFG_BLE_APP
#include <string.h>
#include "rwble.h"
#include "app_task.h"
#include "drvi_eflash.h"
#include "app.h"
#include "rwip.h"
#ifdef FREERTOS
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#endif


/******************************************************************************
 * DEFINITION / CONSTANT / ENUM / TYPE
 ******************************************************************************/
#pragma push
#pragma pack(1)

typedef struct
{

	#if (NVDS_SUPPORT)
    uint32_t  rsvd_for_magic_num;
	#endif
    uint8_t   bd_addr[BD_ADDR_LEN];
    uint8_t   dev_name[APP_DEVICE_NAME_MAX_LEN];
} S_NVDS_Device_Info;

#pragma pop


/******************************************************************************
 * EXTERNAL FUNCTION
 ******************************************************************************/


/******************************************************************************
 *  VARIABLE
 ******************************************************************************/
#ifdef FREERTOS
extern QueueHandle_t qBleMgrSched;
#endif
/******************************************************************************
 * FUNCTION > UART APIs (TODO: should link to real uart APIs)
 ******************************************************************************/
void app_ble_uart_read(uint8_t *bufptr, uint32_t size, rwip_eif_callback callback, void* dummy)
{
}

void app_ble_uart_write(uint8_t *bufptr, uint32_t size, rwip_eif_callback callback, void* dummy)
{
}

void app_ble_uart_flow_on(void)
{
}

bool app_ble_uart_flow_off(void)
{
    return (1);
}

const struct rwip_eif_api app_ble_uart_api =
{
    app_ble_uart_read,
    app_ble_uart_write,
    app_ble_uart_flow_on,
    app_ble_uart_flow_off,
};

/******************************************************************************
 * FUNCTION > rwip_eif_get (hook point requested by BLE stack)
 ******************************************************************************/
const struct rwip_eif_api* rwip_eif_get(uint8_t type)
{
    const struct rwip_eif_api* ret = NULL;
    
    
    switch(type)
    {
    case RWIP_EIF_AHI:
         ret = &app_ble_uart_api;
         break;
    
    #if (BLE_EMB_PRESENT) || (BT_EMB_PRESENT)
    case RWIP_EIF_HCIC:
         ret = &app_ble_uart_api;
         break;
    #elif !(BLE_EMB_PRESENT) || !(BT_EMB_PRESENT)
    case RWIP_EIF_HCIH:
         ret = &app_ble_uart2_api;
         break;
    #endif // (BLE_EMB_PRESENT)
    
    default:
         //ASSERT_INFO(0, type, 0);
         break;
    }

    return (ret);
}


#ifndef BOOTLOADER

void  set_nvds_device_info(void)
{


    uint8_t bd_addr[BD_ADDR_LEN] = APP_DFLT_DEVICE_ADDR;
    uint8_t dev_name[APP_DFLT_DEVICE_NAME_LEN] = APP_DFLT_DEVICE_NAME;

#if (NVDS_SUPPORT)
    nvds_put(NVDS_TAG_BD_ADDRESS, BD_ADDR_LEN, bd_addr);
    nvds_put(NVDS_TAG_DEVICE_NAME, APP_DFLT_DEVICE_NAME_LEN, dev_name);

#else
    S_NVDS_Device_Info dev_info;

    memcpy(dev_info.bd_addr, bd_addr, BD_ADDR_LEN);
    memcpy(dev_info.dev_name, dev_name, APP_DFLT_DEVICE_NAME_LEN);

    drvi_EflashInit();

    drvi_EflashErasePage(0x1003F000);

    drvi_EflashProgram(0x1003F000, (uint8_t*)&dev_info, sizeof(S_NVDS_Device_Info));

    drvi_EflashFlush();
#endif

}


#endif


/******************************************************************************
 * FUNCTION > APP_BLEMGR_Init
 ******************************************************************************/

void    APP_BLEMGR_Init(void)
{
    uint32_t error = 0;
    
    #ifdef CFG_NVDS
    nvds_init((uint8_t *)0x1003F000, 2048);
    #endif
    
    set_nvds_device_info();    
    
    memset (((void *) 0x40006000), 0, 8192);
	
    *((uint32_t *) 0x4000011C) = 0x00000008;
    *((uint32_t *) 0x40000104) = (*((uint32_t *) 0x40000104) & 0xFFFFFE0) | (SYSTEM_CLOCK_MHZ/8);
    
    rwip_init(error);
	
	NVIC_EnableIRQ(BLE_IRQn);
}


#ifdef SW_TIMER_BY_KERNEL
void    APP_SW_Timer_Init(void)
{

     while (APPM_ADVERTISING != ke_state_get(TASK_APP))
     {
            rwip_schedule();
     }
}
#endif


/******************************************************************************
 * FUNCTION > BLE_IRQHandler
 ******************************************************************************/

void    BLE_IRQHandler(void)
{    
    
    #ifdef FREERTOS
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    uint8_t qdata = 0;

    rwble_isr();

    xQueueSendFromISR( qBleMgrSched, &qdata, &xHigherPriorityTaskWoken );

    portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
    
    #else
    rwble_isr();
    #endif
}

#endif    //(CFG_BLE_APP)

