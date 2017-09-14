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

    #if defined _SPI0_INUSE_ && (_SPI0_INUSE_)
    tSpiDev.bBusNum = 0;
    tSpiDev.wMode = SPIM0_CONFIG;
    tSpiDev.dwMaxSpeedHz = 8000000;
    iResult = cc6801_SpimInit(&tSpiDev);
    if (iResult)
        TracerErr("SPI0 initial error\n");
    #endif
    #if defined _SPI1_INUSE_ && (_SPI1_INUSE_)
    tSpiDev.bBusNum = 1;
    tSpiDev.wMode = SPIM1_CONFIG;
    tSpiDev.dwMaxSpeedHz = 80000000;
    iResult = cc6801_SpimInit(&tSpiDev);
    if (iResult)
        TracerErr("SPI1 initial error\n");
    #endif
    #if defined _SPI2_INUSE_ && (_SPI2_INUSE_)
    tSpiDev.bBusNum = 2;
    tSpiDev.wMode = SPIM2_CONFIG;
    tSpiDev.dwMaxSpeedHz = 8000000;
    iResult = cc6801_SpimInit(&tSpiDev);
    if (iResult)
        TracerErr("SPI2 initial error\n");
    #endif

    return iResult;
}

