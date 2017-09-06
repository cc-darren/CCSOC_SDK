/******************************************************************************
*  Copyright 2017, Cloudchip, Inc.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Cloudchip, Inc. (C) 2017
******************************************************************************/

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
#define OLED_NULL               0x4FFF //choose this if OLED is not in using

#endif  //_MODULE_SUPPORTED_H_
