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
#include "ARMCM.h"

#ifdef __cplusplus
extern "C" {
#endif

//macro
#define delayns(x)  do{uint32_t ipp;for(ipp=0;ipp<x;ipp++){volatile uint32_t iqq;iqq=0;}}while(0);
#define wr(x,y)     *((volatile unsigned int *)(x)) = y
#define rd(x,y)     y = *((volatile unsigned int *)(x))
#define setbit(x,y) *((volatile unsigned int *)(x)) |= y
#define clrbit(x,y) *((volatile unsigned int *)(x)) &= (~y)

// Memory
#define ROM_ADDR_BASE               0x00000000
#define ROM_SIZE                    0x00020000  // 128KB
#define EF_BASE                     0x10000000
#define EF_SIZE                     0x10040000  // 256KB
#define SYSRAM_ADDR_BASE            0x20000000
#define SYSRAM_SIZE                 0x0002A000  // 168KB
//#define DATARAM_ADDR_BASE           0x20020000
//#define DATARAM_SIZE                0x00002000  // 8KB

// Register
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
#define SPI3_ADDR_BASE              0x40001500
#define UART0_ADDR_BASE             0x40000e00
#define UART1_ADDR_BASE             0x40000f00
#define UART2_ADDR_BASE             0x40001000
#define I2C0_ADDR_BASE              0x40001100
#define I2C1_ADDR_BASE              0x40001200
#define DMU_ADDR_BASE               0x40001300
#define AOCKGEN_ADDR_BASE           0x40001400
#define EF_ADDR_BASE                0x40002000
#define BLEPHY_ADDR_BASE            0x40002100
#define HS_ADDR_BASE                0x40002200
#define PMU_ADDR_BASE               0x40002300
#define AES_ADDR_BASE               0x40002400
#define CCU_ADDR_BASE               0x40002500
#define VWU_ADDR_BASE               0x40002600
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
#define GPIO_PAD_IN_REG             (GPIO_ADDR_BASE + 0x00000000)
#define GPIO_PAD_OUT_REG            (GPIO_ADDR_BASE + 0x00000004)
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
#define RTC_ALM_DAY_REG             (RTC_ADDR_BASE + 0x00000028)
#define RTC_ALM_MON_REG             (RTC_ADDR_BASE + 0x0000002c)

#define RTC_ALM2_SEC_REG            (RTC_ADDR_BASE + 0x00000030)
#define RTC_ALM2_MIN_REG            (RTC_ADDR_BASE + 0x00000034)
#define RTC_ALM2_HRS_REG            (RTC_ADDR_BASE + 0x00000038)
#define RTC_ALM2_DAY_REG            (RTC_ADDR_BASE + 0x0000003c)
#define RTC_ALM2_MON_REG            (RTC_ADDR_BASE + 0x00000040)

#define RTC_CTRL_REG                (RTC_ADDR_BASE + 0x00000044)
#define RTC_FCAB_REG                (RTC_ADDR_BASE + 0x00000048)
#define RTC_INTR_REG                (RTC_ADDR_BASE + 0x00000050)

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
#define HS_DR0_INTR_REG             (HS_ADDR_BASE + 0x00000014)
#define HS_DR1_PUP_REG              (HS_ADDR_BASE + 0x00000018)
#define HS_DR1_INTR_REG             (HS_ADDR_BASE + 0x00000020)
#define HS_DR2_PUP_REG              (HS_ADDR_BASE + 0x00000024)
#define HS_DR2_INTR_REG             (HS_ADDR_BASE + 0x0000002c)
#define HS_PSO_PDN_REG              (HS_ADDR_BASE + 0x00000030)
#define HS_LDO_PDN_REG              (HS_ADDR_BASE + 0x00000034)

#define AES_INTR_REG                (AES_ADDR_BASE + 0x00000000)
#define AES_CTRL_REG                (AES_ADDR_BASE + 0x00000004)
#define AES_DMA_CTRL_REG            (AES_ADDR_BASE + 0x00000008)
#define AES_DMA_WADDR_REG           (AES_ADDR_BASE + 0x0000000c)
#define AES_DMA_RADDR_REG           (AES_ADDR_BASE + 0x00000010)

//DMIC
#define DMIC_INTR_REG               (DMIC_ADDR_BASE + 0x00000000)
#define DMIC_CTRL_REG               (DMIC_ADDR_BASE + 0x00000004)
#define DMIC_DMA_BYTE_REG           (DMIC_ADDR_BASE + 0x00000008)
#define DMIC_DMA_L_START_REG        (DMIC_ADDR_BASE + 0x0000000C)
#define DMIC_DMA_L_END_REG          (DMIC_ADDR_BASE + 0x00000010)
#define DMIC_DMA_R_START_REG        (DMIC_ADDR_BASE + 0x00000014)
#define DMIC_DMA_R_END_REG          (DMIC_ADDR_BASE + 0x00000018)
#define DMIC_DMA_ENABLE_REG         (DMIC_ADDR_BASE + 0x0000001C)
#define DMIC_DMA_CURRENT_ADDR_REG   (DMIC_ADDR_BASE + 0x00000020)

//I2S
#define I2S_CLK_CTRL_REG            (I2S_ADDR_BASE + 0x00000000)
#define I2S_RX_CTRL_REG             (I2S_ADDR_BASE + 0x00000004)
#define I2S_TX_CTRL_REG             (I2S_ADDR_BASE + 0x00000008)
#define I2S_TX_DATA_LEFT_REG        (I2S_ADDR_BASE + 0x0000000C)
#define I2S_TX_DATA_RIGHT_REG       (I2S_ADDR_BASE + 0x00000010)
#define I2S_RX_DATA_LEFT_REG        (I2S_ADDR_BASE + 0x00000014)
#define I2S_RX_DATA_RIGHT_REG       (I2S_ADDR_BASE + 0x00000018)
#define I2S_STATUS_REG              (I2S_ADDR_BASE + 0x0000001C)
#define I2S_INTR_REG                (I2S_ADDR_BASE + 0x00000020)
#define I2S_DMA_BYTE_REG            (I2S_ADDR_BASE + 0x00000024)
#define I2S_DMA_RX_START_REG        (I2S_ADDR_BASE + 0x00000028)
#define I2S_DMA_RX_END_REG          (I2S_ADDR_BASE + 0x0000002C)
#define I2S_DMA_RX_ENABLE_REG       (I2S_ADDR_BASE + 0x00000030)
#define I2S_DMA_TX_START_REG        (I2S_ADDR_BASE + 0x00000034)
#define I2S_DMA_TX_END_REG          (I2S_ADDR_BASE + 0x00000038)
#define I2S_DMA_TX_ENABLE_REG       (I2S_ADDR_BASE + 0x0000003C)
#define I2S_DMA_RX_CURRENT_ADDR_REG (I2S_ADDR_BASE + 0x00000040)


/******************************/
/* cc6801 register definition */
/******************************/
typedef volatile union U_regSCU
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
        uint32_t reserved2;
        uint32_t cacheBootCtrl;
        uint32_t clk32Calib;
        uint32_t chipIntEn;
        uint32_t chipIntSts;
        uint32_t shutOffAckSts;
        uint32_t productKey;
    }dw;    //double word

    struct
    {
        //0x00
        uint32_t reserved0;
        //0x04 ADC Parameter
        uint32_t pdBgr:1;           //power down for Bang Gap Fule Gauge
        uint32_t pdAdc:1;           //power down for ADC except BGFR
        uint32_t adc_temp:1;        //0:ADC, 1:Temperature sensor
        uint32_t reserved1:5;
        uint32_t bgrSel:3;          //Fine tune for Bang Gap Fule Gauge
        uint32_t reserved2:5;
        uint32_t adc:7;             //ADC digital output
        uint32_t reserved3:9;
        //0x08 PLL Assert/De-assert
        uint32_t pllReset:1;        //0:de-assert, 1:assert reset
        uint32_t reserved4:31;
        //0x0C PLL configuration
        uint32_t divn:8;            //binary + 1, reference divider value
        uint32_t divm:8;            //binary + 1, VCO divider value
        uint32_t divc0:8;           //binary + 1, output divider value
        uint32_t pd:1;              //power down
        uint32_t bypass:1;          //PLL bypass
        uint32_t reserved5:5;
        uint32_t pllLock:1;         //0: PLL unlock, read only
        //0x10 System clock configuration0
        uint32_t sysClockDiv:5;     //System clock divider after source clock selection
        uint32_t clkGenDiv:2;       //ClkGen divider after system clock, 00:bypass, 01:divider by 2, 10:divider by 4, 11:illegal
        uint32_t reserved6:1;
        uint32_t srcClkSel:1;       //Source Clock Selection, 0:XTAL, 1:PLL
        uint32_t sysBypassDiv:1;    //System Bypass Divider, 0:no byass divider, 1:bypass divider
        uint32_t pllEn:1;           //0:disable, 1:enable, self clear after enable
        uint32_t genBypassDiv:1;    //Generated Bypass Divider, 0:no bypass divider, 1:bypass divider
        uint32_t reserved7:20;
        //0x14 System clock configuration1
        uint32_t lowFreq:1;         //Low Freq Clock Selection, 0:32KHz, 1:High Freq from above config0
        uint32_t reserved8:7;
        uint32_t sysGatedClk:1;     //System Gated Clock, 0:disable, 1:enable
        uint32_t hsGatedClk:1;      //Hardware Sequencer Gated Clock, 0:disable, 1:enable
        uint32_t oscSel:1;          //Oscillator selection, 0: oscillator 32KHz, 1:Ring oscillator 40KHz
        uint32_t osc32K:1;          //OSC 32KHz, 0:disable, 1:enable
        uint32_t osc40K:1;          //Ring oscillator 40KHz, 0:disable, 1:enable
        uint32_t dctrlGatedClk:1;   //DCTRL Gated Clock, 0:disable, 1:enable
        uint32_t reserved9:18;
        //0x18 reserved
        uint32_t reserved10:32;
        //0x1C Chip ID
        uint32_t subVer:4;          //Sub Version
        uint32_t mainVer:4;         //Main Version
        uint32_t chipID:24;         //chip ID = 0x80
        //0x20 Isolation Enable
        uint32_t wdt_iso:1;         //0:isolation disable, 1:isolation enable
        uint32_t rtc_iso:1;         //0:isolation disable, 1:isolation enable
        uint32_t pwm0_iso:1;        //0:isolation disable, 1:isolation enable
        uint32_t pwm1_iso:1;        //0:isolation disable, 1:isolation enable
        uint32_t wkt0_iso:1;        //0:isolation disable, 1:isolation enable
        uint32_t wkt1_iso:1;        //0:isolation disable, 1:isolation enable
        uint32_t spi0_iso:1;        //0:isolation disable, 1:isolation enable
        uint32_t spi1_iso:1;        //0:isolation disable, 1:isolation enable
        uint32_t spi2_iso:1;        //0:isolation disable, 1:isolation enable
        uint32_t uart0_iso:1;       //0:isolation disable, 1:isolation enable
        uint32_t uart1_iso:1;       //0:isolation disable, 1:isolation enable
        uint32_t uart2_iso:1;       //0:isolation disable, 1:isolation enable
        uint32_t i2c0_iso:1;        //0:isolation disable, 1:isolation enable
        uint32_t i2c1_iso:1;        //0:isolation disable, 1:isolation enable
        uint32_t i2s_iso:1;         //0:isolation disable, 1:isolation enable
        uint32_t dmic_iso:1;        //0:isolation disable, 1:isolation enable
        uint32_t ble_iso:1;         //0:isolation disable, 1:isolation enable
        uint32_t bleTimingGen_iso:1;//0:isolation disable, 1:isolation enable, BLE Timing Gen isolation
        uint32_t ccu_iso:1;         //0:isolation disable, 1:isolation enable
        uint32_t aes_iso:1;         //0:isolation disable, 1:isolation enable
        uint32_t dRam_iso:1;        //0:isolation disable, 1:isolation enable, D_RAM isolation
        uint32_t m0p_iso:1;         //0:isolation disable, 1:isolation enable
        uint32_t m4_iso:1;          //0:isolation disable, 1:isolation enable
        uint32_t ef_iso:1;          //0:isolation disable, 1:isolation enable, EF/I_Cache isolation
        uint32_t dmu_iso:1;         //0:isolation disable, 1:isolation enable
        uint32_t gpio_iso:1;        //0:isolation disable, 1:isolation enable
        uint32_t clkGen_iso:1;      //0:isolation disable, 1:isolation enable
        uint32_t testLogic_iso:1;   //0:isolation disable, 1:isolation enable
        uint32_t reserved11:1;      //0:isolation disable, 1:isolation enable
        uint32_t blePhy_iso:1;      //0:isolation disable, 1:isolation enable
        uint32_t coreAo_iso:1;      //0:isolation disable, 1:isolation enable
        uint32_t padNao_iso:1;      //0:isolation disable, 1:isolation enable
        //0x24 Power Shut Off Enable
        uint32_t wdt_off:1;         //0:power shut off disable, 1:power shut off enable
        uint32_t rtc_off:1;         //0:power shut off disable, 1:power shut off enable
        uint32_t pwm0_off:1;        //0:power shut off disable, 1:power shut off enable
        uint32_t pwm1_off:1;        //0:power shut off disable, 1:power shut off enable
        uint32_t wkt0_off:1;        //0:power shut off disable, 1:power shut off enable
        uint32_t wkt1_off:1;        //0:power shut off disable, 1:power shut off enable
        uint32_t spi0_off:1;        //0:power shut off disable, 1:power shut off enable
        uint32_t spi1_off:1;        //0:power shut off disable, 1:power shut off enable
        uint32_t spi2_off:1;        //0:power shut off disable, 1:power shut off enable
        uint32_t uart0_off:1;       //0:power shut off disable, 1:power shut off enable
        uint32_t uart1_off:1;       //0:power shut off disable, 1:power shut off enable
        uint32_t uart2_off:1;       //0:power shut off disable, 1:power shut off enable
        uint32_t i2c0_off:1;        //0:power shut off disable, 1:power shut off enable
        uint32_t i2c1_off:1;        //0:power shut off disable, 1:power shut off enable
        uint32_t i2s_off:1;         //0:power shut off disable, 1:power shut off enable
        uint32_t dmic_off:1;        //0:power shut off disable, 1:power shut off enable
        uint32_t ble_off:1;         //0:power shut off disable, 1:power shut off enable
        uint32_t bleTimingGen_off:1;//0:power shut off disable, 1:power shut off enable
        uint32_t ccu_off:1;         //0:power shut off disable, 1:power shut off enable
        uint32_t aes_off:1;         //0:power shut off disable, 1:power shut off enable
        uint32_t dRam_off:1;        //0:power shut off disable, 1:power shut off enable
        uint32_t m0p_off:1;         //0:power shut off disable, 1:power shut off enable
        uint32_t m4_off:1;          //0:power shut off disable, 1:power shut off enable
        uint32_t ef_off:1;          //0:power shut off disable, 1:power shut off enable
        uint32_t dmu_off:1;         //0:power shut off disable, 1:power shut off enable
        uint32_t gpio_off:1;        //0:power shut off disable, 1:power shut off enable
        uint32_t clkGen_off:1;      //0:power shut off disable, 1:power shut off enable
        uint32_t testLogic_off:1;   //0:power shut off disable, 1:power shut off enable
        uint32_t reserved12:1;      //0:power shut off disable, 1:power shut off enable
        uint32_t blePhy_off:1;      //0:power shut off disable, 1:power shut off enable
        uint32_t reserved13:2;
        //0x28 Retention Enable
        uint32_t m0pRet:1;          //0:retention disable, 1:retention enable
        uint32_t m4Ret:1;           //0:retention disable, 1:retention enable
        uint32_t reserved14:30;
        //0x2C External Auxilary Selection
        uint32_t extAuxSel:2;       //External auxilary selection
        uint32_t reserved15:30;
        //0x30 Code Remap
        uint32_t remap:1;           //0:disable, 1:enable, remap memory address 0x20000000 to 0x00
        uint32_t reserved16:31;
        //0x34 I-Cache Control/Booting Control
        uint32_t flushEn:1;         //I-Cache flush enable, 0:disable, 1:enable
        uint32_t reserved17:7;
        uint32_t extFlashBoot:1;    //to indicate booting from external flash, Ready Only
        uint32_t reserved18:1;
        uint32_t pdm25:1;           //PDM25 control(optional), 0:rising edge without impact, 1:falling edge to enable
        uint32_t reserved19:5;
        uint32_t mcuSwReset:1;      //Software reset enable, 0:reset, 1:un-reset
        uint32_t clkRstGenRst:1;    //0:reset, 1:un-reset
        uint32_t reserved20:13;
        uint32_t flushDone:1;       //I-Cache flush done, 0:not complete, 1:complete, Read Only
        //0x38 Clock 32KHz calibration
        uint32_t calib32K:26;       //Clock 32KHz calibration value
        uint32_t reserved21:6;
        //0x3C Chip Interrupt enable
        uint32_t inten;             //apply to CPU non-mask interrupt, 0:disable, 1:enable, refer to 0x40 for bit definition
        //0x40 Chip Interrupt status (all read only)
        uint32_t reserved22:1;
        uint32_t wdt_intSts:1;
        uint32_t rtc_intSts:1;
        uint32_t pwm0_intSts:1;
        uint32_t pwm1_intSts:1;
        uint32_t wktm0_intSts:1;
        uint32_t wktm1_intSts:1;
        uint32_t spi0_intSts:1;
        uint32_t spi1_intSts:1;
        uint32_t spi2_intSts:1;
        uint32_t uart0Rx_intSts:1;
        uint32_t uart0Tx_intSts:1;
        uint32_t uart0Core_intSts:1;
        uint32_t uart1Rx_intSts:1;
        uint32_t uart1Tx_intSts:1;
        uint32_t uart1Core_intSts:1;
        uint32_t uart2Rx_intSts:1;
        uint32_t uart2Tx_intSts:1;
        uint32_t uart2Core_intSts:1;
        uint32_t i2c0_intSts:1;
        uint32_t i2c1_intSts:1;
        uint32_t i2sRx_intSts:1;
        uint32_t i2sTx_intSts:1;
        uint32_t i2sCore_intSts:1;
        uint32_t dmic_intSts:1;
        uint32_t ble_intSts:1;
        uint32_t gpio0_intSts:1;
        uint32_t ccu_intSts:1;
        uint32_t aes_intSts:1;
        uint32_t dmu_intSts:1;
        uint32_t reserved23:1;
        uint32_t ef_intSts:1;
    }bf;    //bit-field
}U_regSCU;

