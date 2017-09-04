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
*  cc6801_clock.h
*
*  Project:
*  --------
*  cc6801
*
*  Description:
*  ------------
*  Config system clock and PLL for cc6801
*
*  Author:
*  -------
*  PPP   (CC-pattaya)
*
*===========================================================================
*
******************************************************************************/

#ifndef _CC6801_CLOCK_H_
#define _CC6801_CLOCK_H_

/******************************************************************************
Head Block of The File
******************************************************************************/
// Sec 0: Comment block of the file

// Sec 1: Include File
#include <stdint.h>
#include "ARMCM.h"
#include "project.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous


/***************************************************
Offset Address : 0x0C PLL Configuration
***************************************************/
#define PLL_LOCKED_BIT                  1UL
#define PLL_LOCKED_MASK                 (PLL_LOCKED_BIT<<31)

#define PLL_BYPASS_BIT                  1UL                             //1: bypass PLL
#define PLL_BYPASS_MASK                 (PLL_BYPASS_BIT<<25)
#define PLL_USE_BIT                     0UL                             //0: use PLL
#define PLL_USE_MASK                    (PLL_USE_BIT<<25)

#define PLL_POWER_UP_BIT                1UL                             //1: PLL power up
#define PLL_POWER_UP_MASK               (PLL_POWER_UP_BIT<<24)
#define PLL_POWER_DOWN_BIT              0UL                             //0: PLL power down
#define PLL_POWER_DOWN_MASK             (PLL_POWER_DOWN_BIT<<24)

#ifndef XTAL_MHZ
    #error "Must define XTAL_MHZ in project.h , XTAL_MHZ is PLL reference clock"
#endif

#define DIVM_BYPASS_BIT                 255UL
#define DIVM_BYPASS_MASK                (DIVM_BYPASS_BIT<<8)
#define DIVN_BYPASS_BIT                 255UL
#define DIVN_BYPASS_MASK                (DIVN_BYPASS_BIT)
#define DIVCO_BYPASS_BIT                255UL
#define DIVCO_BYPASS_MASK               (DIVCO_BYPASS_BIT<<16)

//By ASIC team suggestion:
//Fvco = XTAL_MHZ * (DIVM+1) / (DIVN+1)     must keep Fvco range 80MHz~350MHz
//Fout = Fvco / (DIVCO+1)                   DIVCO as small as possible

#if (XTAL_MHZ == 48)
    #define DIVM_120MHZ_BIT             24
    #define DIVN_120MHZ_BIT             9
    #define DIVCO_120MHZ_BIT            0
    #define DIVM_120MHZ_MASK            (DIVM_120MHZ_BIT<<8)
    #define DIVN_120MHZ_MASK            (DIVN_120MHZ_BIT)
    #define DIVCO_120MHZ_MASK           (DIVCO_120MHZ_BIT<<16)

    #define DIVM_112MHZ_BIT             20
    #define DIVN_112MHZ_BIT             8
    #define DIVCO_112MHZ_BIT            0
    #define DIVM_112MHZ_MASK            (DIVM_112MHZ_BIT<<8)
    #define DIVN_112MHZ_MASK            (DIVN_112MHZ_BIT)
    #define DIVCO_112MHZ_MASK           (DIVCO_112MHZ_BIT<<16)

    #define DIVM_104MHZ_BIT             194
    #define DIVN_104MHZ_BIT             89
    #define DIVCO_104MHZ_BIT            0
    #define DIVM_104MHZ_MASK            (DIVM_104MHZ_BIT<<8)
    #define DIVN_104MHZ_MASK            (DIVN_104MHZ_BIT)
    #define DIVCO_104MHZ_MASK           (DIVCO_104MHZ_BIT<<16)

    #define DIVM_96MHZ_BIT              1
    #define DIVN_96MHZ_BIT              0
    #define DIVCO_96MHZ_BIT             0
    #define DIVM_96MHZ_MASK             (DIVM_96MHZ_BIT<<8)
    #define DIVN_96MHZ_MASK             (DIVN_96MHZ_BIT)
    #define DIVCO_96MHZ_MASK            (DIVCO_96MHZ_BIT<<16)

    #define DIVM_80MHZ_BIT              29
    #define DIVN_80MHZ_BIT              8
    #define DIVCO_80MHZ_BIT             1
    #define DIVM_80MHZ_MASK             (DIVM_80MHZ_BIT<<8)
    #define DIVN_80MHZ_MASK             (DIVN_80MHZ_BIT)
    #define DIVCO_80MHZ_MASK            (DIVCO_80MHZ_BIT<<16)

    #define DIVM_72MHZ_BIT              14
    #define DIVN_72MHZ_BIT              4
    #define DIVCO_72MHZ_BIT             1
    #define DIVM_72MHZ_MASK             (DIVM_72MHZ_BIT<<8)
    #define DIVN_72MHZ_MASK             (DIVN_72MHZ_BIT)
    #define DIVCO_72MHZ_MASK            (DIVCO_72MHZ_BIT<<16)

    #define DIVM_64MHZ_BIT              23
    #define DIVN_64MHZ_BIT              8
    #define DIVCO_64MHZ_BIT             1
    #define DIVM_64MHZ_MASK             (DIVM_64MHZ_BIT<<8)
    #define DIVN_64MHZ_MASK             (DIVN_64MHZ_BIT)
    #define DIVCO_64MHZ_MASK            (DIVCO_64MHZ_BIT<<16)

    #define DIVM_56MHZ_BIT              104
    #define DIVN_56MHZ_BIT              44
    #define DIVCO_56MHZ_BIT             1
    #define DIVM_56MHZ_MASK             (DIVM_56MHZ_BIT<<8)
    #define DIVN_56MHZ_MASK             (DIVN_56MHZ_BIT)
    #define DIVCO_56MHZ_MASK            (DIVCO_56MHZ_BIT<<16)

    #define DIVM_32MHZ_BIT              1
    #define DIVN_32MHZ_BIT              0
    #define DIVCO_32MHZ_BIT             2
    #define DIVM_32MHZ_MASK             (DIVM_32MHZ_BIT<<8)
    #define DIVN_32MHZ_MASK             (DIVN_32MHZ_BIT)
    #define DIVCO_32MHZ_MASK            (DIVCO_32MHZ_BIT<<16)
