
//This file is an example about how to add options check.
//It will be deleted after everything is done


#if defined(SENSOR_ACC)
  #if (SENSOR_ACC & 0x4000)
    #if (SENSOR_ACC == AK09912)
      #include "AK09912.h"

    #elif (SENSOR_ACC == AK09913)
      #include "AK09913.h"
      
    #elif (SENSOR_ACC == AK09914)
      #include "AK09914.h"
      
    #else
      #error "no ACC define"
    #endif
  #else
    #error "ACC wrong"
  #endif
#endif



//Add new check for IF
#if (ACC_IF == MAGIF)
  #error "ACC == MAG"
#elif (MAG_IF == GYR_IF)
  #error "MAG == GYR"
#endif
