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
//#define SPI_9BIT_ENABLE 1

#include <stdio.h>
#include <string.h>

#include "drvi_clock.h"
#include "drvi_spi.h"
#include "spim.h"

#ifdef FREERTOS
#include "FreeRTOS.h"
#include "semphr.h"
#if defined(SEMAPHORE_H)
    extern SemaphoreHandle_t g_xSemaphoreSpim0;
    extern SemaphoreHandle_t g_xSemaphoreSpim1;
    extern SemaphoreHandle_t g_xSemaphoreSpim2;
#endif
#endif

static int cc6801_Spim0Xfer(S_regSPI * pSpim0Base);
static int cc6801_Spim1Xfer(S_regSPI * pSpim1Base);
static int cc6801_Spim2Xfer(S_regSPI * pSpim2Base);
static int cc6801_Spim3Xfer(S_regSPI * pSpim3Base);


extern E_ClockSupported g_dwCurrentClock;
extern const T_ClockConfig g_aClockTable[CLOCK_TOTAL_SUPPORTED];

volatile uint32_t SPI0_M_INTR = 0;
volatile uint32_t SPI1_M_INTR = 0;
volatile uint32_t SPI2_M_INTR = 0;
volatile uint32_t SPI3_M_INTR = 0;

#if defined(SPI_9BIT_ENABLE) && SPI_9BIT_ENABLE
struct S_Spi9BitCmd
{
    uint8_t bit9;
    uint8_t bit8_0;
};

static uint8_t g_baSpi9BitBuf[32] = {0};
#endif

T_SpiMaster g_tSpim[SPIM_TOTAL_SUPPORTED] =
{
    {regSPI0, 0, cc6801_Spim0Xfer},
    {regSPI1, 0, cc6801_Spim1Xfer},
    {regSPI2, 0, cc6801_Spim2Xfer},
    {regSPI3, 0, cc6801_Spim3Xfer},
};


static void cc6801_SpimIntStatusClr(S_regSPI * pSpimBase)
{
    pSpimBase->spiInt |= (SPIM_INT_ERROR_STATUS_CLEAR_MASK | SPIM_INT_EVENT_STATUS_CLEAR_MASK);
}

void SPI0_M_IRQHandler(void)
{
    cc6801_SpimIntStatusClr(regSPI0);
    SPI0_M_INTR = 1;

    #if defined(SEMAPHORE_H)
    {
        BaseType_t xHigherPriorityTaskWoken;
        xHigherPriorityTaskWoken = pdFALSE;

        if( g_xSemaphoreSpim0 != NULL)
        {
            xSemaphoreGiveFromISR( g_xSemaphoreSpim0, &xHigherPriorityTaskWoken );
        }
        portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
    }
    #endif
}

void SPI1_M_IRQHandler(void)
{
    cc6801_SpimIntStatusClr(regSPI1);
    SPI1_M_INTR = 1;

    #if defined(SEMAPHORE_H)
    {
        BaseType_t xHigherPriorityTaskWoken;
        xHigherPriorityTaskWoken = pdFALSE;

        if( g_xSemaphoreSpim1 != NULL)
        {
            xSemaphoreGiveFromISR( g_xSemaphoreSpim1, &xHigherPriorityTaskWoken );
        }
        portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
    }
   #endif
}

void SPI2_M_IRQHandler(void)
{
    cc6801_SpimIntStatusClr(regSPI2);
    SPI2_M_INTR = 1;
    
#if(defined SPI2_INUSE) && (SPI2_INUSE)
    #if defined(SEMAPHORE_H)
    {
        BaseType_t xHigherPriorityTaskWoken;
        xHigherPriorityTaskWoken = pdFALSE;

        if( g_xSemaphoreSpim2 != NULL)
        {
            xSemaphoreGiveFromISR( g_xSemaphoreSpim2, &xHigherPriorityTaskWoken );
        }
        portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
    }
    #endif
#endif    
}

