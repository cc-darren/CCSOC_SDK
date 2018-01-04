#ifdef APP_SERV_MGR_EN


#ifndef _CC_APP_SVC_MANAGER_H
#define _CC_APP_SVC_MANAGER_H


#include <stdint.h>
#include "error.h"




#ifdef SRV_MGR_TEST_EN
//    #define SRV_MGR_TEST_CASE_1     // pedo test    
    #define SRV_MGR_TEST_CASE_2     // hrm test    
//    #define SRV_MGR_TEST_CASE_3     // swimming test    
//    #define SRV_MGR_TEST_CASE_4     // suspend/resume test    

#endif


typedef enum
{
    E_APP_SRV_ERR_NONE = CC_SUCCESS,            
    E_APP_SRV_ERR_TYPE,
    E_APP_SRV_ERR_CONFLICT,  
    E_APP_SRV_ERR_START_FAIL,
    E_APP_SRV_ERR_STOP_FAIL, 
    //E_APP_SRV_ERR_GET_DATA_FAIL, 
    E_APP_SRV_ERR_NOT_WORK_IN_SUSPEND, 
    E_APP_SRV_ERR_CANT_RESUME_FROM_ACTIVE, 
    E_APP_SRV_ERR_RESUME_FAIL,
    
}E_App_Srv_Err_Code;

typedef enum
{
    E_APP_SRV_ID_HRM = 0,
    E_APP_SRV_ID_PEDO,
    E_APP_SRV_ID_SWIM,

    E_APP_SRV_ID_TOTAL,
}E_App_Srv_ID;
   

typedef enum
{
    E_APP_SRV_ST_IDLE = 0,            
    E_APP_SRV_ST_ACTIVE,
    E_APP_SRV_ST_SUSPEND,    
        
}E_App_Srv_State;


/*
typedef struct 
{
    int16_t *p_ppg_data;    
    int16_t *p_accel_data;
    int16_t *p_gyro_data;    
    int16_t *p_mag_data;
}T_CC_SenMgrData;

typedef struct 
{
    uint32_t ppg_SzInBytes;    
    uint32_t accel_SzInBytes;
    uint32_t gyro_SzInBytes;
    uint32_t mag_SzInBytes;
}T_CC_SenMgrData_Bytes;
*/

E_App_Srv_Err_Code CC_AppSrv_Manager_Init(void);
E_App_Srv_Err_Code CC_AppSrv_Manager_Start(E_App_Srv_ID type);
E_App_Srv_Err_Code CC_AppSrv_Manager_Stop(E_App_Srv_ID type);


//E_App_Srv_Err_Code CC_AppSrv_Sensor_GetData(E_App_Srv_ID sensor_id, E_App_Srv_ID type, void* pSampleData, void *pDataSzInBytes);
void CC_AppSrv_Sensor_Test(void);


#endif// end of _CC_APP_SVC_MANAGER_H


#endif
