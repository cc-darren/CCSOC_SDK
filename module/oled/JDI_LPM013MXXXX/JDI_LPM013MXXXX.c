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
#if (MODULE_OLED == OLED_JDI_LPM013M126A) || (MODULE_OLED == OLED_JDI_LPM010M297B)

#include <string.h>
#include <stdio.h>
#include "LCDConf.h"
#include "JDI_LPM013MXXXX.h"
#include "gpio.h"
#include "drvi_gpio.h"
#include "drvi_spi.h"
#include "drvi_pwm.h"
#include "tracer.h"


#define JDILCD_DELAY(x) delayns(x*1000)

#define SPI_PORT                 2
#define LCD_DISP_CLR            cc6801_GpioWrite(27,CC6801_GPIO_OUTPUT_LOW)
#define LCD_DISP_SET            cc6801_GpioWrite(27,CC6801_GPIO_OUTPUT_HIGH)
#define LCD_EXTCOM_CLR          cc6801_GpioWrite(26,CC6801_GPIO_OUTPUT_LOW)
#define LCD_EXTCOM_SET          cc6801_GpioWrite(26,CC6801_GPIO_OUTPUT_HIGH)


#define DRV_MODE_3BIT                   0x80
#define DRV_MODE_4BIT                   0x90
#define DRV_LINE_VERTICAL_OFFSET        1

// BIT MODE DEFINE
#define  DISPLAY_MODE_BIT1              1
#define  DISPLAY_MODE_BIT3              3
#define  DISPLAY_MODE_BIT4              4

#define  DISPLAY_SINLGE_LINE            1
#define  DISPLAY_MULTI_LINE             2

// Please Select this Pre-define bit mode and Line mode
#define DISPLAY_MODE    DISPLAY_MODE_BIT3
#define DISPLAY_LINE    DISPLAY_MULTI_LINE


#if ( DISPLAY_LINE == DISPLAY_SINLGE_LINE)
#define X_ARRAY_BYTE_LEN        2 + (VXSIZE_PHYS * (DISPLAY_MODE)) / 8 + 2
#elif ( DISPLAY_LINE == DISPLAY_MULTI_LINE)
#define X_ARRAY_BYTE_LEN        2 + (VXSIZE_PHYS * (DISPLAY_MODE)) / 8
#endif

uint8_t g_baLcmBuf[YSIZE_PHYS][X_ARRAY_BYTE_LEN];
bool g_aScreenY_Flag[YSIZE_PHYS];
static uint8_t s_ucUpdateEn = 0;


/*********************************************************************
*
*       Static Function
*
**********************************************************************
*/

static void _JDI_Drv_WriteCom(uint8_t _bdata)  __attribute__((unused));
static void _JDI_Drv_WriteData(uint8_t _bdata)  __attribute__((unused));
static void _JDI_Drv_BlinkingColorModeOff(void)  __attribute__((unused));
static void _JDI_Drv_BlinkingColorModeWhite(void)  __attribute__((unused));
static void _JDI_Drv_BlinkingColorModeBlack(void)  __attribute__((unused));
static void _JDI_Drv_Inverse_mode(void)  __attribute__((unused));
static void _JDI_Drv_Clear_AllFlag(void)  __attribute__((unused));
static void _JDI_Drv_DrawLcmBuffer(void)  __attribute__((unused));
static void _JDI_Drv_DrawLcmBuffer_1BitMode(void)  __attribute__((unused));
static void _JDI_Drv_PixletoBuf_Mode_3Bit(uint8_t xPhys, uint8_t yPhys, uint8_t PixelIndex)  __attribute__((unused));
static void _JDI_Drv_PixletoBuf_Mode_4Bit(uint8_t xPhys, uint8_t yPhys, uint8_t PixelIndex)  __attribute__((unused));

static void _JDI_Drv_WriteCom(uint8_t _bdata)
{
    drvi_SpiWrite(SPI_PORT,&_bdata,1);
}
static void _JDI_Drv_WriteData(uint8_t _bdata)
{
    drvi_SpiWrite(SPI_PORT,&_bdata,1);;
}

