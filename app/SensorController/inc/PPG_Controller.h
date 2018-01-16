#ifdef APP_SERV_MGR_EN


#ifndef _PPG_CONTROLLER_
#define _PPG_CONTROLLER_

#include "error.h"
#include "CC_AppSrvc_Manager.h"




typedef enum
{
    E_PPG_SUCCESS = CC_SUCCESS,
    E_PPG_ERROR_INIT_FAIL,      
    E_PPG_ERROR_NOT_INIT,      
    E_PPG_ERROR_FIFO_OVERFLOW,
    E_PPG_ERROR_FIFO_UNDERFLOW, 
    E_PPG_ERROR_DATA_NOT_READY,     
    

}E_PPG_Manager_Status;



E_PPG_Manager_Status PPG_Controller_Init(void);
E_PPG_Manager_Status PPG_Controller_Configure(E_Sensor_User_ID UserID, void *Params);
E_PPG_Manager_Status PPG_Controller_Start(E_Sensor_User_ID UserID);
E_PPG_Manager_Status PPG_Controller_GetData(E_Sensor_User_ID UserID, void* pSampleData, void *pDataSzInBytes);
E_PPG_Manager_Status PPG_Controller_Shutdown(E_Sensor_User_ID UserID, bool power_off);
const struct sensor_manager_itfs* ppg_sm_itf_get(void);

#endif //_PPG_CONTROLLER_


#endif
