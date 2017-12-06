/* Copyright (c) 2013 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */


#ifndef _LOW_POWER_H
#define _LOW_POWER_H

#include <stdint.h>
#include <stdlib.h>
#include "cc6801_reg.h"
#include "global.h"




#define PERIPHERAL_WDT       0x00000001
#define PERIPHERAL_RTC       0x00000002
#define PERIPHERAL_PWM0      0x00000004
#define PERIPHERAL_PWM1      0x00000008
#define PERIPHERAL_WKT0      0x00000010
#define PERIPHERAL_WKT1      0x00000020
#define PERIPHERAL_SPI0      0x00000040
#define PERIPHERAL_SPI1      0x00000080
#define PERIPHERAL_SPI2      0x00000100
#define PERIPHERAL_UART0     0x00000200
#define PERIPHERAL_UART1     0x00000400
#define PERIPHERAL_UART2     0x00000800
#define PERIPHERAL_I2C0      0x00001000
#define PERIPHERAL_I2C1      0x00002000
#define PERIPHERAL_I2S       0x00004000
#define PERIPHERAL_DMIC      0x00008000
#define PERIPHERAL_BLE       0x00010000
#define PERIPHERAL_CCU       0x00020000
#define PERIPHERAL_AES       0x00040000
#define PERIPHERAL_DMU       0x00080000
#define PERIPHERAL_GPIO      0x00100000
#define PERIPHERAL_TESTLOGIC 0x00200000
#define PERIPHERAL_BLEPHY    0x00400000




//#define SCU_ISOCTRL_REG             (SCU_ADDR_BASE + 0x00000020)
//define each bit for peripheral
#define ISO_PSO_RTC       0x00000002
#define ISO_PSO_PWM0      0x00000004
#define ISO_PSO_PWM1      0x00000008
#define ISO_PSO_WKT0      0x00000010
#define ISO_PSO_WKT1      0x00000020
#define ISO_PSO_SPI0      0x00000040
#define ISO_PSO_SPI1      0x00000080
#define ISO_PSO_SPI2      0x00000100
#define ISO_PSO_UART0     0x00000200
#define ISO_PSO_UART1     0x00000400
#define ISO_PSO_UART2     0x00000800
#define ISO_PSO_I2C0      0x00001000
#define ISO_PSO_I2C1      0x00002000
#define ISO_PSO_I2S       0x00004000
#define ISO_PSO_DMIC      0x00008000
#define ISO_PSO_BLE       0x00030000
#define ISO_PSO_CCU       0x00040000
#define ISO_PSO_AES       0x00080000
#define ISO_PSO_DMU       0x01000000
#define ISO_PSO_GPIO      0x02000000
#define ISO_PSO_TESTLOGIC 0x08000000
#define ISO_PSO_BLEPHY    0x20000000
#define ISO_PSO_WDT       0x00000000    //dummy value that WDT won't use ISO/PSO


//#define CKGEN_CLKEN_REG             (CKGEN_ADDR_BASE + 0x00000014)
// definition bit for elk en 
#define CLKEN_BLE       0x00000002
#define CLKEN_WKT0      0x00000004
#define CLKEN_WKT1      0x00000008
#define CLKEN_GPIO      0x00000010
#define CLKEN_WDT       0x00000020
#define CLKEN_PWM0      0x00000040
#define CLKEN_PWM1      0x00000080
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
#define CLKEN_RTC       0x00080000
#define CLKEN_DMU       0x01000000
//dummy values that not exist in CLKEN register
#define CLKEN_AES       0x00000000
#define CLKEN_TESTLOGIC 0x00000000
#define CLKEN_BLEPHY    0x00000000


