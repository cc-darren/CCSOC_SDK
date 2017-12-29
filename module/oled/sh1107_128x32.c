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
*  sh1107_128x32.c
*
*  Project:
*  --------
*  cc6801
*
*  Description:
*  ------------
*  This is SH1107 driver for pixels 128x32
*
*  Author:
*  -------
*  Blake
*
*===========================================================================/
*  20170804 Blake initial version
******************************************************************************/

#include "font.h"
#include "sh1107.h"

void sh1107_SetPosition(uint8_t x, uint8_t y)
{
    sh1107_SendCommand(0xb0+y);

    sh1107_SendCommand((x&0x0f));
    sh1107_SendCommand(((x&0xf0)>>4)|0x10);    
}

void sh1107_DrawFrameBuffer(uint8_t* fb, uint32_t size, uint8_t x, uint8_t y)
{
    uint32_t j;

    sh1107_SendCommand(0xb0+y);
    sh1107_SendCommand(((x&0xf0)>>4)|0x10);
    sh1107_SendCommand((x&0x0f)|0x01);

    for(j=0;j<size;j++)
    {
        sh1107_SendData((uint8_t)fb[j]);
    }
}

void sh1107_DrawWhite(void)
{
    uint32_t j,i;

    for(i=0xB0;i<=0xBF;i++)
    {
        sh1107_SendCommand(i);
        sh1107_SendCommand(0x00);
        sh1107_SendCommand(0x10);

        for(j=0;j<128;j++)
            sh1107_SendData(0xff);
    }
}

void sh1107_DrawBlack(void)
{
    uint32_t j,i;

    for(i=0xB0;i<=0xBF;i++)
    {
        sh1107_SendCommand(i);
        sh1107_SendCommand(0x00);
        sh1107_SendCommand(0x10);

        for(j=0;j<128;j++)
            sh1107_SendData(0x00);
    }
}


void sh1107_DrawPixel6x8(uint8_t x, uint8_t y, uint8_t ch[])
{
    unsigned char c=0,i=0,j=0;   
    
    while (ch[j]!='\0')
    {
        c =ch[j]-32;
        if(x>100){x=0;y++;}
        sh1107_SetPosition(x-1,y);    
        for(i=0;i<6;i++)     
        sh1107_SendData(F6x8[c][i]);  
        x+=6;
        j++;
    }      
}



void sh1107_DrawPixel8x16_Thin(uint8_t x, uint8_t y, uint8_t ch[])
{
    uint8_t c=0,i=0,j=0;
    
    while (ch[j]!='\0')
    {
        c =ch[j]-32;       
        sh1107_SetPosition(x,y);
        for(i=0;i<8;i++)     
            sh1107_SendData(FONT_8x16_THIN[c*16+i]);
        sh1107_SetPosition(x,y+1); 
        for(i=0;i<8;i++)     
            sh1107_SendData(FONT_8x16_THIN[c*16+i+8]);  
        x+=8;
        j++;
   }
}

//

void sh1107_DrawPixel8x16(uint8_t x, uint8_t y, uint8_t ch[])
{
    uint8_t c=0,i=0,j=0;
    
    while (ch[j]!='\0')
    {
        c =ch[j]-32;       
        sh1107_SetPosition(x,y);
        for(i=0;i<8;i++)     
            sh1107_SendData(FONT_8x16[c*16+i]);
        sh1107_SetPosition(x,y+1); 
        for(i=0;i<8;i++)     
            sh1107_SendData(FONT_8x16[c*16+i+8]);  
        x+=8;
        j++;
   }
}

//
void sh1107_DrawPixel2x24(uint8_t x, uint8_t y, uint8_t ch[])
{
    uint8_t c=0,i=0,j=0;
    
    while (ch[j]!='\0')
    {    
        c =ch[j]-32;        
        sh1107_SetPosition(x,y);    
        for(i=0;i<12;i++)     
            sh1107_SendData(FONT_12x24[c*36+i]);
        sh1107_SetPosition(x,y+1);    
        for(i=0;i<12;i++)     
          sh1107_SendData(FONT_12x24[c*36+12+i]);  
        sh1107_SetPosition(x,y+2);    
        for(i=0;i<12;i++)     
          sh1107_SendData(FONT_12x24[c*36+24+i]);          
        x+=12;
        j++;
    }     
}

