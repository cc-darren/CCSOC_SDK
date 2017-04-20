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
*  cc6801_reg.h
*
*  Project:
*  --------
*  cc6801
*
*  Description:
*  ------------
*  This header file defines all CC6801 HW registers in detail.
*  When there is any changes(ECO) from CC6801, this file is needed to be reviewed.
*
*  Author:
*  -------
*  PATTAYA
*
*===========================================================================/
*  20170123 PAT fill in all register
*  20170120 PAT initial version
******************************************************************************/

#ifndef _CC6801_REG_H_
#define _CC6801_REG_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

//macro
#define delayns(x)  do{uint32_t ipp;for(ipp=0;ipp<x;ipp++){volatile uint32_t iqq;iqq=0;}}while(0);
#define wr(x,y)     *((volatile unsigned int *)(x)) = y
#define rd(x,y)     y = *((volatile unsigned int *)(x))


//// ------------------------------------
//// parameter
//// ------------------------------------
//// chip
#define CHIP_ID                     0   //24bit
#define SHRMEM_ADDR                 20
#define SHRMEM_ADDR_OFS             16
#define DBUS_CH                     17
#define EFLASH_MODULE               1   //3bit
#define EFLASH_BYTE_ADDR_WIDTH      18  //5bit
#define IC_A_WIDTH                  13
#define IC_L_WIDTH                  4
#define DATARAM_ADDR_WIDTH          11
#define ROM_ADDR_START              0x00000000
#define ROM_ADDR_SIZE               0x00020000
#define EF_ADDR_START               0x00020000
#define EF_ADDR_SIZE                0x00040000
#define SYSRAM_ADDR_START           0x00060000
#define SYSRAM_ADDR_SIZE            0x00020000
#define DATARAM_ADDR_START          0x00080000
#define DATARAM_ADDR_SIZE           0x00001000
#define BLE_ADDR_START              0x80002200
#define BLE_ADDR_SIZE               0x00000100
#define HS_ADDR_START               0x80002500
#define HS_ADDR_SIZE                0x00001000
#define AHB_CHANNEL                 3


//// clock
#define CLK32K_PERIOD_DIV2          ((1000000000/32768)/2)
#define CLK48M_PERIOD_DIV2          (10.416)
#define CLKPLL_PERIOD_DIV2          (10/2)
#define CLK20M_PERIOD_DIV2          (50/2)


// mem
#define ROM_ADDR_BASE               0x00000000
#define ROM_SIZE                    0x00020000  // 128KB
#define EF_BASE                     0x30000000
#define EF_SIZE                     0x30040000  // 256KB
#define SYSRAM_ADDR_BASE            0x20000000
#define SYSRAM_SIZE                 0x00018000  // 96KB
#define DATARAM_ADDR_BASE           0x20020000
#define DATARAM_SIZE                0x00002000  // 8KB

// mem read write test
#define SYSRAM_TEST_ADDR_BASE       0x20010000

// io
#define REG_SIZE                    0x00000100  // 256B
#define BLE_REG_SIZE                0x00020000  // 8k use 16k
#define SCU_ADDR_BASE               0x40000000
#define CKGEN_ADDR_BASE             0x40000100
#define GPIO_ADDR_BASE              0x40000200
#define WDT_ADDR_BASE               0x40000300
#define PWM0_ADDR_BASE              0x40000400
#define PWM1_ADDR_BASE              0x40000500
#define WKTM0_ADDR_BASE             0x40000600
#define WKTM1_ADDR_BASE             0x40000700

#define RTC_ADDR_BASE               0x40000800
#define I2S_ADDR_BASE               0x40000900
#define DMIC_ADDR_BASE              0x40000a00
#define SPI0_ADDR_BASE              0x40000b00
#define SPI1_ADDR_BASE              0x40000c00
#define SPI2_ADDR_BASE              0x40000d00
#define UART0_ADDR_BASE             0x40000e00
#define UART1_ADDR_BASE             0x40000f00

#define UART2_ADDR_BASE             0x40001000
#define I2C0_ADDR_BASE              0x40001100
#define I2C1_ADDR_BASE              0x40001200
#define DMU_ADDR_BASE               0x40001300

#define EF_ADDR_BASE                0x40002000
#define OTP_ADDR_BASE               0x40002100
#define HS_ADDR_BASE                0x40002200
#define PMU_ADDR_BASE               0x40002300
#define AES_ADDR_BASE               0x40002400
#define CCU_ADDR_BASE               0x40002500

#define BLE_ADDR_BASE               0x40004000

// EF
#define EF_INTERRUPT_REG            (EF_ADDR_BASE + 0x00000000)
#define EF_CONFIG_REG               (EF_ADDR_BASE + 0x00000004)
#define EF_ACCESS_REG               (EF_ADDR_BASE + 0x00000008)
#define EF_WR_DATA0_REG             (EF_ADDR_BASE + 0x0000000c)
#define EF_WR_DATA1_REG             (EF_ADDR_BASE + 0x00000010)
#define EF_WR_DATA2_REG             (EF_ADDR_BASE + 0x00000014)
#define EF_WR_DATA3_REG             (EF_ADDR_BASE + 0x00000018)
#define EF_FLASHMODE_REG            (EF_ADDR_BASE + 0x0000001c)
#define EF_RD_DATA0_REG             (EF_ADDR_BASE + 0x00000020)
#define EF_RD_DATA1_REG             (EF_ADDR_BASE + 0x00000024)
#define EF_REDUN_DATA_REG           (EF_ADDR_BASE + 0x00000028)

