





#ifndef _OPTION_CHECK_H_
#define _OPTION_CHECK_H_

//This file is an example about how to add options check.
//It will be deleted after everything is done


#if defined(MODULE_ACC)
  #if (MODULE_ACC & 0x1000)
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
      #include "LSM6DS3_ACC_GYRO_DRIVER.h"

    #elif (MODULE_ACC == ACC_NULL)
      #include "acc_null.h"
      #undef ACC_IF
      #define ACC_IF _IF_NULL_
    #else
      #error " no matched ACC driver found"
    #endif
  #else
    #error "not a ACC driver"
  #endif
  
  #if ((defined ACC_IF) && (ACC_IF))
    #define ACC_IF_TYPE (ACC_IF&0xF0)
    #define ACC_IF_ID   (ACC_IF&0x0F)
  #else
    #error "ACC interface not found"
  #endif
#else
  #error "Please choose 'ACC_NULL' if ACC is not in use."
#endif


#if defined(MODULE_MAG)
  #if (MODULE_MAG & 0x2000)
    #if (MODULE_MAG == MAG_AKM_AK09912C)
      #include "AK09912.h"

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
      #define MAG_IF _IF_NULL_
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
    #error "MAG interface not found"
  #endif
#else
  #error "Please choose 'MAG_NULL' if MAG is not in use."
#endif


#if defined(MODULE_GYR)
  #if (MODULE_GYR & 0x2000)
    #if (MODULE_GYR == GYR_ST_LSM6DSL)
      #include "LSM6DS3_ACC_GYRO_DRIVER.h"

    #elif (MODULE_GYR == GYR_NULL)
      #include "gyr_null.h"
      #undef GYR_IF
      #define GYR_IF _IF_NULL_
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
    #error "GYR interface not found"
  #endif
#else
  #error "Please choose 'GYR_NULL' if GYR is not in use."
#endif


#if defined(MODULE_OLED)
  #if (MODULE_OLED & 0x4000)
    #if (MODULE_OLED == OLED_SOLOMON_SSD1306)
      #include "ssd1306.h"

    #elif (MODULE_OLED == OLED_NULL)
      #include "oled_null.h"
      #undef OLED_IF
      #define OLED_IF _IF_NULL_
    #else
      #error " no matched oled driver found"
    #endif
  #else
    #error "not a OLED driver"
  #endif
  
  #if ((defined OLED_IF) && (OLED_IF))
    #define OLED_IF_TYPE (OLED_IF&0xF0)
    #define OLED_IF_ID   (OLED_IF&0x0F)
  #else
    #error "OLED interface not found"
  #endif
#else
  #error "Please choose 'OLED_NULL' if OLED is not in use."
#endif


//check which interface is used
#if   ((ACC_IF==_Interface_SPI0_) || (MAG_IF==_Interface_SPI0_) || (GYR_IF==_Interface_SPI0_) || (OLED_IF==_Interface_SPI0_))
  #define _SPI0_INUSE_  TRUE
#endif
#if ((ACC_IF==_Interface_SPI1_) || (MAG_IF==_Interface_SPI1_) || (GYR_IF==_Interface_SPI1_) || (OLED_IF==_Interface_SPI1_))
  #define _SPI1_INUSE_  TRUE
#endif
#if ((ACC_IF==_Interface_SPI2_) || (MAG_IF==_Interface_SPI2_) || (GYR_IF==_Interface_SPI2_) || (OLED_IF==_Interface_SPI2_))
  #define _SPI2_INUSE_  TRUE
#endif
#if ((ACC_IF==_Interface_I2C0_) || (MAG_IF==_Interface_I2C0_) || (GYR_IF==_Interface_I2C0_) || (OLED_IF==_Interface_I2C0_))
  #define _I2C0_INUSE_  TRUE
#endif
#if ((ACC_IF==_Interface_I2C1_) || (MAG_IF==_Interface_I2C1_) || (GYR_IF==_Interface_I2C1_) || (OLED_IF==_Interface_I2C1_))
  #define _I2C1_INUSE_  TRUE
#endif
#if ((ACC_IF==_Interface_UART0_) || (MAG_IF==_Interface_UART0_) || (GYR_IF==_Interface_UART0_) || (OLED_IF==_Interface_UART0_))
  #define _UART0_INUSE_ TRUE
#endif
#if ((ACC_IF==_Interface_UART1_) || (MAG_IF==_Interface_UART1_) || (GYR_IF==_Interface_UART1_) || (OLED_IF==_Interface_UART1_))
  #define _UART1_INUSE_ TRUE
#endif
#if ((ACC_IF==_Interface_UART2_) || (MAG_IF==_Interface_UART2_) || (GYR_IF==_Interface_UART2_) || (OLED_IF==_Interface_UART2_))
  #define _UART2_INUSE_ TRUE
#endif

#if ((_SPI0_INUSE_) || (_SPI1_INUSE_) || (_SPI2_INUSE_))
  #define _SPI_INUSE_ TRUE
#endif

#if ((_I2C0_INUSE_) || (_I2C1_INUSE_))
  #define _I2C_INUSE_ TRUE
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
