/* Copyright (c) 2018 Cloudchip, Inc. All Rights Reserved.
 *
 * The information contained herein is property of Cloudchip, Inc.
 * Terms and conditions of usage are described in detail in CLOUDCHIP
 * STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information.
 * NO WARRANTY of ANY KIND is provided. This heading must NOT be removed 
 * from the file.
 */

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
    uint8_t bPinNum;
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
            case 0x00000001: bPinNum = 0;    break;
            case 0x00000002: bPinNum = 1;    break;
            case 0x00000004: bPinNum = 2;    break;
            case 0x00000008: bPinNum = 3;    break;
            case 0x00000010: bPinNum = 4;    break;
            case 0x00000020: bPinNum = 5;    break;
            case 0x00000040: bPinNum = 6;    break;
            case 0x00000080: bPinNum = 7;    break;
            case 0x00000100: bPinNum = 8;    break;
            case 0x00000200: bPinNum = 9;    break;
            case 0x00000400: bPinNum = 10;   break;
            case 0x00000800: bPinNum = 11;   break;
            case 0x00001000: bPinNum = 12;   break;
            case 0x00002000: bPinNum = 13;   break;
            case 0x00004000: bPinNum = 14;   break;
            case 0x00008000: bPinNum = 15;   break;
            case 0x00010000: bPinNum = 16;   break;
            case 0x00020000: bPinNum = 17;   break;
            case 0x00040000: bPinNum = 18;   break;
            case 0x00080000: bPinNum = 19;   break;
            case 0x00100000: bPinNum = 20;   break;
            case 0x00200000: bPinNum = 21;   break;
            case 0x00400000: bPinNum = 22;   break;
            case 0x00800000: bPinNum = 23;   break;
            case 0x01000000: bPinNum = 24;   break;
            case 0x02000000: bPinNum = 25;   break;
            case 0x04000000: bPinNum = 26;   break;
            case 0x08000000: bPinNum = 27;   break;
            case 0x10000000: bPinNum = 28;   break;
            case 0x20000000: bPinNum = 29;   break;
            case 0x40000000: bPinNum = 30;   break;
            case 0x80000000: bPinNum = 31;   break;
            default: break;
        }

        callback = g_fpGpiIsr[bPinNum];
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
            case 0x00000001: bPinNum = 32;   break;
            case 0x00000002: bPinNum = 33;   break;
            case 0x00000004: bPinNum = 34;   break;
            case 0x00000008: bPinNum = 35;   break;
            case 0x00000010: bPinNum = 36;   break;
            case 0x00000020: bPinNum = 37;   break;
            case 0x00000040: bPinNum = 38;   break;
            case 0x00000080: bPinNum = 39;   break;
            case 0x00000100: bPinNum = 40;   break;
            case 0x00000200: bPinNum = 41;   break;
            case 0x00000400: bPinNum = 42;   break;
            case 0x00000800: bPinNum = 43;   break;
            case 0x00001000: bPinNum = 44;   break;
            case 0x00002000: bPinNum = 45;   break;
            case 0x00004000: bPinNum = 46;   break;
            case 0x00008000: bPinNum = 47;   break;
            default: break;
        }

        callback = g_fpGpiIsr[bPinNum];
        if (callback)
            callback();

        //Write 1 clear interrupt
        *((volatile uint32_t *)(&regGPIO1->dw.intSts)) = dwMask;
        dwIntStatus &= ~dwMask;
    }

    GPIO_INTR = 1;
}

void cc6801_IrqEnable(uint8_t bPinNum)
{
    REG_GPIO(bPinNum)->dw.intSts |= (1UL << PIN(bPinNum));
    REG_GPIO(bPinNum)->dw.intEn |= (1UL << PIN(bPinNum));
}

void cc6801_IrqDisable(uint8_t bPinNum)
{
    REG_GPIO(bPinNum)->dw.intEn &= ~(1UL << PIN(bPinNum));
}

static int cc6801_IrqTypeSet(uint8_t bPinNum, uint32_t dwType)
{

    switch (dwType & IRQ_TYPE_SENSE_MASK)
    {
        case IRQ_TYPE_EDGE_RISING:
            REG_GPIO(bPinNum)->dw.intType |= (1 << PIN(bPinNum));
            REG_GPIO(bPinNum)->dw.intPolarity &= ~(1 << PIN(bPinNum));
            break;

        case IRQ_TYPE_EDGE_FALLING:
            REG_GPIO(bPinNum)->dw.intType |= (1 << PIN(bPinNum));
            REG_GPIO(bPinNum)->dw.intPolarity |= (1 << PIN(bPinNum));
            break;

        case IRQ_TYPE_LEVEL_HIGH:
            REG_GPIO(bPinNum)->dw.intType &= ~(1 << PIN(bPinNum));
            REG_GPIO(bPinNum)->dw.intPolarity &= ~(1 << PIN(bPinNum));
            break;

        case IRQ_TYPE_LEVEL_LOW:
            REG_GPIO(bPinNum)->dw.intType &= ~(1 << PIN(bPinNum));
            REG_GPIO(bPinNum)->dw.intPolarity |= (1 << PIN(bPinNum));
            break;

        default:
            return CC_ERROR_INVALID_DATA;
    }

    REG_GPIO(bPinNum)->dw.intTrig |= (1UL << PIN(bPinNum));

    return CC_SUCCESS;
}


