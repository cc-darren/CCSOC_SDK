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
#ifndef _PRESSURE_NULL_DRIVER_H
#define _PRESSURE_NULL_DRIVER_H

/* Exported macro ------------------------------------------------------------*/
//PRESSURE sensor interface
#define PRESSURE_Init()
#define PRESSURE_Enable()
#define PRESSURE_Disable()
#define PRESSURE_SetMode(modeSetting)
#define PRESSURE_SetOdr(OdrSetting)
#define PRESSURE_GetPRESSURERawData(pBuf,bBuf_size)
#define PRESSURE_Reset()

/* Exported constants --------------------------------------------------------*/

#endif //_PRESSURE_NULL_DRIVER_H

