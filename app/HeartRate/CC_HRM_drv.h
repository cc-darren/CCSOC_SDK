#ifndef _CC_HRM_DRV_
#define _CC_HRM_DRV_
#include "project.h"

#include <stdint.h>
#include <string.h>
#include <stdlib.h>


#define TOTAL_CHANNELS  3 //Using channel numbers 
#define HEART_RATE_MODE_SAMPLES_PER_CH_READ (20)  //Numbers of PPG data per channel 
#define HEART_RATE_MODE_SAMPLES_PER_READ (TOTAL_CHANNELS* HEART_RATE_MODE_SAMPLES_PER_CH_READ)   


uint8_t CC_HRM_PPG_INIT(void);
uint8_t CC_HRM_PPG_PROC(void);
uint8_t CC_HRM_PPG_DEINIT(void);
void CC_HRM_PPG_INTR_ISR(void);
void CC_HRM_Get_Gsensor(int16_t *data);
bool pah8002_sensor_init(void);
void pah8002_sensor_deinit(void);


#endif

