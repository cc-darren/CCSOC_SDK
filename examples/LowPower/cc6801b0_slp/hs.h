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

#ifndef _HS_H_
#define _HS_H_

#include "global.h"

#define HS_STATE_NORMAL 0x00000005


#define HS_PERIPHERAL_WDT       0x00000001
#define HS_PERIPHERAL_RTC       0x00000002
#define HS_PERIPHERAL_PWM0      0x00000004
#define HS_PERIPHERAL_PWM1      0x00000008
#define HS_PERIPHERAL_WKT0      0x00000010
#define HS_PERIPHERAL_WKT1      0x00000020
#define HS_PERIPHERAL_SPI0      0x00000040
#define HS_PERIPHERAL_SPI1      0x00000080
#define HS_PERIPHERAL_SPI2      0x00000100
#define HS_PERIPHERAL_UART0     0x00000200
#define HS_PERIPHERAL_UART1     0x00000400
#define HS_PERIPHERAL_UART2     0x00000800
#define HS_PERIPHERAL_I2C0      0x00001000
#define HS_PERIPHERAL_I2C1      0x00002000
#define HS_PERIPHERAL_I2S       0x00004000
#define HS_PERIPHERAL_DMIC      0x00008000
#define HS_PERIPHERAL_BLE       0x00010000
#define HS_PERIPHERAL_CCU       0x00020000
#define HS_PERIPHERAL_AES       0x00040000
//#define HS_PERIPHERAL_EFLASH    0x00080000
#define HS_PERIPHERAL_DMU       0x00100000
#define HS_PERIPHERAL_GPIO      0x00200000
//#define HS_PERIPHERAL_DRAM      0x00400000
#define HS_PERIPHERAL_TESTLOGIC 0x00800000
#define HS_PERIPHERAL_BLEPHY    0x01000000
#define HS_PERIPHERAL_CLKGEN    0x02000000


typedef struct
{
    uint32_t hs_clst_pos;
    uint32_t hs_clst_type;
    uint32_t hs_clst_cnt;
} cc_hs_clst_t;


typedef struct
{
    uint32_t hs_ISO_PSO;
    uint32_t hs_CLKEN;
    uint32_t hs_SWRST;
} cc_hs_PPS_t;

//caller should use HS_BeginCmdList and HS_EndCmdList as a pair
void        HS_BeginCmdList(cc_hs_clst_t * Cmdset,uint32_t lst_StAddr,uint32_t peripheralswitch,uint32_t cmd_list_type);  //set cmd list start address and periperal switch
void        HS_setHSCmd(cc_hs_clst_t * Cmdset,uint32_t target_addr,uint32_t target_value);  //fill the cmd list content
uint32_t    HS_EndCmdList(cc_hs_clst_t * Cmdset);      //return the end address with the cmd list

enum {
    HS_PWRMODE_NORMAL,
    HS_PWRMODE_SLEEP,
    HS_PWRMODE_DEEPSLEEP,
    HS_PWRMODE_HIBERNATE
};


enum {
    HS_CMDLIST_PRD,     //for power down
    HS_CMDLIST_PUP0,    //for extr 0 pup
    HS_CMDLIST_PUP1,    //for extr 1 pup
    HS_CMDLIST_PUP2,    //for extr 2 pup
    HS_CMDLIST_PUP3   //set peripheral after wakeup
};

//SCU bit definition
//#define SCU_CLKCFG1_REG             (SCU_ADDR_BASE + 0x00000014)
#define SCU_CLKCFG1_REG_HSGatedClk  0x00000200

// HS bit definition
//#define HS_CONFIG_REG               (HS_ADDR_BASE + 0x00000000)
//bit 31
#define HS_CONFIG_ClrIntr           0x80000000
//bit 11
#define HS_CONFIG_2M_Enable         0x00000800
#define HS_CONFIG_2M_Disable        0x00000000
//bit 10
#define HS_CONFIG_32K_Enable        0x00000400
#define HS_CONFIG_32K_Disable       0x00000000
//bit 9
#define HS_CONFIG_WDT_INTR_CLR_EN   0x00000200
#define HS_CONFIG_WDT_INTR_CLR_DIS  0x00000200
//bit 0-1
#define HS_CONFIG_PWRMODE       0x00000003
  #define HS_CONFIG_PWRMODE_SleepLite 0x00000000
  #define HS_CONFIG_PWRMODE_Sleep     0x00000001
  #define HS_CONFIG_PWRMODE_DeepSleep 0x00000002
  #define HS_CONFIG_PWRMODE_Hibernate 0x00000003



//#define HS_STS_REG                  (HS_ADDR_BASE + 0x00000004)
//#define HS_DR_PDN_REG               (HS_ADDR_BASE + 0x00000008)
//#define HS_DR0_PUP_REG              (HS_ADDR_BASE + 0x0000000C)
//#define HS_DR0_INTR_REG             (HS_ADDR_BASE + 0x00000014)
//#define HS_DR1_PUP_REG              (HS_ADDR_BASE + 0x00000018)
//#define HS_DR1_INTR_REG             (HS_ADDR_BASE + 0x00000020)
//#define HS_DR2_PUP_REG              (HS_ADDR_BASE + 0x00000024)
//#define HS_DR2_INTR_REG             (HS_ADDR_BASE + 0x0000002C)
//#define HS_PSO_PDN_REG              (HS_ADDR_BASE + 0x00000030)
//#define HS_LDO_PDN_REG              (HS_ADDR_BASE + 0x00000034)

#endif // _HS_H_
