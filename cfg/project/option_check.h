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

#ifndef _OPTION_CHECK_H_
#define _OPTION_CHECK_H_

#include "ARMCM.h"

//This file is an example about how to add options check.
//It will be deleted after everything is done

//Supported XTAL and CLOCK should be defined by ASIC
//Consider moving these options check to cc6801_options.h
#ifdef XTAL_MHZ
    #if (XTAL_MHZ != 48)
        #error  "Currently, we only support 48MHz XTAL as SOC clock source"
    #endif
#else
    #error "You must define XTAL_MHZ in configuration file."
#endif

#ifndef SYSTEM_CLOCK_MHZ
    #error  "You must define SYSTEM_CLOCK_MHZ in configuration file"
#endif

#ifndef USE_PLL
    #error  "You must define USE_PLL to be TRUE or FALSE in configuration file"
#else
    #if (USE_PLL == FALSE)
        #if   (SYSTEM_CLOCK_MHZ == 32)
            #error  "PLL must be enabled for 32MHz. Pleaes set USE_PLL = TRUE"
        #elif (SYSTEM_CLOCK_MHZ == 64)
            #error  "PLL must be enabled for 64MHz. Pleaes set USE_PLL = TRUE"
        #elif (SYSTEM_CLOCK_MHZ == 96)
            #error  "PLL must be enabled for 96MHz. Pleaes set USE_PLL = TRUE"
        #endif
    #endif
#endif


#ifdef MODULE_ACC
  /*
    define ACC_IF_TYPE and ACC_IF_ID before including module driver header
    or module driver won't be able to use the definitions
  */
  #if ((MODULE_ACC & 0xF000) == 0x1000)
    #if (MODULE_ACC == ACC_ST_LSM303C)
      #define ACC_HEADER acc_STLSM303C

    #elif (MODULE_ACC == ACC_ST_LSM303D)
      #define ACC_HEADER acc_STLSM303D
      
    #elif (MODULE_ACC == ACC_ST_LIS2DH12)
      #define ACC_HEADER acc_STLIS2DH12
      
    #elif (MODULE_ACC == ACC_ST_LIS2DW12)
      #define ACC_HEADER acc_STLIS2DW12
      
    #elif (MODULE_ACC == ACC_KIX_KX022_1020)
      #define ACC_HEADER acc_KIXKX022
      
    #elif (MODULE_ACC == ACC_KIX_KXG03)
      #define ACC_HEADER acc_KIXKXG03
      
    #elif (MODULE_ACC == ACC_AD_ADXL362)
      #define ACC_HEADER acc_ADXL362
      
    #elif (MODULE_ACC == ACC_ST_LSM6DSL)
      #define ACC_HEADER acc_lsm6ds3

    #elif (MODULE_ACC == ACC_NULL)
      #define ACC_HEADER acc_null
      #undef ACC_IF
      #define ACC_IF IF_NULL
    #else
      #error " no matched ACC driver found"
    #endif
  #else
    #error "MODULE_ACC is not a ACC driver"
  #endif
  
  #if ((defined ACC_IF) && (ACC_IF))
    #define ACC_IF_TYPE (ACC_IF&0xF0)
    #define ACC_IF_ID   (ACC_IF&0x0F)
  #else
    #error "please assign ACC interface"
  #endif

  #ifndef ACC_INT_PIN
    #define ACC_INT_PIN 0xFF
  #endif
#else
  #define MODULE_ACC    ACC_NULL
  #define ACC_HEADER    acc_null
  #define ACC_IF        IF_NULL
  #define ACC_INT_PIN   0xFF
#endif



