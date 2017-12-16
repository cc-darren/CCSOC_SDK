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
*  project_test_EVB.h
*
*  Project:
*  --------
*  EVB
*
*  Description:
*  ------------
*  Test case configuration for EVB
*
*  Author:
*  -------
*  PPP   (CC-pattaya)
*
*===========================================================================
*
******************************************************************************/

#ifndef _PROJECT_TEST_EVB_H_
#define _PROJECT_TEST_EVB_H_

/******************************************************************************
Head Block of The File
******************************************************************************/
// Sec 0: Comment block of the file

// Sec 1: Include File
#include "global.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous


#define XTAL_MHZ                48      //uint: MHz
#define SYSTEM_CLOCK_MHZ        32      //unit: MHz


//#define DHRY_TEST               1
#define MSC_CLOCK               1
#define CLK_TCK                 32000   //HW timer clock is 32KHz for Dhrystone measurement
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


/************Add sub-options of this ASIC****************/

//define SPI clock
//define I2C clock
//define UART baudrate, parity, stop bits, flow control ON/OFF
//IO pin mux
//Timer 32K or high clock
//WDT timeout time
//RTC default date/time
//PWM duty cycle


/**************************************************
*  UseInterface(_type,_id) helps to define the interface type and id
*               _type can be set to : SPI/I2C/UART
*               _id   can be set to : 0/1/2/3 , dependson ASIC channels num.
***************************************************/
/**************************************************
*   Choose ACC sensor and config ACC interface
*   (pick one ACC sensor from module_supported.h)
*   (please select ACC_NULL if ACC is not in use)
*
*   ACC_IF :  the interface assigned to ACC
*
*   ACC_IF_TYPE / ACC_IF_ID will be generated automatically.
*   ACC_IF_TYPE : the ACC interface type
*   ACC_IF_ID   : the ACC interface id
***************************************************/
#define MODULE_ACC              ACC_NULL
#define ACC_IF                  UseInterface(SPI,1)



/**************************************************
*   Choose MAG sensor and config MAG interface
*   (pick one MAG sensor from module_supported.h)
*   (please select MAG_NULL if MAG is not in use)
*
*   MAG_IF :  the interface assigned to MAG
*
*   MAG_IF_TYPE / MAG_IF_ID will be generated automatically.
*   MAG_IF_TYPE : the MAG interface type
*   MAG_IF_ID   : the MAG interface id
***************************************************/
#define MODULE_MAG              MAG_NULL
#define MAG_IF                  UseInterface(I2C,0)



/**************************************************
*   Choose GYRO sensor and config GYRO interface
*   (pick one GYRO sensor from module_supported.h)
*   (please select GYR_NULL if GYR is not in use)
*
*   GYR_IF :  the interface assigned to GYR
*
*   GYR_IF_TYPE / GYR_IF_ID will be generated automatically.
*   GYR_IF_TYPE : the GYR interface type
*   GYR_IF_ID   : the GYR interface id
***************************************************/
#define MODULE_GYR              GYR_NULL
#define GYR_IF                  UseInterface(SPI,1)



/**************************************************
*   Choose OLED model and config OLED interface
*   (pick one GYRO sensor from module_supported.h)
*   (please select OLED_NULL if OLED is not in use)
*
*   OLED_IF : the interface assigned to OLED
*
*   OLED_IF_TYPE / OLED_IF_ID will be generated automatically.
*   OLED_IF_TYPE : the OLED interface type
*   OLED_IF_ID   : the OLED interface id
***************************************************/
#define MODULE_OLED             OLED_NULL
#define OLED_IF                 UseInterface(SPI,2)

/**************************************************
*   Choose PPG model and config PPG interface
*   (pick one PPG sensor from module_supported.h)
*   (please select PPG_NULL if OLED is not in use)
*
*   PPG_IF : the interface assigned to PPG
*
*   PPG_IF_TYPE / PPG_IF_ID will be generated automatically.
*   PPG_IF_TYPE : the PPG interface type
*   PPG_IF_ID   : the PPG interface id
***************************************************/
#define MODULE_PPG              PPG_PXT_PAH8002
#define PPG_IF                  UseInterface(I2C,1)

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

