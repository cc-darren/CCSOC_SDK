/******************************************************************************
*  Copyright 2017, Cloud Chip, Inc.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Cloud Chip, Inc. (C) 2017
******************************************************************************/

#include "global.h"
#include "app_uart.h"
#include "app_fifo.h"
#include "drvi_uart.h"
#include "uart.h"

static __INLINE uint32_t fifo_length(app_fifo_t * const fifo)
{
  uint32_t tmp = fifo->read_pos;
  return fifo->write_pos - tmp;
}

#define FIFO_LENGTH(F) fifo_length(&F)              /**< Macro to calculate length of a FIFO. */


static app_uart_event_handler_t   m_event_handler;            /**< Event handler function. */
__align(4) static uint8_t tx_tmp;
__align(4) static uint8_t tx_buffer[1];
__align(4) static uint8_t rx_buffer[1];

static app_fifo_t                  m_rx_fifo;                               /**< RX FIFO buffer for storing data received on the UART until the application fetches them using app_uart_get(). */
static app_fifo_t                  m_tx_fifo;                               /**< TX FIFO buffer for storing data to be transmitted on the UART when TXD is ready. Data is put to the buffer on using app_uart_put(). */

static const cc_drv_uart_t app_uart0 = CC_DRV_UART_INSTANCE(0);

static void uart_event_handler(cc_drv_uart_event_t * p_event, void* p_context)
{
    app_uart_evt_t app_uart_event;

    if (p_event->type == CC_DRV_UART_EVT_RX_DONE)
    {
        // Write received byte to FIFO
        uint32_t err_code = app_fifo_put(&m_rx_fifo, p_event->data.rxtx.p_data[0]);
        if (err_code != CC_SUCCESS)
        {
            app_uart_event.evt_type          = APP_UART_FIFO_ERROR;
            app_uart_event.data.error_code   = err_code;
            if (m_event_handler)
                m_event_handler(&app_uart_event);
        }
        // Notify that new data is available if this was first byte put in the buffer.
        else if (FIFO_LENGTH(m_rx_fifo) == 1)
        {
            app_uart_event.evt_type = APP_UART_DATA_READY;
            if (m_event_handler)
                m_event_handler(&app_uart_event);
        }
        else
        {
            // Do nothing, only send event if first byte was added or overflow in FIFO occurred.
        }
        if (FIFO_LENGTH(m_rx_fifo) <= m_rx_fifo.buf_size_mask)
        {
            (void)cc_drv_uart_rx(&app_uart0, rx_buffer, 1);
        }
    }
    else if (p_event->type == CC_DRV_UART_EVT_ERROR)
    {
        app_uart_event.evt_type                 = APP_UART_COMMUNICATION_ERROR;
        app_uart_event.data.error_communication = p_event->data.error.error_mask;
        m_event_handler(&app_uart_event);
    }
    else if (p_event->type == CC_DRV_UART_EVT_TX_DONE)
    {
        // Get next byte from FIFO.
        if (app_fifo_get(&m_tx_fifo, tx_buffer) == CC_SUCCESS)
        {
            (void)cc_drv_uart_tx(&app_uart0, tx_buffer, 1);
        }
        if (FIFO_LENGTH(m_tx_fifo) == 0)
        {
            // Last byte from FIFO transmitted, notify the application.
            // Notify that new data is available if this was first byte put in the buffer.
            app_uart_event.evt_type = APP_UART_TX_EMPTY;
            m_event_handler(&app_uart_event);
        }
    }
}