#ifdef MODULE_MAG
  /*
    define MAG_IF_TYPE and MAG_IF_ID before including module driver header
    or module driver won't be able to use the definitions
  */
  #if ((MODULE_MAG & 0xF000) == 0x2000)
    #if (MODULE_MAG == MAG_AKM_AK09912C)
      #define MAG_HEADER mag_ak09912

    #elif (MODULE_MAG == MAG_AKM_AK09915C)
      #define MAG_HEADER mag_AK09915
      
    #elif (MODULE_MAG == MAG_ST_LSM303C)
      #define MAG_HEADER mag_STLSM303C
      
    #elif (MODULE_MAG == MAG_ST_LSM303D)
      #define MAG_HEADER mag_STLSM303D
      
    #elif (MODULE_MAG == MAG_ST_LIS2MDL)
      #define MAG_HEADER mag_ST_LIS2MDL
      
    #elif (MODULE_MAG == MAG_NULL)
      #define MAG_HEADER mag_null
      #undef MAG_IF
      #define MAG_IF IF_NULL
    #else
      #error " no matched MAG driver found"
    #endif
  #else
    #error "not a MAG driver"
  #endif

  #if ((defined MAG_IF) && (MAG_IF))
    #define MAG_IF_TYPE (MAG_IF&0xF0)
    #define MAG_IF_ID   (MAG_IF&0x0F)
  #else
    #error "please assign MAG interface"
  #endif

  #ifndef MAG_INT_PIN
    #define MAG_INT_PIN 0xFF
  #endif
#else
  #define MODULE_MAG    MAG_NULL
  #define MAG_HEADER    mag_null
  #define MAG_IF        IF_NULL
  #define MAG_INT_PIN   0xFF
#endif


#if defined(MODULE_PPG)
  /*
    define PPG_IF_TYPE and MAG_IF_ID before including module driver header
    or module driver won't be able to use the definitions
  */
  #if ((MODULE_PPG & 0xF000) == 0x5000)
    #if (MODULE_PPG == PPG_PXT_PAH8002)
      #define PPG_HEADER ppg_pah8002
      
    #elif (MODULE_PPG == PPG_NULL)
      #define PPG_HEADER mag_null
      #undef PPG_IF
      #define PPG_IF IF_NULL
    #else
      #error " no matched PPG driver found"
    #endif
  #else
    #error "not a PPG driver"
  #endif

  #if ((defined PPG_IF) && (PPG_IF))
    #define PPG_IF_TYPE (PPG_IF&0xF0)
    #define PPG_IF_ID   (PPG_IF&0x0F)
  #else
    #error "please assign PPG interface"
  #endif


  #ifndef PPG_INT_PIN
    #define PPG_INT_PIN 0xFF
  #endif
#else
  #define MODULE_PPG PPG_NULL
  #define PPG_HEADER ppg_null
  #define PPG_IF IF_NULL
  #define PPG_INT_PIN 0xFF
#endif


#if defined(MODULE_GYR)
  /*
    define GYR_IF_TYPE and GYR_IF_ID before including module driver header
    or module driver won't be able to use the definitions
  */
  #if ((MODULE_GYR & 0xF000) == 0x3000)
    #if (MODULE_GYR == GYR_ST_LSM6DSL)
      #define GYR_HEADER gyr_LSM6DS3

    #elif (MODULE_GYR == GYR_NULL)
      #define GYR_HEADER gyr_null
      #undef GYR_IF
      #define GYR_IF IF_NULL
    #else
      #error " no matched GYR driver found"
    #endif
  #else
    #error "not a GYR driver"
  #endif

  #if ((defined GYR_IF) && (GYR_IF))
    #define GYR_IF_TYPE (GYR_IF&0xF0)
    #define GYR_IF_ID   (GYR_IF&0x0F)
  #else
    #error "please assign GYR interface"
  #endif

  #ifndef GYR_INT_PIN
    #define GYR_INT_PIN 0xFF
  #endif
#else
  #define MODULE_GYR    GYR_NULL
  #define GYR_HEADER    gyr_null
  #define GYR_IF        IF_NULL
  #define GYR_INT_PIN   0xFF
#endif


