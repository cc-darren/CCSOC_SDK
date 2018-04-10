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
*  project_zeus_EVB.h
*
*  Project:
*  --------
*  EVB
*
*  Description:
*  ------------
*  Zeus configuration for EVB QFN60L
*
*  Author:
*  -------
*  PPP   (CC-pattaya)
*
*===========================================================================
*
******************************************************************************/

#ifndef _PROJECT_ZEUS_EVB_H_
#define _PROJECT_ZEUS_EVB_H_

/******************************************************************************
Head Block of The File
******************************************************************************/
// Sec 0: Comment block of the file

// Sec 1: Include File
#include "global.h"
#include "stdint.h"
//#include "CC_Calorie_burn.h"
#include "util_calendar.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous


#define XTAL_MHZ                48      //uint: MHz
#define SYSTEM_CLOCK_MHZ        32      //unit: MHz


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


/************Add sub-options of this ASIC****************/

// EMWIN AND JDI
#define nEMWIN_ENABLE

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
*   MAG_IF_TYPE : the MAG interface type
*   MAG_IF_ID   : the MAG interface id
***************************************************/
#define MODULE_MAG              MAG_AKM_AK09912C
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
#ifndef EMWIN_ENABLE
#define MODULE_OLED             OLED_SOLOMON_SH1107
#else
#define MODULE_OLED             OLED_JDI_LPM013M126A
#endif
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
#define MODULE_ADC              ADC_MCP_MCP3421
#define ADC_IF                  UseInterface(I2C,0)

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

#ifndef EMWIN_ENABLE
#define SPIM2_CONFIG            (SPI_MODE_3)
#define SPIM2_CLOCK             (8000000)
#else
#define SPIM2_CONFIG            (SPI_MODE_0 | SPI_CS_HIGH)
#define SPIM2_CLOCK             (2000000)
#endif


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
#define GPIO_MODE_PINGROUP0                  GPIO_MODE_GPIO
#define GPIO0_CONFIG                         ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_LOW))
#define GPIO1_CONFIG                         ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_LOW))
#define GPIO2_CONFIG                         ((GPIO_PULL_UP)   | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_INPUT) | (GPIO_INPUT_NOPULL))
#define GPIO3_CONFIG                         ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_INPUT) | (GPIO_INPUT_NOPULL))

// GPIO Pin Group1
#define GPIO_MODE_PINGROUP1                  GPIO_MODE_GPIO
#define GPIO4_CONFIG                         ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_LOW))
#define GPIO5_CONFIG                         ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_LOW))
#define GPIO6_CONFIG                         ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_LOW))
#define GPIO7_CONFIG                         ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_LOW))

// GPIO Pin Group2
#define GPIO_MODE_PINGROUP2                  GPIO_MODE_GPIO
#define GPIO8_CONFIG                         ((GPIO_PULL_UP)   | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_INPUT) | (GPIO_INPUT_NOPULL))
#define GPIO9_CONFIG                         ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_INPUT) | (GPIO_INPUT_NOPULL))
#define GPIO10_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_INPUT) | (GPIO_INPUT_NOPULL))
#define GPIO11_CONFIG                        ((GPIO_PULL_UP)   | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_INPUT) | (GPIO_INPUT_NOPULL))

// GPIO Pin Group3
#define GPIO_MODE_PINGROUP3                  GPIO_MODE_I2C_GPIO
#define GPIO12_CONFIG                        ((GPIO_PULL_UP)   | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_INPUT) | (GPIO_INPUT_NOPULL))
#define GPIO13_CONFIG                        ((GPIO_PULL_UP)   | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_INPUT) | (GPIO_INPUT_NOPULL))
#define GPIO14_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_INPUT) | (GPIO_INPUT_NOPULL))
#define GPIO15_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_LOW))

// GPIO Pin Group4
#define GPIO_MODE_PINGROUP4                  GPIO_MODE_SPI_4WIRE
#define GPIO16_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO17_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO18_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO19_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE) | (GPIO_DIR_INPUT)  | (GPIO_INPUT_NOPULL))

// GPIO Pin Group5
#define GPIO_MODE_PINGROUP5                  GPIO_MODE_SPI_3WIRE
#define GPIO20_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO21_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO22_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO23_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_LOW))

