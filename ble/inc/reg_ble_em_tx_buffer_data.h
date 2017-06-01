#ifndef _REG_BLE_EM_TX_BUFFER_DATA_H_
#define _REG_BLE_EM_TX_BUFFER_DATA_H_

#include <stdint.h>
#include "_reg_ble_em_tx_buffer_data.h"
#include "compiler.h"
#include "arch.h"
#include "em_map.h"
#include "reg_access.h"

#define REG_BLE_EM_TX_BUFFER_DATA_COUNT 1

#define REG_BLE_EM_TX_BUFFER_DATA_DECODING_MASK 0x00000000

#define REG_BLE_EM_TX_BUFFER_DATA_ADDR_GET(idx) (EM_BLE_TX_BUFFER_DATA_OFFSET + (idx) * REG_BLE_EM_TX_BUFFER_DATA_SIZE)

/**
 * @brief TXBUF register definition
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  15:00                TXBUF   0x0
 * </pre>
 */
#define BLE_TXBUF_ADDR   (0x40006000 + EM_BLE_TX_BUFFER_DATA_OFFSET)
#define BLE_TXBUF_INDEX  0x00000000
#define BLE_TXBUF_RESET  0x00000000
#define BLE_TXBUF_COUNT  130

__BLE_INLINE uint16_t ble_txbuf_get(int elt_idx, int reg_idx)
{
    ASSERT_ERR(reg_idx <= 129);
    return EM_BLE_RD(BLE_TXBUF_ADDR + elt_idx * REG_BLE_EM_TX_BUFFER_DATA_SIZE + reg_idx * 2);
}

__BLE_INLINE void ble_txbuf_set(int elt_idx, int reg_idx, uint16_t value)
{
    ASSERT_ERR(reg_idx <= 129);
    EM_BLE_WR(BLE_TXBUF_ADDR + elt_idx * REG_BLE_EM_TX_BUFFER_DATA_SIZE + reg_idx * 2, value);
}

// field definitions
#define BLE_TXBUF_MASK   ((uint16_t)0x0000FFFF)
#define BLE_TXBUF_LSB    0
#define BLE_TXBUF_WIDTH  ((uint16_t)0x00000010)

#define BLE_TXBUF_RST    0x0

__BLE_INLINE uint16_t ble_txbuf_getf(int elt_idx, int reg_idx)
{
    ASSERT_ERR(reg_idx <= 129);
    uint16_t localVal = EM_BLE_RD(BLE_TXBUF_ADDR + elt_idx * REG_BLE_EM_TX_BUFFER_DATA_SIZE + reg_idx * 2);
    ASSERT_ERR((localVal & ~((uint16_t)0x0000FFFF)) == 0);
    return (localVal >> 0);
}

__BLE_INLINE void ble_txbuf_setf(int elt_idx, int reg_idx, uint16_t txbuf)
{
    ASSERT_ERR(reg_idx <= 129);
    ASSERT_ERR((((uint16_t)txbuf << 0) & ~((uint16_t)0x0000FFFF)) == 0);
    EM_BLE_WR(BLE_TXBUF_ADDR + elt_idx * REG_BLE_EM_TX_BUFFER_DATA_SIZE + reg_idx * 2, (uint16_t)txbuf << 0);
}


#endif // _REG_BLE_EM_TX_BUFFER_DATA_H_

