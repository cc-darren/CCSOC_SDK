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

#include "drvi_spi.h"

#ifdef FREERTOS
#include "FreeRTOS.h"
#include "semphr.h"
#if defined(SEMAPHORE_H)
    #if defined(SPI1_INUSE) && (SPI1_INUSE)
        SemaphoreHandle_t g_xSemaphoreSpim0 = NULL;
    #endif

    #if defined(SPI1_INUSE) && (SPI1_INUSE)
        SemaphoreHandle_t g_xSemaphoreSpim1 = NULL;
    #endif

    #if(defined SPI2_INUSE) && (SPI2_INUSE)
        SemaphoreHandle_t g_xSemaphoreSpim2 = NULL;
    #endif
#endif
#endif

#if defined SPI_INUSE && (SPI_INUSE)
int drvi_SpiInit(void)
{
    T_SpiDevice tSpiDev;
    int iResult = 0;

    #if defined SPI0_INUSE && (SPI0_INUSE)
    tSpiDev.bBusNum = 0;
    tSpiDev.wMode = SPIM0_CONFIG;
    tSpiDev.dwMaxSpeedHz = SPIM0_CLOCK;
    iResult = cc6801_SpimInit(&tSpiDev);

    #if defined(SEMAPHORE_H)
    {
        g_xSemaphoreSpim0 = xSemaphoreCreateBinary();
    }
    #endif
    #endif

    #if defined SPI1_INUSE && (SPI1_INUSE)
    tSpiDev.bBusNum = 1;
    tSpiDev.wMode = SPIM1_CONFIG;
    tSpiDev.dwMaxSpeedHz = SPIM1_CLOCK;
    iResult = cc6801_SpimInit(&tSpiDev);

    #if defined(SEMAPHORE_H)
    {
        g_xSemaphoreSpim1 = xSemaphoreCreateBinary();
    }
    #endif
    #endif

    #if defined SPI2_INUSE && (SPI2_INUSE)
    tSpiDev.bBusNum = 2;
    tSpiDev.wMode = SPIM2_CONFIG;
    tSpiDev.dwMaxSpeedHz = SPIM2_CLOCK;
    iResult = cc6801_SpimInit(&tSpiDev);

    #if defined(SEMAPHORE_H)
    {
        g_xSemaphoreSpim2 = xSemaphoreCreateBinary();
    }
    #endif
    #endif

    return iResult;
}
#endif
