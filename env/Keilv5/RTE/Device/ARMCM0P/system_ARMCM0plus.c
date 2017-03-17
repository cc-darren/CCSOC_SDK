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
#define  XTAL            (48000000U)      /* Oscillator frequency */

#define  SYSTEM_CLOCK    (1 * XTAL)


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
    volatile uint32_t  P[32];
/******************************/
/* set new vectors address    */
/******************************/
  #if defined (__VTOR_PRESENT) && (__VTOR_PRESENT == 1)
    SCB->VTOR = (uint32_t) &__Vectors;
  #endif

/******************************/
/* setup PLL                  */
/******************************/



/******************************/
/* re-arrange IRQ priority    */
/******************************/

    P[0 ]=NVIC_GetPriority(SWI0_IRQn       );
    P[1 ]=NVIC_GetPriority(WDT_IRQn        );
    P[2 ]=NVIC_GetPriority(RTC_IRQn        );
    P[3 ]=NVIC_GetPriority(PWM0_IRQn       );
    P[4 ]=NVIC_GetPriority(PWM1_IRQn       );
    P[5 ]=NVIC_GetPriority(WKTM0_IRQn      );
    P[6 ]=NVIC_GetPriority(WKTM1_IRQn      );
    P[7 ]=NVIC_GetPriority(SPI0_M_IRQn     );
    P[8 ]=NVIC_GetPriority(SPI1_M_IRQn     );
    P[9 ]=NVIC_GetPriority(SPI2_M_IRQn     );
    P[10]=NVIC_GetPriority(UART0_RXDMA_IRQn);
    P[11]=NVIC_GetPriority(UART0_TXDMA_IRQn);
    P[12]=NVIC_GetPriority(UART0_IP_IRQn   );
    P[13]=NVIC_GetPriority(UART1_RXDMA_IRQn);
    P[14]=NVIC_GetPriority(UART1_TXDMA_IRQn);
    P[15]=NVIC_GetPriority(UART1_IP_IRQn   );
    P[16]=NVIC_GetPriority(UART2_RXDMA_IRQn);
    P[17]=NVIC_GetPriority(UART2_TXDMA_IRQn);
    P[18]=NVIC_GetPriority(UART2_IP_IRQn   );
    P[19]=NVIC_GetPriority(I2C0_M_IRQn     );
    P[20]=NVIC_GetPriority(I2C1_M_IRQn     );
    P[21]=NVIC_GetPriority(I2S_RXDMA_IRQn  );
    P[22]=NVIC_GetPriority(I2S_TXDMA_IRQn  );
    P[23]=NVIC_GetPriority(I2S_IP_IRQn     );
    P[24]=NVIC_GetPriority(DMIC_IRQn       );
    P[25]=NVIC_GetPriority(BLE_IRQn        );
    P[26]=NVIC_GetPriority(GPIO_IRQn       );
    P[27]=NVIC_GetPriority(CCU_IRQn        );
    P[28]=NVIC_GetPriority(AES_IRQn        );
    P[29]=NVIC_GetPriority(DMU_IRQn        );
    P[30]=NVIC_GetPriority(SWI1_IRQn       );
    P[31]=NVIC_GetPriority(EFLASH_IRQn     );







    NVIC_SetPriority(SWI0_IRQn       ,31);
    NVIC_SetPriority(WDT_IRQn        ,30);
    NVIC_SetPriority(RTC_IRQn        ,29);
    NVIC_SetPriority(PWM0_IRQn       ,28);
    NVIC_SetPriority(PWM1_IRQn       ,27);
    NVIC_SetPriority(WKTM0_IRQn      ,26);
    NVIC_SetPriority(WKTM1_IRQn      ,25);
    NVIC_SetPriority(SPI0_M_IRQn     ,24);
    NVIC_SetPriority(SPI1_M_IRQn     ,23);
    NVIC_SetPriority(SPI2_M_IRQn     ,22);
    NVIC_SetPriority(UART0_RXDMA_IRQn,21);
    NVIC_SetPriority(UART0_TXDMA_IRQn,20);
    NVIC_SetPriority(UART0_IP_IRQn   ,19);
    NVIC_SetPriority(UART1_RXDMA_IRQn,18);
    NVIC_SetPriority(UART1_TXDMA_IRQn,17);
    NVIC_SetPriority(UART1_IP_IRQn   ,16);
    NVIC_SetPriority(UART2_RXDMA_IRQn,15);
    NVIC_SetPriority(UART2_TXDMA_IRQn,14);
    NVIC_SetPriority(UART2_IP_IRQn   ,13);
    NVIC_SetPriority(I2C0_M_IRQn     ,12);
    NVIC_SetPriority(I2C1_M_IRQn     ,11);
    NVIC_SetPriority(I2S_RXDMA_IRQn  ,10);
    NVIC_SetPriority(I2S_TXDMA_IRQn  ,9 );
    NVIC_SetPriority(I2S_IP_IRQn     ,8 );
    NVIC_SetPriority(DMIC_IRQn       ,7 );
    NVIC_SetPriority(BLE_IRQn        ,6 );
    NVIC_SetPriority(GPIO_IRQn       ,5 );
    NVIC_SetPriority(CCU_IRQn        ,4 );
    NVIC_SetPriority(AES_IRQn        ,3 );
    NVIC_SetPriority(DMU_IRQn        ,2 );
    NVIC_SetPriority(SWI1_IRQn       ,1 );
    NVIC_SetPriority(EFLASH_IRQn     ,0 );


    P[0 ]=NVIC_GetPriority(SWI0_IRQn       );
    P[1 ]=NVIC_GetPriority(WDT_IRQn        );
    P[2 ]=NVIC_GetPriority(RTC_IRQn        );
    P[3 ]=NVIC_GetPriority(PWM0_IRQn       );
    P[4 ]=NVIC_GetPriority(PWM1_IRQn       );
    P[5 ]=NVIC_GetPriority(WKTM0_IRQn      );
    P[6 ]=NVIC_GetPriority(WKTM1_IRQn      );
    P[7 ]=NVIC_GetPriority(SPI0_M_IRQn     );
    P[8 ]=NVIC_GetPriority(SPI1_M_IRQn     );
    P[9 ]=NVIC_GetPriority(SPI2_M_IRQn     );
    P[10]=NVIC_GetPriority(UART0_RXDMA_IRQn);
    P[11]=NVIC_GetPriority(UART0_TXDMA_IRQn);
    P[12]=NVIC_GetPriority(UART0_IP_IRQn   );
    P[13]=NVIC_GetPriority(UART1_RXDMA_IRQn);
    P[14]=NVIC_GetPriority(UART1_TXDMA_IRQn);
    P[15]=NVIC_GetPriority(UART1_IP_IRQn   );
    P[16]=NVIC_GetPriority(UART2_RXDMA_IRQn);
    P[17]=NVIC_GetPriority(UART2_TXDMA_IRQn);
    P[18]=NVIC_GetPriority(UART2_IP_IRQn   );
    P[19]=NVIC_GetPriority(I2C0_M_IRQn     );
    P[20]=NVIC_GetPriority(I2C1_M_IRQn     );
    P[21]=NVIC_GetPriority(I2S_RXDMA_IRQn  );
    P[22]=NVIC_GetPriority(I2S_TXDMA_IRQn  );
    P[23]=NVIC_GetPriority(I2S_IP_IRQn     );
    P[24]=NVIC_GetPriority(DMIC_IRQn       );
    P[25]=NVIC_GetPriority(BLE_IRQn        );
    P[26]=NVIC_GetPriority(GPIO_IRQn       );
    P[27]=NVIC_GetPriority(CCU_IRQn        );
    P[28]=NVIC_GetPriority(AES_IRQn        );
    P[29]=NVIC_GetPriority(DMU_IRQn        );
    P[30]=NVIC_GetPriority(SWI1_IRQn       );
    P[31]=NVIC_GetPriority(EFLASH_IRQn     );



