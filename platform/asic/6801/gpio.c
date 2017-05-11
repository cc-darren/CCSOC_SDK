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
#include "drvi_gpio.h"

volatile uint32_t GPIO_INTR = 0;

static cc6801_irq_handler cc6801_isr[NUMBER_OF_PINS] = {0};

void GPIO_IRQHandler(void)
{
    uint32_t pin_number;
    cc6801_irq_handler callback;

    volatile unsigned int int_status;
    unsigned int    mask;

    //get vectors
    int_status = *((volatile unsigned int *)(&regGPIO0->dw.intSts));

    while(int_status)
    {
        mask = int_status & (~int_status+1);
        switch(mask)
        {
            case 0x00000001: pin_number = 0;    break;
            case 0x00000002: pin_number = 1;    break;
            case 0x00000004: pin_number = 2;    break;
            case 0x00000008: pin_number = 3;    break;
            case 0x00000010: pin_number = 4;    break;
            case 0x00000020: pin_number = 5;    break;
            case 0x00000040: pin_number = 6;    break;
            case 0x00000080: pin_number = 7;    break;
            case 0x00000100: pin_number = 8;    break;
            case 0x00000200: pin_number = 9;    break;
            case 0x00000400: pin_number = 10;   break;
            case 0x00000800: pin_number = 11;   break;
            case 0x00001000: pin_number = 12;   break;
            case 0x00002000: pin_number = 13;   break;
            case 0x00004000: pin_number = 14;   break;
            case 0x00008000: pin_number = 15;   break;
            case 0x00010000: pin_number = 16;   break;
            case 0x00020000: pin_number = 17;   break;
            case 0x00040000: pin_number = 18;   break;
            case 0x00080000: pin_number = 19;   break;
            case 0x00100000: pin_number = 20;   break;
            case 0x00200000: pin_number = 21;   break;
            case 0x00400000: pin_number = 22;   break;
            case 0x00800000: pin_number = 23;   break;
            case 0x01000000: pin_number = 24;   break;
            case 0x02000000: pin_number = 25;   break;
            case 0x04000000: pin_number = 26;   break;
            case 0x08000000: pin_number = 27;   break;
            case 0x10000000: pin_number = 28;   break;
            case 0x20000000: pin_number = 29;   break;
            case 0x40000000: pin_number = 30;   break;
            case 0x80000000: pin_number = 31;   break;
            default: break;
        }

        callback = cc6801_isr[pin_number];
        if (callback)
            callback();

        //Write 1 clear interrupt
        *((volatile unsigned int *)(&regGPIO0->dw.intSts)) = mask;
        int_status &= ~mask;
    }

    int_status = *((volatile unsigned int *)(&regGPIO1->dw.intSts));

    while(int_status)
    {
        mask = int_status & (~int_status+1);
        switch(mask)
        {
            case 0x00000001: pin_number = 32;   break;
            case 0x00000002: pin_number = 33;   break;
            case 0x00000004: pin_number = 34;   break;
            case 0x00000008: pin_number = 35;   break;
            case 0x00000010: pin_number = 36;   break;
            case 0x00000020: pin_number = 37;   break;
            case 0x00000040: pin_number = 38;   break;
            case 0x00000080: pin_number = 39;   break;
            case 0x00000100: pin_number = 40;   break;
            case 0x00000200: pin_number = 41;   break;
            case 0x00000400: pin_number = 42;   break;
            case 0x00000800: pin_number = 43;   break;
            case 0x00001000: pin_number = 44;   break;
            case 0x00002000: pin_number = 45;   break;
            case 0x00004000: pin_number = 46;   break;
            case 0x00008000: pin_number = 47;   break;
            default: break;
        }

        callback = cc6801_isr[pin_number];
        if (callback)
            callback();

        //Write 1 clear interrupt
        *((volatile unsigned int *)(&regGPIO1->dw.intSts)) = mask;
        int_status &= ~mask;
    }

    GPIO_INTR = 1;
}

