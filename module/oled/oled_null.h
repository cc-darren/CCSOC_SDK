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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _OLED_NULL_DRIVER_H
#define _OLED_NULL_DRIVER_H



/* Exported macro ------------------------------------------------------------*/
//OLED interface
#ifndef _OLED_SHARE_TYPE_
#define _OLED_SHARE_TYPE_
#define oled_Init()
#define oled_Reset()
#define oled_HardReset()
#define oled_DisplayOn()
#define oled_DisplayOff()
#define oled_Sleep()
#define oled_WakeUp()

#define oled_DrawBlack()
#define oled_DrawWhite()
#define oled_DrawPixel6x8(x,y,buf)
#define oled_DrawPixel8x16(x,y,buf)
#define oled_DrawFrameBuffer(fb,size,x,y)
#define oled_DrawBitmap(x0,y0,x1,y1,bmp)
#endif //_OLED_SHARE_TYPE_


/* Exported constants --------------------------------------------------------*/

#endif //_OLED_NULL_DRIVER_H