/******************************/
/* enable interrupts          */
/******************************/
    NVIC_EnableIRQ(SWI0_IRQn       );
    NVIC_EnableIRQ(WDT_IRQn        );
    NVIC_EnableIRQ(RTC_IRQn        );
    NVIC_EnableIRQ(PWM0_IRQn       );
    NVIC_EnableIRQ(PWM1_IRQn       );
    NVIC_EnableIRQ(WKTM0_IRQn      );
    NVIC_EnableIRQ(WKTM1_IRQn      );
    NVIC_EnableIRQ(SPI0_M_IRQn     );
    NVIC_EnableIRQ(SPI1_M_IRQn     );
    NVIC_EnableIRQ(SPI2_M_IRQn     );
    NVIC_EnableIRQ(UART0_RXDMA_IRQn);
    NVIC_EnableIRQ(UART0_TXDMA_IRQn);
    NVIC_EnableIRQ(UART0_IP_IRQn   );
    NVIC_EnableIRQ(UART1_RXDMA_IRQn);
    NVIC_EnableIRQ(UART1_TXDMA_IRQn);
    NVIC_EnableIRQ(UART1_IP_IRQn   );
    NVIC_EnableIRQ(UART2_RXDMA_IRQn);
    NVIC_EnableIRQ(UART2_TXDMA_IRQn);
    NVIC_EnableIRQ(UART2_IP_IRQn   );
    NVIC_EnableIRQ(I2C0_M_IRQn     );
    NVIC_EnableIRQ(I2C1_M_IRQn     );
    NVIC_EnableIRQ(I2S_RXDMA_IRQn  );
    NVIC_EnableIRQ(I2S_TXDMA_IRQn  );
    NVIC_EnableIRQ(I2S_IP_IRQn     );
    NVIC_EnableIRQ(DMIC_IRQn       );
    NVIC_EnableIRQ(BLE_IRQn        );
    NVIC_EnableIRQ(GPIO_IRQn       );
    NVIC_EnableIRQ(CCU_IRQn        );
    NVIC_EnableIRQ(AES_IRQn        );
    NVIC_EnableIRQ(DMU_IRQn        );
    NVIC_EnableIRQ(SWI1_IRQn       );
    NVIC_EnableIRQ(EFLASH_IRQn     );

/******************************/
/* Config Heap                */
/******************************/
    //if necessary

    SystemCoreClock = SYSTEM_CLOCK;
}
