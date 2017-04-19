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
#include "spi_oled.h"
#include "appDisplay.h"
#include "FP_PED_8Bit.h"

#define APP_TIMER_PRESCALER                     0                                           /**< Value of the RTC1 PRESCALER register. */
#define APP_TIMER_OP_QUEUE_SIZE                 4                                           /**< Size of timer operation queues. */

#define SPEED_AND_CADENCE_MEAS_INTERVAL         APP_TIMER_TICKS(300,   APP_TIMER_PRESCALER)
#define BATTERY_LEVEL_MEAS_INTERVAL             APP_TIMER_TICKS(400,  APP_TIMER_PRESCALER)
#define LED_BLINKING_PERIOD                     APP_TIMER_TICKS(500, APP_TIMER_PRESCALER)
#define MAG_READ_PERIOD                         APP_TIMER_TICKS(700,     APP_TIMER_PRESCALER)     //20ms for ODR=50
#define FIFO_READ_PERIOD                        APP_TIMER_TICKS(20,    APP_TIMER_PRESCALER)     //640ms for 32depth*20ms
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
uint32_t readSensor;

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

    readSensor = 1;
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
        spi_init(2);
        ACC_Init();
        //MAG_Init();
        init_done = 1;
    }
}

// VENUS CONTROL BLOCK & DEFINITION =========================================
enum
{
    E_VENUS_EVENT_SENSOR_DATA_READY_ACCEL = 0,
    E_VENUS_EVENT_SENSOR_DATA_READY_MAG,
    E_VENUS_EVENT_SENSOR_DATA_READY_GYRO,
    E_VENUS_EVENT_SENSOR_DATA_READY,

    E_VENUS_EVENT_HRM,
    E_VENUS_EVENT_HRM_TIMEOUT,

    E_VENUS_EVENT_OLED_UPDATE,

    E_VENUS_EVENT_TOUCH_INT,

    E_VENUS_EVENT_BLE_NOTIFY_TO,

    E_VENUS_EVENT_HRM_STRESSTEST,

    E_VENUS_EVENT_DUMMY_END
};

typedef struct
{
    //////// SYSTEM CONTROL ////////////////////////////////////////////////////
    uint32_t    dwSleepLock;
    uint32_t    dwSleepUnLock;

    //////// EXTERNAL EVENT POST ///////////////////////////////////////////
    uint8_t     baEvent[E_VENUS_EVENT_DUMMY_END];

    #if (E_VENUS_EVENT_DUMMY_END % 4)
        uint8_t     baEventPadding[(4 - (E_VENUS_EVENT_DUMMY_END % 4))];
    #endif

    //////// APPLICATION :: PEDOMETER //////////////////////////////////////////
    uint32_t       dwPedStepCount;
    signed char    cPedState;
    uint8_t        baPedPadding[3];    // adjust this to keep 4-byte alignment

    //////// APPLICATION :: HRM ////////////////////////////////////////////////
    uint8_t     cHrmOnGoing;

    //////// APPLICATION :: AHRS ///////////////////////////////////////////////

}   S_VenusCB;

S_VenusCB    s_tVenusCB;

void _sensor_accel_on_change(void)
{
    short          _wAccelData[3]  = { 0 };
    uint32_t       _dwPedStepCount = 0;
    signed char    _cPedState      = 0;

    //NRF_LOG_INFO("PED tick = %d\n", get_sys_tick());

    ACC_Data_Get(NULL, (i16_t *) _wAccelData);

    _wAccelData[0] = (_wAccelData[0] / 4);
    _wAccelData[1] = (_wAccelData[1] / 4);
    _wAccelData[2] = (_wAccelData[2] / 4);

    printf( "ACC %d %d %d\r\n",_wAccelData[0],_wAccelData[1],_wAccelData[2]);

    FP_PED_8Bit(((const short *) _wAccelData), &_dwPedStepCount, &_cPedState);

    if (s_tVenusCB.dwPedStepCount != _dwPedStepCount)
    {
        printf("PED STEP = %d\n",_dwPedStepCount);

        s_tVenusCB.dwPedStepCount = _dwPedStepCount;

        app_displayoled_setstepcnt(s_tVenusCB.dwPedStepCount);
    }
}

extern void app_trace_init(void);

void _app_init(void)
{
    memset(&s_tVenusCB, 0, sizeof(s_tVenusCB));

    FP_PED_8Bit_initialize();
    app_displayoled_init();
    app_displayoled_start();
}

int main(void)
{
    drvi_initialize();

    app_trace_init();

    pwm_start_test();

    timers_init();
    timers_start_test();

    //start interrupt handling
    GLOBAL_INT_START();
    printf("\r\n== CC6801 Start ==\r\n");

    sensor_init();
    _app_init();

    while(1)
    {
       readSensor = 0;
       _sensor_accel_on_change();
       app_displayoled_routine();
       while(!readSensor);
    }
    rw_main();

    while(1);
    //return(0);
}

