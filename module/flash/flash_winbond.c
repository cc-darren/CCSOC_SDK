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
#include <stdio.h>
#include <string.h>

#include "drvi_gpio.h"
#include "drvi_spi.h"
#include "tracer.h"
#include "flash_winbond.h"

#if defined(MODULE_FLASH) && (MODULE_FLASH==FLASH_W25Q256JV)
#define WINBOND_4BYTE_ADDRESS   1
#define WINBOND_DEVICE          0x18
#endif

#if defined(MODULE_FLASH) && (MODULE_FLASH==FLASH_W25Q80DV)
#define WINBOND_DEVICE          0x13
#endif

#if defined(WINBOND_QUAD_ENABLE) && (MODULE_FLASH==FLASH_W25Q256JV)
#define WINBOND_MANUFACTURER    0x00
#else
#define WINBOND_MANUFACTURER    0xEF
#endif

#define WINBOND_BUFFER_SIZE     128

#define CMD_W25_WREN         0x06    /* Write Enable */
#define CMD_W25_WRDI         0x04    /* Write Disable */
#define CMD_W25_RDSR1        0x05    /* Read Status Register 1*/
#define CMD_W25_RDSR3        0x15    /* Read Status Register 3*/
#define CMD_W25_RDSR2        0x35    /* Read Status Register 2*/
#define CMD_W25_WRSR1        0x01    /* Write Status Register 1*/
#define CMD_W25_WRSR2        0x31    /* Write Status Register 2*/
#define CMD_W25_WRSR3        0x11    /* Write Status Register 3*/
#define CMD_W25_RD           0x03    /* Read Data Bytes */
#define CMD_W25_RD4B         0x13    /* Read Data 4Bytes */
#define CMD_W25_FRD          0x0b    /* Read Data Bytes at Higher Speed */
#define CMD_W25_FRD4B        0x0c    /* Read Data 4Bytes at Higher Speed */
#define CMD_W25_FRDQIO       0xeb    /* Read Data Bytes by Quad IO */
#define CMD_W25_FRDQIO4B     0xec    /* Read Data 4Bytes by Quad IO */
#define CMD_W25_PP           0x02    /* Page Program */
#define CMD_W25_PP4B         0x12    /* Page Program with 4Byte Address */
#define CMD_W25_QIPP         0x32    /* Quad Input Page Program */
#define CMD_W25_QIPP4B       0x34    /* Quad Input Page Program with 4Byte Address */
#define CMD_W25_SE           0x20    /* Sector (4K) Erase */
#define CMD_W25_SE4B         0x21    /* Sector (4K) Erase with 4Byte Address */
#define CMD_W25_BE           0xd8    /* Block (64K) Erase */
#define CMD_W25_CE           0xc7    /* Chip Erase */
#define CMD_W25_DP           0xb9    /* Deep Power-down */
#define CMD_W25_RED          0xab    /* Release from DP, and Device ID */
#define CMD_W25_RMD          0x90    /* Read Manufacturer and Device ID */
#define CMD_W25_RMDQIO       0x94    /* Read Manufacturer and Device ID by Quad IO*/

#define REG_WRSR2_QE         0x02
#define REG_WRSR3_ADP        0x02

#define QUAD_CMD(_x) (uint32_t) ( \
                        ((((_x)&0x40) >> 6) <<  0) | \
                        ((((_x)&0x80) >> 7) <<  4) | \
                        ((((_x)&0x10) >> 4) <<  8) | \
                        ((((_x)&0x20) >> 5) << 12) | \
                        ((((_x)&0x04) >> 2) << 16) | \
                        ((((_x)&0x08) >> 3) << 20) | \
                        ((((_x)&0x01) >> 0) << 24) | \
                        ((((_x)&0x02) >> 1) << 28)   \
                    )

