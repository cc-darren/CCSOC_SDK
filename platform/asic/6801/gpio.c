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

static fpGpiIrqHandler g_fpGpiIsr[NUMBER_OF_PINS] = {0};

void GPIO_IRQHandler(void)
{
    uint32_t dwPinNum;
    fpGpiIrqHandler callback;

    volatile uint32_t dwIntStatus;
    uint32_t dwMask;

    //get vectors
    dwIntStatus = *((volatile unsigned int *)(&regGPIO0->dw.intSts));

    while(dwIntStatus)
    {
        dwMask = dwIntStatus & (~dwIntStatus+1);
        switch(dwMask)
        {
            case 0x00000001: dwPinNum = 0;    break;
            case 0x00000002: dwPinNum = 1;    break;
            case 0x00000004: dwPinNum = 2;    break;
            case 0x00000008: dwPinNum = 3;    break;
            case 0x00000010: dwPinNum = 4;    break;
            case 0x00000020: dwPinNum = 5;    break;
            case 0x00000040: dwPinNum = 6;    break;
            case 0x00000080: dwPinNum = 7;    break;
            case 0x00000100: dwPinNum = 8;    break;
            case 0x00000200: dwPinNum = 9;    break;
            case 0x00000400: dwPinNum = 10;   break;
            case 0x00000800: dwPinNum = 11;   break;
            case 0x00001000: dwPinNum = 12;   break;
            case 0x00002000: dwPinNum = 13;   break;
            case 0x00004000: dwPinNum = 14;   break;
            case 0x00008000: dwPinNum = 15;   break;
            case 0x00010000: dwPinNum = 16;   break;
            case 0x00020000: dwPinNum = 17;   break;
            case 0x00040000: dwPinNum = 18;   break;
            case 0x00080000: dwPinNum = 19;   break;
            case 0x00100000: dwPinNum = 20;   break;
            case 0x00200000: dwPinNum = 21;   break;
            case 0x00400000: dwPinNum = 22;   break;
            case 0x00800000: dwPinNum = 23;   break;
            case 0x01000000: dwPinNum = 24;   break;
            case 0x02000000: dwPinNum = 25;   break;
            case 0x04000000: dwPinNum = 26;   break;
            case 0x08000000: dwPinNum = 27;   break;
            case 0x10000000: dwPinNum = 28;   break;
            case 0x20000000: dwPinNum = 29;   break;
            case 0x40000000: dwPinNum = 30;   break;
            case 0x80000000: dwPinNum = 31;   break;
            default: break;
        }

        callback = g_fpGpiIsr[dwPinNum];
        if (callback)
            callback();

        //Write 1 clear interrupt
        *((volatile uint32_t *)(&regGPIO0->dw.intSts)) = dwMask;
        dwIntStatus &= ~dwMask;
    }

    dwIntStatus = *((volatile uint32_t *)(&regGPIO1->dw.intSts));

    while(dwIntStatus)
    {
        dwMask = dwIntStatus & (~dwIntStatus+1);
        switch(dwMask)
        {
            case 0x00000001: dwPinNum = 32;   break;
            case 0x00000002: dwPinNum = 33;   break;
            case 0x00000004: dwPinNum = 34;   break;
            case 0x00000008: dwPinNum = 35;   break;
            case 0x00000010: dwPinNum = 36;   break;
            case 0x00000020: dwPinNum = 37;   break;
            case 0x00000040: dwPinNum = 38;   break;
            case 0x00000080: dwPinNum = 39;   break;
            case 0x00000100: dwPinNum = 40;   break;
            case 0x00000200: dwPinNum = 41;   break;
            case 0x00000400: dwPinNum = 42;   break;
            case 0x00000800: dwPinNum = 43;   break;
            case 0x00001000: dwPinNum = 44;   break;
            case 0x00002000: dwPinNum = 45;   break;
            case 0x00004000: dwPinNum = 46;   break;
            case 0x00008000: dwPinNum = 47;   break;
            default: break;
        }

        callback = g_fpGpiIsr[dwPinNum];
        if (callback)
            callback();

        //Write 1 clear interrupt
        *((volatile uint32_t *)(&regGPIO1->dw.intSts)) = dwMask;
        dwIntStatus &= ~dwMask;
    }

    GPIO_INTR = 1;
}

void cc6801_IrqEnable(uint32_t dwPinNum)
{
    REG_GPIO(dwPinNum)->dw.intSts |= (1UL << PIN(dwPinNum));
    REG_GPIO(dwPinNum)->dw.intEn |= (1UL << PIN(dwPinNum));
}

void cc6801_IrqDisable(uint32_t dwPinNum)
{
    REG_GPIO(dwPinNum)->dw.intEn &= ~(1UL << PIN(dwPinNum));
}

