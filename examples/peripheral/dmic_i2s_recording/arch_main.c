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

/*
 * INCLUDES
 ****************************************************************************************
 */
#include <string.h>
#include "ll.h"
#include "drvi_clock.h"
#include "drvi_init.h"
#include "drvi_gpio.h"
#include "drvi_i2c.h"
#include "drvi_i2s.h"
#include "tracer.h"
#include "project.h"
#include "drvi_clock.h"

#define I2S_CHUNK_COUNT 128
#define I2S_CHUNK_SIZE  512

T_I2cDevice g_tSgtl5000Dev =
{
    .bBusNum = AUDIO_IF_ID,
    .bAddr = SGTL5000_SLAVE_ADDRESS,
};

extern void sys_InitMain(void);
extern volatile uint32_t g_dwDMIC_INTR;

__align(32) unsigned char g_bAudioSample[I2S_CHUNK_COUNT][I2S_CHUNK_SIZE];
__align(32) unsigned char g_baDmicLChannel[256];
__align(32) unsigned char g_baDmicRChannel[256];

static volatile uint32_t g_dwMainKey_INTR = 0;
static uint16_t  gwChuckIndex = 0;

static uint32_t *pLChannel = (uint32_t *)g_baDmicLChannel;
static uint32_t *pRChannel = (uint32_t *)g_baDmicRChannel;
static uint64_t *pLdest = (uint64_t *)g_bAudioSample;
static uint64_t *pRdest = (uint64_t *)&g_bAudioSample[0][4];

/*
 * MAIN FUNCTION
 ****************************************************************************************
 */

int I2cWordRead(uint16_t wReg, uint16_t *wVal)
{
    int iRet;
    uint8_t bBuf[2];

    //send i2c device address first
    bBuf[0] = (wReg >> 8) & 0xFF;
    bBuf[1] = wReg & 0xFF;
    iRet = drvi_I2cWrite(&g_tSgtl5000Dev, bBuf, sizeof(bBuf));
    if (iRet)
        return iRet;

    //Read data from i2c
    iRet = drvi_I2cRead(&g_tSgtl5000Dev, bBuf, sizeof(bBuf));
    if (iRet != CC_SUCCESS)
        return CC_ERROR_INTERNAL;

    *wVal = ((bBuf[0] << 8) | bBuf[1]);

    return iRet;
}

int I2cWordWrite(uint16_t wReg, uint16_t wVal)
{
    uint8_t bBuf[4];
    int iRet;

    bBuf[0] = (wReg >> 8) & 0xFF;
    bBuf[1] = wReg & 0xFF;
    bBuf[2] = (wVal >> 8) & 0xFF;
    bBuf[3] = wVal & 0xFF;

    iRet = drvi_I2cWrite(&g_tSgtl5000Dev, (uint8_t *)bBuf, sizeof(bBuf));
    if (iRet != CC_SUCCESS)
        return CC_ERROR_INTERNAL;

    return iRet;
}

void mainkey_isr(void)
{
    g_dwMainKey_INTR = 1;
}

void DmicInit(void)
{
    uint32_t dwSwRst = 0;

    rd(0x40001418, dwSwRst);
    dwSwRst &= ~(0x00000800);
    wr(0x40001418, dwSwRst);
    
    drvi_ClockDelayMs(1);

    dwSwRst |= 0x00000800;
    wr(0x40001418, dwSwRst);

    wr(0x40000a2c,0x440EB256);
    wr(0x40000a30,0x28301FA3);
    wr(0x40000a34,0x98981C8B); 
    wr(0x40000a38,0x00060B00);

    wr(0x40000a3c,0x657dd5c5);
    wr(0x40000a40,0x6377d25d);
    wr(0x40000a44,0xb955cf89);
    wr(0x40000a48,0xdd245019);
		
    //memset((void *) 0x20008600, 0, 0xA00); 
    //dma
    wr(0x40000108,0x30002020);
    wr(0x40000a00,0x00000001);
    wr(0x40000a04,0x00000101);
    wr(0x40000a08,0x000000ff);
    wr(0x40000a0c, (uint32_t)g_baDmicRChannel);
    wr(0x40000a10, (uint32_t)&g_baDmicRChannel[256]);
    wr(0x40000a14, (uint32_t)g_baDmicLChannel);
    wr(0x40000a18, (uint32_t)&g_baDmicLChannel[256]);
    wr(0x40000a1c,0x20000001);

    NVIC_EnableIRQ(DMIC_IRQn);
}

