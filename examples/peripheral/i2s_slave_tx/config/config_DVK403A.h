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
#include "CC_Calorie_burn.h"
#include "util_calendar.h"

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

#define MODULE_OLED             OLED_JDI_LPM013M126A
#define LCD_JDI_DISP_PIN        GPIO_PIN_43
#define LCD_JDI_EXTCOM_PIN      GPIO_PIN_21
#define LCD_JDI_PWR_EN          GPIO_PIN_20
#define LCD_JDI_BK_PIN          GPIO_PIN_1
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
*   Choose Touch model and config Touch interface
*   (pick one Touch sensor from module_supported.h)
*   (please select TOUCH_NULL if Touch sensor is not in use)
*
*   TOUCH_IF : the interface assigned to Touch sensor
*
*   TOUCH_IF_TYPE / TOUCH_IF_ID will be generated automatically.
*   TOUCH_IF_TYPE : the Touch interface type
*   TOUCH_IF_ID   : the Touch interface id
***************************************************/
#define MODULE_TOUCH            TOUCH_FOCAL_FT6X36
#define TOUCH_IF                UseInterface(I2C,0)
#define TOUCH2D_INT_PIN         GPIO_PIN_4
#define TOUCH_FT6X36_SLAVE_ADDR (0x38)
#define TOUCH_SUPPORTED_FINGERS (1)

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

/**************************************************
*   Choose FLASH and config FLASH interface
*   (pick one FLASH from module_supported.h)
*   (please select FLASH_NULL if FLASH is not in use)
*
*   FLASH_IF :  the interface assigned to FLASH
*
*   FLASH_IF_TYPE / FLASH_IF_ID will be generated automatically.
*   FLASH_IF_TYPE : the FLASH interface type
*   FLASH_IF_ID   : the FLASH interface id
***************************************************/
#define MODULE_FLASH              FLASH_W25Q256JV//FLASH_W25Q80DV
#define FLASH_IF                  UseInterface(SPI,0)

/**************************************************
*   Config PRESSURE interface
*
*   PRESSURE_IF : the interface assigned to PRESSURE
*
*   PRESSURE_IF_TYPE / ADC_IF_ID will be generated automatically.
*   PRESSURE_IF_TYPE : the ADC interface type
*   PRESSURE_IF_ID   : the ADC interface id
***************************************************/
#define MODULE_PRESSURE              PRESSURE_LPS33HW
#define PRESSURE_IF                  UseInterface(I2C,1)
//#define PRESSURE_INT_PIN             38


/**************************************************
*   Config VIBRATOR interface
*
*   BACKLIGHT_IF : the interface assigned to SWT
*
*   VIBRATOR_IF_TYPE / VIBRATOR_IF_ID will be generated automatically.
*   VIBRATOR_IF_TYPE : the SWT interface type
*   VIBRATOR_IF_ID   : the SWT interface id
***************************************************/
#define BACKLIGHT_IF             UseInterface(PWM,1)


/**************************************************
*   Config GPS interface
*
*   GPS_IF : the interface assigned to HCI
*
*   GPS_IF_TYPE / GPS_IF_ID will be generated automatically.
*   GPS_IF_TYPE : the GPS interface type
*   GPS_IF_ID   : the GPS interface id
***************************************************/
#define GPS_IF                  UseInterface(UART,2)


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
#define GPIO1_CONFIG                         ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_LOW))
#define GPIO2_CONFIG                         ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_INPUT)  | (GPIO_OUTPUT_LOW))
#define GPIO3_CONFIG                         ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_INPUT)  | (GPIO_OUTPUT_LOW))

// GPIO Pin Group1
#define GPIO_MODE_PINGROUP1                  GPIO_MODE_GPIO
#define GPIO4_CONFIG                         ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_INPUT)  | (GPIO_INPUT_NOPULL))
#define GPIO5_CONFIG                         ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_INPUT)  | (GPIO_INPUT_NOPULL))
#define GPIO6_CONFIG                         ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_INPUT)  | (GPIO_INPUT_NOPULL))
#define GPIO7_CONFIG                         ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_INPUT)  | (GPIO_INPUT_NOPULL))