#define EF_TIMING0_REG              (EF_ADDR_BASE + 0x00000044)
#define EF_TIMING1_REG              (EF_ADDR_BASE + 0x00000048)
#define EF_TIMING2_REG              (EF_ADDR_BASE + 0x0000004c)
#define EF_TIMING3_REG              (EF_ADDR_BASE + 0x00000050)
#define EF_TIMING4_REG              (EF_ADDR_BASE + 0x00000054)
#define EF_TIMING5_REG              (EF_ADDR_BASE + 0x00000058)
#define EF_TIMING6_REG              (EF_ADDR_BASE + 0x0000005c)

#define EF_DMA_CTRL_REG             (EF_ADDR_BASE + 0x00000060)
#define EF_DMA_WADDR_REG            (EF_ADDR_BASE + 0x00000064)
#define EF_DMA_RADDR_REG            (EF_ADDR_BASE + 0x00000068)

// SCU
#define SCU_INTRCTRL_REG            (SCU_ADDR_BASE + 0x00000000)
#define SCU_PLLLOCK_REG             (SCU_ADDR_BASE + 0x00000004)
#define SCU_PLLRST_REG              (SCU_ADDR_BASE + 0x00000008)
#define SCU_PLLCFG_REG              (SCU_ADDR_BASE + 0x0000000c)
#define SCU_CLKCFG0_REG             (SCU_ADDR_BASE + 0x00000010)
#define SCU_CLKCFG1_REG             (SCU_ADDR_BASE + 0x00000014)
#define SCU_CHIPID_REG              (SCU_ADDR_BASE + 0x0000001c)
#define SCU_ISOCTRL_REG             (SCU_ADDR_BASE + 0x00000020)
#define SCU_PSOCTRL_REG             (SCU_ADDR_BASE + 0x00000024)
#define SCU_RETCTRL_REG             (SCU_ADDR_BASE + 0x00000028)
#define SCU_EXTAUX_REG              (SCU_ADDR_BASE + 0x0000002c)
#define SCU_HSCLK_REG               (SCU_ADDR_BASE + 0x00000030)
#define SCU_ICACHE_REG              (SCU_ADDR_BASE + 0x00000034)
#define SCU_CLK32K_CAL_REG          (SCU_ADDR_BASE + 0x00000038)
#define SCU_INTRMASK_REG            (SCU_ADDR_BASE + 0x0000003c)
#define SCU_INTRINFO_REG            (SCU_ADDR_BASE + 0x00000040)

// CKGEN
#define CKGEN_CFG1_REG              (CKGEN_ADDR_BASE + 0x00000004)
#define CKGEN_CFG2_REG              (CKGEN_ADDR_BASE + 0x00000008)
#define CKGEN_CFG3_REG              (CKGEN_ADDR_BASE + 0x0000000c)
#define CKGEN_CFG4_REG              (CKGEN_ADDR_BASE + 0x00000010)
#define CKGEN_CLKEN_REG             (CKGEN_ADDR_BASE + 0x00000014)
#define CKGEN_SWRST_REG             (CKGEN_ADDR_BASE + 0x00000018)
#define CKGEN_BLECLKSEL_REG         (CKGEN_ADDR_BASE + 0x0000001c)

// GPIO
#define GPIO_PAD_OUT_REG            (GPIO_ADDR_BASE + 0x00000000)
#define GPIO_PAD_IN_REG             (GPIO_ADDR_BASE + 0x00000004)
#define GPIO_PAD_OE_REG             (GPIO_ADDR_BASE + 0x00000008)
#define GPIO_INTR_PRIO_REG          (GPIO_ADDR_BASE + 0x0000000c)
#define GPIO_INTR_EN_REG            (GPIO_ADDR_BASE + 0x00000010)
#define GPIO_INTR_STAT_REG          (GPIO_ADDR_BASE + 0x00000014)
#define GPIO_INTR_TRIG_REG          (GPIO_ADDR_BASE + 0x00000018)
#define GPIO_AUX_REG                (GPIO_ADDR_BASE + 0x0000001c)
#define GPIO_INTR_TYPE_REG          (GPIO_ADDR_BASE + 0x00000020)
#define GPIO_PAD_PULLUP_REG         (GPIO_ADDR_BASE + 0x00000024)
#define GPIO_AUX_PORT_MODE_REG      (GPIO_ADDR_BASE + 0x00000028)

