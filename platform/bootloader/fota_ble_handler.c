/* Copyright (c) 2016 Nordic Semiconductor. All Rights Reserved.
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



#include "fota_ble_handler.h"
#include "fota_req_handler.h"
#include "tracer.h"
#include "app_ota.h"
#include "string.h"


#define MAX_RESPONSE_LEN                    (15)                                                    /**< Maximum length (in bytes) of the response to a Control Point command. */
#define PKT_CREATE_PARAM_LEN                (6)                                                     /**< Length (in bytes) of the parameters for Create Object request. */
#define PKT_SET_PRN_PARAM_LEN               (3)                                                     /**< Length (in bytes) of the parameters for Set Packet Receipt Notification request. */
#define PKT_READ_OBJECT_INFO_PARAM_LEN      (2)                                                     /**< Length (in bytes) of the parameters for Read Object Info request. */

static uint8_t  m_notif_buffer[MAX_RESPONSE_LEN];                                                    /**< Buffer used for sending notifications to peer. */

static uint16_t             m_pkt_notif_target;                                                      /**< Number of packets of firmware data to be received before transmitting the next Packet Receipt Notification to the DFU Controller. */
static uint16_t             m_pkt_notif_target_cnt;                                                  /**< Number of packets of firmware data received after sending last Packet Receipt Notification or since the receipt of a @ref BLE_DFU_PKT_RCPT_NOTIF_ENABLED event from the DFU service, which ever occurs later.*/



/**@brief Function for encoding a uint32 value.
 *
 * @param[in]   value            Value to be encoded.
 * @param[out]  p_encoded_data   Buffer where the encoded data is to be written.
 *
 * @return      Number of bytes written.
 */
static __INLINE uint8_t uint32_encode(uint32_t value, uint8_t * p_encoded_data)
{
    p_encoded_data[0] = (uint8_t) ((value & 0x000000FF) >> 0);
    p_encoded_data[1] = (uint8_t) ((value & 0x0000FF00) >> 8);
    p_encoded_data[2] = (uint8_t) ((value & 0x00FF0000) >> 16);
    p_encoded_data[3] = (uint8_t) ((value & 0xFF000000) >> 24);
    return sizeof(uint32_t);
}


/**@brief Function for decoding a uint32 value.
 *
 * @param[in]   p_encoded_data   Buffer where the encoded data is stored.
 *
 * @return      Decoded value.
 */
static __INLINE uint32_t uint32_decode(const uint8_t * p_encoded_data)
{
    return ( (((uint32_t)((uint8_t *)p_encoded_data)[0]) << 0)  |
             (((uint32_t)((uint8_t *)p_encoded_data)[1]) << 8)  |
             (((uint32_t)((uint8_t *)p_encoded_data)[2]) << 16) |
             (((uint32_t)((uint8_t *)p_encoded_data)[3]) << 24 ));
}


/**@brief Function for decoding a uint16 value.
 *
 * @param[in]   p_encoded_data   Buffer where the encoded data is stored.
 *
 * @return      Decoded value.
 */
static __INLINE uint16_t uint16_decode(const uint8_t * p_encoded_data)
{
        return ( (((uint16_t)((uint8_t *)p_encoded_data)[0])) |
                 (((uint16_t)((uint8_t *)p_encoded_data)[1]) << 8 ));
}


//static uint32_t send_hvx(uint16_t conn_handle, uint16_t value_handle, uint16_t len)
//static void send_hvx(uint16_t len)
void send_hvx(uint16_t len)
{
    /*
    ble_gatts_hvx_params_t hvx_params   = {0};

    hvx_params.handle = value_handle;
    hvx_params.type   = BLE_GATT_HVX_NOTIFICATION;
    hvx_params.p_len  = &len;
    hvx_params.p_data = m_notif_buffer;

    return sd_ble_gatts_hvx(conn_handle, &hvx_params);
    */

    app_ota_notify_send(m_notif_buffer, len);

}