// GPIO Pin Group2
#define GPIO_MODE_PINGROUP2                  GPIO_MODE_GPIO
#define GPIO8_CONFIG                         ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_INPUT)  | (GPIO_INPUT_NOPULL))
#define GPIO9_CONFIG                         ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_INPUT)  | (GPIO_INPUT_NOPULL))
#define GPIO10_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_LOW))
#define GPIO11_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_INPUT)  | (GPIO_INPUT_NOPULL))

// GPIO Pin Group3
#define GPIO_MODE_PINGROUP3                  GPIO_MODE_I2C_GPIO
#define GPIO12_CONFIG                        ((GPIO_PULL_UP)   | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_INPUT)  | (GPIO_INPUT_NOPULL))
#define GPIO13_CONFIG                        ((GPIO_PULL_UP)   | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_INPUT)  | (GPIO_INPUT_NOPULL))
#define GPIO14_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_INPUT)  | (GPIO_INPUT_NOPULL))
#define GPIO15_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_LOW))

// GPIO Pin Group4
#define GPIO_MODE_PINGROUP4                  GPIO_MODE_SPI_4WIRE
#define GPIO16_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO17_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO18_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO19_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_INPUT)  | (GPIO_INPUT_NOPULL))

// GPIO Pin Group5
#define GPIO_MODE_PINGROUP5                  GPIO_MODE_SPI_3WIRE
#define GPIO20_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO21_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO22_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO23_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_LOW))

// GPIO Pin Group6
#define GPIO_MODE_PINGROUP6                  GPIO_MODE_SPI_4WIRE
#define GPIO24_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO25_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO26_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO27_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_INPUT)  | (GPIO_INPUT_NOPULL))

// GPIO Pin Group7
#define GPIO_MODE_PINGROUP7                  GPIO_MODE_GPIO
#define GPIO28_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_INPUT)  | (GPIO_INPUT_NOPULL))
#define GPIO29_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_INPUT)  | (GPIO_INPUT_NOPULL))
#define GPIO30_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_INPUT)  | (GPIO_INPUT_NOPULL))
#define GPIO31_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_INPUT)  | (GPIO_INPUT_NOPULL))

// GPIO Pin Group8
#define GPIO_MODE_PINGROUP8                  GPIO_MODE_GPIO
#define GPIO32_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO33_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO34_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_INPUT) | (GPIO_INPUT_NOPULL))
#define GPIO35_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_INPUT)  | (GPIO_INPUT_NOPULL))

// GPIO Pin Group9
#define GPIO_MODE_PINGROUP9                  GPIO_MODE_UART
#define GPIO36_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_LOW))
#define GPIO37_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_LOW))
#define GPIO38_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_INPUT)  | (GPIO_INPUT_NOPULL))
#define GPIO39_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_INPUT)  | (GPIO_INPUT_NOPULL))

// GPIO Pin Group10
#define GPIO_MODE_PINGROUP10                 GPIO_MODE_GPIO_I2C
#define GPIO40_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_OUTPUT)  | (GPIO_OUTPUT_LOW))
#define GPIO41_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_DISABLE) | (GPIO_DIR_OUTPUT)  | (GPIO_OUTPUT_LOW))
#define GPIO42_CONFIG                        ((GPIO_PULL_UP)   | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_INPUT)  | (GPIO_INPUT_NOPULL))
#define GPIO43_CONFIG                        ((GPIO_PULL_UP)   | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_INPUT)  | (GPIO_INPUT_NOPULL))