#define QUAD_VAL(_x) (uint8_t) ( \
                        ((((_x)&0x02000000) >> 25) << 0) | \
                        ((((_x)&0x20000000) >> 29) << 1) | \
                        ((((_x)&0x00020000) >> 17) << 2) | \
                        ((((_x)&0x00200000) >> 21) << 3) | \
                        ((((_x)&0x00000200) >> 9)  << 4) | \
                        ((((_x)&0x00002000) >> 13) << 5) | \
                        ((((_x)&0x00000002) >> 1)  << 6) | \
                        ((((_x)&0x00000020) >> 5)  << 7)   \
                    )

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
typedef struct S_SpiPageProg
{
    uint8_t bCmd;
    uint8_t baAddr[3];
    uint8_t baBuffer[WINBOND_BUFFER_SIZE];
}T_SpiPageProg;

#ifdef WINBOND_QUAD_ENABLE
typedef struct S_SpiPageProgQuadCmd
{
    uint32_t dwCmd;
    uint32_t dwaAddr[3];
    uint8_t baBuffer[WINBOND_BUFFER_SIZE];
}T_SpiPageProgQuadCmd;

typedef struct S_SpiReadQuadCmd
{
    uint32_t dwCmd;
    uint8_t baAddr[3];
    uint8_t bMData;
}T_SpiReadQuadCmd;
#endif

#ifdef WINBOND_4BYTE_ADDRESS
typedef struct S_SpiPageProg4ByteAddr
{
    uint8_t bCmd;
    uint8_t baAddr[4];
    uint8_t baBuffer[WINBOND_BUFFER_SIZE];
}T_SpiPageProg4ByteAddr;
#ifdef WINBOND_QUAD_ENABLE
typedef struct S_SpiPageProgQuadCmd4ByteAddr
{
    uint32_t dwCmd;
    uint32_t dwaAddr[4];
    uint8_t baBuffer[WINBOND_BUFFER_SIZE];
}T_SpiPageProgQuadCmd4ByteAddr;

#pragma pack(1)
typedef struct S_SpiReadQuadCmd4ByteAddr
{
    uint32_t dwCmd;
    uint8_t baAddr[4];
    uint8_t bMData;
}T_SpiReadQuadCmd4ByteAddr;
#endif
#endif

/******************************************************************************
Declaration of Global Variables & Functions
******************************************************************************/

/******************************************************************************
Declaration of static Global Variables & Functions
******************************************************************************/

#ifdef WINBOND_QUAD_ENABLE
static uint8_t winbond_SpiRead(uint8_t *pbCmd, int iSize)
{
    uint32_t dwQEcmd = 0;
    uint32_t dwVal = 0;

    dwQEcmd = QUAD_CMD(*pbCmd);
    drvi_SpiWriteThenRead(FLASH_IF_ID, (uint8_t *)&dwQEcmd, sizeof(uint32_t), (uint8_t *)&dwVal, sizeof(uint32_t));

    return QUAD_VAL(dwVal);
}

static int winbond_SpiWrite(uint8_t *pbCmd, int iSize)
{
    uint32_t dwQuadCmd[8] = {0};
    int i = 0;

    for (i=0; i<=iSize; i++)
    {
        dwQuadCmd[i] = QUAD_CMD(*(pbCmd+i));
    }

    return drvi_SpiWrite(FLASH_IF_ID, (uint8_t *)dwQuadCmd, iSize*sizeof(uint32_t));
}

#ifdef WINBOND_4BYTE_ADDRESS
static void winbond_SpiReadData4ByteAddr(uint8_t *pbCmd, uint32_t dwAddr, uint8_t *pbBuf, int iSize)
{
    __align(4) T_SpiReadQuadCmd4ByteAddr tQuadCmd;

    tQuadCmd.dwCmd = QUAD_CMD(*pbCmd);
    tQuadCmd.baAddr[0] = (dwAddr >> 24) & 0x01;
    tQuadCmd.baAddr[1] = (dwAddr >> 16) & 0xFF;
    tQuadCmd.baAddr[2] = (dwAddr >> 8) & 0xFF;
    tQuadCmd.baAddr[3] = dwAddr & 0xFF;

    tQuadCmd.bMData = 0xFF;

    drvi_SpimDummyClkEnable(FLASH_IF_ID, 1);
    drvi_SpiWriteThenRead(FLASH_IF_ID, (uint8_t const *)&tQuadCmd, sizeof(tQuadCmd), pbBuf, iSize);
    drvi_SpimDummyClkEnable(FLASH_IF_ID, 0);
}

