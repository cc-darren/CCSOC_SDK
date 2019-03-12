/* Copyright (c) 2019 Cloudchip, Inc. All Rights Reserved.
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
*  config_DVK403A.h
*
*  Project:
*  --------
*  DVK403A
*
*  Description:
*  ------------
*  FW configuration for DVK403A
*
*  Author:
*  -------
*  PPP   (CC-pattaya)
*
*===========================================================================
*
******************************************************************************/

#ifndef _CONFIG_DVK403A_H_
#define _CONFIG_DVK403A_H_

/******************************************************************************
Head Block of The File
******************************************************************************/
// Sec 0: Comment block of the file

// Sec 1: Include File
#include "global.h"


// Sec 2: Constant Definitions, Imported Symbols, miscellaneous


#define XTAL_MHZ                48      //uint: MHz
#define SYSTEM_CLOCK_MHZ        96      //unit: MHz
#define USE_PLL                 TRUE


//#define DHRY_TEST               1
#define MSC_CLOCK               1
#define CLK_TCK                 32768   //HW timer clock is 32KHz for Dhrystone measurement
#define Number_Of_Runs          1000000


/**************************************************
*
*   Choose ASIC
*   (pick one ASIC from asic_supported.h)
*
***************************************************/
#define ASIC                    CC6801

//find and include project options from the asic you chose
#include "asic_options.h"


/**************************************************
*   Config Tracer interface
*
*   TRACER_IF : the interface assigned to Tracer
*
*   TRACER_IF_TYPE / TRACER_IF_ID will be generated automatically.
*   TRACER_IF_TYPE : the Tracer interface type
*   TRACER_IF_ID   : the Tracer interface id
***************************************************/
#define TRACER_IF               UseInterface(UART,0)
#define TRACER_LOGBUF_SIZE      TRACER_LOGBUF_1024
#define TRACER_LOGLEVEL         TRACER_LOGLEVEL_ALL

/**************************************************
*   Config HCI interface
*
*   HCI_IF : the interface assigned to HCI
*
*   HCI_IF_TYPE / HCI_IF_ID will be generated automatically.
*   HCI_IF_TYPE : the HCI interface type
*   HCI_IF_ID   : the HCI interface id
***************************************************/
#define HCI_IF                  UseInterface(UART,1)

/**************************************************
*   Config SWT interface (Software Timer)
*
*   SWT_IF : the interface assigned to SWT
*   SWT_IF available : WKTM0, WKTM1
*
*   SWT_IF_TYPE / SWT_IF_ID will be generated automatically.
*   SWT_IF_TYPE : the SWT interface type
*   SWT_IF_ID   : the SWT interface id
***************************************************/
#define SWT_IF                  UseInterface(WKTM,0)

/******************************************************************************************************************
*   Set SPI master configuration
*   (all the configs are in cc6801_options.h)
*   (please using OR operation to choose more than 2 configs)
*
*    Ex : SPIM0_CONFIG         (SPI_MODE_0 | SPI_CS_HIGH)
*******************************************************************************************************************/
#define SPIM0_CONFIG            (SPI_MODE_0)
#define SPIM0_CLOCK             (24000000)
#define SPIM1_CONFIG            (SPI_MODE_3)
#define SPIM1_CLOCK             (10000000)
#define SPIM2_CONFIG            (SPI_MODE_0 | SPI_CS_HIGH)
#define SPIM2_CLOCK             (4000000)


/******************************************************************************************************************
*   Set I2C clock configuration
*   (all the configs are in cc6801_options.h)
*******************************************************************************************************************/
#define I2C0_CLOCK              (I2C_FREQ_100K)
#define I2C1_CLOCK              (I2C_FREQ_100K)


/******************************************************************************************************************
*   Set Uart configuration
*   (all the configs are in cc6801_options.h)
*   (please using OR operation to choose more than 2 configs)
*
*    Ex : UART0_CONFIG         (UART_BAUDRATE_115200 | UART_DATA_BITS_8)
*******************************************************************************************************************/
#define UART_USE_DMA            1
#define UART0_CONFIG            ((UART_BAUDRATE_115200) | (UART_DATA_BITS_8))
#define UART1_CONFIG            ((UART_BAUDRATE_115200) | (UART_DATA_BITS_8))
#define UART2_CONFIG            ((UART_BAUDRATE_115200) | (UART_DATA_BITS_8))