static int cc6801_gpio_irq_set_type(uint32_t pin_number, uint32_t type)
{

    switch (type & IRQ_TYPE_SENSE_MASK)
    {
        case IRQ_TYPE_EDGE_RISING:
            REG_GPIO(pin_number)->dw.intType |= (1 << PIN(pin_number));
            REG_GPIO(pin_number)->dw.intPolarity &= (0 << PIN(pin_number));
            break;

        case IRQ_TYPE_EDGE_FALLING:
            REG_GPIO(pin_number)->dw.intType |= (1 << PIN(pin_number));
            REG_GPIO(pin_number)->dw.intPolarity |= (1 << PIN(pin_number));
            break;

        case IRQ_TYPE_LEVEL_HIGH:
            REG_GPIO(pin_number)->dw.intType &= (0 << PIN(pin_number));
            REG_GPIO(pin_number)->dw.intPolarity &= (0 << PIN(pin_number));
            break;

        case IRQ_TYPE_LEVEL_LOW:
            REG_GPIO(pin_number)->dw.intType &= (0 << PIN(pin_number));
            REG_GPIO(pin_number)->dw.intPolarity |= (1 << PIN(pin_number));
            break;

        default:
            return CC_ERROR_INVALID_DATA;
    }

    REG_GPIO(pin_number)->dw.intTrig |= (1UL << PIN(pin_number));

    return CC_SUCCESS;
}


