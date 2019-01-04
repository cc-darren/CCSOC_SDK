/* Copyright (c) 2019 Cloudchip, Inc. All Rights Reserved.
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
#include "CC_Sensor_Manager.h"
#include "CC_AppSrvc_Manager.h"
#include "Acc_Gyro_Controller.h"
#include "Mag_Controller.h"
#include "PPG_Controller.h"
#include "CC_HRM_drv.h"
#include "error.h"
#include "tracer.h"
#include "clock.h"


E_Sensor_Manager_Status  g_sen_srvc_st[E_SEN_ID_TOTAL];
bool g_sen_srvc_in_use[E_SEN_ID_TOTAL][E_SEN_USER_ID_TOTAL];

int16_t hrm_acc_tmpbuf[MEMS_FIFO_SIZE];
int16_t hrm_acc_data[MEMS_FIFO_SIZE];

int16_t pedo_acc_tmpbuf[MEMS_FIFO_SIZE];
int16_t swim_acc_tmpbuf[MEMS_FIFO_SIZE];
int16_t swim_gyro_tmpbuf[MEMS_FIFO_SIZE];




static const struct sensor_manager_itfs * sm_itf_get(E_Sensor_Type SensorType)
{
    
    const struct sensor_manager_itfs* sm_itfs = NULL;

    switch(SensorType)
    {
        case E_SEN_TYPE_Pah8002_PPG:
            sm_itfs = ppg_sm_itf_get();
            break;

        case E_SEN_TYPE_LSM6DS3_ACCEL_GYRO:
            sm_itfs = acc_gyro_sm_itf_get();
            break;

        case E_SEN_TYPE_AK09912_MAG:
            sm_itfs = mag_sm_itf_get();
            break;
        default:
            break;
    };

    return sm_itfs;
}


static void SM_Set_Sensor_Status(E_Sensor_Type SensorType, E_Sensor_Manager_Status status)
{
    g_sen_srvc_st[SensorType] = status;
}

static void SM_Set_Sensor_isUsed(E_Sensor_Type SensorType, E_Sensor_User_ID UserID)
{
    g_sen_srvc_in_use[SensorType][UserID] = true;
}

static void SM_Clr_Sensor_isUsed(E_Sensor_Type SensorType, E_Sensor_User_ID UserID)
{
    g_sen_srvc_in_use[SensorType][UserID] = false;
}

static bool SM_Check_Sensor_isUsed(E_Sensor_Type SensorType)
{
    for(uint8_t user_id = 0; user_id < E_SEN_USER_ID_TOTAL; user_id++)
    {
        if(true == g_sen_srvc_in_use[SensorType][user_id])
            return true;
    }

    return false;
}


void CC_AppSrv_Service_Init(void)
{

    for(uint8_t i = 0; i < E_SEN_ID_TOTAL; i++)
    {
        g_sen_srvc_st[i] = E_SEN_ST_IDLE;
    }
}




static E_Sensor_Error_Code CC_Sensor_Manager_Init(E_Sensor_Type SensorType, E_Sensor_User_ID UserID)
{

    E_Sensor_Error_Code ret = E_SEN_ERROR_NONE;
    const struct sensor_manager_itfs *sm_itfs = sm_itf_get(SensorType);
    //E_Sensor_Manager_Status srvc_st = g_sen_srvc_st[SensorType];


    if(E_SEN_ID_TOTAL <= SensorType)
        return E_SEN_ERROR_INVALID_SENSOR_ID;    
    
/*
    switch(srvc_st)
    {
        case E_SEN_ST_CONFIG:
        case E_SEN_ST_START:
             return E_SEN_ERROR_INVALID_STATE;          

        default:
            break;
    }
*/
    if(SM_Check_Sensor_isUsed(SensorType))
    {
        if(E_SEN_ERROR_NONE != sm_itfs->shutdown(UserID, true))
            return E_SEN_ERROR_INIT_FAIL;
    }

    if(E_SEN_ERROR_NONE != sm_itfs->init())
        return E_SEN_ERROR_INIT_FAIL;

    
    SM_Set_Sensor_Status(SensorType, E_SEN_ST_INIT);

    return ret;
}



static E_Sensor_Error_Code CC_Sensor_Manager_Configure(E_Sensor_Type SensorType, E_Sensor_User_ID UserID, void *Settings)
{
    E_Sensor_Error_Code ret = E_SEN_ERROR_NONE;
    const struct sensor_manager_itfs *sm_itfs = sm_itf_get(SensorType);
    E_Sensor_Manager_Status srvc_st = g_sen_srvc_st[SensorType];

    if(E_SEN_USER_ID_TOTAL <= UserID)
        return E_SEN_ERROR_INVALID_USER_ID;

    if(E_SEN_ID_TOTAL <= SensorType)
        return E_SEN_ERROR_INVALID_SENSOR_ID;    


    switch(srvc_st)
    {
       // case E_SEN_ST_CONFIG:
       //     if(E_SEN_TYPE_LSM6DS3_ACCEL_GYRO == SensorType)            
       //         break;
       //     else            
       //         return E_SEN_ERROR_INVALID_STATE;
        case E_SEN_ST_IDLE:
        case E_SEN_ST_START:
            return E_SEN_ERROR_INVALID_STATE;        

        default:
            break;
    }

    if(E_SEN_ERROR_NONE != sm_itfs->configure(UserID, Settings))
        return E_SEN_ERROR_CONFIG_FAIL;


    SM_Set_Sensor_Status(SensorType, E_SEN_ST_CONFIG);

    return ret;
}



static E_Sensor_Error_Code CC_Sensor_Manager_Start(E_Sensor_Type SensorType, E_Sensor_User_ID UserID)
{

    E_Sensor_Error_Code ret = E_SEN_ERROR_NONE;
    const struct sensor_manager_itfs *sm_itfs = sm_itf_get(SensorType);
    E_Sensor_Manager_Status srvc_st = g_sen_srvc_st[SensorType];

    if(E_SEN_USER_ID_TOTAL <= UserID)
        return E_SEN_ERROR_INVALID_USER_ID;

    if(E_SEN_ID_TOTAL <= SensorType)
        return E_SEN_ERROR_INVALID_SENSOR_ID;    


    switch(srvc_st)
    {
        case E_SEN_ST_IDLE:
        case E_SEN_ST_START:
            return E_SEN_ERROR_INVALID_STATE;          
        case E_SEN_ST_INIT: // not config => pass?
            break;
        default:
            break;
    }

   
    if(E_SEN_ERROR_NONE != sm_itfs->start(UserID))
        return E_SEN_ERROR_START_FAIL;

    SM_Set_Sensor_isUsed(SensorType, UserID);

    SM_Set_Sensor_Status(SensorType, E_SEN_ST_START);

    return ret;
}


static E_Sensor_Error_Code CC_Sensor_Manager_GetData(E_Sensor_Type SensorType, E_Sensor_User_ID UserID, void* pSampleData, void *pDataSzInBytes)
{

    E_Sensor_Error_Code ret = E_SEN_ERROR_NONE;
    const struct sensor_manager_itfs *sm_itfs = sm_itf_get(SensorType);
    E_Sensor_Manager_Status srvc_st = g_sen_srvc_st[SensorType];

    if(E_SEN_USER_ID_TOTAL <= UserID) 
        return E_SEN_ERROR_INVALID_USER_ID;

    if(E_SEN_ID_TOTAL <= SensorType)
        return E_SEN_ERROR_INVALID_SENSOR_ID;    
     
 
    switch(srvc_st)
    {
        case E_SEN_ST_IDLE:
        case E_SEN_ST_INIT:
        case E_SEN_ST_CONFIG:
            return E_SEN_ERROR_INVALID_STATE;
        default:
            break;
    }


    //E_Sensor_Error_Code error_code;

    if(E_SEN_ERROR_NONE != (E_Sensor_Error_Code) sm_itfs->getdata(UserID, pSampleData, pDataSzInBytes))
    {
        //TracerInfo("get ppg fail: %d\r\n", error_code);  
        return E_SEN_ERROR_GET_DATA_FAIL;
    }

    return ret;
}