#else
    #error "XTAL clock defined in project.h is not supported by PLL setting"
#endif

/***************************************************
Offset Address : 0x10 System Clock Configuration0
***************************************************/
#define SYS_DIV_0_BIT                   0                               //divide by 1
#define SYS_DIV_0_MASK                  (SYS_DIV_0_BIT)
#define SYS_DIV_2_BIT                   2                               //divide by 2
#define SYS_DIV_2_MASK                  (SYS_DIV_2_BIT)
#define SYS_DIV_3_BIT                   3                               //divide by 3
#define SYS_DIV_3_MASK                  (SYS_DIV_3_BIT)

#define SELECT_PLL_BIT                  1UL
#define SELECT_PLL_MASK                 (SELECT_PLL_BIT<<8)
#define SELECT_XTAL_BIT                 0UL
#define SELECT_XTAL_MASK                (SELECT_XTAL_BIT<<8)

//#define SYS_DIV_BYPASS_BIT              1                               //1: bypass divider
//#define SYS_DIV_BYPASS_MASK             (SYS_DIV_BYPASS_BIT<<9)
//#define SYS_DIV_USE_BIT                 0                               //0: use divider
//#define SYS_DIV_USE_MASK                (SYS_DIV_USE_BIT<<9)

#define PLL_ENABLE_BIT                  1UL                               //1: enable PLL
#define PLL_ENABLE_MASK                 (PLL_ENABLE_BIT<<10)
#define PLL_DISABLE_BIT                 0UL                               //0: disable PLL
#define PLL_DISABLE_MASK                (PLL_DISABLE_BIT<<10)

/***************************************************
Offset Address : 0x14 System Clock Configuration1
***************************************************/
#define FREQ_HIGH_BIT                   1                               //1: high frequence
#define FREQ_HIGH_MASK                  (FREQ_HIGH_BIT)
#define FREQ_32K_BIT                    0                               //0: use 32K clock
#define FREQ_32K_MASK                   (FREQ_32K_BIT)

//cpu clock must be enabled always by FW ?
#define CPU_CLK_EN_BIT                  1                               //1: CPU/BUS clock enable
#define CPU_CLK_EN_MASK                 (CPU_CLK_EN_BIT<<8)

//no need to disable HS clock
#define HS_CLK_EN_BIT                   1                               //1: CPU/BUS clock enable
#define HS_CLK_EN_MASK                  (HS_CLK_EN_BIT<<9)

