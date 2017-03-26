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

#ifndef CC6801_H
#define CC6801_H

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

//// ------------------------------------
//// define
//// ------------------------------------
//// global
#define CHIP_CLK32K                 topsys.XI_32K
#define CHIP_CLK48M                 topsys.XI_48M
#define CHIP_RSTN                   topsys.RESETN
#define TOPSYS                      topsys
#define TESTBED                     testbed
#define CHIP_TOP                    topsys.chip
#define CHIP_CORE1                  topsys.chip.core_domain1
#define CHIP_CORE0                  topsys.chip.core_domain0
#define CHIP_OTP_AES                topsys.chip.core_domain1.otp_aes_wrap
#define CHIP_IC_EF                  topsys.chip.core_domain1.ic_ef_wrap
#define CHIP_CORE_AO                topsys.chip.core_domain0.core_ao
#define CHIP_DMIC                   `CHIP_CORE0.dmic_top_wrap
#define CHIP_CPU_BFM                topsys.cpu_bfm.m_ahb_bfm

//#define TB_INIT_DONE                `TESTBED.init_done
#define CHIP_PLLCLK                 `CHIP_CORE1.PLL_CC6801.CO0
#define CHIP_SYSCLK                 `CHIP_CORE0.sys_genclk
#define CHIP_SYSRSTN                `CHIP_CORE0.sys_rstn


// mem
#define ROM_ADDR_BASE       0x00000000
#define ROM_SIZE            0x00040000  // 128KB
#define EF_BASE             0x10000000
#define EF_SIZE             0x10040000  // 256KB
#define SYSRAM_ADDR_BASE    0x20000000
#define SYSRAM_SIZE         0x00018000  // 96KB
#define DATARAM_ADDR_BASE   0x20020000
#define DATARAM_SIZE        0x00002000  // 8KB

// mem read write test
#define SYSRAM_TEST_ADDR_BASE   0x20010000

// io
#define REG_SIZE            0x00000100  // 256B
#define BLE_REG_SIZE        0x00020000  // 8k use 16k
#define SCU_ADDR_BASE       0x40000000
#define CKGEN_ADDR_BASE     0x40000100
#define GPIO_ADDR_BASE      0x40000200
#define WDT_ADDR_BASE       0x40000300
#define PWM0_ADDR_BASE      0x40000400
#define PWM1_ADDR_BASE      0x40000500
#define WKTM0_ADDR_BASE     0x40000600
#define WKTM1_ADDR_BASE     0x40000700

#define RTC_ADDR_BASE       0x40000800
#define I2S_ADDR_BASE       0x40000900
#define DMIC_ADDR_BASE      0x40000a00
#define SPI0_ADDR_BASE      0x40000b00
#define SPI1_ADDR_BASE      0x40000c00
#define SPI2_ADDR_BASE      0x40000d00
#define UART0_ADDR_BASE     0x40000e00
#define UART1_ADDR_BASE     0x40000f00

#define UART2_ADDR_BASE     0x40001000
#define I2C0_ADDR_BASE      0x40001100
#define I2C1_ADDR_BASE      0x40001200
#define DMU_ADDR_BASE       0x40001300

#define EF_ADDR_BASE        0x40002000
#define OTP_ADDR_BASE       0x40002100
#define HS_ADDR_BASE        0x40002200
#define PMU_ADDR_BASE       0x40002300
#define AES_ADDR_BASE       0x40002400
#define CCU_ADDR_BASE       0x40002500

#define BLE_ADDR_BASE       0x40004000

// EF
#define EF_INTERRUPT_REG        (EF_ADDR_BASE + 0x00000000)
#define EF_CONFIG_REG           (EF_ADDR_BASE + 0x00000004)
#define EF_ACCESS_REG           (EF_ADDR_BASE + 0x00000008)
#define EF_WR_DATA0_REG         (EF_ADDR_BASE + 0x0000000c)
#define EF_WR_DATA1_REG         (EF_ADDR_BASE + 0x00000010)
#define EF_WR_DATA2_REG         (EF_ADDR_BASE + 0x00000014)
#define EF_WR_DATA3_REG         (EF_ADDR_BASE + 0x00000018)
#define EF_FLASHMODE_REG        (EF_ADDR_BASE + 0x0000001c)
#define EF_RD_DATA0_REG         (EF_ADDR_BASE + 0x00000020)
#define EF_RD_DATA1_REG         (EF_ADDR_BASE + 0x00000024)
#define EF_REDUN_DATA_REG       (EF_ADDR_BASE + 0x00000028)

