/******************************************************************************
*  Copyright 2017, Cloudchip, Inc.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Cloudchip, Inc. (C) 2017
******************************************************************************/

/******************************************************************************
*  Filename:
*  ---------
*  test_wktm.c
*
*  Project:
*  --------
*  CC6801
*
*  Description:
*  ------------
*  the description
*
*  Author:
*  -------
*  PPP   (cc-pattaya)
*
*===========================================================================
*
******************************************************************************/

/******************************************************************************
Head Block of The File
******************************************************************************/
#include "test.h"

#if (TEST_WKTM)

#include "sw_timer.h"

#define APP_TIMER_PRESCALER                     0                                           /**< Value of the RTC1 PRESCALER register. */
#define APP_TIMER_OP_QUEUE_SIZE                 4                                           /**< Size of timer operation queues. */

#define INTERVAL_300ms                          APP_TIMER_TICKS(300,    APP_TIMER_PRESCALER)
#define INTERVAL_400ms                          APP_TIMER_TICKS(400,    APP_TIMER_PRESCALER)
#define INTERVAL_500ms                          APP_TIMER_TICKS(500,    APP_TIMER_PRESCALER)
#define INTERVAL_700ms                          APP_TIMER_TICKS(700,    APP_TIMER_PRESCALER)
#define INTERVAL_800ms                          APP_TIMER_TICKS(800,    APP_TIMER_PRESCALER)
#define INTERVAL_1000ms                         APP_TIMER_TICKS(1000,   APP_TIMER_PRESCALER)

/******************************************************************************
Declaration of External Variables & Functions
******************************************************************************/

/******************************************************************************
Declaration of data structure
******************************************************************************/

/******************************************************************************
Declaration of Global Variables & Functions
******************************************************************************/
APP_TIMER_DEF(m_INTERVAL_1000ms_id);
APP_TIMER_DEF(m_INTERVAL_400ms_id);
APP_TIMER_DEF(m_INTERVAL_300ms_id);
APP_TIMER_DEF(m_INTERVAL_500ms_id);
APP_TIMER_DEF(m_INTERVAL_700ms_id);
APP_TIMER_DEF(m_INTERVAL_800ms_id);


volatile uint32_t Cnt1000 = 0;
volatile uint32_t Cnt400 = 0;
volatile uint32_t Cnt300 = 0;
volatile uint32_t Cnt500 = 0;
volatile uint32_t Cnt700 = 0;
volatile uint32_t Cnt800 = 0;

/******************************************************************************
Declaration of static Global Variables & Functions
******************************************************************************/
static void INTERVAL_1000ms_timeout_handler(void * p_context)
{
    UNUSED_PARAMETER(p_context);
    Cnt1000++;
}

static void INTERVAL_400ms_timeout_handler(void * p_context)
{
    UNUSED_PARAMETER(p_context);
    Cnt400++;
}

static void INTERVAL_300ms_timeout_handler(void * p_context)
{
    UNUSED_PARAMETER(p_context);
    Cnt300++;
}

static void INTERVAL_500ms_timeout_handler(void * p_context)
{
    UNUSED_PARAMETER(p_context);
    Cnt500++;
}

static void INTERVAL_700ms_timeout_handler(void * p_context)
{
    UNUSED_PARAMETER(p_context);
    Cnt700++;
}

static void INTERVAL_800ms_timeout_handler(void *p_context)
{
    UNUSED_PARAMETER(p_context);
    Cnt800++;
}

static void timers_start_test(void)
{
    volatile uint32_t err_code;

    err_code = app_timer_start(m_INTERVAL_300ms_id, INTERVAL_300ms, NULL);
    APP_ERROR_CHECK(err_code);

    err_code = app_timer_start(m_INTERVAL_400ms_id, INTERVAL_400ms, NULL);
    APP_ERROR_CHECK(err_code);

    err_code = app_timer_start(m_INTERVAL_500ms_id, INTERVAL_500ms, NULL);
    APP_ERROR_CHECK(err_code);

    err_code = app_timer_start(m_INTERVAL_1000ms_id, INTERVAL_1000ms, NULL);
    APP_ERROR_CHECK(err_code);

    err_code = app_timer_start(m_INTERVAL_700ms_id, INTERVAL_700ms, NULL);
    APP_ERROR_CHECK(err_code);

    err_code = app_timer_start(m_INTERVAL_800ms_id, INTERVAL_800ms, NULL);
    APP_ERROR_CHECK(err_code);

}

static void timers_init(void)
{

    // Initialize timer module.
    APP_TIMER_INIT(APP_TIMER_PRESCALER, APP_TIMER_OP_QUEUE_SIZE, false);

    // Create idle timers.
    app_timer_create(&m_INTERVAL_1000ms_id,
                     APP_TIMER_MODE_REPEATED,
                     INTERVAL_1000ms_timeout_handler);

    //Create Battery Level Update timers
    app_timer_create(&m_INTERVAL_400ms_id,
                     APP_TIMER_MODE_REPEATED,
                     INTERVAL_400ms_timeout_handler);

    app_timer_create(&m_INTERVAL_300ms_id,
                     APP_TIMER_MODE_REPEATED,
                     INTERVAL_300ms_timeout_handler);

    // Create led timer.
    app_timer_create(&m_INTERVAL_500ms_id,
                     APP_TIMER_MODE_REPEATED,
                     INTERVAL_500ms_timeout_handler);

    // Create Mag_Read timer
    app_timer_create(&m_INTERVAL_700ms_id,
                     APP_TIMER_MODE_REPEATED,
                     INTERVAL_700ms_timeout_handler);

    // Create Mag_Read timer
    app_timer_create(&m_INTERVAL_800ms_id,
                     APP_TIMER_MODE_REPEATED,
                     INTERVAL_800ms_timeout_handler);

}

void TEST_Wktm(void)
{
    timers_init();
    timers_start_test();
}

#endif //TEST_WKTM