void sh1107_DrawPixel2x32(uint8_t x, uint8_t y, uint8_t ch[])
{
    uint8_t c=0,i=0,j=0;
    
    while (ch[j]!='\0')
    {    
        c =ch[j]-32;        
        sh1107_SetPosition(x,y);    
        for(i=0;i<12;i++)     
            sh1107_SendData(FONT_12x32[c*48+i]);
        sh1107_SetPosition(x,y+1);    
        for(i=0;i<12;i++)     
          sh1107_SendData(FONT_12x32[c*48+12+i]);  
        sh1107_SetPosition(x,y+2);    
        for(i=0;i<12;i++)     
          sh1107_SendData(FONT_12x32[c*48+24+i]);          

        sh1107_SetPosition(x,y+3);    
        for(i=0;i<12;i++)     
          sh1107_SendData(FONT_12x32[c*48+36+i]);          

        x+=12;
        j++;
    }     
}


void sh1107_DrawPixel8x36(uint8_t x, uint8_t y, uint8_t ch[])
{
    unsigned char c=0,i=0,j=0;

    while (ch[j]!='\0')
    {    
        c = ch[j]-32;  

        sh1107_SetPosition(x,y);    
        for(i=0;i<18;i++)     
            sh1107_SendData(FONT_18x36[c*90+i]);
        sh1107_SetPosition(x,y+1);    
        for(i=0;i<18;i++)     
          sh1107_SendData(FONT_18x36[c*90+18+i]);  
        sh1107_SetPosition(x,y+2);    
        for(i=0;i<18;i++)     
          sh1107_SendData(FONT_18x36[c*90+36+i]);          
        sh1107_SetPosition(x,y+3);    
        for(i=0;i<18;i++)     
          sh1107_SendData(FONT_18x36[c*90+54+i]); 
        sh1107_SetPosition(x,y+4);    
        for(i=0;i<18;i++)     
          sh1107_SendData(FONT_18x36[c*90+72+i]);         
        x+=18;
        j++;
    }     
}

void sh1107_DrawPixe14x38(uint8_t x, uint8_t y, uint8_t ch[])
{
    unsigned char c=0,i=0,j=0;

    while (ch[j]!='\0')
    {    
        c = ch[j]-0x20; // start from " " space.  

        sh1107_SetPosition(x,y);    
        for(i=0;i<14;i++)     
            sh1107_SendData(FONT_14x38[c*70+i]);
        sh1107_SetPosition(x,y+1);    
        for(i=0;i<14;i++)     
          sh1107_SendData(FONT_14x38[c*70+14+i]);  
        sh1107_SetPosition(x,y+2);    
        for(i=0;i<14;i++)     
          sh1107_SendData(FONT_14x38[c*70+28+i]);          
        sh1107_SetPosition(x,y+3);    
        for(i=0;i<14;i++)     
          sh1107_SendData(FONT_14x38[c*70+42+i]); 
        sh1107_SetPosition(x,y+4);    
        for(i=0;i<14;i++)     
          sh1107_SendData(FONT_14x38[c*70+56+i]); 

        x+=14;
        j++;
    }     
}

void sh1107_DrawBitmap(uint8_t x0, uint8_t y0,uint8_t x1, uint8_t y1,uint8_t BMP[])
{
    uint32_t j=0;
    uint8_t x,y;

    if(y1%8==0) y=y1/8;
    else y=y1/8+1;

    for(y=y0;y<y1;y++)
    {
        sh1107_SetPosition(x0-1,y);
        for(x=x0;x<x1;x++)
        {
            sh1107_SendData(BMP[j++]);
        }
    }
}

