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
    uint32_t error = 0;
#endif
    //Must be first in main()
    sys_InitMain();

#ifdef CFG_BLE_APP
    memset (((void *) 0x40006000), 0, 8192);

    *((uint32_t *) 0x40000010) = 0x00000B40;
    *((uint32_t *) 0x4000011C) = 0x00000008;

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

#ifdef VENUS_WRISTBAND
    venus_main();
#endif









    while(1)
    {
#ifdef CFG_BLE_APP
        rwip_schedule();
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