//static uint32_t response_send(ble_dfu_t          * p_dfu,
static void response_send(      uint8_t op_code, nrf_dfu_res_code_t   resp_val)
{
    uint16_t index = 0;

    //TracerInfo("Sending Response: [0x%01x, 0x%01x]\r\n", op_code, resp_val); // can't use in interrupt state.

/*
    if ((m_conn_handle == BLE_CONN_HANDLE_INVALID) || (m_flags & DFU_BLE_FLAG_SERVICE_INITIALIZED) == 0)
    {
        return NRF_ERROR_INVALID_STATE;
    }
*/
    m_notif_buffer[index++] = BLE_DFU_OP_CODE_RESPONSE;

    // Encode the Request Op code
    m_notif_buffer[index++] = op_code;

    // Encode the Response Value.
    m_notif_buffer[index++] = (uint8_t)resp_val;

    send_hvx(index);

    //return send_hvx(m_conn_handle, p_dfu->dfu_ctrl_pt_handles.value_handle, index);
}




static void response_crc_cmd_send(uint32_t offset, uint32_t crc)
{
    uint16_t               index = 0;

    //TracerInfo("Sending CRC: [0x60, 0x03, 0x01, 0:x%08x, CRC:0x%08x]\r\n", offset, crc);

/*
    if ((m_conn_handle == BLE_CONN_HANDLE_INVALID) || (m_flags & DFU_BLE_FLAG_SERVICE_INITIALIZED) == 0)
    {
        return NRF_ERROR_INVALID_STATE;
    }
*/
    m_notif_buffer[index++] = BLE_DFU_OP_CODE_RESPONSE;

    // Encode the Request Op code
    m_notif_buffer[index++] = BLE_DFU_OP_CODE_CALCULATE_CRC;

    // Encode the Response Value.
    m_notif_buffer[index++] = (uint8_t)NRF_DFU_RES_CODE_SUCCESS;

    // Encode the Offset Value.
    index += uint32_encode(offset, &m_notif_buffer[index]);

    // Encode the Crc Value.
    index += uint32_encode(crc, &m_notif_buffer[index]);

    send_hvx(index);

}





static void response_select_object_cmd_send(uint32_t max_size, uint32_t  offset, uint32_t crc)
{
    uint16_t               index = 0;

    //TracerInfo("Sending Object Info: [0x60, 0x06, 0x01 max: 0:x%08x 0:x%08x, CRC:0x%08x]\r\n", max_size, offset, crc);
/*
    if ((m_conn_handle == BLE_CONN_HANDLE_INVALID) || (m_flags & DFU_BLE_FLAG_SERVICE_INITIALIZED) == 0)
    {
        return NRF_ERROR_INVALID_STATE;
    }
*/
    m_notif_buffer[index++] = BLE_DFU_OP_CODE_RESPONSE;

    // Encode the Request Op code
    m_notif_buffer[index++] = BLE_DFU_OP_CODE_SELECT_OBJECT;

    // Encode the Success Response Value.
    m_notif_buffer[index++] = (uint8_t)NRF_DFU_RES_CODE_SUCCESS;

    // Encode the Max Size Value.
    index += uint32_encode(max_size, &m_notif_buffer[index]);

    // Encode the Offset Value.
    index += uint32_encode(offset, &m_notif_buffer[index]);

    // Encode the Crc Value.
    index += uint32_encode(crc, &m_notif_buffer[index]);

    send_hvx(index);
    //return send_hvx(m_conn_handle, p_dfu->dfu_ctrl_pt_handles.value_handle, index);
}




/**@brief     Function for handling a Write event on the Control Point characteristic.
 *
 * @param[in] p_dfu             DFU Service Structure.
 * @param[in] p_ble_write_evt   Pointer to the write event received from BLE stack.
 *
 * @return    NRF_SUCCESS on successful processing of control point write. Otherwise an error code.
 */
