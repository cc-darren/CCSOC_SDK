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
#if defined(I2C_INUSE) && I2C_INUSE
#include "drvi_i2c.h"
#endif
#if defined(PWM_INUSE) && PWM_INUSE
#include "drvi_pwm.h"
#endif
#if defined(WKTM_INUSE) && WKTM_INUSE
#include "drvi_wktm.h"
#endif
#if defined(SPI_INUSE) && SPI_INUSE
#include "drvi_spi.h"
#endif
#if defined(UART_INUSE) && UART_INUSE
#include "drvi_uart.h"
#endif
#if defined(WDT_INUSE) && WDT_INUSE
#include "drvi_wdt.h"
#endif

void drvi_initialize(void)
{
    #if defined(WKTM_INUSE) && WKTM_INUSE
    drvi_wktmInit();
    #endif
    
    drvi_GpioPinMuxInit();

    #if defined(UART_INUSE) && UART_INUSE
    drvi_UartInit();
    #endif

    #if defined(PWM_INUSE) && PWM_INUSE
    drvi_pwmInit();
    #endif

    #if defined (SPI_INUSE) && SPI_INUSE
    drvi_SpiInit();
    #endif

    #if defined (I2C_INUSE) && I2C_INUSE
    drvi_I2cInit();
    #endif

    #if defined(WDT_INUSE) && WDT_INUSE
    drvi_WdtInit(WDT_DEFAULT_TIMEOUT);
    #endif
}


