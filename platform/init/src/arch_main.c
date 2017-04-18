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
//#include "ak09912.h"
#include "LSM6DS3_ACC_GYRO_driver.h"
#include "spi_sensor.h"

#define APP_TIMER_PRESCALER                     0                                           /**< Value of the RTC1 PRESCALER register. */
#define APP_TIMER_OP_QUEUE_SIZE                 4                                           /**< Size of timer operation queues. */

#define SPEED_AND_CADENCE_MEAS_INTERVAL         APP_TIMER_TICKS(300,   APP_TIMER_PRESCALER)
#define BATTERY_LEVEL_MEAS_INTERVAL             APP_TIMER_TICKS(400,  APP_TIMER_PRESCALER)
#define LED_BLINKING_PERIOD                     APP_TIMER_TICKS(500, APP_TIMER_PRESCALER)
#define MAG_READ_PERIOD                         APP_TIMER_TICKS(700,     APP_TIMER_PRESCALER)     //20ms for ODR=50
#define FIFO_READ_PERIOD                        APP_TIMER_TICKS(800,    APP_TIMER_PRESCALER)     //640ms for 32depth*20ms
#define SYSTEM_IDLE_INTERVAL                    APP_TIMER_TICKS(1100, APP_TIMER_PRESCALER)


APP_TIMER_DEF(m_idle_timer_id);
APP_TIMER_DEF(m_battery_timer_id);
APP_TIMER_DEF(m_csc_meas_timer_id);
APP_TIMER_DEF(m_led_blinking_id);
APP_TIMER_DEF(m_mag_read_id);
APP_TIMER_DEF(m_fifo_read_id);


volatile uint32_t Cnt1100 = 0;
volatile uint32_t Cnt400 = 0;
volatile uint32_t Cnt300 = 0;
volatile uint32_t Cnt500 = 0;
volatile uint32_t Cnt700 = 0;
volatile uint32_t Cnt800 = 0;

unsigned char init_done = 0;

extern void pwm_start_test(void);

static void system_idle_timeout_handler(void * p_context)
{
    UNUSED_PARAMETER(p_context);
    Cnt1100++;
}

static void battery_level_meas_timeout_handler(void * p_context)
{
    UNUSED_PARAMETER(p_context);
    Cnt400++;
}

static void csc_meas_timeout_handler(void * p_context)
{
    UNUSED_PARAMETER(p_context);
    Cnt300++;
}

static void led_blinking_handler(void * p_context)
{
    UNUSED_PARAMETER(p_context);
    Cnt500++;
}

static void mag_read_handler(void * p_context)
{
    UNUSED_PARAMETER(p_context);
    Cnt700++;
}

static void fifo_read_handler(void *p_context)
{
    UNUSED_PARAMETER(p_context);
    Cnt800++;
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
        WFI();
    }
}

static void timers_start_test(void)
{
    volatile uint32_t err_code;

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
                     APP_TIMER_MODE_REPEATED,
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
                     APP_TIMER_MODE_REPEATED,
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


void sensor_init(void)
{
    if(init_done == 0)
    {
        spi_init(1);
        ACC_Init();
        //MAG_Init();
        init_done = 1;
    }
}

int main(void)
{
    drvi_initialize();

    pwm_start_test();

    timers_init();
    timers_start_test();

    //start interrupt handling
    GLOBAL_INT_START();
    printf("\r\n== CC6801 Start ==\r\n");

    sensor_init();
    rw_main();

    while(1);
    //return(0);
}