void fota_on_ctrl_pt_write(struct otat_packet_send_cmd const *param)
{
    nrf_dfu_res_code_t  res_code;
    nrf_dfu_req_t       dfu_req;
    nrf_dfu_res_t       dfu_res = {{{0}}};

    memset(&dfu_req, 0, sizeof(nrf_dfu_req_t));

    switch (param->value[0])
    {
        case BLE_DFU_OP_CODE_CREATE_OBJECT:

            if (param->length != PKT_CREATE_PARAM_LEN)
            {
                response_send(BLE_DFU_OP_CODE_CREATE_OBJECT,
                                     NRF_DFU_RES_CODE_INVALID_PARAMETER);

                return;                     
            }

            TracerInfo("Received create object\r\n");

            // Reset the packet receipt notification on create object
            m_pkt_notif_target_cnt = m_pkt_notif_target;

            // Get type parameter
            //lint -save -e415
            dfu_req.obj_type    =  param->value[1];
            //lint -restore

            // Get length value
            //lint -save -e416
            dfu_req.object_size = uint32_decode(&(param->value[2]));
            //TracerInfo("object_size:%d, param[2]:%x, param[3]:%x\r\n",dfu_req.object_size,param->value[2],param->value[3]);
            //lint -restore

            // Set req type
            dfu_req.req_type        = NRF_DFU_OBJECT_OP_CREATE;

            res_code = nrf_dfu_req_handler_on_req(NULL, &dfu_req, &dfu_res);
            response_send(BLE_DFU_OP_CODE_CREATE_OBJECT, res_code);
            return;
        case BLE_DFU_OP_CODE_EXECUTE_OBJECT:
            TracerInfo("Received execute object\r\n");

            // Set req type
            dfu_req.req_type     =  NRF_DFU_OBJECT_OP_EXECUTE;

            res_code = nrf_dfu_req_handler_on_req(NULL, &dfu_req, &dfu_res);

            response_send(BLE_DFU_OP_CODE_EXECUTE_OBJECT, res_code);

            return;
        case BLE_DFU_OP_CODE_SET_RECEIPT_NOTIF:
            TracerInfo("Set receipt notif\r\n");
            if (param->length != PKT_SET_PRN_PARAM_LEN)
            {
                (response_send(BLE_DFU_OP_CODE_SET_RECEIPT_NOTIF,
                                      NRF_DFU_RES_CODE_INVALID_PARAMETER));
                return;                     
            }

            //lint -save -e415
            m_pkt_notif_target = uint16_decode(&(param->value[1]));
            //lint -restore
            m_pkt_notif_target_cnt   = m_pkt_notif_target;

            response_send(BLE_DFU_OP_CODE_SET_RECEIPT_NOTIF, NRF_DFU_RES_CODE_SUCCESS);
            return;
        case BLE_DFU_OP_CODE_CALCULATE_CRC:
            TracerInfo("Received calculate CRC\r\n");

            dfu_req.req_type     =  NRF_DFU_OBJECT_OP_CRC;

            res_code = nrf_dfu_req_handler_on_req(NULL, &dfu_req, &dfu_res);
            if (res_code == NRF_DFU_RES_CODE_SUCCESS)
            {
                response_crc_cmd_send(dfu_res.offset, dfu_res.crc);
                return;
            }
            else
            {
                response_send(BLE_DFU_OP_CODE_CALCULATE_CRC, res_code);
                return;
            }
            
        case BLE_DFU_OP_CODE_SELECT_OBJECT:

            TracerInfo("Received select object\r\n");
            if (param->length != PKT_READ_OBJECT_INFO_PARAM_LEN)
            {
                response_send(BLE_DFU_OP_CODE_SELECT_OBJECT,
                                     NRF_DFU_RES_CODE_INVALID_PARAMETER);
                return;                     
            }

            // Set object type to read info about
            //lint -save -e415
            dfu_req.obj_type = param->value[1];
            //lint -restore

            dfu_req.req_type = NRF_DFU_OBJECT_OP_SELECT;

            res_code = nrf_dfu_req_handler_on_req(NULL, &dfu_req, &dfu_res);
            if (res_code == NRF_DFU_RES_CODE_SUCCESS)
            {
                response_select_object_cmd_send(dfu_res.max_size, dfu_res.offset, dfu_res.crc);
                return;
            }
            else
            {
                response_send(BLE_DFU_OP_CODE_SELECT_OBJECT, res_code);
                return;
            }

        default:
            TracerInfo("Received unsupported OP code\r\n");
            // Unsupported op code.
            response_send(param->value[0], NRF_DFU_RES_CODE_INVALID_PARAMETER);
            return;
            /*
            return response_send(p_dfu,
                                 param->value[0],
                                 NRF_DFU_RES_CODE_INVALID_PARAMETER);
            */
    }
}



