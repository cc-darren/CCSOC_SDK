/* Copyright (c) 2019 Cloudchip, Inc. All Rights Reserved.
 *
 * The information contained herein is property of Cloudchip, Inc.
 * Terms and conditions of usage are described in detail in CLOUDCHIP
 * STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information.
 * NO WARRANTY of ANY KIND is provided. This heading must NOT be removed 
 * from the file.
 */

/* Header guard */
#ifndef _ERROR_H__
#define _ERROR_H__

/** Definition of ERRORs
 * @{ */
#define CC_ERROR_BASE_NUM      (0x0)       ///< Global error base
/** @} */

#define CC_SUCCESS                           (CC_ERROR_BASE_NUM + 0)  ///< Successful command
#define CC_ERROR_SVC_HANDLER_MISSING         (CC_ERROR_BASE_NUM + 1)  ///< SVC handler is missing
#define CC_ERROR_SOFTDEVICE_NOT_ENABLED      (CC_ERROR_BASE_NUM + 2)  ///< SoftDevice has not been enabled
#define CC_ERROR_INTERNAL                    (CC_ERROR_BASE_NUM + 3)  ///< Internal Error
#define CC_ERROR_NO_MEM                      (CC_ERROR_BASE_NUM + 4)  ///< No Memory for operation
#define CC_ERROR_NOT_FOUND                   (CC_ERROR_BASE_NUM + 5)  ///< Not found
#define CC_ERROR_NOT_SUPPORTED               (CC_ERROR_BASE_NUM + 6)  ///< Not supported
#define CC_ERROR_INVALID_PARAM               (CC_ERROR_BASE_NUM + 7)  ///< Invalid Parameter
#define CC_ERROR_INVALID_STATE               (CC_ERROR_BASE_NUM + 8)  ///< Invalid state, operation disallowed in this state
#define CC_ERROR_INVALID_LENGTH              (CC_ERROR_BASE_NUM + 9)  ///< Invalid Length
#define CC_ERROR_INVALID_FLAGS               (CC_ERROR_BASE_NUM + 10) ///< Invalid Flags
#define CC_ERROR_INVALID_DATA                (CC_ERROR_BASE_NUM + 11) ///< Invalid Data
#define CC_ERROR_DATA_SIZE                   (CC_ERROR_BASE_NUM + 12) ///< Data size exceeds limit
#define CC_ERROR_TIMEOUT                     (CC_ERROR_BASE_NUM + 13) ///< Operation timed out
#define CC_ERROR_NULL                        (CC_ERROR_BASE_NUM + 14) ///< Null Pointer
#define CC_ERROR_FORBIDDEN                   (CC_ERROR_BASE_NUM + 15) ///< Forbidden Operation
#define CC_ERROR_INVALID_ADDR                (CC_ERROR_BASE_NUM + 16) ///< Bad Memory Address
#define CC_ERROR_BUSY                        (CC_ERROR_BASE_NUM + 17) ///< Busy

#endif // ERROR_H__

/**
  @}
*/
