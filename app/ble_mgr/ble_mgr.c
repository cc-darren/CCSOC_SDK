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

#define BLE_CONN_PATCH_EN
#ifdef BLE_CONN_PATCH_EN
#include "ke_config.h"        
#include "ke_task.h"          
#include "ke_env.h"           
#include "ke_queue.h"         
#include "ke_event.h"         
#include "co_list.h"
#include "ke_msg.h"
#include "llc.h"
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
#ifdef BLE_CONN_PATCH_EN
ke_msg_func_t ke_task_handler_get(ke_msg_id_t const msg_id, ke_task_id_t const task_id);
void lld_con_update_ind(struct ea_elt_tag *elt_old,
                        struct llcp_con_upd_ind const *param_pdu);
void ke_event_clear(uint8_t event_type);
uint8_t ke_event_callback_set(uint8_t event_type, void (*p_callback)(void));
bool ke_is_free(void* mem_ptr);
void ke_msg_free(struct ke_msg const *msg);
#endif

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
    uint8_t dev_name_size = APP_DFLT_DEVICE_NAME_LEN;
    
#if (NVDS_SUPPORT)
    nvds_put(NVDS_TAG_BD_ADDRESS, BD_ADDR_LEN, bd_addr);
    nvds_put(NVDS_TAG_DEVICE_NAME, APP_DFLT_DEVICE_NAME_LEN, dev_name);
    nvds_put(NVDS_TAG_DEV_NAME_LEN, 1, &dev_name_size);
    
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


#ifdef BLE_CONN_PATCH_EN
void    APP_BLE_Task_Schedule_Patch(void)
{
    
    do
    {
        int msg_status;
        struct ke_msg *msg;
        // Get a message from the queue
        GLOBAL_INT_DISABLE();
        msg = (struct ke_msg*) ke_queue_pop(&ke_env.queue_sent);
        GLOBAL_INT_RESTORE();
        if (msg == NULL) break;
        // Mark that message is no more in message queue
        msg->hdr.next = ((struct co_list_hdr_t *)KE_MSG_NOT_IN_QUEUE);

        // check if message already free
        if(ke_is_free(msg))
        {
            break;
        }

        // Retrieve a pointer to the task instance data
        ke_msg_func_t func = ke_task_handler_get(msg->id, msg->dest_id);

        // Call the message handler
        if (func != NULL)
        {
            if(LLC_LLCP_RECV_IND == msg->id)
            {
                //struct llc_llcp_recv_ind* msg = LLC_GET_BASE_MSG(param);
                
                struct llc_llcp_recv_ind *ind = ke_msg2param(msg);

                
                if(ind->pdu.opcode == LLCP_CONNECTION_UPDATE_IND_OPCODE)                
                {
                    uint8_t tmp = ind->pdu.con_up_req.win_size;
                    ind->pdu.con_up_req.win_size = 0;

                    struct llc_env_tag *llc_env_ptr = llc_env[0];
                    lld_con_update_ind(llc_env_ptr->elt, &ind->pdu.con_up_req);

                    ind->pdu.con_up_req.win_size = tmp;
                }
                
            }
            
            
            msg_status = func(msg->id, ke_msg2param(msg), msg->dest_id, msg->src_id);
        }
        else
        {
            msg_status = KE_MSG_CONSUMED;
        }

        switch (msg_status)
        {
        case KE_MSG_CONSUMED:
            // Free the message
            ke_msg_free(msg);
            break;

        case KE_MSG_NO_FREE:
            break;

        case KE_MSG_SAVED:
            // The message has been saved
            // Insert it at the end of the save queue
            ke_queue_push(&ke_env.queue_saved, (struct co_list_hdr*) msg);
            break;

        default:
            break;
        } // switch case
    } while(0);

    // Verify if we can clear the event bit
    GLOBAL_INT_DISABLE();
    if (co_list_is_empty(&ke_env.queue_sent))
        ke_event_clear(KE_EVENT_KE_MESSAGE);
    GLOBAL_INT_RESTORE();


}


void    APP_BLE_Patch_Init(void)
{

    ke_event_callback_set(KE_EVENT_KE_MESSAGE, APP_BLE_Task_Schedule_Patch);
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
	
    #ifdef BLE_CONN_PATCH_EN
    APP_BLE_Patch_Init();
    #endif

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