#if 0 // Not used??
/**@brief     Function for handling the @ref BLE_GATTS_EVT_RW_AUTHORIZE_REQUEST event from the
 *            SoftDevice.
 *
 * @param[in] p_dfu     DFU Service Structure.
 * @param[in] p_ble_evt Pointer to the event received from BLE stack.
 */
static bool on_rw_authorize_req(ble_dfu_t * p_dfu, ble_evt_t * p_ble_evt)
{
    uint32_t                                err_code;
    ble_gatts_rw_authorize_reply_params_t   auth_reply = {0};
    ble_gatts_evt_rw_authorize_request_t  * p_authorize_request;
    ble_gatts_evt_write_t                 * p_ble_write_evt;
    
    p_authorize_request = &(p_ble_evt->evt.gatts_evt.params.authorize_request);
    p_ble_write_evt = &(p_ble_evt->evt.gatts_evt.params.authorize_request.request.write);

    if ((p_authorize_request->type == BLE_GATTS_AUTHORIZE_TYPE_WRITE)                           &&
        (p_authorize_request->request.write.handle == p_dfu->dfu_ctrl_pt_handles.value_handle)  &&
        (p_authorize_request->request.write.op != BLE_GATTS_OP_PREP_WRITE_REQ)                  &&
        (p_authorize_request->request.write.op != BLE_GATTS_OP_EXEC_WRITE_REQ_NOW)              &&
        (p_authorize_request->request.write.op != BLE_GATTS_OP_EXEC_WRITE_REQ_CANCEL)            )
    {
        auth_reply.type = BLE_GATTS_AUTHORIZE_TYPE_WRITE;
        auth_reply.params.write.update = 1;
        auth_reply.params.write.offset = p_ble_write_evt->offset;
        auth_reply.params.write.len = p_ble_write_evt->len;
        auth_reply.params.write.p_data = p_ble_write_evt->data;

        if (!is_cccd_configured(p_dfu))
        {
            // Send an error response to the peer indicating that the CCCD is improperly configured.
            auth_reply.params.write.gatt_status = BLE_GATT_STATUS_ATTERR_CPS_CCCD_CONFIG_ERROR;
 
            // Ignore response of auth reply
            (void)sd_ble_gatts_rw_authorize_reply(m_conn_handle, &auth_reply);
            return false;
        }

        auth_reply.params.write.gatt_status = BLE_GATT_STATUS_SUCCESS;

        err_code = sd_ble_gatts_rw_authorize_reply(m_conn_handle, &auth_reply);
        return err_code  == NRF_SUCCESS ? true: false;
    }
    else
    {
        return false;
    }
}
#endif



/**@brief     Function for handling the @ref BLE_GATTS_EVT_WRITE event from the SoftDevice.
 *
 * @param[in] p_dfu     DFU Service Structure.
 * @param[in] p_ble_evt Pointer to the event received from BLE stack.
 */
 // charactertic ota packets over ble cmd? 
//static void fota_on_write(ble_dfu_t * p_dfu, ble_evt_t * p_ble_evt)