static int winbond_SpiWriteData4ByteAddr(uint8_t *pbCmd, uint32_t dwAddr, uint8_t *pbBuf, int iSize)
{
    __align(4) T_SpiPageProgQuadCmd4ByteAddr tSpiTrans;
    int iTxSize = 0;

    if (iSize > WINBOND_BUFFER_SIZE)
        return CC_ERROR_NO_MEM;

    tSpiTrans.dwCmd = QUAD_CMD(*pbCmd);
    tSpiTrans.dwaAddr[0] = QUAD_CMD((dwAddr >> 24) & 0x01);
    tSpiTrans.dwaAddr[1] = QUAD_CMD((dwAddr >> 16) & 0xFF);
    tSpiTrans.dwaAddr[2] = QUAD_CMD((dwAddr >> 8) & 0xFF);
    tSpiTrans.dwaAddr[3] = QUAD_CMD(dwAddr & 0xFF);

    iTxSize = sizeof(tSpiTrans.dwCmd) + sizeof(tSpiTrans.dwaAddr)+iSize;

    memcpy(&tSpiTrans.baBuffer, pbBuf, iSize);

    drvi_SpiWrite(FLASH_IF_ID, (uint8_t const *)&tSpiTrans, iTxSize);

    return CC_SUCCESS;
}
#endif

static void winbond_SpiReadData(uint8_t *pbCmd, uint32_t dwAddr, uint8_t *pbBuf, int iSize)
{
    T_SpiReadQuadCmd tQuadCmd;

    tQuadCmd.dwCmd = QUAD_CMD(*pbCmd);
    tQuadCmd.baAddr[0] = (dwAddr >> 16) & 0x0F;
    tQuadCmd.baAddr[1] = (dwAddr >> 8) & 0xFF;
    tQuadCmd.baAddr[2] = dwAddr & 0xFF;
    tQuadCmd.bMData = 0xFF;

    drvi_SpimDummyClkEnable(FLASH_IF_ID, 1);
    drvi_SpiWriteThenRead(FLASH_IF_ID, (uint8_t const *)&tQuadCmd, sizeof(T_SpiReadQuadCmd), pbBuf, iSize);
    drvi_SpimDummyClkEnable(FLASH_IF_ID, 0);
}

#ifndef WINBOND_4BYTE_ADDRESS
static int winbond_SpiWriteData(uint8_t *pbCmd, uint32_t dwAddr, uint8_t *pbBuf, int iSize)
{
    T_SpiPageProgQuadCmd tQuadCmd;
    int iTxSize = 0;

    if (iSize > WINBOND_BUFFER_SIZE)
        return CC_ERROR_NO_MEM;

    tQuadCmd.dwCmd = QUAD_CMD(*pbCmd);
    tQuadCmd.dwaAddr[0] = QUAD_CMD((dwAddr >> 16) & 0x0F);
    tQuadCmd.dwaAddr[1] = QUAD_CMD((dwAddr >> 8) & 0xFF);
    tQuadCmd.dwaAddr[2] = QUAD_CMD(dwAddr & 0xFF);

    iTxSize = sizeof(tQuadCmd.dwCmd) + sizeof(tQuadCmd.dwaAddr) + iSize;

    memcpy(tQuadCmd.baBuffer, pbBuf, iSize);

    drvi_SpiWrite(FLASH_IF_ID, (uint8_t const *)&tQuadCmd, iTxSize);

    return CC_SUCCESS;
}
#endif
#else
static uint8_t winbond_SpiRead(uint8_t *pbCmd, int iSize)
{
    __align(4) uint8_t bCmd = 0;
    __align(4) uint8_t bVal = 0;

    bCmd = *pbCmd;

    drvi_SpiWriteThenRead(FLASH_IF_ID, &bCmd, iSize, &bVal, 1);
    return bVal;
}