#define EF_TIMING0_REG          (EF_ADDR_BASE + 0x00000044)
#define EF_TIMING1_REG          (EF_ADDR_BASE + 0x00000048)
#define EF_TIMING2_REG          (EF_ADDR_BASE + 0x0000004c)
#define EF_TIMING3_REG          (EF_ADDR_BASE + 0x00000050)
#define EF_TIMING4_REG          (EF_ADDR_BASE + 0x00000054)
#define EF_TIMING5_REG          (EF_ADDR_BASE + 0x00000058)
#define EF_TIMING6_REG          (EF_ADDR_BASE + 0x0000005c)

#define EF_DMA_CTRL_REG         (EF_ADDR_BASE + 0x00000060)
#define EF_DMA_WADDR_REG        (EF_ADDR_BASE + 0x00000064)
#define EF_DMA_RADDR_REG        (EF_ADDR_BASE + 0x00000068)

// SCU
#define SCU_INTRCTRL_REG        (SCU_ADDR_BASE + 0x00000000)
#define SCU_PLLLOCK_REG         (SCU_ADDR_BASE + 0x00000004)
#define SCU_PLLRST_REG          (SCU_ADDR_BASE + 0x00000008)
#define SCU_PLLCFG_REG          (SCU_ADDR_BASE + 0x0000000c)
#define SCU_CLKCFG0_REG         (SCU_ADDR_BASE + 0x00000010)
#define SCU_CLKCFG1_REG         (SCU_ADDR_BASE + 0x00000014)
#define SCU_CHIPID_REG          (SCU_ADDR_BASE + 0x0000001c)
#define SCU_ISOCTRL_REG         (SCU_ADDR_BASE + 0x00000020)
#define SCU_PSOCTRL_REG         (SCU_ADDR_BASE + 0x00000024)
#define SCU_RETCTRL_REG         (SCU_ADDR_BASE + 0x00000028)
#define SCU_EXTAUX_REG          (SCU_ADDR_BASE + 0x0000002c)
#define SCU_HSCLK_REG           (SCU_ADDR_BASE + 0x00000030)
#define SCU_ICACHE_REG          (SCU_ADDR_BASE + 0x00000034)
#define SCU_CLK32K_CAL_REG      (SCU_ADDR_BASE + 0x00000038)
#define SCU_INTRMASK_REG        (SCU_ADDR_BASE + 0x0000003c)
#define SCU_INTRINFO_REG        (SCU_ADDR_BASE + 0x00000040)

// CKGEN
#define CKGEN_CFG1_REG          (CKGEN_ADDR_BASE + 0x00000004)
#define CKGEN_CFG2_REG          (CKGEN_ADDR_BASE + 0x00000008)
#define CKGEN_CFG3_REG          (CKGEN_ADDR_BASE + 0x0000000c)
#define CKGEN_CFG4_REG          (CKGEN_ADDR_BASE + 0x00000010)
#define CKGEN_CLKEN_REG         (CKGEN_ADDR_BASE + 0x00000014)
#define CKGEN_SWRST_REG         (CKGEN_ADDR_BASE + 0x00000018)
#define CKGEN_BLECLKSEL_REG     (CKGEN_ADDR_BASE + 0x0000001c)

