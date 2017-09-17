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
#if defined _I2C_INUSE_ && (_I2C_INUSE_)
#include "drvi_i2c.h"
#endif
#include "drvi_pwm.h"
#if defined(_WKTM_INUSE_) && _WKTM_INUSE_
#include "drvi_wktm.h"
#endif
#if defined _SPI_INUSE_ && (_SPI_INUSE_)
#include "drvi_spi.h"
#endif
#if defined _UART_INUSE_ && (_UART_INUSE_)
#include "drvi_uart.h"
#endif
#include "drvi_wdt.h"

void drvi_initialize(void)
{
    #if defined(_WKTM_INUSE_) && _WKTM_INUSE_
    drvi_wktmInit();
    #endif
    
    drvi_GpioPinMuxInit();

    #if defined _UART_INUSE_ && (_UART_INUSE_)
    drvi_UartInit();
    #endif

    drvi_pwm0_init((void*)NULL);
    drvi_wdt_init(30000);
    
    #if defined _SPI_INUSE_ && (_SPI_INUSE_)
    drvi_SpiInit();
    #endif

    #if defined _I2C_INUSE_ && (_I2C_INUSE_)
    drvi_I2cInit();
    #endif

}


