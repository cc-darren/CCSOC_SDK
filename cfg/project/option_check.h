





#ifndef _OPTION_CHECK_H_
#define _OPTION_CHECK_H_

#include "ARMCM.h"

//This file is an example about how to add options check.
//It will be deleted after everything is done


#if defined(MODULE_ACC)
  /*
    define ACC_IF_TYPE and ACC_IF_ID before including module driver header
    or module driver won't be able to use the definitions
  */
  #if ((defined ACC_IF) && (ACC_IF))
    #define ACC_IF_TYPE (ACC_IF&0xF0)
    #define ACC_IF_ID   (ACC_IF&0x0F)
  #else
    #error "ACC interface not found"
  #endif

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
    #error "not a ACC driver"
  #endif
  
#else
  #error "Please select ACC module from module_supported.h"
#endif


#if defined(MODULE_MAG)
  /*
    define MAG_IF_TYPE and MAG_IF_ID before including module driver header
    or module driver won't be able to use the definitions
  */
  #if ((defined MAG_IF) && (MAG_IF))
    #define MAG_IF_TYPE (MAG_IF&0xF0)
    #define MAG_IF_ID   (MAG_IF&0x0F)
  #else
    #error "MAG interface not found"
  #endif

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
#else
  #error "Please select MAG module from module_supported.h"
#endif


#if defined(MODULE_GYR)
  /*
    define GYR_IF_TYPE and GYR_IF_ID before including module driver header
    or module driver won't be able to use the definitions
  */
  #if ((defined GYR_IF) && (GYR_IF))
    #define GYR_IF_TYPE (GYR_IF&0xF0)
    #define GYR_IF_ID   (GYR_IF&0x0F)
  #else
    #error "GYR interface not found"
  #endif

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
#else
  #error "Please select GYR module from module_supported.h"
#endif


#if defined(MODULE_OLED)
  /*
    define OLED_IF_TYPE and OLED_IF_ID before including module driver header
    or module driver won't be able to use the definitions
  */
  #if ((defined OLED_IF) && (OLED_IF))
    #define OLED_IF_TYPE (OLED_IF&0xF0)
    #define OLED_IF_ID   (OLED_IF&0x0F)
  #else
    #error "OLED interface not found"
  #endif

  #if ((MODULE_OLED & 0xF000) == 0x4000)
    #if (MODULE_OLED == OLED_SOLOMON_SSD1306)
      #define OLED_HEADER ssd1306

    #elif (MODULE_OLED == OLED_NULL)
      #define OLED_HEADER oled_null
      #undef OLED_IF
      #define OLED_IF IF_NULL
    #else
      #error " no matched oled driver found"
    #endif
  #else
    #error "not a OLED driver"
  #endif
#else
  #error "Please select OLED module from module_supported.h"
#endif


#if ((defined TRACER_IF) && (TRACER_IF))
  #define TRACER_IF_TYPE (TRACER_IF&0xF0)
  #define TRACER_IF_ID   (TRACER_IF&0x0F)
#else
  #error "Tracer interface not found"
#endif

#if ((defined HCI_IF) && (HCI_IF))
  #define HCI_IF_TYPE (HCI_IF&0xF0)
  #define HCI_IF_ID   (HCI_IF&0x0F)
#else
  #error "HCI interface not found"
#endif

#if ((defined SWT_IF) && (SWT_IF))
  #define SWT_IF_TYPE (SWT_IF&0xF0)
  #define SWT_IF_ID   (SWT_IF&0x0F)
#else
  #error "SWT interface not found"
#endif

#if ((defined VIBRATOR_IF) && (VIBRATOR_IF))
  #define VIBRATOR_IF_TYPE (VIBRATOR_IF&0xF0)
  #define VIBRATOR_IF_ID   (VIBRATOR_IF&0x0F)
#else
  #error "VIBRATOR interface not found"
#endif

//check which interface is used
#if   ((ACC_IF==Interface_SPI0) || (MAG_IF==Interface_SPI0) || (GYR_IF==Interface_SPI0) || (OLED_IF==Interface_SPI0))
  #define SPI0_INUSE  TRUE
#endif
#if ((ACC_IF==Interface_SPI1) || (MAG_IF==Interface_SPI1) || (GYR_IF==Interface_SPI1) || (OLED_IF==Interface_SPI1))
  #define SPI1_INUSE  TRUE
#endif
#if ((ACC_IF==Interface_SPI2) || (MAG_IF==Interface_SPI2) || (GYR_IF==Interface_SPI2) || (OLED_IF==Interface_SPI2))
  #define SPI2_INUSE  TRUE
#endif
#if ((ACC_IF==Interface_I2C0) || (MAG_IF==Interface_I2C0) || (GYR_IF==Interface_I2C0) || (OLED_IF==Interface_I2C0))
  #define I2C0_INUSE  TRUE
#endif
#if ((ACC_IF==Interface_I2C1) || (MAG_IF==Interface_I2C1) || (GYR_IF==Interface_I2C1) || (OLED_IF==Interface_I2C1))
  #define I2C1_INUSE  TRUE
#endif
#if ((TRACER_IF==Interface_UART0) || (HCI_IF==Interface_UART0))
  #define UART0_INUSE TRUE
#endif
#if ((TRACER_IF==Interface_UART1) || (HCI_IF==Interface_UART1))
  #define UART1_INUSE TRUE
#endif
#if ((TRACER_IF==Interface_UART2) || (HCI_IF==Interface_UART2))
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

#if ((I2C0_INUSE) || (I2C1_INUSE))
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

#include FIND_MODULE(ACC_HEADER)
#include FIND_MODULE(MAG_HEADER)
#include FIND_MODULE(GYR_HEADER)
#include FIND_MODULE(OLED_HEADER)
 


#endif //_OPTION_CHECK_H_
