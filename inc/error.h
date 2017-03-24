/*
 * Copyright (c) Nordic Semiconductor ASA
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *   1. Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 *   2. Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 *   3. Neither the name of Nordic Semiconductor ASA nor the names of other
 *   contributors to this software may be used to endorse or promote products
 *   derived from this software without specific prior written permission.
 *
 *   4. This software must only be used in a processor manufactured by Nordic
 *   Semiconductor ASA, or in a processor manufactured by a third party that
 *   is used in combination with a processor manufactured by Nordic Semiconductor.
 *
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
 /**
  @defgroup nrf_error SoftDevice Global Error Codes
  @{

  @brief Global Error definitions
*/

/* Header guard */
#ifndef _ERROR_H__
#define _ERROR_H__

/** @defgroup NRF_ERRORS_BASE Error Codes Base number definitions
 * @{ */
#define CC_ERROR_BASE_NUM      (0x0)       ///< Global error base
#define CC_ERROR_SDM_BASE_NUM  (0x1000)    ///< SDM error base
#define CC_ERROR_SOC_BASE_NUM  (0x2000)    ///< SoC error base
#define CC_ERROR_STK_BASE_NUM  (0x3000)    ///< STK error base
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