uint32_t app_uart_init(const app_uart_comm_params_t * p_comm_params,
                             app_uart_buffers_t *     p_buffers,
                             app_uart_event_handler_t event_handler)
{
    uint32_t err_code;

    m_event_handler = event_handler;

    if (p_buffers == NULL)
    {
        return CC_ERROR_INVALID_PARAM;
    }

    // Configure buffer RX buffer.
    err_code = app_fifo_init(&m_rx_fifo, p_buffers->rx_buf, p_buffers->rx_buf_size);
    if (err_code != CC_SUCCESS)
    {
        // Propagate error code.
        return err_code;
    }

    // Configure buffer TX buffer.
    err_code = app_fifo_init(&m_tx_fifo, p_buffers->tx_buf, p_buffers->tx_buf_size);
    if (err_code != CC_SUCCESS)
    {
        // Propagate error code.
        return err_code;
    }

    drvi_uart_params_t config = DRVI_UART_DEFAULT_CONFIG;
    //config.baudrate = (drvi_uart_baudrate_t)p_comm_params->baud_rate;
    //config.hw_flow = (p_comm_params->flow_control == APP_UART_FLOW_CONTROL_DISABLED) ?
    //        DRVI_UART_HWFC_DISABLE : DRVI_UART_HWFC_ENABLE;
    //config.parity = p_comm_params->use_parity ? CC_UART_PARITY_INCLUDED : CC_UART_PARITY_EXCLUDED;

    err_code = drvi_uart_init(&app_uart0, &config, uart_event_handler);

    if (err_code != CC_SUCCESS)
    {
        return err_code;
    }

    //cc_drv_uart_rx_enable();
    //return cc_drv_uart_rx(rx_buffer,1);
    return CC_SUCCESS;
}

uint32_t app_uart_flush(void)
{
    uint32_t err_code;

    err_code = app_fifo_flush(&m_rx_fifo);
    if (err_code != CC_SUCCESS)
    {
        return err_code;
    }

    err_code = app_fifo_flush(&m_tx_fifo);
    if (err_code != CC_SUCCESS)
    {
        return err_code;
    }

    return CC_SUCCESS;
}

uint32_t app_uart_get(uint8_t * p_byte)
{
    // ASSERT(p_byte);
    // If FIFO was full new request to receive one byte was not scheduled. Must be done here.
    if (FIFO_LENGTH(m_rx_fifo) == m_rx_fifo.buf_size_mask)
    {
        uint32_t err_code = cc_drv_uart_rx(&app_uart0, rx_buffer,1);
        if (err_code != CC_SUCCESS)
        {
            return CC_ERROR_NOT_FOUND;
        }
    }
    return app_fifo_get(&m_rx_fifo, p_byte);
}

uint32_t app_uart_put(uint8_t byte)
{
    uint32_t err_code;

    tx_tmp = byte;
    err_code = cc_drv_uart_tx(&app_uart0, &tx_tmp, 1);

    if (err_code == CC_ERROR_BUSY)
    {
        err_code = app_fifo_put(&m_tx_fifo, byte);
    }

    return err_code;
}

uint32_t app_uart_close(void)
{
    cc_drv_uart_uninit();
    return CC_SUCCESS;
}



/******************************************************************************
 ***    APIs required by BLE SW IP 
 ***    for HCI / AHI / etc.
 ******************************************************************************/
void app_ble_uart_read(uint8_t *bufptr, uint32_t size, rwip_eif_callback callback, void* dummy)
{
}

void app_ble_uart_write(uint8_t *bufptr, uint32_t size, rwip_eif_callback callback, void* dummy)
{
}

void app_ble_uart_flow_on(void)
{
}

bool app_ble_uart_flow_off(void)
{
    return (1);
}

const struct rwip_eif_api app_ble_uart_api =
{
    app_ble_uart_read,
    app_ble_uart_write,
    app_ble_uart_flow_on,
    app_ble_uart_flow_off,
};

const struct rwip_eif_api* rwip_eif_get(uint8_t type)
{
    const struct rwip_eif_api* ret = NULL;

    switch(type)
    {
    case RWIP_EIF_AHI:
         {
             ret = &app_ble_uart_api;
         }
         break;

#if (BLE_EMB_PRESENT) || (BT_EMB_PRESENT)
    case RWIP_EIF_HCIC:
         {
             ret = &app_ble_uart_api;
         }
         break;
#elif !(BLE_EMB_PRESENT) || !(BT_EMB_PRESENT)
    case RWIP_EIF_HCIH:
         {
             ret = &app_ble_uart2_api;
         }
         break;
#endif // (BLE_EMB_PRESENT)

    default:
         {
         //ASSERT_INFO(0, type, 0);
         }
         break;
    }
    return ret;
}