static E_Sensor_Error_Code CC_Sensor_Manager_Shutdown(E_Sensor_Type SensorType, E_Sensor_User_ID UserID)
{
    E_Sensor_Error_Code ret = E_SEN_ERROR_NONE;
    const struct sensor_manager_itfs *sm_itfs = sm_itf_get(SensorType);

    if(E_SEN_USER_ID_TOTAL <= UserID)
        return E_SEN_ERROR_INVALID_USER_ID;

    if(E_SEN_ID_TOTAL <= SensorType)
        return E_SEN_ERROR_INVALID_SENSOR_ID;    


    SM_Clr_Sensor_isUsed(SensorType, UserID);

    if(!SM_Check_Sensor_isUsed(SensorType))
    {
        sm_itfs->shutdown(UserID, true);
        
        SM_Set_Sensor_Status(SensorType, E_SEN_ST_IDLE);
    }    
    else
        sm_itfs->shutdown(UserID, false);

    return ret;

}






//////////////////////   HRM  /////////////////////////////////


E_Sensor_Error_Code CC_SenMgr_Start_HRM(void)
{    
    
    E_Sensor_Error_Code ret_code = E_SEN_ERROR_NONE;

    // PPG:
    // Init:    
    if(E_SEN_ERROR_NONE != (ret_code = CC_Sensor_Manager_Init(E_SEN_TYPE_Pah8002_PPG, E_SEN_USER_ID_HRM)))
    {
        return ret_code;
    }

    // Configuration:    
    if(E_SEN_ERROR_NONE != (ret_code = CC_Sensor_Manager_Configure(E_SEN_TYPE_Pah8002_PPG, E_SEN_USER_ID_HRM, NULL)))
    {
        return ret_code;        
    }
        
    // Start:
    if(E_SEN_ERROR_NONE != (ret_code = CC_Sensor_Manager_Start(E_SEN_TYPE_Pah8002_PPG, E_SEN_USER_ID_HRM)))
    {
        return ret_code;        
    } 



    // Acc:
    T_Accel_Gyro_Settings acc_gyro_settings;
    
    memset(&acc_gyro_settings, 0x00, sizeof(T_Accel_Gyro_Settings));

    acc_gyro_settings.AccessMode = E_ACC_GYRO_MODE_FIFO,                // fifo mode
    acc_gyro_settings.FifoTarget = E_LSM6DSX_FIFO_CONTROL_ACCEL,        // fifo for Accel
    
    acc_gyro_settings.S_Accel.FifoSet.nFifoDepth = MEMS_FIFO_SIZE,      // fifo size: 300
    acc_gyro_settings.S_Accel.FifoSet.Odr = LSM6DS3_ACC_GYRO_ODR_FIFO_50Hz,  //50Hz
    acc_gyro_settings.S_Accel.FifoSet.iSampleData = hrm_acc_tmpbuf;     // todo: using HEAP!!
    
    // Init:    
    if(E_SEN_ERROR_NONE != (ret_code = CC_Sensor_Manager_Init(E_SEN_TYPE_LSM6DS3_ACCEL_GYRO, E_SEN_USER_ID_HRM)))
    {
        return ret_code;
    }

    // Configuration:    
    if(E_SEN_ERROR_NONE != (ret_code = CC_Sensor_Manager_Configure(E_SEN_TYPE_LSM6DS3_ACCEL_GYRO, E_SEN_USER_ID_HRM, &acc_gyro_settings)))
    {
        return ret_code;        
    }
        
    // Start:
    if(E_SEN_ERROR_NONE != (ret_code = CC_Sensor_Manager_Start(E_SEN_TYPE_LSM6DS3_ACCEL_GYRO, E_SEN_USER_ID_HRM)))
    {
        return ret_code;        
    }        

    return ret_code;
}

E_Sensor_Error_Code CC_SenMgr_Stop_HRM(void)
{
    E_Sensor_Error_Code ret_code = E_SEN_ERROR_NONE;
    
    // Shutdown:
    // PPG
    if(E_SEN_ERROR_NONE != (ret_code = CC_Sensor_Manager_Shutdown(E_SEN_TYPE_Pah8002_PPG, E_SEN_USER_ID_HRM)))
    {
        return ret_code;
    }

    
    // Acc
    if(E_SEN_ERROR_NONE != (ret_code = CC_Sensor_Manager_Shutdown(E_SEN_TYPE_LSM6DS3_ACCEL_GYRO, E_SEN_USER_ID_HRM)))
    {
        return ret_code;
    }

    return ret_code;    
}



//////////////////////   HRM  /////////////////////////////////


E_Sensor_Error_Code CC_SenMgr_Start_Pedometer(void)
{        
    
    E_Sensor_Error_Code ret_code = E_SEN_ERROR_NONE;
    T_Accel_Gyro_Settings acc_gyro_settings;

    memset(&acc_gyro_settings, 0x00, sizeof(T_Accel_Gyro_Settings));

    acc_gyro_settings.AccessMode = E_ACC_GYRO_MODE_FIFO,                       // fifo mode
    acc_gyro_settings.FifoTarget = E_LSM6DSX_FIFO_CONTROL_ACCEL,        // fifo for Accel only

    // Accel settings:
    acc_gyro_settings.S_Accel.FifoSet.nFifoDepth = MEMS_FIFO_SIZE,           
    acc_gyro_settings.S_Accel.FifoSet.Odr = LSM6DS3_ACC_GYRO_ODR_FIFO_50Hz,  // 50 Hz
    acc_gyro_settings.S_Accel.FifoSet.iSampleData = pedo_acc_tmpbuf;        // todo: using HEAP!
#if 0
    // Gyro settings:
    acc_gyro_settings.S_Gyro.FifoSet.nFifoDepth = MEMS_FIFO_SIZE,            // fifo size: 100
    acc_gyro_settings.S_Gyro.FifoSet.Odr = LSM6DS3_ACC_GYRO_ODR_FIFO_50Hz,   // 50 Hz ==> different ODR

    acc_gyro_settings.S_Gyro.FifoSet.iSampleData = pedo_gyro_tmpbuf;
#endif

    // Init:    
    if(E_SEN_ERROR_NONE != (ret_code = CC_Sensor_Manager_Init(E_SEN_TYPE_LSM6DS3_ACCEL_GYRO, E_SEN_USER_ID_PEDO)))
    {
        return ret_code;
    }

    // Configuration:       
    if(E_SEN_ERROR_NONE != (ret_code = CC_Sensor_Manager_Configure(E_SEN_TYPE_LSM6DS3_ACCEL_GYRO, E_SEN_USER_ID_PEDO, &acc_gyro_settings)))
    {
        return ret_code;
    }

    // Start:
    if(E_SEN_ERROR_NONE != (ret_code = CC_Sensor_Manager_Start(E_SEN_TYPE_LSM6DS3_ACCEL_GYRO, E_SEN_USER_ID_PEDO)))
    {
        return ret_code;
    }        

    return ret_code;

}



E_Sensor_Error_Code CC_SenMgr_Stop_Pedometer(void)
{
    
    E_Sensor_Error_Code ret_code = E_SEN_ERROR_NONE;
    // Shutdown:    
    if(E_SEN_ERROR_NONE != (ret_code = CC_Sensor_Manager_Shutdown(E_SEN_TYPE_LSM6DS3_ACCEL_GYRO, E_SEN_USER_ID_PEDO)))
    {
        return ret_code;
    }     


    return ret_code;
}