// GPIO Pin Group6
#ifndef EMWIN_ENABLE
#define GPIO_MODE_PINGROUP6                  GPIO_MODE_SPI_4WIRE
#define GPIO24_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO25_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO26_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO27_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE) | (GPIO_DIR_INPUT)  | (GPIO_INPUT_NOPULL))
#else
#define GPIO_MODE_PINGROUP6                  GPIO_MODE_GPIO
#define GPIO24_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_LOW))
#define GPIO25_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_LOW))
#define GPIO26_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_LOW))
#define GPIO27_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_LOW))
#endif

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
#define GPIO34_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_INPUT)  | (GPIO_INPUT_NOPULL))
#define GPIO35_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_LOW))

// GPIO Pin Group9
#define GPIO_MODE_PINGROUP9                  GPIO_MODE_PWM_GPIO
#define GPIO36_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO37_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO38_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_INPUT) | (GPIO_INPUT_NOPULL))
#define GPIO39_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_INPUT) | (GPIO_INPUT_NOPULL))

// GPIO Pin Group10
#define GPIO_MODE_PINGROUP10                  GPIO_MODE_GPIO_I2C
#define GPIO40_CONFIG                         ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_INPUT) | (GPIO_INPUT_NOPULL))
#define GPIO41_CONFIG                         ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_INPUT) | (GPIO_INPUT_NOPULL))
#define GPIO42_CONFIG                         ((GPIO_PULL_UP)   | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_INPUT) | (GPIO_INPUT_NOPULL))
#define GPIO43_CONFIG                         ((GPIO_PULL_UP)   | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_INPUT) | (GPIO_INPUT_NOPULL))

// GPIO Pin Group11
#define GPIO_MODE_PINGROUP11                  GPIO_MODE_UART
#define GPIO44_CONFIG                         ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO45_CONFIG                         ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO46_CONFIG                         ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE) | (GPIO_DIR_INPUT)  | (GPIO_INPUT_NOPULL))
#define GPIO47_CONFIG                         ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE) | (GPIO_DIR_INPUT)  | (GPIO_INPUT_NOPULL))


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
#define FSTORAGE_ENABLED     1
#define FDS_ENABLED          1


// Sec 9: declaration of static function prototype

/******************************************************************************
// Sec 10: C Functions
******************************************************************************/






/******************************************************************************
// Sec 11: Application settings
******************************************************************************/




#define DEVICE_MODEL "CC-6801"
#define SDK_FW_VERSION "v0.000.004"

#define CFG_APP_CODE_ONLY

#ifndef CFG_APP_CODE_ONLY
#define CFG_JUMP_TABLE_2
#endif

#ifdef EMWIN_ENABLE
#define JDI_DRAW_WIHTTIMER
#endif

#define SLEEP_EN
#define LONGSIT_EN
#define PEDO_EN
#define HRM_EN
//#define BLE_OTA_BL_MODE_EN
//#define DB_EN
#define FIFO_MODE_EN
#define FACTORY_RESET

#define APP_VIB_MGR


#define TOUCH_INT_PIN       GPIO_PIN_2
#define HRM_INT_PIN         GPIO_PIN_9
#define HRM_RST_PIN         GPIO_PIN_15

#if (MODULE_OLED == OLED_SOLOMON_SH1107)
#define OLED_PWR_PIN        GPIO_PIN_1
#define OLED_RST_PIN        GPIO_PIN_23
#define OLED_DC_PIN         GPIO_PIN_0
#endif

#define SWAP_ACC_DIRECTION_EN

#define FORCE_LIFTARM_TEST_EN   // for test
//#define FORCE_SWIM_TEST_EN   // for test
//#define FORCE_HRS_TEST_EN   // for test

#define APP_SERV_MGR_EN        // for new service/sensor manager
#ifdef APP_SERV_MGR_EN
//#define SM_TEST_EN
//#define SRV_MGR_TEST_EN
#endif

#define SW_TIMER_BY_KERNEL  // min timer: 10ms

//-----------------------------------
//  _bState : 1  , Init form system reboot
//          : 2 ,  Init form Venus app setting
//------------------------------------
#define   DB_INIT_FROM_SYSTEM       1
#define   DB_INIT_FROM_APP              2
#define   DB_INIT_FROM_APP_FACTORY_RESET 3

#define  TIME_OF_24_HOUR     86400


typedef enum
{

    eSysStateInit=1,
    eSysStateNormal,
    eSysStateLowPwr,
    eSysInvaild
}eSystem_Battery_State_t;


