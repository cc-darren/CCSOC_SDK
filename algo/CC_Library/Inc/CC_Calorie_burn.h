#ifndef MCUBE_CALORIE_BURN_H
#define MCUBE_CALORIE_BURN_H
//#include "CC_global_config.h"

//#ifdef CFG_MODULE_CALORIE

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define CC_CALORIE_STATE_IDLE 0
#define CC_CALORIE_STATE_WALKING 1
#define CC_CALORIE_STATE_RUNNING 2

typedef enum {
    CC_LIB_GENDER_MALE =0,
    CC_LIB_GENDER_FEMALE = 1,
} CC_Gender_t;

/* Declaration of Function Input */
typedef struct {
    uint8_t age;                //(yr)
    CC_Gender_t gender;       //(1:Male;0:Female)
    float weight;               //(kg)
    float height;               //(m)
    uint32_t stepCount;         //(Counts)
    float duration;             //(Sec)
	uint8_t StepState;          // CC_CALORIE_STATE_XXX
} CC_ActivityInfo_t;

void CC_CalorieBurn_Open(void);
void CC_CalorieBurn_Close(void);
void CC_CalorieBurn_Set_Settings(CC_ActivityInfo_t *Setting);
uint32_t CC_CalorieBurn_Get(void);
#if 0
float CC_CalorieBurn(CC_ActivityInfo_t *info);
#else
float CC_CalorieBurnV2(CC_ActivityInfo_t *info);
#endif
uint32_t CC_CalorieBurnCalStep(uint32_t steps, uint32_t duration);
// For stepState, please use CC_CALORIE_STATE_XXX definition above
uint32_t CC_CalorieBurnCalStepV2(uint32_t steps, uint32_t duration, uint32_t stepState);

#ifdef __cplusplus
}
#endif

//#endif /**< CFG_MODULE_CALORIE */

#endif // MCUBE_CALORIE_BURN_H