#define GPIO1_PAD_OUT_REG           (GPIO_ADDR_BASE + 0x0000002c)
#define GPIO1_PAD_IN_REG            (GPIO_ADDR_BASE + 0x00000030)
#define GPIO1_PAD_OE_REG            (GPIO_ADDR_BASE + 0x00000034)
#define GPIO1_INTR_PRIO_REG         (GPIO_ADDR_BASE + 0x00000038)
#define GPIO1_INTR_EN_REG           (GPIO_ADDR_BASE + 0x0000003c)
#define GPIO1_INTR_STAT_REG         (GPIO_ADDR_BASE + 0x00000040)
#define GPIO1_INTR_TRIG_REG         (GPIO_ADDR_BASE + 0x00000044)
#define GPIO1_AUX_REG               (GPIO_ADDR_BASE + 0x00000048)
#define GPIO1_INTR_TYPE_REG         (GPIO_ADDR_BASE + 0x0000004c)
#define GPIO1_PAD_PULLUP_REG        (GPIO_ADDR_BASE + 0x00000050)
#define GPIO1_AUX_PORT_MODE_REG     (GPIO_ADDR_BASE + 0x00000054)

// SPI
#define SPI0_INTR_REG               (SPI0_ADDR_BASE + 0x00000000)
#define SPI0_CTRL_REG               (SPI0_ADDR_BASE + 0x00000004)
#define SPI0_DMA_CTRL_REG           (SPI0_ADDR_BASE + 0x00000008)
#define SPI0_DMA_WR_REG             (SPI0_ADDR_BASE + 0x0000000c)
#define SPI0_DMA_RD_REG             (SPI0_ADDR_BASE + 0x00000010)

#define SPI1_INTR_REG               (SPI1_ADDR_BASE + 0x00000000)
#define SPI1_CTRL_REG               (SPI1_ADDR_BASE + 0x00000004)
#define SPI1_DMA_CTRL_REG           (SPI1_ADDR_BASE + 0x00000008)
#define SPI1_DMA_WR_REG             (SPI1_ADDR_BASE + 0x0000000c)
#define SPI1_DMA_RD_REG             (SPI1_ADDR_BASE + 0x00000010)

#define SPI1_INTR_REG               (SPI1_ADDR_BASE + 0x00000000)
#define SPI1_CTRL_REG               (SPI1_ADDR_BASE + 0x00000004)
#define SPI1_DMA_CTRL_REG           (SPI1_ADDR_BASE + 0x00000008)
#define SPI1_DMA_WR_REG             (SPI1_ADDR_BASE + 0x0000000c)
#define SPI1_DMA_RD_REG             (SPI1_ADDR_BASE + 0x00000010)

// WDT
#define WDT_TIMER0_REG              (WDT_ADDR_BASE + 0x00000000)
#define WDT_TIMER1_REG              (WDT_ADDR_BASE + 0x00000004)
#define WDT_TIMER2_REG              (WDT_ADDR_BASE + 0x00000008)
#define WDT_TIMER3_REG              (WDT_ADDR_BASE + 0x0000000c)

// RTC
#define RTC_SEC_REG                 (RTC_ADDR_BASE + 0x00000000)
#define RTC_MIN_REG                 (RTC_ADDR_BASE + 0x00000004)
#define RTC_HRS_REG                 (RTC_ADDR_BASE + 0x00000008)
#define RTC_DOW_REG                 (RTC_ADDR_BASE + 0x0000000c)
#define RTC_DAY_REG                 (RTC_ADDR_BASE + 0x00000010)
#define RTC_MON_REG                 (RTC_ADDR_BASE + 0x00000014)
#define RTC_YRS_REG                 (RTC_ADDR_BASE + 0x00000018)

#define RTC_ALM_SEC_REG             (RTC_ADDR_BASE + 0x0000001c)
#define RTC_ALM_MIN_REG             (RTC_ADDR_BASE + 0x00000020)
#define RTC_ALM_HRS_REG             (RTC_ADDR_BASE + 0x00000024)
#define RTC_ALM_DOM_REG             (RTC_ADDR_BASE + 0x00000028)
#define RTC_ALM_MON_REG             (RTC_ADDR_BASE + 0x0000002c)

#define RTC_ALM2_SEC_REG            (RTC_ADDR_BASE + 0x00000030)
#define RTC_ALM2_MIN_REG            (RTC_ADDR_BASE + 0x00000034)
#define RTC_ALM2_HRS_REG            (RTC_ADDR_BASE + 0x00000038)
#define RTC_ALM2_DOM_REG            (RTC_ADDR_BASE + 0x0000003c)
#define RTC_ALM2_MON_REG            (RTC_ADDR_BASE + 0x00000040)

#define RTC_CTRLA_REG               (RTC_ADDR_BASE + 0x00000044)
#define RTC_CTRLB_REG               (RTC_ADDR_BASE + 0x00000048)
#define RTC_CTRLC_REG               (RTC_ADDR_BASE + 0x00000050)

// CCU
#define CCU_INTR_REG                (CCU_ADDR_BASE + 0x00000000)
#define CCU_DELTA_CNT_REG           (CCU_ADDR_BASE + 0x00000004)
#define CCU_FDELTA_CNT_REG          (CCU_ADDR_BASE + 0x00000008)

