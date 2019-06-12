/* Copyright (c) 2019 Cloudchip, Inc. All Rights Reserved.
 *
 * The information contained herein is property of Cloudchip, Inc.
 * Terms and conditions of usage are described in detail in CLOUDCHIP
 * STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information.
 * NO WARRANTY of ANY KIND is provided. This heading must NOT be removed 
 * from the file.
 */

/*
 * INCLUDES
 ****************************************************************************************
 */
#include <stdio.h>
#include "ll.h"
#include "drvi_init.h"
#include "drvi_gpio.h"
#include "tracer.h"
#include "project.h"

extern void sys_InitMain(void);
extern void cc6801_ClockDelayMs(volatile uint32_t);

void gpio_irq_callback(void)
{
    printf ("%s\r\n", __func__);
}

/*
 * MAIN FUNCTION
 ****************************************************************************************
 */
int main(void)
{

    //Must be first in main()
    sys_InitMain();

    //start interrupt handling
    GLOBAL_INT_START(); 

    drvi_initialize();

    TracerInfo("== CC6801 - GPIO - Start ==\r\n");

    /*******************************/
    /****** Application Start ******/
    /*******************************/

    drvi_RequestIrq(GPIO_PIN_31, gpio_irq_callback, IRQ_TYPE_EDGE_FALLING);
    drvi_EnableIrq(GPIO_PIN_31);

    while(1)
    {
        if(drvi_GpioRead(GPIO_PIN_28) == 0)
        {
            drvi_GpioWrite(GPIO_PIN_0, CC6801_GPIO_OUTPUT_HIGH);
        }
        else
        {
            drvi_GpioWrite(GPIO_PIN_0, CC6801_GPIO_OUTPUT_LOW);
        }

        if(drvi_GpioRead(GPIO_PIN_29) == 0)
        {
            drvi_GpioWrite(GPIO_PIN_1, CC6801_GPIO_OUTPUT_HIGH);
        }
        else
        {
            drvi_GpioWrite(GPIO_PIN_1, CC6801_GPIO_OUTPUT_LOW);
        }

        if(drvi_GpioRead(GPIO_PIN_30) == 0)
        {
            drvi_GpioWrite(GPIO_PIN_2, CC6801_GPIO_OUTPUT_HIGH);
        }
        else
        {
            drvi_GpioWrite(GPIO_PIN_2, CC6801_GPIO_OUTPUT_LOW);
        }
    }
}
