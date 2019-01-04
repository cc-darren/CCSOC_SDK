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

#ifdef EMWIN_ENABLE
#if ((MODULE_OLED == OLED_JDI_LPM013M126A) || (MODULE_OLED == OLED_JDI_LPM010M297B))

#ifndef _JDI_LCD_DRV_
#define _JDI_LCD_DRV_

#include "global.h"

void OLED_JDI_Drv_Init(void);
void OLED_JDI_Drv_Start_Draw(void);
void OLED_JDI_Drv_End_Draw(void);
void OLED_JDI_Drv_PutPixel(uint8_t xPhys, uint8_t yPhys, uint8_t PixelIndex);
unsigned int OLED_JDI_Drv_GetPixel(uint8_t xPhys, uint8_t yPhys);

void JDI_Demo(void);

#endif


#endif

#endif