static int winbond_SpiWrite(uint8_t *pbCmd, int iSize)
{
    __align(4) uint8_t bCmd[8] = {0};
    int i = 0;

    for (i=0; i<=iSize; i++)
    {
        bCmd[i] = *(pbCmd+i);
    }

    return drvi_SpiWrite(FLASH_IF_ID, bCmd, iSize);
}

#ifdef WINBOND_4BYTE_ADDRESS
static int winbond_SpiReadData4ByteAddr(uint8_t *pbCmd, uint32_t dwAddr, uint8_t *pbBuf, int iSize)
{
    __align(4) T_SpiPageProg4ByteAddr tSpiTrans;
    int iRxSize = 0;

    if (iSize > WINBOND_BUFFER_SIZE)
        return CC_ERROR_NO_MEM;

    iRxSize = sizeof(tSpiTrans.bCmd) + sizeof(tSpiTrans.baAddr);

    tSpiTrans.bCmd = *pbCmd;
    tSpiTrans.baAddr[0] = (dwAddr >> 24) & 0x01;
    tSpiTrans.baAddr[1] = (dwAddr >> 16) & 0xFF;
    tSpiTrans.baAddr[2] = (dwAddr >> 8) & 0xFF;
    tSpiTrans.baAddr[3] = dwAddr & 0xFF;

    drvi_SpiWriteThenRead(FLASH_IF_ID, (uint8_t const *)&tSpiTrans, iRxSize, pbBuf, iSize);

    return CC_SUCCESS;
}

static int winbond_SpiWriteData4ByteAddr(uint8_t *pbCmd, uint32_t dwAddr, uint8_t *pbBuf, int iSize)
{
    __align(4) T_SpiPageProg4ByteAddr tSpiTrans;
    int iTxSize = 0;

    if (iSize > WINBOND_BUFFER_SIZE)
        return CC_ERROR_NO_MEM;

    tSpiTrans.bCmd = *pbCmd;
    tSpiTrans.baAddr[0] = (dwAddr >> 24) & 0x01;
    tSpiTrans.baAddr[1] = (dwAddr >> 16) & 0xFF;
    tSpiTrans.baAddr[2] = (dwAddr >> 8) & 0xFF;
    tSpiTrans.baAddr[3] = dwAddr & 0xFF;

    iTxSize = sizeof(tSpiTrans.bCmd) + sizeof(tSpiTrans.baAddr)+iSize;

    memcpy(&tSpiTrans.baBuffer, pbBuf, iSize);

    drvi_SpiWrite(FLASH_IF_ID, (uint8_t const *)&tSpiTrans, iTxSize);

    return CC_SUCCESS;
}
#endif

static void winbond_SpiReadData(uint8_t *pbCmd, uint32_t dwAddr, uint8_t *pbBuf, int iSize)
{
    __align(4) T_SpiPageProg tSpiTrans;
    int iTxSize = 0;

    iTxSize = sizeof(tSpiTrans.bCmd) + sizeof(tSpiTrans.baAddr);

    tSpiTrans.bCmd = *pbCmd;
    tSpiTrans.baAddr[0] = (dwAddr >> 16) & 0x0F;
    tSpiTrans.baAddr[1] = (dwAddr >> 8) & 0xFF;
    tSpiTrans.baAddr[2] = dwAddr & 0xFF;

    drvi_SpiWriteThenRead(FLASH_IF_ID, (uint8_t const *)&tSpiTrans, iTxSize, pbBuf, iSize);
}

#ifndef WINBOND_4BYTE_ADDRESS
static int winbond_SpiWriteData(uint8_t *pbCmd, uint32_t dwAddr, uint8_t *pbBuf, int iSize)
{
    __align(4) T_SpiPageProg tSpiTrans;
    int iTxSize = 0;

    if (iSize > WINBOND_BUFFER_SIZE)
        return CC_ERROR_NO_MEM;

    tSpiTrans.bCmd = *pbCmd;
    tSpiTrans.baAddr[0] = (dwAddr >> 16) & 0x0F;
    tSpiTrans.baAddr[1] = (dwAddr >> 8) & 0xFF;
    tSpiTrans.baAddr[2] = dwAddr & 0xFF;

    iTxSize = sizeof(tSpiTrans.bCmd) + sizeof(tSpiTrans.baAddr)+iSize;

    memcpy(&tSpiTrans.baBuffer, pbBuf, iSize);

    drvi_SpiWrite(FLASH_IF_ID, (uint8_t const *)&tSpiTrans, iTxSize);

    return CC_SUCCESS;
}
#endif
#endif