static void _JDI_Drv_BlinkingColorModeOff(void)
{
    uint8_t _bdata[2] = {0};
    _bdata[0] = 0x40;
    _bdata[1] = 0x00;
    drvi_SpiWrite(SPI_PORT,_bdata,2);
}
static void _JDI_Drv_BlinkingColorModeWhite(void)
{

    uint8_t _bdata[2] = {0};
    _bdata[0] = 0x18;
    _bdata[1] = 0x00;
    drvi_SpiWrite(SPI_PORT,_bdata,2);
}
static void _JDI_Drv_BlinkingColorModeBlack(void)
{
    uint8_t _bdata[2] = {0};
    _bdata[0] = 0x10;
    _bdata[1] = 0x00;
    drvi_SpiWrite(SPI_PORT,_bdata,2);;
}

static void _JDI_Drv_Inverse_mode(void)
{
    uint8_t _bdata[2] = {0};
    _bdata[0] = 0x14;
    _bdata[1] = 0x00;
    drvi_SpiWrite(SPI_PORT,_bdata,2);
}

static void _JDI_Drv_Clear_AllFlag(void)
{
    uint8_t _bdata[2] = {0};
    _bdata[0] = 0x20;
    _bdata[1] = 0x00;
    drvi_SpiWrite(SPI_PORT,_bdata,2);
}

static void _JDI_Drv_DrawLcmBuffer(void)
{
#if (DISPLAY_LINE == DISPLAY_SINLGE_LINE)

    uint8_t _aBuff[X_ARRAY_BYTE_LEN] = {0};
    for (int i =0 ;i <YSIZE_PHYS;i++)
    {
        if ( true == g_aScreenY_Flag[i])
        {
            memcpy(_aBuff,&g_baLcmBuf[(i)][0],X_ARRAY_BYTE_LEN);
            drvi_SpiWrite(SPI_PORT,_aBuff,X_ARRAY_BYTE_LEN); // mode 2bytes + YPHYS + dummy 2bytes

            //drvi_SpiWrite(SPI_PORT,&g_baLcmBuf[(i)][0],X_ARRAY_BYTE_LEN  ); // mode 2bytes + YPHYS + dummy 2bytes
            g_aScreenY_Flag[i] = false;
        }
    }

#elif (DISPLAY_LINE == DISPLAY_MULTI_LINE)

    uint32_t    _dwIdxV = 0;

    #if (DISPLAY_MODE  ==  DISPLAY_MODE_BIT3)
        #define    _LINE_UPDATE_BYTE_SIZE_    (((X_ARRAY_BYTE_LEN) * 3) + 2)
    #elif (DISPLAY_MODE  ==  DISPLAY_MODE_BIT4)
        #define    _LINE_UPDATE_BYTE_SIZE_    (((X_ARRAY_BYTE_LEN) * 2) + 2)
    #endif
    for (_dwIdxV = 0; _dwIdxV < YSIZE_PHYS;  )
    {
        if (true == g_aScreenY_Flag[_dwIdxV])
        {
            drvi_SpiWrite(SPI_PORT, &g_baLcmBuf[_dwIdxV][0], _LINE_UPDATE_BYTE_SIZE_);
        }

    #if (DISPLAY_MODE  ==  DISPLAY_MODE_BIT3)
        g_aScreenY_Flag[_dwIdxV    ] = false;
        g_aScreenY_Flag[_dwIdxV + 1] = false;
        g_aScreenY_Flag[_dwIdxV + 2] = false;
        _dwIdxV += 3;
    #elif (DISPLAY_MODE  ==  DISPLAY_MODE_BIT4)
        g_aScreenY_Flag[_dwIdxV    ] = false;
        g_aScreenY_Flag[_dwIdxV + 1] = false;
        _dwIdxV += 2;
    #endif
    }

#endif
}


static void _JDI_Drv_DrawLcmBuffer_1BitMode(void)
{
    //TO DO
}