void CodecInit(void)
{
    int ret;
    uint16_t wId;

    ret = I2cWordRead(0x0000, &wId);
    if (ret)
        TracerInfo("Error reading chip id %d\r\n", ret);

    TracerInfo("Audio Codec SGTL5000 Chip ID 0x%X\r\n", wId);

    ret = I2cWordWrite(0x0004, 0x0011);
    if (ret)
        TracerInfo("Error %d, write 0x0004\r\n", ret);

    ret = I2cWordWrite(0x0030, 0x40FC);
    if (ret)
        TracerInfo("Error %d, write 0x0030\r\n", ret);

    ret = I2cWordWrite(0x0026, 0x0060);
    if (ret)
        TracerInfo("Error %d, write 0x0026\r\n", ret);

    ret = I2cWordWrite(0x003C, 0x0000);
    if (ret)
        TracerInfo("Error %d, write 0x003C\r\n", ret);

    ret = I2cWordWrite(0x000A, 0x0010);
    if (ret)
        TracerInfo("Error %d, write 0x000A\r\n", ret);

    ret = I2cWordWrite(0x0002, 0x0021);
    if (ret)
        TracerInfo("Error %d, write 0x0002\r\n", ret);

    ret = I2cWordWrite(0x000E, 0x0000);
    if (ret)
        TracerInfo("Error %d, write 0x000E\r\n", ret);

    ret = I2cWordWrite(0x0014, 0x015F);
    if (ret)
        TracerInfo("Error %d, write 0x0014\r\n", ret);

    ret = I2cWordWrite(0x0024, 0x0101);
    if (ret)
        TracerInfo("Error %d, write 0x0024\r\n", ret);

    ret = I2cWordWrite(0x0100, 0x0000);
    if (ret)
        TracerInfo("Error %d, write 0x0100\r\n", ret);

    ret = I2cWordWrite(0x0006, 0x00B0);
    if (ret)
        TracerInfo("Error %d, write 0x0006\r\n", ret);

    ret = I2cWordWrite(0x0010, 0x3C3C);
    if (ret)
        TracerInfo("Error %d, write 0x0010\r\n", ret);

    ret = I2cWordWrite(0x0022, 0x3A3A);
    if (ret)
        TracerInfo("Error %d, write 0x0022\r\n", ret);
}



int main(void)
{
    T_DaiHwParams tPcmParams;

    uint32_t dwCtrlA1 = 0;
    uint32_t dwMainKey = 0;
    uint32_t dwStartMoveData = 1;
    
    uint32_t dwDebounce = 10;
    
    //Must be first in main()
    sys_InitMain();

    //start interrupt handling
    GLOBAL_INT_START(); 

    drvi_initialize();

    /*******************************/
    /****** Application Start ******/
    /*******************************/

    drvi_RequestIrq(GPIO_PIN_5, mainkey_isr, IRQ_TYPE_EDGE_FALLING);

    DmicInit();
    CodecInit();
    drvi_I2sInit((T_callback)NULL);

    tPcmParams.iFormat = DAI_PCM_FORMAT_S16_LE;
    tPcmParams.iRate = DAI_HW_PARAM_RATE_16K;
    drvi_I2sHwParams(&tPcmParams);

    drvi_I2sSetFormat(DAI_FORMAT_I2S | DAI_CODEC_MASTER);
    drvi_I2sTxConfig((uint32_t)g_bAudioSample, (uint32_t)&g_bAudioSample[I2S_CHUNK_COUNT-1][I2S_CHUNK_SIZE-1]);
    drvi_I2sStart(1, 1);

    drvi_EnableIrq(GPIO_PIN_5);

    while(1)
    {
        int i = 0;

        dwMainKey = drvi_GpioRead(GPIO_PIN_5);

        if (dwMainKey && dwStartMoveData)
        {
            dwDebounce = 10;
            gwChuckIndex = 0;
            //Close DMIC
            rd(0x40000a30, dwCtrlA1);
            dwCtrlA1 &= ~(0x03000000);
            wr(0x40000a30, dwCtrlA1);
            //Key release, stop move data
            dwStartMoveData = 0;
            //Start play audio
            drvi_I2sStart(1, 1);
        }

        if (g_dwMainKey_INTR && !dwMainKey)
        {
            dwDebounce--;
        }

        if (g_dwMainKey_INTR && !dwDebounce)
        {
            //Key press
            g_dwMainKey_INTR = 0;
            //Stop play audio
            drvi_I2sStop();
            //Clean Buffer
            memset(g_bAudioSample, 0, sizeof(g_bAudioSample));
            //Open DMIC
            rd(0x40000a30, dwCtrlA1);
            dwCtrlA1 |= (0x02000000);
            wr(0x40000a30, dwCtrlA1);
            //Start move data
            dwStartMoveData = 1;
        }

        if (dwStartMoveData)
        {
            while(!g_dwDMIC_INTR);
            g_dwDMIC_INTR = 0;

            pLdest = (uint64_t *)&g_bAudioSample[gwChuckIndex&(I2S_CHUNK_COUNT-1)][0];
            pRdest = (uint64_t *)&g_bAudioSample[gwChuckIndex&(I2S_CHUNK_COUNT-1)][4];

            for (i=0; i<(sizeof(g_baDmicLChannel)/sizeof(&g_baDmicLChannel[0])); i++)
            {
                *pLdest++ = *(pLChannel+i);
                *pRdest++ = *(pRChannel+i);
            }

            rd(0x40000a30, dwCtrlA1);
            dwCtrlA1 |= (0x02000000);
            wr(0x40000a30, dwCtrlA1);
            gwChuckIndex++;
        }
    }
}