// UART
#define UART0_INTR_REG              (UART0_ADDR_BASE + 0x00000000)
#define UART0_DMA_CTRL_REG          (UART0_ADDR_BASE + 0x00000004)
#define UART0_DMA_RADDR_START_REG   (UART0_ADDR_BASE + 0x00000008)
#define UART0_DMA_RADDR_END_REG     (UART0_ADDR_BASE + 0x0000000C)
#define UART0_DMA_RDEN_REG          (UART0_ADDR_BASE + 0x00000010)
#define UART0_DMA_WADDR_START_REG   (UART0_ADDR_BASE + 0x00000014)
#define UART0_DMA_WADDR_END_REG     (UART0_ADDR_BASE + 0x00000018)
#define UART0_DMA_WR_EN_REG         (UART0_ADDR_BASE + 0x0000001C)
#define UART0_DMA_DBUSADDR_RX_REG   (UART0_ADDR_BASE + 0x00000020)
#define UART0_DMA_DBUSADDR_TX_REG   (UART0_ADDR_BASE + 0x00000024)
#define UART0_DMA_AGR_RX_REG        (UART0_ADDR_BASE + 0x00000028)
#define UART0_UnTBUF_REG            (UART0_ADDR_BASE + 0x00000080)
#define UART0_UnRBUF_REG            (UART0_ADDR_BASE + 0x00000084)
#define UART0_UnICTRL_REG           (UART0_ADDR_BASE + 0x00000088)
#define UART0_UnSTAT_REG            (UART0_ADDR_BASE + 0x0000008C)
#define UART0_UnFRS_REG             (UART0_ADDR_BASE + 0x00000090)
#define UART0_UnMDSL_REG            (UART0_ADDR_BASE + 0x00000094)
#define UART0_UnBAUD_REG            (UART0_ADDR_BASE + 0x00000098)
#define UART0_UnPSR_REG             (UART0_ADDR_BASE + 0x0000009C)
#define UART0_UnOVR_REG             (UART0_ADDR_BASE + 0x000000A0)
#define UART0_UnMDSL2_REG           (UART0_ADDR_BASE + 0x000000A4)
#define UART0_UnSPOS_REG            (UART0_ADDR_BASE + 0x000000A8)
#define UART0_UnWPSRH_REG           (UART0_ADDR_BASE + 0x000000AC)
#define UART0_UnWPSRL_REG           (UART0_ADDR_BASE + 0x000000B0)

#define UART1_UnBAUD_REG            (UART1_ADDR_BASE + 0x00000018)
#define UART1_UnPSR_REG             (UART1_ADDR_BASE + 0x0000001c)
#define UART1_UnOVR_REG             (UART1_ADDR_BASE + 0x00000020)

#define UART2_UnBAUD_REG            (UART2_ADDR_BASE + 0x00000018)
#define UART2_UnPSR_REG             (UART2_ADDR_BASE + 0x0000001c)
#define UART2_UnOVR_REG             (UART2_ADDR_BASE + 0x00000020)

// HS
#define HS_CONFIG_REG               (HS_ADDR_BASE + 0x00000000)
#define HS_STS_REG                  (HS_ADDR_BASE + 0x00000004)
#define HS_DR_PDN_REG               (HS_ADDR_BASE + 0x00000008)
#define HS_DR0_PUP_REG              (HS_ADDR_BASE + 0x0000000c)
#define HS_DR0_PUP1_REG             (HS_ADDR_BASE + 0x00000010)
#define HS_DR0_INTR_REG             (HS_ADDR_BASE + 0x00000014)
#define HS_DR1_PUP_REG              (HS_ADDR_BASE + 0x00000018)
#define HS_DR1_PUP1_REG             (HS_ADDR_BASE + 0x0000001c)
#define HS_DR1_INTR_REG             (HS_ADDR_BASE + 0x00000020)
#define HS_DR2_PUP_REG              (HS_ADDR_BASE + 0x00000024)
#define HS_DR2_PUP1_REG             (HS_ADDR_BASE + 0x00000028)
#define HS_DR2_INTR_REG             (HS_ADDR_BASE + 0x0000002c)
#define HS_SCRATCH_BASE_REG         (HS_ADDR_BASE + 0x00000030)

#define AES_INTR_REG                (AES_ADDR_BASE + 0x00000000)
#define AES_CTRL_REG                (AES_ADDR_BASE + 0x00000004)
#define AES_DMA_CTRL_REG            (AES_ADDR_BASE + 0x00000008)
#define AES_DMA_WADDR_REG           (AES_ADDR_BASE + 0x0000000c)
#define AES_DMA_RADDR_REG           (AES_ADDR_BASE + 0x00000010)



/******************************/
/* cc6801 register definition */
/******************************/
typedef union U_regSCU
{
    struct
    {
        uint32_t reserved0;
        uint32_t adc;
        uint32_t pllReset;
        uint32_t pllConfig;
        uint32_t clkConfig0;
        uint32_t clkConfig1;
        uint32_t reserved1;
        uint32_t chipID;
        uint32_t isolationEn;
        uint32_t shutOff;
        uint32_t retentionEn;
        uint32_t extAuxSel;
        uint32_t codeRemap;
        uint32_t cacheBootCtrl;
        uint32_t clk32Calib;
        uint32_t chipIntEn;
        uint32_t chipIntSts;
    }dw;    //double word

    struct
    {
        //PATTAYA: not ready yet
        uint32_t padding0;
    }bf;    //bit-field
}U_regSCU;

