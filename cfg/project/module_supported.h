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
*  Filename:
*  ---------
*  module_supported.h
*
*  Project:
*  --------
*  All
*
*  Description:
*  ------------
*  supported modules list
*
*  Author:
*  -------
*  PPP   (CC-pattaya)
*
*===========================================================================
*
******************************************************************************/
#ifndef _MODULE_SUPPORTED_H_
#define _MODULE_SUPPORTED_H_

//Supported ACC sensors
#define ACC_ST_LSM303C          0x1001
#define ACC_ST_LSM303D          0x1002
#define ACC_ST_LIS2DH12         0x1003
#define ACC_ST_LIS2DW12         0x1004
#define ACC_KIX_KX022_1020      0x1005
#define ACC_KIX_KXG03           0x1006
#define ACC_AD_ADXL362          0x1007
#define ACC_ST_LSM6DSL          0x1008
#define ACC_NULL                0x1FFF //choose this if ACC is not in using

//Supported MAG sensors
#define MAG_AKM_AK09912C        0x2001
#define MAG_AKM_AK09915C        0x2002
#define MAG_ST_LSM303C          0x2003
#define MAG_ST_LSM303D          0x2004
#define MAG_ST_LIS2MDL          0x2005
#define MAG_NULL                0x2FFF //choose this if MAG is not in using


//Supported GYRO sensors
#define GYR_ST_LSM6DSL          0x3001
#define GYR_NULL                0x3FFF //choose this if GYR is not in using


//Supported OLED displays
#define OLED_SOLOMON_SSD1306    0x4001
#define OLED_SOLOMON_SH1107     0x4002
#define OLED_JDI_LPM013M126A    0x4003
#define OLED_JDI_LPM010M297B    0x4004
#define OLED_NULL               0x4FFF //choose this if OLED is not in using

//Supported PPG sensors
#define PPG_PXT_PAH8002         0x5001
#define PPG_NULL                0x5FFF //choose this if PPG is not in using

//Supported ADC modules
#define ADC_MCP_MCP3421         0x6001
#define ADC_NULL                0x6FFF //choose this if ADC is not in using

//Supported PRESSURE modules
#define PRESSURE_LPS33HW        0x7001
#define PRESSURE_NULL           0x7FFF //choose this if pressure IC is not in using

#define TOUCH_FOCAL_FT6X36      0x8001
#define TOUCH_NULL              0x8FFF //choose this if touch IC is not in used

#define FLASH_W25Q80DV          0x9001
#define FLASH_W25Q256JV         0x9002
#define FLASH_NULL              0x9FFF //choose this if flash is not in used

#endif  //_MODULE_SUPPORTED_H_