typedef volatile union U_regCKGEN
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
        //0x00
        uint32_t reserved0;
        //0x04
        uint32_t bleClkDiv:5;
        //uint32_t bleLowFreqClkSel:1;
        uint32_t reserved1:3;
        uint32_t wktClkDiv:5;
        uint32_t wktLowFreqClkSel:1;
        uint32_t reserved2:2;
        uint32_t gpioClkDiv:5;
        //uint32_t gpioLowFreqClkSel:1;
        uint32_t reserved3:3;
        uint32_t wdtClkDiv:5;
        uint32_t wdtLowFreqClkSel:1;
        uint32_t reserved4:2;
        //0x08
        uint32_t pwmClkDiv:5;
        uint32_t pwmLowFreqClkSel:1;
        uint32_t reserved5:2;
        uint32_t i2cClkDiv:5;
        //uint32_t i2cLowFreqClkSel:1;
        uint32_t reserved6:3;
        uint32_t i2sClkDiv:5;
        //uint32_t i2sLowFreqClkSel:1;
        uint32_t reserved7:3;
        uint32_t dmicClkDiv:5;
        //uint32_t dmicLowFreqClkSel:1;
        uint32_t reserved8:3;
        //0x0C
        uint32_t uartClkDiv:5;
        //uint32_t uartLowFreqClkSel:1;
        uint32_t reserved9:3;
        uint32_t spi0ClkDiv:5;
        //uint32_t spi0LowFreqClkSel:1;
        uint32_t reserved10:3;
        uint32_t spi1ClkDiv:5;
        //uint32_t spi1LowFreqClkSel:1;
        uint32_t reserved11:3;
        uint32_t spi2ClkDiv:5;
        //uint32_t spi2LowFreqClkSel:1;
        uint32_t reserved12:3;
        //0x10
        uint32_t ccuClkDiv:5;
        //uint32_t LowFreqClkSel:1;
        uint32_t reserved13:27;
        //0x14
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
        //0x18
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
        uint32_t bleTimingGenSwRst:1;
        uint32_t dmuSwRst:1;
        uint32_t reserved18:7;
        //0x1C
        uint32_t clkSel:6;              //8:8MHz, 16:16MHz, 32:32MHz
        uint32_t reserved19:26;
    }bf;    //bit-field
}U_regCKGEN;

