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
 
/** @file
 *
 * @defgroup sw_timer Application Timer
 * @{
 * @ingroup app_common
 *
 * @brief Application timer functionality.
 */

#ifndef SW_TIMER_H__
#define SW_TIMER_H__

#ifdef SW_TIMER_BY_KERNEL

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "global.h"
#include "drvi_wktm.h"

#define SW_TIMER_TOTOAL_NUM        32
#define SW_TIMER_DEF(timer_id)   static uint16_t timer_id = 0xFFFF
            

typedef enum
{
    SW_TIMER_MODE_SINGLE_SHOT,                 /**< The timer will expire only once. */
    SW_TIMER_MODE_REPEATED                     /**< The timer will restart each time it expires. */
} sw_timer_mode_t;



typedef void (*sw_timer_timeout_handler_t)(void * p_context);


typedef uint32_t (*sw_timer_evt_schedule_func_t) (sw_timer_timeout_handler_t timeout_handler,void *          p_context);


typedef struct
{
    uint32_t                   periodic_interval;     /**< Timer period (for repeating timers). */
    sw_timer_mode_t            mode;                  /**< Timer mode. */
    sw_timer_timeout_handler_t p_timeout_handler;     /**< Pointer to function to be executed when the timer expires. */
} timer_node_t;


extern uint16_t                      m_timer_id;
extern timer_node_t                  m_timer_node[SW_TIMER_TOTOAL_NUM];



uint32_t sw_timer_create(uint16_t *      p_timer_id,
                          sw_timer_mode_t            mode,
                          sw_timer_timeout_handler_t timeout_handler);

uint32_t sw_timer_start(uint16_t timer_id, uint32_t timeout_ticks, void * p_context);


uint32_t sw_timer_stop(uint16_t timer_id);


#endif

#endif // SW_TIMER_H__

/** @} */



