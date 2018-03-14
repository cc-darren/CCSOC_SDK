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

/*
 * INCLUDES
 ****************************************************************************************
 */
#include <stdio.h>
#include <string.h>

#include "ll.h"
#include "drvi_init.h"
#include "drvi_i2s.h"
#include "tracer.h"
#include "project.h"

#define RX_BUFFER_SIZE 64
__align(16) static uint8_t g_bAudioSample[RX_BUFFER_SIZE] = {0};

extern void sys_InitMain(void);

/*
 * MAIN FUNCTION
 ****************************************************************************************
 */

int main(void)
{
    T_DaiHwParams tPcmParams;

    //Must be first in main()
    sys_InitMain();

    //start interrupt handling
    GLOBAL_INT_START();

    drvi_initialize();

    TracerInfo("== CC6801 Start ==\r\n");

    /*******************************/
    /****** Application Start ******/
    /*******************************/

    drvi_I2sInit((T_callback)NULL);

    tPcmParams.iFormat = DAI_PCM_FORMAT_S24_BE;
    tPcmParams.iRate = DAI_HW_PARAM_RATE_32K;
    drvi_I2sHwParams(&tPcmParams);

    drvi_I2sSetFormat(DAI_FORMAT_I2S | DAI_CODEC_MASTER);

    drvi_I2sRxConfig((uint32_t)g_bAudioSample, (uint32_t)(g_bAudioSample+sizeof(g_bAudioSample)-1));
    drvi_I2sStart(1, RX_BUFFER_SIZE/2);

    while(1)
    {
    }
}

