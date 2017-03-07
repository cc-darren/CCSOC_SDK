/**************************************************************************//**
 * @file     system_ARMCM0plus.c
 * @brief    CMSIS Device System Source File for
 *           ARMCM0plus Device Series
 * @version  V5.00
 * @date     08. April 2016
 ******************************************************************************/
/*
 * Copyright (c) 2009-2016 ARM Limited. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "ARMCM0plus.h"

/*----------------------------------------------------------------------------
  Define clocks
 *----------------------------------------------------------------------------*/
#define  XTAL            ( 5000000U)      /* Oscillator frequency */

#define  SYSTEM_CLOCK    (5 * XTAL)


/*----------------------------------------------------------------------------
  Externals
 *----------------------------------------------------------------------------*/
#if defined (__VTOR_PRESENT) && (__VTOR_PRESENT == 1)
    extern uint32_t __Vectors;
#endif

/*----------------------------------------------------------------------------
  System Core Clock Variable
 *----------------------------------------------------------------------------*/
uint32_t SystemCoreClock = SYSTEM_CLOCK;


/*----------------------------------------------------------------------------
  System Core Clock update function
 *----------------------------------------------------------------------------*/
void SystemCoreClockUpdate (void)
{
    SystemCoreClock = SYSTEM_CLOCK;
}

/*----------------------------------------------------------------------------
  System initialization function
 *----------------------------------------------------------------------------*/
void SystemInit (void)
{
    
#if defined (__VTOR_PRESENT) && (__VTOR_PRESENT == 1)
    SCB->VTOR = (uint32_t) &__Vectors;
#endif

    NVIC_EnableIRQ((IRQn_Type)1);   // WDT
    NVIC_EnableIRQ((IRQn_Type)3);   // PWM0
    NVIC_EnableIRQ((IRQn_Type)4);   // PWM1
    NVIC_EnableIRQ((IRQn_Type)7);   // SPI0
    NVIC_EnableIRQ((IRQn_Type)8);   // SPI1
    NVIC_EnableIRQ((IRQn_Type)9);   // SPI2
    NVIC_EnableIRQ((IRQn_Type)10);  // UART0 RX
    NVIC_EnableIRQ((IRQn_Type)11);  // UART0 TX
    NVIC_EnableIRQ((IRQn_Type)13);  // UART1 RX
    NVIC_EnableIRQ((IRQn_Type)14);  // UART1 TX
    NVIC_EnableIRQ((IRQn_Type)16);  // UART2 RX
    NVIC_EnableIRQ((IRQn_Type)17);  // UART2 TX
    NVIC_EnableIRQ((IRQn_Type)19);  // I2C0
    NVIC_EnableIRQ((IRQn_Type)20);  // I2C1
    NVIC_EnableIRQ((IRQn_Type)21);  // I2S RX
    NVIC_EnableIRQ((IRQn_Type)22);  // I2S TX
    NVIC_EnableIRQ((IRQn_Type)24);  // DMIC
    NVIC_EnableIRQ((IRQn_Type)26);  // GPIO
    NVIC_EnableIRQ((IRQn_Type)28);  // AES
    NVIC_EnableIRQ((IRQn_Type)29);  // DMU
    NVIC_EnableIRQ((IRQn_Type)31);  // EFLASH

    SystemCoreClock = SYSTEM_CLOCK;
}
