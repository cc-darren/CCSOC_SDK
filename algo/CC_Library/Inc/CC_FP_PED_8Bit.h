/*****************************************************************************
 * Motion Library: FP_PED_8Bit
 * Version: 1.0.1
 *
 * Revision Description:
 * 1) 3 tunable parameters added for customer  (Apr. 19th, 2016)
 *
 * Copyright (c) 2016 mCube, Inc.  All rights reserved.
 *
 * This source is subject to the mCube Software License.
 * This software is protected by Copyright and the information and source code
 * contained herein is confidential. The software including the source code
 * may not be copied and the information contained herein may not be used or
 * disclosed except with the written permission of mCube Inc.
 *
 * All other rights reserved.
 *****************************************************************************/

#ifndef __FP_PED_8BIT_H__
#define __FP_PED_8BIT_H__
#include <stddef.h>
#include <stdlib.h>
#include "rtwtypes.h"

typedef struct {
  /*For Customer Para-Tuning*/
  short FirstBlockingSteps; //Default:: 10
  short MinRunningSteps; //Default:: 6
  short MinRunningForce; //Default:: 2500
} struct_T2;


extern void FP_PED_8Bit(const short FP_AccIn[3], uint32_t *gTotal_Step, signed char
  *gLibPedState);
extern void FP_PED_8Bit_initialize(void);//(const struct_T2 iParaInput);
extern void FP_PED_8Bit_terminate(void);
extern void CC_Reset_PedCount(void);


#endif