typedef union U_regCKGEN
{
    struct
    {
        uint32_t reserved0;
        uint32_t clkCfg1;
        uint32_t clkCfg2;
        uint32_t clkCfg3;
        uint32_t clkCfg4;
        uint32_t clkEn;
        uint32_t swReset;
        uint32_t bleClkSel;
    }dw;    //double word

    struct
    {
        uint32_t reserved0:32;
        uint32_t bleClkDiv:5;
        uint32_t bleLowFreqClkSel:1;
        uint32_t reserved1:2;
        uint32_t wktClkDiv:5;
        uint32_t wktLowFreqClkSel:1;
        uint32_t reserved2:2;
        uint32_t gpioClkDiv:5;
        uint32_t gpioLowFreqClkSel:1;
        uint32_t reserved3:2;
        uint32_t wdtClkDiv:5;
        uint32_t wdtLowFreqClkSel:1;
        uint32_t reserved4:2;
        uint32_t pwmClkDiv:5;
        uint32_t pwmLowFreqClkSel:1;
        uint32_t reserved5:2;
        uint32_t i2cClkDiv:5;
        uint32_t i2cLowFreqClkSel:1;
        uint32_t reserved6:2;
        uint32_t i2sClkDiv:5;
        uint32_t i2sLowFreqClkSel:1;
        uint32_t reserved7:2;
        uint32_t dmicClkDiv:5;
        uint32_t dmicLowFreqClkSel:1;
        uint32_t reserved8:2;
        uint32_t uartClkDiv:5;
        uint32_t uartLowFreqClkSel:1;
        uint32_t reserved9:2;
        uint32_t spi0ClkDiv:5;
        uint32_t spi0LowFreqClkSel:1;
        uint32_t reserved10:2;
        uint32_t spi1ClkDiv:5;
        uint32_t spi1LowFreqClkSel:1;
        uint32_t reserved11:2;
        uint32_t spi2ClkDiv:5;
        uint32_t spi2LowFreqClkSel:1;
        uint32_t reserved12:2;
        uint32_t ClkDiv:5;
        uint32_t LowFreqClkSel:1;
        uint32_t reserved13:26;
        uint32_t reserved14:1;
        uint32_t bleClkEn:1;
        uint32_t wktm0ClkEn:1;
        uint32_t wktm1ClkEn:1;
        uint32_t gpio0ClkEn:1;
        uint32_t wdtClkEn:1;
        uint32_t pwm0ClkEn:1;
        uint32_t pwm1ClkEn:1;
        uint32_t i2c0ClkEn:1;
        uint32_t i2c1ClkEn:1;
        uint32_t i2sClkEn:1;
        uint32_t dmicClkEn:1;
        uint32_t uart0ClkEn:1;
        uint32_t uart1ClkEn:1;
        uint32_t uart2ClkEn:1;
        uint32_t spim0ClkEn:1;
        uint32_t spim1ClkEn:1;
        uint32_t spim2ClkEn:1;
        uint32_t ccuClkEn:1;
        uint32_t rtcClkEn:1;
        uint32_t reserved15:4;
        uint32_t dmuClkEn:1;
        uint32_t reserved16:7;
        uint32_t reserved17:1;
        uint32_t bleSwRst:1;
        uint32_t wktm0SwRst:1;
        uint32_t wktm1SwRst:1;
        uint32_t gpio0SwRst:1;
        uint32_t wdtSwRst:1;
        uint32_t pwm0SwRst:1;
        uint32_t pwm1SwRst:1;
        uint32_t i2c0SwRst:1;
        uint32_t i2c1SwRst:1;
        uint32_t i2sSwRst:1;
        uint32_t dmicSwRst:1;
        uint32_t uart0SwRst:1;
        uint32_t uart1SwRst:1;
        uint32_t uart2SwRst:1;
        uint32_t spim0SwRst:1;
        uint32_t spim1SwRst:1;
        uint32_t spim2SwRst:1;
        uint32_t ccuSwRst:1;
        uint32_t rtcSwRst:1;
        uint32_t sys0SwRst:1;
        uint32_t aesSwRst:1;
        uint32_t sys1SwRst:1;
        uint32_t bleTimingGenSwRst:4;
        uint32_t dmuSwRst:1;
        uint32_t reserved18:7;
        uint32_t clkSel:6;
        uint32_t reserved19:26;
    }bf;    //bit-field
}U_regCKGEN;

typedef union U_regPWMWKTM
{
    struct
    {
        uint32_t interrupt;
        uint32_t highCounter;
        uint32_t lowCounter;
        uint32_t ctrl;
        uint32_t prescaler;
        uint32_t counter;
    }dw;    //double word

    struct
    {
        uint32_t intEn:1;           //0:disable, 1:enable
        uint32_t reserved1:15;
        uint32_t intSts:1;          //write 1 clear
        uint32_t reserved0:15;

        uint32_t highCounter;

        uint32_t lowCounter;

        uint32_t reserved4:8;
        uint32_t pwmOutEn:1;        //0:disable, 1:enable
        uint32_t repeat:1;          //0:repeat, 1:one-shot
        uint32_t enable:1;          //0:disable, 1:enable
        uint32_t clear:1;           //0:un-clear, 1:clear;
        uint32_t reserved3:4;
        uint32_t pwmTimerSel:1;     //0:PWM, 1:Timer
        uint32_t reserved2:15;
			

        uint32_t prescaler:8;       //0,1:no pre-scaler, 2~255: pre-scaler
        uint32_t reserved5:24;

        uint32_t counter;           //read counter value
    }bf;    //bit-field
}U_regPWMWKTM;

