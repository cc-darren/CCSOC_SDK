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
    
    #ifndef CFG_JUMP_TABLE_2
        if (jump_table2_struct[JT_POS_FUNC_EIF_GET] != 0)
        {
            typedef struct rwip_eif_api* (*my_function)(uint8_t type);
            
            return ((my_function) (jump_table2_struct[JT_POS_FUNC_EIF_GET]))(type);
        }
    #endif
    
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

/******************************************************************************
 * FUNCTION > APP_BLEMGR_Init
 ******************************************************************************/
void    APP_BLEMGR_Init(void)
{
    uint32_t error = 0;
    
    GLOBAL_INT_STOP();
    
    #ifndef CFG_JUMP_TABLE_2
        memset(((void *) JUMP_TABLE_2_BASE_ADDR), 0, 1024);
    #endif
    
    memset (((void *) 0x40006000), 0, 8192);
    memset (((void *) 0x20000048), 0, 0x820);   

    *((uint32_t *) 0x4000011C) = 0x00000008;
    *((uint32_t *) 0x40000104) = (*((uint32_t *) 0x40000104) & 0xFFFFFE0) | 0x04;
    
    rwip_init(error);
    
    GLOBAL_INT_START();
    
    #ifdef SW_TIMER_BY_KERNEL
        while (APPM_ADVERTISING != ke_state_get(TASK_APP))    
        {
            rwip_schedule();
        }
    #endif
}

/******************************************************************************
 * FUNCTION > APP_BLEMGR_IRQHandler
 ******************************************************************************/
void    APP_BLEMGR_IRQHandler(void)
{    
    rwble_isr();
}

#endif    //(CFG_BLE_APP)

