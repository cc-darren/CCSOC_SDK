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

#include "Alg_if.h"
#include "tracer.h"

/******************************************************************************
*  Filename:
*  ---------
*  Alg_if.c
*
*  Project:
*  --------
*  cc6801
*
*  Description:
*  ------------
*  Accelerometer, Gyroscope and Magnetometer sensor config, taking care of,
*      (1) Accelerometer ODR config LSM6DS3_ACC_GYRO_ODR_XL_52Hz 
*      (2) Accelerometer Resolution config LSM6DS3_ACC_GYRO_FS_XL_8g, 14bits
*      (3) Gyroscope ODR config LSM6DS3_ACC_GYRO_ODR_G_52Hz
*      (4) Gryoscope Resolution config LSM6DS3_ACC_GYRO_FS_G_2000dps
*      (5) Magnetometer ODR AK09912_MAG_DO_100_Hz
*  Author:
*  -------
*  CloudChip
*
*===========================================================================/
*  20180209 Jason initial version
******************************************************************************/
int Alg_Ped(S_AccInput sInput)
{
	S_AccInput _sFakeData;
	_sFakeData.wAcc[0] = 1;
	_sFakeData.wAcc[1] = 1;
	_sFakeData.wAcc[2] = 4096;
	TracerInfo("==Alg_Ped %d %d %d == \r\n", _sFakeData.wAcc[0],_sFakeData.wAcc[1],_sFakeData.wAcc[2]);
	
	return 0;
}

int Alg_Sleep(S_AccInput sInput)
{
    S_AccInput _sFakeData;
	_sFakeData.wAcc[0] = 1;
	_sFakeData.wAcc[1] = 1;
	_sFakeData.wAcc[2] = 4096;
	TracerInfo("==Alg_Sleep %d %d %d == \r\n", _sFakeData.wAcc[0],_sFakeData.wAcc[1],_sFakeData.wAcc[2]);
	
	return 0;
}

int Alg_Liftarm(S_AccInput sInput)
{
    S_AccInput _sFakeData;
	_sFakeData.wAcc[0] = 1;
	_sFakeData.wAcc[1] = 1;
	_sFakeData.wAcc[2] = 4096;
	TracerInfo("==Alg_Liftarm %d %d %d == \r\n", _sFakeData.wAcc[0],_sFakeData.wAcc[1],_sFakeData.wAcc[2]);
	return 0;
}

int Alg_Longsit(uint32_t dwInputData)
{

	TracerInfo("==Alg_Longsit %d == \r\n", dwInputData);
	return 0;
}

int Alg_Swimming(S_AccInput sInputData1,S_GyroInput sInputData2,S_MagInput sInputData3)
{
    S_AccInput _sFakeData1;
    S_GyroInput _sFakeData2;
    S_MagInput _sFakeData3;
	_sFakeData1.wAcc[0] = 1;
	_sFakeData1.wAcc[1] = 1;
	_sFakeData1.wAcc[2] = 4096;
    
    _sFakeData2.wGyro[0] = 10;
	_sFakeData2.wGyro[1] = 10;
	_sFakeData2.wGyro[2] = 10;
    
    _sFakeData3.wMag[0] = 1;
	_sFakeData3.wMag[1] = 1;
	_sFakeData3.wMag[2] = 1;
    
	TracerInfo("==Alg_Swimming == \r\n");
    TracerInfo( "Acc %d %d %d \r\n ",_sFakeData1.wAcc[0],_sFakeData1.wAcc[1],_sFakeData1.wAcc[2]);
    TracerInfo( "Acc %d %d %d \r\n ",_sFakeData2.wGyro[0],_sFakeData2.wGyro[1],_sFakeData2.wGyro[2]);
    TracerInfo( "Acc %d %d %d \r\n ",_sFakeData3.wMag[0],_sFakeData3.wMag[1],_sFakeData3.wMag[2]);

	return 0;
}
