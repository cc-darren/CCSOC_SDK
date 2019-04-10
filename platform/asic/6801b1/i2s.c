/* Copyright (c) 2019 Cloudchip, Inc. All Rights Reserved.
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
*  i2s.c
*
*  Project:
*  --------
*  cc6801
*
*  Description:
*  ------------
*  This is I2S driver
*
*  Author:
*  -------
*  CloudChip
*
*===========================================================================/
*  20170206 PAT initial version
******************************************************************************/
#define TracerFormat(fmt) "I2S: " fmt

#include "drvi_i2s.h"
#include "i2s.h"
#include "tracer.h"

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

volatile uint32_t g_dwI2S_RXDMA_INTR = 0;
volatile uint32_t g_dwI2S_TXDMA_INTR = 0;

T_callback  i2s_callback_handler = NULL;

struct S_RateTable{
    uint32_t dwRate;
    uint8_t  bClkDiv;
};

struct S_RateTable sTableWS16[] =
{
    {16000, 0x7C},
    {32000, 0x3D},
    {48000, 0x29},
    {96000, 0x14},
};

struct S_RateTable sTableWS32[] =
{
    {16000, 0x3D},
    {32000, 0x1E},
    {48000, 0x14},
    {96000, 0x09},
};


void I2S_RXDMA_IRQHandler(void)
{
    uint32_t rdata;
    rd(I2S_INTR_REG, rdata);
    wr(I2S_INTR_REG, (rdata | 1<<16) & ~(1<<0));   // clear interrupt
    g_dwI2S_RXDMA_INTR = 1;

    if(NULL != i2s_callback_handler)
    {
        i2s_callback_handler((void*)NULL);
    }
}

void I2S_TXDMA_IRQHandler(void)
{
    uint32_t rdata;
    rd(I2S_INTR_REG, rdata);
    wr(I2S_INTR_REG, (rdata | 1<<17) & ~(1<<1));   // clear interrupt
    g_dwI2S_TXDMA_INTR = 1;
}

void cc6801_I2sInit(T_callback handler)
{
    regI2S->i2sClkCtrl = 0;
    regI2S->i2sRxCtrl = 0x00143C00;
    regI2S->i2sTxCtrl = 0x00143C00;    // TXST = 1, TXFIFOTHRES = 4
                                       // [13-8] TxLR FIFO enable, DMA enable, INT disable,111100

    regI2S->i2sInt = 0;                // interrupt disable
    regI2S->i2sDmaRxEn = 0;            // Rx DMA disable
    regI2S->i2sDmaTxEn = 0;            // Tx DMA disable
    regI2S->i2sDmaByteCtrl = 0;

    i2s_callback_handler = handler;
}

int cc6801_I2sSetFormat(uint16_t wFmt)
{
    uint32_t dwClkCtrl = regI2S->i2sClkCtrl;
    uint32_t dwTxCtrl = regI2S->i2sTxCtrl;
    uint32_t dwRxCtrl = regI2S->i2sRxCtrl;

    /* DAI mode */
    dwTxCtrl &= ~CC6801_I2S_MOD_MASK;
    dwRxCtrl &= ~CC6801_I2S_MOD_MASK;
    switch (wFmt & DAI_FORMAT_MASK)
    {
        case DAI_FORMAT_RIGHT_J:
            dwTxCtrl |= CC6801_I2S_MOD_RJM;
            dwRxCtrl |= CC6801_I2S_MOD_RJM;
            break;
        case DAI_FORMAT_LEFT_J:
            dwTxCtrl |= CC6801_I2S_MOD_LJM;
            dwRxCtrl |= CC6801_I2S_MOD_LJM;
            break;
        default:
            TracerInfo("Not support format%d, using default I2S format...\r\n", wFmt);
        case DAI_FORMAT_I2S:
            dwTxCtrl |= CC6801_I2S_MOD_I2S;
            dwRxCtrl |= CC6801_I2S_MOD_I2S;
            break;
    }

    /* DAI clock master masks */
    dwClkCtrl &= ~CC6801_I2S_MS_MASK;
    switch (wFmt & DAI_MASTER_MASK)
    {
        case DAI_CODEC_MASTER:
            break;
        default:
            TracerInfo("Not support mode%d, using default master mode...\r\n");
        case DAI_CODEC_SLAVE:
            dwClkCtrl |= CC6801_I2S_MS_MASTER;
            break;
    }

    regI2S->i2sClkCtrl |= dwClkCtrl;
    regI2S->i2sTxCtrl |= dwTxCtrl;
    regI2S->i2sRxCtrl |= dwRxCtrl;

    return CC_SUCCESS;
}

static int cc6801_I2sRateValue(int iRate, uint8_t *pbValue)
{
    int iIdx;
    struct S_RateTable *psRateTable;

    uint32_t dwClkCtrl = regI2S->i2sClkCtrl;
    uint32_t dwWordSelect = 0;
    int iTableSize = 0;

    dwWordSelect = (dwClkCtrl & CC6801_I2S_WSRES_MASK);

    if (dwWordSelect == CC6801_I2S_WSRES_16)
    {
        psRateTable = sTableWS16;
        iTableSize = ARRAY_SIZE(sTableWS16);
    }
    else if(dwWordSelect == CC6801_I2S_WSRES_32)
    {
        psRateTable = sTableWS32;
        iTableSize = ARRAY_SIZE(sTableWS32);
    }
    else
        return CC_ERROR_INVALID_PARAM;

    for (iIdx = 0; iIdx < iTableSize; iIdx++) {
        if (psRateTable[iIdx].dwRate >= iRate) {
            *pbValue = psRateTable[iIdx].bClkDiv;
            return CC_SUCCESS;
        }
    }
    *pbValue = psRateTable[0].bClkDiv;
    return CC_ERROR_INVALID_PARAM;
}

