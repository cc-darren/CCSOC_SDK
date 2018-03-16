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
*  hs.c
*
*  Project:
*  --------
*  cc6801
*
*  Description:
*  ------------
*  This is hardware sequencer driver
*
*  Author:
*  -------
*  CloudChip
*
*===========================================================================/
*  20170519 ML initial version
******************************************************************************/

#include <stdio.h>
#include "global.h"
#include "hs.h"
#include "cc6801_reg.h"
#include "clock.h"

extern T_ClockConfig g_aClockTable[CLOCK_TOTAL_SUPPORTED];
extern T_EflashConfig g_aEflashTable[CLOCK_TOTAL_SUPPORTED];

extern E_ClockSupported g_dwCurrentClock;

//define each bit for peripheral
//#define SCU_ISOCTRL_REG             (SCU_ADDR_BASE + 0x00000020)
    //#define ISO_PSO_WDT       0x00000001
    #define ISO_PSO_RTC         0x00000002
    #define ISO_PSO_PWM0        0x00000004
    #define ISO_PSO_PWM1        0x00000008
    #define ISO_PSO_WKT0        0x00000010
    #define ISO_PSO_WKT1        0x00000020
    #define ISO_PSO_SPI0        0x00000040
    #define ISO_PSO_SPI1        0x00000080
    #define ISO_PSO_SPI2        0x00000100
    #define ISO_PSO_UART0       0x00000200
    #define ISO_PSO_UART1       0x00000400
    #define ISO_PSO_UART2       0x00000800
    #define ISO_PSO_I2C0        0x00001000
    #define ISO_PSO_I2C1        0x00002000
    #define ISO_PSO_I2S         0x00004000
    #define ISO_PSO_DMIC        0x00008000
    #define ISO_PSO_BLE         0x00030000 //contains BLE and BLE_TIMING
    #define ISO_PSO_CCU         0x00040000
    #define ISO_PSO_AES         0x00080000
    #define ISO_PSO_VOICEWKUP   0x00100000
    //#define ISO_PSO_MO_M4F    0x00600000
    //#define ISO_PSO_EFLASH    0x00800000
    #define ISO_PSO_DMU         0x01000000
    #define ISO_PSO_GPIO        0x02000000
    #define ISO_PSO_CLKGEN      0x04000000
    #define ISO_PSO_TESTLOGIC   0x08000000
    #define ISO_PSO_SPI3        0x10000000
    #define ISO_PSO_BLEPHY      0x20000000

// definition bit for elk en 
//#define CKGEN_CLKEN_REG             (CKGEN_ADDR_BASE + 0x00000014)
    #define CLKEN_BLE       0x00000002
    //#define CLKEN_WKT0      0x00000004
    //#define CLKEN_WKT1      0x00000008
    #define CLKEN_GPIO      0x00000010
    //#define CLKEN_WDT       0x00000020
    //#define CLKEN_PWM0      0x00000040
    //#define CLKEN_PWM1      0x00000080
    #define CLKEN_I2C0      0x00000100
    #define CLKEN_I2C1      0x00000200
    #define CLKEN_I2S       0x00000400
    #define CLKEN_DMIC      0x00000800
    #define CLKEN_UART0     0x00001000
    #define CLKEN_UART1     0x00002000
    #define CLKEN_UART2     0x00004000
    #define CLKEN_SPI0      0x00008000
    #define CLKEN_SPI1      0x00010000
    #define CLKEN_SPI2      0x00020000
    #define CLKEN_CCU       0x00040000
    //#define CLKEN_RTC       0x00080000
    #define CLKEN_DMU       0x01000000

