/* Copyright (c) 2016 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

#include "global.h"

#include "fota.h"
//#include "nrf_dfu_transport.h"
#include "fota_utils.h"
#include "fota_bootloader_app_start.h"
#include "fota_settings.h"
#include "drvi_gpio.h"
#include "app_scheduler.h"
//#include "app_timer_appsh.h"
#include "tracer.h"
//#include "boards.h"
#include "sw_timer.h"
#include "bootloader_info.h"
#include "fota_req_handler.h"
#include "fota_transport.h"
#ifdef CFG_BLE_APP
#include "app.h"
#include "rwip.h"
#endif


typedef void (*sw_timer_timeout_handler_t)(void * p_context);

typedef struct
{
    sw_timer_timeout_handler_t timeout_handler;
    void *                      p_context;
} sw_timer_event_t;

#define SCHED_MAX_EVENT_DATA_SIZE       MAX(sizeof(sw_timer_event_t), 0)                        /**< Maximum size of scheduler events. */

#define SCHED_QUEUE_SIZE                20                                                      /**< Maximum number of events in the scheduler queue. */

#define APP_TIMER_PRESCALER             0                                                       /**< Value of the RTC1 PRESCALER register. */
#define APP_TIMER_OP_QUEUE_SIZE         4                                                       /**< Size of timer operation queues. */


// Weak function implementation

/** @brief Weak implemenation of nrf_dfu_check_enter.
 *
 * @note    This function must be overridden to enable entering DFU mode at will.
 *          Default behaviour is to enter DFU when BOOTLOADER_BUTTON is pressed.
 */
__WEAK bool nrf_dfu_enter_check(void)
{
/*    
    drvi_GpioDirectInput(GPIO_PIN_1);

    if (drvi_GpioRead(GPIO_PIN_1) == 0)
    {
        return true;
    }
*/
    if (s_dfu_settings.enter_buttonless_dfu == 1)
    {
        s_dfu_settings.enter_buttonless_dfu = 0;
        nrf_dfu_settings_write(NULL);
        return true;
    }
    return false;
}


// Internal Functions

/**@brief Function for initializing the timer handler module (app_timer).
 */
static void timers_init(void)
{
    // Initialize timer module, making it use the scheduler.
    //APP_TIMER_APPSH_INIT(APP_TIMER_PRESCALER, APP_TIMER_OP_QUEUE_SIZE, true);
}


/** @brief Function for event scheduler initialization.
 */
static void scheduler_init(void)
{
    APP_SCHED_INIT(SCHED_MAX_EVENT_DATA_SIZE, SCHED_QUEUE_SIZE);
}


static void wait_for_event()
{
    // Transport is waiting for event?
    while(true)
    {
        // Can't be emptied like this because of lack of static variables
        app_sched_execute();
#ifdef CFG_BLE_APP
        rwip_schedule();
        rwip_detect_disconnect_patch();
        rwip_ignore_ll_conn_param_update_patch();   
        app_wait_for_reset_ble();

#endif        
    }
}