void fota_on_write(struct otat_packet_send_cmd const *param)
{
    //if (p_ble_evt->evt.gatts_evt.params.write.handle == p_dfu->dfu_pkt_handles.value_handle)
    if(1)
    {
        nrf_dfu_res_code_t  res_code;
        nrf_dfu_req_t       dfu_req;
        nrf_dfu_res_t       dfu_res = {{{0}}};

        memset(&dfu_req, 0, sizeof(nrf_dfu_req_t));

        // Set req type
        dfu_req.req_type =  NRF_DFU_OBJECT_OP_WRITE;

        // Set data and length
        dfu_req.p_req    =  (uint8_t*)param->value;//p_ble_evt->evt.gatts_evt.params.write.data;
        dfu_req.req_len  =  param->length;//p_ble_evt->evt.gatts_evt.params.write.len;

        res_code = nrf_dfu_req_handler_on_req(NULL, &dfu_req, &dfu_res); 
        if(res_code != NRF_DFU_RES_CODE_SUCCESS)
        {
            TracerInfo("Failure to run packet write\r\n");
        }

        // Check if a packet receipt notification is needed to be sent.
        if (m_pkt_notif_target != 0 && --m_pkt_notif_target_cnt == 0)
        {

            response_crc_cmd_send(dfu_res.offset, dfu_res.crc);

            // Reset the counter for the number of firmware packets.
            m_pkt_notif_target_cnt = m_pkt_notif_target;
        }
    }
}



#if 0 // Not used!
/**@brief Function for the Application's SoftDevice event handler.
 *
 * @param[in] p_ble_evt SoftDevice event.
 */
