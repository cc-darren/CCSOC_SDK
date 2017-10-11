// Objective: The definition of data structure for algorithm
// Created Date: 2017/04/10
// Last Modified Date: 2017/06/24
// Author: Frank Yu
// Department: System & Application
// (c) Copyright 2017
// Cloudchip, Inc.
// [ 3235 Kifer Road, Suite 150, Santa Clara, CA 95051, USA ]
// All Rights Reserved

#ifndef CC_ALGO_EVENT_H
#define CC_ALGO_EVENT_H

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "stdint.h"

typedef enum {
  ACC_SENSOR = 1,
  MAG_SENSOR = 2,
  GYRO_SENSOR = 4,
} sensor_type;

typedef enum {
  LEFT_HAND = 0,
  RIGHT_HAND,
} band_location;

typedef enum {
  LEN_25M = 0,
  LEN_50M,
  LEN_25YD,
  LEN_33_33M,
  LEN_33_33YD,
  LEN_UNKNOWN,
} swimming_pool_len;

typedef enum {
  SWIMMING_OTHERS = 0,
  SWIMMING_FRONTCRAWL,
  SWIMMING_BACKSTROKE,
  SWIMMING_BREASTSTROKE,
  SWIMMING_BUTTERFLYSTROKE,
  SWIMMING_HEADUPBREASTSTROKE,
} swim_style;

typedef enum {
  NONE = 0,
  SWIMMING_TIMES,
  SWIMMING_STYLE,
  SWIMMING_LAP,
} result_type;

typedef enum {
  NONE0 = 0,
} style_type;

typedef struct {
  unsigned char type;
  short acc_data[3];
  short mag_data[3];
  short gyro_data[3];
  //unsigned long timestamp;
} raw_sensor_event;

typedef struct {
  //sensor_type type;
  float data[6];
  //unsigned long timestamp;
} sensor_event;

typedef struct {
#if 1 //CL
  swim_style swimstyle;
  uint32_t swimcount;
  uint8_t swimlap;
  unsigned long timestamp;
#else
  result_type type;
  style_type style;
  uint32_t data;
  unsigned long timestamp;
#endif
} result_event;

typedef struct {
  swim_style style;
  uint64_t timestamp;
} swim_style_result;
#endif // CC_ALGO_EVENT_H