// definition bit for sw reset 
//#define CKGEN_SWRST_REG             (CKGEN_ADDR_BASE + 0x00000018)
    #define SWRST_BLEPHY    0x00000001
    #define SWRST_BLE       0x00800002
    //#define SWRST_WKT0      0x00000004
    //#define SWRST_WKT1      0x00000008
    #define SWRST_GPIO      0x00000010
    //#define SWRST_WDT       0x00000020
    //#define SWRST_PWM0      0x00000040
    //#define SWRST_PWM1      0x00000080
    #define SWRST_I2C0      0x00000100
    #define SWRST_I2C1      0x00000200
    #define SWRST_I2S       0x00000400
    #define SWRST_DMIC      0x00000800
    #define SWRST_UART0     0x00001000
    #define SWRST_UART1     0x00002000
    #define SWRST_UART2     0x00004000
    #define SWRST_SPI0      0x00008000
    #define SWRST_SPI1      0x00010000
    #define SWRST_SPI2      0x00020000
    #define SWRST_CCU       0x00040000
    //#define SWRST_RTC       0x00080000
    #define SWRST_AES       0x00200000
    #define SWRST_DMU       0x01000000

//#define AO_CKGEN_ADDR_BASE  0x40001400
//#define AO_CKGEN_CLKEN_REG             (AO_CKGEN_ADDR_BASE + 0x00000014)
    #define AO_CLKEN_WKT0      0x00000004
    #define AO_CLKEN_WKT1      0x00000008
    #define AO_CLKEN_WDT       0x00000020
    #define AO_CLKEN_PWM0      0x00000040
    #define AO_CLKEN_PWM1      0x00000080
    #define AO_CLKEN_RTC       0x00080000

//#define AO_CKGEN_SWRST_REG             (AO_CKGEN_ADDR_BASE + 0x00000018)
    #define AO_SWRST_WKT0      0x00000004
    #define AO_SWRST_WKT1      0x00000008
    #define AO_SWRST_WDT       0x00000020
    #define AO_SWRST_PWM0      0x00000040
    #define AO_SWRST_PWM1      0x00000080
    #define AO_SWRST_RTC       0x00080000



uint32_t PeripheralOn;

