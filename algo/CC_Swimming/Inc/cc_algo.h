// Objective: The header file for APIs with system framework
// Created Date: 2017/04/10
// Last Modified Date: 2017/09/22
// Author: Frank Yu
// Department: System & Application
// (c) Copyright 2017
// Cloudchip, Inc.
// [ 3235 Kifer Road, Suite 150, Santa Clara, CA 95051, USA ]
// All Rights Reserved

#ifndef CC_ALGO_H
#define CC_ALGO_H

#include <stdlib.h>
#include <string.h>
#include "cc_algo_event.h"

#define SWIMMING_VERSION_AlGOTITHM_MAJOR                        3
#define SWIMMING_VERSION_AlGOTITHM_MINOR                        2
#define SWIMMING_VERSION_AlGOTITHM_BUILD                        0

#define SWIMMING_VERSION_INTERFACE_MAJOR                        1
#define SWIMMING_VERSION_INTERFACE_MINOR                        0
#define SWIMMING_VERSION_INTERFACE_BUILD                        4

#define ACC_BIT                                                 0
#define MAG_BIT                                                 1
#define GYRO_BIT                                                2
#define ACC_MASK                                                0x01
#define MAG_MASK                                                0x02
#define GYRO_MASK                                               0x04
#define LPF_ALPHA                                               0.2f//0.8f

#define ACC_FACTOR                                              0.0002441406f
#define MAG_FACTOR                                              0.1495361f
#define GYRO_FACTOR                                             0.06103516f

#define SENSOR_NUM                                              3
#define SENSOR_LEN                                              3

void CC_Swim_Init(uint8_t len, band_location loc);
void CC_Swim_DeInit(void);
void CC_Swimming_Proc(const raw_sensor_event raw_event, result_event* presult_event);
unsigned long CC_Swimming_get_version(void);
#endif // CC_ALGO_H
