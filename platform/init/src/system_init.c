/**************************************************************************//**
 * @file     system_init.c
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
#include "ll.h"
#include "cc6801_reg.h"
#include "drvi_clock.h"


extern E_ClockSupported g_dwCurrentClock;

/*----------------------------------------------------------------------------
  Externals
 *----------------------------------------------------------------------------*/
#if defined (__VTOR_PRESENT) && (__VTOR_PRESENT == 1)
    extern uint32_t __Vectors;
#endif

/*----------------------------------------------------------------------------
  System initialization function
 *----------------------------------------------------------------------------*/
void sys_InitStartup (void)
{
    //uint32_t byp=0,pd=1,divc0=0,divn=8,divm=20,vco_div=0;

    GLOBAL_INT_STOP();

/******************************/
/* set new vectors address    */
/******************************/
#if defined (__VTOR_PRESENT) && (__VTOR_PRESENT == 1)
    SCB->VTOR = (uint32_t) &__Vectors;
#endif

#if defined (__FPU_USED) && (__FPU_USED == 1)
    SCB->CPACR |= ((3U << 10*2) |           /* set CP10 Full Access */
                   (3U << 11*2)  );         /* set CP11 Full Access */
#endif

#ifdef UNALIGNED_SUPPORT_DISABLE
    SCB->CCR |= SCB_CCR_UNALIGN_TRP_Msk;
#endif

/******************************/
/* setup PLL                  */
/******************************/

    //wr(SCU_PLLCFG_REG, (0 | vco_div<<26 | byp<<25 | pd<<24 | divc0<<16 | divm<<8 | divn) );
    //wr(SCU_CLKCFG0_REG, 0x00000500);
    //wr(SCU_CLKCFG1_REG, 0x00002B01);
}