// GPIO
#define GPIO_PAD_OUT_REG        (GPIO_ADDR_BASE + 0x00000000)
#define GPIO_PAD_IN_REG         (GPIO_ADDR_BASE + 0x00000004)
#define GPIO_PAD_OE_REG         (GPIO_ADDR_BASE + 0x00000008)
#define GPIO_INTR_PRIO_REG      (GPIO_ADDR_BASE + 0x0000000c)
#define GPIO_INTR_EN_REG        (GPIO_ADDR_BASE + 0x00000010)
#define GPIO_INTR_STAT_REG      (GPIO_ADDR_BASE + 0x00000014)
#define GPIO_INTR_TRIG_REG      (GPIO_ADDR_BASE + 0x00000018)
#define GPIO_AUX_REG            (GPIO_ADDR_BASE + 0x0000001c)
#define GPIO_INTR_TYPE_REG      (GPIO_ADDR_BASE + 0x00000020)
#define GPIO_PAD_PULLUP_REG     (GPIO_ADDR_BASE + 0x00000024)
#define GPIO_AUX_PORT_MODE_REG  (GPIO_ADDR_BASE + 0x00000028)

#define GPIO1_PAD_OUT_REG       (GPIO_ADDR_BASE + 0x0000002c)
#define GPIO1_PAD_IN_REG        (GPIO_ADDR_BASE + 0x00000030)
#define GPIO1_PAD_OE_REG        (GPIO_ADDR_BASE + 0x00000034)
#define GPIO1_INTR_PRIO_REG     (GPIO_ADDR_BASE + 0x00000038)
#define GPIO1_INTR_EN_REG       (GPIO_ADDR_BASE + 0x0000003c)
#define GPIO1_INTR_STAT_REG     (GPIO_ADDR_BASE + 0x00000040)
#define GPIO1_INTR_TRIG_REG     (GPIO_ADDR_BASE + 0x00000044)
#define GPIO1_AUX_REG           (GPIO_ADDR_BASE + 0x00000048)
#define GPIO1_INTR_TYPE_REG     (GPIO_ADDR_BASE + 0x0000004c)
#define GPIO1_PAD_PULLUP_REG    (GPIO_ADDR_BASE + 0x00000050)
#define GPIO1_AUX_PORT_MODE_REG (GPIO_ADDR_BASE + 0x00000054)

// SPI
#define SPI0_INTR_REG           (SPI0_ADDR_BASE + 0x00000000)
#define SPI0_CTRL_REG           (SPI0_ADDR_BASE + 0x00000004)
#define SPI0_DMA_CTRL_REG       (SPI0_ADDR_BASE + 0x00000008)
#define SPI0_DMA_WR_REG         (SPI0_ADDR_BASE + 0x0000000c)
#define SPI0_DMA_RD_REG         (SPI0_ADDR_BASE + 0x00000010)

#define SPI1_INTR_REG           (SPI1_ADDR_BASE + 0x00000000)
#define SPI1_CTRL_REG           (SPI1_ADDR_BASE + 0x00000004)
#define SPI1_DMA_CTRL_REG       (SPI1_ADDR_BASE + 0x00000008)
#define SPI1_DMA_WR_REG         (SPI1_ADDR_BASE + 0x0000000c)
#define SPI1_DMA_RD_REG         (SPI1_ADDR_BASE + 0x00000010)

#define SPI1_INTR_REG           (SPI1_ADDR_BASE + 0x00000000)
#define SPI1_CTRL_REG           (SPI1_ADDR_BASE + 0x00000004)
#define SPI1_DMA_CTRL_REG       (SPI1_ADDR_BASE + 0x00000008)
#define SPI1_DMA_WR_REG         (SPI1_ADDR_BASE + 0x0000000c)
#define SPI1_DMA_RD_REG         (SPI1_ADDR_BASE + 0x00000010)

// WDT
#define WDT_TIMER0_REG          (WDT_ADDR_BASE + 0x00000000)
#define WDT_TIMER1_REG          (WDT_ADDR_BASE + 0x00000004)
#define WDT_TIMER2_REG          (WDT_ADDR_BASE + 0x00000008)
#define WDT_TIMER3_REG          (WDT_ADDR_BASE + 0x0000000c)

// RTC
#define RTC_SEC_REG             (RTC_ADDR_BASE + 0x00000000)
#define RTC_MIN_REG             (RTC_ADDR_BASE + 0x00000004)
#define RTC_HRS_REG             (RTC_ADDR_BASE + 0x00000008)
#define RTC_DOW_REG             (RTC_ADDR_BASE + 0x0000000c)
#define RTC_DAY_REG             (RTC_ADDR_BASE + 0x00000010)
#define RTC_MON_REG             (RTC_ADDR_BASE + 0x00000014)
#define RTC_YRS_REG             (RTC_ADDR_BASE + 0x00000018)

