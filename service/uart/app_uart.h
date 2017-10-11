/* Copyright (c) 2013 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

/**@file
 *
 * @defgroup app_uart UART module
 * @{
 * @ingroup app_common
 *
 * @brief UART module interface.
 */

#ifndef APP_UART_H__
#define APP_UART_H__

#include <stdint.h>
#include <stdbool.h>

#define  UART_PIN_DISCONNECTED 0xFFFFFFFF /**< Value indicating that no pin is connected to this UART register. */

/**@brief UART Flow Control modes for the peripheral.
 */
typedef enum
{
    APP_UART_FLOW_CONTROL_DISABLED, /**< UART Hw Flow Control is disabled. */
    APP_UART_FLOW_CONTROL_ENABLED,  /**< Standard UART Hw Flow Control is enabled. */
    APP_UART_FLOW_CONTROL_LOW_POWER /**< Specialized UART Hw Flow Control is used. The Low Power setting allows the \nRFXX to Power Off the UART module when CTS is in-active, and re-enabling the UART when the CTS signal becomes active. This allows the \nRFXX to safe power by only using the UART module when it is needed by the remote site. */
} app_uart_flow_control_t;

/**
 * @enum cc_uart_baudrate_t
 * @brief Baudrates supported by UART.
 */
typedef enum
{
    APP_UART_BAUDRATE_1200, /**< 1200 baud. */
    APP_UART_BAUDRATE_2400, /**< 2400 baud. */
    APP_UART_BAUDRATE_4800, /**< 4800 baud. */
    APP_UART_BAUDRATE_9600, /**< 9600 baud. */
    APP_UART_BAUDRATE_14400, /**< 14400 baud. */
    APP_UART_BAUDRATE_19200, /**< 19200 baud. */
    APP_UART_BAUDRATE_28800, /**< 28800 baud. */
    APP_UART_BAUDRATE_38400, /**< 38400 baud. */
    APP_UART_BAUDRATE_56000, /**< 56000 baud. */
    APP_UART_BAUDRATE_115200, /**< 115200 baud. */
    APP_UART_BAUDRATE_230400, /**< 230400 baud. */
    APP_UART_BAUDRATE_345600, /**< 345600 baud. */
    APP_UART_BAUDRATE_460800, /**< 460800 baud. */
    APP_UART_BAUDRATE_921600, /**< 921600 baud. */
    APP_UART_BAUDRATE_1105920, /**< 1105920 baud. */
} app_uart_baudrate_t;

/**@brief UART communication structure holding configuration settings for the peripheral.
 */
typedef struct
{
    app_uart_flow_control_t flow_control; /**< Flow control setting, if flow control is used, the system will use low power UART mode, based on CTS signal. */
    bool                    use_parity;   /**< Even parity if TRUE, no parity if FALSE. */
    uint32_t                baud_rate;    /**< Baud rate configuration. */
} app_uart_comm_params_t;

/**@brief UART buffer for transmitting/receiving data.
 */
typedef struct
{
    uint8_t * rx_buf;      /**< Pointer to the RX buffer. */
    uint32_t  rx_buf_size; /**< Size of the RX buffer. */
    uint8_t * tx_buf;      /**< Pointer to the TX buffer. */
    uint32_t  tx_buf_size; /**< Size of the TX buffer. */
} app_uart_buffers_t;

/**@brief Enumeration which defines events used by the UART module upon data reception or error.
 *
 * @details The event type is used to indicate the type of additional information in the event
 * @ref app_uart_evt_t.
 */
typedef enum
{
    APP_UART_DATA_READY,          /**< An event indicating that UART data has been received. The data is available in the FIFO and can be fetched using @ref app_uart_get. */
    APP_UART_FIFO_ERROR,          /**< An error in the FIFO module used by the app_uart module has occured. The FIFO error code is stored in app_uart_evt_t.data.error_code field. */
    APP_UART_COMMUNICATION_ERROR, /**< An communication error has occured during reception. The error is stored in app_uart_evt_t.data.error_communication field. */
    APP_UART_TX_EMPTY,            /**< An event indicating that UART has completed transmission of all available data in the TX FIFO. */
    APP_UART_DATA,                /**< An event indicating that UART data has been received, and data is present in data field. This event is only used when no FIFO is configured. */
} app_uart_evt_type_t;