//#define CKGEN_SWRST_REG             (CKGEN_ADDR_BASE + 0x00000018)
// definition bit for sw reset 
#define SWRST_BLEPHY    0x00000001
#define SWRST_BLE       0x00800002
#define SWRST_WKT0      0x00000004
#define SWRST_WKT1      0x00000008
#define SWRST_GPIO      0x00000010
#define SWRST_WDT       0x00000020
#define SWRST_PWM0      0x00000040
#define SWRST_PWM1      0x00000080
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
#define SWRST_RTC       0x00080000
#define SWRST_AES       0x00200000
#define SWRST_DMU       0x01000000
//dummy values that not exist in CLKEN register
#define SWRST_TESTLOGIC 0x00000000


// HS bit definition
//#define HS_CONFIG_REG               (HS_ADDR_BASE + 0x00000000)
#define HS_CONFIG_ClrIntr       0x80000000
#define HS_CONFIG_PupInIntr     0x07000000
#define HS_CONFIG_PupExIntr     0x00070000
#define HS_CONFIG_PupDelayCnt   0x0000F000
#define HS_CONFIG_RingOsc_Dis   0x00000800
#define HS_CONFIG_32KOsc_En     0x00000400
#define HS_CONFIG_32KOsc_Sel    0x00000200
#define HS_CONFIG_HiberClk_En   0x00000100
#define HS_CONFIG_PwrMode       0x00000003


#define HS_STATE_NORMAL 0x00000005




//register of PMU
#define PMU_MODE_RADIOEN        0x40002300
    #define PMU_MODE_ACTIVE         0x00000001
    #define PMU_MODE_SLEEP          0x00000002
    #define PMU_MODE_DEEPSLEEP      0x00000003
    #define PMU_MODE_HIBERNATE      0x00000004
    #define PMU_MODE_MASK           0x00000007
    #define PMU_RADIOEN_PTATOFF     0x01000000
    #define PMU_RADIOEN_BGP         0x02000000
    #define PMU_RADIOEN_LDOVDDD     0x04000000
    #define PMU_RADIOEN_MASK        0x07000000
    
#define PMU_BUCKEN_RADIOCTRL    0x40002308
#define PMU_CUCTRL              0x40002314
    #define ldo_pd0dig_vreg_trim_MASK   0x0000000F
    #define ldo_pd1dig_vreg_trim_MASK   0x000000F0
    #define ldo_pd1ana_vreg_trim_MASK   0x00000F00
    #define ldo_pd0dig_transient        0x00010000
    #define ldo_pd1dig_transient        0x00020000
    #define ldo_pd1ana_transient        0x00040000
    #define pd0_ldo1_perm_perm          0x00080000
    #define pd0_ldo1_perm_pd0ldo        0x00100000
    #define pd1_ldo1_ret_ret            0x00200000
    #define pd1_ldo1_ret_pd1ldo         0x00400000
    #define ldo_pd0dig_en               0x01000000
    #define ldo_pd1dig_en               0x02000000
    #define ldo_pd1ana_en               0x04000000
    #define ldo_ret_off                 0x08000000
    #define vd3_adc_en                  0x10000000
    #define vddio_postdrv_pd1_en        0x20000000


typedef enum {
    PWRMODE_NORMAL=0,
    PWRMODE_SLEEP,
    PWRMODE_DEEPSLEEP,
    PWRMODE_HIBERNATE
}Lowpower_PowerMode_t;

typedef enum {
    WAKEUP_DR0,
    WAKEUP_DR1,
    WAKEUP_DR2
}Lowpower_WakeUp_t;


void cc6801_LowPower_Sleep(Lowpower_PowerMode_t lpMode);

uint32_t cc6801_LowPower_SetWakeupSource(Lowpower_WakeUp_t SetID,uint32_t dwPinNum,BOOL bIsGetData,uint32_t * pBuf);

void cc6801_LowPower_PeripheralOn(uint32_t dwPeripheral);
void cc6801_LowPower_PeripheralOff(uint32_t dwPeripheral);


void cc6801_LowPower_PowerOn(void);
void cc6801_LowPower_PowerOff(void);


#endif
