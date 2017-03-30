/**
 ****************************************************************************************
 *
 * @file drvi_init.c
 *
 * @brief HW drivers initialize.
 *
 * Copyright (C) CloudChip 2017-2019
 *
 *
 ****************************************************************************************
 */
#include "drvi_timer.h"
#include "drvi_gpio.h"

void drvi_initialize(void)
{
    //drvi_timer0_init();
    drvi_gpio_pinmux_init();
}


