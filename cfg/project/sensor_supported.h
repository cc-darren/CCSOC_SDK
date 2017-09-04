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
*  sensor_supported.h
*
*  Project:
*  --------
*  All
*
*  Description:
*  ------------
*  supported sensors list
*
*  Author:
*  -------
*  PPP   (CC-pattaya)
*
*===========================================================================
*
******************************************************************************/
#ifndef _SENSOR_SUPPORTED_H_
#define _SENSOR_SUPPORTED_H_

//Supported ACC sensors
#define ACC_ST_LSM303C          0x1001
#define ACC_ST_LSM303D          0x1002
#define ACC_ST_LIS2DH12         0x1003
#define ACC_ST_LIS2DW12         0x1004
#define ACC_KIX_KX022_1020      0x1005
#define ACC_KIX_KXG03           0x1006
#define ACC_AD_ADXL362          0x1007
#define ACC_ST_LSM6DSL          0x1008

//Supported MAG sensors
#define MAG_AKM_AK09912C        0x2001
#define MAG_AKM_AK09915C        0x2002
#define MAG_ST_LSM303C          0x2003
#define MAG_ST_LSM303D          0x2004
#define MAG_ST_LIS2MDL          0x2005


//Supported GYRO sensors
#define GYR_ST_LSM6DSL          0x3001



#endif  //_SENSOR_SUPPORTED_H_
