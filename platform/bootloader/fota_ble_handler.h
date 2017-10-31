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

/**@file
 *
 * @defgroup nrf_ble_dfu DFU BLE Service
 * @{
 * @ingroup  sdk_nrf_bootloader
 * @brief    Device Firmware Update (DFU) transport layer for <em>Bluetooth</em> low energy.
 *
 * @details  The Device Firmware Update (DFU) Service is a GATT-based service that can be used for
 *           performing firmware updates over BLE. Note that this implementation uses
 *           vendor-specific UUIDs for the service and characteristics and is intended to demonstrate
 *           firmware updates over BLE. See @ref lib_dfu_transport_ble "DFU Transport: BLE" for more information on the service and the profile.
 */




#ifndef FOTA_BLE_HANDLER_H__
#define FOTA_BLE_HANDLER_H__

#include <stdint.h>
#include "otat_task.h"


#ifdef __cplusplus
extern "C" {
#endif



/**@brief   BLE DFU opcodes.
 *
 * @details These types of opcodes are used in control point access.
 */
typedef enum
{
    BLE_DFU_OP_CODE_CREATE_OBJECT               = 0x01,                                 /**< Value of the opcode field for a 'Create object' request. */
    BLE_DFU_OP_CODE_SET_RECEIPT_NOTIF           = 0x02,                                 /**< Value of the opcode field for a 'Set Packet Receipt Notification' request. */
    BLE_DFU_OP_CODE_CALCULATE_CRC               = 0x03,                                 /**< Value of the opcode field for a 'Calculating checksum' request. */
    BLE_DFU_OP_CODE_EXECUTE_OBJECT              = 0x04,                                 /**< Value of the opcode field for an 'Initialize DFU parameters' request. */
    BLE_DFU_OP_CODE_SELECT_OBJECT               = 0x06,                                 /**< Value of the opcode field for a 'Select object' request. */
    BLE_DFU_OP_CODE_RESPONSE                    = 0x60                                  /**< Value of the opcode field for a response.*/
} ble_dfu_op_code_t;


void fota_on_ctrl_pt_write(struct otat_packet_send_cmd const *param);
void fota_on_write(struct otat_packet_send_cmd const *param);



#ifdef __cplusplus
}
#endif

#endif // NRF_BLE_DFU_H__


