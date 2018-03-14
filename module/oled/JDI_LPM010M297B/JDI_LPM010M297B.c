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

#include <string.h>
#include <stdio.h>

#include "JDI_LPM010M297B.h"
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


#define GATELINE        208
#define PIXEL_POINT     104
#define PIXEL_X         208
#define PIXEL_Y         208

unsigned char JDI_Pixel_Fuffer[GATELINE][PIXEL_POINT];
unsigned char JDI_Pixel_Fuffer_1Bit[GATELINE][13];
static uint8_t s_ucUpdateEn = 0;
extern unsigned char _acHeart[2704];
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

    Display_BlinkingColorModeBlack();

    for (int i =1 ;i <=GATELINE;i++)
    {
        _bBuffer[0] = 0x90;
        _bBuffer[1] = i;
        memcpy(&_bBuffer[2],&JDI_Pixel_Fuffer[(i-1)][0],PIXEL_POINT*sizeof(char));
        drvi_SpiWrite(SPI_PORT,_bBuffer,108);
    }
    Display_NoUpdatMode();
}


void JDI_PlotData_1BitMode(void)
{
    uint8_t _bBuffer[17];

    //Display_BlinkingColorModeBlack();

    for (int i =1 ;i <=GATELINE;i++)
    {
        _bBuffer[0] = 0x90;
        _bBuffer[1] = i;
        memcpy(&_bBuffer[2],&JDI_Pixel_Fuffer_1Bit[(i-1)][0],13*sizeof(char));
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
        memset(&JDI_Pixel_Fuffer[(i-1)][0],BLACK,PIXEL_POINT*sizeof(char));
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
    //drvi_SpiWrite(SPI_PORT,_GeteLineData,108);
    drvi_SpiWrite(SPI_PORT,_GeteLineData,108);
}

void Display_All_RedColor(void)
{
    for (int i = 1;i <=208;i++)
    {

        Dispaly_4Bit_SlngleLine_UpdataMode(i,RED);
    }
}

void Display_All_GreenColor(void)
{
    for (int i = 1;i <=208;i++)
    {

        Dispaly_4Bit_SlngleLine_UpdataMode(i,GREEN);
    }
}

void Display_All_BlueColor(void)
{
    for (int i = 1;i <=208;i++)
    {
        Dispaly_4Bit_SlngleLine_UpdataMode(i,BLUE);
    }
}

void Display_All_WhiteColor(void)
{
    for (int i = 1;i <=208;i++)
    {
        Dispaly_4Bit_SlngleLine_UpdataMode(i,WHITE);
    }
}

void Display_All_BLACKColor(void)
{
    for (int i = 1;i <=208;i++)
    {
        Dispaly_4Bit_SlngleLine_UpdataMode(i,BLACK);
    }
}


void JDI_PutPixel(uint8_t xPhys, uint8_t yPhys, uint8_t PixelIndex)
{


    if (yPhys %2 ==0)
    {
        JDI_Pixel_Fuffer[xPhys][yPhys/2] &= 0x0f;
        JDI_Pixel_Fuffer[xPhys][yPhys/2] |= PixelIndex<<4;
    }
    else
    {
        JDI_Pixel_Fuffer[xPhys][yPhys/2] &= 0xf0;
        JDI_Pixel_Fuffer[xPhys][yPhys/2] |= PixelIndex;
    }


}

unsigned int JDI_GetPixel(uint8_t xPhys, uint8_t yPhys)
{
    return 0;
}

void JDI_LCD_Init(void)
{

    memset(JDI_Pixel_Fuffer,0x00,sizeof(JDI_Pixel_Fuffer));
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


#if 0
void Gui_DrawFont_Size32(uint8_t x, uint8_t y, uint8_t color, char ch[])
{
    uint8_t c=0,j=0;
    while (ch[j]!='\0')
    {
        c =ch[j]-32;
        TracerInfo("== Gui_DrawFont_Size32 ==\r\n");
        //Gui_DrawFont_word(x,y+(32/2)*j,c,color,32);
        JDI_DrawFont_word(x,y+(32/2)*j,c,color,32);
        j++;
    }

}



void Display_showimage(int xarea,int yarea,int xlen,int ylen ,unsigned char *pData)
{

    uint8_t _GeteLineData[108];// = {0x00,0x00,0x00,0x00,0x00};
    int i =0;
    int y =0;
    for(int x=0 ;x<xlen;x++)
    {
        memset(_GeteLineData,0x00,sizeof(_GeteLineData));

        if (ylen%2 !=0)
            ylen+=1;

        for (y=0 ;y <ylen/2; y++)
        {
            _GeteLineData[0] = 0x90;
            _GeteLineData[1] = x +xarea;


            _GeteLineData[2+yarea+y] = (_acHeart[i*((xlen+1)/2)+y]);

            #if 0
                _GeteLineData[2+yarea+y] = (_acHeart[i*((xlen+1)/2)+y]);
            #endif
        }


        drvi_SpiWrite(SPI_PORT,_GeteLineData,108);
        i++;
    }

}

#endif


uint8_t TempByte;
uint16_t Temp;
#if 0
void JDI_DrawFont_word(uint8_t x, uint8_t y,uint8_t word,uint8_t color,uint8_t _bArrayLen)
{
    for (uint8_t _bFontCnt = 0; _bFontCnt<_bArrayLen; _bFontCnt++)
    {
        for (int i= 0;i<2;i++)
        {
            TempByte = FontSize32_32[word*64+_bFontCnt*2+i];
            Temp = y/2;

            JDI_Pixel_Fuffer[x+_bFontCnt+1][Temp+(i*4)] =0;
            if((TempByte & 0x80) == 0x80)
            {
                JDI_Pixel_Fuffer[x+_bFontCnt+1][Temp+(i*4)] = (color & 0xf0);
            }
            TempByte = TempByte<<1;

            if((TempByte & 0x80) == 0x80)
            {
                JDI_Pixel_Fuffer[x+_bFontCnt+1][Temp+(i*4)] |= (color & 0x0f);
            }
            TempByte = TempByte<<1;


            JDI_Pixel_Fuffer[x+_bFontCnt+1][Temp+1+(i*4)] =0;
            if((TempByte & 0x80) == 0x80)
            {
                    JDI_Pixel_Fuffer[x+_bFontCnt+1][Temp+1+(i*4)] = (color & 0xf0);
            }
            TempByte = TempByte<<1;
            if((TempByte & 0x80) == 0x80)
            {
                    JDI_Pixel_Fuffer[x+_bFontCnt+1][Temp+1+(i*4)] |= (color & 0x0f);
            }
            TempByte = TempByte<<1;


            JDI_Pixel_Fuffer[x+_bFontCnt+1][Temp+2+(i*4)] =0;
            if((TempByte & 0x80) == 0x80)
            {
                    JDI_Pixel_Fuffer[x+_bFontCnt+1][Temp+2+(i*4)] = (color & 0xf0);
            }
            TempByte = TempByte<<1;
            if((TempByte & 0x80) == 0x80)
            {
                    JDI_Pixel_Fuffer[x+_bFontCnt+1][Temp+2+(i*4)] |= (color & 0x0f);
            }
            TempByte = TempByte<<1;


            JDI_Pixel_Fuffer[x+_bFontCnt+1][Temp+3+(i*4)] =0;
            if((TempByte & 0x80) == 0x80)
            {
                    JDI_Pixel_Fuffer[x+_bFontCnt+1][Temp+3+(i*4)] = (color & 0xf0);
            }
            TempByte = TempByte<<1;
            if((TempByte & 0x80) == 0x80)
            {
                    JDI_Pixel_Fuffer[x+_bFontCnt+1][Temp+3+(i*4)] |= (color & 0x0f);
            }
            TempByte = TempByte<<1;


        }

    }

}
#endif

#if 0
void Gui_DrawFont_word(uint8_t x, uint8_t y,uint8_t word,uint8_t color,uint8_t _bArrayLen)
{

    uint8_t c=0,i=0;
    uint8_t _GeteLineData[108];// = {0x00,0x00,0x00,0x00,0x00};


    for (uint8_t _bFontCnt = 0; _bFontCnt<_bArrayLen; _bFontCnt++)
    {
        memset(_GeteLineData,0x00,sizeof(_GeteLineData));
        _GeteLineData[0] = 0x90;
        _GeteLineData[1] = x+1;
        //TracerInfo("== Gui_DrawFont_word %d _bFontCnt==\r\n",_bFontCnt);
        for (i= 0;i<2;i++)
        {
            TempByte = FontSize32_32[word*64+_bFontCnt*2+i];

            Temp = 2+y;
            if((TempByte & 0x80) == 0x80)
            {
                    _GeteLineData[Temp+(i*4)] = (color & 0xf0);
            }
            TempByte = TempByte<<1;
            if((TempByte & 0x80) == 0x80)
            {
                    _GeteLineData[Temp+(i*4)] |= (color & 0x0f);
            }
            TempByte = TempByte<<1;

            if((TempByte & 0x80) == 0x80)
            {
                    _GeteLineData[Temp+1+(i*4)] = (color & 0xf0);
            }
            TempByte = TempByte<<1;
            if((TempByte & 0x80) == 0x80)
            {
                    _GeteLineData[Temp+1+(i*4)] |= (color & 0x0f);
            }
            TempByte = TempByte<<1;

            if((TempByte & 0x80) == 0x80)
            {
                    _GeteLineData[Temp+2+(i*4)] = (color & 0xf0);
            }
            TempByte = TempByte<<1;
            if((TempByte & 0x80) == 0x80)
            {
                    _GeteLineData[Temp+2+(i*4)] |= (color & 0x0f);
            }
            TempByte = TempByte<<1;

            if((TempByte & 0x80) == 0x80)
            {
                    _GeteLineData[Temp+3+(i*4)] = (color & 0xf0);
            }
            TempByte = TempByte<<1;
            if((TempByte & 0x80) == 0x80)
            {
                    _GeteLineData[Temp+3+(i*4)] |= (color & 0x0f);
            }
            TempByte = TempByte<<1;




            #if 0
            for (int u=0;u<4;u++)
            {
                Temp = 2+y+u*2;
                //TracerInfo("== Gui_DrawFont_word %d Temp==\r\n",Temp);
                if((TempByte & 0x80) == 0x80)
                {
                    _GeteLineData[Temp] = (color & 0xf0);
                }
                TempByte = TempByte<<1;


                if((TempByte & 0x80) == 0x80)
                {
                    //Temp = 2+y+u*2;
                    _GeteLineData[Temp]|= (color & 0x0f);
                }
                TempByte = TempByte<<1;

            }
            #endif
        }
        x++;

        drvi_SpiWrite(SPI_PORT,_GeteLineData,108);
    }

}
#endif

#if 1
void JDI_Demo(void)
{
    JDI_LCD_Init();

	char color[8] = {YELLOW,BLUE,CYAN,GREEN,MAGENTA,RED,WHITE,MAGENTA};


	for (int i=0 ;i<8;i++)
	{

		Display_BlinkingColorModeBlack();


		for (int j = 1;j <=208;j++)
		{
			Dispaly_4Bit_SlngleLine_UpdataMode(j,color[i]);
		}

		Display_NoUpdatMode();

		 //JDILCD_DELAY(2000);

	}

	#if 0
    while(1)
    {
        char color[8] = {YELLOW,BLUE,CYAN,GREEN,MAGENTA,RED,WHITE,BLACK};

        /*
        for (int i=0 ;i<8;i++)
        {

            Display_BlinkingColorModeBlack();


            for (int j = 1;j <=208;j++)
            {

                Dispaly_4Bit_SlngleLine_UpdataMode(j,color[i]);


            }

            Display_NoUpdatMode();

             JDILCD_DELAY(2000);

        }
        */
        /*
        while(1)
        {
            Display_BlinkingColorModeBlack();
            Display_All_BLACKColor();
            Display_NoUpdatMode();
            JDILCD_DELAY(2000);
            //Display_BlinkingColorModeBlack();
            Gui_DrawFont_Size32(30,32,WHITE,"CLOUDCHIP");
            Gui_DrawFont_Size32(62,36,GREEN,"JDI DEMO");
            JDI_PlotData_4BitMode();
            TFTLED_DELAY(5000);
            Gui_DrawFont_Size32(94,66,RED,"START");
            Gui_DrawFont_Size32(126,100,YELLOW,"5");
            JDI_PlotData_4BitMode();
            TFTLED_DELAY(5000);
            Gui_DrawFont_Size32(126,100,BLUE,"4");
            JDI_PlotData_4BitMode();
            TFTLED_DELAY(5000);
            Gui_DrawFont_Size32(126,100,CYAN,"3");
            JDI_PlotData_4BitMode();
            TFTLED_DELAY(5000);
            Gui_DrawFont_Size32(126,100,GREEN,"2");
            JDI_PlotData_4BitMode();
            TFTLED_DELAY(5000);
            Gui_DrawFont_Size32(126,100,MAGENTA,"1");
            JDI_PlotData_4BitMode();
            TFTLED_DELAY(5000);

            JDI_Clear_buffer(94,94+32);
            Gui_DrawFont_Size32(94,84,RED,"END");
            Gui_DrawFont_Size32(126,100,WHITE,"0");

            JDI_PlotData_4BitMode();
            TFTLED_DELAY(5000);

        }
        */

        //Gui_DrawFont_Size32(20,28,WHITE,"T");
        //Display_showimage(80,40,52,52,_acHeart);//ͼƬ��ʾʾ��
        //Display_NoUpdatMode();


        /*
        JDILCD_DELAY(2000);
        Display_BlinkingColorModeBlack();
        Display_All_WhiteColor();
        Display_NoUpdatMode();
        JDILCD_DELAY(2000);
        Display_BlinkingColorModeBlack();
        Display_All_BLACKColor();
        Display_NoUpdatMode();
        JDILCD_DELAY(2000);
        Display_BlinkingColorModeWhite();
        JDILCD_DELAY(2000);
        Display_BlinkingColorModeBlack();
        JDILCD_DELAY(2000);
        */

    }
    #endif
}
#endif
