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
    CC6801_GPIO_PORT_MODE_0 = 0,
    CC6801_GPIO_PORT_MODE_1,
    CC6801_GPIO_PORT_MODE_2,
    CC6801_GPIO_PORT_MODE_3,
    CC6801_GPIO_PORT_MODE_4,
    CC6801_GPIO_PORT_MODE_5,
    CC6801_GPIO_PORT_MODE_6,
    CC6801_GPIO_PORT_MODE_7,
    CC6801_GPIO_PORT_MODE_8,
    CC6801_GPIO_PORT_MODE_9,
    CC6801_GPIO_PORT_MODE_10,
    CC6801_GPIO_PORT_MODE_11,
} cc6801_gpio_port_mode_t;

#define CC6801_GPIO_PIN_0               0
#define CC6801_GPIO_PIN_1               1
#define CC6801_GPIO_PIN_2               2
#define CC6801_GPIO_PIN_3               3
#define CC6801_GPIO_PIN_4               4
#define CC6801_GPIO_PIN_5               5
#define CC6801_GPIO_PIN_6               6
#define CC6801_GPIO_PIN_7               7
#define CC6801_GPIO_PIN_8               8
#define CC6801_GPIO_PIN_9               9
#define CC6801_GPIO_PIN_10              10
#define CC6801_GPIO_PIN_11              11
#define CC6801_GPIO_PIN_12              12
#define CC6801_GPIO_PIN_13              13
#define CC6801_GPIO_PIN_14              14
#define CC6801_GPIO_PIN_15              15
#define CC6801_GPIO_PIN_16              16
#define CC6801_GPIO_PIN_17              17
#define CC6801_GPIO_PIN_18              18
#define CC6801_GPIO_PIN_19              19
#define CC6801_GPIO_PIN_20              20
#define CC6801_GPIO_PIN_21              21
#define CC6801_GPIO_PIN_22              22
#define CC6801_GPIO_PIN_23              23
#define CC6801_GPIO_PIN_24              24
#define CC6801_GPIO_PIN_25              25
#define CC6801_GPIO_PIN_26              26
#define CC6801_GPIO_PIN_27              27
#define CC6801_GPIO_PIN_28              28
#define CC6801_GPIO_PIN_29              29
#define CC6801_GPIO_PIN_30              30
#define CC6801_GPIO_PIN_31              31
#define CC6801_GPIO_PIN_32              32
#define CC6801_GPIO_PIN_33              33
#define CC6801_GPIO_PIN_34              34
#define CC6801_GPIO_PIN_35              35
#define CC6801_GPIO_PIN_36              36
#define CC6801_GPIO_PIN_37              37
#define CC6801_GPIO_PIN_38              38
#define CC6801_GPIO_PIN_39              39
#define CC6801_GPIO_PIN_40              40
#define CC6801_GPIO_PIN_41              41
#define CC6801_GPIO_PIN_42              42
#define CC6801_GPIO_PIN_43              43
#define CC6801_GPIO_PIN_44              44
#define CC6801_GPIO_PIN_45              45
#define CC6801_GPIO_PIN_46              46
#define CC6801_GPIO_PIN_47              47

#if 0
typedef enum {
    CC6801_GPIO_FUNC_GPIO              = 0,
    CC6801_GPIO_FUNC_SPI0_CLK             ,
    CC6801_GPIO_FUNC_SPI0_CS              ,
    CC6801_GPIO_FUNC_SPI0_DO              ,
    CC6801_GPIO_FUNC_SPI0_DI              ,
    CC6801_GPIO_FUNC_SPI1_CLK             ,
    CC6801_GPIO_FUNC_SPI1_CS              ,
    CC6801_GPIO_FUNC_SPI1_DO              ,
    CC6801_GPIO_FUNC_SPI1_DI              ,
    CC6801_GPIO_FUNC_SPI2_CLK             ,
    CC6801_GPIO_FUNC_SPI2_CS              ,
    CC6801_GPIO_FUNC_SPI2_DO              ,
    CC6801_GPIO_FUNC_SPI2_DI              ,
    CC6801_GPIO_FUNC_DMIC_CLK             ,
    CC6801_GPIO_FUNC_DMIC_DI              ,
    CC6801_GPIO_FUNC_I2C1_SCL             ,
    CC6801_GPIO_FUNC_I2C1_SDA             ,
    CC6801_GPIO_FUNC_I2C0_SCL             ,
    CC6801_GPIO_FUNC_I2C0_SDA             ,
    CC6801_GPIO_FUNC_UART0_TX             ,
    CC6801_GPIO_FUNC_UART0_RTS            ,
    CC6801_GPIO_FUNC_UART0_CTS            ,
    CC6801_GPIO_FUNC_UART0_RX             ,
    CC6801_GPIO_FUNC_PWM0                 ,
    CC6801_GPIO_FUNC_PWM1                 ,
    CC6801_GPIO_FUNC_I2S_CLK              ,
    CC6801_GPIO_FUNC_I2S_WS               ,
    CC6801_GPIO_FUNC_I2S_DO               ,
    CC6801_GPIO_FUNC_I2S_DI               ,
    CC6801_GPIO_FUNC_UART1_TX             ,
    CC6801_GPIO_FUNC_UART1_RTS            ,
    CC6801_GPIO_FUNC_UART1_CTS            ,
    CC6801_GPIO_FUNC_UART1_RX             ,
    CC6801_GPIO_FUNC_UART2_TX             ,
    CC6801_GPIO_FUNC_UART2_RTS            ,
    CC6801_GPIO_FUNC_UART2_CTS            ,
    CC6801_GPIO_FUNC_UART2_RX             ,
} cc6801_gpio_func_t;
#endif

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

