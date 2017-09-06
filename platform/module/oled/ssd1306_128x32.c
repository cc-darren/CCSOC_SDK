/******************************************************************************
*  Copyright 2017, CloudChip, Inc.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of CloudChip, Inc. (C) 2017
******************************************************************************/

/******************************************************************************
*  Filename:
*  ---------
*  ssd1306_128x32.c
*
*  Project:
*  --------
*  cc6801
*
*  Description:
*  ------------
*  This is SSD1306 driver for pixels 128x32
*
*  Author:
*  -------
*  Blake
*
*===========================================================================/
*  20170804 Blake initial version
******************************************************************************/

#include "font.h"
#include "ssd1306.h"

void ssd1306_SetPosition(uint8_t x, uint8_t y)
{
    ssd1306_SendCommand(0xb0+y);
    ssd1306_SendCommand((x&0x0f)|0x01);
    ssd1306_SendCommand(((x&0xf0)>>4)|0x10);
}

void ssd1306_DrawFrameBuffer(uint8_t* fb, uint32_t size, uint8_t x, uint8_t y)
{
    uint32_t j;

    ssd1306_SendCommand(0xb0+y);
    ssd1306_SendCommand(((x&0xf0)>>4)|0x10);
    ssd1306_SendCommand((x&0x0f)|0x01);

    for(j=0;j<size;j++)
    {
        ssd1306_SendData((uint8_t)fb[j]);
    }
}

void ssd1306_DrawWhite(void)
{
    uint32_t j,i;

    for(i=0xB0;i<=0xB4;i++)
    {
        ssd1306_SendCommand(i);
        ssd1306_SendCommand(0x0c);
        ssd1306_SendCommand(0x11);

        for(j=0;j<128;j++)
            ssd1306_SendData(0xff);
    }
}

void ssd1306_DrawBlack(void)
{
    uint32_t j,i;

    for(i=0xB0;i<=0xB4;i++)
    {
        ssd1306_SendCommand(i);
        ssd1306_SendCommand(0x0c);
        ssd1306_SendCommand(0x11);

        for(j=0;j<128;j++)
            ssd1306_SendData(0x00);
    }
}

void ssd1306_DrawPixel6x8(uint8_t x, uint8_t y,uint8_t ch[])
{
    uint8_t c=0,i=0,j=0;

    while (ch[j]!='\0')
    {
        c =ch[j]-32;
        if(x>126){x=0;y++;}
        ssd1306_SetPosition(x-1,y);
        for(i=0;i<6;i++)
            ssd1306_SendData(F6x8[c][i]);
        x+=6;
        j++;
    }
}

void ssd1306_DrawPixel8x16(uint8_t x, uint8_t y,uint8_t ch[])
{
    uint8_t c=0,i=0,j=0;

    while (ch[j]!='\0')
    {
        c =ch[j]-32;
        if(x>120){x=0;y++;}
        ssd1306_SetPosition(x-1,y);
        for(i=0;i<8;i++)
            ssd1306_SendData(FONT_8x16[c*16+i]);
        ssd1306_SetPosition(x-1,y+1);
        for(i=0;i<8;i++)
            ssd1306_SendData(FONT_8x16[c*16+i+8]);
        x+=8;
        j++;
    }
}

void ssd1306_DrawBitmap(uint8_t x0, uint8_t y0,uint8_t x1, uint8_t y1,uint8_t BMP[])
{
    uint32_t j=0;
    uint8_t x,y;

    if(y1%8==0) y=y1/8;
    else y=y1/8+1;

    for(y=y0;y<y1;y++)
    {
        ssd1306_SetPosition(x0-1,y);
        for(x=x0;x<x1;x++)
        {
            ssd1306_SendData(BMP[j++]);
        }
    }
}
