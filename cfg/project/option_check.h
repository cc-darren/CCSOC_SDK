





//This file is an example about how to add options check.
//It will be deleted after everything is done


#if defined(SENSOR_ACC)
  #if (SENSOR_ACC & 0x1000)
    #if (SENSOR_ACC == ACC_ST_LSM303C)
      #include "AK09912.h"

    #elif (SENSOR_ACC == ACC_ST_LSM303D)
      #include "AK09913.h"
      
    #elif (SENSOR_ACC == ACC_ST_LIS2DH12)
      #include "AK09914.h"
      
    #elif (SENSOR_ACC == ACC_ST_LIS2DW12)
      #include "AK09914.h"
      
    #elif (SENSOR_ACC == ACC_KIX_KX022_1020)
      #include "AK09914.h"
      
    #elif (SENSOR_ACC == ACC_KIX_KXG03)
      #include "AK09914.h"
      
    #elif (SENSOR_ACC == ACC_AD_ADXL362)
      #include "AK09914.h"
      
    #elif (SENSOR_ACC == ACC_ST_LSM6DSL)
      #include "LSM6DS3_ACC_GYRO_DRIVER.h"
      
    #else
      #error "no ACC define"
    #endif
  #else
    #error "ACC wrong"
  #endif
  
  #if ((defined ACC_IF) && (ACC_IF))
    //TODO: ACC interface available
  #else
    #error "ACC interface not found"
  #endif

#endif


#if defined(SENSOR_MAG)
  #if (SENSOR_MAG & 0x2000)
    #if (SENSOR_MAG == MAG_AKM_AK09912C)
      #include "AK09912.h"

    #elif (SENSOR_MAG == MAG_AKM_AK09915C)
      #include "AK09913.h"
      
    #elif (SENSOR_MAG == MAG_ST_LSM303C)
      #include "AK09914.h"
      
    #elif (SENSOR_MAG == MAG_ST_LSM303D)
      #include "AK09914.h"
      
    #elif (SENSOR_MAG == MAG_ST_LIS2MDL)
      #include "AK09914.h"
      
    #else
      #error "no MAG define"
    #endif
  #else
    #error "MAG wrong"
  #endif
  
  #if ((defined MAG_IF) && (MAG_IF))
    //TODO: MAG interface available
  #else
    #error "MAG interface not found"
  #endif

#endif


#if defined(SENSOR_GYR)
  #if (SENSOR_GYR & 0x2000)
    #if (SENSOR_GYR == GYR_ST_LSM6DSL)
      #include "LSM6DS3_ACC_GYRO_DRIVER.h"

    #else
      #error "no GYR define"
    #endif
  #else
    #error "GYR wrong"
  #endif
  
  #if ((defined GYR_IF) && (GYR_IF))
    //TODO: GYR interface available
  #else
    #error "GYR interface not found"
  #endif

#endif

//check which interface is used
#if   ((ACC_IF==_Interface_SPI0_) || (MAG_IF==_Interface_SPI0_) || (GYR_IF==_Interface_SPI0_) || (OLED_IF==_Interface_SPI0_))
  #define _SPI0_INUSE_  TRUE
#elif ((ACC_IF==_Interface_SPI1_) || (MAG_IF==_Interface_SPI1_) || (GYR_IF==_Interface_SPI1_) || (OLED_IF==_Interface_SPI1_))
  #define _SPI1_INUSE_  TRUE
#elif ((ACC_IF==_Interface_SPI2_) || (MAG_IF==_Interface_SPI2_) || (GYR_IF==_Interface_SPI2_) || (OLED_IF==_Interface_SPI2_))
  #define _SPI2_INUSE_  TRUE
#elif ((ACC_IF==_Interface_I2C0_) || (MAG_IF==_Interface_I2C0_) || (GYR_IF==_Interface_I2C0_) || (OLED_IF==_Interface_I2C0_))
  #define _I2C0_INUSE_  TRUE
#elif ((ACC_IF==_Interface_I2C1_) || (MAG_IF==_Interface_I2C1_) || (GYR_IF==_Interface_I2C1_) || (OLED_IF==_Interface_I2C1_))
  #define _I2C1_INUSE_  TRUE
#elif ((ACC_IF==_Interface_UART0_) || (MAG_IF==_Interface_UART0_) || (GYR_IF==_Interface_UART0_) || (OLED_IF==_Interface_UART0_))
  #define _UART0_INUSE_ TRUE
#elif ((ACC_IF==_Interface_UART1_) || (MAG_IF==_Interface_UART1_) || (GYR_IF==_Interface_UART1_) || (OLED_IF==_Interface_UART1_))
  #define _UART1_INUSE_ TRUE
#elif ((ACC_IF==_Interface_UART2_) || (MAG_IF==_Interface_UART2_) || (GYR_IF==_Interface_UART2_) || (OLED_IF==_Interface_UART2_))
  #define _UART2_INUSE_ TRUE
#endif

//Add new check for IF
#if (ACC_IF == MAGIF)
  #error "ACC == MAG"
#elif (MAG_IF == GYR_IF)
  #error "MAG == GYR"
#endif