static void on_ble_evt(ble_evt_t * p_ble_evt)
{
    uint32_t err_code;

    switch (p_ble_evt->header.evt_id)
    {
        case BLE_GAP_EVT_CONNECTED:
            nrf_gpio_pin_clear(CONNECTED_LED_PIN_NO);
            nrf_gpio_pin_set(ADVERTISING_LED_PIN_NO);

            m_conn_handle    = p_ble_evt->evt.gap_evt.conn_handle;
            m_flags &= ~DFU_BLE_FLAG_IS_ADVERTISING;
            break;

        case BLE_GAP_EVT_DISCONNECTED:
            // Restart advertising so that the DFU Controller can reconnect if possible.
            err_code = advertising_start();
            APP_ERROR_CHECK(err_code);

            m_conn_handle = BLE_CONN_HANDLE_INVALID;

            break;

        case BLE_GAP_EVT_SEC_PARAMS_REQUEST:
            {
                err_code = sd_ble_gap_sec_params_reply(m_conn_handle,
                                                       BLE_GAP_SEC_STATUS_PAIRING_NOT_SUPP,
                                                       NULL,
                                                       NULL);
                APP_ERROR_CHECK(err_code);
            }
            break;

        case BLE_GATTS_EVT_TIMEOUT:
            if (p_ble_evt->evt.gatts_evt.params.timeout.src == BLE_GATT_TIMEOUT_SRC_PROTOCOL)
            {
                err_code = sd_ble_gap_disconnect(m_conn_handle,
                                                 BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
                APP_ERROR_CHECK(err_code);
            }
            break;

        case BLE_EVT_USER_MEM_REQUEST:
            err_code = sd_ble_user_mem_reply(m_conn_handle, NULL);
            APP_ERROR_CHECK(err_code);
            break;

        case BLE_GATTS_EVT_RW_AUTHORIZE_REQUEST:
            if (p_ble_evt->evt.gatts_evt.params.authorize_request.type
                != BLE_GATTS_AUTHORIZE_TYPE_INVALID)
            {
                if (on_rw_authorize_req(&m_dfu, p_ble_evt))
                {
                    err_code = on_ctrl_pt_write(&m_dfu, 
                           &(p_ble_evt->evt.gatts_evt.params.authorize_request.request.write));
#ifdef NRF_DFU_DEBUG_VERSION  
                    if (err_code != NRF_SUCCESS)
                    {
                        NRF_LOG_ERROR("Could not handle on_ctrl_pt_write. err_code: 0x%04x\r\n", err_code);
                    }
#else
                    // Swallow result
                    (void) err_code;
#endif
                }
            }
            break;

        case BLE_GAP_EVT_SEC_INFO_REQUEST:
            err_code = sd_ble_gap_sec_info_reply(p_ble_evt->evt.gap_evt.conn_handle, NULL, NULL, NULL);
            APP_ERROR_CHECK(err_code);
            break;

        case BLE_GATTS_EVT_SYS_ATTR_MISSING:
            err_code = sd_ble_gatts_sys_attr_set(p_ble_evt->evt.gap_evt.conn_handle, NULL, 0, 0);
            APP_ERROR_CHECK(err_code);
            break;
        
        case BLE_GATTS_EVT_WRITE:
            on_write(&m_dfu, p_ble_evt);
            break;

#if (NRF_SD_BLE_API_VERSION == 3)
        case BLE_GATTS_EVT_EXCHANGE_MTU_REQUEST:
            err_code = sd_ble_gatts_exchange_mtu_reply(p_ble_evt->evt.gatts_evt.conn_handle, 
                                                       NRF_BLE_MAX_MTU_SIZE);
            APP_ERROR_CHECK(err_code);
            break; // BLE_GATTS_EVT_EXCHANGE_MTU_REQUEST
#endif
        
        default:
            // No implementation needed.
            break;
    }
}


/**@brief     Function for dispatching a SoftDevice event.
 *
 * @details   This function is called from the SoftDevice event interrupt handler after a
 *            SoftDevice event has been received.
 *
 * @param[in] p_ble_evt SoftDevice event.
 */
static void ble_evt_dispatch(ble_evt_t * p_ble_evt)
{
    ble_conn_params_on_ble_evt(p_ble_evt);
    on_ble_evt(p_ble_evt);
}

#endif

#if 0 // not used!
static uint32_t gap_address_change(void)
{
    uint32_t            err_code;
    ble_gap_addr_t      addr;

#ifdef NRF51
    err_code = sd_ble_gap_address_get(&addr);
#elif NRF52
    err_code = sd_ble_gap_addr_get(&addr);
#else

#endif

    VERIFY_SUCCESS(err_code);

    // Increase the BLE address by one when advertising openly.
    addr.addr[0] += 1;

#ifdef NRF51
    err_code = sd_ble_gap_address_set(BLE_GAP_ADDR_CYCLE_MODE_NONE, &addr);
#elif NRF52
    err_code = sd_ble_gap_addr_set(&addr);
#else

#endif

    VERIFY_SUCCESS(err_code);

    return NRF_SUCCESS;
}



/**@brief     Function for the GAP initialization.
 *
 * @details   This function will setup all the necessary GAP (Generic Access Profile) parameters of
 *            the device. It also sets the permissions and appearance.
 */
static uint32_t gap_params_init(void)
{
    uint32_t                err_code;
    ble_gap_conn_params_t   gap_conn_params = {0};
    ble_gap_conn_sec_mode_t sec_mode;


    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&sec_mode);

    err_code = gap_address_change();
    VERIFY_SUCCESS(err_code);

    sprintf(&deviceName[4],"%05d", (NRF_FICR->DEVICEID[0] & 0x0000FFFF));
    err_code = sd_ble_gap_device_name_set(&sec_mode,
                                          (const uint8_t *)deviceName,
                                          strlen(deviceName));
	

    //err_code = sd_ble_gap_device_name_set(&sec_mode,
    //                                      (const uint8_t *)DEVICE_NAME,
    //                                      strlen(DEVICE_NAME));
    
    VERIFY_SUCCESS(err_code);

    gap_conn_params.min_conn_interval = MIN_CONN_INTERVAL;
    gap_conn_params.max_conn_interval = MAX_CONN_INTERVAL;
    gap_conn_params.slave_latency     = SLAVE_LATENCY;
    gap_conn_params.conn_sup_timeout  = CONN_SUP_TIMEOUT;

    err_code = sd_ble_gap_ppcp_set(&gap_conn_params);
    return err_code;
}



