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
#include <stddef.h>
#include "drvi_timer.h"
#include "drvi_pwm.h"
#include "drvi_gpio.h"
#include "drvi_wdt.h"

void drvi_initialize(void)
{
    drvi_timer0_init((void*)NULL);
    drvi_gpio_pinmux_init();
    drvi_pwm0_init((void*)NULL);
    drvi_wdt_init(30000);
    
    #if defined _SPI_INUSE_ && (_SPI_INUSE_)
    drvi_SpiInit();
    #endif
    
    #if defined _I2C0_INUSE_ && (_I2C0_INUSE_)
    drvi_I2CInit(0);
    #endif
    #if defined _I2C1_INUSE_ && (_I2C1_INUSE_)
    drvi_I2CInit(1);
    #endif
    
    
    #if defined _UART0_INUSE_ && (_UART0_INUSE_)
    drvi_UartInit(0);
    #endif
    #if defined _UART1_INUSE_ && (_UART0_INUSE_)
    drvi_UartInit(1);
    #endif
    #if defined _UART2_INUSE_ && (_UART0_INUSE_)
    drvi_UartInit(2);
    #endif
}


