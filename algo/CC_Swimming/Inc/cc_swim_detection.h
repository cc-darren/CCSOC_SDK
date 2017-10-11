// Objective: The header file for algorithm
// Created Date: 2017/04/10
// Last Modified Date: 2017/09/27
// Author: Frank Yu
// Department: System & Application
// (c) Copyright 2017
// Cloudchip, Inc.
// [ 3235 Kifer Road, Suite 150, Santa Clara, CA 95051, USA ]
// All Rights Reserved
#ifndef CC_SWIM_DETECTION_H
#define CC_SWIM_DETECTION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "cc_algo.h"

#define VENUS_UART_OUTPUT                                       0
#define VENUS_FLASH_LOG                                         0

//#define TRUE                                                    1
//#define FALSE                                                   0

#define ACC_TIMESTAMP_UNIT                                      20              // Unit: ms
#define GYRO_TIMESTAMP_UNIT                                     20              // Unit: ms
#define MAG_TIMESTAMP_UNIT                                      100             // Unit: ms

// For stroke detection
#define STROKE_TIMESTAMP_BUFFER_NUM                             30
#define ACC_SUM_THRES_UP                                        1.5f//1.3f//1.7f//1.9f
#define ACC_SUM_THRES_DOWN                                      0.95f//1.2f
#define GYRO_SUM_THRES_UP                                       250.0f//220.0f//300.0f
#define GYRO_SUM_THRES_DOWN                                     100.0f//150.0f
#define MIN_GAP_ST                                              600//1300            // Unit: ms
#define MAX_GAP_ST                                              3000            // Unit: ms
#define TIME_DIFF_RESET                                         10000
#define TIME_GAP_STROKES_NUM                                    20
#define AFTER_LAP_TIMESTAMP_GAP                                 3000
#define STROKES_THRESHOLD                                       2

// For style detection
  // For professionals
#define PRO_GYRO_X_Z_STD_TH                                     10.0f
#define PRO_GYRO_BUFFER_NUM                                     10//5
#define PRO_GYRO_TIME_DIFF                                      25

  // For generals
#define CHECK_SWIM_STYLE_TIME_GAP                               3000//5000
  // For butterfly stroke
#define QUEUE_SENSOR_MS_NUM                                     50//10
#define BUTTERFLYSTROKE_ACC_MS_TH                               5.0f//6.0f
#define BUTTERFLYSTROKE_GYRO_MS_TH                              700.0f
#define BUTTERFLYSTROKE_MS_PERIOD                               10000           // Unit: ms
#define BUTTERFLYSTROKE_MS_NUM_TH                               3
// #define MIN_ST_TIME_GAP                                         3000            // Unit: ms
  // For breast stroke
#define BREASTSTROKE_ACC_Z_PEAK_TH                              0.9f
#define BREASTSTROKE_ACC_Z_BOTTOM_HIGH_TH                       0.6f
#define BREASTSTROKE_ACC_Z_BOTTOM_LOW_TH                        -0.5f
#define BREASTSTROKE_ACC_Z_PEAK_BOTTOM_DIFF_LOW_TH              1.56f
#define BREASTSTROKE_ACC_Z_PEAK_BOTTOM_DIFF_HIGH_TH             1.9f
#define BREASTSTROKE_GYRO_X_PEAK_TH                             50.0f
#define BREASTSTROKE_GYRO_X_BOTTOM_TH                           -50.0f
#define BREASTFRONTBACK_Z_NUM                                   6
  // For free and back stroke
#define QUEUE_SENSOR_GYRO_Z_NUM                                 6//10
#define FRONTBACK_GYRO_Z_PEAK_TH                                50.0f
#define FRONTBACK_GYRO_Z_BOTTOM_TH                              -50.0f
  // For style decision for each lap
#define SWIM_STYLE_BUFFER_NUM                                   3
#define SWIM_STYLE_BUFFER_FOR_LAP_NUM                           30//20
#define SWIM_STYLE_NUM                                          5

// For lap detection
#define QUEUE_SENSOR_DATA_NUM_MAG                               30//20              // For 10HZ ODR
#define QUEUE_SWIM_STROKES_PER_LAP                              2
#define LAP_DETECTION_CHOICE                                    1               // 0: By sign, 1: By diff
#define MAG_LAP_DIFF1                                           40.0f//25.0f
#define MAG_LAP_STD1                                            15.0f//25.0f
#define MAG_LAP_DIFF2                                           30.0f//25.0f
#define MAG_LAP_STD2                                            10.0f//25.0f
#define MAG_LAP_DIFF3                                           20.0f//25.0f
#define MAG_LAP_STD3                                            5.0f//25.0f

#define LAP_NO_STROKE_DURATION                                  10000           // Unit: ms
#define MIDDLE_LAP_NO_STROKE_DURATION                           6000            // Unit: ms
#define SWIMMING_TIMES_RATIO                                    0.6f
#define MAX_TIMESTAMPE_DIFF_SWIM_STROKES                        500

void init_swim_stroke_detection(void);
void init_swim_style_detection(band_location loc);
void init_swim_lap_detection(void);
void deinit_swim_stroke_detection(void);
void deinit_swim_style_detection(void);
void deinit_swim_lap_detection(void);
void on_sensor_changed_swim_stroke_detection(const sensor_event event, result_event *result);
void on_sensor_changed_swim_style_detection(const sensor_event event, result_event *result);
void on_sensor_changed_swim_lap_detection(const sensor_event event, result_event *result);
void final_lap_detection(result_event *result);
void cc_mean_std_array(float sensor[][SENSOR_LEN], float *mean, float *std, uint16_t len);
void cc_mean_std(float sensor[], float *mean, float *std, int num);
void CC_set_swimpool_len(swimming_pool_len len);
#if defined(MAKE_GCC)
  void CC_Swim_Result_Summary();
#endif // defined(MAKE_GCC)

#endif // CC_SWIM_DETECTION_H