static uint32_t ble_stack_init(bool init_softdevice)
{
    uint32_t         err_code;
    nrf_clock_lf_cfg_t clock_lf_cfg = NRF_CLOCK_LFCLKSRC;

    if (init_softdevice)
    {
        err_code = nrf_dfu_mbr_init_sd();
        VERIFY_SUCCESS(err_code);
    }

    TracerInfo("vector table: 0x%08x\r\n", BOOTLOADER_START_ADDR);
    err_code = sd_softdevice_vector_table_base_set(BOOTLOADER_START_ADDR);
    VERIFY_SUCCESS(err_code);

    SOFTDEVICE_HANDLER_APPSH_INIT(&clock_lf_cfg, true);

    ble_enable_params_t ble_enable_params;
    // Only one connection as a central is used when performing dfu.
    err_code = softdevice_enable_get_default_config(1, 1, &ble_enable_params);
    VERIFY_SUCCESS(err_code);

#if (NRF_SD_BLE_API_VERSION == 3)
    ble_enable_params.gatt_enable_params.att_mtu = NRF_BLE_MAX_MTU_SIZE;
#endif    
    
    // Enable BLE stack.
    err_code = softdevice_enable(&ble_enable_params);
    return err_code;
}



/**@brief       Function for adding DFU Packet characteristic to the BLE Stack.
 *
 * @param[in]   p_dfu DFU Service structure.
 *
 * @return      NRF_SUCCESS on success. Otherwise an error code.
 */
static uint32_t dfu_pkt_char_add(ble_dfu_t * const p_dfu)
{
    ble_gatts_char_md_t char_md             = {{0}};
    ble_gatts_attr_t    attr_char_value     = {0};
    ble_gatts_attr_md_t attr_md             = {{0}};
    ble_uuid_t          char_uuid;

    char_md.char_props.write_wo_resp = 1;

    char_uuid.type = p_dfu->uuid_type;
    char_uuid.uuid = BLE_DFU_PKT_CHAR_UUID;

    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);

    attr_md.vloc    = BLE_GATTS_VLOC_STACK;
    attr_md.vlen    = 1;

    attr_char_value.p_uuid    = &char_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.max_len   = MAX_DFU_PKT_LEN;
    attr_char_value.p_value   = NULL;

    return sd_ble_gatts_characteristic_add(p_dfu->service_handle,
                                           &char_md,
                                           &attr_char_value,
                                           &p_dfu->dfu_pkt_handles);
}


/**@brief       Function for adding DFU Control Point characteristic to the BLE Stack.
 *
 * @param[in]   p_dfu DFU Service structure.
 *
 * @return      NRF_SUCCESS on success. Otherwise an error code.
 */
static uint32_t dfu_ctrl_pt_add(ble_dfu_t * const p_dfu)
{
    ble_gatts_char_md_t char_md             = {{0}};
    ble_gatts_attr_t    attr_char_value     = {0};
    ble_gatts_attr_md_t attr_md             = {{0}};
    ble_uuid_t          char_uuid;

    char_md.char_props.write  = 1;
    char_md.char_props.notify = 1;

    char_uuid.type = p_dfu->uuid_type;
    char_uuid.uuid = BLE_DFU_CTRL_PT_UUID;

    BLE_GAP_CONN_SEC_MODE_SET_NO_ACCESS(&attr_md.read_perm);
    BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);

    attr_md.vloc    = BLE_GATTS_VLOC_STACK;
    attr_md.wr_auth = 1;
    attr_md.vlen    = 1;

    attr_char_value.p_uuid    = &char_uuid;
    attr_char_value.p_attr_md = &attr_md;
    attr_char_value.max_len   = BLE_L2CAP_MTU_DEF;
    attr_char_value.p_value   = NULL;

    return sd_ble_gatts_characteristic_add(p_dfu->service_handle,
                                           &char_md,
                                           &attr_char_value,
                                           &p_dfu->dfu_ctrl_pt_handles);
}


