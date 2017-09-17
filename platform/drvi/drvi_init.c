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
#include "drvi_gpio.h"
#if defined I2C_INUSE && (I2C_INUSE)
#include "drvi_i2c.h"
#endif
#include "drvi_pwm.h"
#if defined(WKTM_INUSE) && WKTM_INUSE
#include "drvi_wktm.h"
#endif
#if defined SPI_INUSE && (SPI_INUSE)
#include "drvi_spi.h"
#endif
#if defined UART_INUSE && (UART_INUSE)
#include "drvi_uart.h"
#endif
#include "drvi_wdt.h"

void drvi_initialize(void)
{
    #if defined(WKTM_INUSE) && WKTM_INUSE
    drvi_wktmInit();
    #endif
    
    drvi_GpioPinMuxInit();

    #if defined UART_INUSE && (UART_INUSE)
    drvi_UartInit();
    #endif

    drvi_pwm0_init((void*)NULL);
    drvi_wdt_init(30000);
    
    #if defined SPI_INUSE && (SPI_INUSE)
    drvi_SpiInit();
    #endif

    #if defined I2C_INUSE && (I2C_INUSE)
    drvi_I2cInit();
    #endif

}


