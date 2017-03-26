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

#ifndef _UART_H
#define _UART_H

#define UART0_CONFIG_HWFC         CC_UART_HWFC_DISABLED
#define UART0_CONFIG_PARITY       CC_UART_PARITY_EXCLUDED
#define UART0_CONFIG_BAUDRATE     CC_UART_BAUDRATE_115200
#define UART0_CONFIG_PSEL_TXD 7
#define UART0_CONFIG_PSEL_RXD 8
#define UART0_CONFIG_PSEL_CTS 0		//no use in Cadence
#define UART0_CONFIG_PSEL_RTS 0		//no use in Cadence

/**
 * @enum cc_uart_parity_t
 * @brief Types of UART parity modes.
 */
typedef enum
{
    CC_UART_PARITY_EXCLUDED, /**< Parity excluded. */
    CC_UART_PARITY_INCLUDED, /**< Parity included. */
} cc_uart_parity_t;

/**
 * @enum cc_uart_hwfc_t
 * @brief Types of UART flow control modes.
 */
typedef enum
{
    CC_UART_HWFC_DISABLED, /**< HW flow control disabled. */
    CC_UART_HWFC_ENABLED,  /**< HW flow control enabled. */
} cc_uart_hwfc_t;

/**
 * @enum cc_uart_baudrate_t
 * @brief Baudrates supported by UART.
 */
typedef enum
{
    CC_UART_BAUDRATE_1200, /**< 1200 baud. */
    CC_UART_BAUDRATE_2400, /**< 2400 baud. */
    CC_UART_BAUDRATE_4800, /**< 4800 baud. */
    CC_UART_BAUDRATE_9600, /**< 9600 baud. */
    CC_UART_BAUDRATE_14400, /**< 14400 baud. */
    CC_UART_BAUDRATE_19200, /**< 19200 baud. */
    CC_UART_BAUDRATE_28800, /**< 28800 baud. */
    CC_UART_BAUDRATE_38400, /**< 38400 baud. */
    CC_UART_BAUDRATE_56000, /**< 56000 baud. */
    CC_UART_BAUDRATE_115200, /**< 115200 baud. */
    CC_UART_BAUDRATE_230400, /**< 230400 baud. */
    CC_UART_BAUDRATE_345600, /**< 345600 baud. */
    CC_UART_BAUDRATE_460800, /**< 460800 baud. */
    CC_UART_BAUDRATE_921600, /**< 921600 baud. */
    CC_UART_BAUDRATE_1105920, /**< 1105920 baud. */
} cc_uart_baudrate_t;

/**
 * @brief Types of UART driver events.
 */
typedef enum
{
    CC_DRV_UART_EVT_TX_DONE, ///< Requested TX transfer completed.
    CC_DRV_UART_EVT_RX_DONE, ///< Requested RX transfer completed.
    CC_DRV_UART_EVT_ERROR,   ///< Error reported by UART peripheral.
} cc_drv_uart_evt_type_t;

/**@brief Structure for UART configuration. */
typedef struct
{
    uint32_t            pseltxd;            ///< TXD pin number.
    uint32_t            pselrxd;            ///< RXD pin number.
    uint32_t            pselcts;            ///< CTS pin number.
    uint32_t            pselrts;            ///< RTS pin number.
    void *              p_context;          ///< Context passed to interrupt handler.
    cc_uart_hwfc_t     hwfc;               ///< Flow control configuration.
    cc_uart_parity_t   parity;             ///< Parity configuration.
    cc_uart_baudrate_t baudrate;           ///< Baudrate.
} cc_drv_uart_config_t;

/**@brief UART default configuration. */
#define CC_DRV_UART_DEFAULT_CONFIG                                                   \
    {                                                                                 \
        .pseltxd            = UART0_CONFIG_PSEL_TXD,                                  \
        .pselrxd            = UART0_CONFIG_PSEL_RXD,                                  \
        .pselcts            = UART0_CONFIG_PSEL_CTS,                                  \
        .pselrts            = UART0_CONFIG_PSEL_RTS,                                  \
        .p_context          = NULL,                                                   \
        .hwfc               = UART0_CONFIG_HWFC,                                      \
        .parity             = UART0_CONFIG_PARITY,                                    \
        .baudrate           = UART0_CONFIG_BAUDRATE,                                  \
    }

/**@brief Structure for UART transfer completion event. */
typedef struct
{
    uint8_t * p_data; ///< Pointer to memory used for transfer.
    uint8_t   bytes;  ///< Number of bytes transfered.
} cc_drv_uart_xfer_evt_t;

/**@brief Structure for UART error event. */
typedef struct
{
    cc_drv_uart_xfer_evt_t rxtx;      ///< Transfer details includes number of bytes transfered.
    uint32_t                error_mask;///< Mask of error flags that generated the event.
} cc_drv_uart_error_evt_t;

/**@brief Structure for UART event. */
typedef struct
{
    cc_drv_uart_evt_type_t type;      ///< Event type.
    union
    {
        cc_drv_uart_xfer_evt_t  rxtx; ///< Data provided for transfer completion events.
        cc_drv_uart_error_evt_t error;///< Data provided for error event.
    } data;
} cc_drv_uart_event_t;

