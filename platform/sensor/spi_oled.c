
/**
 ****************************************************************************************
 *
 * @file spi_oled.c
 *
 * @brief SFL & OLED Configuration Functions.
 *
 * Copyright (C) 2016. Dialog Semiconductor Ltd, unpublished work. This computer
 * program includes Confidential, Proprietary Information and is a Trade Secret of
 * Dialog Semiconductor Ltd.  All use, disclosure, and/or reproduction is prohibited
 * unless authorized in writing. All Rights Reserved.
 *
 * <bluetooth.support@diasemi.com> and contributors.
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup APP
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************

 */
//#include "CC_global_config.h"
#define OLED_EN
#ifdef  OLED_EN

#include <stddef.h>

#include "spi_oled.h"
#include "spi_sensor.h"
#include "drvi_gpio.h"
#include "drvi_spi.h"

#define SPI2_OLED_PWR_EN    15
#define SPI2_OLED_RESET     31
#define SPI2_OLED_DC        14
#define SPI_ID 2
#define OLED_DELAY(x) delayns(x*1000)

static volatile bool spi_oled_xfer_done;  /**< Flag used to indicate that SPI instance completed the transfer. */

void spi_oled_event_handler(E_DrviSpiEvent * p_event)
{
    spi_oled_xfer_done = true;
}

void set_oled_pwrctl_hi( void )
{
    //setGpioOutputHigh( SPI2_OLED_PWR_EN);
    drvi_gpio_write(SPI2_OLED_PWR_EN, 1);
}
void set_oled_pwrctl_lo( void )
{
    //setGpioOutputLow(SPI2_OLED_PWR_EN);
    drvi_gpio_write(SPI2_OLED_PWR_EN, 0);
}
void set_oled_dc_hi( void )
{
    //setGpioOutputHigh( SPI2_OLED_DC);
    drvi_gpio_write(SPI2_OLED_DC, 1);
}
void set_oled_dc_lo( void )
{
    //setGpioOutputLow(SPI2_OLED_DC);
    drvi_gpio_write(SPI2_OLED_DC, 0);
}
void set_oled_reset_hi( void )
{
    //setGpioOutputHigh( SPI2_OLED_RESET );
    drvi_gpio_write(SPI2_OLED_RESET, 1);
}
void set_oled_reset_lo( void )
{
    //setGpioOutputLow( SPI2_OLED_RESET);
    drvi_gpio_write(SPI2_OLED_RESET, 0);
}

void oled_spi_init( void )
{
    T_SpiDevice spi;

    set_oled_pwrctl_lo();
    set_oled_dc_lo();

    spi.bBusNum = 2;
    spi.wMode = DRVI_SPI_MODE_3;
    spi.fpComplete = spi_oled_event_handler;

    drvi_SpiInit(&spi);
}

/**
 ****************************************************************************************
 * @brief releases SPI
 ****************************************************************************************
 */

void ssd1306_init(void)
{
    // Initialize the interface
    ssd1306_interface_init();
    OLED_DELAY(100);
    // Do a hard reset of the OLED display controller
    ssd1306_hard_reset();

    ssd1306_write_command(SSD1306_CMD_SET_DISPLAY_OFF); //0xAE

    ssd1306_write_command(SSD1306_CMD_SET_DISPLAY_CLOCK_DIVIDE_RATIO); //0xD5
    ssd1306_write_command(0x80);    //0x80

    // 1/32 Duty (0x0F~0x3F)
    ssd1306_write_command(SSD1306_CMD_SET_MULTIPLEX_RATIO); //0xA8
    ssd1306_write_command(0x1F);    //0x1f

    // Shift Mapping RAM Counter (0x00~0x3F)
    ssd1306_write_command(SSD1306_CMD_SET_DISPLAY_OFFSET);  //0xD3
    ssd1306_write_command(0x00); //0x00

    // Set Mapping RAM Display Start Line (0x00~0x3F)
    ssd1306_write_command(SSD1306_CMD_SET_DISPLAY_START_LINE(0x00));    //0x40

    // Enable charge pump regulator
    ssd1306_write_command(SSD1306_CMD_SET_CHARGE_PUMP_SETTING);     //0x8D
    ssd1306_write_command(0x14);    //0x14

    // Set Column Address 0 Mapped to SEG0
    ssd1306_write_command(SSD1306_CMD_SET_SEGMENT_RE_MAP_COL127_SEG0);  //0xA1

    ssd1306_write_command(0xc8); //0xC8

    // Set COM Pins hardware configuration
    ssd1306_write_command(SSD1306_CMD_SET_COM_PINS);        //0xDA
    ssd1306_write_command(0x02); //Annars kastas linjerna om fel, kolla upp varfied

    ssd1306_write_command(SSD1306_CMD_SET_CONTRAST_CONTROL_FOR_BANK0);  //0x81
    ssd1306_write_command(0x8f);    //0x8f

    // Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    ssd1306_write_command(SSD1306_CMD_SET_PRE_CHARGE_PERIOD);   //0xD9
    ssd1306_write_command(0xf1);   //0xF1

    // Set VCOMH Deselect Level
    ssd1306_write_command(SSD1306_CMD_SET_VCOMH_DESELECT_LEVEL);    //0xDb
    ssd1306_write_command(0x40); // Default => 0x20 (0.77*VCC)  //0x40

    // Disable Entire display On
    ssd1306_write_command(0xa4);    //0xA4

    ssd1306_write_command(0xA6);    //0xA6
    //OLED_DELAY(100);
    OLED_Display_Black();

    ssd1306_display_on();   //0xAf

}

void ssd1306_reset(void)
{
    OLED_DELAY(10);
    set_oled_reset_lo();
    OLED_DELAY(50);
    set_oled_reset_lo();
    OLED_DELAY(100);
}
void	ssd1306_hard_reset(void)
{

    set_oled_reset_lo();
    OLED_DELAY(300);
    set_oled_reset_hi();
    OLED_DELAY(300);
    set_oled_reset_lo();
    OLED_DELAY(300);


    set_oled_pwrctl_hi(); //close vddb
    OLED_DELAY(300);
    set_oled_pwrctl_lo(); //open vddb
    OLED_DELAY(300);

    set_oled_reset_hi();
    OLED_DELAY(10);
}

void ssd1306_interface_init(void)
{
    oled_spi_init();
}

void ssd1306_write_command(uint8_t command)
{
    set_oled_dc_lo();

    spi_data_write(command, NULL, 0, SPI_ID);
}

void ssd1306_write_data(uint8_t command)
{
    set_oled_dc_hi();

    spi_data_write(command, NULL, 0, SPI_ID);
}

void ssd1306_display_on(void)
{
    ssd1306_write_command(SSD1306_CMD_SET_DISPLAY_ON);

}
void ssd1306_display_off(void)
{
    ssd1306_write_command(SSD1306_CMD_SET_DISPLAY_OFF);
}
#endif