typedef union U_regRTC
{
    struct
    {
        uint32_t sec;
        uint32_t min;
        uint32_t hour;
        uint32_t weekDay;
        uint32_t day;
        uint32_t month;
        uint32_t year;
        uint32_t alarm1Sec;
        uint32_t alarm1Min;
        uint32_t alarm1Hour;
        uint32_t alarm1Day;
        uint32_t alarm1Month;
        uint32_t alarm2Sec;
        uint32_t alarm2Min;
        uint32_t alarm2Hour;
        uint32_t alarm2Day;
        uint32_t alarm2Month;
        uint32_t timing;
        uint32_t calib;
        uint32_t interrupt;
    }dw;    //double word

    struct
    {
        uint32_t reserved0:25;
        uint32_t sec:7;

        uint32_t reserved1:25;
        uint32_t min:7;

        uint32_t reserved2:24;
        uint32_t pm:1;
        uint32_t reserved3:1;
        uint32_t hour:6;

        uint32_t reserved4:29;
        uint32_t weekDay:3;

        uint32_t reserved5:26;
        uint32_t day:6;

        uint32_t reserved6:25;
        uint32_t month:7;

        uint32_t reserved7:23;
        uint32_t century:1;
        uint32_t year:8;

        uint32_t reserved8:25;
        uint32_t alarm1Sec:7;

        uint32_t reserved9:25;
        uint32_t alarm1Min:7;

        uint32_t reserved10:24;
        uint32_t alarm1Pm:1;
        uint32_t reserved11:1;
        uint32_t alarm1Hour:6;

        uint32_t reserved12:24;
        uint32_t alarm1Day:8;

        uint32_t reserved13:25;
        uint32_t alarm1Month:7;

        uint32_t reserved14:25;
        uint32_t alarm2Sec:7;

        uint32_t reserved15:25;
        uint32_t alarm2Min:7;

        uint32_t reserved16:24;
        uint32_t alarm2Pm:1;
        uint32_t reserved17:1;
        uint32_t alarm2Hour:6;

        uint32_t reserved18:24;
        uint32_t alarm2Day:8;

        uint32_t reserved19:25;
        uint32_t alarm2Month:7;

        uint32_t reserved20:3;
        uint32_t dataMode:1;         //0:binary, 1:BCD
        uint32_t hourFormat:1;       //0:12-hour, 1:24-hour
        uint32_t dse:1;              //day time saving enable
        uint32_t dv:2;               //0: test mode, 1: calibration mode, 2: normal mode, 3: rtc disable
        uint32_t reserved21:2;
        uint32_t cabSign:1;          //0:positive sign, 1:negative sign
        uint32_t cab:21;             //256Hz *

        uint32_t reserved22:21;
        uint32_t fcab:11;

        uint32_t interrupt:1;
        uint32_t uip:1;
        uint32_t reserved23:11;
        uint32_t alarm2Int:1;
        uint32_t alarm1Int:1;
        uint32_t updateInt:1;
        uint32_t reserved24:13;
        uint32_t alarm2IntEn:1;
        uint32_t alarm1IntEn:1;
        uint32_t updateIntEn:1;
    }bf;    //bit-field
}U_regRTC;


typedef union U_regGPIO
{
    struct
    {
        uint32_t input;
        uint32_t output;
        uint32_t outputEn;
        uint32_t intPolarity;       //0:high, 1:low
        uint32_t intEn;             //0:disable, 1:enable
        uint32_t intSts;
        uint32_t intTrig;
        uint32_t pinmux;
        uint32_t intType;           //0:level change, 1:single edge
        uint32_t puEn;              //0:disable, 1:enable
        uint32_t portModeSel;
    }dw;    //double word

    struct
    {
        uint32_t input;
        uint32_t output;
        uint32_t outputEn;
        uint32_t intAct;
        uint32_t intEn;
        uint32_t intSts;
        uint32_t intTrig;
        uint32_t pinmux;
        uint32_t intType;
        uint32_t puEn;
        uint32_t pmSel7:4;
        uint32_t pmSel6:4;
        uint32_t pmSel5:4;
        uint32_t pmSel4:4;
        uint32_t pmSel3:4;
        uint32_t pmSel2:4;
        uint32_t pmSel1:4;
        uint32_t pmSel0:4;
    }bf;    //bit-field
}U_regGPIO;