/******************************************************************************************************************
*   Set RTC configuration   (all the configs are in cc6801_options.h)
*   Options:
*   RTC_INUSE               TRUE or FALSE
*   RTC_DEFAULT_YEAR        2017, 2018, 2019 ...    must NOT be less than 2000
*   RTC_DEFAULT_MONTH       RTC_JAN ~ RTC_DEC
*   RTC_DEFAULT_DAY         1~31
*   RTC_DEFAULT_HOUR        0~59
*   RTC_DEFAULT_MIN         0~59
*   RTC_DEFAULT_SEC         0~59
*   RTC_DEFAULT_WEEK        RTC_SUNDAY ~ RTC_SATURDAY
*   RTC_DEFAULT_H24         RTC_HOUR_24H, RTC_HOUR_AM, RTC_HOUR_PM
*******************************************************************************************************************/
#define RTC_INUSE               TRUE
#define RTC_DEFAULT_YEAR        2017
#define RTC_DEFAULT_MONTH       RTC_JAN
#define RTC_DEFAULT_DAY         1
#define RTC_DEFAULT_HOUR        0
#define RTC_DEFAULT_MIN         0
#define RTC_DEFAULT_SEC         0
#define RTC_DEFAULT_WEEK        RTC_SUNDAY   //2017-1-1 is Sunday
#define RTC_DEFAULT_H24         RTC_HOUR_AM


/******************************************************************************************************************
*   Set WDT configuration   (all the configs are in cc6801_options.h)
*   Options:
*   WDT_INUSE               TRUE or FALSE
*   WDT_DEFAULT_TIMEOUT     Unit : ms
*******************************************************************************************************************/
#define WDT_INUSE               TRUE
#define WDT_DEFAULT_TIMEOUT     30000


/******************************************************************************************************************
*   Set GPIO configuration
*   (all the configs are in cc6801_options.h)
*   (please using OR operation to choose more than 2 configs)
*
*    Ex : GPIO0_CONFIG         ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
*******************************************************************************************************************/
// GPIO Pin Group0
#define GPIO_MODE_PINGROUP0                  GPIO_MODE_PWM_GPIO
#define GPIO0_CONFIG                         ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_LOW))
#define GPIO1_CONFIG                         ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_LOW))
#define GPIO2_CONFIG                         ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_LOW))
#define GPIO3_CONFIG                         ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_LOW))

// GPIO Pin Group1
#define GPIO_MODE_PINGROUP1                  GPIO_MODE_GPIO
#define GPIO4_CONFIG                         ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_INPUT)  | (GPIO_INPUT_NOPULL))
#define GPIO5_CONFIG                         ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_INPUT)  | (GPIO_INPUT_NOPULL))
#define GPIO6_CONFIG                         ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_INPUT)  | (GPIO_INPUT_NOPULL))
#define GPIO7_CONFIG                         ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_INPUT)  | (GPIO_INPUT_NOPULL))

// GPIO Pin Group2
#define GPIO_MODE_PINGROUP2                  GPIO_MODE_I2C_GPIO
#define GPIO8_CONFIG                         ((GPIO_PULL_UP)   | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_INPUT)  | (GPIO_INPUT_NOPULL))
#define GPIO9_CONFIG                         ((GPIO_PULL_UP)   | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_INPUT)  | (GPIO_INPUT_NOPULL))
#define GPIO10_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_INPUT)  | (GPIO_INPUT_NOPULL))
#define GPIO11_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_INPUT)  | (GPIO_INPUT_NOPULL))

// GPIO Pin Group3
#define GPIO_MODE_PINGROUP3                  GPIO_MODE_UART
#define GPIO12_CONFIG                        ((GPIO_PULL_UP)   | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO13_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO14_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_INPUT)  | (GPIO_INPUT_NOPULL))
#define GPIO15_CONFIG                        ((GPIO_PULL_UP)   | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_INPUT)  | (GPIO_INPUT_NOPULL))

// GPIO Pin Group4
#define GPIO_MODE_PINGROUP4                  GPIO_MODE_SPI_4WIRE
#define GPIO16_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO17_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO18_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO19_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_INPUT)  | (GPIO_INPUT_NOPULL))

