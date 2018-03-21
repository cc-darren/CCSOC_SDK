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
#ifdef JDI_OLED_ENABLE_176x176

#include <string.h>
#include <stdio.h>
#include "LCDConf.h"


#include "JDI_LPM013M126A.h"
#include "gpio.h"
#include "drvi_gpio.h"
#include "drvi_spi.h"
#include "drvi_pwm.h"
#include "tracer.h"

#define LCD_DISP_CLR            cc6801_GpioWrite(27,CC6801_GPIO_OUTPUT_LOW)
#define LCD_DISP_SET            cc6801_GpioWrite(27,CC6801_GPIO_OUTPUT_HIGH)
#define LCD_EXTCOM_CLR          cc6801_GpioWrite(26,CC6801_GPIO_OUTPUT_LOW)
#define LCD_EXTCOM_SET          cc6801_GpioWrite(26,CC6801_GPIO_OUTPUT_HIGH)
#define SPI_PORT                 2

#define JDILCD_DELAY(x) delayns(x*1000)

#define WHITE       0xFF
#define YELLOW      0xcc
#define MAGENTA     0xaa
#define RED         0x88
#define CYAN        0x66
#define GREEN       0x44
#define BLUE        0x22
#define BLACK       0x00

#define WHITE_MSB       0xF0
#define YELLOW_MSB      0xc0
#define MAGENTA_MSB     0xa0
#define RED_MSB         0x80
#define CYAN_MSB        0x60
#define GREEN_MSB       0x40
#define BLUE_MSB        0x20
#define BLACK_MSB       0x00

#define WHITE_LSB       0x0F
#define YELLOW_LSB      0x0c
#define MAGENTA_LSB     0x0a
#define RED_LSB         0x08
#define CYAN_LSB        0x06
#define GREEN_LSB       0x04
#define BLUE_LSB        0x02
#define BLACK_LSB       0x00
uint8_t g_aScreenBuff[YSIZE_PHYS][XSIZE_PHYS];
bool g_aScreenY_Flag[YSIZE_PHYS];
uint8_t g_aScreenBuff_1Bit[YSIZE_PHYS][XSIZE_PHYS/8];
static uint8_t s_ucUpdateEn = 0;

void Display_NoUpdatMode(void)
{
    uint8_t _bdata[2] = {0};
    _bdata[0] = 0x40;
    _bdata[1] = 0x00;
    drvi_SpiWrite(SPI_PORT,_bdata,2);
}
void Display_Inverse_mode(void)
{
    uint8_t _bdata[2] = {0};
    _bdata[0] = 0x14;
    _bdata[1] = 0x00;
    drvi_SpiWrite(SPI_PORT,_bdata,2);
}

void Display_BlinkingColorModeWhite(void)
{

    uint8_t _bdata[2] = {0};
    _bdata[0] = 0x18;
    _bdata[1] = 0x00;
    drvi_SpiWrite(SPI_PORT,_bdata,2);
}

void Display_BlinkingColorModeBlack(void)
{
    uint8_t _bdata[2] = {0};
    _bdata[0] = 0x10;
    _bdata[1] = 0x00;
    drvi_SpiWrite(SPI_PORT,_bdata,2);;
}


void Display_Clear_All_Flag(void)
{
    uint8_t _bdata[2] = {0};
    _bdata[0] = 0x20;
    _bdata[1] = 0x00;
    drvi_SpiWrite(SPI_PORT,_bdata,2);
}


void WriteCom(uint8_t _bdata)
{
    drvi_SpiWrite(SPI_PORT,&_bdata,1);
}
void WriteData(uint8_t _bdata)
{
    drvi_SpiWrite(SPI_PORT,&_bdata,1);;
}




void JDI_PlotData_4BitMode(void)
{
    uint8_t _bBuffer[108];

    //Display_BlinkingColorModeBlack();

    for (int i =1 ;i <=YSIZE_PHYS;i++)
    {
        if ( true == g_aScreenY_Flag[i-1])
        {
            _bBuffer[0] = 0x90;
            _bBuffer[1] = i;
            memcpy(&_bBuffer[2],&g_aScreenBuff[(i-1)][0],XSIZE_PHYS*sizeof(char));
            drvi_SpiWrite(SPI_PORT,_bBuffer,2+XSIZE_PHYS+2); // mode 2bytes + YPHYS + dummy 2bytes
            g_aScreenY_Flag[i-1] = false;
        }
    }
    Display_NoUpdatMode();
}


void JDI_PlotData_1BitMode(void)
{
    uint8_t _bBuffer[17];

    //Display_BlinkingColorModeBlack();

    for (int i =1 ;i <=YSIZE_PHYS;i++)
    {
        _bBuffer[0] = 0x90;
        _bBuffer[1] = i;
        memcpy(&_bBuffer[2],&g_aScreenBuff_1Bit[(i-1)][0],(XSIZE_PHYS/8)*sizeof(char));
        drvi_SpiWrite(SPI_PORT,_bBuffer,17);
    }
    Display_NoUpdatMode();
}


