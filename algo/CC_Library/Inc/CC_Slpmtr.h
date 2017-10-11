#ifndef CC_SLPMTR_H
#define CC_SLPMTR_H

/** 
 * -----------------------------------------------------------------------------
 * Included headers
 * -----------------------------------------------------------------------------
 */
#include <stdint.h>

// Stage definition
#define CC_SLEEPMETER_AWAKE          0x00000000
#define CC_SLEEPMETER_DEEPSLEEP      0x00000001
#define CC_SLEEPMETER_LIGHTSLEEP     0x00000002
#define CC_SLEEPMETER_ROLLOVER       0x00000004
#define CC_SLEEPMETER_IDLE           0x00000008

#define CC_SLEEPMETER_MASK              (CC_SLEEPMETER_DEEPSLEEP | \
                                         CC_SLEEPMETER_LIGHTSLEEP | \
                                         CC_SLEEPMETER_ROLLOVER | \
                                         CC_SLEEPMETER_IDLE)

/** 
 * -----------------------------------------------------------------------------
 * Type declarations
 * -----------------------------------------------------------------------------
 */
typedef enum slpmtr_sens {
    SLPMTR_SENS_LOW = 0,                        /**< Low sensitivity. */
    SLPMTR_SENS_MEDIUM,                         /**< Medium sensitivity. */
    SLPMTR_SENS_HIGH,                           /**< High sensitivity. */

    SLPMTR_SENS_END,
} slpmtr_sens_t;

typedef struct slpmtr_param {
    slpmtr_sens_t sensitivity;                  /**< sensitivity of the library. */
    float idle_timeout;                         /**< Timeout for the idle status detection. (seconds) */
} slpmtr_param_t;

typedef union slpmtr_input {
    float mpss[3];                              /**< Input in m/s^s. */
} slpmtr_input_t;

typedef struct slpmtr_output {
    uint32_t OldStage;                          /* Old stage */
    uint32_t NewStage;                          /* New stage */  
    float time;                                
    float period;                               /* elapsed time */
    float covariant;
} slpmtr_output_t;


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
void slpmtr_open(void);

/**
 * @brief Opens the sleepmeter library with the sensitivity
 *
 * @param sensitivity The sensitivity of the sleepmeter libaray. It could be LOW
 *                    or MEDIUM or HIGH. Please refere the enumeration
 *                    slpmtr_sens in this header file. Higher sensitivity means
 *                    it is more sensitive when the libaray detecting the
 *                    activity.
 */
void slpmtr_open_with_sensitivity(const slpmtr_sens_t sensitivity);

/**
 * @brief Opens the sleepmeter libaray with the parameters.
 *
 * @param p_param Point to the structure of the sleepmeter parameter.
 */
void slpmtr_open_with_param(const slpmtr_param_t *p_param);

/**
 * @brief Closes the sleepmeter library.
 */
void slpmtr_close(void);

/**
 * @brief Determines the sleepmeter is opened or not.
 *
 * @return int8_t If it is opened, returns 1. Otherwise returns 0.
 */
int8_t slpmtr_is_opened(void);

/**
 * @brief Detect the sleep status
 *
 * @param input Point to the input structure of the sleepmeter.
 * @param seconds The timestamp of the input in seconds.
 * @param input Point to the output structure of the sleepmeter.
 */
void slpmtr_detect(const slpmtr_input_t *input, float seconds, slpmtr_output_t *output);

/**
 * @brief Get sleep status.
 * 
 * @return uint8_t Returns the sleep status. 0 for awake, 1 for sleeping, 2 for 
 *         restless, 3 for rollover and 4 for idle.
 */
uint8_t slpmtr_get_sleep_status(void);

/**
 * @brief Get the version.
 *
 * @return int32_t Return a 32 bits value it includes
 *         MAIN(8bits).MINOR(8bits).BUILD(8bits).YYYYMMDD(8bits).
 */
uint32_t slpmtr_get_version(void);

#ifdef __cplusplus
}
#endif


#endif /**< CC_SLPMTR_H */