void cc6801_RequestIrq(uint8_t bPinNum,
                        fpGpiIrqHandler callback,
                        uint32_t dwType)
{
    g_fpGpiIsr[bPinNum] = callback;
    cc6801_IrqTypeSet(bPinNum, dwType);

    //Clear interrupt status before request
    REG_GPIO(bPinNum)->dw.intSts |= (1UL << PIN(bPinNum));
}


static void cc6801_GpioDirectSet(uint8_t bPinNum, cc6801_gpio_dir_t eDir)
{
    if (eDir == CC6801_GPIO_DIR_OUTPUT)
    {
        REG_GPIO(bPinNum)->dw.outputEn |= (1UL << PIN(bPinNum));
    }
    else
    {
        REG_GPIO(bPinNum)->dw.outputEn &= ~(1UL << PIN(bPinNum));
    }
}

void cc6801_GpioDirectInput(uint8_t bPinNum)
{
    cc6801_GpioDirectSet(bPinNum, CC6801_GPIO_DIR_INPUT);
}

void cc6801_GpioDirectOutput(uint8_t bPinNum)
{
    cc6801_GpioDirectSet(bPinNum, CC6801_GPIO_DIR_OUTPUT);
}

static void cc6801_GpioSet(uint8_t bPinNum)
{
    REG_GPIO(bPinNum)->dw.output |= (1UL << PIN(bPinNum));
}

static void cc6801_GpioClear(uint8_t bPinNum)
{
    REG_GPIO(bPinNum)->dw.output &= ~(1UL << PIN(bPinNum));
}

uint32_t cc6801_GpioRead(uint8_t bPinNum)
{
    return ((REG_GPIO(bPinNum)->dw.input) >> PIN(bPinNum)) & 1UL;
}

void cc6801_GpioWrite(uint8_t bPinNum, uint8_t bVal)
{
    if (bVal == CC6801_GPIO_OUTPUT_LOW)
    {
        cc6801_GpioClear(bPinNum);
    }
    else
    {
        cc6801_GpioSet(bPinNum);
    }
}

void cc6801_GpioPuPdSet(uint8_t bPinNum)
{
    REG_GPIO(bPinNum)->dw.puEn |= (1UL << PIN(bPinNum));
}

void cc6801_GpioPuPdClear(uint8_t bPinNum)
{
    REG_GPIO(bPinNum)->dw.puEn &= ~(1UL << PIN(bPinNum));
}

void cc6801_GpioPinmuxSet(uint8_t bPinNum)
{
    REG_GPIO(bPinNum)->dw.pinmux |= (1UL << PIN(bPinNum));
}

void cc6801_GpioPinmuxClear(uint8_t bPinNum)
{
    REG_GPIO(bPinNum)->dw.pinmux &= ~(1UL << PIN(bPinNum));
}


void cc6801_GpioPortModeSet(U_regGPIO *pGpioBase,
                                   uint8_t bPort,
                                   uint8_t bMode)
{
    pGpioBase->dw.portModeSel = (pGpioBase->dw.portModeSel & ~CC6801_GPIO_PORT_MODE_MASK(bPort)) |
                                ((bMode << CC6801_GPIO_PORT_MODE_SHIFT(bPort)) &
                                CC6801_GPIO_PORT_MODE_MASK(bPort));
}

void cc6801_GpioPinmuxConfig(uint8_t bPin, const uint16_t wConfig)
{
    if (wConfig & GPIO_PULL_UP)
        cc6801_GpioPuPdSet(bPin);
    else
        cc6801_GpioPuPdClear(bPin);

    if (wConfig & GPIO_PINMUX_ENABLE)
        cc6801_GpioPinmuxSet(bPin);
    else
        cc6801_GpioPinmuxClear(bPin);

    if (wConfig & GPIO_DIR_OUTPUT)
        cc6801_GpioDirectSet(bPin, CC6801_GPIO_DIR_OUTPUT);
    else
        cc6801_GpioDirectSet(bPin, CC6801_GPIO_DIR_INPUT);

    if (wConfig & GPIO_OUTPUT_HIGH)
        cc6801_GpioWrite(bPin, 1);
    else
        cc6801_GpioWrite(bPin, 0);
}

static void cc6801_GpioPinmuxTableConfig(const uint16_t *pConfig, int wLen)
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

    uint16_t wGpioConfig[] =
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

    cc6801_GpioPinmuxTableConfig(wGpioConfig,
                    ARRAY_SIZE(wGpioConfig));

    NVIC_EnableIRQ(GPIO_IRQn);
    return 0;
}