void SPI3_M_IRQHandler(void)
{
    cc6801_SpimIntStatusClr(regSPI3);
    SPI3_M_INTR = 1;
}
static int cc6801_Spim0Xfer(S_regSPI * pSpim0Base)
{
    NVIC_EnableIRQ(SPI0_M_IRQn);

    pSpim0Base->dmaCtrl |= SPIM_DMA_ENALBE_MASK;

    #if defined(SEMAPHORE_H)
    {
        BaseType_t xResult = xSemaphoreTake( g_xSemaphoreSpim0, portMAX_DELAY );
    }
    #endif

    while(!SPI0_M_INTR);
    SPI0_M_INTR = 0;

    NVIC_DisableIRQ(SPI0_M_IRQn);
    return CC_SUCCESS;
}

static int cc6801_Spim1Xfer(S_regSPI * pSpim1Base)
{
    NVIC_EnableIRQ(SPI1_M_IRQn);

    pSpim1Base->dmaCtrl |= SPIM_DMA_ENALBE_MASK;

    #if defined(SEMAPHORE_H)
    {
        BaseType_t xResult = xSemaphoreTake( g_xSemaphoreSpim1, portMAX_DELAY );
    }
    #endif

    while(!SPI1_M_INTR);
    SPI1_M_INTR = 0;

    NVIC_DisableIRQ(SPI1_M_IRQn);
    return CC_SUCCESS;
}

static int cc6801_Spim2Xfer(S_regSPI * pSpim2Base)
{
    NVIC_EnableIRQ(SPI2_M_IRQn);

    pSpim2Base->dmaCtrl |= SPIM_DMA_ENALBE_MASK;

    #if(defined SPI2_INUSE) && (SPI2_INUSE)
    #if defined(SEMAPHORE_H)
    {
        BaseType_t xResult = xSemaphoreTake( g_xSemaphoreSpim2, portMAX_DELAY );
    }
    #endif
    #endif
    
    while(!SPI2_M_INTR);
    SPI2_M_INTR = 0;

    NVIC_DisableIRQ(SPI2_M_IRQn);
    return CC_SUCCESS;
}

static int cc6801_Spim3Xfer(S_regSPI * pSpim3Base)
{
    NVIC_EnableIRQ(SPI3_M_IRQn);

    pSpim3Base->dmaCtrl |= SPIM_DMA_ENALBE_MASK;

    while(!SPI3_M_INTR);
    SPI3_M_INTR = 0;

    NVIC_DisableIRQ(SPI3_M_IRQn);
    return CC_SUCCESS;
}

static int cc6801_SpimTransfer(uint8_t bBusNum,
                               T_SpimTransfer const * pXfer)
{
    S_regSPI *pSpimBase = g_tSpim[bBusNum].pReg;
    uint32_t dwSpiDmaCtrl = 0;
    uint32_t dwDmaWrAddr = 0;
    uint32_t dwDmaRdAddr = 0;
    uint8_t bDmaWrByte = 0;
    uint8_t bDmaRdByte = 0;

    if (((uint32_t)pXfer->pTxBuffer & 0x3UL) ||
         ((uint32_t)pXfer->pRxBuffer & 0x3UL))
        return CC_ERROR_INVALID_ADDR;

    dwSpiDmaCtrl = pSpimBase->dmaCtrl;

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

    pSpimBase->wAddr = dwDmaWrAddr & SPIM_DMA_RWADDR_MASK;
    pSpimBase->rAddr = dwDmaRdAddr & SPIM_DMA_RWADDR_MASK;
    pSpimBase->dmaCtrl = dwSpiDmaCtrl;

    cc6801_SpimIntStatusClr(pSpimBase);

    return g_tSpim[bBusNum].fpSpimXfer(pSpimBase);
}

static void cc6801_SpimConfig(S_regSPI * pSpimBase,
                                        T_SpiDevice *spi)
{
    uint32_t dwDmaCtrl = pSpimBase->dmaCtrl;
    uint32_t dwSpiCtrl = pSpimBase->spiCtrl;

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

    pSpimBase->spiCtrl = dwSpiCtrl;
    pSpimBase->dmaCtrl = dwDmaCtrl;

#if defined(SPI_9BIT_ENABLE) && SPI_9BIT_ENABLE
    pSpimBase->bit9Mode |= SPIM_9BIT_ENABLE_MASK;
#endif

}

