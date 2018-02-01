/* Copyright (c) 2018 Cloudchip, Inc. All Rights Reserved.
 *
 * The information contained herein is property of Cloudchip, Inc.
 * Terms and conditions of usage are described in detail in CLOUDCHIP
 * STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information.
 * NO WARRANTY of ANY KIND is provided. This heading must NOT be removed 
 * from the file.
 */

#ifndef _SPIM_H__
#define _SPIM_H__

#include "global.h"
#include "cc6801_reg.h"

#define SPIM_INT_EVENT_DISABLE_BIT                     (0)
#define SPIM_INT_EVENT_DISABLE_MASK                    (SPIM_INT_EVENT_DISABLE_BIT << 0)
#define SPIM_INT_EVENT_ENABLE_BIT                      (1)
#define SPIM_INT_EVENT_ENABLE_MASK                     (SPIM_INT_EVENT_ENABLE_BIT << 0)

#define SPIM_INT_ERROR_DISABLE_BIT                     (0)
#define SPIM_INT_ERROR_DISABLE_MASK                    (SPIM_INT_ERROR_DISABLE_BIT << 1)
#define SPIM_INT_ERROR_ENABLE_BIT                      (1)
#define SPIM_INT_ERROR_ENABLE_MASK                     (SPIM_INT_ERROR_ENABLE_BIT << 1)

#define SPIM_INT_EVENT_STATUS_CLEAR_BIT                (1)
#define SPIM_INT_EVENT_STATUS_CLEAR_MASK               (SPIM_INT_EVENT_STATUS_CLEAR_BIT << 16)

#define SPIM_INT_ERROR_STATUS_CLEAR_BIT                (1)
#define SPIM_INT_ERROR_STATUS_CLEAR_MASK               (SPIM_INT_ERROR_STATUS_CLEAR_BIT << 17)

#define SPIM_CTRL_CPHA_PHASE_NO_SHIFT_BIT              (0)
#define SPIM_CTRL_CPHA_PHASE_NO_SHIFT_MASK             (SPIM_CTRL_CPHA_PHASE_NO_SHIFT_BIT << 0)
#define SPIM_CTRL_CPHA_PHASE_SHIFT_BIT                 (1)
#define SPIM_CTRL_CPHA_PHASE_SHIFT_MASK                (SPIM_CTRL_CPHA_PHASE_SHIFT_BIT << 0)

#define SPIM_CTRL_CPOL_EDGE_RISING_BIT                 (0)
#define SPIM_CTRL_CPOL_EDGE_RISING_MASK                (SPIM_CTRL_CPOL_EDGE_RISING_BIT << 1)
#define SPIM_CTRL_CPOL_EDGE_FALLING_BIT                (1)
#define SPIM_CTRL_CPOL_EDGE_FALLING_MASK               (SPIM_CTRL_CPOL_EDGE_FALLING_BIT << 1)

#define SPIM_CTRL_CS_POLARITY_HIGH_BIT                 (0)
#define SPIM_CTRL_CS_POLARITY_HIGH_MASK                (SPIM_CTRL_CS_POLARITY_HIGH_BIT << 10)
#define SPIM_CTRL_CS_POLARITY_LOW_BIT                  (1)
#define SPIM_CTRL_CS_POLARITY_LOW_MASK                 (SPIM_CTRL_CS_POLARITY_LOW_BIT << 10)

#define SPIM_CTRL_DISABLE_BIT                          (0)
#define SPIM_CTRL_DISABLE_MASK                         (SPIM_CTRL_DISABLE_BIT << 24)
#define SPIM_CTRL_ENABLE_BIT                           (1)
#define SPIM_CTRL_ENABLE_MASK                          (SPIM_CTRL_ENABLE_BIT << 24)

