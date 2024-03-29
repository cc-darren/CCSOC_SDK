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

#if (MODULE_OLED == OLED_SOLOMON_SH1107)

#ifndef _SH1107_SPI_H_
#define _SH1107_SPI_H_

#include <stdint.h>
#include "project.h"
#include "drvi_gpio.h"
#include "sh1107_128x32.h"

/* Exported macro ------------------------------------------------------------*/
//OLED interface
#ifndef _OLED_SHARE_TYPE_
#define _OLED_SHARE_TYPE_
#define oled_Init()                                   sh1107_Init()
#define oled_Reset()                                  sh1107_Reset()
#define oled_HardReset()                              sh1107_HardReset()
#define oled_DisplayOn()                              sh1107_DisplayOn()
#define oled_DisplayOff()                             sh1107_DisplayOff()
#define oled_Sleep()                                  sh1107_Sleep()
#define oled_WakeUp()                                 sh1107_WakeUp()

#define oled_DrawBlack()                              sh1107_DrawBlack()
#define oled_DrawWhite()                              sh1107_DrawWhite()
#define oled_DrawPixel6x8(x,y,buf)                    sh1107_DrawPixel6x8(x,y,buf)
#define oled_DrawPixel8x16(x,y,buf)                   sh1107_DrawPixel8x16(x,y,buf)
#define oled_DrawPixel8x16_Thin(x,y,buf)              sh1107_DrawPixel8x16_Thin(x,y,buf)
#define oled_DrawPixel2x24(x,y,buf)                   sh1107_DrawPixel2x24(x,y,buf)
#define oled_DrawPixel2x32(x,y,buf)                   sh1107_DrawPixel2x32(x,y,buf)
#define oled_DrawPixel4x38(x,y,buf)                   sh1107_DrawPixe14x38(x,y,buf)
#define oled_DrawPixel8x36(x,y,buf)                   sh1107_DrawPixel8x36(x,y,buf)
#define oled_DrawFrameBuffer(fb,size,x,y)             sh1107_DrawFrameBuffer(fb,size,x,y)
#define oled_DrawBitmap(x0,y0,x1,y1,bmp)              sh1107_DrawBitmap(x0,y0,x1,y1,bmp)

#define oled_SendData(bByte)                          sh1107_SendData(bByte)
#define oled_SetPosition(x,y)                         sh1107_SetPosition(x,y)
#define oled_SetPwrCtrlHigh()                         sh1107_SetPwrCtlHigh()
#define oled_SetPwrCtrlLow()                          sh1107_SetPwrCtlLow()



#endif //_OLED_SHARE_TYPE_

#define OLED_SH1107_SPI                              OLED_IF_ID
#define OLED_SH1107_PWR                              OLED_PWR_PIN
#define OLED_SH1107_RST                              OLED_RST_PIN
#define OLED_SH1107_DC                               OLED_DC_PIN

#define SH1107_LCDWIDTH                              128
#define SH1107_LCDHEIGHT                             32

#define SH1107_CMD_COL_ADD_SET_LSB(column)           (0x00 | (column))
#define SH1107_CMD_COL_ADD_SET_MSB(column)           (0x10 | (column))
#define SH1107_CMD_SET_ADDRESSING_MODE_PAGE          0x20
#define SH1107_CMD_SET_ADDRESSING_MODE_VERTICAL      0x21

#define SH1107_CMD_SET_DISPLAY_START_LINE(line)      (0x40 | (line))
#define SH1107_CMD_SET_CONTRAST_CONTROL_FOR_BANK0    0x81
#define SH1107_CMD_SET_CHARGE_PUMP_SETTING           0x8D
#define SH1107_CMD_SET_SEGMENT_RE_MAP_COL0_SEG0      0xA0
#define SH1107_CMD_SET_SEGMENT_RE_MAP_COL127_SEG0    0xA1
#define SH1107_CMD_ENTIRE_DISPLAY_AND_GDDRAM_ON      0xA4
#define SH1107_CMD_ENTIRE_DISPLAY_ON                 0xA5
#define SH1107_CMD_SET_NORMAL_DISPLAY                0xA6
#define SH1107_CMD_SET_INVERSE_DISPLAY               0xA7
#define SH1107_CMD_SET_MULTIPLEX_RATIO               0xA8
#define SH1107_CMD_SET_DISPLAY_ON                    0xAF
#define SH1107_CMD_SET_DISPLAY_OFF                   0xAE
#define SH1107_CMD_SET_PAGE_START_ADDRESS(page)      (0xB0 | (page))
#define SH1107_CMD_SET_COM_OUTPUT_SCAN_UP            0xC0
#define SH1107_CMD_SET_COM_OUTPUT_SCAN_DOWN          0xC8
#define SH1107_CMD_SET_DISPLAY_OFFSET                0xD3
#define SH1107_CMD_SET_DISPLAY_CLOCK_DIVIDE_RATIO    0xD5
#define SH1107_CMD_SET_PRE_CHARGE_PERIOD             0xD9
#define SH1107_CMD_SET_COM_PINS                      0xDA
#define SH1107_CMD_SET_VCOMH_DESELECT_LEVEL          0xDB
#define SH1107_CMD_NOP                               0xE3
#define SH1107_MAXROWS                               4

// address setting
#define SH1107_SETLOWCOLUMN                          0x00
#define SH1107_SETHIGHCOLUMN                         0x10
#define SH1107_MEMORYMODE                            0x20
#define SH1107_COLUMNADDRESS                         0x21
#define SH1107_COLUMNADDRESS_MSB                     0x00
#define SH1107_COLUMNADDRESS_LSB                     0x7F
#define SH1107_PAGEADDRESS                           0x22
#define SH1107_PAGE_START_ADDRESS                    0xB0

void sh1107_SetPwrCtlHigh(void);
void sh1107_SetPwrCtlLow(void);
void sh1107_SetDataCmdHigh(void);
void sh1107_SetDataCmdLow(void);
void sh1107_SetResetHigh(void);
void sh1107_SetResetLow(void);

void sh1107_Init(void);
void sh1107_Reset(void);
void sh1107_HardReset(void);

void sh1107_SendCommand(uint8_t bByte);
void sh1107_SendData(uint8_t bByte);

void sh1107_DisplayOn(void);
void sh1107_DisplayOff(void);

void sh1107_Sleep(void);
void sh1107_WakeUp(void);

#endif //_SH1107_SPI_H_

#endif