static void winbond_WriteEnable(void)
{
    uint8_t bCmd = 0;

    bCmd = CMD_W25_WREN;
    winbond_SpiWrite(&bCmd, 1);
}

static uint8_t winbond_ReadStatusReg1(void)
{
    uint8_t bCmd = 0;

    bCmd = CMD_W25_RDSR1;
    return winbond_SpiRead(&bCmd, 1);;
}

#ifdef WINBOND_QUAD_ENABLE
static uint8_t winbond_ReadStatusReg2(void)
{
    uint8_t bCmd = 0;

    bCmd = CMD_W25_RDSR2;
    return winbond_SpiRead(&bCmd, 1);
}
#endif

#ifdef WINBOND_4BYTE_ADDRESS
static uint8_t winbond_ReadStatusReg3(void)
{
    uint8_t bCmd = 0;

    bCmd = CMD_W25_RDSR3;
    return winbond_SpiRead(&bCmd, 1);;
}
#endif

#ifdef WINBOND_QUAD_ENABLE
static void winbond_WriteStatusReg1(uint8_t bVal)
{
    uint8_t baBuff[3];

    baBuff[0] = CMD_W25_WRSR1; //write status cmd
    baBuff[1] = winbond_ReadStatusReg1();
    baBuff[2] = winbond_ReadStatusReg2();

    if (!(baBuff[2] & bVal))
    {
        baBuff[2] |= bVal;
        winbond_SpiWrite(baBuff, sizeof(baBuff));
    }
}
#endif

#ifdef WINBOND_4BYTE_ADDRESS
static void winbond_WriteStatusReg3(uint8_t bVal)
{
    uint8_t baBuff[2];

    baBuff[0] = CMD_W25_WRSR3; //write status cmd
    baBuff[1] = winbond_ReadStatusReg3();

    if (!(baBuff[1] & bVal))
    {
        baBuff[1] |= bVal;
        winbond_WriteEnable();
        winbond_SpiWrite(baBuff, sizeof(baBuff));
        WAIT_WINBOND_BUSY();
    }
}
#endif

/* static void winbond_ChipErase(void)
{
    uint8_t bCmd = 0;

    bCmd = CMD_W25_CE;
    winbond_SpiWrite(&bCmd, 1);
} */

/* static void winbond_BlockErase64(uint32_t dwAddr)
{
    uint8_t bCmdAddr[4] = {0};

    bCmdAddr[0] = CMD_W25_BE;
    bCmdAddr[1] = (dwAddr >> 16) & 0x0F;
    bCmdAddr[2] = (dwAddr >> 8) & 0xFF;
    bCmdAddr[3] = dwAddr & 0xFF;

    winbond_SpiWrite(bCmdAddr, sizeof(bCmdAddr));
}
 */

#ifdef WINBOND_4BYTE_ADDRESS
static void winbond_SectorErase4ByteAddr(uint32_t dwAddr)
{
    uint8_t bCmdAddr[5] = {0};

    bCmdAddr[0] = CMD_W25_SE4B;
    bCmdAddr[1] = (dwAddr >> 24) & 0xFF;
    bCmdAddr[2] = (dwAddr >> 16) & 0xFF;
    bCmdAddr[3] = (dwAddr >> 8) & 0xFF;
    bCmdAddr[4] = dwAddr & 0xFF;

    winbond_SpiWrite(bCmdAddr, sizeof(bCmdAddr));
}
#else
static void winbond_SectorErase(uint32_t dwAddr)
{
    uint8_t bCmdAddr[4] = {0};

    bCmdAddr[0] = CMD_W25_SE;
    bCmdAddr[1] = (dwAddr >> 16) & 0x0F;
    bCmdAddr[2] = (dwAddr >> 8) & 0xFF;
    bCmdAddr[3] = dwAddr & 0xFF;

    winbond_SpiWrite(bCmdAddr, sizeof(bCmdAddr));
}
#endif

