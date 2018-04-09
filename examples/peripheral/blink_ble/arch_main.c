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
#include "ble_mgr.h"
#include "drvi_init.h"
#include "drvi_gpio.h"
#include "sw_timer.h"
#include "tracer.h"

#ifdef CFG_BLE_APP
#include "rwble.h"  //avoid warning or rwble_isr()
#include "app.h"
#endif

static int iTracerFlag = 0;

uint8_t g_GyroEnable = 1;

#ifdef CFG_BLE_APP
extern void rwip_init(uint32_t);
extern void rwip_schedule(void);
#endif

extern void sys_InitMain(void);

SW_TIMER_DEF(s_tBlinkLedTimer);
SW_TIMER_DEF(s_tTracerLogTimer);

static uint8_t g_baLed[4] = {GPIO_PIN_36, GPIO_PIN_37, GPIO_PIN_38, GPIO_PIN_39};

static void TracerLogTimer(void *pContext)
{
    iTracerFlag = 1;
}

static void BlinkLedTimer(void * pContext)
{
    static uint32_t dwPinStat = 0;
    static int iIdx = 0;

    if (iIdx >= sizeof(g_baLed))
    {
        dwPinStat ^= 1UL;
        iIdx = 0;
    }

    drvi_GpioWrite(g_baLed[iIdx++], dwPinStat);
}

/*
 * MAIN FUNCTION
 ****************************************************************************************
 */

int main(void)
{
    int iSec = 0;

    //Must be first in main()
    sys_InitMain();

    //start interrupt handling
    GLOBAL_INT_START();

    drvi_initialize();

    #ifdef CFG_BLE_APP
    APP_BLEMGR_Init();
    #endif

    #ifdef CFG_BLE_APP
    #ifdef SW_TIMER_BY_KERNEL
    APP_SW_Timer_Init();
    #endif
    #endif

    sw_timer_create(&s_tBlinkLedTimer,
                     SW_TIMER_MODE_REPEATED,
                     BlinkLedTimer);

    sw_timer_create(&s_tTracerLogTimer,
                     SW_TIMER_MODE_REPEATED,
                     TracerLogTimer);

    sw_timer_start(s_tBlinkLedTimer, 500, NULL);
    sw_timer_start(s_tTracerLogTimer, 1000, NULL);

    TracerInfo("\r\n===== [CC6801] SDK %s =====\r\n", SDK_FW_VERSION);

    /*******************************/
    /****** Application Start ******/
    /*******************************/

    while(1)
    {
        if (iTracerFlag)
        {
            iTracerFlag = 0;
            TracerInfo("sec: %d\r\n", iSec++);
        }

#ifdef CFG_BLE_APP
        rwip_schedule();
        rwip_detect_disconnect_patch();
        rwip_ignore_ll_conn_param_update_patch();
#endif
    }
    //return(0);
}

