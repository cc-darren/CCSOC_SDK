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
*  project_FPGA.h
*
*  Project:
*  --------
*  FPGA
*
*  Description:
*  ------------
*  FW configuration for FPGA
*
*  Author:
*  -------
*  PPP   (CC-pattaya)
*
*===========================================================================
*
******************************************************************************/

#ifndef _PROJECT_FPGA_H_
#define _PROJECT_FPGA_H_

/******************************************************************************
Head Block of The File
******************************************************************************/
// Sec 0: Comment block of the file

// Sec 1: Include File

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
*   ACC_IF_TYPE : the ACC interface tpye
*   ACC_IF_ID   : the ACC interface id
***************************************************/
#define MODULE_ACC              ACC_ST_LSM6DSL
#define ACC_IF                  UseInterface(SPI,1)



/**************************************************
*   Choose MAG sensor and config MAG interface
*   (pick one MAG sensor from module_supported.h)
*   (please select MAG_NULL if MAG is not in use)
*
*   MAG_IF :  the interface assigned to MAG
*
*   MAG_IF_TYPE / MAG_IF_ID will be generated automatically.
*   MAG_IF_TYPE : the MAG interface tpye
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
*   GYR_IF_TYPE : the GYR interface tpye
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
*   OLED_IF_TYPE : the OLED interface tpye
*   OLED_IF_ID   : the OLED interface id
***************************************************/
#define MODULE_OLED             OLED_SOLOMON_SSD1306
#define OLED_IF                 UseInterface(SPI,2)


/******************************************************************************************************************
*   Set SPI master configuration
*   (all the configs are in cc6801_options.h)
*   (please using OR operation to choose more than 2 configs)
*
*    Ex : SPIM0_CONFIG         (_SPI_MODE_0_ | _SPI_CS_HIGH_)
*******************************************************************************************************************/
#define SPIM0_CONFIG            (_SPI_MODE_0_)
#define SPIM1_CONFIG            (_SPI_MODE_3_)
#define SPIM2_CONFIG            (_SPI_MODE_3_)


/******************************************************************************************************************
*   Set I2C clock configuration
*   (all the configs are in cc6801_options.h)
*******************************************************************************************************************/
#define I2C0_CLOCK              (_I2C_FREQ_100K_)
#define I2C1_CLOCK              (_I2C_FREQ_100K_)


/******************************************************************************************************************
*   Set GPIO configuration
*   (all the configs are in cc6801_options.h)
*   (please using OR operation to choose more than 2 configs)
*
*    Ex : GPIO0_CONFIG         ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
*******************************************************************************************************************/
// GPIO Pin Group0
#define GPIO_MODE_PINGROUP0                  GPIO_MODE_SPI_4WIRE
#define GPIO0_CONFIG                         ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO1_CONFIG                         ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO2_CONFIG                         ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO3_CONFIG                         ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE) | (GPIO_DIR_INPUT)  | (GPIO_INPUT_NOPULL))

// GPIO Pin Group1
#define GPIO_MODE_PINGROUP1                  GPIO_MODE_SPI_4WIRE
#define GPIO4_CONFIG                         ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO5_CONFIG                         ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO6_CONFIG                         ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO7_CONFIG                         ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE) | (GPIO_DIR_INPUT)  | (GPIO_INPUT_NOPULL))

// GPIO Pin Group2
#define GPIO_MODE_PINGROUP2                  GPIO_MODE_SPI_4WIRE
#define GPIO8_CONFIG                         ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO9_CONFIG                         ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO10_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO11_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE) | (GPIO_DIR_INPUT)  | (GPIO_INPUT_NOPULL))

// GPIO Pin Group3
#define GPIO_MODE_PINGROUP3                  GPIO_MODE_DMIC
#define GPIO12_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO13_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_INPUT)  | (GPIO_INPUT_NOPULL))
#define GPIO14_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_LOW))
#define GPIO15_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_LOW))

// GPIO Pin Group4
#define GPIO_MODE_PINGROUP4                  GPIO_MODE_I2C_GPIO
#define GPIO16_CONFIG                        ((GPIO_PULL_UP)   | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_INPUT) | (GPIO_INPUT_NOPULL))
#define GPIO17_CONFIG                        ((GPIO_PULL_UP)   | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_INPUT) | (GPIO_INPUT_NOPULL))
#define GPIO18_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_INPUT) | (GPIO_INPUT_NOPULL))
#define GPIO19_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_INPUT) | (GPIO_INPUT_NOPULL))

// GPIO Pin Group5
#define GPIO_MODE_PINGROUP5                  GPIO_MODE_I2C_GPIO
#define GPIO20_CONFIG                        ((GPIO_PULL_UP)   | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_INPUT) | (GPIO_INPUT_NOPULL))
#define GPIO21_CONFIG                        ((GPIO_PULL_UP)   | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_INPUT) | (GPIO_INPUT_NOPULL))
#define GPIO22_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_INPUT) | (GPIO_INPUT_NOPULL))
#define GPIO23_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_INPUT) | (GPIO_INPUT_NOPULL))

// GPIO Pin Group6
#define GPIO_MODE_PINGROUP6                  GPIO_MODE_UART
#define GPIO24_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO25_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO26_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE) | (GPIO_DIR_INPUT)  | (GPIO_INPUT_NOPULL))
#define GPIO27_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE) | (GPIO_DIR_INPUT)  | (GPIO_INPUT_NOPULL))

// GPIO Pin Group7
#define GPIO_MODE_PINGROUP7                  GPIO_MODE_PWM_GPIO
#define GPIO28_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO29_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO30_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_LOW))
#define GPIO31_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_LOW))

// GPIO Pin Group8
#define GPIO_MODE_PINGROUP8                  GPIO_MODE_I2S_MASTER
#define GPIO32_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO33_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO34_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO35_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE) | (GPIO_DIR_INPUT)  | (GPIO_INPUT_NOPULL))

// GPIO Pin Group9
#define GPIO_MODE_PINGROUP9                  GPIO_MODE_UART
#define GPIO36_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO37_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO38_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_INPUT) | (GPIO_INPUT_NOPULL))
#define GPIO39_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_INPUT) | (GPIO_INPUT_NOPULL))

// GPIO Pin Group10
#define GPIO_MODE_PINGROUP10                  GPIO_MODE_UART
#define GPIO40_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO41_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO42_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_INPUT)  | (GPIO_INPUT_NOPULL))
#define GPIO43_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_INPUT)  | (GPIO_INPUT_NOPULL))

// GPIO Pin Group11
#define GPIO_MODE_PINGROUP11                  GPIO_MODE_GPIO
#define GPIO44_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_LOW))
#define GPIO45_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_LOW))
#define GPIO46_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_LOW))
#define GPIO47_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_LOW))


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

// Sec 9: declaration of static function prototype

/******************************************************************************
// Sec 10: C Functions
******************************************************************************/

#endif // _PROJECT_FPGA_H_