typedef void (*cc6801_irq_handler)(void);

struct cc6801_gpio_config
{
    cc6801_gpio_pupd_t pupd;
    cc6801_gpio_pinmux_t pinmux;
    cc6801_gpio_dir_t io;
    cc6801_gpio_drive_t od;
};

int cc6801_gpio_pinmux_init(void);

__STATIC_INLINE void cc6801_gpio_set_dir(uint32_t pin_number, cc6801_gpio_dir_t dir)
{
    if (dir == CC6801_GPIO_DIR_OUTPUT)
    {
        REG_GPIO(pin_number)->dw.outputEn |= (1UL << PIN(pin_number));
    }
    else
    {
        REG_GPIO(pin_number)->dw.outputEn &= ~(1UL << PIN(pin_number));
    }
}

__STATIC_INLINE void cc6801_gpio_direction_input(uint32_t pin_number)
{
    cc6801_gpio_set_dir(pin_number, CC6801_GPIO_DIR_INPUT);
}

__STATIC_INLINE void cc6801_gpio_direction_output(uint32_t pin_number)
{
    cc6801_gpio_set_dir(pin_number, CC6801_GPIO_DIR_OUTPUT);
}

__STATIC_INLINE void cc6801_gpio_set(uint32_t pin_number)
{
    REG_GPIO(pin_number)->dw.output |= (1UL << PIN(pin_number));
}

__STATIC_INLINE void cc6801_gpio_clear(uint32_t pin_number)
{
    REG_GPIO(pin_number)->dw.output &= ~(1UL << PIN(pin_number));
}

__STATIC_INLINE uint32_t cc6801_gpio_read(uint32_t pin_number)
{
    return ((REG_GPIO(pin_number)->dw.input) >> PIN(pin_number)) & 1UL;
}

__STATIC_INLINE void cc6801_gpio_write(uint32_t pin_number, uint32_t value)
{
    if (value == CC6801_GPIO_OUTPUT_LOW)
    {
        cc6801_gpio_clear(pin_number);
    }
    else
    {
        cc6801_gpio_set(pin_number);
    }
}

__STATIC_INLINE void cc6801_gpio_set_pullup(uint32_t pin_number)
{
    REG_GPIO(pin_number)->dw.puEn |= (1UL << PIN(pin_number));
}

__STATIC_INLINE void cc6801_gpio_clear_pullup(uint32_t pin_number)
{
    REG_GPIO(pin_number)->dw.puEn &= ~(1UL << PIN(pin_number));
}

__STATIC_INLINE void cc6801_gpio_set_pinmux(uint32_t pin_number)
{
    REG_GPIO(pin_number)->dw.pinmux |= (1UL << PIN(pin_number));
}

__STATIC_INLINE void cc6801_gpio_clear_pinmux(uint32_t pin_number)
{
    REG_GPIO(pin_number)->dw.pinmux &= ~(1UL << PIN(pin_number));
}


__STATIC_INLINE void cc6801_gpio_set_port_mode(U_regGPIO *p_group, uint8_t port,
                                                        cc6801_gpio_port_mode_t mode)
{
    p_group->dw.portModeSel = (p_group->dw.portModeSel & ~CC6801_GPIO_PORT_MODE_MASK(port)) |
                              ((mode << CC6801_GPIO_PORT_MODE_SHIFT(port)) &
                                        CC6801_GPIO_PORT_MODE_MASK(port));
}

__STATIC_INLINE void cc6801_enable_irq(uint32_t pin_number)
{
    REG_GPIO(pin_number)->dw.intSts |= (1UL << PIN(pin_number));
    REG_GPIO(pin_number)->dw.intEn |= (1UL << PIN(pin_number));
}

__STATIC_INLINE void cc6801_disable_irq(uint32_t pin_number)
{
    REG_GPIO(pin_number)->dw.intEn &= ~(1UL << PIN(pin_number));
}

void cc6801_request_irq(uint32_t pin_number,
                        cc6801_irq_handler callback,
                        uint32_t type);

#endif //_GPIO_H_
