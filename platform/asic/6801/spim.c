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

volatile uint32_t SPI0_M_INTR = 0;
volatile uint32_t SPI1_M_INTR = 0;
volatile uint32_t SPI2_M_INTR = 0;

T_SpiMaster g_tSpim[SPIM_TOTAL_SUPPORTED] = {0};

void SPI0_M_IRQHandler(void)
{
    regSPI0->dw.spiInt |= (SPIM_INT_ERROR_STATUS_CLEAR_MASK | SPIM_INT_EVENT_STATUS_CLEAR_MASK);
    SPI0_M_INTR = 1;
}

void SPI1_M_IRQHandler(void)
{
    regSPI1->dw.spiInt |= (SPIM_INT_ERROR_STATUS_CLEAR_MASK | SPIM_INT_EVENT_STATUS_CLEAR_MASK);
    SPI1_M_INTR = 1;
}

void SPI2_M_IRQHandler(void)
{
    regSPI2->dw.spiInt |= (SPIM_INT_ERROR_STATUS_CLEAR_MASK | SPIM_INT_EVENT_STATUS_CLEAR_MASK);
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
                                    uint8_t const * pBuffer,
                                    uint8_t   bLength)
{
    pSpimBase->bf.dma_str_raddr = (uint32_t)pBuffer;
    pSpimBase->bf.total_wbyte = bLength - 1;
}

static void cc6801_SpimRxBufferSet(U_regSPI * pSpimBase,
                                    uint8_t const * pBuffer,
                                    uint8_t   bLength)
{
    pSpimBase->bf.dma_str_waddr = (uint32_t)pBuffer;
    pSpimBase->bf.total_rbyte = bLength - 1;
}

static void cc6801_SpimIntStatusClr(U_regSPI * pSpimBase)
{
    pSpimBase->dw.spiInt |= (SPIM_INT_ERROR_STATUS_CLEAR_MASK | SPIM_INT_EVENT_STATUS_CLEAR_MASK);
}

static int cc6801_SpimTransfer(uint8_t bBusNum,
                               T_SpimTransfer const * pXfer)
{
    U_regSPI *pSpimBase = g_tSpim[bBusNum].pReg;

    if (((uint32_t)pXfer->pTxBuffer & 0x3UL) ||
         ((uint32_t)pXfer->pRxBuffer & 0x3UL))
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

    return g_tSpim[bBusNum].fpSpimXfer(pSpimBase);
}

__STATIC_INLINE void cc6801_SpimConfig(U_regSPI * pSpimBase,
                                        T_SpiDevice *spi)
{
    uint32_t dwDmaCtrl = pSpimBase->dw.DmaCtrl;
    uint32_t dwSpiCtrl = pSpimBase->dw.spiCtrl;

    if (spi->wMode & SPI_LSB_FIRST)
    {
        dwDmaCtrl |= SPIM_DMA_BYTE_SWAP_TX_MSB_MASK;
        dwDmaCtrl |= SPIM_DMA_BYTE_SWAP_RX_MSB_MASK;
    }
    else
    {
        dwDmaCtrl &= ~SPIM_DMA_BYTE_SWAP_TX_MSB_MASK;
        dwDmaCtrl &= ~SPIM_DMA_BYTE_SWAP_RX_MSB_MASK;
    }

    if (spi->wMode & SPI_CPHA)
        dwSpiCtrl |= SPIM_CTRL_CPHA_PHASE_SHIFT_MASK;
    else
        dwSpiCtrl &= ~SPIM_CTRL_CPHA_PHASE_SHIFT_MASK;

    if (spi->wMode & SPI_CPOL)
        dwSpiCtrl |= SPIM_CTRL_CPOL_EDGE_FALLING_MASK;
    else
        dwSpiCtrl &= ~SPIM_CTRL_CPOL_EDGE_FALLING_MASK;

    if (spi->wMode & SPI_CS_HIGH)
        dwSpiCtrl &= ~SPIM_CTRL_CS_POLARITY_LOW_MASK;
    else
        dwSpiCtrl |= SPIM_CTRL_CS_POLARITY_LOW_MASK;

    pSpimBase->dw.spiCtrl = dwSpiCtrl;
    pSpimBase->dw.DmaCtrl = dwDmaCtrl;

    printf("setup mode %d, %s%s%s %u Hz max\n",
        (int) (spi->wMode & (SPI_CPOL | SPI_CPHA)),
        (spi->wMode & SPI_CS_HIGH) ? "cs_high, " : "",
        (spi->wMode & SPI_LSB_FIRST) ? "lsb, " : "",
        (spi->wMode & SPI_3WIRE) ? "3wire, " : "",
        spi->dwMaxSpeedHz);
}

