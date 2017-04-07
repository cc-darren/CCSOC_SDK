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

#ifndef _SPIM_H__
#define _SPIM_H__

#include "global.h"
#include "cc6801_reg.h"
#include <stddef.h>
#include <stdbool.h>

#define SPI0_INSTANCE_INDEX 0

#define CC_DRV_SPI_PERIPHERAL(id)  (void *)regSPI##id

/**
 * @brief SPI master driver instance data structure.
 */
typedef struct
{
    void *    p_registers;  ///< Pointer to the structure with SPI/SPIM peripheral instance registers.
    uint8_t   drv_inst_idx; ///< Driver instance index.
} cc_drv_spi_t;

/**
 * @brief Macro for creating an SPI master driver instance.
 */
#define CC_DRV_SPI_INSTANCE(id)                        \
{                                                       \
    .p_registers  = CC_DRV_SPI_PERIPHERAL(id),         \
    .drv_inst_idx = SPI##id##_INSTANCE_INDEX, \
}

/**
 * @brief SPI modes.
 */
typedef enum
{
    CC_SPIM_MODE_0, ///< SCK active high, sample on leading edge of clock.
    CC_SPIM_MODE_1, ///< SCK active high, sample on trailing edge of clock.
    CC_SPIM_MODE_2, ///< SCK active low, sample on leading edge of clock.
    CC_SPIM_MODE_3  ///< SCK active low, sample on trailing edge of clock.
} cc_spim_mode_t;

/**
 * @brief SPI bit orders.
 */
typedef enum
{
    CC_SPIM_BIT_ORDER_MSB_FIRST, ///< Most significant bit shifted out first.
    CC_SPIM_BIT_ORDER_LSB_FIRST  ///< Least significant bit shifted out first.
} cc_spim_bit_order_t;

/**
 * @brief SPI master driver instance configuration structure.
 */
typedef struct
{
    cc_spim_mode_t      mode;      ///< SPI mode.
    cc_spim_bit_order_t bit_order; ///< SPI bit order.
} cc_drv_spi_config_t;

/**
 * @brief SPI master instance default configuration.
 */
#define CC_DRV_SPI_DEFAULT_CONFIG(id)                       \
{                                                            \
    .mode         = CC_SPIM_MODE_0,                      \
    .bit_order    = CC_SPIM_BIT_ORDER_LSB_FIRST,         \
}

/**
 * @brief Single transfer descriptor structure.
 */
typedef struct
{
    uint8_t const * p_tx_buffer; ///< Pointer to TX buffer.
    uint8_t         tx_length;   ///< TX buffer length.
    uint8_t       * p_rx_buffer; ///< Pointer to RX buffer.
    uint8_t         rx_length;   ///< RX buffer length.
}cc_drv_spi_xfer_desc_t;

/**
 * @brief SPI master driver event types, passed to the handler routine provided
 *        during initialization.
 */
typedef enum
{
    CC_DRV_SPI_EVENT_DONE, ///< Transfer done.
} cc_drv_spi_evt_type_t;

typedef struct
{
    cc_drv_spi_evt_type_t  type;      ///< Event type.
    cc_drv_spi_xfer_desc_t done;  ///< Event data for DONE event.
} cc_drv_spi_evt_t;

/**
 * @brief SPI master driver event handler type.
 */
typedef void (*cc_drv_spi_handler_t)(cc_drv_spi_evt_t const * p_event);


/**
 * @brief Function for initializing the SPI master driver instance.
 *
 * This function configures and enables the specified peripheral.
 *
 * @param[in] p_instance Pointer to the instance structure.
 * @param[in] p_config   Pointer to the structure with the initial configuration.
 *                       If NULL, the default configuration is used.
 * @param     handler    Event handler provided by the user. If NULL, transfers
 *                       will be performed in blocking mode.
 *
 * @retval CC_SUCCESS             If initialization was successful.
 * @retval CC_ERROR_INVALID_STATE If the driver was already initialized.
 * @retval CC_ERROR_BUSY          If some other peripheral with the same
 *                                 instance ID is already in use. This is
 *                                 possible only if PERIPHERAL_RESOURCE_SHARING_ENABLED
 *                                 is set to a value other than zero.
 */
int cc_drv_spi_init(cc_drv_spi_t const * const p_instance,
                            cc_drv_spi_config_t const * p_config,
                            cc_drv_spi_handler_t handler);

/**
 * @brief Function for uninitializing the SPI master driver instance.
 *
 * @param[in] p_instance Pointer to the instance structure.
 */
void cc_drv_spi_uninit(cc_drv_spi_t const * const p_instance);