/**************************************************
*   Config VIBRATOR interface
*
*   VIBRATOR_IF : the interface assigned to SWT
*
*   VIBRATOR_IF_TYPE / VIBRATOR_IF_ID will be generated automatically.
*   VIBRATOR_IF_TYPE : the SWT interface type
*   VIBRATOR_IF_ID   : the SWT interface id
***************************************************/
#define VIBRATOR_IF             UseInterface(PWM,0)

/**************************************************
*   Config ADC interface
*
*   ADC_IF : the interface assigned to ADC
*
*   ADC_IF_TYPE / ADC_IF_ID will be generated automatically.
*   ADC_IF_TYPE : the ADC interface type
*   ADC_IF_ID   : the ADC interface id
***************************************************/
#define MODULE_ADC              ADC_NULL
#define ADC_IF                  UseInterface(I2C,0)

/**************************************************
*   Config TEST interface
*
*   TEST_XXX_IF : the interface assigned to Tester
*
***************************************************/
#define MODULE_TEST                   1
#define TEST_I2C0_IF                  UseInterface(I2C,0)
#define TEST_I2C1_IF                  UseInterface(I2C,1)

#define TEST_SPI0_IF                  UseInterface(SPI,0)
#define TEST_SPI1_IF                  UseInterface(SPI,1)
#define TEST_SPI2_IF                  UseInterface(SPI,2)

#define TEST_UART0_IF                 UseInterface(UART,0)
#define TEST_UART1_IF                 UseInterface(UART,1)
#define TEST_UART2_IF                 UseInterface(UART,2)

/******************************************************************************************************************
*   Set SPI master configuration
*   (all the configs are in cc6801_options.h)
*   (please using OR operation to choose more than 2 configs)
*
*    Ex : SPIM0_CONFIG         (SPI_MODE_0 | SPI_CS_HIGH)
*******************************************************************************************************************/
#define SPIM0_CONFIG            (SPI_MODE_0)
#define SPIM0_CLOCK             (1000000)
#define SPIM1_CONFIG            (SPI_MODE_3)
#define SPIM1_CLOCK             (10000000)
#define SPIM2_CONFIG            (SPI_MODE_3)
#define SPIM2_CLOCK             (8000000)


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
#define GPIO_MODE_PINGROUP0                  GPIO_MODE_UART
#define GPIO0_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO1_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO2_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE) | (GPIO_DIR_INPUT)  | (GPIO_INPUT_NOPULL))
#define GPIO3_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE) | (GPIO_DIR_INPUT)  | (GPIO_INPUT_NOPULL))

// GPIO Pin Group1
#define GPIO_MODE_PINGROUP1                  GPIO_MODE_GPIO
#define GPIO4_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_LOW))
#define GPIO5_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_LOW))
#define GPIO6_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_LOW))
#define GPIO7_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_LOW))

// GPIO Pin Group2
#define GPIO_MODE_PINGROUP2                  GPIO_MODE_GPIO_I2C
#define GPIO8_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_INPUT) | (GPIO_INPUT_NOPULL))
#define GPIO9_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_INPUT) | (GPIO_INPUT_NOPULL))
#define GPIO10_CONFIG                       ((GPIO_PULL_UP)   | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_INPUT) | (GPIO_INPUT_NOPULL))
#define GPIO11_CONFIG                       ((GPIO_PULL_UP)   | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_INPUT) | (GPIO_INPUT_NOPULL))

// GPIO Pin Group3
#define GPIO_MODE_PINGROUP3                  GPIO_MODE_I2C_GPIO
#define GPIO12_CONFIG                        ((GPIO_PULL_UP)   | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_INPUT) | (GPIO_INPUT_NOPULL))
#define GPIO13_CONFIG                        ((GPIO_PULL_UP)   | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_INPUT) | (GPIO_INPUT_NOPULL))
#define GPIO14_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_INPUT) | (GPIO_INPUT_NOPULL))
#define GPIO15_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_INPUT) | (GPIO_INPUT_NOPULL))