typedef volatile union U_regGPIO
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
        //See S_regGPIOnew for new a1 GPIO registers
    }dw;    //double word
}U_regGPIO;

typedef volatile struct
{
    uint32_t pad0InEn;
    uint32_t pad1InEn;
    uint32_t pad0DrvStrength;
    uint32_t pad1DrvStrength;
    uint32_t specialPadInEn;
    uint32_t specialPadDrvStrength;
} S_regGPIOnew;


typedef volatile union U_regWDT
{
    struct
    {
        uint32_t interrupt;
        uint32_t ctrl;
        uint32_t intCounter;
        uint32_t rstCounter;
        uint32_t counter;
    }dw;    //double word

    struct
    {
        uint32_t intEn:1;           //0:disable, 1:enable
        uint32_t reserved0:15;
        uint32_t intSts:1;          //write 1 clear
        uint32_t reserved1:15;

        uint32_t wdtEn:1;           //0:disable, 1:enable
        uint32_t timerSel:1;        //0:disable, 1:enable
        uint32_t rstEn:1;           //0:disable, 1:enable
        uint32_t reserved2:5;
        uint32_t prescaler:8;       //0,1:no pre-scaler, 2~255: pre-scaler
        uint32_t reserved3:16;

        uint32_t intCounter;
        uint32_t rstCounter;
        uint32_t counter;           //read counter value
    }bf;    //bit-field
}U_regWDT;