#define RTC_ALM_SEC_REG         (RTC_ADDR_BASE + 0x0000001c)
#define RTC_ALM_MIN_REG         (RTC_ADDR_BASE + 0x00000020)
#define RTC_ALM_HRS_REG         (RTC_ADDR_BASE + 0x00000024)
#define RTC_ALM_DOM_REG         (RTC_ADDR_BASE + 0x00000028)
#define RTC_ALM_MON_REG         (RTC_ADDR_BASE + 0x0000002c)

#define RTC_ALM2_SEC_REG        (RTC_ADDR_BASE + 0x00000030)
#define RTC_ALM2_MIN_REG        (RTC_ADDR_BASE + 0x00000034)
#define RTC_ALM2_HRS_REG        (RTC_ADDR_BASE + 0x00000038)
#define RTC_ALM2_DOM_REG        (RTC_ADDR_BASE + 0x0000003c)
#define RTC_ALM2_MON_REG        (RTC_ADDR_BASE + 0x00000040)

#define RTC_CTRLA_REG           (RTC_ADDR_BASE + 0x00000044)
#define RTC_CTRLB_REG           (RTC_ADDR_BASE + 0x00000048)
#define RTC_CTRLC_REG           (RTC_ADDR_BASE + 0x00000050)

// CCU
#define CCU_INTR_REG            (CCU_ADDR_BASE + 0x00000000)
#define CCU_DELTA_CNT_REG       (CCU_ADDR_BASE + 0x00000004)
#define CCU_FDELTA_CNT_REG      (CCU_ADDR_BASE + 0x00000008)

// UART
#define UART0_INTR_REG             (UART0_ADDR_BASE + 0x00000000)
#define UART0_DMA_CTRL_REG         (UART0_ADDR_BASE + 0x00000004)
#define UART0_DMA_RADDR_START_REG  (UART0_ADDR_BASE + 0x00000008)
#define UART0_DMA_RADDR_END_REG    (UART0_ADDR_BASE + 0x0000000C)
#define UART0_DMA_RDEN_REG         (UART0_ADDR_BASE + 0x00000010)
#define UART0_DMA_WADDR_START_REG  (UART0_ADDR_BASE + 0x00000014)
#define UART0_DMA_WADDR_END_REG    (UART0_ADDR_BASE + 0x00000018)
#define UART0_DMA_WR_EN_REG        (UART0_ADDR_BASE + 0x0000001C)
#define UART0_DMA_DBUSADDR_RX_REG  (UART0_ADDR_BASE + 0x00000020)
#define UART0_DMA_DBUSADDR_TX_REG  (UART0_ADDR_BASE + 0x00000024)
#define UART0_DMA_AGR_RX_REG       (UART0_ADDR_BASE + 0x00000028)
#define UART0_UnTBUF_REG           (UART0_ADDR_BASE + 0x00000080)
#define UART0_UnRBUF_REG           (UART0_ADDR_BASE + 0x00000084)
#define UART0_UnICTRL_REG          (UART0_ADDR_BASE + 0x00000088)
#define UART0_UnSTAT_REG           (UART0_ADDR_BASE + 0x0000008C)
#define UART0_UnFRS_REG            (UART0_ADDR_BASE + 0x00000090)
#define UART0_UnMDSL_REG           (UART0_ADDR_BASE + 0x00000094)
#define UART0_UnBAUD_REG           (UART0_ADDR_BASE + 0x00000098)
#define UART0_UnPSR_REG            (UART0_ADDR_BASE + 0x0000009C)
#define UART0_UnOVR_REG            (UART0_ADDR_BASE + 0x000000A0)
#define UART0_UnMDSL2_REG          (UART0_ADDR_BASE + 0x000000A4)
#define UART0_UnSPOS_REG           (UART0_ADDR_BASE + 0x000000A8)
#define UART0_UnWPSRH_REG          (UART0_ADDR_BASE + 0x000000AC)
#define UART0_UnWPSRL_REG          (UART0_ADDR_BASE + 0x000000B0)

