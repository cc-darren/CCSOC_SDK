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

/******************************************************************************
*  Filename:
*  ---------
*  spim.c
*
*  Project:
*  --------
*  cc6801
*
*  Description:
*  ------------
*  This is SPI master driver
*
*  Author:
*  -------
*  PATTAYA
*
*===========================================================================/
*  20170203 PAT initial version
******************************************************************************/

#include <stdio.h>

#include "drvi_spi.h"
#include "spim.h"

volatile UINT32 SPI0_M_INTR = 0;
volatile UINT32 SPI1_M_INTR = 0;
volatile UINT32 SPI2_M_INTR = 0;

void SPI0_M_IRQHandler(void)
{
    regSPI0->dw.spiInt |= (SPIM_INT_ERROR_STATUS_MASK | SPIM_INT_EVENT_STATUS_MASK);
    SPI0_M_INTR = 1;
}

void SPI1_M_IRQHandler(void)
{
    regSPI1->dw.spiInt |= (SPIM_INT_ERROR_STATUS_MASK | SPIM_INT_EVENT_STATUS_MASK);
    SPI1_M_INTR = 1;
}

void SPI2_M_IRQHandler(void)
{
    regSPI2->dw.spiInt |= (SPIM_INT_ERROR_STATUS_MASK | SPIM_INT_EVENT_STATUS_MASK);
    SPI2_M_INTR = 1;
}


static int cc6801_Spim0Xfer(U_regSPI * pSpim0Base)
{
    NVIC_EnableIRQ(SPI0_M_IRQn);

    pSpim0Base->bf.spi_m_dma_en = SPIM_DMA_ENABLE_BIT;

    while(!SPI0_M_INTR);
    SPI0_M_INTR = 0;

    NVIC_DisableIRQ(SPI0_M_IRQn);
    return CC_SUCCESS;
}

static int cc6801_Spim1Xfer(U_regSPI * pSpim1Base)
{
    NVIC_EnableIRQ(SPI1_M_IRQn);

    pSpim1Base->bf.spi_m_dma_en = SPIM_DMA_ENABLE_BIT;

    while(!SPI1_M_INTR);
    SPI1_M_INTR = 0;

    NVIC_DisableIRQ(SPI1_M_IRQn);
    return CC_SUCCESS;
}

static int cc6801_Spim2Xfer(U_regSPI * pSpim2Base)
{
    NVIC_EnableIRQ(SPI2_M_IRQn);

    pSpim2Base->bf.spi_m_dma_en = SPIM_DMA_ENABLE_BIT;

    while(!SPI2_M_INTR);
    SPI2_M_INTR = 0;

    NVIC_DisableIRQ(SPI2_M_IRQn);
    return CC_SUCCESS;
}

static void cc6801_SpimTxBufferSet(U_regSPI * pSpimBase,
                                    UINT8 const * pBuffer,
                                    UINT8   bLength)
{
    pSpimBase->bf.dma_str_raddr = (UINT32)pBuffer;
    pSpimBase->bf.total_wbyte = bLength - 1;
}

static void cc6801_SpimRxBufferSet(U_regSPI * pSpimBase,
                                    UINT8 const * pBuffer,
                                    UINT8   bLength)
{
    pSpimBase->bf.dma_str_waddr = (UINT32)pBuffer;
    pSpimBase->bf.total_rbyte = bLength - 1;
}

static void cc6801_SpimIntStatusClr(U_regSPI * pSpimBase)
{
    pSpimBase->dw.spiInt |= (SPIM_INT_ERROR_STATUS_MASK | SPIM_INT_EVENT_STATUS_MASK);
}

static T_SpiMaster cc6801_SpiMasterGet(UINT8 bSpimId)
{
    T_SpiMaster master;

    switch (bSpimId)
    {
        case SPIM_0:
            master.pReg = (void *)regSPI0,
            master.index = SPIM_0,
            master.fpSpimXfer = cc6801_Spim0Xfer;
            break;
        case SPIM_1:
            master.pReg = (void *)regSPI1,
            master.index = SPIM_1,
            master.fpSpimXfer = cc6801_Spim1Xfer;
            break;
        case SPIM_2:
            master.pReg = (void *)regSPI2,
            master.index = SPIM_2,
            master.fpSpimXfer = cc6801_Spim2Xfer;
            break;
        default:
            break;
    }

    return master;
}

static int cc6801_SpimTransfer(T_SpiMaster const * const pSpim,
                                T_SpimTransfer const * pXfer)
{
    U_regSPI *pSpimBase = pSpim->pReg;

    if (((UINT32)pXfer->pTxBuffer & 0x3UL) ||
         ((UINT32)pXfer->pRxBuffer & 0x3UL))
        return CC_ERROR_INVALID_ADDR;

    if ((pXfer->pTxBuffer) && (pXfer->pRxBuffer))
    {
        pSpimBase->bf.op_mode = SPIM_OP_MODE_WRITE_THEN_READ;
        cc6801_SpimTxBufferSet(pSpimBase, pXfer->pTxBuffer, pXfer->bTxLength);
        cc6801_SpimRxBufferSet(pSpimBase, pXfer->pRxBuffer, pXfer->bRxLength);
    }
    else if (pXfer->pTxBuffer)
    {
        pSpimBase->bf.op_mode = SPIM_OP_MODE_WRITE;
        cc6801_SpimTxBufferSet(pSpimBase, pXfer->pTxBuffer, pXfer->bTxLength);
    }
    else if (pXfer->pRxBuffer)
    {
        pSpimBase->bf.op_mode = SPIM_OP_MODE_READ;
        cc6801_SpimRxBufferSet(pSpimBase, pXfer->pRxBuffer, pXfer->bRxLength);
    }
    else
        return CC_ERROR_INVALID_ADDR;

    cc6801_SpimIntStatusClr(pSpimBase);

    return pSpim->fpSpimXfer(pSpimBase);
}