/**
 * @brief Function for starting the SPI data transfer.
 *
 * If an event handler was provided in the @ref cc_drv_spi_init call, this function
 * returns immediately and the handler is called when the transfer is done.
 * Otherwise, the transfer is performed in blocking mode, which means that this function
 * returns when the transfer is finished.
 *
 * @note Peripherals using EasyDMA (for example, SPIM) require the transfer buffers
 *       to be placed in the Data RAM region. If they are not and an SPIM instance is
 *       used, this function will fail with the error code CC_ERROR_INVALID_ADDR.
 *
 * @param[in] p_instance       Pointer to the instance structure.
 * @param[in] p_tx_buffer      Pointer to the transmit buffer. Can be NULL
 *                             if there is nothing to send.
 * @param     tx_buffer_length Length of the transmit buffer.
 * @param[in] p_rx_buffer      Pointer to the receive buffer. Can be NULL
 *                             if there is nothing to receive.
 * @param     rx_buffer_length Length of the receive buffer.
 *
 * @retval CC_SUCCESS            If the operation was successful.
 * @retval CC_ERROR_BUSY         If a previously started transfer has not finished
 *                                yet.
 * @retval CC_ERROR_INVALID_ADDR If the provided buffers are not placed in the Data
 *                                RAM region.
 */
int cc_drv_spi_transfer(cc_drv_spi_t const * const p_instance,
                                uint8_t const * p_tx_buffer,
                                uint8_t         tx_buffer_length,
                                uint8_t       * p_rx_buffer,
                                uint8_t         rx_buffer_length);


/**
 * @brief Function for starting the SPI data transfer with additional option flags.
 *
 * Function enables customizing the transfer by using option flags.
 *
 * Additional options are provided using the flags parameter:
 *
 * - @ref CC_DRV_SPI_FLAG_TX_POSTINC and @ref CC_DRV_SPI_FLAG_RX_POSTINC<span></span>:
 *   Post-incrementation of buffer addresses. Supported only by SPIM.
 * - @ref CC_DRV_SPI_FLAG_HOLD_XFER<span></span>: Driver is not starting the transfer. Use this
 *   flag if the transfer is triggered externally by PPI. Supported only by SPIM. Use
 *   @ref cc_drv_twi_start_task_get to get the address of the start task.
 * - @ref CC_DRV_SPI_FLAG_NO_XFER_EVT_HANDLER<span></span>: No user event handler after transfer
 *   completion. This also means no interrupt at the end of the transfer. Supported only by SPIM.
 *   If @ref CC_DRV_SPI_FLAG_NO_XFER_EVT_HANDLER is used, the driver does not set the instance into
 *   busy state, so you must ensure that the next transfers are set up when SPIM is not active.
 *   @ref cc_drv_spi_end_event_get function can be used to detect end of transfer. Option can be used
 *   together with @ref CC_DRV_SPI_FLAG_REPEATED_XFER to prepare a sequence of SPI transfers
 *   without interruptions.
 * - @ref CC_DRV_SPI_FLAG_REPEATED_XFER<span></span>: Prepare for repeated transfers. You can set
 *   up a number of transfers that will be triggered externally (for example by PPI). An example is
 *   a TXRX transfer with the options @ref CC_DRV_SPI_FLAG_RX_POSTINC,
 *   @ref CC_DRV_SPI_FLAG_NO_XFER_EVT_HANDLER, and @ref CC_DRV_SPI_FLAG_REPEATED_XFER. After the
 *   transfer is set up, a set of transfers can be triggered by PPI that will read, for example,
 *   the same register of an external component and put it into a RAM buffer without any interrupts.
 *   @ref cc_drv_spi_end_event_get can be used to get the address of the END event, which can be
 *   used to count the number of transfers. If @ref CC_DRV_SPI_FLAG_REPEATED_XFER is used,
 *   the driver does not set the instance into busy state, so you must ensure that the next
 *   transfers are set up when SPIM is not active. Supported only by SPIM.
 * @note Function is intended to be used only in non-blocking mode.
 *
 * @param p_instance  SPI instance.
 * @param p_xfer_desc Pointer to the transfer descriptor.
 * @param flags       Transfer options (0 for default settings).
 *
 * @retval CC_SUCCESS             If the procedure was successful.
 * @retval CC_ERROR_BUSY          If the driver is not ready for a new transfer.
 * @retval CC_ERROR_NOT_SUPPORTED If the provided parameters are not supported.
 * @retval CC_ERROR_INVALID_ADDR  If the provided buffers are not placed in the Data
 *                                 RAM region.
 */
int cc_drv_spi_xfer(cc_drv_spi_t     const * const p_instance,
                            cc_drv_spi_xfer_desc_t const * p_xfer_desc,
                            uint32_t                        flags);

#endif // _SPIM_H__