static void _JDI_Drv_PixletoBuf_Mode_3Bit(uint8_t xPhys, uint8_t yPhys, uint8_t PixelIndex)
{

    uint8_t bX = xPhys % 8;

    uint8_t bX_Axis = xPhys * 3 /8;


    switch (bX)
    {
        case 0:
            g_baLcmBuf[yPhys][2 + bX_Axis] &= 0x1f;
            g_baLcmBuf[yPhys][2 + bX_Axis] |= PixelIndex<<4;
            break;
        case 1:
            g_baLcmBuf[yPhys][2 + bX_Axis] &= 0xe3;
            g_baLcmBuf[yPhys][2 + bX_Axis] |= PixelIndex<<1;
            break;
        case 2:
            g_baLcmBuf[yPhys][2 + bX_Axis] &= 0xfc;
            g_baLcmBuf[yPhys][2 + bX_Axis] |= PixelIndex>>2;

            g_baLcmBuf[yPhys][2 + bX_Axis + 1] &= 0x7f;
            g_baLcmBuf[yPhys][2 + bX_Axis + 1] |= PixelIndex<<6;
            break;
        case 3:
            g_baLcmBuf[yPhys][2 + bX_Axis] &= 0x8f;
            g_baLcmBuf[yPhys][2 + bX_Axis] |= PixelIndex<<3;
            break;
        case 4:
            g_baLcmBuf[yPhys][2 + bX_Axis] &= 0xf1;
            g_baLcmBuf[yPhys][2 + bX_Axis] |= PixelIndex;
            break;
        case 5:
            g_baLcmBuf[yPhys][2 + bX_Axis] &= 0xfe;
            g_baLcmBuf[yPhys][2 + bX_Axis] |= PixelIndex>>3;

            g_baLcmBuf[yPhys][2 + bX_Axis + 1] &= 0x3f;
            g_baLcmBuf[yPhys][2 + bX_Axis + 1] |= PixelIndex<<5;
            break;
        case 6:
            g_baLcmBuf[yPhys][2 + bX_Axis] &= 0xc7;
            g_baLcmBuf[yPhys][2 + bX_Axis] |= PixelIndex<<2;
            break;
        case 7:
            g_baLcmBuf[yPhys][2 + bX_Axis] &= 0xf8;
            g_baLcmBuf[yPhys][2 + bX_Axis] |= PixelIndex>>1;
            break;
        default:
            break;

    }


}

static void _JDI_Drv_PixletoBuf_Mode_4Bit(uint8_t xPhys, uint8_t yPhys, uint8_t PixelIndex)
{
    if (xPhys %2 ==0)
    {
        g_baLcmBuf[yPhys][2 + xPhys/2] &= 0x0f;
        g_baLcmBuf[yPhys][2 + xPhys/2] |= PixelIndex<<4;

    }
    else
    {
        g_baLcmBuf[yPhys][2 + xPhys/2] &= 0xf0;
        g_baLcmBuf[yPhys][2 + xPhys/2] |= PixelIndex;
    }
}

/*********************************************************************
*
*       Public Function
*
**********************************************************************
*/