static int cc6801_SpimClkDivSet(uint8_t bBusNum, uint32_t dwMaxSpeedHz)
{
    uint32_t dwSysClkHz = 0;
    uint32_t dwSpiMaxClkHz = 0;
    uint8_t bClkDiv = 1;

    //CC6801 SPI max clock support 24MHz
    if (dwMaxSpeedHz > 24000000)
        dwMaxSpeedHz = 24000000;

    dwSysClkHz = g_aClockTable[g_dwCurrentClock].dwMHz;
    dwSpiMaxClkHz = dwSysClkHz/2;
    bClkDiv = dwSpiMaxClkHz/dwMaxSpeedHz;

    if(bClkDiv > SPIM_CLKDIV_BIT)
        bClkDiv = SPIM_CLKDIV_BIT;
    else if (bClkDiv <= 0)
        bClkDiv = 1;

    g_tSpim[bBusNum].dwClkHz = dwSpiMaxClkHz/bClkDiv;


    if (bBusNum == SPIM_0)
    {
        regCKGEN->dw.clkCfg3 &= ~SPIM_0_CLKDIV_MASK;
        regCKGEN->dw.clkCfg3 |= ((bClkDiv & SPIM_CLKDIV_BIT) << SPIM_0_CLKDIV_SHIFT);
    }
    else if (bBusNum == SPIM_1)
    {
        regCKGEN->dw.clkCfg3 &= ~SPIM_1_CLKDIV_MASK;
        regCKGEN->dw.clkCfg3 |= ((bClkDiv & SPIM_CLKDIV_BIT) << SPIM_1_CLKDIV_SHIFT);
    }
    else if (bBusNum == SPIM_2)
    {
        regCKGEN->dw.clkCfg3 &= ~SPIM_2_CLKDIV_MASK;
        regCKGEN->dw.clkCfg3 |= ((bClkDiv & SPIM_CLKDIV_BIT) << SPIM_2_CLKDIV_SHIFT);
    }
    else if (bBusNum == SPIM_3)
    {
        regCKGEN->dw.clkCfg4 &= ~SPIM_3_CLKDIV_MASK;
        regCKGEN->dw.clkCfg4 |= ((bClkDiv & SPIM_CLKDIV_BIT) << SPIM_3_CLKDIV_SHIFT);
    }
    else
    {
        //TracerErr("Not Support SPI%d\r\n", bBusNum);
        return CC_ERROR_INVALID_PARAM;
    }

    return CC_SUCCESS;
}

#if defined(SPI_9BIT_ENABLE) && SPI_9BIT_ENABLE
void cc6801_Spim9BitCmd(uint8_t bBusNum, uint8_t const *pTxBuf, uint8_t bTxBufLen)
{
    uint32_t dwReg9bit = 0;
    struct S_Spi9BitCmd *sp9bitCmd;
    int iIdx = 0;

    sp9bitCmd = (struct S_Spi9BitCmd *)pTxBuf;

    for (iIdx=0; iIdx<(bTxBufLen/2); iIdx++)
    {
        dwReg9bit |= sp9bitCmd->bit9 << iIdx;
        g_baSpi9BitBuf[iIdx] = sp9bitCmd->bit8_0;
        sp9bitCmd++;
    }

    g_tSpim[bBusNum].pReg->bit9Mode |= dwReg9bit;

}
#endif

int cc6801_SpimInit(T_SpiDevice *pSpiDev)
{
    int iResult = 0;
    uint8_t bIdx = pSpiDev->bBusNum;

    iResult = cc6801_SpimClkDivSet(bIdx, pSpiDev->dwMaxSpeedHz);
    if (CC_SUCCESS != iResult)
        return iResult;

    g_tSpim[bIdx].pReg->spiCtrl = 0;
    g_tSpim[bIdx].pReg->bit9Mode = 0;
    g_tSpim[bIdx].pReg->dummyCycle = 0;

    cc6801_SpimConfig(g_tSpim[bIdx].pReg, pSpiDev);

    g_tSpim[bIdx].pReg->spiCtrl |= SPIM_CTRL_ENABLE_MASK;
    g_tSpim[bIdx].pReg->spiInt = (SPIM_INT_EVENT_ENABLE_MASK |
                                  SPIM_INT_ERROR_ENABLE_MASK);

    cc6801_SpimIntStatusClr(g_tSpim[bIdx].pReg);

    return CC_SUCCESS;
}

