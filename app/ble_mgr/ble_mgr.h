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

#ifndef _APP_BLE_MGR_H_
#define _APP_BLE_MGR_H_

/******************************************************************************
 * INCLUDE FILE
 ******************************************************************************/
#include "project.h"
#include "jump_table.h"
#include "rwip.h"
#include "scheduler.h"

 /******************************************************************************
 * DEFINITION / CONSTANT / ENUM / TYPE
 ******************************************************************************/


/******************************************************************************
 * EXPORTED FUNCTION
 ******************************************************************************/

/******************************************************************************
 * [FUNCTION] APP_BLEMGR_Init
 *     1. Initialize BLE Manager (BLE_MGR)
 *     2. Start BLE stack
 * [ARGUMENT] None.
 * [RETURN  ] None.
 ******************************************************************************/
extern void    APP_BLEMGR_Init(void);

/******************************************************************************
 * [FUNCTION] rwip_eif_get
 *     1. Hook functions of UART interfaces for stack to call
 *     2. This interface is for BLE stack only. APP should not call this.
 * [ARGUMENT] None.
 * [RETURN  ] None.
 ******************************************************************************/
const struct rwip_eif_api* rwip_eif_get(uint8_t type);


/******************************************************************************
 * [FUNCTION] APP_SW_Timer_Init
 *     1. Initialize SW timer 
 * [ARGUMENT] None.
 * [RETURN  ] None.
 ******************************************************************************/
void    APP_SW_Timer_Init(void);


#endif    //(_APP_BLE_MGR_H_)

