#ifndef CC_UTILITY_H
#define CC_UTILITY_H

/** 
 * -----------------------------------------------------------------------------
 * Included headers
 * -----------------------------------------------------------------------------
 */
#include <stdint.h>

/** 
 * -----------------------------------------------------------------------------
 * Definition
 * -----------------------------------------------------------------------------
 */

#define UTILITY_ENERGY_UNKNOWN        0
#define UTILITY_ENERGY_IDLE           1
#define UTILITY_ENERGY_ULTRA_LOW      2
#define UTILITY_ENERGY_LOW            3
#define UTILITY_ENERGY_MEDIUM         4
#define UTILITY_ENERGY_HIGH           5

/** 
 * -----------------------------------------------------------------------------
 * Type declarations
 * -----------------------------------------------------------------------------
 */

typedef union utility_accel_input {
    float mpss[3];                              /**< Input in m/s^s. */
} utility_accel_input_t;

typedef struct utility_output {
    uint32_t energystate;                       /* UTILITY_ENERGY_XXX */  
    float time;                                
    float period;                               /* elapsed time */
    float covariant;
} utility_output_t;

#ifdef __cplusplus
extern "C" {
#endif

/** 
 * -----------------------------------------------------------------------------
 * External linkage variables
 * -----------------------------------------------------------------------------
 */
 
/** 
 * -----------------------------------------------------------------------------
 * API declarations
 * -----------------------------------------------------------------------------
 */
/**
 * @brief Opens the sleepmeter libaray with the default setting.
 */
 
void utility_open(void);
void utility_close(void);
int8_t utility_is_opened(void);
void utility_energy_detect(const utility_accel_input_t *input, float seconds, utility_output_t *output);
uint8_t utility_get_energy_status(void);

#ifdef __cplusplus
}
#endif


#endif /**< CC_UTILITY_H */

