





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
      #include "STLSM303C_ACC.h"

    #elif (MODULE_ACC == ACC_ST_LSM303D)
      #include "STLSM303D_ACC.h"
      
    #elif (MODULE_ACC == ACC_ST_LIS2DH12)
      #include "STLIS2DH12_ACC.h"
      
    #elif (MODULE_ACC == ACC_ST_LIS2DW12)
      #include "STLIS2DW12_ACC.h"
      
    #elif (MODULE_ACC == ACC_KIX_KX022_1020)
      #include "KIXKX022_ACC.h"
      
    #elif (MODULE_ACC == ACC_KIX_KXG03)
      #include "KIXKXG03_ACC.h"
      
    #elif (MODULE_ACC == ACC_AD_ADXL362)
      #include "ADXL362_ACC.h"
      
    #elif (MODULE_ACC == ACC_ST_LSM6DSL)
      #include "acc_lsm6ds3.h"

    #elif (MODULE_ACC == ACC_NULL)
      #include "acc_null.h"
      #undef ACC_IF
      #define ACC_IF IF_NULL
    #else
      #error " no matched ACC driver found"
    #endif
  #else
    #error "not a ACC driver"
  #endif
  
#else
  #error "Please choose 'ACC_NULL' if ACC is not in use."
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
      #include "mag_ak09912.h"
    #elif (MODULE_MAG == MAG_AKM_AK09915C)
      #include "AK09915.h"
      
    #elif (MODULE_MAG == MAG_ST_LSM303C)
      #include "STLSM303C.h"
      
    #elif (MODULE_MAG == MAG_ST_LSM303D)
      #include "STLSM303D.h"
      
    #elif (MODULE_MAG == MAG_ST_LIS2MDL)
      #include "STLIS2MDL.h"
      
    #elif (MODULE_MAG == MAG_NULL)
      #include "mag_null.h"
      #undef MAG_IF
      #define MAG_IF IF_NULL
    #else
      #error " no matched MAG driver found"
    #endif
  #else
    #error "not a MAG driver"
  #endif
#else
  #error "Please choose 'MAG_NULL' if MAG is not in use."
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
      #include "LSM6DS3_ACC_GYRO_DRIVER.h"

    #elif (MODULE_GYR == GYR_NULL)
      #include "gyr_null.h"
      #undef GYR_IF
      #define GYR_IF IF_NULL
    #else
      #error " no matched GYR driver found"
    #endif
  #else
    #error "not a GYR driver"
  #endif
#else
  #error "Please choose 'GYR_NULL' if GYR is not in use."
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
      #include "ssd1306.h"

    #elif (MODULE_OLED == OLED_NULL)
      #include "oled_null.h"
      #undef OLED_IF
      #define OLED_IF IF_NULL
    #else
      #error " no matched oled driver found"
    #endif
  #else
    #error "not a OLED driver"
  #endif
#else
  #error "Please choose 'OLED_NULL' if OLED is not in use."
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

//remove IF check temporately, I2C interface can connect to mutilple slave
/*
#if (ACC_IF == MAGIF)
  #error "ACC == MAG"
#elif (MAG_IF == GYR_IF)
  #error "MAG == GYR"
#endif
*/

#endif //_OPTION_CHECK_H_
