#ifndef _CC_HRM_DRV_
#define _CC_HRM_DRV_
#include "project.h"

#include <stdint.h>
#include <string.h>
#include <stdlib.h>


uint8_t CC_HRM_PPG_INIT(void);
uint8_t CC_HRM_PPG_PROC(void);
uint8_t CC_HRM_PPG_DEINIT(void);
void CC_HRM_PPG_INTR_ISR(void);
void CC_HRM_Get_Gsensor(int16_t *data);
#endif

