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
         UINT8 cBusy = 0;                              \
         do                                            \
         {                                             \
             cBusy = winbond_ReadStatusReg1(&spi);   \
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
    UINT8 bCmd;
    UINT8 baAddr[3];
    UINT8 baBuffer[SPI_BUFFER_SIZE];
}T_SpiTxBuffer;

/******************************************************************************
Declaration of Global Variables & Functions
******************************************************************************/

/******************************************************************************
Declaration of static Global Variables & Functions
******************************************************************************/
__align(4) static T_SpiTxBuffer g_tSpi0TxBuffer = {0, {0}, {0}};
__align(4) static UINT8 g_baSpi0RxBuffer[SPI_BUFFER_SIZE] = {0};

static void winbond_WriteEnable(const T_SpiDevice *spi)
{
    UINT8 bCmd = 0;

    bCmd = 0x06;
    drvi_SpiWrite(spi, &bCmd, 1);
}

static UINT8 winbond_ReadStatusReg1(const T_SpiDevice *spi)
{
    UINT8 bCmd = 0;
    UINT8 bVal = 0;

    bCmd = 0x05;
    drvi_SpiWriteThenRead(spi, &bCmd, 1, &bVal, 1);

    return bVal;
}

static void winbond_ChipErase(const T_SpiDevice *spi)
{
    UINT8 bCmd = 0;

    bCmd = 0xC7;
    drvi_SpiWrite(spi, &bCmd, 1);
}

static void winbond_BlockErase64(const T_SpiDevice *spi, UINT32 dwAddr)
{
    UINT32 dwCmdAddr = 0;

    dwCmdAddr = (((dwAddr & 0xFF) << 24) | (((dwAddr >> 8) & 0xFF) << 16) | (((dwAddr >> 16) & 0x0F) << 8) | 0xD8);

    drvi_SpiWrite(spi, (UINT8 *)&dwCmdAddr, sizeof(dwCmdAddr));
}

static INT16 winbond_PageProgram(const T_SpiDevice *spi, UINT32 dwAddr, UINT8 *pbBuf, INT16 iSize)
{
    UINT8 bCmd = 0;
    INT16 iTxSize = 0;

    if (iSize > SPI_BUFFER_SIZE)
        return CC_ERROR_NO_MEM;

    bCmd = 0x02;
    g_tSpi0TxBuffer.bCmd = bCmd;
    g_tSpi0TxBuffer.baAddr[0] = (dwAddr >> 16) & 0x0F;
    g_tSpi0TxBuffer.baAddr[1] = (dwAddr >> 8) & 0xFF;
    g_tSpi0TxBuffer.baAddr[2] = dwAddr & 0xFF;

    iTxSize = sizeof(g_tSpi0TxBuffer.bCmd) + sizeof(g_tSpi0TxBuffer.baAddr)+iSize;

    memcpy(&g_tSpi0TxBuffer.baBuffer, pbBuf, iSize);

    drvi_SpiWrite(spi, (UINT8 const *)&g_tSpi0TxBuffer, iTxSize);

    return CC_SUCCESS;
}

static void winbond_ReadData(T_SpiDevice *spi, UINT32 dwAddr, UINT8 *pbBuf, INT16 iSize)
{
    UINT8 bCmd = 0;
    INT16 iTxSize = 0;

    bCmd = 0x03;
    iTxSize = sizeof(g_tSpi0TxBuffer.bCmd) + sizeof(g_tSpi0TxBuffer.baAddr);

    g_tSpi0TxBuffer.bCmd = bCmd;
    g_tSpi0TxBuffer.baAddr[0] = (dwAddr >> 16) & 0x0F;
    g_tSpi0TxBuffer.baAddr[1] = (dwAddr >> 8) & 0xFF;
    g_tSpi0TxBuffer.baAddr[2] = dwAddr & 0xFF;

    drvi_SpiWriteThenRead(spi, (UINT8 const *)&g_tSpi0TxBuffer, iTxSize, pbBuf, iSize);
}

INT16 TEST_SpiInit(T_SpiDevice *spi)
{
    spi->bBusNum = 1;
    spi->wMode = DRVI_SPI_MODE_0;

    drvi_SpiInit(spi);

    return CC_SUCCESS;
}

void TEST_SpiRW(UINT32 dwCount)
{
    UINT32 dwIndex = 0;
    UINT32 dwAddr = 0;
    INT16 iError = CC_SUCCESS;
    INT16 iBlock = 0;
    INT16 iSize = 0;

    T_SpiDevice spi;

    iError = TEST_SpiInit(&spi);
    if (iError)
        return;

    printf("SPI RW test Start!\r\n");

    winbond_WriteEnable(&spi);
    winbond_ChipErase(&spi);
    WAIT_WINBOND_BUSY();

    while(dwIndex<dwCount)
    {
        memset(g_tSpi0TxBuffer.baBuffer, (dwIndex&(0xFF)), sizeof(g_tSpi0TxBuffer.baBuffer));

        if (iBlock >= WINBOND_BLOCK_NUM)
            iBlock = 0;

        dwAddr = iBlock++ << 16;
        iSize = SPI_BUFFER_SIZE - (dwIndex&(0x3F));

        winbond_WriteEnable(&spi);
        winbond_PageProgram(&spi, dwAddr, g_tSpi0TxBuffer.baBuffer, iSize);
        WAIT_WINBOND_BUSY();

        winbond_ReadData(&spi, dwAddr, g_baSpi0RxBuffer, iSize);

        winbond_WriteEnable(&spi);
        winbond_BlockErase64(&spi, dwAddr);
        WAIT_WINBOND_BUSY();

        if (memcmp(g_tSpi0TxBuffer.baBuffer, g_baSpi0RxBuffer, iSize))
            printf("SPI RW test Fail, w[0x%X],r[0x%X],s[%d]\r\n", g_tSpi0TxBuffer.baBuffer[0], g_baSpi0RxBuffer[0], iSize);

        winbond_ReadData(&spi, dwAddr, g_baSpi0RxBuffer, iSize);
        dwIndex++;
    }

    printf("SPI RW test Done!\r\n");
}

#endif //TEST_SPI
