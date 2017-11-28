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
#include "test.h"

#if (TEST_I2C)

#include <stdio.h>
#include <string.h>

#include "drvi_i2c.h"
#include "tracer.h"

#define I2C_BUFFER_SIZE 64

#define MICROCHIP_24XX16_BLOCK_NUM 8
#define MICROCHIP_24XX16_BLOCK_SIZE 256
#define MICROCHIP_24XX16_PAGE_SIZE 16
#define MICROCHIP_24XX16_MAX_WRITE_SIZE 8

/******************************************************************************
Declaration of External Variables & Functions
******************************************************************************/

/******************************************************************************
Declaration of data structure
******************************************************************************/
typedef struct S_I2CTxBuffer
{
    uint8_t bAddr;
    uint8_t baBuffer[I2C_BUFFER_SIZE];
} T_I2CTxBuffer;

/******************************************************************************
Declaration of Global Variables & Functions
******************************************************************************/

/******************************************************************************
Declaration of static Global Variables & Functions
******************************************************************************/
__align(4) static T_I2CTxBuffer g_tI2c0TxBuffer = {0, {0}};
__align(4) static uint8_t g_baI2c0RxBuffer[I2C_BUFFER_SIZE] = {0};

int microchip_24xx16_write(uint8_t bAddr, uint8_t *pbBuf, int iSize)
{
    int iWSize = 0;
    uint8_t bWAddr = bAddr;
    int iRet = CC_SUCCESS;

    if (iSize > I2C_BUFFER_SIZE)
        return CC_ERROR_NO_MEM;

    memcpy(&g_tI2c0TxBuffer.baBuffer, pbBuf, iSize);

    do
    {
        if (iSize > MICROCHIP_24XX16_MAX_WRITE_SIZE)
            iWSize = MICROCHIP_24XX16_MAX_WRITE_SIZE;
        else
            iWSize = iSize;

        g_tI2c0TxBuffer.bAddr = bWAddr & 0xFF;

        iRet = drvi_I2cWrite(TEST_I2C_BUS, (uint8_t const *) &g_tI2c0TxBuffer, iWSize+sizeof(g_tI2c0TxBuffer.bAddr));
        if (CC_SUCCESS != iRet)
            continue;

        iSize -= iWSize;
        bWAddr += iWSize;
    } while(iSize>0);

    return iRet;
}

void microchip_24xx16_read(uint8_t bAddr, uint8_t *pbBuf, int iSize)
{
    int iRet = CC_SUCCESS;

    do
    {
        iRet = drvi_I2cWrite(TEST_I2C_BUS, &bAddr, sizeof(bAddr));
    } while(CC_SUCCESS != iRet);

    do
    {
        iRet = drvi_I2cRead(TEST_I2C_BUS, pbBuf, iSize);
    } while(CC_SUCCESS != iRet);
}


int TEST_I2cInit(T_I2cDevice *tConfig)
{
    drvi_I2cDeviceRegister(tConfig);

    return CC_SUCCESS;
}

void TEST_I2cRW(uint32_t dwCount)
{
    uint32_t dwIndex = 0;
    int iBlock = 0;
    int iSize = 0;
    uint8_t bAddr = 0;

    T_I2cDevice i2c_config = {
       .bBusNum          = TEST_I2C_BUS,
       .bAddr            = TEST_I2C_ADDRESS,
    };

    TracerInfo("I2C RW test...\r\n");

    while(dwIndex<dwCount)
    {
        if (iBlock >= MICROCHIP_24XX16_BLOCK_NUM)
            iBlock = 0;

        if (iSize >= I2C_BUFFER_SIZE)
            iSize = 0;

        i2c_config.bAddr = (TEST_I2C_ADDRESS | iBlock++);
        TEST_I2cInit(&i2c_config);

        bAddr = 0x00;
        iSize += MICROCHIP_24XX16_PAGE_SIZE;

        //memset(g_tI2c0TxBuffer, 0xFF, 16);
        //microchip_24xx16_write(&i2c, cAddr, g_tI2c0TxBuffer, 16);

        memset(g_tI2c0TxBuffer.baBuffer, (dwIndex&(0xFF)), sizeof(g_tI2c0TxBuffer.baBuffer));

//        memset(g_tI2c0TxBuffer.baBuffer, 0x11, 4);
//        memset(&g_tI2c0TxBuffer.baBuffer[4], 0xAA, 4);
//        memset(&g_tI2c0TxBuffer.baBuffer[8], 0x22, 4);
//        memset(&g_tI2c0TxBuffer.baBuffer[12], 0xBB, 4);
//        memset(&g_tI2c0TxBuffer.baBuffer[16], 0x33, 4);
//        memset(&g_tI2c0TxBuffer.baBuffer[20], 0xCC, 4);
//        memset(&g_tI2c0TxBuffer.baBuffer[24], 0x44, 4);
//        memset(&g_tI2c0TxBuffer.baBuffer[28], 0xDD, 4);
//        memset(&g_tI2c0TxBuffer.baBuffer[32], 0x55, 4);
//        memset(&g_tI2c0TxBuffer.baBuffer[36], 0xEE, 4);
//        memset(&g_tI2c0TxBuffer.baBuffer[40], 0x66, 4);
//        memset(&g_tI2c0TxBuffer.baBuffer[44], 0xFF, 4);
//        memset(&g_tI2c0TxBuffer.baBuffer[48], 0x77, 4);
//        memset(&g_tI2c0TxBuffer.baBuffer[52], 0x88, 4);
//        memset(&g_tI2c0TxBuffer.baBuffer[56], 0x00, 4);
//        memset(&g_tI2c0TxBuffer.baBuffer[60], 0x99, 4);

        microchip_24xx16_write(bAddr, g_tI2c0TxBuffer.baBuffer, iSize);

        microchip_24xx16_read(bAddr, g_baI2c0RxBuffer, iSize);

        if (memcmp(g_tI2c0TxBuffer.baBuffer, g_baI2c0RxBuffer, iSize))
            printf("I2C RW test Fail, w[0x%X],r[0x%X],s[%d]\r\n", g_tI2c0TxBuffer.baBuffer[0], g_baI2c0RxBuffer[0], iSize);

        dwIndex++;
    }

    TracerInfo("I2C RW test Done...\r\n");
}

#endif //TEST_I2C
