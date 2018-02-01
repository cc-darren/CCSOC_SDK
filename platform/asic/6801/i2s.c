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
#include <stdio.h>
#include "global.h"
#include "cc6801_reg.h"

volatile uint32_t g_dwI2S_RXDMA_INTR = 0;
volatile uint32_t g_dwI2S_TXDMA_INTR = 0;

T_callback  i2s_callback_handler = NULL;

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
    wr(I2S_CLK_CTRL_REG,      0x000F2909);      // I2S enabled, 16bit, 48Khz, Master
    //wr(I2S_CLK_CTRL_REG,      0x001F2C09);    // I2S enabled, 32bit, master
    //wr(I2S_CLK_CTRL_REG,      0x000F0100);    // I2S enabled, 16bit, slave

    //wr(I2S_RX_CTRL_REG,       ----------);    // for RX setting
    wr(I2S_TX_CTRL_REG,       0x0014BC3D);      // [15-14] 16bit,10 , [6-2] 16bit,01111 , [1-0] I2S,01
                                                // [13-8] TxLR FIFO enable, DMA enable, INT disable,111100
    wr(I2S_INTR_REG,          0x00000000);      // interrupt disable
    wr(I2S_DMA_TX_ENABLE_REG, 0x00000000);      // DMA disable

    i2s_callback_handler = handler;
}

void cc6801_I2sTxConfig(uint32_t dwDmaBufStart, uint32_t dwDmaBufEnd)
{
    wr(I2S_DMA_TX_START_REG, dwDmaBufStart);    //Tx start address
    wr(I2S_DMA_TX_END_REG,   dwDmaBufEnd);      //Tx end address
}

void cc6801_I2sRxConfig(uint32_t dwDmaBufStart, uint32_t dwDmaBufEnd)
{
    wr(I2S_DMA_RX_START_REG, dwDmaBufStart);    //Rx start address
    wr(I2S_DMA_RX_END_REG,   dwDmaBufEnd);      //Rx end address
}

void cc6801_I2sStart(uint16_t dwTxByte, uint16_t dwRxByte)
{
    wr(I2S_DMA_BYTE_REG,      (0 | ((dwTxByte-1) << 8) | (dwRxByte-1)));  //0:1byte, FF:256bytes
    wr(I2S_INTR_REG,          0x00000003);                                // Tx/Rx interrupt enable

    wr(I2S_DMA_TX_ENABLE_REG, 0x00000001);                                // Tx DMA enable
    NVIC_EnableIRQ(I2S_TXDMA_IRQn);

    wr(I2S_DMA_RX_ENABLE_REG, 0x00000001);                                // Rx DMA enable
    NVIC_EnableIRQ(I2S_RXDMA_IRQn);
}

void cc6801_I2sStop(void)
{
    NVIC_DisableIRQ(I2S_TXDMA_IRQn);
    NVIC_DisableIRQ(I2S_RXDMA_IRQn);

    wr(I2S_INTR_REG, 0x00000000);                 //disable interrupt
    wr(I2S_DMA_TX_ENABLE_REG, 0x00000000);        //stop transmiting data
    wr(I2S_DMA_RX_ENABLE_REG, 0x00000000);        //stop receiving data
}