#define SELECT_OSC_32K_BIT              0                               //0: 32KHz oscillator
#define SELECT_OSC_32K_MASK             (SELECT_OSC_32K_BIT<<10)
#define SELECT_OSC_40K_BIT              1                               //1: 40KHz ring oscillator
#define SELECT_OSC_40K_MASK             (SELECT_OSC_40K_BIT<<10)

//no need to disable 32K osc
#define OSC_32K_EN_BIT                  1                               //1: enable OSC 32KHz
#define OSC_32K_EN_MASK                 (OSC_32K_EN_BIT<<11)

//no need to enable 40K osc
#define OSC_40K_DIS_BIT                 0                               //0: disable OSC 40KHz
#define OSC_40K_DIS_MASK                (OSC_40K_DIS_BIT<<12)

//no need to disable retention RAM clock
#define RET_RAM_CLK_EN_BIT              1                               //1: enable retention ram clock
#define RET_RAM_CLK_EN_MASK             (RET_RAM_CLK_EN_BIT<<13)

//clk_cfg1 has same value for every clock setting
#define SCU_CLK_CFG1    (RET_RAM_CLK_EN_MASK|OSC_40K_DIS_MASK|OSC_32K_EN_MASK|SELECT_OSC_32K_MASK|HS_CLK_EN_MASK|CPU_CLK_EN_MASK|FREQ_HIGH_MASK)


/***************************************************
eFlash setting
***************************************************/
#define EF_TADH             3       //no need to change for different clock

#define EF_16M_TAH          1
#define EF_16M_TNVS         80
#define EF_16M_TPGS         160
#define EF_16M_TWK          32
#define EF_16M_TNVH         80
#define EF_16M_TRCV         160
#define EF_16M_TPROG        128
#define EF_16M_TCPS         3
#define EF_16M_TERASE       1600000
#define EF_16M_TME          1600000
#define EF_16M_TNVH1        1600
#define EF_16M_TCPS_TADH_TAH    ((EF_16M_TCPS<<24)|(EF_TADH<<16)|(EF_16M_TAH))
#define EF_16M_TWK_TPGS         ((EF_16M_TWK <<16)|(EF_16M_TPGS<<8))
#define EF_16M_TRCV_TNVH        ((EF_16M_TRCV<<16)|(EF_16M_TNVH))
#define EF_16M_TNVS_TNVH1       ((EF_16M_TNVS<<16)|(EF_16M_TNVH1))

#define EF_24M_TAH          1
#define EF_24M_TNVS         120
#define EF_24M_TPGS         240
#define EF_24M_TWK          48
#define EF_24M_TNVH         120
#define EF_24M_TRCV         240
#define EF_24M_TPROG        192
#define EF_24M_TCPS         3
#define EF_24M_TERASE       1920000
#define EF_24M_TME          1920000
#define EF_24M_TNVH1        2400
#define EF_24M_TCPS_TADH_TAH    ((EF_24M_TCPS<<24)|(EF_TADH<<16)|(EF_24M_TAH))
#define EF_24M_TWK_TPGS         ((EF_24M_TWK <<16)|(EF_24M_TPGS<<8))
#define EF_24M_TRCV_TNVH        ((EF_24M_TRCV<<16)|(EF_24M_TNVH))
#define EF_24M_TNVS_TNVH1       ((EF_24M_TNVS<<16)|(EF_24M_TNVH1))

#define EF_32M_TAH          1
#define EF_32M_TNVS         160
#define EF_32M_TPGS         320
#define EF_32M_TWK          64
#define EF_32M_TNVH         160
#define EF_32M_TRCV         320
#define EF_32M_TPROG        256
#define EF_32M_TCPS         3
#define EF_32M_TERASE       3200000
#define EF_32M_TME          3200000
#define EF_32M_TNVH1        3200
#define EF_32M_TCPS_TADH_TAH    ((EF_32M_TCPS<<24)|(EF_TADH<<16)|(EF_32M_TAH))
#define EF_32M_TWK_TPGS         ((EF_32M_TWK <<16)|(EF_32M_TPGS<<8))
#define EF_32M_TRCV_TNVH        ((EF_32M_TRCV<<16)|(EF_32M_TNVH))
#define EF_32M_TNVS_TNVH1       ((EF_32M_TNVS<<16)|(EF_32M_TNVH1))

