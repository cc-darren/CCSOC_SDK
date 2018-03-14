/* Copyright (c) 2018 Cloudchip, Inc. All Rights Reserved.
 *
 * The information contained herein is property of Cloudchip, Inc.
 * Terms and conditions of usage are described in detail in CLOUDCHIP
 * STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information.
 * NO WARRANTY of ANY KIND is provided. This heading must NOT be removed 
 * from the file.
 */

#ifndef _JDI_LCD_DRV_
#define _JDI_LCD_DRV_

#include "global.h"

void JDI_LCD_Init(void);
void JDI_Demo(void);
void JDI_Start_Draw(void);
void JDI_End_Draw(void);
void JDI_PutPixel(uint8_t xPhys, uint8_t yPhys, uint8_t PixelIndex);
unsigned int JDI_GetPixel(uint8_t xPhys, uint8_t yPhys);

void JDI_Demo(void);

#endif