typedef union U_regUARTDMA
{
    struct
    {
        uint32_t interrupt;
        uint32_t dmaByteNum;
        uint32_t dmaRxStart;
        uint32_t dmaRxEnd;
        uint32_t dmaRxEn;
        uint32_t dmaTxStart;
        uint32_t dmaTxEnd;
        uint32_t dmaTxEn;
        uint32_t dmaDbusRx;
        uint32_t dmaDbusTx;
        uint32_t dmaRxByteCnt;
    }dw;    //double word

    struct
    {
        //UART interrupt:0x00
        uint32_t  rx_dma_done_intr_en:1;
        uint32_t  tx_dma_done_intr_en:1;
        uint32_t  reserved0:14;
        uint32_t  tx_dma_done_intr:1;
        uint32_t  rx_dma_done_intr:1;
        uint32_t  reserved1:14;
        //UART DMA bytenum control:0x04
        uint32_t  dma_rxbyte_num:8;
        uint32_t  dma_txbyte_num:8;
        uint32_t  reserved2:16;
        //UART DMA read start:0x08
        uint32_t  dma_rxstrart_addr:17;
        uint32_t  reserved3:15;
        //UART DMA read end:0x0C
        uint32_t  dma_rxend_addr:17;
        uint32_t  reserved4:15;
        //UART DMA read enable:0x10
        uint32_t  dma_rxen:1;
        uint32_t  reserved5:7;
        uint32_t  rx_flush:1;
        uint32_t  reserved6:7;
        uint32_t  rx_b_endian:1;
        uint32_t  reserved7:15;
        //UART DMA write start:0x14
        uint32_t  dma_txstrart_addr:17;
        uint32_t  reserved8:15;
        //UART DMA write end:0x18
        uint32_t  dma_txend_addr:17;
        uint32_t  reserved9:15;
        //UART DMA write enable:0x1C
        uint32_t  dma_txen:1;
        uint32_t  reserved10:7;
        uint32_t  tx_flush:1;
        uint32_t  reserved11:7;
        uint32_t  tx_b_endian:1;
        uint32_t  reserved12:15;
        //UART DMA DBUS rx:0x20
        uint32_t  dbus_rx_addr:17;
        uint32_t  reserved13:15;
        //UART DMA DBUS tx:0x24
        uint32_t  dbus_tx_addr:17;
        uint32_t  reserved14:15;
        //UART DMA rx aggregated byteword:0x28
        uint32_t  rx_pbcnt:2;
        uint32_t  reserved15:6;
        uint32_t  rx_data:24;
    }bf;    //bit-field
}U_regUARTDMA;

typedef union U_regUARTCTRL
{
    struct
    {
        uint32_t bufTx;
        uint32_t bufRx;
        uint32_t ictrl;
        uint32_t stat;
        uint32_t frs;
        uint32_t mdsl;
        uint32_t baud;
        uint32_t psr;
        uint32_t ovr;
        uint32_t mdsl2;
        uint32_t spos;
        uint32_t wpsrh;
        uint32_t wpsrl;
    }dw;    //double word

    struct
    {
        //UART UnTBUF:0x80
        uint32_t   untbuf:8;
        uint32_t   reserved0:24;
        //UART UnRBUF:0x84
        uint32_t   unrbuf:8;
        uint32_t   reserved1:24;
        //UART UnCTRL:0x88
        uint32_t   untbe:1;
        uint32_t   unrbf:1;
        uint32_t   undcts:1;
        uint32_t   uncts:1;
        uint32_t   unefci:1;
        uint32_t   uneti:1;
        uint32_t   uneri:1;
        uint32_t   uneei:1;
        uint32_t   reserved2:24;
        //UART UnSTAT:0x8C
        uint32_t   unpe:1;
        uint32_t   unfe:1;
        uint32_t   undoe:1;
        uint32_t   unerr:1;
        uint32_t   unbkd:1;
        uint32_t   unrb9:1;
        uint32_t   unxmip:1;
        uint32_t   reserved3:1;
        uint32_t   reserved4:24;
        //UART UnFRS:0x90
        uint32_t   unchar:2;
        uint32_t   unstp:1;
        uint32_t   unxb9:1;
        uint32_t   unpsel:2;
        uint32_t   unpen:1;
        uint32_t   reserved5:1;
        uint32_t   reserved6:24;
        //UART UnMDSL:0x94
        uint32_t   unmod:1;
        uint32_t   unatn:1;
        uint32_t   unbrk:1;
        uint32_t   uncks:1;
        uint32_t   unetd:1;
        uint32_t   unerd:1;
        uint32_t   unfce:1;
        uint32_t   unrts:1;
        uint32_t   reserved7:24;
        //UART UnBAUD:0x98
        uint32_t   undiv_lower:8;
        uint32_t   reserved8:24;
        //UART UnPSR:0x9C
        uint32_t   undiv_upper:4;
        uint32_t   unpsc:4;
        uint32_t   reserved9:24;
        //UART UnOVR:0xA0
        uint32_t   unovr:4;
        uint32_t   reserved10:4;
        uint32_t   reserved11:24;
        //UART UnMDSL2:0xA4
        uint32_t   unsmd:1;
        uint32_t   reserved12:7;
        uint32_t   reserved13:24;
        //UART UnSPOS:0xA8
        uint32_t   unsamp:4;
        uint32_t   reserved14:4;
        uint32_t   reserved15:24;
        //UART UnWPSRH:0xAC
        uint32_t   unwpsrh:8;
        uint32_t   reserved16:24;
        //UART UnWPSRL:0xB0
        uint32_t   unwpsrl:8;
        uint32_t   reserved17:24;
    }bf;    //bit-field
}U_regUARTCTRL;

