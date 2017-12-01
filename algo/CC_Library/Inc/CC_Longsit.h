/**
 * @file longsit.h
 * @author Chenglong Fu 
 * @date 2015/12/07
 * @brief Long sitting algorithm
 * 
 * Details
 */
//#include "CC_global_config.h"

//#ifdef LONGSIT_EN

#ifndef _LONGSIT_H_
#define _LONGSIT_H_

#define LIB_OPEN_PASS           1
#define LIB_OPEN_FAIL         0

/** 
 * -----------------------------------------------------------------------------
 * Included headers
 * -----------------------------------------------------------------------------
 */
#include <stdint.h>

/** 
 * -----------------------------------------------------------------------------
 * Type declarations
 * -----------------------------------------------------------------------------
 */

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
 * @brief Open long sitting algorithm
 */
uint8_t LongSit_open(void);

/**
 * @brief Close long sitting algorithm
 */
void LongSit_close(void);

/**
 * @brief Process long sitting algorithm
 * 
 * @param steps Step count
 * @param time Current system time in seconds
 * 
 * @return uint8_t Returns 1 if it is notified. Otherwise, returns 0.
 */
 #if 0
uint8_t LongSit_Process(uint32_t steps, uint32_t seconds);
#else
uint8_t LongSit_Process(uint32_t steps);
#endif

/**
 * @brief Set time threshold
 * 
 * @param threshold Threshold in minutes.
 */
void LongSit_set_time_threshold(uint16_t threshold);

/**
 * @brief Reset notification
 */
void LongSit_reset_notification(void);


/**
 * @brief Get the longsit lib open stete
 */
uint8_t LongSit_is_open(void);
uint32_t mCubeLongSit_get_version(void);
extern void CC_LongSit_onNotified(uint8_t notified);


//#endif

#endif // _LONGSIT_H_