void OLED_JDI_Drv_Init(void)
{

    //memset(g_aScreenBuff,0x00,sizeof(g_aScreenBuff));

    uint32_t    _dwIdxV = 0;

    #if (DISPLAY_MODE == DISPLAY_MODE_BIT1)
        // TODO
    #elif (DISPLAY_MODE == DISPLAY_MODE_BIT3)
        for (_dwIdxV = 0; _dwIdxV < YSIZE_PHYS; _dwIdxV++)
        {
        g_baLcmBuf[_dwIdxV][0] = DRV_MODE_3BIT;
        g_baLcmBuf[_dwIdxV][1] = _dwIdxV+DRV_LINE_VERTICAL_OFFSET;

            g_baLcmBuf[_dwIdxV][X_ARRAY_BYTE_LEN - 2] = 0;
            g_baLcmBuf[_dwIdxV][X_ARRAY_BYTE_LEN - 1] = 0;
        }
    #elif (DISPLAY_MODE == DISPLAY_MODE_BIT4)
        for (_dwIdxV = 0; _dwIdxV < YSIZE_PHYS; _dwIdxV++)
        {
        g_baLcmBuf[_dwIdxV][0] = DRV_MODE_4BIT;
        g_baLcmBuf[_dwIdxV][1] = _dwIdxV+DRV_LINE_VERTICAL_OFFSET;
            g_baLcmBuf[_dwIdxV][X_ARRAY_BYTE_LEN - 2] = 0;
            g_baLcmBuf[_dwIdxV][X_ARRAY_BYTE_LEN - 1] = 0;

        }
    #else

    #endif


#if 1
    _JDI_Drv_WriteCom(0xFD);
    _JDI_Drv_WriteData(0x12);


    _JDI_Drv_WriteCom(0xAE);    /*sleep in*/

    _JDI_Drv_WriteCom(0xb3);
    _JDI_Drv_WriteData(0x91);

    _JDI_Drv_WriteCom(0xca);
    _JDI_Drv_WriteData(0x3f);

    _JDI_Drv_WriteCom(0xa2);
    _JDI_Drv_WriteData(0x00);

    _JDI_Drv_WriteCom(0xa1);
    _JDI_Drv_WriteData(0x00);

    _JDI_Drv_WriteCom(0xa0);
    _JDI_Drv_WriteData(0x14);
    _JDI_Drv_WriteData(0x11);

    _JDI_Drv_WriteCom(0xb5);
    _JDI_Drv_WriteData(0x00);

    _JDI_Drv_WriteCom(0xab);
    _JDI_Drv_WriteData(0x01);

    _JDI_Drv_WriteCom(0xb4);
    _JDI_Drv_WriteData(0xa0);
    _JDI_Drv_WriteData(0xfd);

    _JDI_Drv_WriteCom(0xc1);
    _JDI_Drv_WriteData(0x9f);

    _JDI_Drv_WriteCom(0xc7);
    _JDI_Drv_WriteData(0x0f);

    _JDI_Drv_WriteCom(0xb9);

    _JDI_Drv_WriteCom(0xb1);
    _JDI_Drv_WriteData(0xe2);

    _JDI_Drv_WriteCom(0xd1);
    _JDI_Drv_WriteData(0x82);
    _JDI_Drv_WriteData(0x20);

    _JDI_Drv_WriteCom(0xbb);
    _JDI_Drv_WriteData(0x1f);

    _JDI_Drv_WriteCom(0xb6);
    _JDI_Drv_WriteData(0x08);

    _JDI_Drv_WriteCom(0xbe);
    _JDI_Drv_WriteData(0x07);

    _JDI_Drv_WriteCom(0xa6);

    _JDI_Drv_WriteCom(0xAF);    /*sleep out*/
#endif
       // Clear

    _JDI_Drv_Clear_AllFlag();
    //JDILCD_DELAY(10);
    #if 1
    // Set display pin h
    LCD_DISP_SET;
    //JDILCD_DELAY(100);
    LCD_EXTCOM_SET;
    #endif


}

void OLED_JDI_Drv_Start_Draw(void)
{
	s_ucUpdateEn++;
}


void OLED_JDI_Drv_End_Draw(void)
{
    if (s_ucUpdateEn > 0)
    	s_ucUpdateEn--;
    if (s_ucUpdateEn == 0)
    {

        #if (DISPLAY_MODE == DISPLAY_MODE_BIT1)
        // TODO
        #else
            _JDI_Drv_DrawLcmBuffer();
        #endif

    }
}

void OLED_JDI_Drv_PutPixel(uint8_t xPhys, uint8_t yPhys, uint8_t PixelIndex)
{
    g_aScreenY_Flag[yPhys]= true;
    #if (DISPLAY_MODE == DISPLAY_MODE_BIT1)
        // TODO
    #elif (DISPLAY_MODE == DISPLAY_MODE_BIT3)
        _JDI_Drv_PixletoBuf_Mode_3Bit(xPhys,yPhys,PixelIndex);
    #elif (DISPLAY_MODE == DISPLAY_MODE_BIT4)
        _JDI_Drv_PixletoBuf_Mode_4Bit(xPhys,yPhys,PixelIndex);
    #else

    #endif


}

unsigned int OLED_JDI_Drv_GetPixel(uint8_t xPhys, uint8_t yPhys)
{
    return 0;
}

#endif

