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
#include "drvi_init.h"
#include "drvi_gpio.h"
#include "tracer.h"
#include "project.h"
#include "sdk_ccps.h"
#include "ble_mgr.h"


extern void sys_InitMain(void);


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
    
    APP_BLEMGR_Init();  

    APP_SW_Timer_Init();    
    
    TracerInfo("\r\n===== [CC6801] SDK %s =====\r\n", SDK_FW_VERSION);
            
    while(1)
    {
        rwip_schedule();
    }
}

