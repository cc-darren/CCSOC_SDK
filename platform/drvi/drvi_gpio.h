/**
 ****************************************************************************************
 *
 * @file drvi_gpio.h
 *
 * @brief Head file
 *
 * Copyright (C) CloudChip 2017-2019
 *
 *
 ****************************************************************************************
 */
#ifndef _DRVI_GPIO_H_
#define _DRVI_GPIO_H_
#include "project.h"
#include "gpio.h"

#define GPIO_PIN_0               0
#define GPIO_PIN_1               1
#define GPIO_PIN_2               2
#define GPIO_PIN_3               3
#define GPIO_PIN_4               4
#define GPIO_PIN_5               5
#define GPIO_PIN_6               6
#define GPIO_PIN_7               7
#define GPIO_PIN_8               8
#define GPIO_PIN_9               9
#define GPIO_PIN_10              10
#define GPIO_PIN_11              11
#define GPIO_PIN_12              12
#define GPIO_PIN_13              13
#define GPIO_PIN_14              14
#define GPIO_PIN_15              15
#define GPIO_PIN_16              16
#define GPIO_PIN_17              17
#define GPIO_PIN_18              18
#define GPIO_PIN_19              19
#define GPIO_PIN_20              20
#define GPIO_PIN_21              21
#define GPIO_PIN_22              22
#define GPIO_PIN_23              23
#define GPIO_PIN_24              24
#define GPIO_PIN_25              25
#define GPIO_PIN_26              26
#define GPIO_PIN_27              27
#define GPIO_PIN_28              28
#define GPIO_PIN_29              29
#define GPIO_PIN_30              30
#define GPIO_PIN_31              31
#define GPIO_PIN_32              32
#define GPIO_PIN_33              33
#define GPIO_PIN_34              34
#define GPIO_PIN_35              35
#define GPIO_PIN_36              36
#define GPIO_PIN_37              37
#define GPIO_PIN_38              38
#define GPIO_PIN_39              39
#define GPIO_PIN_40              40
#define GPIO_PIN_41              41
#define GPIO_PIN_42              42
#define GPIO_PIN_43              43
#define GPIO_PIN_44              44
#define GPIO_PIN_45              45
#define GPIO_PIN_46              46
#define GPIO_PIN_47              47


/*
 * IRQ line status.
 *
 * IRQ_TYPE_NONE            - default, unspecified type
 * IRQ_TYPE_EDGE_RISING     - rising edge triggered
 * IRQ_TYPE_EDGE_FALLING    - falling edge triggered
 * IRQ_TYPE_EDGE_BOTH       - rising and falling edge triggered
 * IRQ_TYPE_LEVEL_HIGH      - high level triggered
 * IRQ_TYPE_LEVEL_LOW       - low level triggered
 * IRQ_TYPE_LEVEL_MASK      - Mask to filter out the level bits
 * IRQ_TYPE_SENSE_MASK      - Mask for all the above bits
 * IRQ_TYPE_PROBE           - Special flag for probing in progress
 */
enum
{
    IRQ_TYPE_NONE           = 0x00000000,
    IRQ_TYPE_EDGE_RISING    = 0x00000001,
    IRQ_TYPE_EDGE_FALLING   = 0x00000002,
    IRQ_TYPE_EDGE_BOTH      = (IRQ_TYPE_EDGE_FALLING | IRQ_TYPE_EDGE_RISING),
    IRQ_TYPE_LEVEL_HIGH     = 0x00000004,
    IRQ_TYPE_LEVEL_LOW      = 0x00000008,
    IRQ_TYPE_LEVEL_MASK     = (IRQ_TYPE_LEVEL_LOW | IRQ_TYPE_LEVEL_HIGH),
    IRQ_TYPE_SENSE_MASK     = 0x0000000f,

    IRQ_TYPE_PROBE      = 0x00000010,
};

typedef void (*fpGpiIrqHandler)(void);


__forceinline int drvi_GpioPinMuxInit(void)
{
    return cc6801_gpio_pinmux_init();
}

__forceinline void drvi_GpioDirectInput(uint8_t bPinNum)
{
    cc6801_GpioDirectInput(bPinNum);
}

__forceinline void drvi_GpioDirectOutput(uint8_t bPinNum)
{
    cc6801_GpioDirectOutput(bPinNum);
}

__forceinline uint32_t drvi_GpioRead(uint8_t bPinNum)
{
    return cc6801_GpioRead(bPinNum);
}

__forceinline void drvi_GpioWrite(uint8_t bPinNum, uint8_t bVal)
{
    cc6801_GpioWrite(bPinNum, bVal);
}

__forceinline void drvi_EnableIrq(uint8_t bPinNum)
{
    cc6801_IrqEnable(bPinNum);
}

__forceinline void drvi_DisableIrq(uint8_t bPinNum)
{
    cc6801_IrqDisable(bPinNum);
}

__forceinline void drvi_RequestIrq(uint8_t bPinNum, fpGpiIrqHandler callback, uint32_t dwType)
{
    cc6801_RequestIrq(bPinNum, callback, dwType);
}



#endif //_DRVI_GPIO_H_


