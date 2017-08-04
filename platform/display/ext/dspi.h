/**
 ****************************************************************************************
 *
 * @file dspi.h
 *
 * @brief Head file of dspi.c
 *
 * Copyright (C) CloudChip 2017-2019
 *
 *
 ****************************************************************************************
 */
#ifndef _DSPI_H_
#define _DSPI_H_
#include "dsp_config.h"

#if (DISPLAY_SUPPORT)
#if (DISPLAY_OLED_SSD1306)
#include "ssd1306_spi.h"
#include "ssd1306_128x32.h"

#define dspi_Init()                                   ssd1306_Init()
#define dspi_Reset()                                  ssd1306_Reset()
#define dspi_HardReset()                              ssd1306_HardReset()
#define dspi_DisplayOn()                              ssd1306_DisplayOn()
#define dspi_DisplayOff()                             ssd1306_DisplayOff()
#define dspi_Sleep()                                  ssd1306_Sleep()
#define dspi_WakeUp()                                 ssd1306_WakeUp()

#define dspi_DrawBlack()                              ssd1306_DrawBlack()
#define dspi_DrawWhite()                              ssd1306_DrawWhite()
#define dspi_DrawPixel6x8(x,y,buf)                    ssd1306_DrawPixel6x8(x,y,buf)
#define dspi_DrawPixel8x16(x,y,buf)                   ssd1306_DrawPixel8x16(x,y,buf)
#define dspi_DrawFrameBuffer(fb,size,x,y)             ssd1306_DrawFrameBuffer(fb,size,x,y)
#define dspi_DrawBitmap(x0,y0,x1,y1,bmp)              ssd1306_DrawBitmap(x0,y0,x1,y1,bmp)

#endif //DISPLAY_OLED_SSD1306

#else

#define dspi_Init()
#define dspi_Reset()
#define dspi_HardReset()
#define dspi_DisplayOn()
#define dspi_DisplayOff()
#define dspi_Sleep()
#define dspi_WakeUp()

#define dspi_DrawBlack()
#define dspi_DrawWhite()
#define dspi_DrawPixel6x8(x,y,buf)
#define dspi_DrawPixel8x16(x,y,buf)
#define dspi_DrawFrameBuffer(fb,size,x,y)
#define dspi_DrawBitmap(x0,y0,x1,y1,bmp)

#endif // DISPLAY_SUPPORT














#endif //_DSPI_H_