__STATIC_INLINE void cc6801_SpimConfig(U_regSPI * pSpimBase,
                                        T_SpiDevice *spi)
{
    UINT32 dwDmaCtrl = pSpimBase->dw.DmaCtrl;
    UINT32 dwSpiCtrl = pSpimBase->dw.spiCtrl;

    if (spi->wMode & DRVI_SPI_LSB_FIRST)
    {
        dwDmaCtrl &= ~SPIM_DMA_BYTE_SWAP_TX_MASK;
        dwDmaCtrl &= ~SPIM_DMA_BYTE_SWAP_RX_MASK;
    }
    else
    {
        dwDmaCtrl |= SPIM_DMA_BYTE_SWAP_TX_MASK;
        dwDmaCtrl |= SPIM_DMA_BYTE_SWAP_RX_MASK;
    }

    if (spi->wMode & DRVI_SPI_CPHA)
        dwSpiCtrl |= SPIM_CTRL_CPHA_MASK;
    else
        dwSpiCtrl &= ~SPIM_CTRL_CPHA_MASK;

    if (spi->wMode & DRVI_SPI_CPOL)
        dwSpiCtrl |= SPIM_CTRL_CPOL_MASK;
    else
        dwSpiCtrl &= ~SPIM_CTRL_CPOL_MASK;

    if (spi->wMode & DRVI_SPI_CS_HIGH)
        dwSpiCtrl &= ~SPIM_CTRL_CS_POLARITY_MASK;
    else
        dwSpiCtrl |= SPIM_CTRL_CS_POLARITY_MASK;

    pSpimBase->dw.spiCtrl = dwSpiCtrl;
    pSpimBase->dw.DmaCtrl = dwDmaCtrl;

    printf("setup mode %d, %s%s%s %lu Hz max\n",
        (int) (spi->wMode & (DRVI_SPI_CPOL | DRVI_SPI_CPHA)),
        (spi->wMode & DRVI_SPI_CS_HIGH) ? "cs_high, " : "",
        (spi->wMode & DRVI_SPI_LSB_FIRST) ? "lsb, " : "",
        (spi->wMode & DRVI_SPI_3WIRE) ? "3wire, " : "",
        spi->dwMaxSpeedHz);
}

int cc6801_SpimInit(T_SpiDevice *pSpiDev)
{
    T_SpiMaster master = cc6801_SpiMasterGet(pSpiDev->bBusNum);
    U_regSPI *pSpimBase = master.pReg;

    if (master.index > SPIM_TOTAL_SUPPORTED)
    {
        printf("Not Support SPI%d\r\n", master.index);
        return CC_ERROR_INVALID_PARAM;
    }

    regCKGEN->bf.spi0ClkDiv = 2;
    regCKGEN->bf.spi1ClkDiv = 2;
    regCKGEN->bf.spi2ClkDiv = 2;

    cc6801_SpimConfig(pSpimBase, pSpiDev);

    pSpimBase->bf.spi_m_en = SPIM_CTRL_ENABLE_BIT;

    pSpimBase->bf.error_int_en = SPIM_INT_ERROR_ENABLE_BIT;
    pSpimBase->bf.event_int_en = SPIM_INT_EVENT_ENABLE_BIT;

    return CC_SUCCESS;
}

int cc6801_SpimWrite(T_SpiDevice const * const pSpiDev,
                           UINT8 const * pTxBuf,
                           UINT8         bTxBufLen)
{
    T_SpiMaster master = cc6801_SpiMasterGet(pSpiDev->bBusNum);

    T_SpimTransfer xfer =
    {
        xfer.pTxBuffer = pTxBuf,
        xfer.bTxLength = bTxBufLen,
    };

    return cc6801_SpimTransfer(&master, &xfer);
}

int cc6801_SpimRead(T_SpiDevice const * const pSpiDev,
                          UINT8       * pRxBuf,
                          UINT8         bRxBufLen)
{
    T_SpiMaster master = cc6801_SpiMasterGet(pSpiDev->bBusNum);

    T_SpimTransfer xfer =
    {
        xfer.pRxBuffer = pRxBuf,
        xfer.bRxLength = bRxBufLen,
    };

    return cc6801_SpimTransfer(&master, &xfer);
}

int cc6801_SpimWriteThenRead(T_SpiDevice const * const pSpiDev,
                                   UINT8 const * pTxBuf,
                                   UINT8         bTxBufLen,
                                   UINT8       * pRxBuf,
                                   UINT8         bRxBufLen)
{
    T_SpiMaster master = cc6801_SpiMasterGet(pSpiDev->bBusNum);

    T_SpimTransfer xfer =
    {
        xfer.pTxBuffer = pTxBuf,
        xfer.bTxLength = bTxBufLen,
        xfer.pRxBuffer = pRxBuf,
        xfer.bRxLength = bRxBufLen,
    };

    return cc6801_SpimTransfer(&master, &xfer);
}