/**
 * @brief UART interrupt event handler.
 *
 * @param[in] p_event     Pointer to event structure. Event is allocated on the stack so it is available
 *                        only within the context of the event handler.
 * @param[in] p_context   Context passed to interrupt handler, set on initialization.
 */
typedef void (*cc_uart_event_handler_t)(cc_drv_uart_event_t * p_event, void * p_context);

/**
 * @brief Function for initializing the UART driver.
 *
 * This function configures and enables UART. After this function GPIO pins are controlled by UART.
 *
 * @param[in] p_config       Initial configuration. Default configuration used if NULL.
 * @param[in] event_handler  Event handler provided by the user. If not provided driver works in
 *                           blocking mode.
 *
 * @retval    CC_SUCCESS             If initialization was successful.
 * @retval    CC_ERROR_INVALID_STATE If driver is already initialized.
 */
int cc_drv_uart_init(cc_drv_uart_config_t const * p_config,
                             cc_uart_event_handler_t      event_handler);

/**
 * @brief Function for uninitializing  the UART driver.
 */
void cc_drv_uart_uninit(void);

/**
 * @brief Function for sending data over UART.
 *
 * If an event handler was provided in cc_drv_uart_init() call, this function
 * returns immediately and the handler is called when the transfer is done.
 * Otherwise, the transfer is performed in blocking mode, i.e. this function
 * returns when the transfer is finished. Blocking mode is not using interrupt so
 * there is no context switching inside the function.
 *
 * @note Peripherals using EasyDMA (i.e. UARTE) require that the transfer buffers
 *       are placed in the Data RAM region. If they are not and UARTE instance is
 *       used, this function will fail with error code CC_ERROR_INVALID_ADDR.
 *
 * @param[in] p_data Pointer to data.
 * @param[in] length Number of bytes to send.
 *
 * @retval    CC_SUCCESS            If initialization was successful.
 * @retval    CC_ERROR_BUSY         If driver is already transferring.
 * @retval    CC_ERROR_INVALID_ADDR If p_data does not point to RAM buffer (UARTE only).
 * @retval    CC_ERROR_FORBIDDEN    If transfer was aborted (blocking mode only).
 */
int cc_drv_uart_tx(uint8_t const * const p_data, uint8_t length);

/**
 * @brief Function for aborting any ongoing transmission.
 * @note @ref CC_DRV_UART_EVT_TX_DONE event will be generated in non-blocking mode. Event will
 *       contain number of bytes sent until abort was called. If Easy DMA is not used event will be
 *       called from the function context. If Easy DMA is used it will be called from UART interrupt
 *       context.
 */
void cc_drv_uart_tx_abort(void);

/**
 * @brief Function for receiving data over UART.
 *
 * If an event handler was provided in cc_drv_uart_init() call, this function
 * returns immediately and the handler is called when the transfer is done.
 * Otherwise, the transfer is performed in blocking mode, i.e. this function
 * returns when the transfer is finished. Blocking mode is not using interrupt so
 * there is no context switching inside the function.
 *
 * @note Peripherals using EasyDMA (i.e. UARTE) require that the transfer buffers
 *       are placed in the Data RAM region. If they are not and UARTE instance is
 *       used, this function will fail with error code CC_ERROR_INVALID_ADDR.
 * @param[in] p_data Pointer to data.
 * @param[in] length Number of bytes to receive.
 *
 * @retval    CC_SUCCESS            If initialization was successful.
 * @retval    CC_ERROR_BUSY         If driver is already receiving.
 * @retval    CC_ERROR_INVALID_ADDR If p_data does not point to RAM buffer (UARTE only).
 * @retval    CC_ERROR_FORBIDDEN    If transfer was aborted (blocking mode only).
 */
int cc_drv_uart_rx(uint8_t * p_data, uint8_t length);

/**
 * @brief Function for enabling receiver.
 *
 * UART has 6 byte long RX FIFO and it will be used to store incoming data. If user will not call
 * UART receive function before FIFO is filled, overrun error will encounter. Enabling receiver
 * without specifying RX buffer is supported only in UART mode (without Easy DMA). Receiver must be
 * explicitly closed by the user @sa cc_drv_uart_rx_disable. Function asserts if mode is wrong.
 */
void cc_drv_uart_rx_enable(void);

/**
 * @brief Function for disabling receiver.
 *
 * Function must be called to close the receiver after it has been explicitly enabled by
 * @sa cc_drv_uart_rx_enable. Feature is supported only in UART mode (without Easy DMA). Function
 * asserts if mode is wrong.
 */
void cc_drv_uart_rx_disable(void);

/**
 * @brief Function for aborting any ongoing reception.
 * @note @ref CC_DRV_UART_EVT_RX_DONE event will be generated in non-blocking mode. Event will
 *       contain number of bytes received until abort was called. If Easy DMA is not used event will be
 *       called from the function context. If Easy DMA is used it will be called from UART interrupt
 *       context.
 */
void cc_drv_uart_rx_abort(void);

#endif //_UART_H
/** @} */