#if defined(MODULE_OLED)
  /*
    define OLED_IF_TYPE and OLED_IF_ID before including module driver header
    or module driver won't be able to use the definitions
  */
  #if ((MODULE_OLED & 0xF000) == 0x4000)
    #if (MODULE_OLED == OLED_SOLOMON_SSD1306)
      #define OLED_HEADER ssd1306

    #elif (MODULE_OLED == OLED_SOLOMON_SH1107)
      #define OLED_HEADER sh1107

    #elif (MODULE_OLED == OLED_JDI_LPM013M126A)
      #define OLED_HEADER JDI_LPM013MXXXX

    #elif (MODULE_OLED == OLED_JDI_LPM010M297B)
      #define OLED_HEADER JDI_LPM013MXXXX

    #elif (MODULE_OLED == OLED_NULL)
      #define OLED_HEADER oled_null
      #undef OLED_IF
      #define OLED_IF IF_NULL
    #else
      #error " no matched OLED driver found"
    #endif
  #else
    #error "not a OLED driver"
  #endif

  #if ((defined OLED_IF) && (OLED_IF))
    #define OLED_IF_TYPE (OLED_IF&0xF0)
    #define OLED_IF_ID   (OLED_IF&0x0F)
  #else
    #error "please assign OLED interface"
  #endif

#else
  #define MODULE_OLED OLED_NULL
  #define OLED_HEADER oled_null
  #define OLED_IF IF_NULL
#endif



#if defined(MODULE_PRESSURE)
  /*
    define PRESSURE_IF_TYPE and PRESSURE_IF_ID before including module driver header
    or module driver won't be able to use the definitions
  */
  #if ((MODULE_PRESSURE & 0xF000) == 0x7000)
    #if (MODULE_PRESSURE == PRESSURE_LPS33HW)
      #define PRESSURE_HEADER pressure_lps33hw

    #elif (MODULE_PRESSURE == PRESSURE_NULL)
      #define PRESSURE_HEADER pressure_null
      #undef PRESSURE_IF
      #define PRESSURE_IF IF_NULL
    #else
      #error " no matched PRESSURE driver found"
    #endif
  #else
    #error "not a PRESSURE driver"
  #endif

  #if ((defined PRESSURE_IF) && (PRESSURE_IF))
    #define PRESSURE_IF_TYPE (PRESSURE_IF&0xF0)
    #define PRESSURE_IF_ID   (PRESSURE_IF&0x0F)
  #else
    #error "please assign PRESSURE interface"
  #endif

#else
  #define MODULE_PRESSURE PRESSURE_NULL
  #define PRESSURE_HEADER pressure_null
  #define PRESSURE_IF IF_NULL
#endif

#if defined(MODULE_TOUCH)
  /*
    define TOUCH_IF_TYPE and TOUCH_IF_ID before including module driver header
    or module driver won't be able to use the definitions
  */
  #if ((MODULE_TOUCH & 0xF000) == 0x8000)
    #if (MODULE_TOUCH == TOUCH_FOCAL_FT6X36)
      #define TOUCH_HEADER touch_ft6x36

    #elif (MODULE_TOUCH == TOUCH_NULL)
      #define TOUCH_HEADER touch_null
      #undef TOUCH_IF
      #define TOUCH_IF IF_NULL
    #else
      #error " no matched TOUCH driver found"
    #endif
  #else
    #error "not a TOUCH driver"
  #endif

  #if ((defined TOUCH_IF) && (TOUCH_IF))
    #define TOUCH_IF_TYPE (TOUCH_IF&0xF0)
    #define TOUCH_IF_ID   (TOUCH_IF&0x0F)
  #else
    #error "please assign TOUCH interface"
  #endif

#else
  #define MODULE_TOUCH TOUCH_NULL
  #define TOUCH_HEADER touch_null
  #define TOUCH_IF IF_NULL
#endif


#if ((defined TRACER_IF) && (TRACER_IF))
  #define TRACER_IF_TYPE (TRACER_IF&0xF0)
  #define TRACER_IF_ID   (TRACER_IF&0x0F)
#else
  #define TRACER_IF IF_NULL
#endif

#if ((defined HCI_IF) && (HCI_IF))
  #define HCI_IF_TYPE (HCI_IF&0xF0)
  #define HCI_IF_ID   (HCI_IF&0x0F)
#else
  #define HCI_IF IF_NULL
#endif

