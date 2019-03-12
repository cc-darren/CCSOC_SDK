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
#include "drvi_init.h"
#include "drvi_gpio.h"
#include "tracer.h"
#include "project.h"
#include "ble_mgr.h"
#include "app_bleuarts.h"
#include "stdio.h"

/**
 *
 * UART over BLE application main file.
 *
 * This file contains the source code for a sample application that uses the BLE UART service.
 *
 */

extern void sys_InitMain(void);

#define UART_BUFFER_LEN	256
static uint8_t	g_UrBuf[UART_BUFFER_LEN] = {0};
static uint32_t g_len = 0;
static bool		g_ready = false;

/**
* @brief       This function is UART interrupt handler
* @param[in]   N/A
* @return      N/A
*/
void uart_rx_handler(uint8_t *ptr, uint16_t length)
{
	g_UrBuf[g_len++] = regUART0CTRL->dw.bufRx;
			
	if ((g_UrBuf[g_len-2]==0x0D) && (g_UrBuf[g_len-1]==0x0A))
	{
		g_ready = true;
	}
}

/**
* @brief       This function used to check data is ready or not
* @param[in]   N/A
* @return      bool: false: data not ready, true: data ready
**/
bool uart_cmd_is_ready(void)
{
	return g_ready;
}

/**
* @brief       This function used to get data from buffer
* @param[in]   ptr: Pointer to a data buffer
* @param[in]   length: data length
* @return      N/A
**/
uint16_t uart_buf_get(uint8_t *ptr)
{
	uint8_t len = g_len;
	NVIC_DisableIRQ(UART0_IP_IRQn);
	
	memcpy(ptr, (uint8_t*)&g_UrBuf[0], g_len);
	g_len = 0;
	g_ready = false;
	
	NVIC_EnableIRQ(UART0_IP_IRQn);
	
	return len;
}

/**
* @brief       This function will be called when BLE received datas
* @param[in/out]   rx_data: Pointer to a data buffer
* @param[in]   length: data length
* @return      N/A
*/
void ble_bleuarts_rx_req_data(const uint8_t *rx_data, uint16_t length)
{
	fputmc((uint8_t *)rx_data, length);
}

/*
 * MAIN FUNCTION
 ****************************************************************************************
 */

int main(void)
{
	__align(4) uint8_t data[64];
	uint16_t len;
	
    //Must be first in main()
    sys_InitMain();

    //start interrupt handling
    GLOBAL_INT_START(); 

    drvi_initialize();
	
	uart_RxCallbackRegister(uart_rx_handler);

	printf("== BLE UARTS Example Start ==\r\n");

	APP_BLEMGR_Init();  

    while(1)
    {
        rwip_schedule();

		//
		// If UART received "\r\n" characters, copy and bypass to BLE interface
		//
        if(uart_cmd_is_ready()== true)
        {
			len = uart_buf_get(&data[0]);
			app_bleuarts_notify_send(&data[0], len);
		}
    }
}

