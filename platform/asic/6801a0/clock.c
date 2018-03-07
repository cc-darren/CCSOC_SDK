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
*  cc6801_clock.c
*
*  Project:
*  --------
*  cc6801
*
*  Description:
*  ------------
*  Config system clock and PLL for cc6801
*  This code will be executed in RAM because eFlash will not work when clock is changed.
*  So, this driver will also config eFlash parameters and eFlash driver won't take care of this(clock).
*
*  Author:
*  -------
*  PPP   (CC-pattaya)
*
*===========================================================================
*
******************************************************************************/

/******************************************************************************
Head Block of The File
******************************************************************************/
// Sec 0: Comment block of the file

// Sec 1: Include File
#include <string.h>
#include "ARMCM.h"
#include "cc6801_reg.h"
#include "clock.h"
// Sec 2: Constant Definitions, Imported Symbols, miscellaneous


/******************************************************************************
Declaration of External Variables & Functions
******************************************************************************/
// Sec 3: declaration of external variable

// Sec 4: declaration of external function prototype

/******************************************************************************
Declaration of data structure
******************************************************************************/
// Sec 5: structure, uniou, enum, linked list

/******************************************************************************
Declaration of Global Variables & Functions
******************************************************************************/
// Sec 6: declaration of global variable

//g_dwCurrentClock gets default setting from project.h but will be changed in FW running
#if (SYSTEM_CLOCK_MHZ == 16)
    E_ClockSupported g_dwCurrentClock = CLOCK_16;
#elif (SYSTEM_CLOCK_MHZ == 24)
    E_ClockSupported g_dwCurrentClock = CLOCK_24;
#elif (SYSTEM_CLOCK_MHZ == 32)
    E_ClockSupported g_dwCurrentClock = CLOCK_32;
#elif (SYSTEM_CLOCK_MHZ == 48)
    E_ClockSupported g_dwCurrentClock = CLOCK_48;
#elif (SYSTEM_CLOCK_MHZ == 64)
    E_ClockSupported g_dwCurrentClock = CLOCK_64;
#else
    #error "System clock defined in project.h is not supported"
#endif


// Sec 7: declaration of global function prototype