typedef volatile union U_regPWMWKTM
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

typedef volatile union U_regRTC
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
        uint32_t reserved;
        uint32_t interrupt;
    }dw;    //double word

    struct
    {
        //0x00
        uint32_t sec:7;
        uint32_t reserved0:25;
        //0x04
        uint32_t min:7;
        uint32_t reserved1:25;
        //0x08
        uint32_t hour:6;
        uint32_t reserved3:1;
        uint32_t pm:1;
        uint32_t reserved2:24;
        //0x0C
        uint32_t weekDay:3;         //0:Sunday
        uint32_t reserved4:29;
        //0x10
        uint32_t day:6;
        uint32_t reserved5:26;
        //0x14
        uint32_t month:7;
        uint32_t reserved6:25;
        //0x18
        uint32_t year:8;            //00: year2000
        uint32_t century:1;
        uint32_t reserved7:23;
        //0x1C
        uint32_t alarm1Sec:7;
        uint32_t reserved8:25;
        //0x20
        uint32_t alarm1Min:7;
        uint32_t reserved9:25;
        //0x24
        uint32_t alarm1Hour:6;
        uint32_t reserved11:1;
        uint32_t alarm1Pm:1;
        uint32_t reserved10:24;
        //0x28
        uint32_t alarm1Day:8;
        uint32_t reserved12:24;
        //0x2C
        uint32_t alarm1Month:7;
        uint32_t reserved13:25;
        //0x30
        uint32_t alarm2Sec:7;
        uint32_t reserved14:25;
        //0x34
        uint32_t alarm2Min:7;
        uint32_t reserved15:25;
        //0x38
        uint32_t alarm2Hour:6;
        uint32_t reserved17:1;
        uint32_t alarm2Pm:1;
        uint32_t reserved16:24;
        //0x3C
        uint32_t alarm2Day:8;
        uint32_t reserved18:24;
        //0x40
        uint32_t alarm2Month:7;
        uint32_t reserved19:25;
        //0x44
        uint32_t cab:21;             //256Hz *
        uint32_t cabSign:1;          //0:positive sign, 1:negative sign
        uint32_t reserved21:2;
        uint32_t dv:2;               //0: test mode, 1: calibration mode, 2: normal mode, 3: rtc disable
        uint32_t dse:1;              //day time saving enable
        uint32_t hourFormat:1;       //0:12-hour, 1:24-hour
        uint32_t dataMode:1;         //1:binary, 0:BCD
        uint32_t reserved20:3;
        //0x48
        uint32_t fcab:11;
        uint32_t reserved22:21;
        //0x4C
        uint32_t reserved25;
        //0x50
        uint32_t secIntEn:1;
        uint32_t alarm1IntEn:1;
        uint32_t alarm2IntEn:1;
        uint32_t minIntEn:1;
        uint32_t hourIntEn:1;
        uint32_t dayIntEn:1;
        uint32_t reserved23:10;
        uint32_t secSts:1;          //read clear
        uint32_t alarm1IntSts:1;    //read clear
        uint32_t alarm2IntSts:1;    //read clear
        uint32_t minSts:1;          //read clear
        uint32_t hourSts:1;         //read clear
        uint32_t daySts:1;          //read clear
        uint32_t reserved24:9;
        uint32_t allSts:1;
    }bf;    //bit-field
}U_regRTC;

