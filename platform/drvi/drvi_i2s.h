/**
 ****************************************************************************************
 *
 * @file drvi_i2s.h
 *
 * @brief Head file of drvi_i2s.c
 *
 * Copyright (C) CloudChip 2017-2019
 *
 *
 ****************************************************************************************
 */
#ifndef _DRVI_I2S_H_
#define _DRVI_I2S_H_
#include "project.h"
#include "i2s.h"

#define	DAI_PCM_FORMAT_S8       (0)
#define	DAI_PCM_FORMAT_U8	    (1)
#define	DAI_PCM_FORMAT_S16_LE	(2)
#define	DAI_PCM_FORMAT_S16_BE	(3)
#define	DAI_PCM_FORMAT_U16_LE	(4)
#define	DAI_PCM_FORMAT_U16_BE	(5)
#define	DAI_PCM_FORMAT_S24_LE	(6)
#define	DAI_PCM_FORMAT_S24_BE	(7)
#define	DAI_PCM_FORMAT_U24_LE	(8)
#define	DAI_PCM_FORMAT_U24_BE	(9)
#define	DAI_PCM_FORMAT_S20_3LE	(10)
#define	DAI_PCM_FORMAT_S20_3BE	(11)
#define	DAI_PCM_FORMAT_S32_LE	(12)
#define	DAI_PCM_FORMAT_S32_BE	(13)

#define DAI_HW_PARAM_RATE_16K   (16000)
#define DAI_HW_PARAM_RATE_32K   (32000)
#define DAI_HW_PARAM_RATE_48K   (48000)
#define DAI_HW_PARAM_RATE_96K   (96000)

#define DAI_FORMAT_I2S          1 /* I2S mode */
#define DAI_FORMAT_RIGHT_J      2 /* Right Justified mode */
#define DAI_FORMAT_LEFT_J       3 /* Left Justified mode */

#define DAI_CODEC_MASTER        (1 << 12) /* codec clk master*/
#define DAI_CODEC_SLAVE         (2 << 12) /* codec clk slave*/

#define DAI_FORMAT_MASK	        0x000F
#define DAI_MASTER_MASK	        0xF000

typedef struct S_DaiHwParams
{
    int iFormat;
    int iRate;
} T_DaiHwParams;

__forceinline void drvi_I2sInit(T_callback handler)
{
    cc6801_I2sInit(handler);
}

__forceinline int drvi_I2sSetFormat(uint16_t wFmt)
{
    return cc6801_I2sSetFormat(wFmt);
}

__forceinline int drvi_I2sHwParams(T_DaiHwParams *tpParams)
{
    return cc6801_I2sHwParams(tpParams);
}

__forceinline void drvi_I2sTxConfig(uint32_t dwDmaBufStart, uint32_t dwDmaBufEnd)
{
    cc6801_I2sTxConfig(dwDmaBufStart, dwDmaBufEnd);
}

__forceinline void drvi_I2sRxConfig(uint32_t dwDmaBufStart, uint32_t dwDmaBufEnd)
{
    cc6801_I2sRxConfig(dwDmaBufStart, dwDmaBufEnd);
}

__forceinline void drvi_I2sStart(uint8_t bTxByte, uint8_t bRxByte)
{
    cc6801_I2sStart(bTxByte, bRxByte);
}

__forceinline void drvi_I2sStop(void)
{
    cc6801_I2sStop();
}

#endif //_DRVI_I2S_H_
