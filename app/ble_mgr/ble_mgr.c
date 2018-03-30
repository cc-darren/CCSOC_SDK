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

#include "rwble.h"
#include "app_task.h"
#include "drvi_eflash.h"
#include "app.h"


/******************************************************************************
 * DEFINITION / CONSTANT / ENUM / TYPE
 ******************************************************************************/
#pragma push
#pragma pack(1)
    
typedef struct 
{
    uint8_t  bd_addr[BD_ADDR_LEN];
    uint8_t  dev_name[APP_DEVICE_NAME_MAX_LEN];
} S_NVDS_Device_Info;
    
#pragma pop


/******************************************************************************
 * EXTERNAL FUNCTION
 ******************************************************************************/


/******************************************************************************
 *  VARIABLE
 ******************************************************************************/

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

volatile static bool bWaitForWriteFlashDone = false;


static void  flash_write_done_callback(uint32_t sys_evt)
{

    bWaitForWriteFlashDone = false;
}


void    jumptable_check_and_move(void)
{

    uint8_t empty_space[JUMP_TABLE_SIZE];
    #ifdef FPGA
    uint8_t empty_space2[JUMP_TABLE_SIZE];
    #endif
    
    for(uint32_t i = 0; i < JUMP_TABLE_SIZE; i++)
        empty_space[i] = 0xFF;
    #ifdef FPGA
    for(uint32_t i = 0; i < JUMP_TABLE_SIZE; i++)
        empty_space2[i] = 0x00;
    #endif
    
    #ifdef FPGA
    if((0x00 == memcmp(empty_space, (uint8_t*)JUMP_TABLE_SWITCH_ADDRESS, JUMP_TABLE_SIZE))
        ||(0x00 == memcmp(empty_space2, (uint8_t*)JUMP_TABLE_SWITCH_ADDRESS, JUMP_TABLE_SIZE)))
    #else
    if(0x00 == memcmp(empty_space, (uint8_t*)JUMP_TABLE_SWITCH_ADDRESS, JUMP_TABLE_SIZE))
    #endif
    {
         drvi_EflashRegisterCallback(flash_write_done_callback);

         drvi_EflashInit();


         bWaitForWriteFlashDone = true;

         drvi_EflashErasePage(0);

         while(bWaitForWriteFlashDone)
            ;


         bWaitForWriteFlashDone = true;
         
         drvi_EflashProgram(JUMP_TABLE_SWITCH_ADDRESS, (uint8_t*)jump_table_base, JUMP_TABLE_SIZE);
         
         while(bWaitForWriteFlashDone)
            ;
                  
         drvi_EflashFlush();         
    }

}


void  set_nvds_device_info(void)
{
    

    uint8_t bd_addr[BD_ADDR_LEN] = APP_DFLT_DEVICE_ADDR;
    uint8_t dev_name[APP_DFLT_DEVICE_NAME_LEN] = APP_DFLT_DEVICE_NAME;
    S_NVDS_Device_Info dev_info;

    memcpy(dev_info.bd_addr, bd_addr, BD_ADDR_LEN);
    memcpy(dev_info.dev_name, dev_name, APP_DFLT_DEVICE_NAME_LEN);


    drvi_EflashRegisterCallback(flash_write_done_callback);

    drvi_EflashInit();   


    bWaitForWriteFlashDone = true;

    drvi_EflashErasePage(0x1003F000);

    while(bWaitForWriteFlashDone)
       ;

    bWaitForWriteFlashDone = true;

    drvi_EflashProgram(0x1003F000, (uint8_t*)&dev_info, sizeof(S_NVDS_Device_Info));

    while(bWaitForWriteFlashDone)
       ;

    
    drvi_EflashFlush();   
}


#endif

/******************************************************************************
 * FUNCTION > APP_BLEMGR_Init
 ******************************************************************************/
void    APP_BLEMGR_Init(void)
{
    uint32_t error = 0;

    #ifndef BOOTLOADER
    #ifndef FPGA 
    jumptable_check_and_move();

    set_nvds_device_info();
    #endif
    #endif
    
    GLOBAL_INT_STOP();
            
    memset (((void *) 0x40006000), 0, 8192);
    memset (((void *) 0x20000048), 0, 0x820);   

    *((uint32_t *) 0x4000011C) = 0x00000008;
    #if (SYSTEM_CLOCK_MHZ == 24)
        *((uint32_t *) 0x40000104) = (*((uint32_t *) 0x40000104) & 0xFFFFFE0) | 0x03; // if SYSTEM_CLOCK_MHZ: 24MHz
    #elif (SYSTEM_CLOCK_MHZ == 32)
        *((uint32_t *) 0x40000104) = (*((uint32_t *) 0x40000104) & 0xFFFFFE0) | 0x04; // if SYSTEM_CLOCK_MHZ: 32MHz
    #endif
    
    rwip_init(error);
    
    GLOBAL_INT_START();

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
    rwble_isr();
}

#endif    //(CFG_BLE_APP)