// GPIO Pin Group5
#define GPIO_MODE_PINGROUP5                  GPIO_MODE_GPIO_I2C
#define GPIO20_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_LOW))
#define GPIO21_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_LOW))
#define GPIO22_CONFIG                        ((GPIO_PULL_UP)   | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_INPUT)  | (GPIO_INPUT_NOPULL))
#define GPIO23_CONFIG                        ((GPIO_PULL_UP)   | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_INPUT)  | (GPIO_INPUT_NOPULL))

// GPIO Pin Group6
#define GPIO_MODE_PINGROUP6                  GPIO_MODE_GPIO
#define GPIO24_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_INPUT)  | (GPIO_INPUT_NOPULL))
#define GPIO25_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_LOW))
#define GPIO26_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_LOW))
#define GPIO27_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_LOW))

// GPIO Pin Group7
#define GPIO_MODE_PINGROUP7                  GPIO_MODE_I2S_MASTER
#define GPIO28_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO29_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO30_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO31_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_INPUT)  | (GPIO_INPUT_NOPULL))

// GPIO Pin Group8
#define GPIO_MODE_PINGROUP8                  GPIO_MODE_SPI_4WIRE
#define GPIO32_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO33_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO34_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO35_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_INPUT)  | (GPIO_INPUT_NOPULL))

// GPIO Pin Group9
#define GPIO_MODE_PINGROUP9                  GPIO_MODE_GPIO
#define GPIO36_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE)  | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO37_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE)  | (GPIO_DIR_INPUT)  | (GPIO_INPUT_NOPULL))
#define GPIO38_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_LOW))
#define GPIO39_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_LOW))

// GPIO Pin Group10
#define GPIO_MODE_PINGROUP10                 GPIO_MODE_SPI_3WIRE
#define GPIO40_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO41_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO42_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO43_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_LOW))

// GPIO Pin Group11
#define GPIO_MODE_PINGROUP11                 GPIO_MODE_GPIO
#define GPIO44_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_LOW))
#define GPIO45_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_LOW))
#define GPIO46_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_LOW))
#define GPIO47_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_LOW))


/******************************************************************************
// Application settings
******************************************************************************/
#define DEVICE_MODEL "CC_WATCH"
#define SDK_FW_VERSION "v1.000.003"

/******************************************************************************
// BLE config settings
******************************************************************************/

// <h> BLE - ENABLE/DISABLE
#define CFG_BLE    1

// <h> BLE - DEVICE NAME
#define APP_DFLT_DEVICE_NAME    "BLE-UART"

// <h> BLE - DEVICE ADDRESS
#define APP_DFLT_DEVICE_ADDR    { 0x59, 0xBB, 0xAA, 0x79, 0x23, 0x01 }

// <h> BLE - CCPS Profile
//#define CFG_SDK_CCPS_EN

// <h> BLE - Data Length Extension (v4.2)
#define CFG_DLE_EN

// <h> BLE IP ARCH CONFIGURATION
#define CFG_BLE_APP

// <q> EMB   - BLE Embedded Sub-block
#ifndef CFG_EMB
    #define CFG_EMB    1
#endif

// <q> HOST - BLE HOST Sub-block
#ifndef CFG_HOST
    #define CFG_HOST    1
#endif

#define CFG_PERIPHERAL
//#define CFG_CENTRAL

/******************************************************************************
// C Functions
******************************************************************************/

#define CFG_CON    8
#define CFG_SLEEP

#define CFG_CHNL_ASSESS

//#define CFG_AHITL
//#define CFG_HCITL
#define CFG_NVDS
#define CFG_HW_AUDIO

#define CFG_SEC_CON
#define CFG_APP_SEC

//#define CFG_ATTC
#define CFG_ATTS

#define CFG_RF_ATLAS

#define CFG_PRF
#define CFG_NB_PRF    1
//#define CFG_PRF_CCPS

#define CFG_APP
//#define CFG_APP_CCPS   1

#define CFG_PRF_BLEUARTS   1
#define CFG_APP_BLEUARTS   1

#define UART_IP_MODE	1
#define SW_TIMER_BY_KERNEL  // min timer: 10ms


#endif // _CONFIG_EVB_H_


