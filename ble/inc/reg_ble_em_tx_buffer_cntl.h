#ifndef _REG_BLE_EM_TX_BUFFER_CNTL_H_
#define _REG_BLE_EM_TX_BUFFER_CNTL_H_

#include <stdint.h>
#include "_reg_ble_em_tx_buffer_cntl.h"
#include "compiler.h"
#include "arch.h"
#include "em_map.h"
#include "reg_access.h"

#define REG_BLE_EM_TX_BUFFER_CNTL_COUNT 1

#define REG_BLE_EM_TX_BUFFER_CNTL_DECODING_MASK 0x00000000

#define REG_BLE_EM_TX_BUFFER_CNTL_ADDR_GET(idx) (EM_BLE_TX_BUFFER_CNTL_OFFSET + (idx) * REG_BLE_EM_TX_BUFFER_CNTL_SIZE)

/**
 * @brief TXBUFCNTL register definition
 * <pre>
 *   Bits           Field Name   Reset Value
 *  -----   ------------------   -----------
 *  15:00            TXBUFCNTL   0x0
 * </pre>
 */
#define BLE_TXBUFCNTL_ADDR   (0x40006000 + EM_BLE_TX_BUFFER_CNTL_OFFSET)
#define BLE_TXBUFCNTL_INDEX  0x00000000
#define BLE_TXBUFCNTL_RESET  0x00000000
#define BLE_TXBUFCNTL_COUNT  19

__BLE_INLINE uint16_t ble_txbufcntl_get(int elt_idx, int reg_idx)
{
    ASSERT_ERR(reg_idx <= 18);
    return EM_BLE_RD(BLE_TXBUFCNTL_ADDR + elt_idx * REG_BLE_EM_TX_BUFFER_CNTL_SIZE + reg_idx * 2);
}

__BLE_INLINE void ble_txbufcntl_set(int elt_idx, int reg_idx, uint16_t value)
{
    ASSERT_ERR(reg_idx <= 18);
    EM_BLE_WR(BLE_TXBUFCNTL_ADDR + elt_idx * REG_BLE_EM_TX_BUFFER_CNTL_SIZE + reg_idx * 2, value);
}

// field definitions
#define BLE_TXBUFCNTL_MASK   ((uint16_t)0x0000FFFF)
#define BLE_TXBUFCNTL_LSB    0
#define BLE_TXBUFCNTL_WIDTH  ((uint16_t)0x00000010)

#define BLE_TXBUFCNTL_RST    0x0

__BLE_INLINE uint16_t ble_txbufcntl_getf(int elt_idx, int reg_idx)
{
    ASSERT_ERR(reg_idx <= 18);
    uint16_t localVal = EM_BLE_RD(BLE_TXBUFCNTL_ADDR + elt_idx * REG_BLE_EM_TX_BUFFER_CNTL_SIZE + reg_idx * 2);
    ASSERT_ERR((localVal & ~((uint16_t)0x0000FFFF)) == 0);
    return (localVal >> 0);
}

__BLE_INLINE void ble_txbufcntl_setf(int elt_idx, int reg_idx, uint16_t txbufcntl)
{
    ASSERT_ERR(reg_idx <= 18);
    ASSERT_ERR((((uint16_t)txbufcntl << 0) & ~((uint16_t)0x0000FFFF)) == 0);
    EM_BLE_WR(BLE_TXBUFCNTL_ADDR + elt_idx * REG_BLE_EM_TX_BUFFER_CNTL_SIZE + reg_idx * 2, (uint16_t)txbufcntl << 0);
}


#endif // _REG_BLE_EM_TX_BUFFER_CNTL_H_