#ifdef FOTA_TEST_NO_BLE
static void fota_test(void) 
{
    nrf_dfu_res_code_t  res_code;
    nrf_dfu_req_t       dfu_req;
    nrf_dfu_res_t       dfu_res = {{{0}}};

    char *buffer;
    buffer = (char *)0x20010000;

    // 1st cmd, SELECT
    dfu_req.obj_type = NRF_DFU_OBJ_TYPE_COMMAND;
    dfu_req.req_type = NRF_DFU_OBJECT_OP_SELECT;
    res_code = nrf_dfu_req_handler_on_req(NULL, &dfu_req, &dfu_res);
    TracerInfo("Sending Response: [0x%01x, 0x%01x]\r\n", dfu_req.req_type, res_code);

    // 2nd cmd, CREATE
    dfu_req.obj_type    =  NRF_DFU_OBJ_TYPE_COMMAND;
    dfu_req.object_size = 0x40; //Header DAT size
    dfu_req.req_type    = NRF_DFU_OBJECT_OP_CREATE;
    res_code = nrf_dfu_req_handler_on_req(NULL, &dfu_req, &dfu_res);
    TracerInfo("Sending Response: [0x%01x, 0x%01x]\r\n", dfu_req.req_type, res_code);

    // 3rd cmd, WRITE Header/20 bytes, around 7 times
    {
        int offset = 0;
        int total_size = dfu_req.object_size;

        while(offset < total_size)
        {
            memset(&dfu_req, 0, sizeof(nrf_dfu_req_t));
            dfu_req.req_type =  NRF_DFU_OBJECT_OP_WRITE;
            // Set data and length
            dfu_req.p_req    =  (uint8_t *)(buffer + offset);
            //dfu_req.req_len  =  20;
            if ((offset+20) > total_size)
                dfu_req.req_len = total_size - offset;
            else
                dfu_req.req_len = 20;

            offset += dfu_req.req_len;
            res_code = nrf_dfu_req_handler_on_req(NULL, &dfu_req, &dfu_res);
            //TracerInfo("Sending Response: [0x%01x, 0x%01x]\r\n", dfu_req.req_type, res_code);
        }
    }
    // 4th cmd, EXECUTE
    dfu_req.req_type     =  NRF_DFU_OBJECT_OP_EXECUTE;
    res_code = nrf_dfu_req_handler_on_req(NULL, &dfu_req, &dfu_res);
    TracerInfo("Sending Response: [0x%01x, 0x%01x]\r\n", dfu_req.req_type, res_code);

    //Data transfer start
    buffer = (char *)0x20011000;

    // 1st cmd, SELECT
    dfu_req.obj_type = NRF_DFU_OBJ_TYPE_DATA;
    dfu_req.req_type = NRF_DFU_OBJECT_OP_SELECT;
    res_code = nrf_dfu_req_handler_on_req(NULL, &dfu_req, &dfu_res);
    TracerInfo("Sending Response: [0x%01x, 0x%01x]\r\n", dfu_req.req_type, res_code);

    // 2nd cmd, CREATE
    dfu_req.obj_type    =  NRF_DFU_OBJ_TYPE_DATA;
    dfu_req.object_size = 0x358C; //Image BIN size
    dfu_req.req_type    = NRF_DFU_OBJECT_OP_CREATE;
    res_code = nrf_dfu_req_handler_on_req(NULL, &dfu_req, &dfu_res);
    TracerInfo("Sending Response: [0x%01x, 0x%01x]\r\n", dfu_req.req_type, res_code);

    // 3rd cmd, WRITE Header/20 bytes, around 7 times
    {
        int offset = 0;
        int total_size = dfu_req.object_size;

        while(offset < total_size)
        {
            memset(&dfu_req, 0, sizeof(nrf_dfu_req_t));
            dfu_req.req_type =  NRF_DFU_OBJECT_OP_WRITE;
            // Set data and length
            dfu_req.p_req    =  (uint8_t *)(buffer + offset);
            //dfu_req.req_len  =  20;
            if ((offset+20) > total_size)
                dfu_req.req_len = total_size - offset;
            else
                dfu_req.req_len = 20;

            offset += dfu_req.req_len;
            res_code = nrf_dfu_req_handler_on_req(NULL, &dfu_req, &dfu_res);
            //TracerInfo("Sending Response: [0x%01x, 0x%01x]\r\n", dfu_req.req_type, res_code);
        }
    }

    // 4th cmd, EXECUTE
    dfu_req.req_type     =  NRF_DFU_OBJECT_OP_EXECUTE;
    res_code = nrf_dfu_req_handler_on_req(NULL, &dfu_req, &dfu_res);
    TracerInfo("Sending Response: [0x%01x, 0x%01x]\r\n", dfu_req.req_type, res_code);
}
#endif

void nrf_dfu_wait()
{
    app_sched_execute();
}


uint32_t nrf_dfu_init()
{
    uint32_t ret_val = CC_SUCCESS;
    uint32_t enter_bootloader_mode = 0;

    TracerInfo("In real nrf_dfu_init\r\n");

#if defined(FSTORAGE_ENABLED) && FSTORAGE_ENABLED
    ret_val = nrf_dfu_flash_init(true);
#else
    ret_val = nrf_dfu_flash_init(false);
#endif

    if (ret_val)
    {
        TracerInfo("Could not initalize flash\n");
        return ret_val;
    }

    nrf_dfu_settings_init();

    // Continue ongoing DFU operations
    // Note that this part does not rely on SoftDevice interaction
    ret_val = nrf_dfu_continue(&enter_bootloader_mode);
    if(ret_val != CC_SUCCESS)
    {
        TracerInfo("Could not continue DFU operation: 0x%08x\r\n");
        enter_bootloader_mode = 1;
    }

    // Check if there is a reason to enter DFU mode
    // besides the effect of the continuation
    if (nrf_dfu_enter_check())
    {
        TracerInfo("Application sent bootloader request\n");
        enter_bootloader_mode = 1;
    }

    if(enter_bootloader_mode != 0 || !nrf_dfu_app_is_valid())
    {
        timers_init();
        scheduler_init();

        // Initializing transports
        ret_val = fota_transports_init();
        if (ret_val != CC_SUCCESS)
        {
            TracerInfo("Could not initalize DFU transport: 0x%08x\r\n");
            return ret_val;
        }

        (void)nrf_dfu_req_handler_init();

        // This function will never return
        TracerInfo("Waiting for events\r\n");

#ifdef FOTA_TEST_NO_BLE
        // Blake, Add for testing DFU request handle to simulation BLE
        fota_test();
#endif

        wait_for_event();
        TracerInfo("After waiting for events\r\n");
    }

    if (nrf_dfu_app_is_valid())
    {
        TracerInfo("Jumping to: 0x%08x\r\n", MAIN_APPLICATION_START_ADDR);
        nrf_bootloader_app_start(MAIN_APPLICATION_START_ADDR);
    }

    // Should not be reached!
    TracerInfo("After real nrf_dfu_init\r\n");
    return CC_SUCCESS;
}
