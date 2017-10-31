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

#ifndef _GPIO_H_
#define _GPIO_H_

#include "global.h"
#include "cc6801_reg.h"

#define NUMBER_OF_PINS_GPIO0 32
#define NUMBER_OF_PINS_GPIO1 16
#define NUMBER_OF_PINS ((NUMBER_OF_PINS_GPIO0)+(NUMBER_OF_PINS_GPIO1))

#define REG_GPIO(pin) (((pin)<NUMBER_OF_PINS_GPIO0)? regGPIO0: regGPIO1)
#define PIN(pin) (((pin)<NUMBER_OF_PINS_GPIO0)? (pin): ((pin)-NUMBER_OF_PINS_GPIO0))

#define NUMBER_OF_PORT_MODES_GPIO0 8
#define NUMBER_OF_PORT_MODES_GPIO1 4
#define NUMBER_OF_PORT_MODES ((NUMBER_OF_PORT_MODES_GPIO0)+(NUMBER_OF_PORT_MODES_GPIO1))

#define CC6801_GPIO_PORT_MODE_SHIFT(x) (4*(x))
#define CC6801_GPIO_PORT_MODE_MASK(x)  ((0xFUL) << (4*(x)))

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

typedef enum
{
    CC6801_GPIO_PUPD_PULL_DOWN = 0,
    CC6801_GPIO_PUPD_PULL_UP,
} cc6801_gpio_pupd_t;

typedef enum
{
    CC6801_GPIO_INPUT_NOPULL = 0,
    CC6801_GPIO_OUTPUT_LOW = 0,
    CC6801_GPIO_OUTPUT_HIGH,
} cc6801_gpio_drive_t;

typedef enum
{
    CC6801_GPIO_PINMUX_DISABLE = 0,
    CC6801_GPIO_PINMUX_ENABLE,
} cc6801_gpio_pinmux_t;

typedef enum
{
    CC6801_GPIO_DIR_INPUT = 0,
    CC6801_GPIO_DIR_OUTPUT,
} cc6801_gpio_dir_t;

typedef enum
{
  CC6801_IRQ_POLARITY_HIGH,       ///<  Low to high.
  CC6801_IRQ_POLARITY_LOW,        ///<  High to low.
} cc6801_irq_polarity_t;


typedef enum
{
  CC6801_IRQ_TYPE_LEVEL,       ///<  Low to high.
  CC6801_IRQ_TYPE_EDGE,        ///<  High to low.
} cc6801_irq_type_t;

typedef void (*fpGpiIrqHandler)(void);

int cc6801_gpio_pinmux_init(void);

void cc6801_GpioDirectInput(uint8_t bPinNum);

void cc6801_GpioDirectOutput(uint8_t bPinNum);

uint32_t cc6801_GpioRead(uint8_t bPinNum);

void cc6801_GpioWrite(uint8_t bPinNum, uint8_t bVal);

void cc6801_GpioPuPdSet(uint8_t bPinNum);

void cc6801_GpioPuPdClear(uint8_t bPinNum);

void cc6801_GpioPinmuxSet(uint8_t bPinNum);

void cc6801_GpioPinmuxClear(uint8_t bPinNum);

void cc6801_GpioPortModeSet(U_regGPIO *p_group,
                                uint8_t port,
                                uint8_t mode);

void cc6801_IrqEnable(uint8_t bPinNum);

void cc6801_IrqDisable(uint8_t bPinNum);

void cc6801_RequestIrq(uint8_t bPinNum,
                        fpGpiIrqHandler callback,
                        uint32_t dwType);

#endif //_GPIO_H_
