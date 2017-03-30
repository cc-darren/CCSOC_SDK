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
*  gpio.c
*
*  Project:
*  --------
*  cc6801
*
*  Description:
*  ------------
*  This is GPIO driver
*
*  Author:
*  -------
*  CloudChip
*
*===========================================================================/
*  20170206 PAT initial version
******************************************************************************/

#include "gpio.h"

volatile uint32_t GPIO_INTR = 0;

void GPIO_IRQHandler(void)
{
    wr(GPIO_ADDR_BASE+0x10, 0x00000000);   // clear interrupt
    wr(GPIO_ADDR_BASE+0x3c, 0x00000000);   // clear interrupt
    GPIO_INTR = 1;
}

#define DEFAULT_PINMUX(_pupd, _pinmux, _io, _od)  \
    {                                                       \
        .pupd       = CC6801_GPIO_PUPD_##_pupd,                 \
        .pinmux     = CC6801_GPIO_PINMUX_##_pinmux,             \
        .io         = CC6801_GPIO_DIR_##_io,                    \
        .od         = CC6801_GPIO_##_io##_##_od,                \
    }

cc6801_gpio_port_mode_t cc6801_fpga_port_mode_table[] =
{
    CC6801_GPIO_PORT_MODE_1,
    CC6801_GPIO_PORT_MODE_1,
    CC6801_GPIO_PORT_MODE_1,
    CC6801_GPIO_PORT_MODE_5,
    CC6801_GPIO_PORT_MODE_7,
    CC6801_GPIO_PORT_MODE_7,
    CC6801_GPIO_PORT_MODE_4,
    CC6801_GPIO_PORT_MODE_9,
    CC6801_GPIO_PORT_MODE_3,
    CC6801_GPIO_PORT_MODE_4,
    CC6801_GPIO_PORT_MODE_4,
    CC6801_GPIO_PORT_MODE_0
};

