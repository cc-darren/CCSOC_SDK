/******************************************************************************
*  Copyright 2017, Cloudchip, Inc.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Cloudchip, Inc. (C) 2017
******************************************************************************/

/******************************************************************************
*  Filename:
*  ---------
*
*
*  Project:
*  --------
*  CC6801
*
*  Description:
*  ------------
*  the description
*
*  Author:
*  -------
*  Blake   (cc-blake)
*
*===========================================================================
*
******************************************************************************/

/******************************************************************************
Head Block of The File
******************************************************************************/
#include "test_config.h"

#if (TEST_SPI)

#include <stdio.h>
#include <string.h>

#include "drvi_spi.h"

#define SPI_BUFFER_SIZE 64
#define WINBOND_BLOCK_NUM 16

#define WAIT_WINBOND_BUSY()                            \
    {                                                  \
         uint8_t cBusy = 0;                            \
         do                                            \
         {                                             \
             cBusy = winbond_ReadStatusReg1();         \
         } while(cBusy & 0x01);                        \
    }

/******************************************************************************
Declaration of External Variables & Functions
******************************************************************************/

/******************************************************************************
Declaration of data structure
******************************************************************************/
typedef struct S_SpiTxBuffer
{
    uint8_t bCmd;
    uint8_t baAddr[3];
    uint8_t baBuffer[SPI_BUFFER_SIZE];
}T_SpiTxBuffer;

/******************************************************************************
Declaration of Global Variables & Functions
******************************************************************************/

/******************************************************************************
Declaration of static Global Variables & Functions
******************************************************************************/
__align(4) static T_SpiTxBuffer g_tSpi0TxBuffer = {0, {0}, {0}};
__align(4) static uint8_t g_baSpi0RxBuffer[SPI_BUFFER_SIZE] = {0};

static void winbond_WriteEnable(void)
{
    uint8_t bCmd = 0;

    bCmd = 0x06;
    drvi_SpiWrite(TEST_SPIM, &bCmd, 1);
}

static uint8_t winbond_ReadStatusReg1(void)
{
    uint8_t bCmd = 0;
    uint8_t bVal = 0;

    bCmd = 0x05;
    drvi_SpiWriteThenRead(TEST_SPIM, &bCmd, 1, &bVal, 1);

    return bVal;
}

static void winbond_ChipErase(void)
{
    uint8_t bCmd = 0;

    bCmd = 0xC7;
    drvi_SpiWrite(TEST_SPIM, &bCmd, 1);
}

static void winbond_BlockErase64(uint32_t dwAddr)
{
    uint32_t dwCmdAddr = 0;

    dwCmdAddr = (((dwAddr & 0xFF) << 24) | (((dwAddr >> 8) & 0xFF) << 16) | (((dwAddr >> 16) & 0x0F) << 8) | 0xD8);

    drvi_SpiWrite(TEST_SPIM, (uint8_t *)&dwCmdAddr, sizeof(dwCmdAddr));
}

static int winbond_PageProgram(uint32_t dwAddr, uint8_t *pbBuf, int iSize)
{
    uint8_t bCmd = 0;
    int iTxSize = 0;

    if (iSize > SPI_BUFFER_SIZE)
        return CC_ERROR_NO_MEM;

    bCmd = 0x02;
    g_tSpi0TxBuffer.bCmd = bCmd;
    g_tSpi0TxBuffer.baAddr[0] = (dwAddr >> 16) & 0x0F;
    g_tSpi0TxBuffer.baAddr[1] = (dwAddr >> 8) & 0xFF;
    g_tSpi0TxBuffer.baAddr[2] = dwAddr & 0xFF;

    iTxSize = sizeof(g_tSpi0TxBuffer.bCmd) + sizeof(g_tSpi0TxBuffer.baAddr)+iSize;

    memcpy(&g_tSpi0TxBuffer.baBuffer, pbBuf, iSize);

    drvi_SpiWrite(TEST_SPIM, (uint8_t const *)&g_tSpi0TxBuffer, iTxSize);

    return CC_SUCCESS;
}

static void winbond_ReadData(uint32_t dwAddr, uint8_t *pbBuf, int iSize)
{
    uint8_t bCmd = 0;
    int iTxSize = 0;

    bCmd = 0x03;
    iTxSize = sizeof(g_tSpi0TxBuffer.bCmd) + sizeof(g_tSpi0TxBuffer.baAddr);

    g_tSpi0TxBuffer.bCmd = bCmd;
    g_tSpi0TxBuffer.baAddr[0] = (dwAddr >> 16) & 0x0F;
    g_tSpi0TxBuffer.baAddr[1] = (dwAddr >> 8) & 0xFF;
    g_tSpi0TxBuffer.baAddr[2] = dwAddr & 0xFF;

    drvi_SpiWriteThenRead(TEST_SPIM, (uint8_t const *)&g_tSpi0TxBuffer, iTxSize, pbBuf, iSize);
}

int TEST_SpiInit(void)
{
    return CC_SUCCESS;
}

void TEST_SpiRW(uint32_t dwCount)
{
    uint32_t dwIndex = 0;
    uint32_t dwAddr = 0;
    int iError = CC_SUCCESS;
    int iBlock = 0;
    int iSize = 0;

    iError = TEST_SpiInit();
    if (iError)
        return;

    printf("SPI RW test Start!\r\n");

    winbond_WriteEnable();
    winbond_ChipErase();
    WAIT_WINBOND_BUSY();

    while(dwIndex<dwCount)
    {
        memset(g_tSpi0TxBuffer.baBuffer, (dwIndex&(0xFF)), sizeof(g_tSpi0TxBuffer.baBuffer));

        if (iBlock >= WINBOND_BLOCK_NUM)
            iBlock = 0;

        dwAddr = iBlock++ << 16;
        iSize = SPI_BUFFER_SIZE - (dwIndex&(0x3F));

        winbond_WriteEnable();
        winbond_PageProgram(dwAddr, g_tSpi0TxBuffer.baBuffer, iSize);
        WAIT_WINBOND_BUSY();

        winbond_ReadData(dwAddr, g_baSpi0RxBuffer, iSize);

        winbond_WriteEnable();
        winbond_BlockErase64(dwAddr);
        WAIT_WINBOND_BUSY();

        if (memcmp(g_tSpi0TxBuffer.baBuffer, g_baSpi0RxBuffer, iSize))
            printf("SPI RW test Fail, w[0x%X],r[0x%X],s[%d]\r\n", g_tSpi0TxBuffer.baBuffer[0], g_baSpi0RxBuffer[0], iSize);

        winbond_ReadData(dwAddr, g_baSpi0RxBuffer, iSize);
        dwIndex++;
    }

    printf("SPI RW test Done!\r\n");
}

#endif //TEST_SPI
