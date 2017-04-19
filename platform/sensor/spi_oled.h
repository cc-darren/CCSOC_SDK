/**
 ****************************************************************************************
 *
 * @file spi_sensor.h
 *
 * @brief  SFL & Sensor Configuration Functions.
 *
 * Copyright (C) 2015. Dialog Semiconductor Ltd, unpublished work. This computer
 * program includes Confidential, Proprietary Information and is a Trade Secret of
 * Dialog Semiconductor Ltd.  All use, disclosure, and/or reproduction is prohibited
 * unless authorized in writing. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com> and contributors.
 *
 ****************************************************************************************
 */

#ifndef _SPI_OLED_H_
#define _SPI_OLED_H_

#include <stdint.h>
#include "ssd1306_oled_driver.h"


#define SPI2_INSTANCE  2 /**< SPI instance index. */


#define SSD1306_LCDWIDTH                  128
#define SSD1306_LCDHEIGHT                 32


#define SSD1306_CMD_COL_ADD_SET_LSB(column)         (0x00 | (column))
#define SSD1306_CMD_COL_ADD_SET_MSB(column)         (0x10 | (column))
#define SSD1306_CMD_SET_MEMORY_ADDRESSING_MODE      0x20
#define SSD1306_CMD_SET_COLUMN_ADDRESS              0x21
#define SSD1306_CMD_SET_PAGE_ADDRESS                0x22
#define SSD1306_CMD_SET_DISPLAY_START_LINE(line)    (0x40 | (line))
#define SSD1306_CMD_SET_CONTRAST_CONTROL_FOR_BANK0  0x81
#define SSD1306_CMD_SET_CHARGE_PUMP_SETTING         0x8D
#define SSD1306_CMD_SET_SEGMENT_RE_MAP_COL0_SEG0    0xA0
#define SSD1306_CMD_SET_SEGMENT_RE_MAP_COL127_SEG0  0xA1
#define SSD1306_CMD_ENTIRE_DISPLAY_AND_GDDRAM_ON    0xA4
#define SSD1306_CMD_ENTIRE_DISPLAY_ON               0xA5
#define SSD1306_CMD_SET_NORMAL_DISPLAY              0xA6
#define SSD1306_CMD_SET_INVERSE_DISPLAY             0xA7
#define SSD1306_CMD_SET_MULTIPLEX_RATIO             0xA8
#define SSD1306_CMD_SET_DISPLAY_ON                  0xAF
#define SSD1306_CMD_SET_DISPLAY_OFF                 0xAE
#define SSD1306_CMD_SET_PAGE_START_ADDRESS(page)    (0xB0 | (page))
#define SSD1306_CMD_SET_COM_OUTPUT_SCAN_UP          0xC0
#define SSD1306_CMD_SET_COM_OUTPUT_SCAN_DOWN        0xC8
#define SSD1306_CMD_SET_DISPLAY_OFFSET              0xD3
#define SSD1306_CMD_SET_DISPLAY_CLOCK_DIVIDE_RATIO  0xD5
#define SSD1306_CMD_SET_PRE_CHARGE_PERIOD           0xD9
#define SSD1306_CMD_SET_COM_PINS                    0xDA
#define SSD1306_CMD_SET_VCOMH_DESELECT_LEVEL        0xDB
#define SSD1306_CMD_NOP                             0xE3
#define SSD1306_MAXROWS 4

	// address setting
#define SSD1306_SETLOWCOLUMN 0x00
#define SSD1306_SETHIGHCOLUMN 0x10
#define SSD1306_MEMORYMODE 0x20
#define SSD1306_COLUMNADDRESS 0x21
#define SSD1306_COLUMNADDRESS_MSB 0x00
#define SSD1306_COLUMNADDRESS_LSB 0x7F
#define SSD1306_PAGEADDRESS	0x22
#define SSD1306_PAGE_START_ADDRESS 0xB0

/**
 ****************************************************************************************
 * @brief LIS2DH clr cs
 ****************************************************************************************
 */
inline void set_oled_pwrctl_hi( void )__attribute__((always_inline));
inline void set_oled_pwrctl_lo( void )__attribute__((always_inline));
inline void set_oled_dc_hi( void )__attribute__((always_inline));
inline void set_oled_dc_lo( void )__attribute__((always_inline));
inline void set_oled_reset_hi( void )__attribute__((always_inline));
inline void set_oled_reset_lo( void )__attribute__((always_inline));



/**
 ****************************************************************************************
 * @brief initializes the DA1458x SPI peripheral
 *
 * @param[in] none
 *
 * @return none
 ****************************************************************************************
 */
void oled_spi_init( void );
void oled_spi_release(void);
void oled_pins_init( void ) ;
void ssd1306_init(void);
void ssd1306_reset(void);

void ssd1306_hard_reset(void);
void ssd1306_interface_init(void);
void ssd1306_write_command(uint8_t command);
void ssd1306_write_data(uint8_t command);
uint8_t ssd1306_set_contrast(uint8_t contrast);
void ssd1306_display_invert_disable(void);
void ssd1306_display_on(void);
void ssd1306_display_off(void);

void OLED_Display_White(void);
void OLED_Display_Black(void);





#endif //_SPI_SENSOR_H_
