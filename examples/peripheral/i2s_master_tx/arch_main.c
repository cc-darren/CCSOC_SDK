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

#define SAMPLE_CLOUDCHIP    (1)

#ifdef SAMPLE_CLOUDCHIP
#include "cloudchip_16bit_16khz.h"
#else
#include "sinwave1k_16bit_48khz.h"
#endif

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

#ifdef SAMPLE_CLOUDCHIP
    tPcmParams.iFormat = DAI_PCM_FORMAT_S16_LE;
    tPcmParams.iRate = DAI_HW_PARAM_RATE_16K;
    drvi_I2sHwParams(&tPcmParams);
#else
    tPcmParams.iFormat = DAI_PCM_FORMAT_S16_LE;
    tPcmParams.iRate = DAI_HW_PARAM_RATE_48K;
    drvi_I2sHwParams(&tPcmParams);
#endif

    drvi_I2sSetFormat(DAI_FORMAT_I2S | DAI_CODEC_SLAVE);

    drvi_I2sTxConfig((uint32_t)g_bAudioSample, (uint32_t)(g_bAudioSample+sizeof(g_bAudioSample)-1));
    drvi_I2sStart(1, 1);

    while(1)
    {
    }
}

