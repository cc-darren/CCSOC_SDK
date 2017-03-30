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
#include "sw_timer.h"


#define APP_TIMER_PRESCALER                     0                                           /**< Value of the RTC1 PRESCALER register. */
#define APP_TIMER_OP_QUEUE_SIZE                 4                                           /**< Size of timer operation queues. */

#define SPEED_AND_CADENCE_MEAS_INTERVAL         APP_TIMER_TICKS(1000,   APP_TIMER_PRESCALER)
#define BATTERY_LEVEL_MEAS_INTERVAL             APP_TIMER_TICKS(60000,  APP_TIMER_PRESCALER)
#define LED_BLINKING_PERIOD                     APP_TIMER_TICKS(180000, APP_TIMER_PRESCALER)
#define MAG_READ_PERIOD                         APP_TIMER_TICKS(20,     APP_TIMER_PRESCALER)     //20ms for ODR=50
#define FIFO_READ_PERIOD                        APP_TIMER_TICKS(640,    APP_TIMER_PRESCALER)     //640ms for 32depth*20ms
#define SYSTEM_IDLE_INTERVAL                    APP_TIMER_TICKS(276000, APP_TIMER_PRESCALER)


APP_TIMER_DEF(m_idle_timer_id);
APP_TIMER_DEF(m_battery_timer_id);
APP_TIMER_DEF(m_csc_meas_timer_id);
APP_TIMER_DEF(m_led_blinking_id);
APP_TIMER_DEF(m_mag_read_id);
APP_TIMER_DEF(m_fifo_read_id);


volatile uint32_t systemIdleCnt = 0;
volatile uint32_t batteryLevelCnt = 0;
volatile uint32_t cscMeasCnt = 0;
volatile uint32_t ledBlinkCnt = 0;
volatile uint32_t magReadCnt = 0;
volatile uint32_t fifoReadCnt = 0;


static void system_idle_timeout_handler(void * p_context)
{
    UNUSED_PARAMETER(p_context);
    systemIdleCnt++;
}

static void battery_level_meas_timeout_handler(void * p_context)
{
    UNUSED_PARAMETER(p_context);
    batteryLevelCnt++;
}

static void csc_meas_timeout_handler(void * p_context)
{
    UNUSED_PARAMETER(p_context);
    cscMeasCnt++;
}

static void led_blinking_handler(void * p_context)
{
    UNUSED_PARAMETER(p_context);
    ledBlinkCnt++;
}

static void mag_read_handler(void * p_context)
{
    UNUSED_PARAMETER(p_context);
    magReadCnt++;
}

static void fifo_read_handler(void *p_context)
{
    UNUSED_PARAMETER(p_context);
    fifoReadCnt++;
}



/*
 * MAIN FUNCTION
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief RW main function.
 *
 * This function is called right after the booting process has completed.
 *
 * @return status   exit status
 ****************************************************************************************
 */
void rw_main(void)
{
    /*
     ************************************************************************************
     * Platform initialization
     ************************************************************************************
     */
    //intc_init();



    /*
     ************************************************************************************
     * RW SW stack initialization
     ************************************************************************************
     */


    /*
     ************************************************************************************
     * Main loop
     ************************************************************************************
     */

    while(1)
    {
        uint32_t i;
        uint32_t size = 0x100;
        uint32_t *romAddr   = (uint32_t*)0x00000E0;
        //uint32_t *eFlashAddr= (uint32_t*)0x10030000;
        uint32_t *sRamAddr  = (uint32_t*)0x20008000;
        uint32_t *dRamAddr  = (uint32_t*)0x20011000;

        //memset(sRamAddr, 0x55, size);
        memcpy(sRamAddr, romAddr, size);
        //memcpy(sRamAddr, eFlashAddr, size);
        memcpy(dRamAddr, sRamAddr, size);

        for(i=0;i<(size>>2);i++)
        {
            if(*(dRamAddr+i) != *(sRamAddr+i))
            {
                *((volatile unsigned int *)(0x40000220)) = i;
            }
        }

        //finish testing
        *((volatile unsigned int *)(0x4000020C)) = 1;


        WFI();
    }
}

static void timers_start_test(void)
{
    uint32_t err_code;

    err_code = app_timer_start(m_csc_meas_timer_id, SPEED_AND_CADENCE_MEAS_INTERVAL, NULL);
    APP_ERROR_CHECK(err_code);

    err_code = app_timer_start(m_battery_timer_id, BATTERY_LEVEL_MEAS_INTERVAL, NULL);
    APP_ERROR_CHECK(err_code);

    err_code = app_timer_start(m_led_blinking_id, LED_BLINKING_PERIOD, NULL);
    APP_ERROR_CHECK(err_code);

    err_code = app_timer_start(m_idle_timer_id, SYSTEM_IDLE_INTERVAL, NULL);
    APP_ERROR_CHECK(err_code);

    err_code = app_timer_start(m_mag_read_id, MAG_READ_PERIOD, NULL);
    APP_ERROR_CHECK(err_code);

    err_code = app_timer_start(m_fifo_read_id, FIFO_READ_PERIOD, NULL);
    APP_ERROR_CHECK(err_code);

}

static void timers_init(void)
{

    // Initialize timer module.
    APP_TIMER_INIT(APP_TIMER_PRESCALER, APP_TIMER_OP_QUEUE_SIZE, false);

    // Create idle timers.
    app_timer_create(&m_idle_timer_id,
                     APP_TIMER_MODE_SINGLE_SHOT,
                     system_idle_timeout_handler);


    //Create Battery Level Update timers
    app_timer_create(&m_battery_timer_id,
                     APP_TIMER_MODE_REPEATED,
                     battery_level_meas_timeout_handler);


    app_timer_create(&m_csc_meas_timer_id,
                     APP_TIMER_MODE_REPEATED,
                     csc_meas_timeout_handler);


    // Create led timer.
    app_timer_create(&m_led_blinking_id,
                     APP_TIMER_MODE_SINGLE_SHOT,
                     led_blinking_handler);

    // Create Mag_Read timer
    app_timer_create(&m_mag_read_id,
                     APP_TIMER_MODE_REPEATED,
                     mag_read_handler);



    // Create Mag_Read timer
    app_timer_create(&m_fifo_read_id,
                     APP_TIMER_MODE_REPEATED,
                     fifo_read_handler);


}

int main(void)
{
    drvi_initialize();

    timers_init();
    timers_start_test();

    //start interrupt handling
    GLOBAL_INT_START();

    rw_main();

    while(1);
    //return(0);
}

