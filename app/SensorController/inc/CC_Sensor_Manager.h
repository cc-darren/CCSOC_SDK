#ifdef APP_SERV_MGR_EN


#ifndef _CC_SENSOR_MANAGER_H_
#define _CC_SENSOR_MANAGER_H_



#include "error.h"
#include "CC_AppSrvc_Manager.h"


#ifdef SM_TEST_EN
//#define SM_TEST_CASE_1      // Test access Accel, need to make sure No Gyro data output
    //#ifdef SM_TEST_CASE_1
    //#define SM_TEST_CASE_1_1   // verify fifo time in const number
   // #endif

//#define SM_TEST_CASE_2      // Test access Gyro, need to make sure No Accel data output
    #ifdef SM_TEST_CASE_2
    //#define SM_TEST_CASE_2_1    // verify fifo time in const number
    #endif

//#define SM_TEST_CASE_3      // Test access Mag

#define SM_TEST_CASE_4      // Test access PPG


//#define SM_TEST_CASE_5      // Test access Gyro/Accel


#endif

#define MEMS_FIFO_SIZE   100// 100 for test because of no enough memory, 300 => formal number  


struct sensor_params
{
    uint16_t param_len;    
    uint8_t  param[];
};

/*
typedef enum
{
    E_APP_SRV_ID_HRM              = 0,
    E_APP_SRV_ID_PEDO             = 1,
    E_APP_SRV_ID_SWIM             = 2,

    E_APP_SRV_ID_TOTAL,    
}E_APP_SRV_ID;
*/

typedef enum
{
    E_SEN_ST_IDLE = 0,
    E_SEN_ST_INIT,
    E_SEN_ST_CONFIG,
    E_SEN_ST_START,
}E_Sensor_Manager_Status;



typedef enum
{
    E_SEN_ERROR_NONE = CC_SUCCESS,            
    E_SEN_ERROR_INVALID_STATE,        
    E_SEN_ERROR_INIT_FAIL,      
    E_SEN_ERROR_CONFIG_FAIL, 
    E_SEN_ERROR_START_FAIL, 
    E_SEN_ERROR_GET_DATA_FAIL,   
//    E_SEN_ERROR_NOT_INIT,      
//    E_SEN_ERROR_SENSOR_IN_USE,
//    E_SEN_ERROR_REPEAT_CONFIG,
    E_SEN_ERROR_INVALID_CONTROL_TARGET,    
    E_SEN_ERROR_INVALID_ACCESS_MODE,
    E_SEN_ERROR_INVALID_USER_ID,
    E_SEN_ERROR_INVALID_SENSOR_ID,
        
}E_Sensor_Error_Code;



typedef enum
{
    E_SEN_TYPE_Pah8002_PPG = 0,
    E_SEN_TYPE_LSM6DS3_ACCEL_GYRO,
    E_SEN_TYPE_AK09912_MAG,

    E_SEN_ID_TOTAL,
}E_Sensor_Type;




typedef uint8_t (*sensor_init_fnct)    (void);
typedef uint8_t (*sensor_configure_fnct)    (E_App_Srv_ID UserID, void *Settings);
typedef uint8_t (*sensor_start_fnct)    (E_App_Srv_ID UserID);
typedef uint8_t (*sensor_getdata_fnct)    (E_App_Srv_ID UserID, void* pSampleData, void *pDataSzInBytes);
typedef uint8_t (*sensor_shutdown_fnct)    (E_App_Srv_ID UserID, bool power_off);



struct sensor_manager_itfs
{
    sensor_init_fnct        init;
    sensor_configure_fnct   configure;
    sensor_start_fnct       start;
    sensor_getdata_fnct     getdata;
    sensor_shutdown_fnct    shutdown;    
};

/*
E_Sensor_Error_Code CC_Sensor_Manager_Init(E_Sensor_Type SensorType, E_App_Srv_ID UserID);
E_Sensor_Error_Code CC_Sensor_Manager_Configure(E_Sensor_Type SensorType, E_App_Srv_ID UserID, void *Settings);
E_Sensor_Error_Code CC_Sensor_Manager_Start(E_Sensor_Type SensorType, E_App_Srv_ID UserID);
E_Sensor_Error_Code CC_Sensor_Manager_GetData(E_Sensor_Type SensorType, E_App_Srv_ID UserID, void* pSampleData, void *pDataSzInBytes);
E_Sensor_Error_Code CC_Sensor_Manager_Shutdown(E_Sensor_Type SensorType, E_App_Srv_ID UserID);
*/



E_Sensor_Error_Code CC_SenMgr_Start_HRM(void);
E_Sensor_Error_Code CC_SenMgr_Stop_HRM(void);
E_Sensor_Error_Code CC_SenMgr_Start_Pedometer(void);
E_Sensor_Error_Code CC_SenMgr_Stop_Pedometer(void);
E_Sensor_Error_Code CC_SenMgr_Start_Swim(void);
E_Sensor_Error_Code CC_SenMgr_Stop_Swim(void);


E_Sensor_Error_Code CC_SenMgr_PPG_GetData(E_App_Srv_ID user_id, uint32_t *pdata, uint32_t *psize);
E_Sensor_Error_Code CC_SenMgr_Acc_GetData(E_App_Srv_ID user_id, int16_t *pdata, uint32_t *psize);
E_Sensor_Error_Code CC_SenMgr_Gyro_GetData(E_App_Srv_ID user_id, int16_t *pdata, uint32_t *psize);
E_Sensor_Error_Code CC_SenMgr_Mag_GetData(E_App_Srv_ID user_id, int16_t *pdata, uint32_t *psize);




void SM_Test(void);

#endif    // END of _CC_SENSOR_MANAGER_H_


#endif