#if ((defined GPS_IF) && (GPS_IF))
  #define GPS_IF_TYPE (GPS_IF&0xF0)
  #define GPS_IF_ID   (GPS_IF&0x0F)
#else
  #define GPS_IF IF_NULL
#endif


#if ((defined SWT_IF) && (SWT_IF))
  #define SWT_IF_TYPE (SWT_IF&0xF0)
  #define SWT_IF_ID   (SWT_IF&0x0F)
#else
  #define SWT_IF IF_NULL
#endif

#if ((defined VIBRATOR_IF) && (VIBRATOR_IF))
  #define VIBRATOR_IF_TYPE (VIBRATOR_IF&0xF0)
  #define VIBRATOR_IF_ID   (VIBRATOR_IF&0x0F)
#else
  #define VIBRATOR_IF IF_NULL
#endif

#if ((defined ADC_IF) && (ADC_IF))
  #define ADC_IF_TYPE (ADC_IF&0xF0)
  #define ADC_IF_ID   (ADC_IF&0x0F)
#else
  #define ADC_IF IF_NULL
#endif

#if ((defined AUDIO_IF) && (AUDIO_IF))
  #define AUDIO_IF_TYPE (AUDIO_IF&0xF0)
  #define AUDIO_IF_ID   (AUDIO_IF&0x0F)
#else
  #define AUDIO_IF IF_NULL
#endif

#if ((defined FLASH_IF) && (FLASH_IF))
  #define FLASH_IF_TYPE (FLASH_IF&0xF0)
  #define FLASH_IF_ID   (FLASH_IF&0x0F)
#else
  #define FLASH_IF IF_NULL
#endif

#if defined(MODULE_TEST)
  #if ((defined TEST_I2C0_IF) && (TEST_I2C0_IF))
    #define TEST_I2C0_IF_TYPE (TEST_I2C0_IF&0xF0)
    #define TEST_I2C0_IF_ID   (TEST_I2C0_IF&0x0F)
  #endif
  #if ((defined TEST_I2C1_IF) && (TEST_I2C1_IF))
    #define TEST_I2C1_IF_TYPE (TEST_I2C1_IF&0xF0)
    #define TEST_I2C1_IF_ID   (TEST_I2C1_IF&0x0F)
  #endif
  #if ((defined TEST_UART0_IF) && (TEST_UART0_IF))
    #define TEST_UART0_IF_TYPE (TEST_UART0_IF&0xF0)
    #define TEST_UART0_IF_ID   (TEST_UART0_IF&0x0F)
  #endif
  #if ((defined TEST_UART1_IF) && (TEST_UART1_IF))
    #define TEST_UART1_IF_TYPE (TEST_UART1_IF&0xF0)
    #define TEST_UART1_IF_ID   (TEST_UART1_IF&0x0F)
  #endif
  #if ((defined TEST_UART2_IF) && (TEST_UART2_IF))
    #define TEST_UART2_IF_TYPE (TEST_UART2_IF&0xF0)
    #define TEST_UART2_IF_ID   (TEST_UART2_IF&0x0F)
  #endif
#endif

//check which interface is used
#if ((ACC_IF==Interface_SPI0) || (MAG_IF==Interface_SPI0) || (GYR_IF==Interface_SPI0) || (OLED_IF==Interface_SPI0) || (FLASH_IF==Interface_SPI0))
  #define SPI0_INUSE  TRUE
#endif
#if ((ACC_IF==Interface_SPI1) || (MAG_IF==Interface_SPI1) || (GYR_IF==Interface_SPI1) || (OLED_IF==Interface_SPI1) || (FLASH_IF==Interface_SPI1))
  #define SPI1_INUSE  TRUE
#endif
#if ((ACC_IF==Interface_SPI2) || (MAG_IF==Interface_SPI2) || (GYR_IF==Interface_SPI2) || (OLED_IF==Interface_SPI2) || (FLASH_IF==Interface_SPI2))
  #define SPI2_INUSE  TRUE