int cc6801_SpimInit(T_SpiDevice *pSpiDev)
{
    U_regSPI *pSpimBase;
    uint8_t bIdx = pSpiDev->bBusNum;

    if (pSpiDev->bBusNum == SPIM_0)
    {
        g_tSpim[bIdx].pReg = (void *)regSPI0,
        g_tSpim[bIdx].fpSpimXfer = cc6801_Spim0Xfer;
    }
    else if (pSpiDev->bBusNum == SPIM_1)
    {
        g_tSpim[bIdx].pReg = (void *)regSPI1,
        g_tSpim[bIdx].fpSpimXfer = cc6801_Spim1Xfer;
    }
    else if (pSpiDev->bBusNum == SPIM_2)
    {
        g_tSpim[bIdx].pReg = (void *)regSPI2,
        g_tSpim[bIdx].fpSpimXfer = cc6801_Spim2Xfer;
    }
    else
    {
        printf("Not Support SPI%d\r\n", pSpiDev->bBusNum);
        return CC_ERROR_INVALID_PARAM;
    }

    pSpimBase = g_tSpim[bIdx].pReg;

    regCKGEN->bf.spi0ClkDiv = 2;
    regCKGEN->bf.spi1ClkDiv = 2;
    regCKGEN->bf.spi2ClkDiv = 2;

    cc6801_SpimConfig(g_tSpim[bIdx].pReg, pSpiDev);

    pSpimBase->bf.spi_m_en = SPIM_CTRL_ENABLE_BIT;

    pSpimBase->bf.error_int_en = SPIM_INT_ERROR_ENABLE_BIT;
    pSpimBase->bf.event_int_en = SPIM_INT_EVENT_ENABLE_BIT;

    return CC_SUCCESS;
}

int cc6801_SpimWrite(uint8_t         bBusNum,
                     uint8_t const * pTxBuf,
                     uint8_t         bTxBufLen)
{
    T_SpimTransfer xfer =
    {
        xfer.pTxBuffer = pTxBuf,
        xfer.bTxLength = bTxBufLen,
    };

    return cc6801_SpimTransfer(bBusNum, &xfer);
}

int cc6801_SpimRead(uint8_t   bBusNum,
                    uint8_t * pRxBuf,
                    uint8_t   bRxBufLen)
{
    T_SpimTransfer xfer =
    {
        xfer.pRxBuffer = pRxBuf,
        xfer.bRxLength = bRxBufLen,
    };

    return cc6801_SpimTransfer(bBusNum, &xfer);
}

int cc6801_SpimWriteThenRead(uint8_t         bBusNum,
                             uint8_t const * pTxBuf,
                             uint8_t         bTxBufLen,
                             uint8_t       * pRxBuf,
                             uint8_t         bRxBufLen)
{
    T_SpimTransfer xfer =
    {
        xfer.pTxBuffer = pTxBuf,
        xfer.bTxLength = bTxBufLen,
        xfer.pRxBuffer = pRxBuf,
        xfer.bRxLength = bRxBufLen,
    };

    return cc6801_SpimTransfer(bBusNum, &xfer);
}