// GPIO Pin Group11
#define GPIO_MODE_PINGROUP11                 GPIO_MODE_UART
#define GPIO44_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO45_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_OUTPUT) | (GPIO_OUTPUT_HIGH))
#define GPIO46_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_INPUT)  | (GPIO_INPUT_NOPULL))
#define GPIO47_CONFIG                        ((GPIO_PULL_DOWN) | (GPIO_PINMUX_ENABLE)  | (GPIO_DIR_INPUT)  | (GPIO_INPUT_NOPULL))


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


//TODO: should seperate two configuration files: one for BLE, the other for APP

/**************************/
/*** CONFIG > SCHEDULER ***/
/**************************/
#define CC_SCHEDULER    (1)
#define FREE_RTOS       (2)

#define CC_CONFIG_SCHEDULER    FREE_RTOS
#define CFG_RTOS_TICKLESS_LCLOCK_EN

/**************************/
/*** CONFIG > XXXXXXXXX ***/
/**************************/
#define FSTORAGE_ENABLED     1
#define FDS_ENABLED          1


/**************************/
/*** CONFIG > BLE Sleep ***/
/**************************/
#define CFG_BLE_BB_OFF
#define CFG_TOUCH_POLL_MODE

// Sec 9: declaration of static function prototype

/******************************************************************************
// Sec 10: C Functions
******************************************************************************/






/******************************************************************************
// Sec 11: Application settings
******************************************************************************/




#define DEVICE_MODEL "CC_WATCH"
#define SDK_FW_VERSION "v1.000.002"

// EMWIN AND JDI
#define EMWIN_ENABLE
#define EMWIN_LOAD_EXT_FLASH            1
#define SYS_DO_STRESS_TEST              0
#ifdef EMWIN_ENABLE
#define JDI_DRAW_WIHTTIMER
//#define EN_PAH80211ES
#endif

#define SLEEP_EN
#define SWIMMING_EN

//#define LONGSIT_EN
//#define PEDO_EN
//#define HRM_EN
//#define BLE_OTA_BL_MODE_EN
#define nFS_EN
#define nDB_EN
#define FS_CC_MODE_EN
#define FIFO_MODE_EN
#define FACTORY_RESET

#define APP_VIB_MGR


#ifdef EN_PAH80211ES
#define HRM_INT_PIN         GPIO_PIN_14 //9
#else
#define HRM_INT_PIN         GPIO_PIN_9
#endif
#define HRM_RST_PIN         GPIO_PIN_15
#define HRM_PWR_EN          GPIO_PIN_32

#if (MODULE_OLED == OLED_SOLOMON_SH1107)
#define OLED_PWR_PIN        GPIO_PIN_41
#define OLED_RST_PIN        GPIO_PIN_23
#define OLED_DC_PIN         GPIO_PIN_40
#endif

#define SWAP_ACC_DIRECTION_EN

#define FORCE_LIFTARM_TEST_EN   // for test
//#define FORCE_SWIM_TEST_EN   // for test
//#define FORCE_HRS_TEST_EN   // for test

#define SENSOR_MGR_EN
#ifdef SENSOR_MGR_EN
//#define SM_TEST_EN
//#define SVC_MGR_TEST_EN
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

#endif

#endif


/******************************************************************************
// Sec 12: BLE config settings
******************************************************************************/

//BLE is always supported
#define CFG_BLE    1

// <h> BLE - DEVICE NAME
#define APP_DFLT_DEVICE_NAME    "DVK-B0-ZEUS"

// <h> BLE - DEVICE ADDRESS
#define APP_DFLT_DEVICE_ADDR    { 0x55, 0xBB, 0xAA, 0x79, 0x23, 0x01 }

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

//#define CFG_AHITL
//#define CFG_HCITL
#define CFG_NVDS
#define CFG_HW_AUDIO

//#define CFG_SEC_CON
//#define CFG_APP_SEC

//#define CFG_ATTC
#define CFG_ATTS

#define CFG_RF_ATLAS

#define CFG_PRF
#define CFG_NB_PRF    1
#define CFG_PRF_CCPS

#define CFG_APP
#define CFG_APP_CCPS   1
#endif // _CONFIG_EVB_H_
