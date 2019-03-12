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

#if (TEST_SPI)

#include <stdio.h>
#include <string.h>

#include "drvi_spi.h"
#include "tracer.h"
#include "flash_winbond.h"
#include "drvi_wktm.h"

#define SPI_BUFFER_SIZE 128

/******************************************************************************
Declaration of External Variables & Functions
******************************************************************************/

/******************************************************************************
Declaration of data structure
******************************************************************************/

/******************************************************************************
Declaration of Global Variables & Functions
******************************************************************************/

/******************************************************************************
Declaration of static Global Variables & Functions
******************************************************************************/
__align(4) static uint8_t g_baSpi0TxBuffer[SPI_BUFFER_SIZE] = {0};
__align(4) static uint8_t g_baSpi0RxBuffer[SPI_BUFFER_SIZE] = {0};

int TEST_SpiInit(void)
{
    return winbond_Init();
}

//#define SPI_PERFORMANCE_CHECK   1

void TEST_SpiRW(uint32_t dwCount)
{
    uint32_t dwIndex = 0;
    uint32_t dwAddr = 0;
    int iError = CC_SUCCESS;
    int iBlock = 0;
    int iSize = 0;

    iError = TEST_SpiInit();
    if (!iError)
        return;

  #if defined(WINBOND_QUAD_ENABLE) && WINBOND_QUAD_ENABLE
    TracerInfo("Q");
  #endif
    TracerInfo("SPI RW test...\n");
    
    while(dwIndex<dwCount)
    {
        memset(g_baSpi0TxBuffer, (dwIndex&(0xFF)), sizeof(g_baSpi0TxBuffer));

        if (iBlock >= WINBOND_BLOCK_NUM)
            iBlock = 0;

        dwAddr = iBlock++*WINBOND_BLOCK_SIZE;
        iSize = SPI_BUFFER_SIZE - (dwIndex&(SPI_BUFFER_SIZE-1));

        winbond_Write(dwAddr, g_baSpi0TxBuffer, iSize);
        winbond_Read(g_baSpi0RxBuffer, dwAddr, iSize);
        winbond_Erase(dwAddr, iSize/WINBOND_SECTOR_SIZE);

        if (memcmp(g_baSpi0TxBuffer, g_baSpi0RxBuffer, iSize))
        {
          #if defined(WINBOND_QUAD_ENABLE) && WINBOND_QUAD_ENABLE
            TracerInfo("Q");
          #endif
            TracerInfo("SPI RW test Fail, addr[0x%X], w[0x%X],r[0x%X],s[%d]\r\n", dwAddr, g_baSpi0TxBuffer[0], g_baSpi0RxBuffer[0], iSize);
        }

        winbond_Read(g_baSpi0RxBuffer, dwAddr, iSize);
        dwIndex++;
    }

  #if defined(WINBOND_QUAD_ENABLE) && WINBOND_QUAD_ENABLE
    TracerInfo("Q");
  #endif
    TracerInfo("SPI RW test Done...\n");
 



  #if defined(SPI_PERFORMANCE_CHECK) && SPI_PERFORMANCE_CHECK
    uint32_t counterUsed;
    uint32_t counterS;
    uint32_t counterE;

//****************************************************************************************
//Start write performance check
    drvi_wktmInit();
    drvi_wktmStart(WKTM_0);
  #if defined(WINBOND_QUAD_ENABLE) && WINBOND_QUAD_ENABLE
    TracerInfo("Q");
  #endif
    TracerInfo("SPI Write Performance check...\n");
    dwIndex = 0;
    iBlock = 0;

    drvi_wktmCounterGet(WKTM_0, &counterS);
    while(dwIndex<dwCount)
    {
        //memset(g_baSpi0TxBuffer, (dwIndex&(0xFF)), sizeof(g_baSpi0TxBuffer));

        if (iBlock >= WINBOND_BLOCK_NUM)
            iBlock = 0;

        dwAddr = iBlock++*WINBOND_BLOCK_SIZE;
        iSize = SPI_BUFFER_SIZE - (dwIndex&(SPI_BUFFER_SIZE-1));

        winbond_Write(dwAddr, g_baSpi0TxBuffer, iSize);
        dwIndex++;
    }

    drvi_wktmCounterGet(WKTM_0, &counterE);
    counterUsed = counterE - counterS;
  #if defined(WINBOND_QUAD_ENABLE) && WINBOND_QUAD_ENABLE
    TracerInfo("Q");
  #endif
    TracerInfo("SPI Write Performance check Done... ... %d ticks of 32K\n", counterUsed);

//****************************************************************************************
//Start read performance check
    drvi_wktmInit();
    drvi_wktmStart(WKTM_0);
  #if defined(WINBOND_QUAD_ENABLE) && WINBOND_QUAD_ENABLE
    TracerInfo("Q");
  #endif
    TracerInfo("SPI Read Performance check...\n");
    dwIndex = 0;
    iBlock = 0;

    drvi_wktmCounterGet(WKTM_0, &counterS);
    while(dwIndex<dwCount)
    {
        //memset(g_baSpi0TxBuffer, (dwIndex&(0xFF)), sizeof(g_baSpi0TxBuffer));

        if (iBlock >= WINBOND_BLOCK_NUM)
            iBlock = 0;

        dwAddr = iBlock++*WINBOND_BLOCK_SIZE;
        iSize = SPI_BUFFER_SIZE - (dwIndex&(SPI_BUFFER_SIZE-1));

        winbond_Read(g_baSpi0RxBuffer, dwAddr, iSize);
        dwIndex++;
    }

    drvi_wktmCounterGet(WKTM_0, &counterE);
    counterUsed = counterE - counterS;
  #if defined(WINBOND_QUAD_ENABLE) && WINBOND_QUAD_ENABLE
    TracerInfo("Q");
  #endif
    TracerInfo("SPI Read Performance check Done... ... %d ticks of 32K\n", counterUsed);

//****************************************************************************************
//Start erase performance check
    drvi_wktmInit();
    drvi_wktmStart(WKTM_0);
  #if defined(WINBOND_QUAD_ENABLE) && WINBOND_QUAD_ENABLE
    TracerInfo("Q");
  #endif
    TracerInfo("SPI Erase Performance check...\n");
    dwIndex = 0;
    iBlock = 0;

    drvi_wktmCounterGet(WKTM_0, &counterS);
    while(dwIndex<dwCount)
    {
        //memset(g_baSpi0TxBuffer, (dwIndex&(0xFF)), sizeof(g_baSpi0TxBuffer));

        if (iBlock >= WINBOND_BLOCK_NUM)
            iBlock = 0;

        dwAddr = iBlock++*WINBOND_BLOCK_SIZE;
        iSize = SPI_BUFFER_SIZE - (dwIndex&(SPI_BUFFER_SIZE-1));

        winbond_Erase(dwAddr, iSize/WINBOND_SECTOR_SIZE);
        dwIndex++;
    }

    drvi_wktmCounterGet(WKTM_0, &counterE);
    counterUsed = counterE - counterS;
  #if defined(WINBOND_QUAD_ENABLE) && WINBOND_QUAD_ENABLE
    TracerInfo("Q");
  #endif
    TracerInfo("SPI Erase Performance check Done... ... %d ticks of 32K\n", counterUsed);
    
    drvi_wktmStop(WKTM_0);
    drvi_wktmCounterClear(WKTM_0);
  #endif
    
}

#endif //TEST_SPI