int cc6801_SpimWrite(uint8_t         bBusNum,
                     uint8_t const * pTxBuf,
                     uint8_t         bTxBufLen)
{
    T_SpimTransfer tXfer;

    memset((void *)&tXfer, 0, sizeof(T_SpimTransfer));

#if defined(SPI_9BIT_ENABLE) && SPI_9BIT_ENABLE
    cc6801_Spim9BitCmd(bBusNum, pTxBuf, bTxBufLen);
    tXfer.pTxBuffer = g_baSpi9BitBuf;
    tXfer.bTxLength = bTxBufLen/2;
#else
    tXfer.pTxBuffer = pTxBuf;
    tXfer.bTxLength = bTxBufLen;
#endif

    return cc6801_SpimTransfer(bBusNum, &tXfer);
}

int cc6801_SpimRead(uint8_t   bBusNum,
                    uint8_t * pRxBuf,
                    uint8_t   bRxBufLen)
{
    T_SpimTransfer tXfer;

    memset((void *)&tXfer, 0, sizeof(T_SpimTransfer));

    tXfer.pRxBuffer = pRxBuf;
    tXfer.bRxLength = bRxBufLen;

    return cc6801_SpimTransfer(bBusNum, &tXfer);
}

int cc6801_SpimWriteThenRead(uint8_t         bBusNum,
                             uint8_t const * pTxBuf,
                             uint8_t         bTxBufLen,
                             uint8_t       * pRxBuf,
                             uint8_t         bRxBufLen)
{
    T_SpimTransfer tXfer;

    memset((void *)&tXfer, 0, sizeof(T_SpimTransfer));

#if defined(SPI_9BIT_ENABLE) && SPI_9BIT_ENABLE
    cc6801_Spim9BitCmd(bBusNum, pTxBuf, bTxBufLen);
    tXfer.pTxBuffer = g_baSpi9BitBuf;
    tXfer.bTxLength = bTxBufLen/2;
#else
    tXfer.pTxBuffer = pTxBuf;
    tXfer.bTxLength = bTxBufLen;
#endif

    tXfer.pRxBuffer = pRxBuf;
    tXfer.bRxLength = bRxBufLen;

    return cc6801_SpimTransfer(bBusNum, &tXfer);
}

void cc6801_SpimQuadEnable(uint8_t bBusNum, int iEnable)
    {
    if (iEnable)
        g_tSpim[bBusNum].pReg->spiCtrl |= SPIM_CTRL_QUAD_MODE_MASK;
    else
        g_tSpim[bBusNum].pReg->spiCtrl &= ~SPIM_CTRL_QUAD_MODE_MASK;
}


int cc6801_SpimDummyClkSet(uint8_t bBusNum, int iCnt)
{
    if (iCnt > SPIM_WRB_DUMMY_CLK_MASK)
        return CC_ERROR_INVALID_PARAM;

    g_tSpim[bBusNum].pReg->dummyCycle &= ~SPIM_WRB_DUMMY_CLK_MASK;
    g_tSpim[bBusNum].pReg->dummyCycle |= (iCnt & SPIM_WRB_DUMMY_CLK_MASK);


    return CC_SUCCESS;
}

void cc6801_SpimDummyClkEnable(uint8_t bBusNum, int iEnable)
{
    if (iEnable)
        g_tSpim[bBusNum].pReg->dummyCycle |= SPIM_WRB_DUMMY_CLK_ENABLE_MASK;
    else
        g_tSpim[bBusNum].pReg->dummyCycle &= ~SPIM_WRB_DUMMY_CLK_ENABLE_MASK;
}