#define UART1_UnBAUD_REG        (UART1_ADDR_BASE + 0x00000018)
#define UART1_UnPSR_REG         (UART1_ADDR_BASE + 0x0000001c)
#define UART1_UnOVR_REG         (UART1_ADDR_BASE + 0x00000020)

#define UART2_UnBAUD_REG        (UART2_ADDR_BASE + 0x00000018)
#define UART2_UnPSR_REG         (UART2_ADDR_BASE + 0x0000001c)
#define UART2_UnOVR_REG         (UART2_ADDR_BASE + 0x00000020)

// HS
#define HS_CONFIG_REG           (HS_ADDR_BASE + 0x00000000)
#define HS_STS_REG              (HS_ADDR_BASE + 0x00000004)
#define HS_DR_PDN_REG           (HS_ADDR_BASE + 0x00000008)
#define HS_DR0_PUP_REG          (HS_ADDR_BASE + 0x0000000c)
#define HS_DR0_PUP1_REG         (HS_ADDR_BASE + 0x00000010)
#define HS_DR0_INTR_REG         (HS_ADDR_BASE + 0x00000014)
#define HS_DR1_PUP_REG          (HS_ADDR_BASE + 0x00000018)
#define HS_DR1_PUP1_REG         (HS_ADDR_BASE + 0x0000001c)
#define HS_DR1_INTR_REG         (HS_ADDR_BASE + 0x00000020)
#define HS_DR2_PUP_REG          (HS_ADDR_BASE + 0x00000024)
#define HS_DR2_PUP1_REG         (HS_ADDR_BASE + 0x00000028)
#define HS_DR2_INTR_REG         (HS_ADDR_BASE + 0x0000002c)
#define HS_SCRATCH_BASE_REG     (HS_ADDR_BASE + 0x00000030)

#define AES_INTR_REG            (AES_ADDR_BASE + 0x00000000)
#define AES_CTRL_REG            (AES_ADDR_BASE + 0x00000004)
#define AES_DMA_CTRL_REG        (AES_ADDR_BASE + 0x00000008)
#define AES_DMA_WADDR_REG       (AES_ADDR_BASE + 0x0000000c)
#define AES_DMA_RADDR_REG       (AES_ADDR_BASE + 0x00000010)




typedef struct S_regSCU
{
    //cancelled:0x00
    uint32_t  padding1;
    //ADC parameter:0x04
    uint32_t  padding2:8;
    uint32_t  VBG12:1;
    uint32_t  ADC:7;
    uint32_t  padding3:5;
    uint32_t  BRG_SEL:3;
    uint32_t  padding4:5;
    uint32_t  ADC0TS1:1;
    uint32_t  PD_AD_ONLY:1;
    uint32_t  PD:1;
    //PLL assert/de-assert:0x08
    uint32_t  padding5:31;
    uint32_t  PLL_reset:1;

}S_regSCU;

