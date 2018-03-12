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



/******************************/
/* enable interrupts          */
/******************************/

    //should be enabled by driver init respectively
    //for test, we can easily enable IRQ here

#ifdef CFG_BLE_APP    
    NVIC_EnableIRQ(BLE_IRQn        );
#endif

/******************************/
/* Config Heap                */
/******************************/
    //if necessary

}