#if defined (WINBOND_QUAD_ENABLE) && (WINBOND_4BYTE_ADDRESS)
static int winbond_PageProgramQuad4ByteAddr(uint32_t dwAddr, uint8_t *pbBuf, int iSize)
{
    uint8_t bCmd = 0;

    bCmd = CMD_W25_QIPP4B;
    return winbond_SpiWriteData4ByteAddr(&bCmd, dwAddr, pbBuf, iSize);
}

static void winbond_FastReadQuad4ByteAddr(uint32_t dwAddr, uint8_t *pbBuf, int iSize)
{
    uint8_t bCmd = 0;

    bCmd = CMD_W25_FRDQIO4B;
    winbond_SpiReadData4ByteAddr(&bCmd, dwAddr, pbBuf, iSize);
}
#elif WINBOND_QUAD_ENABLE
static void winbond_FastReadQuad(uint32_t dwAddr, uint8_t *pbBuf, int iSize)
{
    uint8_t bCmd = 0;

    bCmd = CMD_W25_FRDQIO;
    winbond_SpiReadData(&bCmd, dwAddr, pbBuf, iSize);
}

static int winbond_PageProgramQuad(uint32_t dwAddr, uint8_t *pbBuf, int iSize)
{
    uint8_t bCmd = 0;

    bCmd = CMD_W25_QIPP;
    return winbond_SpiWriteData(&bCmd, dwAddr, pbBuf, iSize);
}
#elif WINBOND_4BYTE_ADDRESS
static int winbond_PageProgram4ByteAddr(uint32_t dwAddr, uint8_t *pbBuf, int iSize)
{
    uint8_t bCmd = 0;

    bCmd = CMD_W25_PP4B;
    return winbond_SpiWriteData4ByteAddr(&bCmd, dwAddr, pbBuf, iSize);
}

static void winbond_ReadData4ByteAddr(uint32_t dwAddr, uint8_t *pbBuf, int iSize)
{
    uint8_t bCmd = 0;

    bCmd = CMD_W25_RD4B;
    winbond_SpiReadData4ByteAddr(&bCmd, dwAddr, pbBuf, iSize);
}
#else
static int winbond_PageProgram(uint32_t dwAddr, uint8_t *pbBuf, int iSize)
{
    uint8_t bCmd = 0;

    bCmd = CMD_W25_PP;
    return winbond_SpiWriteData(&bCmd, dwAddr, pbBuf, iSize);
}

static void winbond_ReadData(uint32_t dwAddr, uint8_t *pbBuf, int iSize)
{
    uint8_t bCmd = 0;

    bCmd = CMD_W25_RD;
    winbond_SpiReadData(&bCmd, dwAddr, pbBuf, iSize);
}
#endif


#ifdef WINBOND_QUAD_ENABLE
static void winbond_ReadDeviceIdQuad(uint8_t *pbManu, uint8_t *pbDevice)
{
    uint8_t bDeviceId[2] = {0};
    uint8_t bCmd = 0;

    bCmd = CMD_W25_RMDQIO;
    winbond_SpiReadData(&bCmd, 0, bDeviceId, sizeof(bDeviceId));

    *pbManu = bDeviceId[0];
    *pbDevice = bDeviceId[1];
}
#else
static void winbond_ReadDeviceId(uint8_t *pbManu, uint8_t *pbDevice)
{
    uint8_t bCmd = 0;
    uint8_t bDeviceId[2] = {0};

    bCmd = CMD_W25_RMD;
    winbond_SpiReadData(&bCmd, 0, bDeviceId, sizeof(bDeviceId));

    *pbManu = bDeviceId[0];
    *pbDevice = bDeviceId[1];
}
#endif

