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
*  This is SSD1306 driver using SPI interface
*
*  Author:
*  -------
*  Blake
*
*===========================================================================/
*  20170804 Blake initial version
******************************************************************************/

#include "drvi_gpio.h"
#include "drvi_spi.h"

#include "ssd1306.h"
#include "ssd1306_128x32.h"

#define OLED_DELAY(x) delayns(x*1000)

static volatile int8_t cSpiXferDone;

static void ssd1306_SetPwrCtlHigh(void)
{
    drvi_gpio_write(OLED_SSD1306_PWR, 1);
}

static void ssd1306_SetPwrCtlLow(void)
{
    drvi_gpio_write(OLED_SSD1306_PWR, 0);
}

static void ssd1306_SetDataCmdHigh(void)
{
    drvi_gpio_write(OLED_SSD1306_DC, 1);
}

static void ssd1306_SetDataCmdLow(void)
{
    drvi_gpio_write(OLED_SSD1306_DC, 0);
}

static void ssd1306_SetResetHigh(void)
{
    drvi_gpio_write(OLED_SSD1306_RST, 1);
}

static void ssd1306_SetResetLow(void)
{
    drvi_gpio_write(OLED_SSD1306_RST, 0);
}

//static void ssd1306_EventHandlerCB(E_DrviSpiEvent * p_event)
//{
//    cSpiXferDone = 1;
//}

static void ssd1306_SpiInit(void)
{
    ssd1306_SetPwrCtlHigh();
    ssd1306_SetDataCmdLow();

    //ssd1306BoardInfo.fpComplete = ssd1306_EventHandlerCB;

    drvi_SpiInit(OLED_SSD1306_SPI);
}

void ssd1306_SendCommand(uint8_t bByte)
{
    ssd1306_SetDataCmdLow();
    drvi_SpiWrite(OLED_SSD1306_SPI, &bByte, sizeof(bByte));
}

void ssd1306_SendData(uint8_t bByte)
{
    ssd1306_SetDataCmdHigh();
    drvi_SpiWrite(OLED_SSD1306_SPI, &bByte, sizeof(bByte));
}

void ssd1306_Init(void)
{
    // Initialize the interface
    ssd1306_SpiInit();
    OLED_DELAY(100);
    // Do a hard reset of the OLED display controller
    ssd1306_HardReset();

    ssd1306_SendCommand(SSD1306_CMD_SET_DISPLAY_OFF); //0xAE

    ssd1306_SendCommand(SSD1306_CMD_SET_DISPLAY_CLOCK_DIVIDE_RATIO); //0xD5
    ssd1306_SendCommand(0x80);    //0x80

    // 1/32 Duty (0x0F~0x3F)
    ssd1306_SendCommand(SSD1306_CMD_SET_MULTIPLEX_RATIO); //0xA8
    ssd1306_SendCommand(0x1F);    //0x1f

    // Shift Mapping RAM Counter (0x00~0x3F)
    ssd1306_SendCommand(SSD1306_CMD_SET_DISPLAY_OFFSET); //0xD3
    ssd1306_SendCommand(0x00);    //0x00

    // Set Mapping RAM Display Start Line (0x00~0x3F)
    ssd1306_SendCommand(SSD1306_CMD_SET_DISPLAY_START_LINE(0x00)); //0x40

    // Enable charge pump regulator
    ssd1306_SendCommand(SSD1306_CMD_SET_CHARGE_PUMP_SETTING); //0x8D
    ssd1306_SendCommand(0x14);    //0x14

    // Set Column Address 0 Mapped to SEG0
    ssd1306_SendCommand(SSD1306_CMD_SET_SEGMENT_RE_MAP_COL127_SEG0); //0xA1

    ssd1306_SendCommand(0xc8);    //0xC8

    // Set COM Pins hardware configuration
    ssd1306_SendCommand(SSD1306_CMD_SET_COM_PINS); //0xDA
    ssd1306_SendCommand(0x02); //Annars kastas linjerna om fel, kolla upp varfied

    ssd1306_SendCommand(SSD1306_CMD_SET_CONTRAST_CONTROL_FOR_BANK0); //0x81
    ssd1306_SendCommand(0x8f);    //0x8f

    // Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    ssd1306_SendCommand(SSD1306_CMD_SET_PRE_CHARGE_PERIOD); //0xD9
    ssd1306_SendCommand(0xf1);    //0xF1

    // Set VCOMH Deselect Level
    ssd1306_SendCommand(SSD1306_CMD_SET_VCOMH_DESELECT_LEVEL); //0xDb
    ssd1306_SendCommand(0x40);    // Default => 0x20 (0.77*VCC) //0x40

    // Disable Entire display On
    ssd1306_SendCommand(0xa4);    //0xA4

    ssd1306_SendCommand(0xA6);    //0xA6
    //OLED_DELAY(100);
    ssd1306_DrawBlack();

    ssd1306_DisplayOn();   //0xAf

}

void ssd1306_Reset(void)
{
    OLED_DELAY(10);
    ssd1306_SetResetLow();
    OLED_DELAY(50);
    ssd1306_SetResetLow();
    OLED_DELAY(100);
}

void ssd1306_HardReset(void)
{

    ssd1306_SetResetLow();
    OLED_DELAY(300);
    ssd1306_SetResetHigh();
    OLED_DELAY(300);
    ssd1306_SetResetLow();
    OLED_DELAY(300);


    ssd1306_SetPwrCtlHigh(); //close vddb
    OLED_DELAY(300);
    ssd1306_SetPwrCtlHigh(); //open vddb
    OLED_DELAY(300);

    ssd1306_SetResetHigh();
    OLED_DELAY(10);
}

void ssd1306_DisplayOn(void)
{
    ssd1306_SendCommand(SSD1306_CMD_SET_DISPLAY_ON);

}
void ssd1306_DisplayOff(void)
{
    ssd1306_SendCommand(SSD1306_CMD_SET_DISPLAY_OFF);
}

void ssd1306_Sleep(void)
{
    ssd1306_SendCommand(0xAE);
    ssd1306_SendCommand(0x8D); //Set Charge bump
    ssd1306_SendCommand(0x10);
    OLED_DELAY(100);
    ssd1306_SetPwrCtlLow();
}

void ssd1306_WakeUp(void)
{
    ssd1306_SetPwrCtlLow();
    OLED_DELAY(100);
    ssd1306_SendCommand(0x8D); //Set Charge bump
    ssd1306_SendCommand(0x14);
    ssd1306_SendCommand(0xAF);
}
