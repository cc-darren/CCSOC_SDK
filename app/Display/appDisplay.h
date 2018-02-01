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
 
#ifndef APP_DISPLAY_H
#define APP_DISPLAY_H

/**
 * -----------------------------------------------------------------------------
 * Included headers
 * -----------------------------------------------------------------------------
 */
#include <stdint.h>

#ifdef OLED_SOLOMON_SH1107
#include "sh1107.h"
#elif defined (OLED_SOLOMON_SSD1306)
#include "ssd1306.h"
#endif

/**
 * -----------------------------------------------------------------------------
 * Type declarations
 * -----------------------------------------------------------------------------
 */
#ifdef __cplusplus
extern "C" {
#endif


typedef enum
{
    E_OLED_PWRSTATE_OFF= 0,
    E_OLED_PWRSTATE_INIT,
    E_OLED_PWRSTATE_TURNON,
    E_OLED_PWRSTATE_TURNOFF,
    E_OLED_PWRSTATE_WAKEUP,
    E_OLED_PWRSTATE_SLEEP,
    E_OLED_PWRSTATE_ERR,
}E_OLED_PWRSTATE;

/**
 * -----------------------------------------------------------------------------
 * API declarations
 * -----------------------------------------------------------------------------
 */
/**
 * @brief Initalize long sitting monitor
 */
void app_displayoled_init(void);

/**
 * @brief Start long sitting monitor
 */
void app_displayoled_start(void);

int16_t app_displayoled_routine(void);

/**
 * @brief Stop long sitting monitor
 */
void app_displayoled_stop(void);

/**
 * @brief Reset long sitting monitor
 */
void app_displayoled_reset(void);

void app_displayoled_setstepcnt(uint32_t step_cnt);
void app_displayoled_changestate(E_OLED_PWRSTATE e_nxState,char state);

#ifdef __cplusplus
}
#endif

#endif /**< CFG_MODULE_LONGSIT */

