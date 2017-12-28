#ifdef APP_SERV_MGR_EN


#ifndef _MAG_CONTROLLER_
#define _MAG_CONTROLLER_

#include "error.h"
#include "CC_AppSrvc_Manager.h"
#include "mag_ak09912.h"


typedef enum
{
    E_MAG_SUCCESS = CC_SUCCESS,
    E_MAG_ERROR_INIT_FAIL,      
    E_MAG_ERROR_NOT_INIT,      
    E_MAG_ERROR_SENSOR_IN_USE,
    E_MAG_ERROR_REPEAT_CONFIG,
    E_MAG_ERROR_GET_DATA,
    E_MAG_ERROR_INVALID_CONTROL_TARGET,    
    E_MAG_ERROR_INVALID_ACCESS_MODE,
    

}E_Mag_Manager_Status;



E_Mag_Manager_Status Mag_Controller_Init(void);
E_Mag_Manager_Status Mag_Controller_Configure(E_App_Srv_ID UserID, void *Params);
E_Mag_Manager_Status Mag_Controller_Start(E_App_Srv_ID UserID);
E_Mag_Manager_Status Mag_Controller_GetData(E_App_Srv_ID UserID, void* pSampleData, void *pDataSzInBytes);
E_Mag_Manager_Status Mag_Controller_Shutdown(E_App_Srv_ID UserID, bool power_off);
const struct sensor_manager_itfs* mag_sm_itf_get(void);

#endif //_MAG_CONTROLLER_

#endif