typedef struct
{
    //I2C interrupt:0x1100
    uint32_t  padding1:12;
    uint32_t  ms_resync_done_intr:1;
    uint32_t  i2c_cd_err_intr:1;
    uint32_t  i2c_err_ack_intr:1;
    uint32_t  dma_done_intr:1;
    uint32_t  padding2:12;
    uint32_t  ms_resync_intr_en:1;
    uint32_t  i2c_cd_err_intr_en:1;
    uint32_t  i2c_err_ack_intr_en:1;
    uint32_t  dma_done_intr_en:1;
    //I2C DMA control:0x1108
    uint32_t  dma_enable:1;
    uint32_t  padding3:3;
    uint32_t  rdata_byte_num:12;
    uint32_t  dbus_burst:1;
    uint32_t  padding4:3;
    uint32_t  wdata_byte_num:12;
    //I2C DMA write address:0x110C
    uint32_t  padding5:15;
    uint32_t  dma_str_waddr:17;
    //I2C DMA read address:0x1110
    uint32_t  padding6:15;
    uint32_t  dma_str_raddr:17;
    //I2C core configuration:0x1114
    uint32_t  padding7:4;
    uint32_t  ms_prescaler:12;
    uint32_t  padding8:7;
    uint32_t  ms_resync_bus:1;
    uint32_t  cfg_i2c_mask:6;
    uint32_t  cfg_core_select:2;
    //I2C core ms configuration:0x1118
    uint32_t  ms_word_addr:16;
    uint32_t  padding9:1;
    uint32_t  ms_slave_addr:7;
    uint32_t  padding10:5;
    uint32_t  ms_no_stop:1;
    uint32_t  ms_addr_en:1;
    uint32_t  ms_addr_16bit:1;
}S_regI2C;

typedef union U_regSPI
{
    struct
    {
        uint32_t spiInt;
        uint32_t spiCtrl;
        uint32_t DmaCtrl;
        uint32_t wAddr;
        uint32_t rAddr;
    }dw;    //double word

    struct
    {
        //SPI interrupt:0x00
        uint32_t  event_int_en:1;
        uint32_t  error_int_en:1;
        uint32_t  reserved0:14;
        uint32_t  event_int_status:1;
        uint32_t  error_int_status:1;
        uint32_t  reserved1:14;
        //SPI control:0x04
        uint32_t  cpha:1;
        uint32_t  cpol:1;
        uint32_t  reserved2:6;
        uint32_t  cs:2;
        uint32_t  cs_polarity:1;
        uint32_t  reserved3:13;
        uint32_t  spi_m_en:1;
        uint32_t  reserved4:7;
        //SPI DMA control:0x08
        uint32_t  total_wbyte:8;
        uint32_t  total_rbyte:8;
        uint32_t  op_mode:2;
        uint32_t  wbyte_swap:1;
        uint32_t  rbyte_swap:1;
        uint32_t  reserved5:4;
        uint32_t  spi_m_dma_en:1;
        uint32_t  reserved6:7;
        //DMA start write address:0x0C
        uint32_t  dma_str_waddr:17;
        uint32_t  reserved7:15;
        //DMA start read address:0x10
        uint32_t  dma_str_raddr:17;
        uint32_t  reserved8:15;
    }bf;    //bit-field
}U_regSPI;

typedef struct
{
    //remap control
    uint32_t  padding1:31;
    uint32_t  Rmap_en:1;
    //remap start address
    uint32_t  padding2:18;
    uint32_t  Rmap_st_addr:14;
    //remap end address
    uint32_t  padding3:18;
    uint32_t  Rmap_end_addr:14;
    //DMA write address
    uint32_t  padding4:15;
    uint32_t  Dma_str_waddr:17;
    //DMA read address
    uint32_t  padding5:15;
    uint32_t  Dma_str_raddr:17;
}S_regBLE;




#define regSCU          ((U_regSCU         *) SCU_ADDR_BASE)
#define regCKGEN        ((U_regCKGEN       *) CKGEN_ADDR_BASE)
#define regBLE          ((S_regBLE         *) BLE_ADDR_BASE)
#define regPWM0         ((U_regPWMWKTM     *) PWM0_ADDR_BASE)
#define regPWM1         ((U_regPWMWKTM     *) PWM1_ADDR_BASE)
#define regWKTM0        ((U_regPWMWKTM     *) WKTM0_ADDR_BASE)
#define regWKTM1        ((U_regPWMWKTM     *) WKTM1_ADDR_BASE)
#define regGPIO0        ((U_regGPIO        *) GPIO_ADDR_BASE)
#define regGPIO1        ((U_regGPIO        *) (GPIO_ADDR_BASE + 0x0000002c))
#define regUART0DMA     ((U_regUARTDMA     *) UART0_ADDR_BASE)
#define regUART0CTRL    ((U_regUARTCTRL    *) (UART0_ADDR_BASE + 0x00000080))
#define regSPI0         ((U_regSPI         *) SPI0_ADDR_BASE)
#define regSPI1         ((U_regSPI         *) SPI1_ADDR_BASE)
#define regSPI2         ((U_regSPI         *) SPI2_ADDR_BASE)
#define regRTC          ((U_regRTC         *) RTC_ADDR_BASE)


#ifdef __cplusplus
}
#endif

#endif  // _CC6801_REG_H_

