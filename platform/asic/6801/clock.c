/******************************************************************************
*  Copyright 2017, Cloudchip, Inc.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Cloudchip, Inc. (C) 2017
******************************************************************************/

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
            (PLL_BYPASS_MASK|PLL_POWER_DOWN_MASK|DIVCO_BYPASS_MASK|DIVM_BYPASS_MASK|DIVN_BYPASS_MASK),
            (PLL_DISABLE_MASK|SELECT_XTAL_MASK|SYS_DIV_3_MASK),
            //(RET_RAM_CLK_EN_MASK|OSC_40K_DIS_MASK|OSC_32K_EN_MASK|SELECT_OSC_32K_MASK|HS_CLK_EN_MASK|CPU_CLK_EN_MASK|FREQ_HIGH_MASK)
        },

        //24MHz
        {
            //XTAL=48MHz, 48/2=24MHz, we don't need to use PLL
            (PLL_BYPASS_MASK|PLL_POWER_DOWN_MASK|DIVCO_BYPASS_MASK|DIVM_BYPASS_MASK|DIVN_BYPASS_MASK),
            (PLL_DISABLE_MASK|SELECT_XTAL_MASK|SYS_DIV_2_MASK),
            //(RET_RAM_CLK_EN_MASK|OSC_40K_DIS_MASK|OSC_32K_EN_MASK|SELECT_OSC_32K_MASK|HS_CLK_EN_MASK|CPU_CLK_EN_MASK|FREQ_HIGH_MASK)
        },

        //32MHz
        {
            //XTAL=48MHz, must enable PLL
            (PLL_USE_MASK|PLL_POWER_UP_MASK|DIVCO_32MHZ_MASK|DIVM_32MHZ_MASK|DIVN_32MHZ_MASK),
            (PLL_ENABLE_MASK|SELECT_PLL_MASK|SYS_DIV_0_MASK),
            //(RET_RAM_CLK_EN_MASK|OSC_40K_DIS_MASK|OSC_32K_EN_MASK|SELECT_OSC_32K_MASK|HS_CLK_EN_MASK|CPU_CLK_EN_MASK|FREQ_HIGH_MASK)
        },

        //48MHz
        {
            //XTAL=48MHz, 48/1=48MHz, we don't need to use PLL
            (PLL_BYPASS_MASK|PLL_POWER_DOWN_MASK|DIVCO_BYPASS_MASK|DIVM_BYPASS_MASK|DIVN_BYPASS_MASK),
            (PLL_DISABLE_MASK|SELECT_XTAL_MASK|SYS_DIV_0_MASK),
            //(RET_RAM_CLK_EN_MASK|OSC_40K_DIS_MASK|OSC_32K_EN_MASK|SELECT_OSC_32K_MASK|HS_CLK_EN_MASK|CPU_CLK_EN_MASK|FREQ_HIGH_MASK)
        },

        //64MHz
        {
            //XTAL=48MHz, must enable PLL
            (PLL_USE_MASK|PLL_POWER_UP_MASK|DIVCO_64MHZ_MASK|DIVM_64MHZ_MASK|DIVN_64MHZ_MASK),
            (PLL_ENABLE_MASK|SELECT_PLL_MASK|SYS_DIV_0_MASK),
            //(RET_RAM_CLK_EN_MASK|OSC_40K_DIS_MASK|OSC_32K_EN_MASK|SELECT_OSC_32K_MASK|HS_CLK_EN_MASK|CPU_CLK_EN_MASK|FREQ_HIGH_MASK)
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
static void cc6801_ClockSysClkSet(E_ClockSupported clk)
{
    uint32_t dwNewPLL;
    uint32_t i = 0xFFFFF;

    if(clk >= CLOCK_TOTAL_SUPPORTED)
    {
        return;
    }

    //dwCurPLL = ((regSCU->dw.clkConfig0 & SELECT_PLL_MASK) && (regSCU->dw.clkConfig1 & FREQ_HIGH_MASK))?1:0;
    dwNewPLL = (g_aClockTable[clk].dwClkCfg0 & SELECT_PLL_MASK)?1:0;

    if(dwNewPLL)    // -> with PLL
    {
        //make sure CPU run at XTAL(48MHz)
        regSCU->dw.clkConfig0 = (PLL_DISABLE_MASK|SELECT_XTAL_MASK|SYS_DIV_0_MASK);
        //Power up PLL(but bypass)
        regSCU->dw.pllConfig = (g_aClockTable[clk].dwPllCfg | PLL_BYPASS_MASK);
        //Release PLL reset
        regSCU->dw.pllReset = 1;
        __NOP();
        regSCU->dw.pllReset = 0;
        //wait for PLL lock
        while((i--) && !(regSCU->dw.pllConfig & PLL_LOCKED_MASK));
        //bypass = 0
        regSCU->dw.pllConfig = g_aClockTable[clk].dwPllCfg;

        regSCU->dw.clkConfig0 = g_aClockTable[clk].dwClkCfg0;
    }
    else    // -> no PLL
    {
        //must write clkConfig0 first because we must switch to XTAL then turn OFF PLL
        regSCU->dw.clkConfig0 = g_aClockTable[clk].dwClkCfg0;
        regSCU->dw.pllConfig  = g_aClockTable[clk].dwPllCfg;
    }

    regSCU->dw.clkConfig1 = SCU_CLK_CFG1;
    g_dwCurrentClock = clk;
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
static void cc6801_ClockEflashCfg(E_ClockSupported clk)
{
    if(clk >= CLOCK_TOTAL_SUPPORTED)
    {
        return;
    }

    regEFLASH->dwTcpsTadhTah = g_aEflashTable[clk].dwTcpsTadhTah;
    regEFLASH->dwTwkTpgs     = g_aEflashTable[clk].dwTwkTpgs;
    regEFLASH->dwTrcvTnvh    = g_aEflashTable[clk].dwTrcvTnvh;
    regEFLASH->dwTprog       = g_aEflashTable[clk].dwTprog;
    regEFLASH->dwTerase      = g_aEflashTable[clk].dwTerase;
    regEFLASH->dwTme         = g_aEflashTable[clk].dwTme;
    regEFLASH->dwTnvsTnvh1   = g_aEflashTable[clk].dwTnvsTnvh1;
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
    none
RETURNS
    none
******************************************************************************/
void cc6801_ClockSysClkAdjust(E_ClockSupported clk)
{
    //first change system clock
    cc6801_ClockSysClkSet(clk);   //current clock will be updated

    //before go back to eFlash, must update eFlash parameters for current clock
    cc6801_ClockEflashCfg(clk);
}

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



