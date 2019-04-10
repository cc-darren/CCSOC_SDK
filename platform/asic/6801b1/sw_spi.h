/* Copyright (c) 2019 Cloudchip, Inc. All Rights Reserved.
 *
 * The information contained herein is property of Cloudchip, Inc.
 * Terms and conditions of usage are described in detail in CLOUDCHIP
 * STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information.
 * NO WARRANTY of ANY KIND is provided. This heading must NOT be removed 
 * from the file.
 */

/******************************************************************************
 * Filename:
 * ---------
 * sw_spi.c
 *
 * Project:
 * --------
 * cc6801
 *
 * Description:
 * ------------
 * This is software SPI driver
 *
 * Author:
 * -------
 * CloudChip
 *
 * ============================================================================
 *  2019/02/19 C.H.Wu initial version
 *****************************************************************************/

#ifndef _SW_SPI_H_ /* { */
#define _SW_SPI_H_

#include <stdint.h>

#include "drvi_gpio.h"
#include "drvi_clock.h"

/*
 * Clock is Low when inactive (CPOL = 0)
 * Clock is High when inactive (CPOL = 1)
 *
 * Data is Valid on Clock Leading Edge (CPHA = 0)
 * Data is Valid on Clock Trailing Eage (CPHA = 1)
 * 
 */
#define SW_SPI_CPOL             (1)
#define SW_SPI_CPHA             (1)

/*
 * Chip Select is Low when active (CS = 0)
 * Chip Select is High when active (CS = 1)
 */
#define SW_SPI_CS               (0)

#define SW_SPI_CS_GPIO_PIN      (GPIO_PIN_33)
#define SW_SPI_CLK_GPIO_PIN     (GPIO_PIN_32)
#define SW_SPI_MOSI_GPIO_PIN    (GPIO_PIN_34)
#define SW_SPI_MISO_GPIO_PIN    (GPIO_PIN_35)

#define SW_SPI_DELAY            drvi_ClockDelayUs (2)

#define SET_SW_SPI_CS_INACTIVE  drvi_GpioWrite (SW_SPI_CS_GPIO_PIN, !(SW_SPI_CS))
#define SET_SW_SPI_CS_ACTIVE    drvi_GpioWrite (SW_SPI_CS_GPIO_PIN, !!(SW_SPI_CS))

#define SET_SW_SPI_CLK_INACTIVE drvi_GpioWrite (SW_SPI_CLK_GPIO_PIN, !!(SW_SPI_CPOL))
#define SET_SW_SPI_CLK_ACTIVE   drvi_GpioWrite (SW_SPI_CLK_GPIO_PIN, !(SW_SPI_CPOL))

#define SET_SW_SPI_MOSI_HIGH    drvi_GpioWrite (SW_SPI_MOSI_GPIO_PIN, CC6801_GPIO_OUTPUT_HIGH)
#define SET_SW_SPI_MOSI_LOW     drvi_GpioWrite (SW_SPI_MOSI_GPIO_PIN, CC6801_GPIO_OUTPUT_LOW)

#define GET_SW_SPI_MISO         drvi_GpioRead (SW_SPI_MISO_GPIO_PIN)

void
cc6801_sw_spi_init (
    void
    );

void
cc6801_sw_spi_write (
    uint8_t const   command,
    uint8_t const   * const p_write,
    int32_t const   write_length
    );

void
cc6801_sw_spi_write_then_read (
    uint8_t const   command,
    uint8_t const   * const p_write,
    int32_t const   write_length,
    int32_t const   dummy_length,
    uint8_t         * const p_read,
    int32_t const   read_length
    );

#endif /* } #ifndef _SW_SPI_H_ */
