/**
 ****************************************************************************************
 *
 * @file arch_main.c
 *
 * @brief Main loop of the application.
 *
 * Copyright (C) CloudChip 2017-2019
 *
 *
 ****************************************************************************************
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
extern void venus_main(void);

/*
 * MAIN FUNCTION
 ****************************************************************************************
 */


int main(void)
{
#ifdef CFG_BLE_APP
//    uint32_t error = 0;
#endif
    //Must be first in main()
    sys_InitMain();

#if 0//def CFG_BLE_APP
    memset (((void *) 0x40006000), 0, 8192);
    memset (((void *) 0x20000048), 0, 0x820);
 //   *((uint32_t *) 0x40000010) = 0x00000B40;
    *((uint32_t *) 0x4000011C) = 0x00000008;
    *((uint32_t *) 0x40000104) = (*((uint32_t *) 0x40000104) & 0xFFFFFE0) | 0x04;
    //*((uint32_t *) 0x20000648) = 0x00;
    //regCKGEN->bf.bleClkDiv = 0x04;

    // Initialize RW SW stack
    rwip_init(error);
#endif

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
    bootloader_main();
#elif VENUS_WRISTBAND
    venus_main();
#endif





    while(1)
    {
#ifdef CFG_BLE_APP
        rwip_schedule();
        rwip_ignore_ll_conn_param_update_patch();
#endif
    }
    //return(0);
}

#ifdef CFG_BLE_APP
void BLE_IRQHandler(void)
{    
    rwble_isr();
}
#endif