/**@brief Struct containing events from the UART module.
 *
 * @details The app_uart_evt_t is used to notify the application of asynchronous events when data
 * are received on the UART peripheral or in case an error occured during data reception.
 */
typedef struct
{
    app_uart_evt_type_t evt_type; /**< Type of event. */
    union
    {
        uint32_t error_communication; /**< Field used if evt_type is: APP_UART_COMMUNICATION_ERROR. This field contains the value in the ERRORSRC register for the UART peripheral. The UART_ERRORSRC_x defines from nrf5x_bitfields.h can be used to parse the error code. See also the \nRFXX Series Reference Manual for specification. */
        uint32_t error_code;          /**< Field used if evt_type is: NRF_ERROR_x. Additional status/error code if the error event type is APP_UART_FIFO_ERROR. This error code refer to errors defined in nrf_error.h. */
        uint8_t  value;               /**< Field used if evt_type is: NRF_ERROR_x. Additional status/error code if the error event type is APP_UART_FIFO_ERROR. This error code refer to errors defined in nrf_error.h. */
    } data;
} app_uart_evt_t;

/**@brief Function for handling app_uart event callback.
 *
 * @details Upon an event in the app_uart module this callback function will be called to notify
 *          the application about the event.
 *
 * @param[in]   p_app_uart_event Pointer to UART event.
 */
typedef void (* app_uart_event_handler_t) (app_uart_evt_t * p_app_uart_event);

/**@brief Macro for safe initialization of the UART module in a single user instance when using
 *        a FIFO together with UART.
 *
 * @param[in]   P_COMM_PARAMS   Pointer to a UART communication structure: app_uart_comm_params_t
 * @param[in]   RX_BUF_SIZE     Size of desired RX buffer, must be a power of 2 or ZERO (No FIFO).
 * @param[in]   TX_BUF_SIZE     Size of desired TX buffer, must be a power of 2 or ZERO (No FIFO).
 * @param[in]   EVT_HANDLER   Event handler function to be called when an event occurs in the
 *                              UART module.
 * @param[in]   IRQ_PRIO        IRQ priority, app_irq_priority_t, for the UART module irq handler.
 * @param[out]  ERR_CODE        The return value of the UART initialization function will be
 *                              written to this parameter.
 *
 * @note Since this macro allocates a buffer and registers the module as a GPIOTE user when flow
 *       control is enabled, it must only be called once.
 */
#define APP_UART_FIFO_INIT(P_COMM_PARAMS, RX_BUF_SIZE, TX_BUF_SIZE, EVT_HANDLER, ERR_CODE) \
    do                                                                                             \
    {                                                                                              \
        app_uart_buffers_t buffers;                                                                \
        static uint8_t     rx_buf[RX_BUF_SIZE];                                                    \
        static uint8_t     tx_buf[TX_BUF_SIZE];                                                    \
                                                                                                   \
        buffers.rx_buf      = rx_buf;                                                              \
        buffers.rx_buf_size = sizeof (rx_buf);                                                     \
        buffers.tx_buf      = tx_buf;                                                              \
        buffers.tx_buf_size = sizeof (tx_buf);                                                     \
        ERR_CODE = app_uart_init(P_COMM_PARAMS, &buffers, EVT_HANDLER);                  \
    } while (0)

/**@brief Macro for safe initialization of the UART module in a single user instance.
 *
 * @param[in]   P_COMM_PARAMS   Pointer to a UART communication structure: app_uart_comm_params_t
 * @param[in]   EVT_HANDLER   Event handler function to be called when an event occurs in the
 *                              UART module.
 * @param[in]   IRQ_PRIO        IRQ priority, app_irq_priority_t, for the UART module irq handler.
 * @param[out]  ERR_CODE        The return value of the UART initialization function will be
 *                              written to this parameter.
 *
 * @note Since this macro allocates registers the module as a GPIOTE user when flow control is
 *       enabled, it must only be called once.
 */
#define APP_UART_INIT(P_COMM_PARAMS, EVT_HANDLER, IRQ_PRIO, ERR_CODE)                              \
    do                                                                                             \
    {                                                                                              \
        ERR_CODE = app_uart_init(P_COMM_PARAMS, NULL, EVT_HANDLER, IRQ_PRIO);                      \
    } while (0)