#define EF_48M_TAH          2
#define EF_48M_TNVS         240
#define EF_48M_TPGS         480
#define EF_48M_TWK          96
#define EF_48M_TNVH         240
#define EF_48M_TRCV         480
#define EF_48M_TPROG        384
#define EF_48M_TCPS         3
#define EF_48M_TERASE       3840000
#define EF_48M_TME          3840000
#define EF_48M_TNVH1        4800
#define EF_48M_TCPS_TADH_TAH    ((EF_48M_TCPS<<24)|(EF_TADH<<16)|(EF_48M_TAH))
#define EF_48M_TWK_TPGS         ((EF_48M_TWK <<16)|(EF_48M_TPGS<<8))
#define EF_48M_TRCV_TNVH        ((EF_48M_TRCV<<16)|(EF_48M_TNVH))
#define EF_48M_TNVS_TNVH1       ((EF_48M_TNVS<<16)|(EF_48M_TNVH1))

#define EF_64M_TAH          2
#define EF_64M_TNVS         320
#define EF_64M_TPGS         640
#define EF_64M_TWK          128
#define EF_64M_TNVH         320
#define EF_64M_TRCV         640
#define EF_64M_TPROG        512
#define EF_64M_TCPS         3
#define EF_64M_TERASE       6400000
#define EF_64M_TME          6400000
#define EF_64M_TNVH1        6400
#define EF_64M_TCPS_TADH_TAH    ((EF_64M_TCPS<<24)|(EF_TADH<<16)|(EF_64M_TAH))
#define EF_64M_TWK_TPGS         ((EF_64M_TWK <<16)|(EF_64M_TPGS<<8))
#define EF_64M_TRCV_TNVH        ((EF_64M_TRCV<<16)|(EF_64M_TNVH))
#define EF_64M_TNVS_TNVH1       ((EF_64M_TNVS<<16)|(EF_64M_TNVH1))



/******************************************************************************
Declaration of External Variables & Functions
******************************************************************************/
// Sec 3: declaration of external variable

// Sec 4: declaration of external function prototype

/******************************************************************************
Declaration of data structure
******************************************************************************/
// Sec 5: structure, uniou, enum, linked list

//List supported system clock
typedef enum
{
    //add new clock support , must also add new parameters to g_aClockTable & g_aElfashTable
    CLOCK_16 = 0,
    CLOCK_24,
    CLOCK_32,
    CLOCK_48,
    CLOCK_64,
    CLOCK_TOTAL_SUPPORTED
}E_ClockSupported;

/******************************************************************************
Declaration of Global Variables & Functions
******************************************************************************/
// Sec 6: declaration of global variable

// Sec 7: declaration of global function prototype
void cc6801_ClockSysClkAdjust(E_ClockSupported clk);
void cc6801_ClockPeripheralClkAdjust(void);
void cc6801_ClockDelayUs(volatile uint32_t);
void cc6801_ClockDelayMs(volatile uint32_t);

/******************************************************************************
Declaration of static Global Variables & Functions
******************************************************************************/
// Sec 8: declaration of static global variable
typedef struct
{
    uint32_t dwMHz;
    uint32_t dwPllCfg;
    uint32_t dwClkCfg0;
    //uint32_t dwClkCfg1;   //value is same for every clock
    void (*pDelayFunc)(uint32_t);
} T_ClockConfig;

typedef struct
{
    uint32_t dwTcpsTadhTah;
    uint32_t dwTwkTpgs;
    uint32_t dwTrcvTnvh;
    uint32_t dwTprog;
    uint32_t dwTerase;
    uint32_t dwTme;
    uint32_t dwTnvsTnvh1;
} T_EflashConfig;


// Sec 9: declaration of static function prototype
static __ASM void __INLINE cc6801_ClockDelayUs_16MHz(volatile uint32_t);
static __ASM void __INLINE cc6801_ClockDelayUs_24MHz(volatile uint32_t);
static __ASM void __INLINE cc6801_ClockDelayUs_32MHz(volatile uint32_t);
static __ASM void __INLINE cc6801_ClockDelayUs_48MHz(volatile uint32_t);
static __ASM void __INLINE cc6801_ClockDelayUs_64MHz(volatile uint32_t);

/******************************************************************************
// Sec 10: C Functions
******************************************************************************/

#endif // _CC6801_CLOCK_H_
