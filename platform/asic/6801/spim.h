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

#define SPIM_INT_ERROR_STATUS_MASK (0x1UL << 17)
#define SPIM_INT_ERROR_STATUS_CLEAR_BIT (1)

#define SPIM_INT_EVENT_STATUS_MASK (0x1UL << 16)
#define SPIM_INT_EVENT_STATUS_CLEAR_BIT (1)

#define SPIM_INT_ERROR_ENABLE_MASK (0x1UL << 1)
#define SPIM_INT_ERROR_DISABLE_BIT (0)
#define SPIM_INT_ERROR_ENABLE_BIT (1)

#define SPIM_INT_EVENT_ENABLE_MASK (0x1UL << 0)
#define SPIM_INT_EVENT_DISABLE_BIT (0)
#define SPIM_INT_EVENT_ENABLE_BIT (1)

#define SPIM_CTRL_ENABLE_MASK (0x1UL << 24)
#define SPIM_CTRL_DISABLE_BIT (0)
#define SPIM_CTRL_ENABLE_BIT (1)

#define SPIM_CTRL_CS_POLARITY_MASK (0x1UL << 10)
#define SPIM_CTRL_CS_POLARITY_HIGH_BIT (0)
#define SPIM_CTRL_CS_POLARITY_LOW_BIT (1)

#define SPIM_CTRL_CPOL_MASK (0x1UL << 1)
#define SPIM_CTRL_CPOL_EDGE_RISING_BIT (0)
#define SPIM_CTRL_CPOL_EDGE_FALLING_BIT (1)

#define SPIM_CTRL_CPHA_MASK (0x1UL << 0)
#define SPIM_CTRL_CPHA_PHASE_NO_SHIFT_BIT (0)
#define SPIM_CTRL_CPHA_PHASE_SHIFT_BIT (1)

#define SPIM_DMA_ENALBE_MASK (0x1UL << 24)
#define SPIM_DMA_DISABLE_BIT (0)
#define SPIM_DMA_ENABLE_BIT (1)

#define SPIM_DMA_BYTE_SWAP_RX_MASK (0x1UL << 19)
#define SPIM_DMA_BYTE_SWAP_RX_LSB_BIT (0)
#define SPIM_DMA_BYTE_SWAP_RX_MSB_BIT (1)

#define SPIM_DMA_BYTE_SWAP_TX_MASK (0x1UL << 18)
#define SPIM_DMA_BYTE_SWAP_TX_LSB_BIT (0)
#define SPIM_DMA_BYTE_SWAP_TX_MSB_BIT (1)

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
    E_SpimSupported index;
    int (*fpSpimXfer)(U_regSPI * p_spim);
} T_SpiMaster;

typedef struct
{
    UINT8 const *pTxBuffer;
    UINT8        bTxLength;
    UINT8       *pRxBuffer;
    UINT8        bRxLength;
}T_SpimTransfer;

int cc6801_SpimInit(T_SpiDevice *spi);

int cc6801_SpimWrite(T_SpiDevice const * const pSpiDev,
                           UINT8 const * pTxBuf,
                           UINT8         bTxBufLen);

int cc6801_SpimRead(T_SpiDevice const * const pSpiDev,
                          UINT8       * pRxBuf,
                          UINT8         bRxBufLen);

int cc6801_SpimWriteThenRead(T_SpiDevice const * const pSpiDev,
                                   UINT8 const * pTxBuf,
                                   UINT8         bTxBufLen,
                                   UINT8       * pRxBuf,
                                   UINT8         bRxBufLen);

#endif // _SPIM_H__
