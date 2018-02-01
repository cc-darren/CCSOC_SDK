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

#ifndef _ACC_GYRO_CONTROLLER_
#define _ACC_GYRO_CONTROLLER_

#include "error.h"
#include "CC_Sensor_Manager.h"
#include "acc_lsm6ds3.h"

typedef enum
{
    E_ACC_GYRO_SUCCESS = CC_SUCCESS,
    E_ACC_GYRO_ERROR_INIT_FAIL,      
    E_ACC_GYRO_ERROR_NOT_INIT,      
    E_ACC_GYRO_ERROR_SENSOR_IN_USE,
    E_ACC_GYRO_ERROR_REPEAT_CONFIG,
    E_ACC_GYRO_ERROR_INVALID_CONTROL_TARGET,    
    E_ACC_GYRO_ERROR_INVALID_ACCESS_MODE,
    

}E_Accel_Gyro_Manager_Status;

typedef enum
{
    LSM6DS3_POLL_MODE_NOT_CONFIG   = 0x00,
    LSM6DS3_POLL_SINGLE_MODE_ACCEL = 0x01,
    LSM6DS3_POLL_SINGLE_MODE_GYRO  = 0x02,    
    LSM6DS3_POLL_DUAL_MODE         = 0x03,    
} CC_LSM6DSX_Poll_Mode_t;

typedef enum
{
    E_LSM6DSX_FIFO_CONTROL_NONE = 0x00,
    E_LSM6DSX_FIFO_CONTROL_ACCEL = 0x01,
    E_LSM6DSX_FIFO_CONTROL_GYRO,
    E_LSM6DSX_FIFO_CONTROL_ACCEL_GYRO
}   E_LSM6DSX_FIFO_TARGET_DEVICE;


typedef struct 
{
    uint16_t nFifoDepth;    // max number in 16bits
    LSM6DS3_ACC_GYRO_ODR_FIFO_t Odr;
    int16_t *iSampleData;   // data buffer 
    
}S_CC_LSM6DSX_Fifo_Settings;


typedef struct 
{
    //LSM6DS3_ACC_GYRO_ODR_G_t Odr;
    uint8_t Odr; // accel => use LSM6DS3_ACC_GYRO_ODR_XL_t;  gyro => use LSM6DS3_ACC_GYRO_ODR_G_t
    int16_t *iSampleData;   // data buffer
}S_CC_LSM6DSX_Poll_Settings;

typedef struct
{   
    bool     isRegistered;
    uint32_t nSamples;  // number of (x+y+z)
    uint16_t nFifoDepth; // in 16bits
    uint16_t PeriodInHz;
    int16_t *fifo_data;
} CC_LSM6DSX_Fifo_Settings_t;

typedef struct
{   
    CC_LSM6DSX_Fifo_Settings_t Accel;
    CC_LSM6DSX_Fifo_Settings_t Gyro;
    
} CC_LSM6DSX_Fifo_Handle_t;

typedef struct
{   
    bool isConfigured;
    E_LSM6DSX_FIFO_TARGET_DEVICE ControlTarget;
    LSM6DS3_ACC_GYRO_ODR_FIFO_t OdrLevel;
    u16_t OdrHzVal;
    uint32_t SampleCount;
    
} CC_Lsm6dsx_Fifo_Configure_t;


typedef enum
{
    E_ACC_GYRO_MODE_NOT_CONFIG    = 0,     
    E_ACC_GYRO_MODE_POLL          = 1,
    E_ACC_GYRO_MODE_FIFO          = 2,        

}E_Accel_Gyro_Access_Mode;


typedef struct 
{
    S_CC_LSM6DSX_Fifo_Settings  FifoSet;
    S_CC_LSM6DSX_Poll_Settings  PollSet;
}T_Accel_Gyro_Target_Settings ;


typedef struct 
{
    E_Accel_Gyro_Access_Mode        AccessMode;    
    E_LSM6DSX_FIFO_TARGET_DEVICE    FifoTarget;    
    CC_LSM6DSX_Poll_Mode_t          PollTarget;
    T_Accel_Gyro_Target_Settings    S_Accel;
    T_Accel_Gyro_Target_Settings    S_Gyro;   
}T_Accel_Gyro_Settings;


typedef struct 
{
    int16_t *p_accel_data;
    int16_t *p_gyro_data;
    uint16_t accel_nSamples;    
    uint16_t gyro_nSamples;
}T_Accel_Gyro_Fifo_Data;

typedef struct 
{
    uint32_t accel_SzInBytes;
    uint32_t gyro_SzInBytes;
}T_Accel_Gyro_Fifo_Data_Bytes;


E_Accel_Gyro_Manager_Status Acc_Gyro_Controller_Init(void);
E_Accel_Gyro_Manager_Status Acc_Gyro_Controller_Configure(E_Sensor_User_ID UserID, void *Params);
E_Accel_Gyro_Manager_Status Acc_Gyro_Controller_Start(E_Sensor_User_ID UserID);
E_Accel_Gyro_Manager_Status Acc_Gyro_Controller_GetData(E_Sensor_User_ID UserID, void* pSampleData, void *pDataSzInBytes);
E_Accel_Gyro_Manager_Status Acc_Gyro_Controller_Shutdown(E_Sensor_User_ID UserID, bool power_off);
const struct sensor_manager_itfs* acc_gyro_sm_itf_get(void);



void Acc_Gyro_Dump_All_Settings(void);

#endif // END of _ACC_GYRO_CONTROLLER_


#endif