enum
{
    eEvent_None = 0,
    eEvent_LOWPOWER,
    eEvent_TOUCH,
//    eEvent_LONG_TOUCH,
    eEvent_SWIM_ON,
    eEvent_SWIM_OFF,
    eEvent_SWIM_CONFIRM_ON,
    eEvent_SWIM_CONFIRM_OFF,
    eEvent_CHARGINGIN,
    eEvent_CHARGINGOUT,
    eEvent_CHARGINGFULL,
    eEvent_HRM_DATA,
    eEvent_HRM_TIMEOUT,
    eEvent_HEARTRATESTRAPMODE_ON,
    eEvent_HEARTRATESTRAPMODE_OFF,
    eEvent_LIFTARM_UP,
    eEvent_LIFTARM_DOWN,
    eEvent_LONGSIT,
    eEvent_INCOMMINGCALL_ON,
    eEvent_INCOMMINGCALL_OFF,
    eEvent_INCOMMINGSMS,
    eEvent_ALARM,
    eEvent_HR_WARNING_HIHG,
    eEvent_HR_WARNING_LOW,
    eEvent_FACTORY_RESET_START,
    eEvent_FACTORY_RESET_DONE,
    eEvent_FACTORY_RESET_STOP,  //LOW POWER FAIL
    eEvent_PAIR_PASSKEY,
    eEvent_PAIR_SUCCESS,
    eEvent_PAIR_FAIL,
#ifdef PED_GOAL
    eEvent_PED_GOAL_ACHIEVE,
    eEvent_SWIMMING_GOAL_ACHIEVE,
#endif
    eEvent_Dummy
};

#if 0
enum
{
    eEvent_None = 0,
    eEvent_LOWPOWER,
    eEvent_TOUCH,
//    eEvent_LONG_TOUCH,
    eEvent_SWIM_ON,
    eEvent_SWIM_OFF,
    eEvent_SWIM_CONFIRM_ON,
    eEvent_SWIM_CONFIRM_OFF,
    eEvent_CHARGINGIN,
    eEvent_CHARGINGOUT,
    eEvent_CHARGINGFULL,
    eEvent_HRM_DATA,
    eEvent_HRM_TIMEOUT,
    eEvent_HEARTRATESTRAPMODE_ON,
    eEvent_HEARTRATESTRAPMODE_OFF,
    eEvent_LIFTARM,
    eEvent_LONGSIT,
    eEvent_INCOMMINGCALL_ON,
    eEvent_INCOMMINGCALL_OFF,
    eEvent_INCOMMINGSMS,
    eEvent_ALARM,
    eEvent_FACTORY_RESET_START,
    eEvent_FACTORY_RESET_DONE,
    eEvent_FACTORY_RESET_STOP,  //LOW POWER FAIL
    eEvent_Dummy
};
#endif
typedef enum
{
    eDEVICE_CHARGE_IN = 0,
    eDEVICE_CHARGE_OUT
}eDEV_CHARGE_STATE_t;

typedef enum
{
    eDEVICE_CHARGE_NOFULL = 0,
    eDEVICE_CHARGE_FULL
}eDEV_CHARGEFULL_STATE_t;

typedef enum
{
    eSWIM_25M = 0,
    eSWIM_50M,
    eSWIM_25YD,
    eSWIM_33_33M,
    eSWIM_33_33YD,
    eeSWIM_UNKNOWN
}eSWIM_LEN_SET_t;

typedef enum
{
    eMMI_STANDBY_PAGE =0,
    eMMI_CLOCK_PAGE = 1,
    eMMI_PEDO_PAGE,
    eMMI_HRM_PAGE,
    eMMI_BATTERY_PAGE, //4//4
    eMMI_DISTANCE_PAGE,
    eMMI_CAL_PAGE,
    eMMI_SWINMING_PAGE,
    eMMI_DEVICE_INFO_PAGE,
    eMMI_DUMMY_PAGE,
    eMMI_CHARGING_IN_PAGE = 10,
    eMMI_CHARGING_IN_FULL,
    eMMI_HRMDATA_PAGE,
    eMMI_HRMTIMEOUT_PAGE,
    eMMI_HRM_HRS_INIT_PAGE,
    eMMI_HRM_HRS_ACTIVATED_PAGE,
    eMMI_HRM_HRS_DATA_PAGE,
    eMMI_HRM_HRS_DEACTIVATED_PAGE,
    eMMI_LONGSIT_PAGE,
    eMMI_INCOMMING_CALL_PAGE,
    eMMI_INCOMMING_SMS_PAGE,
    eMMI_ALARM_PAGE,
    eMMI_SWIMMING_ON_PAGE,
    eMMI_SWIMMING_OFF_PAGE,
    eMMI_SWIMMING_CONFIRM_ON_PAGE,
    eMMI_SWIMMING_CONFIRM_OFF_PAGE,
    eMMI_PRE_LOWPOWER,
    eMMI_LOWPOWER,
    eMMI_DB_FULL,
    eMMI_HR_WARNING_HIGH,
    eMMI_HR_WARNING_LOW,
    eMMI_FACTORY_RESET_START,
    eMMI_FACTORY_RESET_DONE,
    eMMI_FACTORY_RESET_STOP,
    eMMI_PAIR_PASSKEY,
    eMMI_PAIR_SUCCESS,
    eMMI_PAIR_FAIL,
#ifdef PED_GOAL
    eMMI_PED_GOAL_ACHIEVE,
    eMMI_SWIMMING_GOAL_ACHIEVE,
#endif
    eMMI_LIFTARM_DOWN_OLEDOFF,
    eMMI_DUMMY_END
}eMMI_Page_t;

