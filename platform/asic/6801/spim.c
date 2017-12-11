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

#include "drvi_clock.h"
#include "drvi_spi.h"
#include "spim.h"
#include "tracer.h"

#define SPI_CLKDIV_MAX    0x1F

extern E_ClockSupported g_dwCurrentClock;
extern const T_ClockConfig g_aClockTable[CLOCK_TOTAL_SUPPORTED];

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

//static void cc6801_SpimTxBufferSet(U_regSPI * pSpimBase,
//                                    uint8_t const * pBuffer,
//                                    uint8_t   bLength)
//{
//    pSpimBase->bf.dma_str_raddr = (uint32_t)pBuffer;
//    pSpimBase->bf.total_wbyte = bLength - 1;
//}

//static void cc6801_SpimRxBufferSet(U_regSPI * pSpimBase,
//                                    uint8_t const * pBuffer,
//                                    uint8_t   bLength)
//{
//    pSpimBase->bf.dma_str_waddr = (uint32_t)pBuffer;
//    pSpimBase->bf.total_rbyte = bLength - 1;
//}

static void cc6801_SpimIntStatusClr(U_regSPI * pSpimBase)
{
    pSpimBase->dw.spiInt |= (SPIM_INT_ERROR_STATUS_CLEAR_MASK | SPIM_INT_EVENT_STATUS_CLEAR_MASK);
}

static int cc6801_SpimTransfer(uint8_t bBusNum,
                               T_SpimTransfer const * pXfer)
{
    U_regSPI *pSpimBase = g_tSpim[bBusNum].pReg;
    uint32_t dwSpiDmaCtrl = 0;
    uint32_t dwDmaWrAddr = 0;
    uint32_t dwDmaRdAddr = 0;
    uint8_t bDmaWrByte = 0;
    uint8_t bDmaRdByte = 0;

    if (((uint32_t)pXfer->pTxBuffer & 0x3UL) ||
         ((uint32_t)pXfer->pRxBuffer & 0x3UL))
        return CC_ERROR_INVALID_ADDR;

    dwSpiDmaCtrl = pSpimBase->dw.DmaCtrl;

    if ((pXfer->pTxBuffer) && (pXfer->pRxBuffer))
    {
        bDmaRdByte = pXfer->bTxLength - 1;
        bDmaWrByte = pXfer->bRxLength - 1;
        dwSpiDmaCtrl &= ~(SPIM_DMA_OP_MODE_MASK | SPIM_DMA_RDSIZE_MASK | SPIM_DMA_WRSIZE_MASK);
        dwSpiDmaCtrl |= (bDmaRdByte << SPIM_DMA_RDSIZE_SHIFT |
                         bDmaWrByte << SPIM_DMA_WRSIZE_SHIFT |
                         SPIM_OP_MODE_WRITE_THEN_READ << SPIM_DMA_OP_MODE_SHIFT);
        dwDmaRdAddr = (uint32_t)pXfer->pTxBuffer;
        dwDmaWrAddr = (uint32_t)pXfer->pRxBuffer;
    }
    else if (pXfer->pTxBuffer)
    {
        bDmaRdByte = pXfer->bTxLength - 1;
        dwSpiDmaCtrl &= ~(SPIM_DMA_OP_MODE_MASK | SPIM_DMA_RDSIZE_MASK | SPIM_DMA_WRSIZE_MASK);
        dwSpiDmaCtrl |= (bDmaRdByte << SPIM_DMA_RDSIZE_SHIFT |
                         SPIM_OP_MODE_WRITE << SPIM_DMA_OP_MODE_SHIFT);
        dwDmaRdAddr = (uint32_t)pXfer->pTxBuffer;
    }
    else if (pXfer->pRxBuffer)
    {
        bDmaWrByte = pXfer->bRxLength - 1;
        dwSpiDmaCtrl &= ~(SPIM_DMA_OP_MODE_MASK | SPIM_DMA_RDSIZE_MASK | SPIM_DMA_WRSIZE_MASK);
        dwSpiDmaCtrl |= (bDmaWrByte << SPIM_DMA_WRSIZE_SHIFT |
                         SPIM_OP_MODE_READ << SPIM_DMA_OP_MODE_SHIFT);
        dwDmaWrAddr = (uint32_t)pXfer->pRxBuffer;
    }
    else
        return CC_ERROR_INVALID_ADDR;

    pSpimBase->dw.wAddr = dwDmaWrAddr & SPIM_DMA_RWADDR_MASK;
    pSpimBase->dw.rAddr = dwDmaRdAddr & SPIM_DMA_RWADDR_MASK;
    pSpimBase->dw.DmaCtrl = dwSpiDmaCtrl;

    cc6801_SpimIntStatusClr(pSpimBase);

    return g_tSpim[bBusNum].fpSpimXfer(pSpimBase);
}

static void cc6801_SpimConfig(U_regSPI * pSpimBase,
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

    TracerInfo("setup mode %d, %s%s%s %u Hz\n",
        (int) (spi->wMode & (SPI_CPOL | SPI_CPHA)),
        (spi->wMode & SPI_CS_HIGH) ? "cs_high, " : "",
        (spi->wMode & SPI_LSB_FIRST) ? "lsb, " : "",
        (spi->wMode & SPI_3WIRE) ? "3wire, " : "",
        g_tSpim[spi->bBusNum].dwClkHz);
}

int cc6801_SpimInit(T_SpiDevice *pSpiDev)
{
    U_regSPI *pSpimBase;
    uint32_t dwSysClkHz = 0;
    uint32_t dwSpiMaxClkHz = 0;
    uint8_t bClkDiv = 1;

    uint8_t bIdx = pSpiDev->bBusNum;

    dwSysClkHz = g_aClockTable[g_dwCurrentClock].dwMHz;
    dwSpiMaxClkHz = dwSysClkHz/2;
    bClkDiv = dwSpiMaxClkHz/pSpiDev->dwMaxSpeedHz;

    if(bClkDiv > SPI_CLKDIV_MAX)
        bClkDiv = SPI_CLKDIV_MAX;
    else if (bClkDiv <= 0)
        bClkDiv = 1;

    g_tSpim[bIdx].dwClkHz = dwSpiMaxClkHz/bClkDiv;

    if (pSpiDev->bBusNum == SPIM_0)
    {
        regCKGEN->bf.spi0ClkDiv = bClkDiv;
        g_tSpim[bIdx].pReg = (void *)regSPI0,
        g_tSpim[bIdx].fpSpimXfer = cc6801_Spim0Xfer;
    }
    else if (pSpiDev->bBusNum == SPIM_1)
    {
        regCKGEN->bf.spi1ClkDiv = bClkDiv;
        g_tSpim[bIdx].pReg = (void *)regSPI1,
        g_tSpim[bIdx].fpSpimXfer = cc6801_Spim1Xfer;
    }
    else if (pSpiDev->bBusNum == SPIM_2)
    {
        regCKGEN->bf.spi2ClkDiv = bClkDiv;
        g_tSpim[bIdx].pReg = (void *)regSPI2,
        g_tSpim[bIdx].fpSpimXfer = cc6801_Spim2Xfer;
    }
    else
    {
        TracerErr("Not Support SPI%d\n", pSpiDev->bBusNum);
        return CC_ERROR_INVALID_PARAM;
    }

    cc6801_SpimConfig(g_tSpim[bIdx].pReg, pSpiDev);

    pSpimBase = g_tSpim[bIdx].pReg;
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