void JDI_Start_Draw(void)
{
	s_ucUpdateEn++;
}


void JDI_End_Draw(void)
{
    if (s_ucUpdateEn > 0)
    	s_ucUpdateEn--;
    if (s_ucUpdateEn == 0)
    {
        JDI_PlotData_4BitMode();
    }
}



void JDI_Clear_buffer(uint8_t _bGateLine_Start, uint8_t _bGeteLine_End)
{

    for (int i =_bGateLine_Start ;i <_bGeteLine_End;i++)
    {
        memset(&g_aScreenBuff[(i-1)][0],BLACK,XSIZE_PHYS*sizeof(char));
    }
}

void Dispaly_4Bit_SlngleLine_UpdataMode(uint8_t _wData,uint8_t Color)
{

    uint8_t _GeteLineData[108];// = {0x00,0x00,0x00,0x00,0x00};
    memset(_GeteLineData,0x00,sizeof(_GeteLineData));
    _GeteLineData[0] = 0x90;
    _GeteLineData[1] = (_wData);
    for (int j=0;j<104;j++)
        _GeteLineData[2+j] = Color;
    drvi_SpiWrite(SPI_PORT,_GeteLineData,2+XSIZE_PHYS+2);// mode 2bytes + YPHYS + dummy 2bytes
}

void Display_All_RedColor(void)
{
    for (int i = 1;i <=YSIZE_PHYS;i++)
    {

        Dispaly_4Bit_SlngleLine_UpdataMode(i,RED);
    }
}

void Display_All_GreenColor(void)
{
    for (int i = 1;i <=YSIZE_PHYS;i++)
    {

        Dispaly_4Bit_SlngleLine_UpdataMode(i,GREEN);
    }
}

void Display_All_BlueColor(void)
{
    for (int i = 1;i <=YSIZE_PHYS;i++)
    {
        Dispaly_4Bit_SlngleLine_UpdataMode(i,BLUE);
    }
}

void Display_All_WhiteColor(void)
{
    for (int i = 1;i <=YSIZE_PHYS;i++)
    {
        Dispaly_4Bit_SlngleLine_UpdataMode(i,WHITE);
    }
}

void Display_All_BLACKColor(void)
{
    for (int i = 1;i <=YSIZE_PHYS;i++)
    {
        Dispaly_4Bit_SlngleLine_UpdataMode(i,BLACK);
    }
}


void JDI_PutPixel(uint8_t xPhys, uint8_t yPhys, uint8_t PixelIndex)
{
    g_aScreenY_Flag[yPhys]= true;

    if (xPhys %2 ==0)
    {
        g_aScreenBuff[yPhys][xPhys/2] &= 0x0f;
        g_aScreenBuff[yPhys][xPhys/2] |= PixelIndex<<4;

    }
    else
    {
        g_aScreenBuff[yPhys][xPhys/2] &= 0xf0;
        g_aScreenBuff[yPhys][xPhys/2] |= PixelIndex;
    }
}

unsigned int JDI_GetPixel(uint8_t xPhys, uint8_t yPhys)
{
    return 0;
}

void JDI_LCD_Init(void)
{

    memset(g_aScreenBuff,0x00,sizeof(g_aScreenBuff));
#if 1
    WriteCom(0xFD);
    WriteData(0x12);


    WriteCom(0xAE);    /*sleep in*/

    WriteCom(0xb3);
    WriteData(0x91);

    WriteCom(0xca);
    WriteData(0x3f);

    WriteCom(0xa2);
    WriteData(0x00);

    WriteCom(0xa1);
    WriteData(0x00);

    WriteCom(0xa0);
    WriteData(0x14);
    WriteData(0x11);

    WriteCom(0xb5);
    WriteData(0x00);

    WriteCom(0xab);
    WriteData(0x01);

    WriteCom(0xb4);
    WriteData(0xa0);
    WriteData(0xfd);

    WriteCom(0xc1);
    WriteData(0x9f);

    WriteCom(0xc7);
    WriteData(0x0f);

    WriteCom(0xb9);

    WriteCom(0xb1);
    WriteData(0xe2);

    WriteCom(0xd1);
    WriteData(0x82);
    WriteData(0x20);

    WriteCom(0xbb);
    WriteData(0x1f);

    WriteCom(0xb6);
    WriteData(0x08);

    WriteCom(0xbe);
    WriteData(0x07);

    WriteCom(0xa6);

    WriteCom(0xAF);    /*sleep out*/
#endif
       // Clear

    Display_Clear_All_Flag();
    //JDILCD_DELAY(10);
    #if 1
    // Set display pin h
    LCD_DISP_SET;
    //JDILCD_DELAY(100);
    LCD_EXTCOM_SET;
    #endif


}

#endif