static int cc6801_IrqTypeSet(uint32_t dwPinNum, uint32_t dwType)
{

    switch (dwType & IRQ_TYPE_SENSE_MASK)
    {
        case IRQ_TYPE_EDGE_RISING:
            REG_GPIO(dwPinNum)->dw.intType |= (1 << PIN(dwPinNum));
            REG_GPIO(dwPinNum)->dw.intPolarity &= ~(1 << PIN(dwPinNum));
            break;

        case IRQ_TYPE_EDGE_FALLING:
            REG_GPIO(dwPinNum)->dw.intType |= (1 << PIN(dwPinNum));
            REG_GPIO(dwPinNum)->dw.intPolarity |= (1 << PIN(dwPinNum));
            break;

        case IRQ_TYPE_LEVEL_HIGH:
            REG_GPIO(dwPinNum)->dw.intType &= ~(1 << PIN(dwPinNum));
            REG_GPIO(dwPinNum)->dw.intPolarity &= ~(1 << PIN(dwPinNum));
            break;

        case IRQ_TYPE_LEVEL_LOW:
            REG_GPIO(dwPinNum)->dw.intType &= ~(1 << PIN(dwPinNum));
            REG_GPIO(dwPinNum)->dw.intPolarity |= (1 << PIN(dwPinNum));
            break;

        default:
            return CC_ERROR_INVALID_DATA;
    }

    REG_GPIO(dwPinNum)->dw.intTrig |= (1UL << PIN(dwPinNum));

    return CC_SUCCESS;
}


void cc6801_RequestIrq(uint32_t dwPinNum,
                        fpGpiIrqHandler callback,
                        uint32_t dwType)
{
    g_fpGpiIsr[dwPinNum] = callback;
    cc6801_IrqTypeSet(dwPinNum, dwType);

    //Clear interrupt status before request
    REG_GPIO(dwPinNum)->dw.intSts |= (1UL << PIN(dwPinNum));
}


static void cc6801_GpioDirectSet(uint32_t dwPinNum, cc6801_gpio_dir_t eDir)
{
    if (eDir == CC6801_GPIO_DIR_OUTPUT)
    {
        REG_GPIO(dwPinNum)->dw.outputEn |= (1UL << PIN(dwPinNum));
    }
    else
    {
        REG_GPIO(dwPinNum)->dw.outputEn &= ~(1UL << PIN(dwPinNum));
    }
}

void cc6801_GpioDirectInput(uint32_t dwPinNum)
{
    cc6801_GpioDirectSet(dwPinNum, CC6801_GPIO_DIR_INPUT);
}

void cc6801_GpioDirectOutput(uint32_t dwPinNum)
{
    cc6801_GpioDirectSet(dwPinNum, CC6801_GPIO_DIR_OUTPUT);
}

static void cc6801_GpioSet(uint32_t dwPinNum)
{
    REG_GPIO(dwPinNum)->dw.output |= (1UL << PIN(dwPinNum));
}

static void cc6801_GpioClear(uint32_t dwPinNum)
{
    REG_GPIO(dwPinNum)->dw.output &= ~(1UL << PIN(dwPinNum));
}

uint32_t cc6801_GpioRead(uint32_t dwPinNum)
{
    return ((REG_GPIO(dwPinNum)->dw.input) >> PIN(dwPinNum)) & 1UL;
}

void cc6801_GpioWrite(uint32_t dwPinNum, uint32_t dwValue)
{
    if (dwValue == CC6801_GPIO_OUTPUT_LOW)
    {
        cc6801_GpioClear(dwPinNum);
    }
    else
    {
        cc6801_GpioSet(dwPinNum);
    }
}

void cc6801_GpioPuPdSet(uint32_t dwPinNum)
{
    REG_GPIO(dwPinNum)->dw.puEn |= (1UL << PIN(dwPinNum));
}

void cc6801_GpioPuPdClear(uint32_t dwPinNum)
{
    REG_GPIO(dwPinNum)->dw.puEn &= ~(1UL << PIN(dwPinNum));
}

void cc6801_GpioPinmuxSet(uint32_t dwPinNum)
{
    REG_GPIO(dwPinNum)->dw.pinmux |= (1UL << PIN(dwPinNum));
}

void cc6801_GpioPinmuxClear(uint32_t dwPinNum)
{
    REG_GPIO(dwPinNum)->dw.pinmux &= ~(1UL << PIN(dwPinNum));
}


void cc6801_GpioPortModeSet(U_regGPIO *pGpioBase,
                                   uint8_t bPort,
                                   uint8_t bMode)
{
    pGpioBase->dw.portModeSel = (pGpioBase->dw.portModeSel & ~CC6801_GPIO_PORT_MODE_MASK(bPort)) |
                                ((bMode << CC6801_GPIO_PORT_MODE_SHIFT(bPort)) &
                                CC6801_GPIO_PORT_MODE_MASK(bPort));
}