/******************************************************************************
Declaration of static Global Variables & Functions
******************************************************************************/
// Sec 8: declaration of static global variable
#if (XTAL_MHZ == 48)
    //array content must has same sequence as E_ClockSupported
    const T_ClockConfig g_aClockTable[CLOCK_TOTAL_SUPPORTED] =
    {
        //16MHz
        {
            //XTAL=48MHz, 48/3=16MHz, we don't need to use PLL
            (16000000),
            (PLL_BYPASS_MASK|PLL_POWER_DOWN_MASK|DIVCO_BYPASS_MASK|DIVM_BYPASS_MASK|DIVN_BYPASS_MASK),
            (PLL_DISABLE_MASK|SELECT_XTAL_MASK|SYS_DIV_3_MASK),
            //(RET_RAM_CLK_EN_MASK|OSC_40K_DIS_MASK|OSC_32K_EN_MASK|SELECT_OSC_32K_MASK|HS_CLK_EN_MASK|CPU_CLK_EN_MASK|FREQ_HIGH_MASK),
            cc6801_ClockDelayUs_16MHz
        },

        //24MHz
        {
            //XTAL=48MHz, 48/2=24MHz, we don't need to use PLL
            (24000000),
            (PLL_BYPASS_MASK|PLL_POWER_DOWN_MASK|DIVCO_BYPASS_MASK|DIVM_BYPASS_MASK|DIVN_BYPASS_MASK),
            (PLL_DISABLE_MASK|SELECT_XTAL_MASK|SYS_DIV_2_MASK),
            //(RET_RAM_CLK_EN_MASK|OSC_40K_DIS_MASK|OSC_32K_EN_MASK|SELECT_OSC_32K_MASK|HS_CLK_EN_MASK|CPU_CLK_EN_MASK|FREQ_HIGH_MASK),
            cc6801_ClockDelayUs_24MHz
        },

        //32MHz
        {
            //XTAL=48MHz, must enable PLL
            (32000000),
            (PLL_USE_MASK|PLL_POWER_UP_MASK|DIVCO_32MHZ_MASK|DIVM_32MHZ_MASK|DIVN_32MHZ_MASK),
            (PLL_ENABLE_MASK|SELECT_PLL_MASK|SYS_DIV_0_MASK),
            //(RET_RAM_CLK_EN_MASK|OSC_40K_DIS_MASK|OSC_32K_EN_MASK|SELECT_OSC_32K_MASK|HS_CLK_EN_MASK|CPU_CLK_EN_MASK|FREQ_HIGH_MASK),
            cc6801_ClockDelayUs_32MHz
        },

        //48MHz
        {
            //XTAL=48MHz, 48/1=48MHz, we don't need to use PLL
            (48000000),
            (PLL_BYPASS_MASK|PLL_POWER_DOWN_MASK|DIVCO_BYPASS_MASK|DIVM_BYPASS_MASK|DIVN_BYPASS_MASK),
            (PLL_DISABLE_MASK|SELECT_XTAL_MASK|SYS_DIV_0_MASK),
            //(RET_RAM_CLK_EN_MASK|OSC_40K_DIS_MASK|OSC_32K_EN_MASK|SELECT_OSC_32K_MASK|HS_CLK_EN_MASK|CPU_CLK_EN_MASK|FREQ_HIGH_MASK),
            cc6801_ClockDelayUs_48MHz
        },

        //64MHz
        {
            //XTAL=48MHz, must enable PLL
            (64000000),
            (PLL_USE_MASK|PLL_POWER_UP_MASK|DIVCO_64MHZ_MASK|DIVM_64MHZ_MASK|DIVN_64MHZ_MASK),
            (PLL_ENABLE_MASK|SELECT_PLL_MASK|SYS_DIV_0_MASK),
            //(RET_RAM_CLK_EN_MASK|OSC_40K_DIS_MASK|OSC_32K_EN_MASK|SELECT_OSC_32K_MASK|HS_CLK_EN_MASK|CPU_CLK_EN_MASK|FREQ_HIGH_MASK),
            cc6801_ClockDelayUs_64MHz
        },
    };

    const T_EflashConfig g_aEflashTable[CLOCK_TOTAL_SUPPORTED] =
    {
        {EF_16M_TCPS_TADH_TAH, EF_16M_TWK_TPGS, EF_16M_TRCV_TNVH, EF_16M_TPROG, EF_16M_TERASE, EF_16M_TME, EF_16M_TNVS_TNVH1},
        {EF_24M_TCPS_TADH_TAH, EF_24M_TWK_TPGS, EF_24M_TRCV_TNVH, EF_24M_TPROG, EF_24M_TERASE, EF_24M_TME, EF_24M_TNVS_TNVH1},
        {EF_32M_TCPS_TADH_TAH, EF_32M_TWK_TPGS, EF_32M_TRCV_TNVH, EF_32M_TPROG, EF_32M_TERASE, EF_32M_TME, EF_32M_TNVS_TNVH1},
        {EF_48M_TCPS_TADH_TAH, EF_48M_TWK_TPGS, EF_48M_TRCV_TNVH, EF_48M_TPROG, EF_48M_TERASE, EF_48M_TME, EF_48M_TNVS_TNVH1},
        {EF_64M_TCPS_TADH_TAH, EF_64M_TWK_TPGS, EF_64M_TRCV_TNVH, EF_64M_TPROG, EF_64M_TERASE, EF_64M_TME, EF_64M_TNVS_TNVH1},
    };
#else
    #error "XTAL clock defined in project.h is not supported by clock table"
#endif




// Sec 9: declaration of static function prototype

/******************************************************************************
// Sec 10: C Functions
******************************************************************************/

