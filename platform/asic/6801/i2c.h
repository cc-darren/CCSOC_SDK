/******************************************************************************
*  Copyright 2017, CloudChip, Inc.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of CloudChip, Inc. (C) 2017
******************************************************************************/

#ifndef _I2C_H_
#define _I2C_H_

#include "global.h"
#include "cc6801_reg.h"
#include <stddef.h>
#include <stdbool.h>

/**
 * @enum nrf_i2c_frequency_t
 * @brief I2C master clock frequency.
 */
typedef enum
{
    CC_I2C_FREQ_100K,
    CC_I2C_FREQ_200K,
    CC_I2C_FREQ_400K,
} cc_i2c_frequency_t;

/**@brief I2C events. */
typedef enum
{
    CC_DRV_I2C_RX_DONE,
    CC_DRV_I2C_TX_DONE,
    CC_DRV_I2C_ERROR
} cc_drv_i2c_evt_type_t;

/**@brief Structure for a I2C event. */
typedef struct
{
    cc_drv_i2c_evt_type_t type;  /**< Event type. */
    uint8_t * p_data;             /**< Pointer to transferred data. */
    uint32_t  length;             /**< Number of bytes transferred. */
} cc_drv_i2c_evt_t;

/**@brief I2C driver instance structure. */
typedef struct
{
    U_regI2C           * p_reg;       /**< Pointer to the instance register set. */
    uint8_t            instance_id;   /**< Instance index. */
} cc_drv_i2c_t;

/**@brief Macro for creating a I2C driver instance.*/
#define CC_DRV_I2C_INSTANCE(id)                      \
{                                                     \
    .p_reg       = regI2C##id,             \
    .instance_id = I2C##id##_INSTANCE_INDEX \
}

/**@brief Structure for I2C instance configuration. */
typedef struct
{
    uint8_t            address;            /**< Address. */
    cc_i2c_frequency_t frequency;          /**< Frequency. */
} cc_drv_i2c_config_t;

/**@brief I2C event handler prototype. */
typedef void (* cc_drv_i2c_evt_handler_t)(cc_drv_i2c_evt_t const * p_event,
                                           void *                    p_context);

/**
 * @brief Function for initializing the I2C instance.
 *
 * @param[in] p_instance      I2C instance.
 * @param[in] p_config        Initial configuration. If NULL, the default configuration is used.
 * @param[in] event_handler   Event handler provided by the user. If NULL, blocking mode is enabled.
 * @param[in] p_context       Context pointer, passed to event handler.
 *
 * @retval  CC_SUCCESS If initialization was successful.
 * @retval  CC_ERROR_INVALID_STATE If the driver is in invalid state.
 */
int cc_drv_i2c_init(cc_drv_i2c_t const * const  p_instance,
                            cc_drv_i2c_config_t const * p_config,
                            cc_drv_i2c_evt_handler_t    event_handler,
                            void *                       p_context);

/**
 * @brief Function for uninitializing the I2C.
 *
 * @param[in] p_instance  I2C instance.
 */
void cc_drv_i2c_uninit(cc_drv_i2c_t const * const p_instance);

/**
 * @brief Function for enabling the I2C instance.
 *
 * @param[in] p_instance  I2C instance.
 */
void cc_drv_i2c_enable(cc_drv_i2c_t const * const p_instance);

/**
 * @brief Function for disabling the I2C instance.
 *
 * @param[in] p_instance  I2C instance.
 */
void cc_drv_i2c_disable(cc_drv_i2c_t const * const p_instance);

/**
 * @brief Function for sending data to a I2C slave.
 *
 * The transmission will be stopped when an error or time-out occurs.
 *
 * @param[in] p_instance      I2C instance.
 * @param[in] p_data          Pointer to a transmit buffer.
 * @param[in] length          Number of bytes to send.
 * @param[in] xfer_pending    After a specified number of bytes, transmission will
 *                            be suspended (if xfer_pending is set) or stopped (if not).
 *
 * @retval  CC_SUCCESS        If the procedure was successful.
 * @retval  CC_ERROR_BUSY     If the driver is not ready for a new transfer.
 * @retval  CC_ERROR_INTERNAL If an @ref CC_I2C_EVENTS_ERROR or a time-out has occurred (only in blocking mode).
 */
int cc_drv_i2c_tx(cc_drv_i2c_t const * const p_instance,
                          uint8_t const *             p_data,
                          uint32_t                    length,
                          bool                        xfer_pending);

/**
 * @brief Function for reading data from a I2C slave.
 *
 * Transmission will be stopped when error or time-out occurs.
 *
 * @param[in] p_instance      I2C instance.
 * @param[in] p_data          Pointer to a receive buffer.
 * @param[in] length          Number of bytes to be received.
 * @param[in] xfer_pending    After a specified number of bytes, transmission will
 *                            be suspended (if xfer_pending is set) or stopped (if not).
 *
 * @retval  CC_SUCCESS        If the procedure was successful.
 * @retval  CC_ERROR_BUSY     If the driver is not ready for a new transfer.
 * @retval  CC_ERROR_INTERNAL If an @ref CC_I2C_EVENTS_ERROR or a time-out has occured (only in blocking mode).
 */
int cc_drv_i2c_rx(cc_drv_i2c_t const * const p_instance,
                          uint8_t *                   p_data,
                          uint32_t                    length,
                          bool                        xfer_pending);

/**
 *@}
 **/

#endif // _I2C_H_