typedef struct
{
    //UART interrupt:0x00
    uint32_t  padding1:14;
    uint32_t  rx_dma_done_intr:1;
    uint32_t  tx_dma_done_intr:1;
    uint32_t  padding2:14;
    uint32_t  tx_dma_done_intr_en:1;
    uint32_t  rx_dma_done_intr_en:1;
    //UART DMA bytenum control:0x04
    uint32_t  padding3:8;
    uint32_t  dma_txbyte_num:8;
    uint32_t  padding4:8;
    uint32_t  dma_rxbyte_num:8;
    //UART DMA read start:0x08
    uint32_t  padding5:15;
    uint32_t  dma_rxstrart_addr:17;
    //UART DMA read end:0x0C
    uint32_t  padding6:15;
    uint32_t  dma_rxend_addr:17;
    //UART DMA read enable:0x10
    uint32_t  padding7:15;
    uint32_t  rx_b_endian:1;
    uint32_t  padding8:7;
    uint32_t  rx_flush:1;
    uint32_t  padding9:7;
    uint32_t  dma_rxen:1;
    //UART DMA write start:0x14
    uint32_t  padding10:15;
    uint32_t  dma_txstrart_addr:17;
    //UART DMA write end:0x18
    uint32_t  padding11:15;
    uint32_t  dma_txend_addr:17;
    //UART DMA write enable:0x1C
    uint32_t  padding12:15;
    uint32_t  tx_b_endian:1;
    uint32_t  padding13:7;
    uint32_t  tx_flush:1;
    uint32_t  padding14:7;
    uint32_t  dma_txen:1;
    //UART DMA DBUS rx:0x20
    uint32_t  padding15:15;
    uint32_t  dbus_rx_addr:17;
    //UART DMA DBUS tx:0x24
    uint32_t  padding16:15;
    uint32_t  dbus_tx_addr:17;
    //UART DMA rx aggregated byteword:0x28
    uint32_t  rx_data:24;
    uint32_t  padding17:6;
    uint32_t  rx_pbcnt:2;
    //UART UnTBUF:0x80
    uint8_t   untbuf;
    //UART UnRBUF:0x84
    uint8_t   unrbuf;
    //UART UnCTRL:0x88
    uint8_t   uneei:1;
    uint8_t   uneri:1;
    uint8_t   uneti:1;
    uint8_t   unefci:1;
    uint8_t   uncts:1;
    uint8_t   undcts:1;
    uint8_t   unrbf:1;
    uint8_t   untbe:1;
    //UART UnSTAT:0x8C
    uint8_t   padding18:1;
    uint8_t   unxmip:1;
    uint8_t   unrb9:1;
    uint8_t   unbkd:1;
    uint8_t   unerr:1;
    uint8_t   undoe:1;
    uint8_t   unfe:1;
    uint8_t   unpe:1;
    //UART UnFRS:0x90
    uint8_t   padding19:1;
    uint8_t   unpen:1;
    uint8_t   unpsel:2;
    uint8_t   unxb9:1;
    uint8_t   unstp:1;
    uint8_t   unchar:2;
    //UART UnMDSL:0x94
    uint8_t   unrts:1;
    uint8_t   unfce:1;
    uint8_t   unerd:1;
    uint8_t   unetd:1;
    uint8_t   uncks:1;
    uint8_t   unbrk:1;
    uint8_t   unatn:1;
    uint8_t   unmod:1;
    //UART UnBAUD:0x98
    uint8_t   undiv_lower:8;
    //UART UnPSR:0x9C
    uint8_t   unpsc:4;
    uint8_t   undiv_upper:4;
    //UART UnOVR:0xA0
    uint8_t   padding20:4;
    uint8_t   unovr:4;
    //UART UnMDSL2:0xA4
    uint8_t   padding21:7;
    uint8_t   unsmd:1;
    //UART UnSPOS:0xA8
    uint8_t   padding22:4;
    uint8_t   unsamp:4;
    //UART UnWPSRH:0xAC
    uint8_t   unwpsrh:8;
    //UART UnWPSRL:0xB0
    uint8_t   unwpsrl:8;
}S_regUART;

typedef struct
{
    //PWM interrupt
    uint32_t  padding1:31;
    uint32_t  interrupt_status:1;
    uint32_t  padding2:1;
    uint32_t  interrupt_enable:1;
    //PWM high counter load
    uint32_t  pwmhighcounterload:32;
    //PWM low counter load
    uint32_t  pwmlowcounterload:32;
    //PWM control
    uint32_t  padding3:18;
    uint32_t  pwmconfig:14;
    uint32_t  padding4;
    uint32_t  pwmclear;
    uint32_t  pwmenable;
    uint32_t  pwmsingle;
    uint32_t  pwmoutputenable;
    uint32_t  reserved;
    //PWM pre-scaler
    uint32_t  padding5:15;
    uint32_t  pwmprescaler:17;
    //PWM counter
    uint32_t  pwmcounter:32;
}S_regPWM;

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




#define regSCU      ((S_regSCU*) SCU_ADDR_BASE)
#define regBLE      ((S_regBLe*) BLE_ADDR_BASE)





#ifdef __cplusplus
}
#endif

#endif  /* CC6801_H */