typedef volatile struct
{
    uint32_t i2sClkCtrl;
    uint32_t i2sRxCtrl;
    uint32_t i2sTxCtrl;
    uint32_t i2sTxDataLeft;
    uint32_t i2sTxDataRight;
    uint32_t i2sRxDataLeft;
    uint32_t i2sRxDataRight;
    uint32_t i2sStatus;
    uint32_t i2sInt;
    uint32_t i2sDmaByteCtrl;
    uint32_t i2sDmaRxStartAddr;
    uint32_t i2sDmaRxEndAddr;
    uint32_t i2sDmaRxEn;
    uint32_t i2sDmaTxStartAddr;
    uint32_t i2sDmaTxEndAddr;
    uint32_t i2sDmaTxEn;
    uint32_t i2sDmaRxCurrAddr;
    uint32_t i2sDmaTxCurrAddr;
}S_regI2S;

typedef volatile union U_regSPI
{
    struct
    {
        uint32_t spiInt;
        uint32_t spiCtrl;
        uint32_t DmaCtrl;
        uint32_t wAddr;
        uint32_t rAddr;
        uint32_t bit9Mode;
        uint32_t dummyCycle;
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

typedef volatile union U_regUARTDMA
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

typedef volatile union U_regUARTCTRL
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
        uint32_t   undiv_upper:3;
        uint32_t   unpsc:5;
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

typedef volatile struct
{
    uint32_t interrupt;
    uint32_t reserved0;
    uint32_t dmaCtrl;
    uint32_t wAddr;
    uint32_t rAddr;
    uint32_t coreCfg;
    uint32_t masterCfg;
}S_regI2C;

typedef volatile struct
{
    uint32_t reserved0;
    uint32_t clkCfg1;
    uint32_t clkCfg2;
    uint32_t reserved1[2];
    uint32_t clkEn;
    uint32_t swReset;
}S_regAOCKGEN;

typedef volatile struct
{
    uint32_t dwInterrupt;
    uint32_t dwCtrl;
    uint32_t dwIndirStart;
    uint32_t dwProgBuf[4];
    uint32_t dwAccessCtrl;
    uint32_t dwRdBuf1;
    uint32_t dwRdBuf2;
    uint32_t dwRedundancy;
    uint32_t dwProtect;
    uint32_t dwPadding[5];
    uint32_t dwTcpsTadhTah;
    uint32_t dwTwkTpgs;
    uint32_t dwTrcvTnvh;
    uint32_t dwTprog;
    uint32_t dwTerase;
    uint32_t dwTme;
    uint32_t dwTnvsTnvh1;
    uint32_t dwDmaCtrl;
    uint32_t dwDmaWrAddr;
    uint32_t dwDmaRdAddr;
}S_regEFLASH;

typedef volatile struct
{
    uint32_t dwConfig;
    uint32_t dwStatus;
    uint32_t dwDrPdn;
    uint32_t dwDr0Pup;
    uint32_t dwDr0Pup1;
    uint32_t dwDr1Pup;
    uint32_t dwDr1Pup1;
    uint32_t dwDr2Pup;
    uint32_t dwDr2Pup1;
    uint32_t dwPdnIsoPso;
    uint32_t dwPdnLdo;
    uint32_t dwPmuLdoTrim;
    uint32_t dwPmuDcdcReg;
    uint32_t dwDr3Pup;
    uint32_t dwRetRamCtrl;
}S_regHS;

typedef volatile union U_regAES
{
    struct
    {
        uint32_t interrupt;
        uint32_t ctrl;
        uint32_t dmaCtrl;
        uint32_t dmaWrAddr;
        uint32_t dmaRdAddr;
    }dw;    //double word

    struct
    {
        uint32_t intEn:1;           //0:disable, 1:enable
        uint32_t reserved0:15;
        uint32_t intSts:1;          //write 1 clear
        uint32_t reserved1:15;

        uint32_t bcMode:5;
        uint32_t reserved2:3;
        uint32_t keySize:2;
        uint32_t reserved3:6;
        uint32_t decrypt:1;
        uint32_t reserved4:15;

        uint32_t  wByteNum:12;
        uint32_t  reserved5:3;
        uint32_t  dbusBurst:1;
        uint32_t  rByteNum:12;
        uint32_t  opMode:2;
        uint32_t  reserved6:1;
        uint32_t  dmaEn:1;

        uint32_t  dmaWrAddr:17;
        uint32_t  reserved7:15;

        uint32_t  dmaRdAddr:17;
        uint32_t  reserved8:15;
    }bf;    //bit-field
}U_regAES;

typedef volatile struct
{
    uint32_t dwInterrupt;
    uint32_t dwDeltaCnt;
    uint32_t dwFineDeltaCnt;
}S_regCCU;

typedef volatile struct
{
    uint32_t none;
}S_regVMU;

typedef volatile struct
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
#define regGPIOnew      ((S_regGPIOnew     *) (GPIO_ADDR_BASE + 0x00000058))
#define regUART0DMA     ((U_regUARTDMA     *) UART0_ADDR_BASE)
#define regUART0CTRL    ((U_regUARTCTRL    *) (UART0_ADDR_BASE + 0x00000080))
#define regUART1DMA     ((U_regUARTDMA     *) UART1_ADDR_BASE)
#define regUART1CTRL    ((U_regUARTCTRL    *) (UART1_ADDR_BASE + 0x00000080))
#define regUART2DMA     ((U_regUARTDMA     *) UART2_ADDR_BASE)
#define regUART2CTRL    ((U_regUARTCTRL    *) (UART2_ADDR_BASE + 0x00000080))
#define regSPI0         ((U_regSPI         *) SPI0_ADDR_BASE)
#define regSPI1         ((U_regSPI         *) SPI1_ADDR_BASE)
#define regSPI2         ((U_regSPI         *) SPI2_ADDR_BASE)
#define regSPI3         ((U_regSPI         *) SPI3_ADDR_BASE)
#define regRTC          ((U_regRTC         *) RTC_ADDR_BASE)
#define regI2C0         ((S_regI2C         *) I2C0_ADDR_BASE)
#define regI2C1         ((S_regI2C         *) I2C1_ADDR_BASE)
#define regAOCKGEN      ((S_regAOCKGEN     *) AOCKGEN_ADDR_BASE)
#define regEFLASH       ((S_regEFLASH      *) EF_ADDR_BASE)
#define regHS           ((S_regHS          *) HS_ADDR_BASE)
#define regAES          ((U_regAES         *) AES_ADDR_BASE)
#define regCCU          ((S_regCCU         *) CCU_ADDR_BASE)
#define regVWU          ((S_regVWU         *) VWU_ADDR_BASE)
#define regWDT          ((U_regWDT         *) WDT_ADDR_BASE)
#define regI2S          ((S_regI2S         *) I2S_ADDR_BASE)

#ifdef __cplusplus
}
#endif

#endif  // _CC6801_REG_H_