typedef enum
{
    eHRMCLOSE_ID_TOUCH=0,
    eHRMCLOSE_ID_GENERALTIMEOUT,
    eHRMCLOSE_ID_DUMMY
}eHrm_Close_EventID;

typedef enum
{
    eHRM_Off=0,
    eHRM_On,
    eHRM_Invaild
}eHrmOp_State;


typedef enum
{
    eStateOff=0,
    eStateOn,
    eStateInvaild
}eCommon_State;


typedef enum
{
    eDisable=0,
    eEnable,
    eInvaild,
}eStete_t;


typedef enum
{
    eOne=1,
    eDayofChange
}eClear_DB_Setting_t;


typedef enum
{
    eSwimCalProcStop=0,
    eSwimCalProcStart =1,
    eSwimCalProcInvaild,
}eSwim_Cal_ProcState_t;

typedef enum
{
    eSwimCalInit=1,
    eSwimCalProc,
    eSwimCalRetry,
    eSwimCalExit,
    eSwimCalInvaild,
}eSwim_Cal_State_t;

typedef enum
{
    eLongsitNoResult = 0,
    eLongsitNoWear =1,
    eLongsitWearOnHand,
}eLongsit_Wear_State_t;


typedef enum
{
    ePedo_Stop = 0x00,
    ePedo_Walk = 0x01,
    ePedo_Run  = 0x02,
}ePedo_State_t;

typedef struct
{
    uint8_t    bCount;
    uint8_t    baPadding[2];
    uint32_t  dwData;
}   S_VenusEvent;

typedef enum
{
    eLiftarm_None =0,
    eLiftarm_Up = 1,
    eLiftarm_Down,
}eLiftarm_Mode;

#define CHARGE_MAX_USERS 8
typedef void (*charge_cb_t)(eDEV_CHARGE_STATE_t eState);

#if 1


#if 0

typedef struct
{
   int16_t    Data[3];
   uint16_t     wVaildFlag;
}db_sys_static_gyro_offset_t __attribute__((aligned(4)));


typedef struct
{
    uint8_t start_time_hour;
    uint8_t start_time_min;
    uint8_t end_time_hour;
    uint8_t end_time_min;
}db_sys_sleep_monitor_t __attribute__((aligned(4))); // from cc_db_structrure.h


typedef struct
{
    uint16_t count;
    uint8_t rsvd[2];
}db_sleep_record_count_t __attribute__((aligned(4)));



typedef struct
{
    uint8_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t min;
    uint8_t sec;
    uint8_t sleep_state;
    uint8_t rsvd[1];
}db_sleep_time_t __attribute__((aligned(4)));


typedef struct
{
    uint32_t period_second;
}db_sleep_time_period_t __attribute__((aligned(4)));


typedef struct
{
    db_sleep_time_t         detect_time;
    db_sleep_time_period_t  period;
}db_sleep_t __attribute__((aligned(4)));
#endif

#if 1 // from venus: ble_rscs.h

typedef struct
{
   uint8_t     command;
   uint8_t     is_update_steps;
   uint32_t    dwTotal_steps;
   uint32_t    dwTotal_calorie;

}CC_Ble_Ped_Info_T;

typedef struct
{
   uint8_t     command;
   uint8_t     is_update_hrm;  //bit 0: Ped = 0xF1, Hrm = 0xF2 Sleep = 0xF3
   uint16_t    hrmdata;
}CC_Ble_Hrm_Info_T;