// GPIO Pin Group4
#define GPIO_MODE_PINGROUP4                  GPIO_MODE_SPI_4WIRE
#define GPIO16_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO17_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO18_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO19_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE) | (GPIO_DIR_INPUT)  | (GPIO_INPUT_NOPULL))

// GPIO Pin Group5
#define GPIO_MODE_PINGROUP5                  GPIO_MODE_SPI_3WIRE
#define GPIO20_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO21_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO22_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO23_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_INPUT) | (GPIO_INPUT_NOPULL))

// GPIO Pin Group6
#define GPIO_MODE_PINGROUP6                  GPIO_MODE_SPI_4WIRE
#define GPIO24_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO25_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO26_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO27_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE) | (GPIO_DIR_INPUT)  | (GPIO_INPUT_NOPULL))

// GPIO Pin Group7
#define GPIO_MODE_PINGROUP7                  GPIO_MODE_I2S_MASTER
#define GPIO28_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO29_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO30_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO31_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE) | (GPIO_DIR_INPUT)  | (GPIO_INPUT_NOPULL))

// GPIO Pin Group8
#define GPIO_MODE_PINGROUP8                  GPIO_MODE_DMIC
#define GPIO32_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO33_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_INPUT)  | (GPIO_INPUT_NOPULL))
#define GPIO34_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_LOW))
#define GPIO35_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_LOW))

// GPIO Pin Group9
#define GPIO_MODE_PINGROUP9                  GPIO_MODE_PWM_GPIO
#define GPIO36_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO37_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO38_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_LOW))
#define GPIO39_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_LOW))

// GPIO Pin Group10
#define GPIO_MODE_PINGROUP10                  GPIO_MODE_GPIO_I2C
#define GPIO40_CONFIG                         ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_INPUT) | (GPIO_INPUT_NOPULL))
#define GPIO41_CONFIG                         ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_INPUT) | (GPIO_INPUT_NOPULL))
#define GPIO42_CONFIG                        ((GPIO_PULL_UP)   | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_INPUT) | (GPIO_INPUT_NOPULL))
#define GPIO43_CONFIG                        ((GPIO_PULL_UP)   | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_INPUT) | (GPIO_INPUT_NOPULL))

// GPIO Pin Group11
#define GPIO_MODE_PINGROUP11                  GPIO_MODE_GPIO
#define GPIO44_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_LOW))
#define GPIO45_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_LOW))
#define GPIO46_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_LOW))
#define GPIO47_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_LOW))


#define TEST_I2C_BUS        TEST_I2C0_IF_ID
#define TEST_I2C_ADDRESS    0x50
#define TEST_I2C_CLKRATE    I2C0_CLOCK

#define TEST_SPIM_BUS       TEST_SPI0_IF_ID

#define TEST_UART0_BUS      TEST_UART0_IF_ID
//#define TEST_UART1_BUS      TEST_UART1_IF_ID
//#define TEST_UART2_BUS      TEST_UART2_IF_ID


//#define TEST_AES 1
//#define TEST_CLOCK 1
//#define TEST_MEMRW 1
//#define TEST_SPI 1
//#define TEST_I2C 1
//#define TEST_UART_LOOPBACK 1
#define TEST_WKTM 1
//#define TEST_DMIC  1        //When DMIC is enabled, other testing must be disabled.

/******************************************************************************
Declaration of External Variables & Functions
******************************************************************************/
// Sec 3: declaration of external variable

// Sec 4: declaration of external function prototype


//I2C parameters

//UART parameters

/******************************************************************************
Declaration of data structure
******************************************************************************/
// Sec 5: structure, uniou, enum, linked list

/******************************************************************************
Declaration of Global Variables & Functions
******************************************************************************/
// Sec 6: declaration of global variable

// Sec 7: declaration of global function prototype

/******************************************************************************
Declaration of static Global Variables & Functions
******************************************************************************/
// Sec 8: declaration of static global variable
//#define FSTORAGE_ENABLED     1
//#define FDS_ENABLED          1


// Sec 9: declaration of static function prototype

/******************************************************************************
// Sec 10: C Functions
******************************************************************************/

#endif // _PROJECT_TEST_EVB_H_