//////////////////////   SWIM  /////////////////////////////////


E_Sensor_Error_Code CC_SenMgr_Start_Swim(void)
{

    E_Sensor_Error_Code ret_code = E_SEN_ERROR_NONE;  
    

    // Mag:     
    AK09912_MAG_MODE_t mag_settings = AK09912_MAG_DO_10_Hz;

    
    // Init:    
    if(E_SEN_ERROR_NONE != (ret_code = CC_Sensor_Manager_Init(E_SEN_TYPE_AK09912_MAG, E_SEN_USER_ID_SWIM)))
    {
        return ret_code;
    }

    // Configuration:       
    if(E_SEN_ERROR_NONE != (ret_code = CC_Sensor_Manager_Configure(E_SEN_TYPE_AK09912_MAG, E_SEN_USER_ID_SWIM, &mag_settings)))
    {
        return ret_code;
    }

    // Start:
    if(E_SEN_ERROR_NONE != (ret_code = CC_Sensor_Manager_Start(E_SEN_TYPE_AK09912_MAG, E_SEN_USER_ID_SWIM)))
    {
        return ret_code;
    }       



    // ACC/Gyro  
    T_Accel_Gyro_Settings acc_gyro_settings;

    memset(&acc_gyro_settings, 0x00, sizeof(T_Accel_Gyro_Settings));

    acc_gyro_settings.AccessMode = E_ACC_GYRO_MODE_FIFO,                       // fifo mode
    acc_gyro_settings.FifoTarget = E_LSM6DSX_FIFO_CONTROL_ACCEL_GYRO,        // fifo for Accel/Gyro

    // Accel settings:
    acc_gyro_settings.S_Accel.FifoSet.nFifoDepth = MEMS_FIFO_SIZE,           
    acc_gyro_settings.S_Accel.FifoSet.Odr = LSM6DS3_ACC_GYRO_ODR_FIFO_50Hz,  // 50 Hz
    acc_gyro_settings.S_Accel.FifoSet.iSampleData = swim_acc_tmpbuf;        // todo: using HEAP!

    // Gyro settings:
    acc_gyro_settings.S_Gyro.FifoSet.nFifoDepth = MEMS_FIFO_SIZE,            
    acc_gyro_settings.S_Gyro.FifoSet.Odr = LSM6DS3_ACC_GYRO_ODR_FIFO_50Hz,   // 50 Hz

    acc_gyro_settings.S_Gyro.FifoSet.iSampleData = swim_gyro_tmpbuf;        // todo: using HEAP!
    

    if(E_SEN_ERROR_NONE != (ret_code = CC_Sensor_Manager_Init(E_SEN_TYPE_LSM6DS3_ACCEL_GYRO, E_SEN_USER_ID_SWIM)))
    {
        return ret_code;
    }


    if(E_SEN_ERROR_NONE != (ret_code = CC_Sensor_Manager_Configure(E_SEN_TYPE_LSM6DS3_ACCEL_GYRO, E_SEN_USER_ID_SWIM, &acc_gyro_settings)))
    {
        return ret_code;
    }


    if(E_SEN_ERROR_NONE != (ret_code = CC_Sensor_Manager_Start(E_SEN_TYPE_LSM6DS3_ACCEL_GYRO, E_SEN_USER_ID_SWIM)))
    {
        return ret_code;
    }       


    return ret_code;


}

E_Sensor_Error_Code CC_SenMgr_Stop_Swim(void)
{
    E_Sensor_Error_Code ret_code = E_SEN_ERROR_NONE;

    // Mag
    if(E_SEN_ERROR_NONE != (ret_code = CC_Sensor_Manager_Shutdown(E_SEN_TYPE_AK09912_MAG, E_SEN_USER_ID_SWIM)))
    {
        return ret_code;
    }  
    

    // Acc
    if(E_SEN_ERROR_NONE != (ret_code = CC_Sensor_Manager_Shutdown(E_SEN_TYPE_LSM6DS3_ACCEL_GYRO, E_SEN_USER_ID_SWIM)))   
    {
        return ret_code;
    }     

    return ret_code;

}







// Get Data:
E_Sensor_Error_Code CC_SenMgr_PPG_GetData(E_Sensor_User_ID user_id, uint32_t *pdata, uint32_t *psize)
{

    return CC_Sensor_Manager_GetData(E_SEN_TYPE_Pah8002_PPG, user_id, pdata, psize);
}


E_Sensor_Error_Code CC_SenMgr_Acc_GetData(E_Sensor_User_ID user_id, int16_t *pdata, uint32_t *psize)
{
    E_Sensor_Error_Code ret_code = E_SEN_ERROR_NONE;
    T_Accel_Gyro_Fifo_Data  pfifoData;
    T_Accel_Gyro_Fifo_Data_Bytes pfifoSz;


    pfifoData.p_accel_data = pdata;
    pfifoData.p_gyro_data = NULL;


    if(E_SEN_ERROR_NONE != (ret_code = CC_Sensor_Manager_GetData(E_SEN_TYPE_LSM6DS3_ACCEL_GYRO, user_id, &pfifoData, &pfifoSz)))
    {
        return ret_code;      
    }   

    *psize = pfifoSz.accel_SzInBytes;   

    return ret_code;
}


E_Sensor_Error_Code CC_SenMgr_Gyro_GetData(E_Sensor_User_ID user_id, int16_t *pdata, uint32_t *psize)
{
    E_Sensor_Error_Code ret_code = E_SEN_ERROR_NONE;
    T_Accel_Gyro_Fifo_Data  pfifoData;
    T_Accel_Gyro_Fifo_Data_Bytes pfifoSz;


    //TracerInfo("pGyro_addr1: 0x%x\r\n", pdata);

    pfifoData.p_accel_data = NULL;
    pfifoData.p_gyro_data = pdata;


    //TracerInfo("pGyro_addr2: 0x%x\r\n", pfifoData.p_gyro_data);

    if(E_SEN_ERROR_NONE != (ret_code = CC_Sensor_Manager_GetData(E_SEN_TYPE_LSM6DS3_ACCEL_GYRO, user_id, &pfifoData, &pfifoSz)))
    {
        return ret_code;       
    }   

/*
            TracerInfo("Get Gyro[0]: %d\r\n", pfifoData.p_gyro_data[0]);
            TracerInfo("Get Gyro[1]: %d\r\n", pfifoData.p_gyro_data[1]);
            TracerInfo("Get Gyro[2]: %d\r\n", pfifoData.p_gyro_data[2]);
            TracerInfo("Get pGyro[0]: %d\r\n", pdata[0]);
            TracerInfo("Get pGyro[1]: %d\r\n", pdata[1]);
            TracerInfo("Get pGyro[2]: %d\r\n", pdata[2]);
*/

    *psize = pfifoSz.gyro_SzInBytes;      

    return ret_code;
}


E_Sensor_Error_Code CC_SenMgr_Mag_GetData(E_Sensor_User_ID user_id, int16_t *pdata, uint32_t *psize)
{
    E_Sensor_Error_Code ret_code = E_SEN_ERROR_NONE;



    if(E_SEN_ERROR_NONE != (ret_code = CC_Sensor_Manager_GetData(E_SEN_TYPE_AK09912_MAG, user_id, pdata, psize)))
    {
        return ret_code;        
    }   

    return ret_code;
}



















#ifdef SM_TEST_EN