IN_SYS_RAM_BEGIN
/******************************************************************************
FUNCTION
  cc6801_ClockSysClkSet

DESCRIPTION
  *Set system clock by project definition (see project.h)
  *Run in system RAM
  *Procedure to enable PLL
    Power up PLL
    Release PLL reset
    Wait for PLL lock status
    PLL_BYPASS = 0

PARAMETERS
    none
RETURNS
    none
******************************************************************************/
static void cc6801_ClockSysClkSet(T_ClockConfig *pClk)
{
    uint32_t dwNewPLL;
    uint32_t i = 0xFFFFF;

    //dwCurPLL = ((regSCU->dw.clkConfig0 & SELECT_PLL_MASK) && (regSCU->dw.clkConfig1 & FREQ_HIGH_MASK))?1:0;
    dwNewPLL = (pClk->dwClkCfg0 & SELECT_PLL_MASK)?1:0;

    if(dwNewPLL)    // -> with PLL
    {
        //make sure CPU run at XTAL(48MHz)
        regSCU->dw.clkConfig0 = (PLL_DISABLE_MASK|SELECT_XTAL_MASK|SYS_DIV_0_MASK);
        //Power up PLL(but bypass)
        regSCU->dw.pllConfig = (pClk->dwPllCfg | PLL_BYPASS_MASK);
        //Release PLL reset
        regSCU->dw.pllReset = 1;
        __NOP();
        regSCU->dw.pllReset = 0;
        //wait for PLL lock
        while((i--) && !(regSCU->dw.pllConfig & PLL_LOCKED_MASK));
        //bypass = 0
        regSCU->dw.pllConfig = pClk->dwPllCfg;

        regSCU->dw.clkConfig0 = pClk->dwClkCfg0;
    }
    else    // -> no PLL
    {
        //must write clkConfig0 first because we must switch to XTAL then turn OFF PLL
        regSCU->dw.clkConfig0 = pClk->dwClkCfg0;
        regSCU->dw.pllConfig  = pClk->dwPllCfg;
    }

    regSCU->dw.clkConfig1 = SCU_CLK_CFG1;
}


/******************************************************************************
FUNCTION
  cc6801_ClockEflashCfg

DESCRIPTION
  *Adjust corresponding eFlash parameters by system clock
  *Run in system RAM

PARAMETERS
    none
RETURNS
    none
******************************************************************************/
static void cc6801_ClockEflashCfg(T_EflashConfig *pFlash)
{
    //regEFLASH->dwTcpsTadhTah = pFlash->dwTcpsTadhTah;
    //regEFLASH->dwTwkTpgs     = pFlash->dwTwkTpgs;
    //regEFLASH->dwTrcvTnvh    = pFlash->dwTrcvTnvh;
    //regEFLASH->dwTprog       = pFlash->dwTprog;
    //regEFLASH->dwTerase      = pFlash->dwTerase;
    //regEFLASH->dwTme         = pFlash->dwTme;
    //regEFLASH->dwTnvsTnvh1   = pFlash->dwTnvsTnvh1;
}

/******************************************************************************
FUNCTION
  cc6801_ClockSysClkAdjust

DESCRIPTION
  *Adjust system clock by project definition (see project.h)
  *Run in system RAM
  Step1:Set clock
  Step2:Adjust eFlash parameter

PARAMETERS
    E_ClockSupported
RETURNS
    none
******************************************************************************/
void cc6801_ClockSysClkAdjust(E_ClockSupported clk)
{
    T_ClockConfig   sClk;
    T_EflashConfig  sFlash;

    if(clk >= CLOCK_TOTAL_SUPPORTED)
    {
        return;
    }

    //We are going to change clock now.
    //All data/code resided in eFlash will failed to read.
    //Both g_aClockTable & g_aEflashTable are const table which are in eFlash
    //Before we read them, we must move them to RAM.
    //Also, here, we know the required clock.
    //So, we have sClk and sFlash delcared in Stack and store the required parameters
    memcpy(&sClk, &g_aClockTable[clk], sizeof(T_ClockConfig));
    memcpy(&sFlash, &g_aEflashTable[clk], sizeof(T_EflashConfig));

    //Critical section start
    GLOBAL_INT_DISABLE();

    //first change system clock
    cc6801_ClockSysClkSet(&sClk);   //current clock will be updated

    //before go back to eFlash, must update eFlash parameters for current clock
    cc6801_ClockEflashCfg(&sFlash);

    g_dwCurrentClock = clk;

    GLOBAL_INT_RESTORE();
    //Critical section end
}
IN_SYS_RAM_END

/******************************************************************************
FUNCTION
  cc6801_ClockPeripheralClkAdjust

DESCRIPTION
  Adjust every peripheral clock by system clock
  Note: whenever system clock is changed, must call this function to update all peripheral clock afterwards.

PARAMETERS
    none
RETURNS
    none
******************************************************************************/
void cc6801_ClockPeripheralClkAdjust(void)
{


}

