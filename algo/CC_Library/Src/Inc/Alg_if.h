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


#ifndef _ALG_IF_
#define _ALG_IF_

#include "global.h"

typedef struct
{
	uint16_t wAcc[3];	
}S_AccInput;

typedef struct
{
	uint16_t wGyro[3];
	
}S_GyroInput;

typedef struct
{
	uint16_t wMag[3];
	
}S_MagInput;

extern int Alg_Ped(S_AccInput sInput);
extern int Alg_Sleep(S_AccInput sInput);
extern int Alg_Liftarm(S_AccInput sInput);
extern int Alg_Longsit(uint32_t dwInputData);
extern int Alg_Swimming(S_AccInput sInputData1,S_GyroInput sInputData2,S_MagInput sInputData3);
#endif
