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

#include "drvi_spi.h"

#if (defined(SPI_INUSE) && (SPI_INUSE))
int drvi_SpiInit(void)
{
    T_SpiDevice tSpiDev;
    int32_t iResult = 0;

    #if (defined(SPI0_INUSE) && (SPI0_INUSE))
        tSpiDev.bBusNum = 0;
        tSpiDev.wMode = SPIM0_CONFIG;
        tSpiDev.dwMaxSpeedHz = SPIM0_CLOCK;
        iResult = cc6801_SpimInit(&tSpiDev);
        //Todo: When RTOS is enabled, interrupt must be disabled during initialization.
        //      So, tracer function can't be enabled.
        //      We need to implement UART direct output function
        //if (iResult)
        //    TracerErr("SPI0 initial error\n");
    #endif

    #if (defined(SPI1_INUSE) && (SPI1_INUSE))
        tSpiDev.bBusNum = 1;
        tSpiDev.wMode = SPIM1_CONFIG;
        tSpiDev.dwMaxSpeedHz = SPIM1_CLOCK;
        iResult = cc6801_SpimInit(&tSpiDev);
        //Todo: When RTOS is enabled, interrupt must be disabled during initialization.
        //      So, tracer function can't be enabled.
        //      We need to implement UART direct output function
        //if (iResult)
        //    TracerErr("SPI1 initial error\n");
    #endif

    #if (defined(SPI2_INUSE) && (SPI2_INUSE))
        tSpiDev.bBusNum = 2;
        tSpiDev.wMode = SPIM2_CONFIG;
        tSpiDev.dwMaxSpeedHz = SPIM2_CLOCK;
        iResult = cc6801_SpimInit(&tSpiDev);
        //Todo: When RTOS is enabled, interrupt must be disabled during initialization.
        //      So, tracer function can't be enabled.
        //      We need to implement UART direct output function
        //if (iResult)
        //    TracerErr("SPI2 initial error\n");
    #endif

    return iResult;
}
#endif