/**@brief     Function for checking if the CCCD of DFU Control point is configured for Notification.
 *
 * @details   This function checks if the CCCD of DFU Control Point characteristic is configured
 *            for Notification by the DFU Controller.
 *
 * @param[in] p_dfu DFU Service structure.
 *
 * @return    True if the CCCD of DFU Control Point characteristic is configured for Notification.
 *            False otherwise.
 */
uint32_t ble_dfu_init(ble_dfu_t * p_dfu)
{
    ble_uuid_t service_uuid;
    uint32_t   err_code;

#ifndef NRF51
    if (p_dfu == NULL)
    {
        return NRF_ERROR_NULL;
    }
#endif

    m_conn_handle = BLE_CONN_HANDLE_INVALID;

    BLE_UUID_BLE_ASSIGN(service_uuid, BLE_DFU_SERVICE_UUID);

    err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY,
                                        &service_uuid,
                                        &(p_dfu->service_handle));
    VERIFY_SUCCESS(err_code);

    const ble_uuid128_t base_uuid128 =
    {
        {
            0x50, 0xEA, 0xDA, 0x30, 0x88, 0x83, 0xB8, 0x9F,
            0x60, 0x4F, 0x15, 0xF3,  0x00, 0x00, 0xC9, 0x8E
        }
    };
    err_code = sd_ble_uuid_vs_add(&base_uuid128, &p_dfu->uuid_type);
    VERIFY_SUCCESS(err_code);

    err_code = dfu_pkt_char_add(p_dfu);
    VERIFY_SUCCESS(err_code);

    err_code = dfu_ctrl_pt_add(p_dfu);
    VERIFY_SUCCESS(err_code);

    m_flags |= DFU_BLE_FLAG_SERVICE_INITIALIZED;

    return NRF_SUCCESS;
}



uint32_t ble_dfu_transport_init(void)
{
    uint32_t err_code;

    m_flags &= ~DFU_BLE_FLAG_NONE;

    leds_init();

    err_code = ble_stack_init(true);
    VERIFY_SUCCESS(err_code);

    err_code = softdevice_ble_evt_handler_set(ble_evt_dispatch);
    VERIFY_SUCCESS(err_code);

    err_code = gap_params_init();
    VERIFY_SUCCESS(err_code);

    // Initialize the Device Firmware Update Service.
    err_code = ble_dfu_init(&m_dfu);
    VERIFY_SUCCESS(err_code);

    err_code = conn_params_init();
    VERIFY_SUCCESS(err_code);

    err_code = advertising_start();
    VERIFY_SUCCESS(err_code);

    return NRF_SUCCESS;
}


uint32_t ble_dfu_transport_close(void)
{
    uint32_t err_code = NRF_SUCCESS;

    if ((m_flags & DFU_BLE_FLAG_TEAR_DOWN_IN_PROGRESS) != 0)
    {
        return NRF_SUCCESS;
    }

    m_flags |= DFU_BLE_FLAG_TEAR_DOWN_IN_PROGRESS;

    TracerInfo("Waiting for buffers to be cleared before disconnect\r\n");
    nrf_delay_ms(MAX_CONN_INTERVAL_MS*4);
    TracerInfo("Disconnecting\r\n");

    if (m_conn_handle != BLE_CONN_HANDLE_INVALID)
    {
        // Disconnect from peer.
        err_code = sd_ble_gap_disconnect(m_conn_handle, BLE_HCI_REMOTE_USER_TERMINATED_CONNECTION);
        VERIFY_SUCCESS(err_code);
    }
    else if ((m_flags & DFU_BLE_FLAG_IS_ADVERTISING) != 0)
    {
        // If not connected, then the device will be advertising. Hence stop the advertising.
        err_code = advertising_stop();
        VERIFY_SUCCESS(err_code);
    }

    // Stop the timer, disregard the result.
    (void)ble_conn_params_stop();
    return err_code;
}
#endif
