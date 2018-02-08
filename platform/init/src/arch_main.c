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
#include "tracer.h"
#include "project.h"
#ifdef CFG_BLE_APP
#include "rwble.h"  //avoid warning or rwble_isr()
#include "app.h"
//#include "cc6801_reg.h"
#endif
#ifdef BOOTLOADER
#include "bootloader.h"
#endif
#if defined(TESTCASE) && TESTCASE
#include "test.h"
#endif

uint8_t g_GyroEnable = 1;

#ifdef CFG_BLE_APP
extern void rwip_init(uint32_t);
extern void rwip_schedule(void);
#endif

extern void sys_InitMain(void);
extern int venus_main(void);

/*
 * MAIN FUNCTION
 ****************************************************************************************
 */

int main(void)
{
    #if ZEUS_WRISTBAND
        venus_main();    // infinite loop in venus_main()
    #elif VENUS_WRISTBAND
        venus_main();    // infinite loop in venus_main()
    #endif


    //Must be first in main()
    sys_InitMain();

    //start interrupt handling
    GLOBAL_INT_START(); 

    drvi_initialize();

    TracerInfo("== CC6801 Start ==\r\n");

    /*******************************/
    /****** Application Start ******/
    /*******************************/

#if defined(TESTCASE) && TESTCASE
    TEST_Main();
#endif

#ifdef BOOTLOADER
    BootloaderMain();
#endif





    while(1)
    {
#ifdef CFG_BLE_APP
        rwip_schedule();
        rwip_detect_disconnect_patch();
        rwip_ignore_ll_conn_param_update_patch();
#endif
    }
    //return(0);
}