/**@brief Function for initializing the UART module. Use this initialization when several instances of the UART
 *        module are needed.
 *
 *
 * @note Normally single initialization should be done using the APP_UART_INIT() or
 *       APP_UART_INIT_FIFO() macro depending on whether the FIFO should be used by the UART, as
 *       that will allocate the buffers needed by the UART module (including aligning the buffer
 *       correctly).

 * @param[in]     p_comm_params     Pin and communication parameters.
 * @param[in]     p_buffers         RX and TX buffers, NULL is FIFO is not used.
 * @param[in]     error_handler     Function to be called in case of an error.
 * @param[in]     irq_priority      Interrupt priority level.
 *
 * @retval      NRF_SUCCESS               If successful initialization.
 * @retval      NRF_ERROR_INVALID_LENGTH  If a provided buffer is not a power of two.
 * @retval      NRF_ERROR_NULL            If one of the provided buffers is a NULL pointer.
 *
 * The below errors are propagated by the UART module to the caller upon registration when Hardware
 * Flow Control is enabled. When Hardware Flow Control is not used, these errors cannot occur.
 * @retval      NRF_ERROR_INVALID_STATE   The GPIOTE module is not in a valid state when registering
 *                                        the UART module as a user.
 * @retval      NRF_ERROR_INVALID_PARAM   The UART module provides an invalid callback function when
 *                                        registering the UART module as a user.
 *                                        Or the value pointed to by *p_uart_uid is not a valid
 *                                        GPIOTE number.
 * @retval      NRF_ERROR_NO_MEM          GPIOTE module has reached the maximum number of users.
 */
uint32_t app_uart_init(const app_uart_comm_params_t * p_comm_params,
                       app_uart_buffers_t *           p_buffers,
                       app_uart_event_handler_t       error_handler);

/**@brief Function for getting a byte from the UART.
 *
 * @details This function will get the next byte from the RX buffer. If the RX buffer is empty
 *          an error code will be returned and the app_uart module will generate an event upon
 *          reception of the first byte which is added to the RX buffer.
 *
 * @param[out] p_byte    Pointer to an address where next byte received on the UART will be copied.
 *
 * @retval NRF_SUCCESS          If a byte has been received and pushed to the pointer provided.
 * @retval NRF_ERROR_NOT_FOUND  If no byte is available in the RX buffer of the app_uart module.
 */
uint32_t app_uart_get(uint8_t * p_byte);

/**@brief Function for putting a byte on the UART.
 *
 * @details This call is non-blocking.
 *
 * @param[in] byte   Byte to be transmitted on the UART.
 *
 * @retval NRF_SUCCESS        If the byte was successfully put on the TX buffer for transmission.
 * @retval NRF_ERROR_NO_MEM   If no more space is available in the TX buffer.
 *                            NRF_ERROR_NO_MEM may occur if flow control is enabled and CTS signal
 *                            is high for a long period and the buffer fills up.
 * @retval NRF_ERROR_INTERNAL If UART driver reported error.
 */
uint32_t app_uart_put(uint8_t byte);

/**@brief Function for flushing the RX and TX buffers (Only valid if FIFO is used).
 *        This function does nothing if FIFO is not used.
 *
 * @retval  NRF_SUCCESS  Flushing completed (Current implementation will always succeed).
 */
uint32_t app_uart_flush(void);

/**@brief Function for closing the UART module.
 *
 * @retval  NRF_SUCCESS             If successfully closed.
 * @retval  NRF_ERROR_INVALID_PARAM If an invalid user id is provided or the user id differs from
 *                                  the current active user.
 */
uint32_t app_uart_close(void);





//#ifdef CFG_BLE_APP
/******************************************************************************
 ***    APIs required by BLE SW IP 
 ***    for HCI / AHI / etc.
 ******************************************************************************/
#include "rwip.h"

void app_ble_uart_read(uint8_t *bufptr, uint32_t size, rwip_eif_callback callback, void* dummy);
void app_ble_uart_write(uint8_t *bufptr, uint32_t size, rwip_eif_callback callback, void* dummy);
void app_ble_uart_flow_on(void);
bool app_ble_uart_flow_off(void);
const struct rwip_eif_api* rwip_eif_get(uint8_t type);
//#endif

#endif //APP_UART_H__

/** @} */