//set start address in retention ram for HS command list
void HS_BeginCmdList(cc_hs_clst_t * Cmdset,uint32_t lst_StAddr,uint32_t peripheralOn,uint32_t cmd_list_type)
{
    /*if (Cmdset!=NULL)
    {
        Cmdset->hs_clst_pos  = lst_StAddr;
        Cmdset->hs_clst_cnt  = 0;
        Cmdset->hs_clst_type = cmd_list_type;
    }*/
    uint32_t tmpPSO,tmpClkEN,tmpSWRST,tmpAOClkEN,tmpAOSWRST;
    
    Cmdset->hs_clst_pos  = lst_StAddr;
    Cmdset->hs_clst_cnt  = 0;
    Cmdset->hs_clst_type = cmd_list_type;
    
    tmpPSO      = 0x2F0FFFFE;
    tmpClkEN    = 0xFEF00001;
    tmpAOClkEN  = 0xFEF00001;
    tmpSWRST    = 0xFE500000;
    tmpAOSWRST  = 0xFE500000;
    
    if ( peripheralOn & HS_PERIPHERAL_WDT   ) {
        tmpAOClkEN |= AO_CLKEN_WDT;
        tmpAOSWRST |= AO_SWRST_WDT;
    }
    if ( peripheralOn & HS_PERIPHERAL_RTC   ) {
        tmpPSO   &= (~ISO_PSO_RTC);
        tmpAOClkEN |= AO_CLKEN_RTC;
        tmpAOSWRST |= AO_SWRST_RTC;
    }
    if ( peripheralOn & HS_PERIPHERAL_PWM0  ) {
        tmpPSO   &= (~ISO_PSO_PWM0);
        tmpAOClkEN |= AO_CLKEN_PWM0;
        tmpAOSWRST |= AO_SWRST_PWM0;
    }
    if ( peripheralOn & HS_PERIPHERAL_PWM1  ) {
        tmpPSO   &= (~ISO_PSO_PWM1);
        tmpAOClkEN |= AO_CLKEN_PWM1;
        tmpAOSWRST |= AO_SWRST_PWM1;
    }
    if ( peripheralOn & HS_PERIPHERAL_WKT0  ) {
        tmpPSO   &= (~ISO_PSO_WKT0);
        tmpAOClkEN |= AO_CLKEN_WKT0;
        tmpAOSWRST |= AO_SWRST_WKT0;
    }
    if ( peripheralOn & HS_PERIPHERAL_WKT1  ) {
        tmpPSO   &= (~ISO_PSO_WKT1);
        tmpAOClkEN |= AO_CLKEN_WKT1;
        tmpAOSWRST |= AO_SWRST_WKT1;
    }
    if ( peripheralOn & HS_PERIPHERAL_SPI0  ) {
        tmpPSO   &= (~ISO_PSO_SPI0);
        tmpClkEN |= CLKEN_SPI0;
        tmpSWRST |= SWRST_SPI0;
    }
    if ( peripheralOn & HS_PERIPHERAL_SPI1  ) {
        tmpPSO   &= (~ISO_PSO_SPI1);
        tmpClkEN |= CLKEN_SPI1;
        tmpSWRST |= SWRST_SPI1;
    }
    if ( peripheralOn & HS_PERIPHERAL_SPI2  ) {
        tmpPSO   &= (~ISO_PSO_SPI2);
        tmpClkEN |= CLKEN_SPI2;
        tmpSWRST |= SWRST_SPI2;
    }
    if ( peripheralOn & HS_PERIPHERAL_UART0 ) {
        tmpPSO   &= (~ISO_PSO_UART0);
        tmpClkEN |= CLKEN_UART0;
        tmpSWRST |= SWRST_UART0;
    }
    if ( peripheralOn & HS_PERIPHERAL_UART1 ) {
        tmpPSO   &= (~ISO_PSO_UART1);
        tmpClkEN |= CLKEN_UART1;
        tmpSWRST |= SWRST_UART1;
    }
    if ( peripheralOn & HS_PERIPHERAL_UART2 ) {
        tmpPSO   &= (~ISO_PSO_UART2);
        tmpClkEN |= CLKEN_UART2;
        tmpSWRST |= SWRST_UART2;
    }
    if ( peripheralOn & HS_PERIPHERAL_I2C0  ) {
        tmpPSO   &= (~ISO_PSO_I2C0);
        tmpClkEN |= CLKEN_I2C0;
        tmpSWRST |= SWRST_I2C0;
    }
    if ( peripheralOn & HS_PERIPHERAL_I2C1  ) {
        tmpPSO   &= (~ISO_PSO_I2C1);
        tmpClkEN |= CLKEN_I2C1;
        tmpSWRST |= SWRST_I2C1;
    }
    if ( peripheralOn & HS_PERIPHERAL_I2S   ) {
        tmpPSO   &= (~ISO_PSO_I2S);
        tmpClkEN |= CLKEN_I2S;
        tmpSWRST |= SWRST_I2S;
    }
    if ( peripheralOn & HS_PERIPHERAL_DMIC  ) {
        tmpPSO   &= (~ISO_PSO_DMIC);
        tmpClkEN |= CLKEN_DMIC;
        tmpSWRST |= SWRST_DMIC;
    }
    if ( peripheralOn & HS_PERIPHERAL_BLE   ) { 
        tmpPSO   &= (~ISO_PSO_BLE);
        tmpClkEN |= CLKEN_BLE;
        tmpSWRST |= SWRST_BLE;
    }
    if ( peripheralOn & HS_PERIPHERAL_CCU   ) {
        //Clock Calibrater Unit
        tmpPSO   &= (~ISO_PSO_CCU);
        tmpClkEN |= CLKEN_CCU;
        tmpSWRST |= SWRST_CCU;
    }
    if ( peripheralOn & HS_PERIPHERAL_AES   ) {
        tmpPSO   &= (~ISO_PSO_AES);
        //tmpClkEN |= CLKEN_AES;
        tmpSWRST |= SWRST_AES;
    }
//    if ( peripheralOn & HS_PERIPHERAL_EFLASH) {
//        tmpPSO   &= (~ISO_PSO_EFLASH);
//    }
    if ( peripheralOn & HS_PERIPHERAL_DMU   ) {
        tmpPSO   &= (~ISO_PSO_DMU);
        tmpClkEN |= CLKEN_DMU;
        tmpSWRST |= SWRST_DMU;
    }
    if ( peripheralOn & HS_PERIPHERAL_GPIO  ) {
        tmpPSO   &= (~ISO_PSO_GPIO);
        tmpClkEN |= CLKEN_GPIO;
        tmpSWRST |= SWRST_GPIO;
    }
//    if ( peripheralOn & HS_PERIPHERAL_DRAM  ) {
//        tmpPSO   &= (~ISO_PSO_DRAM);
//    }
    if ( peripheralOn & HS_PERIPHERAL_TESTLOGIC  ) {
        tmpPSO   &= (~ISO_PSO_TESTLOGIC);
    }
    if ( peripheralOn & HS_PERIPHERAL_BLEPHY   ) { //BLEPHY is default set on in HS
        tmpPSO   &= (~ISO_PSO_BLEPHY);
        tmpSWRST |= SWRST_BLEPHY;
    }
    if ( peripheralOn & HS_PERIPHERAL_CLKGEN   ) {
        tmpPSO   &= (~ISO_PSO_CLKGEN);
    }


    if (cmd_list_type == HS_CMDLIST_PRD) {

        //switch clock to 2M, or set clock as reset will casue device reset
        //rd(SCU_CLKCFG1_REG,rdata);
        //HS_setHSCmd(Cmdset,SCU_CLKCFG1_REG,(rdata&(~0x00000003)) ); 

        //set peripheral to reset and disable clock
        HS_setHSCmd(Cmdset,CKGEN_SWRST_REG,tmpSWRST);
        HS_setHSCmd(Cmdset,CKGEN_CLKEN_REG,tmpClkEN);

        HS_setHSCmd(Cmdset,(uint32_t)(&(regAOCKGEN->swReset)),tmpAOSWRST);
        HS_setHSCmd(Cmdset,(uint32_t)(&(regAOCKGEN->clkEn  )),tmpAOClkEN);

        wr(HS_PSO_PDN_REG,(tmpPSO|0x80000000)); //0x80000000 this will be dones after all command. set bit31 before enter sleep and HS will clear this bit automatically
                                                //0x00600000 set CPU to power off in sleep period
    }
    else
    {
        // this part is all about wakeup.

        // set clock to original clock.
        HS_setHSCmd(Cmdset,SCU_PLLCFG_REG, g_aClockTable[g_dwCurrentClock].dwPllCfg |PLL_BYPASS_MASK);
        HS_setHSCmd(Cmdset,SCU_PLLRST_REG, 1);
        HS_setHSCmd(Cmdset,SCU_PLLRST_REG, 0);
        HS_setHSCmd(Cmdset,SCU_PLLCFG_REG, g_aClockTable[g_dwCurrentClock].dwPllCfg );
        HS_setHSCmd(Cmdset,SCU_CLKCFG0_REG,g_aClockTable[g_dwCurrentClock].dwClkCfg0);
        HS_setHSCmd(Cmdset,SCU_CLKCFG1_REG,SCU_CLK_CFG1);

        HS_setHSCmd(Cmdset,EF_TIMING0_REG,g_aEflashTable[g_dwCurrentClock].dwTcpsTadhTah);
        HS_setHSCmd(Cmdset,EF_TIMING1_REG,g_aEflashTable[g_dwCurrentClock].dwTwkTpgs);
        HS_setHSCmd(Cmdset,EF_TIMING2_REG,g_aEflashTable[g_dwCurrentClock].dwTrcvTnvh);
        HS_setHSCmd(Cmdset,EF_TIMING3_REG,g_aEflashTable[g_dwCurrentClock].dwTprog);
        HS_setHSCmd(Cmdset,EF_TIMING4_REG,g_aEflashTable[g_dwCurrentClock].dwTerase);
        HS_setHSCmd(Cmdset,EF_TIMING5_REG,g_aEflashTable[g_dwCurrentClock].dwTme);
        HS_setHSCmd(Cmdset,EF_TIMING6_REG,g_aEflashTable[g_dwCurrentClock].dwTnvsTnvh1);

        if (cmd_list_type == HS_CMDLIST_PUP3)
        {
            HS_setHSCmd(Cmdset,CKGEN_CLKEN_REG,tmpClkEN);
            //HS_setHSCmd(Cmdset,AO_CKGEN_CLKEN_REG,tmpAOClkEN);
            HS_setHSCmd(Cmdset,(uint32_t)(&(regAOCKGEN->clkEn)),tmpAOClkEN);
            HS_setHSCmd(Cmdset,SCU_PSOCTRL_REG,tmpPSO);
            HS_setHSCmd(Cmdset,SCU_ISOCTRL_REG,tmpPSO);
            HS_setHSCmd(Cmdset,CKGEN_SWRST_REG,tmpSWRST);
            //HS_setHSCmd(Cmdset,AO_CKGEN_SWRST_REG,tmpAOSWRST);
            HS_setHSCmd(Cmdset,(uint32_t)(&(regAOCKGEN->swReset)),tmpAOSWRST);
        }
        else
        {
            HS_setHSCmd(Cmdset,CKGEN_CLKEN_REG,tmpClkEN);
            //HS_setHSCmd(Cmdset,AO_CKGEN_CLKEN_REG,tmpAOClkEN);
            HS_setHSCmd(Cmdset,(uint32_t)(&(regAOCKGEN->clkEn)),tmpAOClkEN);
            HS_setHSCmd(Cmdset,SCU_PSOCTRL_REG,tmpPSO);
            HS_setHSCmd(Cmdset,SCU_ISOCTRL_REG,tmpPSO);
            HS_setHSCmd(Cmdset,CKGEN_SWRST_REG,tmpSWRST);
            //HS_setHSCmd(Cmdset,AO_CKGEN_SWRST_REG,tmpAOSWRST);
            HS_setHSCmd(Cmdset,(uint32_t)(&(regAOCKGEN->swReset)),tmpAOSWRST);
        }
    }
    
}