static struct cc6801_gpio_config cc6801_fpga_pinmux[] =
{
    DEFAULT_PINMUX(PULL_DOWN, ENABLE, OUTPUT, HIGH),          //SPI0_CLK,
    DEFAULT_PINMUX(PULL_DOWN, ENABLE, OUTPUT, HIGH),          //SPI0_CS,
    DEFAULT_PINMUX(PULL_DOWN, ENABLE, OUTPUT, HIGH),          //SPI0_DO,
    DEFAULT_PINMUX(PULL_DOWN, ENABLE, INPUT,  NOPULL),        //SPI0_DI,
    DEFAULT_PINMUX(PULL_DOWN, ENABLE, OUTPUT, HIGH),          //SPI1_CLK,
    DEFAULT_PINMUX(PULL_DOWN, ENABLE, OUTPUT, HIGH),          //SPI1_CS,
    DEFAULT_PINMUX(PULL_DOWN, ENABLE, OUTPUT, HIGH),          //SPI1_DO,
    DEFAULT_PINMUX(PULL_DOWN, ENABLE, INPUT,  NOPULL),        //SPI1_DI,
    DEFAULT_PINMUX(PULL_DOWN, ENABLE, OUTPUT, HIGH),          //SPI2_CLK,
    DEFAULT_PINMUX(PULL_DOWN, ENABLE, OUTPUT, HIGH),          //SPI2_CS,
    DEFAULT_PINMUX(PULL_DOWN, ENABLE, OUTPUT, HIGH),          //SPI2_DO,
    DEFAULT_PINMUX(PULL_DOWN, ENABLE, INPUT,  NOPULL),        //SPI2_DI,
    DEFAULT_PINMUX(PULL_DOWN, ENABLE, OUTPUT, HIGH),          //DMIC_CLK,
    DEFAULT_PINMUX(PULL_DOWN, ENABLE, INPUT,  NOPULL),        //DMIC_DI,
    DEFAULT_PINMUX(PULL_DOWN, ENABLE, OUTPUT, HIGH),          //GPIO,
    DEFAULT_PINMUX(PULL_DOWN, ENABLE, OUTPUT, HIGH),          //GPIO,
    DEFAULT_PINMUX(PULL_DOWN, ENABLE, OUTPUT, HIGH),          //I2C1_SCL,
    DEFAULT_PINMUX(PULL_DOWN, ENABLE, OUTPUT, HIGH),          //I2C1_SDA,
    DEFAULT_PINMUX(PULL_DOWN, ENABLE, OUTPUT, HIGH),          //GPIO,
    DEFAULT_PINMUX(PULL_DOWN, ENABLE, OUTPUT, HIGH),          //GPIO,
    DEFAULT_PINMUX(PULL_DOWN, ENABLE, OUTPUT, HIGH),          //I2C0_SCL,
    DEFAULT_PINMUX(PULL_DOWN, ENABLE, OUTPUT, HIGH),          //I2C0_SDA,
    DEFAULT_PINMUX(PULL_DOWN, ENABLE, OUTPUT, HIGH),          //GPIO,
    DEFAULT_PINMUX(PULL_DOWN, ENABLE, OUTPUT, HIGH),          //GPIO,
    DEFAULT_PINMUX(PULL_DOWN, ENABLE, OUTPUT, HIGH),          //UART0_TX,
    DEFAULT_PINMUX(PULL_DOWN, ENABLE, OUTPUT, HIGH),          //UART0_RTS,
    DEFAULT_PINMUX(PULL_DOWN, ENABLE, INPUT,  NOPULL),        //UART0_CTS,
    DEFAULT_PINMUX(PULL_DOWN, ENABLE, INPUT,  NOPULL),        //UART0_RX,
    DEFAULT_PINMUX(PULL_DOWN, ENABLE, OUTPUT, HIGH),          //PWM0,
    DEFAULT_PINMUX(PULL_DOWN, ENABLE, OUTPUT, HIGH),          //PWM1,
    DEFAULT_PINMUX(PULL_DOWN, ENABLE, OUTPUT, HIGH),          //GPIO,
    DEFAULT_PINMUX(PULL_DOWN, ENABLE, OUTPUT, HIGH),          //GPIO,
    DEFAULT_PINMUX(PULL_DOWN, ENABLE, OUTPUT, HIGH),          //I2S_CLK,
    DEFAULT_PINMUX(PULL_DOWN, ENABLE, OUTPUT, HIGH),          //I2S_WS,
    DEFAULT_PINMUX(PULL_DOWN, ENABLE, OUTPUT, HIGH),          //I2S_DO,
    DEFAULT_PINMUX(PULL_DOWN, ENABLE, INPUT,  NOPULL),        //I2S_DI,
    DEFAULT_PINMUX(PULL_DOWN, ENABLE, OUTPUT, HIGH),          //UART1_TX,
    DEFAULT_PINMUX(PULL_DOWN, ENABLE, OUTPUT, HIGH),          //UART1_RTS,
    DEFAULT_PINMUX(PULL_DOWN, ENABLE, INPUT,  NOPULL),        //UART1_CTS,
    DEFAULT_PINMUX(PULL_DOWN, ENABLE, INPUT,  NOPULL),        //UART1_RX,
    DEFAULT_PINMUX(PULL_DOWN, ENABLE, OUTPUT, HIGH),          //UART2_TX,
    DEFAULT_PINMUX(PULL_DOWN, ENABLE, OUTPUT, HIGH),          //UART2_RTS,
    DEFAULT_PINMUX(PULL_DOWN, ENABLE, INPUT,  NOPULL),        //UART2_CTS,
    DEFAULT_PINMUX(PULL_DOWN, ENABLE, INPUT,  NOPULL),        //UART2_RX,
    DEFAULT_PINMUX(PULL_DOWN, ENABLE, OUTPUT, HIGH),          //GPIO,
    DEFAULT_PINMUX(PULL_DOWN, ENABLE, OUTPUT, HIGH),          //GPIO,
    DEFAULT_PINMUX(PULL_DOWN, ENABLE, OUTPUT, HIGH),          //GPIO,
    DEFAULT_PINMUX(PULL_DOWN, ENABLE, OUTPUT, HIGH),          //GPIO,
};

static void cc6801_gpio_pinmux_config(uint8_t pin, const struct cc6801_gpio_config *config)
{
    cc6801_gpio_pupd_t pupd          = config->pupd;
    cc6801_gpio_pinmux_t pinmux      = config->pinmux;
    cc6801_gpio_dir_t io             = config->io;
    cc6801_gpio_drive_t od           = config->od;

    if (pinmux)
        cc6801_gpio_set_pinmux(pin);

    if (io)
    {
        cc6801_gpio_set_dir(pin, CC6801_GPIO_DIR_OUTPUT);
        if (od)
            cc6801_gpio_write(pin, od);
    }

    if (pupd)
        cc6801_gpio_set_pullup(pin);

}

void cc6801_gpio_pinmux_config_table(const struct cc6801_gpio_config *config, int len)
{
    int i;

    for (i = 0; i < len; i++)
        cc6801_gpio_pinmux_config(i, &config[i]);
}

void cc6801_gpio_mode_config_table(cc6801_gpio_port_mode_t *mode, int len)
{
    U_regGPIO *group;
    uint8_t i, port;

    for (i = 0; i < len; i++)
    {
        if (i < NUMBER_OF_PORT_MODES_GPIO0)
        {
            group = regGPIO0;
            port = i;
        }
        else
        {
            group = regGPIO1;
            port = i - NUMBER_OF_PORT_MODES_GPIO0;
        }

        cc6801_gpio_set_port_mode(group, port, mode[i]);
    }
}

int cc6801_gpio_pinmux_init(void)
{
    cc6801_gpio_mode_config_table(cc6801_fpga_port_mode_table,
                    ARRAY_SIZE(cc6801_fpga_port_mode_table));

    cc6801_gpio_pinmux_config_table(cc6801_fpga_pinmux,
                    ARRAY_SIZE(cc6801_fpga_pinmux));

    return 0;
}