typedef struct {
    void   (*read)(uint32_t dwAddr, uint8_t *pbBuf, int iSize);
    int    (*write)(uint32_t dwAddr, uint8_t *pbBuf, int iSize);
    void   (*erase)(uint32_t dwAddr);
    void   (*read_id)(uint8_t *pbManu, uint8_t *pbDevice);
} S_FlashInterface;

#if defined (WINBOND_QUAD_ENABLE) && (WINBOND_4BYTE_ADDRESS)
static const S_FlashInterface  sWinbond = {
    winbond_FastReadQuad4ByteAddr,
    winbond_PageProgramQuad4ByteAddr,
    winbond_SectorErase4ByteAddr,
    winbond_ReadDeviceIdQuad
};
#elif WINBOND_QUAD_ENABLE
static const S_FlashInterface  sWinbond = {
    winbond_FastReadQuad,
    winbond_PageProgramQuad,
    winbond_SectorErase,
    winbond_ReadDeviceIdQuad
};
#elif WINBOND_4BYTE_ADDRESS
static const S_FlashInterface  sWinbond = {
    winbond_ReadData4ByteAddr,
    winbond_PageProgram4ByteAddr,
    winbond_SectorErase4ByteAddr,
    winbond_ReadDeviceId
};
#else
static const S_FlashInterface  sWinbond = {
    winbond_ReadData,
    winbond_PageProgram,
    winbond_SectorErase,
    winbond_ReadDeviceId
};
#endif

const S_FlashInterface *g_pFlash;

int winbond_Erase(uint32_t dwFlashAddr, uint32_t dwSectorCount)
{
    int i = 0;

    for (i=0; i<=dwSectorCount; i++)
    {
        winbond_WriteEnable();

        g_pFlash->erase(dwFlashAddr);
        WAIT_WINBOND_BUSY();
    }

    return CC_SUCCESS;
}

int winbond_Read(uint8_t *pBuff, uint32_t dwFlashAddr, uint32_t dwSize)
{
    int iIdx = 0;

    do
    {
        int iRxSize = 0;

        if (dwSize > WINBOND_BUFFER_SIZE)
            iRxSize += WINBOND_BUFFER_SIZE;
        else
            iRxSize += dwSize;

        g_pFlash->read(dwFlashAddr+iIdx, pBuff+iIdx, iRxSize);

        iIdx += iRxSize;
        dwSize -= iRxSize;
    } while(dwSize > 0);

    return CC_SUCCESS;
}

int winbond_Write(uint32_t dwFlashAddr, uint8_t *pBuff, uint32_t dwSize)
{
    int iIdx = 0;

    do
    {
        int iTxSize = 0;

        if (dwSize > WINBOND_BUFFER_SIZE)
            iTxSize += WINBOND_BUFFER_SIZE;
        else
            iTxSize += dwSize;

        winbond_WriteEnable();

        g_pFlash->write(dwFlashAddr+iIdx, pBuff+iIdx, iTxSize);
        WAIT_WINBOND_BUSY();

        iIdx += iTxSize;
        dwSize -= iTxSize;
    } while(dwSize > 0);

    return CC_SUCCESS;
}

int winbond_Init(void)
{
    uint8_t bManu = 0, bDevice = 0;

    g_pFlash = &sWinbond;

#ifdef WINBOND_QUAD_ENABLE
    drvi_SpimDummyClkSet(FLASH_IF_ID, 4);
    drvi_SpimDummyClkEnable(FLASH_IF_ID, 0);
    drvi_SpimQuadEnable(FLASH_IF_ID, 1);

    winbond_WriteEnable();
    winbond_WriteStatusReg1(REG_WRSR2_QE);
#endif

    g_pFlash->read_id(&bManu, &bDevice);

    TracerInfo("SPI Flash Manufacturer=0x%X, Device ID=0x%X...\n", bManu, bDevice);
    if ((bManu != WINBOND_MANUFACTURER) || (bDevice != WINBOND_DEVICE))
    {
        TracerInfo("SPI Flash error...\n");
        return CC_ERROR_NO_MEM;
    }

#ifdef WINBOND_4BYTE_ADDRESS
    winbond_WriteStatusReg3(REG_WRSR3_ADP);
#endif

    return 1;
}