void HS_setHSCmd(cc_hs_clst_t * Cmdset,uint32_t target_addr,uint32_t target_value)
{
    uint32_t offset;
    offset = Cmdset->hs_clst_cnt<<2;
    wr(Cmdset->hs_clst_pos+offset  ,target_addr);
    wr(Cmdset->hs_clst_pos+offset+4,target_value);
    Cmdset->hs_clst_cnt+=2;
}

/*uint32_t HS_EndCmdList(cc_hs_clst_t * Cmdset)
{
    uint32_t cmdlst_reg;
    
    
    switch (Cmdset->hs_clst_type) {
        case HS_CMDLIST_PRD:
            cmdlst_reg = HS_DR_PDN_REG;
            break;
        case HS_CMDLIST_PUP0:
            cmdlst_reg = HS_DR0_PUP_REG;
            break;
        case HS_CMDLIST_PUP1:
            cmdlst_reg = HS_DR1_PUP_REG;
            break;
        case HS_CMDLIST_PUP2:
            cmdlst_reg = HS_DR2_PUP_REG;
            break;
        case HS_CMDLIST_PUP3:
            cmdlst_reg = HS_DR3_PUP_REG;
            break;
    }
    wr(cmdlst_reg,( (Cmdset->hs_clst_cnt<<24) | (Cmdset->hs_clst_pos&0x0003FFFF) ) );
    return Cmdset->hs_clst_pos;
}*/


