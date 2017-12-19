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
*  This is SH1107 driver using SPI interface
*
*  Author:
*  -------
*  Blake
*
*===========================================================================/
*  20170804 Blake initial version
******************************************************************************/

#include "drvi_spi.h"

#include "sh1107.h"
#include "sh1107_128x32.h"

#define OLED_DELAY(x) delayns(x*1000)

static volatile int8_t cSpiXferDone;

void sh1107_SetPwrCtlHigh(void)
{
    drvi_GpioWrite(OLED_SH1107_PWR, 1);
}

void sh1107_SetPwrCtlLow(void)
{
    drvi_GpioWrite(OLED_SH1107_PWR, 0);
}

void sh1107_SetDataCmdHigh(void)
{
    drvi_GpioWrite(OLED_SH1107_DC, 1);
}

void sh1107_SetDataCmdLow(void)
{
    drvi_GpioWrite(OLED_SH1107_DC, 0);
}

void sh1107_SetResetHigh(void)
{
    drvi_GpioWrite(OLED_SH1107_RST, 1);
}

void sh1107_SetResetLow(void)
{
    drvi_GpioWrite(OLED_SH1107_RST, 0);
}

//static void sh1107_EventHandlerCB(E_DrviSpiEvent * p_event)
//{
//    cSpiXferDone = 1;
//}

static void sh1107_SpiInit(void)
{
    sh1107_SetPwrCtlHigh();
    sh1107_SetDataCmdLow();

    //sh1107BoardInfo.fpComplete = sh1107_EventHandlerCB;
}

void sh1107_SendCommand(uint8_t bByte)
{
    sh1107_SetDataCmdLow();
    drvi_SpiWrite(OLED_SH1107_SPI, &bByte, sizeof(bByte));
}

void sh1107_SendData(uint8_t bByte)
{
    sh1107_SetDataCmdHigh();
    drvi_SpiWrite(OLED_SH1107_SPI, &bByte, sizeof(bByte));
}

void sh1107_Init(void)
{
    // Initialize the interface
    sh1107_SpiInit();
    OLED_DELAY(100);
    // Do a hard reset of the OLED display controller
    sh1107_HardReset();

    sh1107_SendCommand(SH1107_CMD_SET_DISPLAY_OFF); //0xAE

    sh1107_SendCommand(SH1107_CMD_SET_DISPLAY_CLOCK_DIVIDE_RATIO);  //0xD5
    sh1107_SendCommand(0x51);


    sh1107_SendCommand(SH1107_CMD_SET_MEMORY_ADDRESSING_MODE);  //0x20
    
    //contrast control
    sh1107_SendCommand(SH1107_CMD_SET_CONTRAST_CONTROL_FOR_BANK0);  //0x81
    sh1107_SendCommand(0x4F);

    // DC/DC off
    sh1107_SendCommand(0xAD);
    sh1107_SendCommand(0x8A);

    // Segament remap
    sh1107_SendCommand(SH1107_CMD_SET_SEGMENT_RE_MAP_COL0_SEG0);    //0xA0

    // output scan dir
    sh1107_SendCommand(SH1107_CMD_SET_COM_OUTPUT_SCAN_UP);  //0xC0

    // Set dispaly start line
    sh1107_SendCommand(0xDC); //0xDC
    sh1107_SendCommand(0x00);

    // Set dispaly offset
    sh1107_SendCommand(SH1107_CMD_SET_DISPLAY_OFFSET); //0xD3
    sh1107_SendCommand(0x60);
    
    // Set pre-charge period
    sh1107_SendCommand(SH1107_CMD_SET_PRE_CHARGE_PERIOD); //0xD9
    sh1107_SendCommand(0x22);

    // Set VCOM select level
    sh1107_SendCommand(SH1107_CMD_SET_VCOMH_DESELECT_LEVEL); //0xDB
    sh1107_SendCommand(0x35);

    // Set Mutiplex ration
    sh1107_SendCommand(SH1107_CMD_SET_MULTIPLEX_RATIO); //0xA8
    sh1107_SendCommand(0x3F);    //0x3F

    // Disable Entire display On
    sh1107_SendCommand(0xa4);    //0xA4

    sh1107_SendCommand(0xA6);    //0xA6

    sh1107_DrawBlack();

    sh1107_DisplayOn();   //0xAf

}

void sh1107_Reset(void)
{
    OLED_DELAY(10);
    sh1107_SetResetLow();
    OLED_DELAY(50);
    sh1107_SetResetLow();
    OLED_DELAY(100);
}

void sh1107_HardReset(void)
{

    sh1107_SetResetLow();
    OLED_DELAY(300);
    sh1107_SetResetHigh();
    OLED_DELAY(300);
    sh1107_SetResetLow();
    OLED_DELAY(300);


    sh1107_SetPwrCtlHigh(); //close vddb
    OLED_DELAY(300);
    sh1107_SetPwrCtlHigh(); //open vddb
    OLED_DELAY(300);

    sh1107_SetResetHigh();
    OLED_DELAY(10);
}

void sh1107_DisplayOn(void)
{
    sh1107_SendCommand(SH1107_CMD_SET_DISPLAY_ON);

}
void sh1107_DisplayOff(void)
{
    sh1107_SendCommand(SH1107_CMD_SET_DISPLAY_OFF);
}

void sh1107_Sleep(void)
{
    sh1107_SendCommand(0xAE);
    sh1107_SendCommand(0x8D); //Set Charge bump
    sh1107_SendCommand(0x10);
    OLED_DELAY(100);
    sh1107_SetPwrCtlLow();
}

void sh1107_WakeUp(void)
{ 
    sh1107_SetPwrCtlHigh();
    OLED_DELAY(100);
    sh1107_SendCommand(0x8D); //Set Charge bump
    sh1107_SendCommand(0x14);
    sh1107_SendCommand(0xAF);
}