void sys_InitMain (void)
{
    //enable high speed as early as possible
    drvi_ClockSysClkAdjust(g_dwCurrentClock);
    
    //Set init value, it should be config by driver respectively
    regCKGEN->dw.clkCfg1 = (0x00000000 | 0<<29 | 1<<21 | 0<<13 | 1<<5);
    regCKGEN->dw.clkCfg2 = (0x00000000 | 1<<29 | 1<<21 | 1<<13 | 1<<5);
    regCKGEN->dw.clkCfg3 = (0x00000000 | 1<<29 | 1<<21 | 1<<13 | 1<<5);
    regCKGEN->dw.clkCfg4 = (0x00000000 | 1<<5);

/******************************/
/* re-arrange IRQ priority    */
/******************************/

#if defined (ARMCM4_FP)
    NVIC_SetPriority(SWI0_IRQn       ,1);   //Based on Nordic sw timer, WKTM0 and SWI0 must have same priority
    NVIC_SetPriority(WDT_IRQn        ,6);
    NVIC_SetPriority(RTC_IRQn        ,6);
    NVIC_SetPriority(PWM0_IRQn       ,1);
    NVIC_SetPriority(PWM1_IRQn       ,1);
    NVIC_SetPriority(WKTM0_IRQn      ,1);
    NVIC_SetPriority(WKTM1_IRQn      ,1);
    NVIC_SetPriority(SPI0_M_IRQn     ,2);
    NVIC_SetPriority(SPI1_M_IRQn     ,2);
    NVIC_SetPriority(SPI2_M_IRQn     ,2);
    NVIC_SetPriority(UART0_RXDMA_IRQn,5);
    NVIC_SetPriority(UART0_TXDMA_IRQn,5);
    NVIC_SetPriority(UART0_IP_IRQn   ,5);
    NVIC_SetPriority(UART1_RXDMA_IRQn,5);
    NVIC_SetPriority(UART1_TXDMA_IRQn,5);
    NVIC_SetPriority(UART1_IP_IRQn   ,5);
    NVIC_SetPriority(UART2_RXDMA_IRQn,5);
    NVIC_SetPriority(UART2_TXDMA_IRQn,5);
    NVIC_SetPriority(UART2_IP_IRQn   ,5);
    NVIC_SetPriority(I2C0_M_IRQn     ,2);
    NVIC_SetPriority(I2C1_M_IRQn     ,2);
    NVIC_SetPriority(I2S_RXDMA_IRQn  ,7);
    NVIC_SetPriority(I2S_TXDMA_IRQn  ,7);
    NVIC_SetPriority(I2S_IP_IRQn     ,7);
    NVIC_SetPriority(DMIC_IRQn       ,7);
    NVIC_SetPriority(BLE_IRQn        ,0);
    NVIC_SetPriority(GPIO_IRQn       ,4);
    NVIC_SetPriority(CCU_IRQn        ,8);
    NVIC_SetPriority(AES_IRQn        ,8);
    NVIC_SetPriority(DMU_IRQn        ,8);
    NVIC_SetPriority(SWI1_IRQn       ,3);
    NVIC_SetPriority(EFLASH_IRQn     ,8);
#elif defined (ARMCM0P)
    NVIC_SetPriority(SWI0_IRQn       ,1);
    NVIC_SetPriority(WDT_IRQn        ,3);
    NVIC_SetPriority(RTC_IRQn        ,3);
    NVIC_SetPriority(PWM0_IRQn       ,1);
    NVIC_SetPriority(PWM1_IRQn       ,1);
    NVIC_SetPriority(WKTM0_IRQn      ,1);
    NVIC_SetPriority(WKTM1_IRQn      ,1);
    NVIC_SetPriority(SPI0_M_IRQn     ,1);
    NVIC_SetPriority(SPI1_M_IRQn     ,1);
    NVIC_SetPriority(SPI2_M_IRQn     ,1);
    NVIC_SetPriority(UART0_RXDMA_IRQn,2);
    NVIC_SetPriority(UART0_TXDMA_IRQn,2);
    NVIC_SetPriority(UART0_IP_IRQn   ,2);
    NVIC_SetPriority(UART1_RXDMA_IRQn,2);
    NVIC_SetPriority(UART1_TXDMA_IRQn,2);
    NVIC_SetPriority(UART1_IP_IRQn   ,2);
    NVIC_SetPriority(UART2_RXDMA_IRQn,2);
    NVIC_SetPriority(UART2_TXDMA_IRQn,2);
    NVIC_SetPriority(UART2_IP_IRQn   ,2);
    NVIC_SetPriority(I2C0_M_IRQn     ,1);
    NVIC_SetPriority(I2C1_M_IRQn     ,1);
    NVIC_SetPriority(I2S_RXDMA_IRQn  ,3);
    NVIC_SetPriority(I2S_TXDMA_IRQn  ,3);
    NVIC_SetPriority(I2S_IP_IRQn     ,3);
    NVIC_SetPriority(DMIC_IRQn       ,3);
    NVIC_SetPriority(BLE_IRQn        ,0);
    NVIC_SetPriority(GPIO_IRQn       ,2);
    NVIC_SetPriority(CCU_IRQn        ,3);
    NVIC_SetPriority(AES_IRQn        ,3);
    NVIC_SetPriority(DMU_IRQn        ,3);
    NVIC_SetPriority(SWI1_IRQn       ,1);
    NVIC_SetPriority(EFLASH_IRQn     ,3);
#else
    #error device not specified!
#endif

/******************************/
/* enable interrupts          */
/******************************/

    //should be enabled by driver init respectively
    //for test, we can easily enable IRQ here

    //NVIC_EnableIRQ(SWI0_IRQn       );
    //NVIC_EnableIRQ(WDT_IRQn        );
    //NVIC_EnableIRQ(RTC_IRQn        );
    //NVIC_EnableIRQ(PWM0_IRQn       );
    //NVIC_EnableIRQ(PWM1_IRQn       );
    //NVIC_EnableIRQ(WKTM0_IRQn      );
    //NVIC_EnableIRQ(WKTM1_IRQn      );
    //NVIC_EnableIRQ(SPI0_M_IRQn     );
    //NVIC_EnableIRQ(SPI1_M_IRQn     );
    //NVIC_EnableIRQ(SPI2_M_IRQn     );
    //NVIC_EnableIRQ(UART0_RXDMA_IRQn);
    //NVIC_EnableIRQ(UART0_TXDMA_IRQn);
    //NVIC_EnableIRQ(UART0_IP_IRQn   );
    //NVIC_EnableIRQ(UART1_RXDMA_IRQn);
    //NVIC_EnableIRQ(UART1_TXDMA_IRQn);
    //NVIC_EnableIRQ(UART1_IP_IRQn   );
    //NVIC_EnableIRQ(UART2_RXDMA_IRQn);
    //NVIC_EnableIRQ(UART2_TXDMA_IRQn);
    //NVIC_EnableIRQ(UART2_IP_IRQn   );
    //NVIC_EnableIRQ(I2C0_M_IRQn     );
    //NVIC_EnableIRQ(I2C1_M_IRQn     );
    //NVIC_EnableIRQ(I2S_RXDMA_IRQn  );
    //NVIC_EnableIRQ(I2S_TXDMA_IRQn  );
    //NVIC_EnableIRQ(I2S_IP_IRQn     );
    //NVIC_EnableIRQ(DMIC_IRQn       );
#ifdef CFG_BLE_APP    
    NVIC_EnableIRQ(BLE_IRQn        );
#endif
    //NVIC_EnableIRQ(GPIO_IRQn       );
    //NVIC_EnableIRQ(CCU_IRQn        );
    //NVIC_EnableIRQ(AES_IRQn        );
    //NVIC_EnableIRQ(DMU_IRQn        );
    //NVIC_EnableIRQ(SWI1_IRQn       );
    //NVIC_EnableIRQ(EFLASH_IRQn     );

/******************************/
/* Config Heap                */
/******************************/
    //if necessary

}
