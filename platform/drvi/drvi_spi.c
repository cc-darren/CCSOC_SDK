/**
 ****************************************************************************************
 *
 * @file drvi_spi.c
 *
 * @brief HW SPI interface for all SOC.
 *
 * Copyright (C) CloudChip 2017-2019
 *
 *
 ****************************************************************************************
 */
#include "drvi_spi.h"
#include "tracer.h"

int drvi_SpiInit(void)
{
    T_SpiDevice tSpiDev;
    int iResult = 0;

    #if defined SPI0_INUSE && (SPI0_INUSE)
    tSpiDev.bBusNum = 0;
    tSpiDev.wMode = SPIM0_CONFIG;
    tSpiDev.dwMaxSpeedHz = 8000000;
    iResult = cc6801_SpimInit(&tSpiDev);
    if (iResult)
        TracerErr("SPI0 initial error\n");
    #endif
    #if defined SPI1_INUSE && (SPI1_INUSE)
    tSpiDev.bBusNum = 1;
    tSpiDev.wMode = SPIM1_CONFIG;
    tSpiDev.dwMaxSpeedHz = 80000000;
    iResult = cc6801_SpimInit(&tSpiDev);
    if (iResult)
        TracerErr("SPI1 initial error\n");
    #endif
    #if defined SPI2_INUSE && (SPI2_INUSE)
    tSpiDev.bBusNum = 2;
    tSpiDev.wMode = SPIM2_CONFIG;
    tSpiDev.dwMaxSpeedHz = 8000000;
    iResult = cc6801_SpimInit(&tSpiDev);
    if (iResult)
        TracerErr("SPI2 initial error\n");
    #endif

    return iResult;
}