void cc6801_request_irq(uint32_t pin_number,
                        cc6801_irq_handler callback,
                        uint32_t type)
{
    cc6801_isr[pin_number] = callback;
    cc6801_gpio_irq_set_type(pin_number, type);

    //Clear interrupt status before request
    REG_GPIO(pin_number)->dw.intSts |= (1UL << PIN(pin_number));
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
/*                 PU-EN       AUX     OE      OUT             */
    DEFAULT_PINMUX(PULL_DOWN,  ENABLE, OUTPUT, HIGH),          //SPI0_CLK,
    DEFAULT_PINMUX(PULL_DOWN,  ENABLE, OUTPUT, HIGH),          //SPI0_CS,
    DEFAULT_PINMUX(PULL_DOWN,  ENABLE, OUTPUT, HIGH),          //SPI0_DO,
    DEFAULT_PINMUX(PULL_DOWN,  ENABLE,  INPUT, NOPULL),        //SPI0_DI,
    DEFAULT_PINMUX(PULL_DOWN,  ENABLE, OUTPUT, HIGH),          //SPI1_CLK,
    DEFAULT_PINMUX(PULL_DOWN,  ENABLE, OUTPUT, HIGH),          //SPI1_CS,
    DEFAULT_PINMUX(PULL_DOWN,  ENABLE, OUTPUT, HIGH),          //SPI1_DO,
    DEFAULT_PINMUX(PULL_DOWN,  ENABLE,  INPUT, NOPULL),        //SPI1_DI,
    DEFAULT_PINMUX(PULL_DOWN,  ENABLE, OUTPUT, HIGH),          //SPI2_CLK,
    DEFAULT_PINMUX(PULL_DOWN,  ENABLE, OUTPUT, HIGH),          //SPI2_CS,
    DEFAULT_PINMUX(PULL_DOWN,  ENABLE, OUTPUT, HIGH),          //SPI2_DO,
    DEFAULT_PINMUX(PULL_DOWN,  ENABLE,  INPUT, NOPULL),        //SPI2_DI,
    DEFAULT_PINMUX(PULL_DOWN,  ENABLE, OUTPUT, HIGH),          //DMIC_CLK,
    DEFAULT_PINMUX(PULL_DOWN,  ENABLE,  INPUT, NOPULL),        //DMIC_DI,
    DEFAULT_PINMUX(PULL_DOWN, DISABLE, OUTPUT, LOW),           //GPIO_14,
    DEFAULT_PINMUX(PULL_DOWN, DISABLE, OUTPUT, LOW),           //GPIO_15,
    DEFAULT_PINMUX(PULL_UP  ,  ENABLE,  INPUT, NOPULL),        //I2C1_SCL,
    DEFAULT_PINMUX(PULL_UP  ,  ENABLE,  INPUT, NOPULL),        //I2C1_SDA,
    DEFAULT_PINMUX(PULL_DOWN, DISABLE,  INPUT, NOPULL),        //GPIO_18,
    DEFAULT_PINMUX(PULL_DOWN, DISABLE,  INPUT, NOPULL),        //GPIO_19,
    DEFAULT_PINMUX(PULL_UP  ,  ENABLE,  INPUT, NOPULL),        //I2C0_SCL,
    DEFAULT_PINMUX(PULL_UP  ,  ENABLE,  INPUT, NOPULL),        //I2C0_SDA,
    DEFAULT_PINMUX(PULL_DOWN, DISABLE,  INPUT, NOPULL),        //GPIO_22,
    DEFAULT_PINMUX(PULL_DOWN, DISABLE,  INPUT, NOPULL),        //GPIO_23,
    DEFAULT_PINMUX(PULL_DOWN,  ENABLE, OUTPUT, HIGH),          //UART0_TX,
    DEFAULT_PINMUX(PULL_DOWN,  ENABLE, OUTPUT, HIGH),          //UART0_RTS,
    DEFAULT_PINMUX(PULL_DOWN,  ENABLE,  INPUT, NOPULL),        //UART0_CTS,
    DEFAULT_PINMUX(PULL_DOWN,  ENABLE,  INPUT, NOPULL),        //UART0_RX,
    DEFAULT_PINMUX(PULL_DOWN,  ENABLE, OUTPUT, HIGH),          //PWM0,
    DEFAULT_PINMUX(PULL_DOWN,  ENABLE, OUTPUT, HIGH),          //PWM1,
    DEFAULT_PINMUX(PULL_DOWN, DISABLE, OUTPUT, LOW),           //GPIO_30,
    DEFAULT_PINMUX(PULL_DOWN, DISABLE, OUTPUT, LOW),           //GPIO_31,
    DEFAULT_PINMUX(PULL_DOWN,  ENABLE, OUTPUT, HIGH),          //I2S_CLK,
    DEFAULT_PINMUX(PULL_DOWN,  ENABLE, OUTPUT, HIGH),          //I2S_WS,
    DEFAULT_PINMUX(PULL_DOWN,  ENABLE, OUTPUT, HIGH),          //I2S_DO,
    DEFAULT_PINMUX(PULL_DOWN,  ENABLE,  INPUT, NOPULL),        //I2S_DI,
    DEFAULT_PINMUX(PULL_DOWN,  ENABLE, OUTPUT, HIGH),          //UART1_TX,
    DEFAULT_PINMUX(PULL_DOWN,  ENABLE, OUTPUT, HIGH),          //UART1_RTS,
    DEFAULT_PINMUX(PULL_DOWN,  ENABLE,  INPUT, NOPULL),        //UART1_CTS,
    DEFAULT_PINMUX(PULL_DOWN,  ENABLE,  INPUT, NOPULL),        //UART1_RX,
    DEFAULT_PINMUX(PULL_DOWN,  ENABLE, OUTPUT, HIGH),          //UART2_TX,
    DEFAULT_PINMUX(PULL_DOWN,  ENABLE, OUTPUT, HIGH),          //UART2_RTS,
    DEFAULT_PINMUX(PULL_DOWN,  ENABLE,  INPUT, NOPULL),        //UART2_CTS,
    DEFAULT_PINMUX(PULL_DOWN,  ENABLE,  INPUT, NOPULL),        //UART2_RX,
    DEFAULT_PINMUX(PULL_DOWN, DISABLE, OUTPUT, HIGH),          //GPIO_44,
    DEFAULT_PINMUX(PULL_DOWN, DISABLE, OUTPUT, HIGH),          //GPIO_45,
    DEFAULT_PINMUX(PULL_DOWN, DISABLE, OUTPUT, HIGH),          //GPIO_46,
    DEFAULT_PINMUX(PULL_DOWN, DISABLE, OUTPUT, HIGH),          //GPIO_47,
};

static void cc6801_gpio_pinmux_config(uint8_t pin, const struct cc6801_gpio_config *config)
{
    cc6801_gpio_pupd_t pupd          = config->pupd;
    cc6801_gpio_pinmux_t pinmux      = config->pinmux;
    cc6801_gpio_dir_t io             = config->io;
    cc6801_gpio_drive_t od           = config->od;

    if (pinmux)
        cc6801_gpio_set_pinmux(pin);
    else
        cc6801_gpio_clear_pinmux(pin);

    if (io)
        cc6801_gpio_set_dir(pin, CC6801_GPIO_DIR_OUTPUT);
    else
        cc6801_gpio_set_dir(pin, CC6801_GPIO_DIR_INPUT);

    cc6801_gpio_write(pin, od);

    if (pupd)
        cc6801_gpio_set_pullup(pin);
    else
        cc6801_gpio_clear_pullup(pin);
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

    NVIC_EnableIRQ(GPIO_IRQn);
    return 0;
}

