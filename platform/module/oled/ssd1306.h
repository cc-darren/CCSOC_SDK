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

#ifndef _SSD1306_SPI_H_
#define _SSD1306_SPI_H_

#include <stdint.h>
#include "project.h"
#include "drvi_gpio.h"
#include "ssd1306_128x32.h"

/* Exported macro ------------------------------------------------------------*/
//OLED interface
#ifndef _OLED_SHARE_TYPE_
#define _OLED_SHARE_TYPE_
#define oled_Init()                                   ssd1306_Init()
#define oled_Reset()                                  ssd1306_Reset()
#define oled_HardReset()                              ssd1306_HardReset()
#define oled_DisplayOn()                              ssd1306_DisplayOn()
#define oled_DisplayOff()                             ssd1306_DisplayOff()
#define oled_Sleep()                                  ssd1306_Sleep()
#define oled_WakeUp()                                 ssd1306_WakeUp()

#define oled_DrawBlack()                              ssd1306_DrawBlack()
#define oled_DrawWhite()                              ssd1306_DrawWhite()
#define oled_DrawPixel6x8(x,y,buf)                    ssd1306_DrawPixel6x8(x,y,buf)
#define oled_DrawPixel8x16(x,y,buf)                   ssd1306_DrawPixel8x16(x,y,buf)
#define oled_DrawFrameBuffer(fb,size,x,y)             ssd1306_DrawFrameBuffer(fb,size,x,y)
#define oled_DrawBitmap(x0,y0,x1,y1,bmp)              ssd1306_DrawBitmap(x0,y0,x1,y1,bmp)
#endif //_OLED_SHARE_TYPE_

#define OLED_SSD1306_SPI                              OLED_IF_ID
#define OLED_SSD1306_PWR                              GPIO_PIN_15
#define OLED_SSD1306_RST                              GPIO_PIN_31
#define OLED_SSD1306_DC                               GPIO_PIN_14

#define SSD1306_LCDWIDTH                              128
#define SSD1306_LCDHEIGHT                             32

#define SSD1306_CMD_COL_ADD_SET_LSB(column)           (0x00 | (column))
#define SSD1306_CMD_COL_ADD_SET_MSB(column)           (0x10 | (column))
#define SSD1306_CMD_SET_MEMORY_ADDRESSING_MODE        0x20
#define SSD1306_CMD_SET_COLUMN_ADDRESS                0x21
#define SSD1306_CMD_SET_PAGE_ADDRESS                  0x22
#define SSD1306_CMD_SET_DISPLAY_START_LINE(line)      (0x40 | (line))
#define SSD1306_CMD_SET_CONTRAST_CONTROL_FOR_BANK0    0x81
#define SSD1306_CMD_SET_CHARGE_PUMP_SETTING           0x8D
#define SSD1306_CMD_SET_SEGMENT_RE_MAP_COL0_SEG0      0xA0
#define SSD1306_CMD_SET_SEGMENT_RE_MAP_COL127_SEG0    0xA1
#define SSD1306_CMD_ENTIRE_DISPLAY_AND_GDDRAM_ON      0xA4
#define SSD1306_CMD_ENTIRE_DISPLAY_ON                 0xA5
#define SSD1306_CMD_SET_NORMAL_DISPLAY                0xA6
#define SSD1306_CMD_SET_INVERSE_DISPLAY               0xA7
#define SSD1306_CMD_SET_MULTIPLEX_RATIO               0xA8
#define SSD1306_CMD_SET_DISPLAY_ON                    0xAF
#define SSD1306_CMD_SET_DISPLAY_OFF                   0xAE
#define SSD1306_CMD_SET_PAGE_START_ADDRESS(page)      (0xB0 | (page))
#define SSD1306_CMD_SET_COM_OUTPUT_SCAN_UP            0xC0
#define SSD1306_CMD_SET_COM_OUTPUT_SCAN_DOWN          0xC8
#define SSD1306_CMD_SET_DISPLAY_OFFSET                0xD3
#define SSD1306_CMD_SET_DISPLAY_CLOCK_DIVIDE_RATIO    0xD5
#define SSD1306_CMD_SET_PRE_CHARGE_PERIOD             0xD9
#define SSD1306_CMD_SET_COM_PINS                      0xDA
#define SSD1306_CMD_SET_VCOMH_DESELECT_LEVEL          0xDB
#define SSD1306_CMD_NOP                               0xE3
#define SSD1306_MAXROWS                               4

// address setting
#define SSD1306_SETLOWCOLUMN                          0x00
#define SSD1306_SETHIGHCOLUMN                         0x10
#define SSD1306_MEMORYMODE                            0x20
#define SSD1306_COLUMNADDRESS                         0x21
#define SSD1306_COLUMNADDRESS_MSB                     0x00
#define SSD1306_COLUMNADDRESS_LSB                     0x7F
#define SSD1306_PAGEADDRESS                           0x22
#define SSD1306_PAGE_START_ADDRESS                    0xB0

void ssd1306_Init(void);
void ssd1306_Reset(void);
void ssd1306_HardReset(void);

void ssd1306_SendCommand(uint8_t bByte);
void ssd1306_SendData(uint8_t bByte);

void ssd1306_DisplayOn(void);
void ssd1306_DisplayOff(void);

void ssd1306_Sleep(void);
void ssd1306_WakeUp(void);

#endif //_SSD1306_SPI_H_