void SM_Test(void)
{

    TracerInfo("SM_Test\r\n");
    E_Sensor_Error_Code ret_code;



    int16_t hrm_acc_tmpbuf[MEMS_FIFO_SIZE];
    int16_t hrm_acc_data[MEMS_FIFO_SIZE];
    T_Accel_Gyro_Fifo_Data  hrm_fifo_data;
    T_Accel_Gyro_Fifo_Data_Bytes hrm_fifo_sz;


    int16_t pedo_acc_tmpbuf[MEMS_FIFO_SIZE];
    int16_t pedo_gyro_tmpbuf[MEMS_FIFO_SIZE];
    int16_t pedo_acc_data[MEMS_FIFO_SIZE];
    int16_t pedo_gyro_data[MEMS_FIFO_SIZE];
    T_Accel_Gyro_Fifo_Data  pedo_fifo_data;
    T_Accel_Gyro_Fifo_Data_Bytes pedo_fifo_sz;

    uint32_t fifo_size = 0;

    uint32_t get_data_size = 0;


    hrm_fifo_data.p_accel_data = hrm_acc_data;    
    pedo_fifo_data.p_accel_data = pedo_acc_data;
    pedo_fifo_data.p_gyro_data = pedo_gyro_data;


#ifdef SM_TEST_CASE_1

    #define TEST_CASE_ROUND_NB  10

    TracerInfo("[SM_TC-1] Start:\r\n");

    T_Accel_Gyro_Settings hrm_settings =
    {
        .AccessMode = E_ACC_GYRO_MODE_FIFO,               // fifo mode
        .FifoTarget = E_LSM6DSX_FIFO_CONTROL_ACCEL,       // fifo for Accel
        
        .S_Accel.FifoSet.nFifoDepth = MEMS_FIFO_SIZE,     // fifo size: 100
        .S_Accel.FifoSet.Odr = LSM6DS3_ACC_GYRO_ODR_FIFO_25Hz,  //25Hz
        .S_Accel.FifoSet.iSampleData = hrm_acc_tmpbuf,
    };

    

    for(uint8_t round = 0; round < TEST_CASE_ROUND_NB; round++)
    {
        TracerInfo("[SM_TC-1]: Test Round: %d\r\n", round);


        TracerInfo("[SM_TC-1]: CC_Sensor_Manager_Init()\r\n");
        if(E_SEN_ERROR_NONE != (ret_code = CC_Sensor_Manager_Init(E_SEN_TYPE_LSM6DS3_ACCEL_GYRO, E_SEN_USER_ID_HRM)))
        {
            TracerInfo("[SM_TC-1]: CC_Sensor_Manager_Init fail!\r\n");

        }    

        TracerInfo("[SM_TC-1]: CC_Sensor_Manager_Configure()\r\n");
        if(E_SEN_ERROR_NONE != (ret_code = CC_Sensor_Manager_Configure(E_SEN_TYPE_LSM6DS3_ACCEL_GYRO, E_SEN_USER_ID_HRM, &hrm_settings)))
        {
            TracerInfo("[SM_TC-1]: CC_Sensor_Manager_Configure fail!\r\n");
        }


        TracerInfo("[SM_TC-1]: CC_Sensor_Manager_Start()\r\n");
        if(E_SEN_ERROR_NONE != (ret_code = CC_Sensor_Manager_Start(E_SEN_TYPE_LSM6DS3_ACCEL_GYRO, E_SEN_USER_ID_HRM)))
        {
            TracerInfo("[SM_TC-1]: CC_Sensor_Manager_Start fail!\r\n");
        }    

        get_data_size = 0;

#ifdef SM_TEST_CASE_1_1        
        while(1)
#else        
        while(get_data_size < MEMS_FIFO_SIZE)
#endif            
        {
            //if(E_SEN_ERROR_NONE != (ret_code = CC_Sensor_Manager_GetData(E_SEN_TYPE_LSM6DS3_ACCEL_GYRO, E_SEN_USER_ID_HRM, &hrm_fifo_data, &hrm_fifo_sz)))
            if(E_APP_SRV_ERR_NONE == CC_SenMgr_Acc_GetData(E_SEN_USER_ID_HRM, hrm_fifo_data, &fifo_size))
            {
                TracerInfo("[SM_TC-1]: CC_Sensor_Manager_GetData(HRM) fail!\r\n");
                
            }

            
            for(uint16_t i = 0; i < fifo_size/sizeof(int16_t); i+=3)
            {
                if((hrm_fifo_sz.accel_SzInBytes/2) < 3)
                     TracerInfo("[SM_TC-1]: Error Size: %d\r\n", fifo_size);
            
                TracerInfo("[SM_TC-1]: hrm_accel_X: %d\r\n", hrm_fifo_data.p_accel_data[i]);
                TracerInfo("[SM_TC-1]: hrm_accel_Y: %d\r\n", hrm_fifo_data.p_accel_data[i+1]);
                TracerInfo("[SM_TC-1]: hrm_accel_Z: %d\r\n", hrm_fifo_data.p_accel_data[i+2]);        
            }

            if(0 != fifo_size)
                TracerInfo("[SM_TC-1]: Gyro Data should not get here!! size: %d\r\n", fifo_size);


            get_data_size += hrm_fifo_sz.accel_SzInBytes;
            
#ifdef SM_TEST_CASE_1_1
            if(get_data_size >= 100)
            {
                static uint32_t old_tick = 0;

                TracerInfo("[SM_TC-1]: nb: %d, tick_diff: %d\r\n", get_data_size, Hrm_get_sys_tick() - old_tick);

                old_tick = Hrm_get_sys_tick();

                get_data_size = 0;
            }
#endif

            cc6801_ClockDelayMs(10);
            
            
            

        }


        TracerInfo("[SM_TC-1]: HRM get Accel data done.\r\n");

        TracerInfo("[SM_TC-1]: CC_Sensor_Manager_Shutdown()\r\n");
        if(E_SEN_ERROR_NONE != (ret_code = CC_Sensor_Manager_Shutdown(E_SEN_TYPE_LSM6DS3_ACCEL_GYRO, E_SEN_USER_ID_HRM)))
        {
            TracerInfo("[SM_TC-1]: CC_Sensor_Manager_Shutdown fail!\r\n");
        }


        //if(E_SEN_ERROR_NONE != (ret_code = CC_Sensor_Manager_GetData(E_SEN_TYPE_LSM6DS3_ACCEL_GYRO, E_SEN_USER_ID_HRM, &hrm_fifo_data, &hrm_fifo_sz)))
        if(E_APP_SRV_ERR_NONE == CC_SenMgr_Acc_GetData(E_SEN_USER_ID_HRM, hrm_fifo_data, &fifo_size))
        {
            TracerInfo("[SM_TC-1]: CC_Sensor_Manager_GetData() don't get any data after shutdown.\r\n");
                
        }
        else
        {
            TracerInfo("[SM_TC-1]: Accel Data should not get here!! size: %d\r\n", fifo_size);
        }

        
        

    }
    
    TracerInfo("[SM_TC-1]: SM Test Case 1 pass!\r\n");

    
    

    while(1)
        ;

#elif defined(SM_TEST_CASE_2)

    #define TEST_CASE_ROUND_NB  10

    TracerInfo("[SM_TC-2] Start:\r\n");

    T_Accel_Gyro_Settings pedo_settings =
    {
        .AccessMode = E_ACC_GYRO_MODE_FIFO,               // fifo mode
        .FifoTarget = E_LSM6DSX_FIFO_CONTROL_GYRO,       // fifo for Gyro
        
        .S_Gyro.FifoSet.nFifoDepth = MEMS_FIFO_SIZE,     // fifo size: 100
        .S_Gyro.FifoSet.Odr = LSM6DS3_ACC_GYRO_ODR_FIFO_25Hz,  //26Hz
        .S_Gyro.FifoSet.iSampleData = pedo_gyro_tmpbuf,
    };



    for(uint8_t round = 0; round < TEST_CASE_ROUND_NB; round++)
    {
        TracerInfo("[SM_TC-2]: Test Round: %d\r\n", round);


        TracerInfo("[SM_TC-2]: CC_Sensor_Manager_Init()\r\n");
        if(E_SEN_ERROR_NONE != (ret_code = CC_Sensor_Manager_Init(E_SEN_TYPE_LSM6DS3_ACCEL_GYRO, E_SEN_USER_ID_PEDO)))
        {
            TracerInfo("[SM_TC-2]: CC_Sensor_Manager_Init fail!\r\n");

        }    

        TracerInfo("[SM_TC-2]: CC_Sensor_Manager_Configure()\r\n");
        if(E_SEN_ERROR_NONE != (ret_code = CC_Sensor_Manager_Configure(E_SEN_TYPE_LSM6DS3_ACCEL_GYRO, E_SEN_USER_ID_PEDO, &pedo_settings)))
        {
            TracerInfo("[SM_TC-2]: CC_Sensor_Manager_Configure fail!\r\n");
        }

        Acc_Gyro_Dump_All_Settings();


        TracerInfo("[SM_TC-2]: CC_Sensor_Manager_Start()\r\n");
        if(E_SEN_ERROR_NONE != (ret_code = CC_Sensor_Manager_Start(E_SEN_TYPE_LSM6DS3_ACCEL_GYRO, E_SEN_USER_ID_PEDO)))
        {
            TracerInfo("[SM_TC-2]: CC_Sensor_Manager_Start fail!\r\n");
        }    

        get_data_size = 0;
        
        
#ifdef SM_TEST_CASE_2_1        
        while(1)
#else
        while(get_data_size < MEMS_FIFO_SIZE)
#endif
        {
            if(E_SEN_ERROR_NONE != (ret_code = CC_Sensor_Manager_GetData(E_SEN_TYPE_LSM6DS3_ACCEL_GYRO, E_SEN_USER_ID_PEDO, &pedo_fifo_data, &pedo_fifo_sz)))
            //if(E_APP_SRV_ERR_NONE == CC_SenMgr_Gyro_GetData(E_SEN_USER_ID_PEDO, &pedo_fifo_data, &fifo_size))
            {
                TracerInfo("[SM_TC-2]: CC_Sensor_Manager_GetData fail!\r\n");
                
            }

            
            for(uint16_t i = 0; i < pedo_fifo_sz.gyro_SzInBytes/sizeof(int16_t); i+=3)
            {
                if((pedo_fifo_sz.gyro_SzInBytes/2) < 3)
                     TracerInfo("[SM_TC-2]: Error Size: %d\r\n", pedo_fifo_sz.gyro_SzInBytes);
            
                TracerInfo("[SM_TC-2]: pedo_gyro_X: %d\r\n", pedo_fifo_data.p_gyro_data[i]);
                TracerInfo("[SM_TC-2]: pedo_gyro_Y: %d\r\n", pedo_fifo_data.p_gyro_data[i+1]);
                TracerInfo("[SM_TC-2]: pedo_gyro_Z: %d\r\n", pedo_fifo_data.p_gyro_data[i+2]);        
            }

            if(0 != fifo_size)
                TracerInfo("[SM_TC-2]: Gyro Data should not get here!! size: %d\r\n", fifo_size);


            get_data_size += fifo_size;
            
#ifdef SM_TEST_CASE_2_1
            if(get_data_size >= 100)
            {
                static uint32_t old_tick = 0;

                TracerInfo("[SM_TC-2]: nb: %d, tick_diff: %d\r\n", get_data_size, Hrm_get_sys_tick() - old_tick);

                old_tick = Hrm_get_sys_tick();

                get_data_size = 0;
            }
#endif

            cc6801_ClockDelayMs(10);
            
            
            

        }


        TracerInfo("[SM_TC-2]: PEDO get Gyro data done.\r\n");

        TracerInfo("[SM_TC-2]: CC_Sensor_Manager_Shutdown()\r\n");
        if(E_SEN_ERROR_NONE != (ret_code = CC_Sensor_Manager_Shutdown(E_SEN_TYPE_LSM6DS3_ACCEL_GYRO, E_SEN_USER_ID_PEDO)))
        {
            TracerInfo("[SM_TC-2]: CC_Sensor_Manager_Shutdown fail!\r\n");
        }


        if(E_SEN_ERROR_NONE != (ret_code = CC_Sensor_Manager_GetData(E_SEN_TYPE_LSM6DS3_ACCEL_GYRO, E_SEN_USER_ID_PEDO, &pedo_fifo_data, &pedo_fifo_sz)))
        //if(E_APP_SRV_ERR_NONE == CC_SenMgr_Gyro_GetData(E_SEN_USER_ID_PEDO, &pedo_fifo_data, &fifo_size))
        {
            TracerInfo("[SM_TC-2]: CC_Sensor_Manager_GetData() don't get any data after shutdown.\r\n");
                
        }
        else
        {
            TracerInfo("[SM_TC-2]: Gyro Data should not get here!! size: %d\r\n", fifo_size);
        }

        
        

    }

    TracerInfo("[SM_TC-2]: SM Test Case 2 pass!\r\n");

    
    

    while(1)
        ;

#elif defined(SM_TEST_CASE_3)
    
    #define TEST_CASE_ROUND_NB  10
    
        TracerInfo("[SM_TC-3] Start:\r\n");
    
        AxesRaw_t mag_data;
        uint16_t mag_size = 0;    
        AK09912_MAG_MODE_t mag_settings = AK09912_MAG_DO_10_Hz;

    
    
    
        for(uint8_t round = 0; round < TEST_CASE_ROUND_NB; round++)
        {
            TracerInfo("[SM_TC-3]: Test Round: %d\r\n", round);
    
    
            TracerInfo("[SM_TC-3]: CC_Sensor_Manager_Init()\r\n");
            if(E_SEN_ERROR_NONE != (ret_code = CC_Sensor_Manager_Init(E_SEN_TYPE_AK09912_MAG, E_SEN_USER_ID_SWIM)))
            {
                TracerInfo("[SM_TC-3]: CC_Sensor_Manager_Init fail!\r\n");
            
            }

    
            if(E_SEN_ERROR_NONE != (ret_code = CC_Sensor_Manager_Configure(E_SEN_TYPE_AK09912_MAG, E_SEN_USER_ID_SWIM, &mag_settings)))
            {
                TracerInfo("[SM_TC-3]: CC_Sensor_Manager_Configure fail!\r\n");
            }

    
            //Acc_Gyro_Dump_All_Settings();
    
            if(E_SEN_ERROR_NONE != (ret_code = CC_Sensor_Manager_Start(E_SEN_TYPE_AK09912_MAG, E_SEN_USER_ID_SWIM)))
            {
                TracerInfo("[SM_TC-3]: CC_Sensor_Manager_Start fail!\r\n");
            }     
    
            get_data_size = 0;
            
            
            while(get_data_size < MEMS_FIFO_SIZE)
            {

                //if(E_SEN_ERROR_NONE != (ret_code = CC_Sensor_Manager_GetData(E_SEN_TYPE_AK09912_MAG, E_SEN_USER_ID_SWIM, &mag_data, &mag_size)))
                if(E_SEN_ERROR_NONE != (ret_code = CC_SenMgr_Mag_GetData(E_SEN_USER_ID_SWIM, &mag_data, &fifo_size)))
                {
                    TracerInfo("[SM_TC-3]: CC_Sensor_Manager_GetData fail!\r\n");
                }  


                if(mag_size != 6)
                {
                    TracerInfo("Error Size: %d\r\n", fifo_size);
                }
                else
                {
                    TracerInfo("[SM_TC-3]: swim_mag_X: %d\r\n", mag_data.AXIS_X);
                    TracerInfo("[SM_TC-3]: swim_mag_Y: %d\r\n", mag_data.AXIS_Y);
                    TracerInfo("[SM_TC-3]: swim_mag_Z: %d\r\n", mag_data.AXIS_Z);  
                }            
                                 
    
                get_data_size += mag_size;
                

                cc6801_ClockDelayMs(10);
                
                
                
    
            }
    
    
            TracerInfo("[SM_TC-3]: Swim get Mag data done.\r\n");
    
            TracerInfo("[SM_TC-3]: CC_Sensor_Manager_Shutdown()\r\n");
            if(E_SEN_ERROR_NONE != (ret_code = CC_Sensor_Manager_Shutdown(E_SEN_TYPE_AK09912_MAG, E_SEN_USER_ID_SWIM)))
            {
                TracerInfo("[SM_TC-3]: CC_Sensor_Manager_Shutdown fail!\r\n");
            }
    
    
            //if(E_SEN_ERROR_NONE != (ret_code = CC_Sensor_Manager_GetData(E_SEN_TYPE_AK09912_MAG, E_SEN_USER_ID_SWIM, &mag_data, &mag_size)))
            if(E_SEN_ERROR_NONE != (ret_code = CC_SenMgr_Mag_GetData(E_SEN_USER_ID_SWIM, &mag_data, &fifo_size)))
            {
                TracerInfo("[SM_TC-3]: CC_Sensor_Manager_GetData() don't get any data after shutdown.\r\n");
                    
            }
            else
            {
                TracerInfo("[SM_TC-3]: Gyro Data should not get here!! size: %d\r\n", fifo_size);
            }
    
            
            
    
        }
    
        TracerInfo("[SM_TC-3]: SM Test Case 3 pass!\r\n");
    
        
        
    
        while(1)
            ;

        
#elif defined(SM_TEST_CASE_4)

        #define TOTAL_CHANNELS  3 //Using channel numbers 
        #define HEART_RATE_MODE_SAMPLES_PER_CH_READ (20)  //Numbers of PPG data per channel   
        #define HEART_RATE_MODE_SAMPLES_PER_READ (TOTAL_CHANNELS* HEART_RATE_MODE_SAMPLES_PER_CH_READ) 
        #define TEST_CASE_ROUND_NB  10

        
        uint32_t hrm_ppg_data[HEART_RATE_MODE_SAMPLES_PER_READ];
        uint32_t hrm_ppg_size = 0;          



        for(uint8_t round = 0; round < TEST_CASE_ROUND_NB; round++)
        {
            TracerInfo("[SM_TC-4]: Test Round: %d\r\n", round);
    
    
            TracerInfo("[SM_TC-4]: CC_Sensor_Manager_Init()\r\n");
            if(E_SEN_ERROR_NONE != (ret_code = CC_Sensor_Manager_Init(E_SEN_TYPE_Pah8002_PPG, E_SEN_USER_ID_HRM)))
            {
                TracerInfo("[SM_TC-4]: CC_Sensor_Manager_Init fail!\r\n");
            
            }
            
            
            if(E_SEN_ERROR_NONE != (ret_code = CC_Sensor_Manager_Configure(E_SEN_TYPE_Pah8002_PPG, E_SEN_USER_ID_HRM, NULL)))
            {
                TracerInfo("[SM_TC-4]: CC_Sensor_Manager_Configure fail!\r\n");
            }
            
            
            //Acc_Gyro_Dump_All_Settings();
            
            if(E_SEN_ERROR_NONE != (ret_code = CC_Sensor_Manager_Start(E_SEN_TYPE_Pah8002_PPG, E_SEN_USER_ID_HRM)))
            {
                TracerInfo("[SM_TC-4]: CC_Sensor_Manager_Start fail!\r\n");
            }   
    
            get_data_size = 0;
            
            
            while(get_data_size < MEMS_FIFO_SIZE)
            {

                if(E_APP_SRV_ERR_NONE == CC_SenMgr_PPG_GetData(E_SEN_USER_ID_HRM, hrm_ppg_data, &fifo_size))
                {
                    if(0x00 != fifo_size)
                    {
                        TracerInfo("hrm_acc_data:\r\n");        

                        for(uint16_t i = 0; i < fifo_size/sizeof(uint32_t); i+=3)
                        {
                            TracerInfo("hrm_ppg_ch1: %d\r\n", hrm_ppg_data[i]);        
                            TracerInfo("hrm_ppg_ch2: %d\r\n", hrm_ppg_data[i+1]);
                            TracerInfo("hrm_ppg_ch3: %d\r\n", hrm_ppg_data[i+2]);                    
                        }
                       

                        TracerInfo("\r\n");            
                    }            
                }        
                                 
    
                get_data_size += hrm_ppg_size;
                

                cc6801_ClockDelayMs(10);
                
                
                
    
            }
    
    
            TracerInfo("[SM_TC-4]: HRM get PPG data done.\r\n");
    
            TracerInfo("[SM_TC-4]: CC_Sensor_Manager_Shutdown()\r\n");
            if(E_SEN_ERROR_NONE != (ret_code = CC_Sensor_Manager_Shutdown(E_SEN_TYPE_Pah8002_PPG, E_SEN_USER_ID_HRM)))
            {
                TracerInfo("[SM_TC-4]: CC_Sensor_Manager_Shutdown fail!\r\n");
            }
    
    
            //if(E_SEN_ERROR_NONE != (ret_code = CC_Sensor_Manager_GetData(E_SEN_TYPE_Pah8002_PPG, E_SEN_USER_ID_HRM, &hrm_ppg_data, &hrm_ppg_size)))
            if(E_APP_SRV_ERR_NONE == CC_SenMgr_PPG_GetData(E_SEN_USER_ID_HRM, hrm_ppg_data, &fifo_size))
            {
                TracerInfo("[SM_TC-4]: CC_Sensor_Manager_GetData() don't get any data after shutdown.\r\n");
                    
            }
            else
            {
                TracerInfo("[SM_TC-4]: Gyro Data should not get here!! size: %d\r\n", fifo_size);
            }
    
            
            
    
        }
    
        TracerInfo("[SM_TC-4]: SM Test Case 4 pass!\r\n");
    
        
        
    
        while(1)
            ;
    
                
#elif defined(SM_TEST_CASE_5)   

    #define TEST_CASE_ROUND_NB  10

    TracerInfo("[SM_TC-5] Start:\r\n");

    T_Accel_Gyro_Settings pedo_settings =
    {
        .AccessMode = E_ACC_GYRO_MODE_FIFO,                     // fifo mode
        .FifoTarget = E_LSM6DSX_FIFO_CONTROL_ACCEL_GYRO,       // fifo for Accel/Gyro
        
        .S_Gyro.FifoSet.nFifoDepth = MEMS_FIFO_SIZE,            // fifo size: 100
        .S_Gyro.FifoSet.Odr = LSM6DS3_ACC_GYRO_ODR_FIFO_50Hz,  //52Hz
        .S_Gyro.FifoSet.iSampleData = pedo_gyro_tmpbuf,

        .S_Accel.FifoSet.nFifoDepth = MEMS_FIFO_SIZE,     // fifo size: 100
        .S_Accel.FifoSet.Odr = LSM6DS3_ACC_GYRO_ODR_FIFO_25Hz,  //25Hz
        .S_Accel.FifoSet.iSampleData = pedo_acc_tmpbuf,        
    };



    for(uint8_t round = 0; round < TEST_CASE_ROUND_NB; round++)
    {
        TracerInfo("[SM_TC-5]: Test Round: %d\r\n", round);


        TracerInfo("[SM_TC-5]: CC_Sensor_Manager_Init()\r\n");
        if(E_SEN_ERROR_NONE != (ret_code = CC_Sensor_Manager_Init(E_SEN_TYPE_LSM6DS3_ACCEL_GYRO, E_SEN_USER_ID_PEDO)))
        {
            TracerInfo("[SM_TC-5]: CC_Sensor_Manager_Init fail!\r\n");

        }    

        TracerInfo("[SM_TC-5]: CC_Sensor_Manager_Configure()\r\n");
        if(E_SEN_ERROR_NONE != (ret_code = CC_Sensor_Manager_Configure(E_SEN_TYPE_LSM6DS3_ACCEL_GYRO, E_SEN_USER_ID_PEDO, &pedo_settings)))
        {
            TracerInfo("[SM_TC-5]: CC_Sensor_Manager_Configure fail!\r\n");
        }

        Acc_Gyro_Dump_All_Settings();


        TracerInfo("[SM_TC-5]: CC_Sensor_Manager_Start()\r\n");
        if(E_SEN_ERROR_NONE != (ret_code = CC_Sensor_Manager_Start(E_SEN_TYPE_LSM6DS3_ACCEL_GYRO, E_SEN_USER_ID_PEDO)))
        {
            TracerInfo("[SM_TC-5]: CC_Sensor_Manager_Start fail!\r\n");
        }    

        get_data_size = 0;
        
        
        while(get_data_size < MEMS_FIFO_SIZE)
        {
            //if(E_SEN_ERROR_NONE != (ret_code = CC_Sensor_Manager_GetData(E_SEN_TYPE_LSM6DS3_ACCEL_GYRO, E_SEN_USER_ID_PEDO, &pedo_fifo_data, &pedo_fifo_sz)))
            if(E_APP_SRV_ERR_NONE == CC_SenMgr_PPG_GetData(E_SEN_USER_ID_PEDO, &pedo_fifo_data, &pedo_fifo_sz))
            {
                TracerInfo("[SM_TC-5]: CC_Sensor_Manager_GetData fail!\r\n");
                
            }


            for(uint16_t i = 0; i < pedo_fifo_sz.accel_SzInBytes/sizeof(int16_t); i+=3)
            {
                if((pedo_fifo_sz.accel_SzInBytes/2) < 3)
                     TracerInfo("[SM_TC-5]: Error Size: %d\r\n", pedo_fifo_sz.accel_SzInBytes);
            
                TracerInfo("[SM_TC-5]: pedo_accel_X: %d\r\n", pedo_fifo_data.p_accel_data[i]);
                TracerInfo("[SM_TC-5]: pedo_accel_Y: %d\r\n", pedo_fifo_data.p_accel_data[i+1]);
                TracerInfo("[SM_TC-5]: pedo_accel_Z: %d\r\n", pedo_fifo_data.p_accel_data[i+2]);        
            }

            
            for(uint16_t i = 0; i < pedo_fifo_sz.gyro_SzInBytes/sizeof(int16_t); i+=3)
            {
                if((pedo_fifo_sz.gyro_SzInBytes/2) < 3)
                     TracerInfo("[SM_TC-5]: Error Size: %d\r\n", pedo_fifo_sz.gyro_SzInBytes);
            
                TracerInfo("[SM_TC-5]: pedo_gyro_X: %d\r\n", pedo_fifo_data.p_gyro_data[i]);
                TracerInfo("[SM_TC-5]: pedo_gyro_Y: %d\r\n", pedo_fifo_data.p_gyro_data[i+1]);
                TracerInfo("[SM_TC-5]: pedo_gyro_Z: %d\r\n", pedo_fifo_data.p_gyro_data[i+2]);        
            }


            get_data_size += pedo_fifo_sz.gyro_SzInBytes;
            
            cc6801_ClockDelayMs(10);
            
            
            

        }


        TracerInfo("[SM_TC-5]: PEDO get Gyro data done.\r\n");

        TracerInfo("[SM_TC-5]: CC_Sensor_Manager_Shutdown()\r\n");
        if(E_SEN_ERROR_NONE != (ret_code = CC_Sensor_Manager_Shutdown(E_SEN_TYPE_LSM6DS3_ACCEL_GYRO, E_SEN_USER_ID_PEDO)))
        {
            TracerInfo("[SM_TC-5]: CC_Sensor_Manager_Shutdown fail!\r\n");
        }


        //if(E_SEN_ERROR_NONE != (ret_code = CC_Sensor_Manager_GetData(E_SEN_TYPE_LSM6DS3_ACCEL_GYRO, E_SEN_USER_ID_PEDO, &pedo_fifo_data, &pedo_fifo_sz)))
        if(E_APP_SRV_ERR_NONE == CC_SenMgr_PPG_GetData(E_SEN_USER_ID_PEDO, &pedo_fifo_data, &pedo_fifo_sz))
        {
            TracerInfo("[SM_TC-5]: CC_Sensor_Manager_GetData() don't get any data after shutdown.\r\n");
                
        }
        else
        {
            TracerInfo("[SM_TC-5]: Gyro Data should not get here!! size: %d\r\n", pedo_fifo_sz.gyro_SzInBytes);
        }

        
        

    }

    TracerInfo("[SM_TC-5]: SM Test Case 5 pass!\r\n");

    
    

    while(1)
        ;


    
#else   // Old Test Case:

    AxesRaw_t mag_data;
    uint16_t mag_size = 0;    
    AK09912_MAG_MODE_t mag_settings = AK09912_MAG_DO_10_Hz;
   

    T_Accel_Gyro_Settings hrm_settings =
    {
        .AccessMode = E_ACC_GYRO_MODE_FIFO,               // fifo mode
        .FifoTarget = E_LSM6DSX_FIFO_CONTROL_ACCEL,       // fifo for Accel
        
        .S_Accel.FifoSet.nFifoDepth = MEMS_FIFO_SIZE,     // fifo size: 100
        .S_Accel.FifoSet.Odr = LSM6DS3_ACC_GYRO_ODR_FIFO_25Hz,  //25Hz
        .S_Accel.FifoSet.iSampleData = hrm_acc_tmpbuf,
    };

    T_Accel_Gyro_Settings pedo_settings = 
    {
        .AccessMode = E_ACC_GYRO_MODE_FIFO,                     // fifo mode
        //.FifoTarget = E_LSM6DSX_FIFO_CONTROL_ACCEL_GYRO,        // fifo for Accel/Gyro
        .FifoTarget = E_LSM6DSX_FIFO_CONTROL_GYRO,        // fifo for Gyro

         // Accel settings:
        .S_Accel.FifoSet.nFifoDepth = MEMS_FIFO_SIZE,           // fifo size: 100
        .S_Accel.FifoSet.Odr = LSM6DS3_ACC_GYRO_ODR_FIFO_25Hz,  // 25 Hz
        .S_Accel.FifoSet.iSampleData = pedo_acc_tmpbuf,

        // Gyro settings:
        .S_Gyro.FifoSet.nFifoDepth = MEMS_FIFO_SIZE,            // fifo size: 100
        .S_Gyro.FifoSet.Odr = LSM6DS3_ACC_GYRO_ODR_FIFO_50Hz,   // 50 Hz ==> different ODR
        .S_Gyro.FifoSet.iSampleData = pedo_gyro_tmpbuf,
    };
        

    // Init:    
    //if(E_SEN_ERROR_NONE != (ret_code = CC_Sensor_Manager_Init(E_SEN_TYPE_LSM6DS3_ACCEL_GYRO, E_SEN_USER_ID_HRM)))
    if(E_SEN_ERROR_NONE != (ret_code = CC_Sensor_Manager_Init(E_SEN_TYPE_LSM6DS3_ACCEL_GYRO, E_SEN_USER_ID_PEDO)))
    {
        TracerInfo("CC_Sensor_Manager_Init fail!\r\n");

    }

    if(E_SEN_ERROR_NONE != (ret_code = CC_Sensor_Manager_Init(E_SEN_TYPE_AK09912_MAG, E_SEN_USER_ID_SWIM)))
    {
        TracerInfo("CC_Sensor_Manager_Init fail!\r\n");

    }


    // Configuration:
/*    
    if(E_SEN_ERROR_NONE != (ret_code = CC_Sensor_Manager_Configure(E_SEN_TYPE_LSM6DS3_ACCEL_GYRO, E_SEN_USER_ID_HRM, &hrm_settings)))
    {
        TracerInfo("CC_Sensor_Manager_Configure fail!\r\n");
    }
 */   
    if(E_SEN_ERROR_NONE != (ret_code = CC_Sensor_Manager_Configure(E_SEN_TYPE_LSM6DS3_ACCEL_GYRO, E_SEN_USER_ID_PEDO, &pedo_settings)))
    {
        TracerInfo("CC_Sensor_Manager_Configure fail!\r\n");
    }


    if(E_SEN_ERROR_NONE != (ret_code = CC_Sensor_Manager_Configure(E_SEN_TYPE_AK09912_MAG, E_SEN_USER_ID_SWIM, &mag_settings)))
    {
        TracerInfo("CC_Sensor_Manager_Configure fail!\r\n");
    }
    

    Acc_Gyro_Dump_All_Settings();


    // Start:
    if(E_SEN_ERROR_NONE != (ret_code = CC_Sensor_Manager_Start(E_SEN_TYPE_LSM6DS3_ACCEL_GYRO, E_SEN_USER_ID_PEDO)))
    {
        TracerInfo("CC_Sensor_Manager_Start fail!\r\n");
    }        

    if(E_SEN_ERROR_NONE != (ret_code = CC_Sensor_Manager_Start(E_SEN_TYPE_AK09912_MAG, E_SEN_USER_ID_SWIM)))
    {
        TracerInfo("CC_Sensor_Manager_Start fail!\r\n");
    }   


    

    // Get Data:
    //while(get_data_size < MEMS_FIFO_SIZE) // get data until hrm fifo full!
    while(1)
    {
#if 1 // mode 1:
/*
        if(E_SEN_ERROR_NONE != (ret_code = CC_Sensor_Manager_GetData(E_SEN_TYPE_LSM6DS3_ACCEL_GYRO, E_SEN_USER_ID_HRM, &hrm_fifo_data, &hrm_fifo_sz)))
        {
            TracerInfo("CC_Sensor_Manager_GetData(HRM) fail!\r\n");
        }
*/        
        if(E_SEN_ERROR_NONE != (ret_code = CC_Sensor_Manager_GetData(E_SEN_TYPE_LSM6DS3_ACCEL_GYRO, E_SEN_USER_ID_PEDO, &pedo_fifo_data, &pedo_fifo_sz)))
        {
            TracerInfo("CC_Sensor_Manager_GetData(PEDO) fail!\r\n");
        }        
#else  // mode 2:
        if(E_SEN_ERROR_NONE != (ret_code = CC_Sensor_Manager_GetData(E_SEN_TYPE_LSM6DS3_ACCEL_GYRO, E_SEN_USER_ID_HRM, NULL, &hrm_fifo_sz)))
        {
            TracerInfo("CC_Sensor_Manager_GetData(HRM) fail!\r\n");
        }
        
        if(E_SEN_ERROR_NONE != (ret_code = CC_Sensor_Manager_GetData(E_SEN_TYPE_LSM6DS3_ACCEL_GYRO, E_SEN_USER_ID_PEDO, NULL, &pedo_fifo_sz)))
        {
            TracerInfo("CC_Sensor_Manager_GetData(PEDO) fail!\r\n");
        }                
#endif

        get_data_size += hrm_fifo_sz.accel_SzInBytes;

/*
        for(uint16_t i = 0; i < hrm_fifo_sz.accel_SzInBytes/sizeof(int16_t); i+=3)
        {
            if((hrm_fifo_sz.accel_SzInBytes/2) < 3)
                 TracerInfo("Error Size: %d\r\n", hrm_fifo_sz.accel_SzInBytes);
        
            TracerInfo("hrm_accel_X: %d\r\n", hrm_fifo_data.p_accel_data[i]);
            TracerInfo("hrm_accel_Y: %d\r\n", hrm_fifo_data.p_accel_data[i+1]);
            TracerInfo("hrm_accel_Z: %d\r\n", hrm_fifo_data.p_accel_data[i+2]);        
        }
        
        for(uint16_t i = 0; i < hrm_fifo_sz.gyro_SzInBytes/sizeof(int16_t); i+=3)
        {
            if((hrm_fifo_sz.gyro_SzInBytes/2) < 3)
                 TracerInfo("Error Size: %d\r\n", hrm_fifo_sz.gyro_SzInBytes);

        
            TracerInfo("hrm_gyro_X: %d\r\n", hrm_fifo_data.p_gyro_data[i]);
            TracerInfo("hrm_gyro_Y: %d\r\n", hrm_fifo_data.p_gyro_data[i+1]);
            TracerInfo("hrm_gyro_Z: %d\r\n", hrm_fifo_data.p_gyro_data[i+2]);        
        }
*/        
        for(uint16_t i = 0; i < pedo_fifo_sz.accel_SzInBytes/sizeof(int16_t); i+=3)
        {
            if((pedo_fifo_sz.accel_SzInBytes/2) < 3)
                 TracerInfo("Error Size: %d\r\n", pedo_fifo_sz.accel_SzInBytes);

        
            TracerInfo("pedo_accel_X: %d\r\n", pedo_fifo_data.p_accel_data[i]);
            TracerInfo("pedo_accel_Y: %d\r\n", pedo_fifo_data.p_accel_data[i+1]);
            TracerInfo("pedo_accel_Z: %d\r\n", pedo_fifo_data.p_accel_data[i+2]);        
        }
        
        for(uint16_t i = 0; i < pedo_fifo_sz.gyro_SzInBytes/sizeof(int16_t); i+=3)
        {
            if((pedo_fifo_sz.gyro_SzInBytes/2) < 3)
                 TracerInfo("Error Size: %d\r\n", pedo_fifo_sz.gyro_SzInBytes);

        
            TracerInfo("pedo_gyro_X: %d\r\n", pedo_fifo_data.p_gyro_data[i]);
            TracerInfo("pedo_gyro_Y: %d\r\n", pedo_fifo_data.p_gyro_data[i+1]);
            TracerInfo("pedo_gyro_Z: %d\r\n", pedo_fifo_data.p_gyro_data[i+2]);        
        }



        if(E_SEN_ERROR_NONE != (ret_code = CC_Sensor_Manager_GetData(E_SEN_TYPE_AK09912_MAG, E_SEN_USER_ID_SWIM, &mag_data, &mag_size)))
        {
            TracerInfo("CC_Sensor_Manager_GetData(SWIM) fail!\r\n");
        }  


        if(mag_size != 6)
        {
            TracerInfo("Error Size: %d\r\n", mag_size);
        }
        else
        {
            TracerInfo("swim_mag_X: %d\r\n", mag_data.AXIS_X);
            TracerInfo("swim_mag_Y: %d\r\n", mag_data.AXIS_Y);
            TracerInfo("swim_mag_Z: %d\r\n", mag_data.AXIS_Z);  
        }


        
        cc6801_ClockDelayMs(10);
    };





    // Shutdown:
    if(E_SEN_ERROR_NONE != (ret_code = CC_Sensor_Manager_Shutdown(E_SEN_TYPE_LSM6DS3_ACCEL_GYRO, E_SEN_USER_ID_HRM)))
    {
        TracerInfo("CC_Sensor_Manager_Shutdown(HRM) fail!\r\n");
    }
    
    if(E_SEN_ERROR_NONE != (ret_code = CC_Sensor_Manager_Shutdown(E_SEN_TYPE_LSM6DS3_ACCEL_GYRO, E_SEN_USER_ID_PEDO)))    
    {
        TracerInfo("CC_Sensor_Manager_Shutdown(PEDO) fail!\r\n");
    }        

#endif


    
}
#endif

#endif