typedef struct
{
   uint8_t     command;
   uint8_t     is_Swim_En;  //bit 0: Ped = 0xF1, Hrm = 0xF2 Sleep = 0xF3  Swim = 0xF4
   uint8_t     style_type;
   uint32_t   dwSwimCnt;
   uint32_t     cSwimLap;
   unsigned long   dwTimestamp;
}CC_Ble_Swim_Info_T;

typedef enum {
    CC_LIB_GENDER_MALE =0,
    CC_LIB_GENDER_FEMALE = 1,
} CC_Gender_t;

typedef struct
{
    uint8_t cHeight;
    uint8_t cWeight;
    uint8_t cAge;
    CC_Gender_t cGender; // 0: Man, 1: Female
    uint8_t cStride_Lenght;
    eSWIM_LEN_SET_t cSwim_Pool_Size;
    uint8_t bBandLocation;

    uint8_t bRestingHrMax;
    uint8_t bRestingHrMin;
    uint8_t bExerciseHrMax;
    uint8_t bExerciseHrMin;
    uint8_t BRsv;

}CC_Ble_General_Info_T;


typedef struct
{
    uint8_t cUnitLength;
    uint8_t cUnitWeight;
    uint16_t rsvd;
}CC_Ble_Unit_Info_T;


typedef enum
{
    eCALLDISABLE =0,
    eCALLENABLE,
    eCALLINCOMMING,
    eCALLINCOMMINGOFF,
    eCALLUNKNOWN
}eCALL_state_t;


typedef enum
{
    eSMSDISABLE =0,
    eSMSENABLE,
    eSMSCOMMING,
    eSMSUNKNOWN
}eSMS_state_t;

typedef enum
{
    eLONGSITDISABLE =0,
    eLONGSITENABLE,
    eLONGSITUNKNOWN
}eLONGSIT_state_t;

typedef enum
{
    eLIFTARMDISABLE =0,
    eLIFTARMENABLE,
    eLIFTARMUNKNOWN
}eLIFTARM_state_t;

typedef struct
{
    uint8_t cIndex;
    uint8_t cHour;
    uint8_t cMinute;
    uint8_t cSetting;

}CC_Ble_Clock_Set_T;

typedef struct
{
    uint8_t cTotalNum;
    CC_Ble_Clock_Set_T cAlarmTime[4];
}CC_Ble_Clock_Alarm_T;




#if 0
typedef struct
{
    eStete_t eIsHrsEnabled;
    eStete_t eIs24HrEnabled;

    uint32_t dw24HrPeriodicMeasurementTime;    // ms
    uint32_t dw24HrOneMeasurementMaxTime;      // ms
}db_sys_hr_setting_t  __attribute__((aligned(4)));



typedef struct
{
    uint8_t incomming_call_en;
    uint8_t incomming_sms_en;
    uint8_t longsit_en;
    uint8_t lifearm_en;
}db_sys_notify_enabled_t __attribute__((aligned(4)));
#endif


#endif

#endif


/******************************************************************************
// Sec 12: BLE config settings
******************************************************************************/

//BLE is always supported
#define CFG_BLE    1

// <h> BLE - DEVICE NAME
#define APP_DFLT_DEVICE_NAME    "CC6801-77"

// <h> BLE - DEVICE ADDRESS
#define APP_DFLT_DEVICE_ADDR    { 0xAB, 0x36, 0x61, 0x45, 0x23, 0x01 }

// <h> BLE - Data Length Extension (v4.2)
#define CFG_DLE_EN

// <h> BLE IP ARCH CONFIGURATION
#define CFG_BLE_APP

//==========================================================
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

#define CFG_CON    8
#define CFG_SLEEP

#define CFG_CHNL_ASSESS

#define CFG_AHITL
#define CFG_HCITL
//#define CFG_NVDS
#define CFG_HW_AUDIO

#define CFG_SEC_CON
#define CFG_APP_SEC

#define CFG_ATTC
#define CFG_ATTS

#define CFG_RF_ATLAS

#define CFG_PRF
#define CFG_NB_PRF    5
#define CFG_PRF_HTPT
#define CFG_PRF_BASS
#define CFG_PRF_DISS
#define CFG_PRF_OTA
#define CFG_PRF_CCPS

#define CFG_APP
#define CFG_APP_HT
#define BLE_APP_BATT  1
#define CFG_APP_DIS    1
#define CFG_APP_OTA
#define CFG_APP_CCPS


#define CFG_EXT_DB

#endif // _PROJECT_ZEUS_EVB_H_