#define SPIM_DMA_RDSIZE_SHIFT                          (0)
#define SPIM_DMA_RDSIZE_BIT                            (255)
#define SPIM_DMA_RDSIZE_MASK                           (SPIM_DMA_RDSIZE_BIT << SPIM_DMA_RDSIZE_SHIFT)

#define SPIM_DMA_WRSIZE_SHIFT                          (8)
#define SPIM_DMA_WRSIZE_BIT                            (255)
#define SPIM_DMA_WRSIZE_MASK                           (SPIM_DMA_WRSIZE_BIT << SPIM_DMA_WRSIZE_SHIFT)

#define SPIM_DMA_OP_MODE_SHIFT                         (16)
#define SPIM_DMA_OP_MODE_BIT                           (3)
#define SPIM_DMA_OP_MODE_MASK                          (SPIM_DMA_OP_MODE_BIT << SPIM_DMA_OP_MODE_SHIFT)

#define SPIM_DMA_BYTE_SWAP_TX_LSB_BIT                  (0)
#define SPIM_DMA_BYTE_SWAP_TX_LSB_MASK                 (SPIM_DMA_BYTE_SWAP_TX_LSB_BITL << 18)
#define SPIM_DMA_BYTE_SWAP_TX_MSB_BIT                  (1)
#define SPIM_DMA_BYTE_SWAP_TX_MSB_MASK                 (SPIM_DMA_BYTE_SWAP_TX_MSB_BIT << 18)

#define SPIM_DMA_BYTE_SWAP_RX_LSB_BIT                  (0)
#define SPIM_DMA_BYTE_SWAP_RX_LSB_MASK                 (SPIM_DMA_BYTE_SWAP_RX_LSB_BIT << 19)
#define SPIM_DMA_BYTE_SWAP_RX_MSB_BIT                  (1)
#define SPIM_DMA_BYTE_SWAP_RX_MSB_MASK                 (SPIM_DMA_BYTE_SWAP_RX_MSB_BIT << 19)

#define SPIM_DMA_DISABLE_BIT                           (0)
#define SPIM_DMA_DISABLE_MASK                          (SPIM_DMA_DISABLE_BIT << 24)
#define SPIM_DMA_ENABLE_BIT                            (1)
#define SPIM_DMA_ENALBE_MASK                           (SPIM_DMA_ENABLE_BIT << 24)

#define SPIM_DMA_RWADDR_BIT                            (0x1FFFF)
#define SPIM_DMA_RWADDR_MASK                           (SPIM_DMA_RWADDR_BIT << 0)

typedef struct S_SpiDevice T_SpiDevice;

typedef enum
{
    SPIM_0 = 0,
    SPIM_1,
    SPIM_2,
    SPIM_TOTAL_SUPPORTED
}E_SpimSupported;

typedef enum
{
    SPIM_OP_MODE_WRITE,
    SPIM_OP_MODE_READ,
    SPIM_OP_MODE_WRITE_THEN_READ
} E_SpimOpMode;

typedef struct
{
    void *pReg;
    uint32_t dwClkHz;
    int (*fpSpimXfer)(U_regSPI * p_spim);
} T_SpiMaster;

typedef struct
{
    uint8_t const *pTxBuffer;
    uint8_t        bTxLength;
    uint8_t       *pRxBuffer;
    uint8_t        bRxLength;
}T_SpimTransfer;

int cc6801_SpimInit(T_SpiDevice *spi);

int cc6801_SpimWrite(uint8_t         bBusNum,
                     uint8_t const * pTxBuf,
                     uint8_t         bTxBufLen);

int cc6801_SpimRead(uint8_t   bBusNum,
                    uint8_t * pRxBuf,
                    uint8_t   bRxBufLen);

int cc6801_SpimWriteThenRead(uint8_t         bBusNum,
                             uint8_t const * pTxBuf,
                             uint8_t         bTxBufLen,
                             uint8_t       * pRxBuf,
                             uint8_t         bRxBufLen);

#endif // _SPIM_H__
