/**
 ****************************************************************************************
 *
 * @file drvi_gpio.h
 *
 * @brief Head file
 *
 * Copyright (C) CloudChip 2017-2019
 *
 *
 ****************************************************************************************
 */
#ifndef _DRVI_GPIO_H_
#define _DRVI_GPIO_H_
#include "gpio.h"


#define drvi_gpio_pinmux_init()            cc6801_gpio_pinmux_init()
#define drvi_gpio_direction_input(x)       cc6801_gpio_direction_input(x)
#define drvi_gpio_direction_output(x)      cc6801_gpio_direction_output(x)

#define drvi_gpio_read(x)                  cc6801_gpio_read(x)
#define drvi_gpio_write(x,y)               cc6801_gpio_write(x,y)

#define drvi_request_irq(pin,func,type)    cc6801_request_irq(pin,func,type);
#define drvi_enable_irq(pin)               cc6801_enable_irq(pin);
#define drvi_disable_irq(pin)              cc6801_disable_irq(pin);


/*
 * IRQ line status.
 *
 * IRQ_TYPE_NONE            - default, unspecified type
 * IRQ_TYPE_EDGE_RISING     - rising edge triggered
 * IRQ_TYPE_EDGE_FALLING    - falling edge triggered
 * IRQ_TYPE_EDGE_BOTH       - rising and falling edge triggered
 * IRQ_TYPE_LEVEL_HIGH      - high level triggered
 * IRQ_TYPE_LEVEL_LOW       - low level triggered
 * IRQ_TYPE_LEVEL_MASK      - Mask to filter out the level bits
 * IRQ_TYPE_SENSE_MASK      - Mask for all the above bits
 * IRQ_TYPE_PROBE           - Special flag for probing in progress
 */
enum
{
    IRQ_TYPE_NONE           = 0x00000000,
    IRQ_TYPE_EDGE_RISING    = 0x00000001,
    IRQ_TYPE_EDGE_FALLING   = 0x00000002,
    IRQ_TYPE_EDGE_BOTH      = (IRQ_TYPE_EDGE_FALLING | IRQ_TYPE_EDGE_RISING),
    IRQ_TYPE_LEVEL_HIGH     = 0x00000004,
    IRQ_TYPE_LEVEL_LOW      = 0x00000008,
    IRQ_TYPE_LEVEL_MASK     = (IRQ_TYPE_LEVEL_LOW | IRQ_TYPE_LEVEL_HIGH),
    IRQ_TYPE_SENSE_MASK     = 0x0000000f,

    IRQ_TYPE_PROBE      = 0x00000010,
};














#endif //_DRVI_GPIO_H_


