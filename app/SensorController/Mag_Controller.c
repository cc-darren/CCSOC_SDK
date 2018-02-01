/* Copyright (c) 2018 Cloudchip, Inc. All Rights Reserved.
 *
 * The information contained herein is property of Cloudchip, Inc.
 * Terms and conditions of usage are described in detail in CLOUDCHIP
 * STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information.
 * NO WARRANTY of ANY KIND is provided. This heading must NOT be removed 
 * from the file.
 */

#ifdef APP_SERV_MGR_EN

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "project.h"
#include "CC_AppSrvc_Manager.h"
#include "CC_Sensor_Manager.h"
#include "mag_ak09912.h"
#include "Mag_Controller.h"
#include "error.h"

#include "tracer.h"

AxesRaw_t s_tMagData;
static AK09912_MAG_MODE_t m_mag_odr;



//////////////// Interface definition /////////////////////

E_Mag_Manager_Status Mag_Controller_Init(void)
{
    E_Mag_Manager_Status ret = E_MAG_SUCCESS;

    
    memset(&s_tMagData, 0x00, sizeof(AxesRaw_t));

    if(MEMS_ERROR == AK09912_MAG_Init())
        return E_MAG_ERROR_INIT_FAIL;

    return ret;
}


E_Mag_Manager_Status Mag_Controller_Configure(E_Sensor_User_ID UserID, void *Params)
{
    E_Mag_Manager_Status ret = E_MAG_SUCCESS;

    m_mag_odr = *(AK09912_MAG_MODE_t *) Params;

    return ret;
}



E_Mag_Manager_Status Mag_Controller_Start(E_Sensor_User_ID UserID)
{
    E_Mag_Manager_Status ret = E_MAG_SUCCESS;

    CC_AK09912_MAG_SET_ODR(m_mag_odr); 

    return ret;
}


E_Mag_Manager_Status Mag_Controller_GetData(E_Sensor_User_ID UserID, void* pSampleData, void *pDataSzInBytes)
{
    E_Mag_Manager_Status ret = E_MAG_SUCCESS;
	  uint16_t* pdataBytes = (uint16_t*) pDataSzInBytes;
    
    if(MEMS_ERROR != AK09912_MAG_GetMagRaw(&s_tMagData))
    {
        *pdataBytes = sizeof(AxesRaw_t);

        memcpy(pSampleData, &s_tMagData, *pdataBytes);
    }
    else
    {
        *pdataBytes = 0;

        return E_MAG_ERROR_GET_DATA;
    }

    return ret;
}


E_Mag_Manager_Status Mag_Controller_Shutdown(E_Sensor_User_ID UserID, bool power_off)
{
    E_Mag_Manager_Status ret = E_MAG_SUCCESS;

    if(true == power_off)        
        AK09912_MAG_SLEEP();

    return ret;
}


//////////////   interface of sensor manager  /////////////////

const struct sensor_manager_itfs mag_itf =
{
    Mag_Controller_Init,
    Mag_Controller_Configure,
    Mag_Controller_Start,
    Mag_Controller_GetData,
    Mag_Controller_Shutdown
};


const struct sensor_manager_itfs* mag_sm_itf_get(void)
{
    return &mag_itf;
}

#endif