static void cc6801_GpioPinmuxConfig(uint8_t bPin, const uint8_t bConfig)
{
    if (bConfig & GPIO_PULL_UP)
        cc6801_GpioPuPdSet(bPin);
    else
        cc6801_GpioPuPdClear(bPin);

    if (bConfig & GPIO_PINMUX_ENABLE)
        cc6801_GpioPinmuxSet(bPin);
    else
        cc6801_GpioPinmuxClear(bPin);

    if (bConfig & GPIO_DIR_OUTPUT)
        cc6801_GpioDirectSet(bPin, CC6801_GPIO_DIR_OUTPUT);
    else
        cc6801_GpioDirectSet(bPin, CC6801_GPIO_DIR_INPUT);

    if (bConfig & GPIO_OUTPUT_HIGH)
        cc6801_GpioWrite(bPin, 1);
    else
        cc6801_GpioWrite(bPin, 0);
}

static void cc6801_GpioPinmuxTableConfig(const uint8_t *pConfig, int wLen)
{
    int wIndex;

    for (wIndex = 0; wIndex < wLen; wIndex++)
        cc6801_GpioPinmuxConfig(wIndex, pConfig[wIndex]);
}

static void cc6801_GpioPinGroupTableConfig(uint8_t *pPortModeTable, int wLen)
{
    U_regGPIO *pGpioBase;
    uint8_t bIndex, bPort;

    for (bIndex = 0; bIndex < wLen; bIndex++)
    {
        if (bIndex < NUMBER_OF_PORT_MODES_GPIO0)
        {
            pGpioBase = regGPIO0;
            bPort = bIndex;
        }
        else
        {
            pGpioBase = regGPIO1;
            bPort = bIndex - NUMBER_OF_PORT_MODES_GPIO0;
        }

        cc6801_GpioPortModeSet(pGpioBase, bPort, pPortModeTable[bIndex]);
    }
}

int cc6801_gpio_pinmux_init(void)
{
    uint8_t bGpioPortModeTable[] =
    {
        GPIO_MODE_PINGROUP0,
        GPIO_MODE_PINGROUP1,
        GPIO_MODE_PINGROUP2,
        GPIO_MODE_PINGROUP3,
        GPIO_MODE_PINGROUP4,
        GPIO_MODE_PINGROUP5,
        GPIO_MODE_PINGROUP6,
        GPIO_MODE_PINGROUP7,
        GPIO_MODE_PINGROUP8,
        GPIO_MODE_PINGROUP9,
        GPIO_MODE_PINGROUP10,
        GPIO_MODE_PINGROUP11
    };

    uint8_t bGpioConfig[] =
    {
        GPIO0_CONFIG,
        GPIO1_CONFIG,
        GPIO2_CONFIG,
        GPIO3_CONFIG,
        GPIO4_CONFIG,
        GPIO5_CONFIG,
        GPIO6_CONFIG,
        GPIO7_CONFIG,
        GPIO8_CONFIG,
        GPIO9_CONFIG,
        GPIO10_CONFIG,
        GPIO11_CONFIG,
        GPIO12_CONFIG,
        GPIO13_CONFIG,
        GPIO14_CONFIG,
        GPIO15_CONFIG,
        GPIO16_CONFIG,
        GPIO17_CONFIG,
        GPIO18_CONFIG,
        GPIO19_CONFIG,
        GPIO20_CONFIG,
        GPIO21_CONFIG,
        GPIO22_CONFIG,
        GPIO23_CONFIG,
        GPIO24_CONFIG,
        GPIO25_CONFIG,
        GPIO26_CONFIG,
        GPIO27_CONFIG,
        GPIO28_CONFIG,
        GPIO29_CONFIG,
        GPIO30_CONFIG,
        GPIO31_CONFIG,
        GPIO32_CONFIG,
        GPIO33_CONFIG,
        GPIO34_CONFIG,
        GPIO35_CONFIG,
        GPIO36_CONFIG,
        GPIO37_CONFIG,
        GPIO38_CONFIG,
        GPIO39_CONFIG,
        GPIO40_CONFIG,
        GPIO41_CONFIG,
        GPIO42_CONFIG,
        GPIO43_CONFIG,
        GPIO44_CONFIG,
        GPIO45_CONFIG,
        GPIO46_CONFIG,
        GPIO47_CONFIG
    };

    cc6801_GpioPinGroupTableConfig(bGpioPortModeTable,
                    ARRAY_SIZE(bGpioPortModeTable));

    cc6801_GpioPinmuxTableConfig(bGpioConfig,
                    ARRAY_SIZE(bGpioConfig));

    NVIC_EnableIRQ(GPIO_IRQn);
    return 0;
}