/******************************************************************************
FUNCTION
  cc6801_ClockDelayUs_16MHz

DESCRIPTION
  Implement blocked delay function in micro-second unit.

  Becuse we support dynamically clock change, delay function must be also changed when clock is changed.
  We provide us delay function for every clock setting.
  When application call delay function, it will be directed to proper function by function pointer stored in table

PARAMETERS
    number of us
RETURNS
    none
******************************************************************************/
static __ASM void __INLINE cc6801_ClockDelayUs_16MHz(volatile uint32_t _us)
{
    subs    r0, r0, #1
loop16
    subs    r0, r0, #1
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
#if defined (ARMCM4_FP)
        NOP
#endif
    bne     loop16
    bx      lr
}

/******************************************************************************
FUNCTION
  cc6801_ClockDelayUs_24MHz

DESCRIPTION
  Implement blocked delay function in micro-second unit.

  Becuse we support dynamically clock change, delay function must be also changed when clock is changed.
  We provide us delay function for every clock setting.
  When application call delay function, it will be directed to proper function by function pointer stored in table

PARAMETERS
    number of us
RETURNS
    none
******************************************************************************/
static __ASM void __INLINE cc6801_ClockDelayUs_24MHz(volatile uint32_t _us)
{
    subs    r0, r0, #1
loop24
    subs    r0, r0, #1
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
#if defined (ARMCM4_FP)
        NOP
#endif
    bne     loop24
    bx      lr
}

/******************************************************************************
FUNCTION
  cc6801_ClockDelayUs_32MHz

DESCRIPTION
  Implement blocked delay function in micro-second unit.

  Becuse we support dynamically clock change, delay function must be also changed when clock is changed.
  We provide us delay function for every clock setting.
  When application call delay function, it will be directed to proper function by function pointer stored in table

PARAMETERS
    number of us
RETURNS
    none
******************************************************************************/
static __ASM void __INLINE cc6801_ClockDelayUs_32MHz(volatile uint32_t _us)
{
    subs    r0, r0, #1
loop32
    subs    r0, r0, #1
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
#if defined (ARMCM4_FP)
        NOP
        NOP
#endif
    bne     loop32
    bx      lr
}

/******************************************************************************
FUNCTION
  cc6801_ClockDelayUs_48MHz

DESCRIPTION
  Implement blocked delay function in micro-second unit.

  Becuse we support dynamically clock change, delay function must be also changed when clock is changed.
  We provide us delay function for every clock setting.
  When application call delay function, it will be directed to proper function by function pointer stored in table

PARAMETERS
    number of us
RETURNS
    none
******************************************************************************/
static __ASM void __INLINE cc6801_ClockDelayUs_48MHz(volatile uint32_t _us)
{
    subs    r0, r0, #1
loop48
    subs    r0, r0, #1
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
#if defined (ARMCM4_FP)
        NOP
#endif
    bne     loop48
    bx      lr
}

/******************************************************************************
FUNCTION
  cc6801_ClockDelayUs_64MHz

DESCRIPTION
  Implement blocked delay function in micro-second unit.

  Becuse we support dynamically clock change, delay function must be also changed when clock is changed.
  We provide us delay function for every clock setting.
  When application call delay function, it will be directed to proper function by function pointer stored in table

PARAMETERS
    number of us
RETURNS
    none
******************************************************************************/
static __ASM void __INLINE cc6801_ClockDelayUs_64MHz(volatile uint32_t _us)
{
    subs    r0, r0, #1
loop64
    subs    r0, r0, #1
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
        NOP
#if defined (ARMCM4_FP)
        NOP
#endif
    bne     loop64
    bx      lr
}

/******************************************************************************
FUNCTION
  cc6801_ClockDelayUs

DESCRIPTION
  Implement blocked delay function in micro-second unit.

  Becuse we support dynamically clock change, delay function must be also changed when clock is changed.
  We provide us delay function for every clock setting.
  When application call delay function, it will be directed to proper function by function pointer stored in table

PARAMETERS
    number of us
RETURNS
    none
******************************************************************************/
void __INLINE cc6801_ClockDelayUs(volatile uint32_t _us)
{
    (*g_aClockTable[g_dwCurrentClock].pDelayFunc)(_us);
}

/******************************************************************************
FUNCTION
  cc6801_ClockDelayMs

DESCRIPTION
  Implement blocked delay function in milli-second unit.

PARAMETERS
    number of ms
RETURNS
    none
******************************************************************************/
void __INLINE cc6801_ClockDelayMs(volatile uint32_t _ms)
{
    while(_ms--)
    {
        cc6801_ClockDelayUs(1000);
    }
}