static int cc6801_I2sSetClkDiv(int iFrameRate)
{
    uint8_t bClkDiv = 0;
    uint32_t dwClkCtrl = regI2S->i2sClkCtrl;

    if (cc6801_I2sRateValue(iFrameRate, &bClkDiv) != CC_SUCCESS)
        return CC_ERROR_INVALID_PARAM;

    dwClkCtrl &= ~CC6801_I2S_CLKDIV_MASK;
    dwClkCtrl |= bClkDiv << CC6801_I2S_CLKDIV_SHIFT;
    dwClkCtrl |= CC6801_I2S_CLKEN_ENABLE;

    regI2S->i2sClkCtrl |= dwClkCtrl;
    return 0;
}

int cc6801_I2sHwParams(T_DaiHwParams *tpParams)
{
    uint32_t dwClkCtrl = regI2S->i2sClkCtrl;
    uint32_t dwTxCtrl = regI2S->i2sTxCtrl;
    uint32_t dwRxCtrl = regI2S->i2sRxCtrl;

    dwClkCtrl &= ~CC6801_I2S_WSRES_MASK;
    dwTxCtrl &= (~CC6801_I2S_ALIGN_MASK | ~CC6801_I2S_RES_MASK);
    dwRxCtrl &= (~CC6801_I2S_ALIGN_MASK | ~CC6801_I2S_RES_MASK);

    switch (tpParams->iFormat)
    {
        case DAI_PCM_FORMAT_S8:
            dwClkCtrl |= CC6801_I2S_WSRES_16;
            dwTxCtrl |= (CC6801_I2S_ALIGN_8LE | CC6801_I2S_RES_8);
            dwRxCtrl |= (CC6801_I2S_ALIGN_8LE | CC6801_I2S_RES_8);
            break;
        case DAI_PCM_FORMAT_S20_3LE:
            dwClkCtrl |= CC6801_I2S_WSRES_32;
            dwTxCtrl |= (CC6801_I2S_ALIGN_24LE | CC6801_I2S_RES_20);
            dwRxCtrl |= (CC6801_I2S_ALIGN_24LE | CC6801_I2S_RES_20);
            break;
        case DAI_PCM_FORMAT_S24_LE:
            dwClkCtrl |= CC6801_I2S_WSRES_32;
            dwTxCtrl |= (CC6801_I2S_ALIGN_24LE | CC6801_I2S_RES_24);
            dwRxCtrl |= (CC6801_I2S_ALIGN_24LE | CC6801_I2S_RES_24);
            break;
        case DAI_PCM_FORMAT_S24_BE:
            dwClkCtrl |= CC6801_I2S_WSRES_32;
            dwTxCtrl |= (CC6801_I2S_ALIGN_24BE | CC6801_I2S_RES_24);
            dwRxCtrl |= (CC6801_I2S_ALIGN_24BE | CC6801_I2S_RES_24);
            break;
        default:
            TracerInfo("Not support fomat%d, using default 16bits...\r\n", tpParams->iFormat);
        case DAI_PCM_FORMAT_S16_LE:
            dwClkCtrl |= CC6801_I2S_WSRES_16;
            dwTxCtrl |= (CC6801_I2S_ALIGN_16LE | CC6801_I2S_RES_16);
            dwRxCtrl |= (CC6801_I2S_ALIGN_16LE | CC6801_I2S_RES_16);
            break;
    }

    regI2S->i2sClkCtrl |= dwClkCtrl;
    regI2S->i2sTxCtrl |= dwTxCtrl;
    regI2S->i2sRxCtrl |= dwRxCtrl;

    cc6801_I2sSetClkDiv(tpParams->iRate);

    return CC_SUCCESS;
}

void cc6801_I2sTxConfig(uint32_t dwDmaBufStart, uint32_t dwDmaBufEnd)
{
    regI2S->i2sDmaTxStartAddr = dwDmaBufStart;    //Tx start address
    regI2S->i2sDmaTxEndAddr = dwDmaBufEnd;        //Tx end address
}

void cc6801_I2sRxConfig(uint32_t dwDmaBufStart, uint32_t dwDmaBufEnd)
{
    regI2S->i2sDmaRxStartAddr = dwDmaBufStart;    //Rx start address
    regI2S->i2sDmaRxEndAddr = dwDmaBufEnd;        //Rx end address
}

void cc6801_I2sStart(uint8_t bTxByte, uint8_t bRxByte)
{
    regI2S->i2sDmaByteCtrl = (0 | ((bTxByte-1) << 8) | (bRxByte-1));    //0:1byte, FF:256bytes

    regI2S->i2sInt = 0x00000003;                // Tx/Rx interrupt enable

    regI2S->i2sDmaRxEn = 0x00000001;            // Rx DMA enable
    regI2S->i2sDmaTxEn = 0x00000001;            // Tx DMA enable

    NVIC_EnableIRQ(I2S_RXDMA_IRQn);
    NVIC_EnableIRQ(I2S_TXDMA_IRQn);

}

void cc6801_I2sStop(void)
{
    NVIC_DisableIRQ(I2S_TXDMA_IRQn);
    NVIC_DisableIRQ(I2S_RXDMA_IRQn);

    regI2S->i2sInt = 0x00000000;            //disable interrupt
    regI2S->i2sDmaTxEn = 0x00000000;        //stop transmiting data
    regI2S->i2sDmaRxEn = 0x00000000;        //stop receiving data
}