#endif
#if ((ACC_IF==Interface_I2C0) || (MAG_IF==Interface_I2C0) || (GYR_IF==Interface_I2C0) || (OLED_IF==Interface_I2C0) || (PPG_IF==Interface_I2C0) || (ADC_IF==Interface_I2C0) || (AUDIO_IF==Interface_I2C0) || (TOUCH_IF == Interface_I2C0) ||(TEST_I2C0_IF==Interface_I2C0))
  #define I2C0_INUSE  TRUE
#endif
#if ((ACC_IF==Interface_I2C1) || (MAG_IF==Interface_I2C1) || (GYR_IF==Interface_I2C1) || (OLED_IF==Interface_I2C1) || (PPG_IF==Interface_I2C1) || (ADC_IF==Interface_I2C1) || (AUDIO_IF==Interface_I2C1) || (TOUCH_IF == Interface_I2C1) ||(TEST_I2C1_IF==Interface_I2C1))
  #define I2C1_INUSE  TRUE
#endif
#if ((MAG_IF==Interface_SW_I2C))
  #define SW_I2C_INUSE  TRUE
#endif
#if ((TRACER_IF==Interface_UART0) || (HCI_IF==Interface_UART0) || (TEST_UART0_IF==Interface_UART0) || (GPS_IF==Interface_UART0))
  #define UART0_INUSE TRUE
#endif
#if ((TRACER_IF==Interface_UART1) || (HCI_IF==Interface_UART1) || (TEST_UART1_IF==Interface_UART1) || (GPS_IF==Interface_UART1))
  #define UART1_INUSE TRUE
#endif
#if ((TRACER_IF==Interface_UART2) || (HCI_IF==Interface_UART2) || (TEST_UART2_IF==Interface_UART2) || (GPS_IF==Interface_UART2))
  #define UART2_INUSE TRUE
#endif

#if (SWT_IF ==Interface_WKTM0)
  #define WKTM0_INUSE TRUE
  #define SWT_IF_IRQ  WKTM0_IRQn
#endif

#if (SWT_IF ==Interface_WKTM1)
  #define WKTM1_INUSE TRUE
  #define SWT_IF_IRQ  WKTM1_IRQn
#endif

#if (VIBRATOR_IF ==Interface_PWM0)
  #define PWM0_INUSE TRUE
#endif

#if (VIBRATOR_IF ==Interface_PWM1)
  #define PWM1_INUSE TRUE
#endif

/************************  SOC peripheral INUSE definition ***********************/

#if ((SPI0_INUSE) || (SPI1_INUSE) || (SPI2_INUSE))
  #define SPI_INUSE TRUE
#endif

#if ((I2C0_INUSE) || (I2C1_INUSE) || (SW_I2C_INUSE))
  #define I2C_INUSE TRUE
#endif

#if ((UART0_INUSE) || (UART1_INUSE) || (UART2_INUSE))
  #define UART_INUSE TRUE
#endif

#if ((WKTM0_INUSE) || (WKTM1_INUSE))
  #define WKTM_INUSE TRUE
#endif

#if ((PWM0_INUSE) || (PWM1_INUSE))
  #define PWM_INUSE TRUE
#endif



/************************  Include module driver header ************************
    Module driver should be included at the last. Module driver may use the 
  definitions in this doc. If header is included before "#define XXX yyy" appears,
  definition XXX will be discarded in module driver.
********************************************************************************/
#define FIND_MODULE(headername) STRINGIZE(CAT(headername, .h))

#if (MODULE_ACC != ACC_NULL)
    #include FIND_MODULE(ACC_HEADER)
#endif
#if (MODULE_MAG != MAG_NULL)
    #include FIND_MODULE(MAG_HEADER)
#endif
#if (MODULE_GYR != GYR_NULL)
    #include FIND_MODULE(GYR_HEADER)
#endif
#if (MODULE_OLED != OLED_NULL)
    #include FIND_MODULE(OLED_HEADER)
#endif
#if (MODULE_PRESSURE != PRESSURE_NULL)
    #include FIND_MODULE(PRESSURE_HEADER)
#endif
#if (MODULE_TOUCH != TOUCH_NULL)
    #include FIND_MODULE(TOUCH_HEADER)
#endif


#endif //_OPTION_CHECK_H_
