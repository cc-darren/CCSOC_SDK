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
#include "Acc_Gyro_Controller.h"
#include "acc_lsm6ds3.h"
#include "CC_AppSrvc_Manager.h"
#include "CC_Sensor_Manager.h"

#include "error.h"
#include "tracer.h"

//#define LSM6DSX_FIFO_MAX_HANDLE        8



typedef struct 
{
    int16_t accel_data[3];
    int16_t gyro_data[3];
}T_Accel_Gyro_Hybrid_Data;






static T_Accel_Gyro_Settings g_Accel_Gyro_Settings[E_SEN_USER_ID_TOTAL];


//////////////////// LSM6DSX Data Dispatch  /////////////////////////////////////////

static CC_Lsm6dsx_Fifo_Configure_t Lsm6dsx_Fifo_Config; 
CC_LSM6DSX_Fifo_Handle_t S_Lsm6dsx_Fifo_Handle[E_SEN_USER_ID_TOTAL];



static LSM6DS3_ACC_GYRO_ODR_G_t CC_LSM6DSX_Fifo2Gyro_ODR(LSM6DS3_ACC_GYRO_ODR_FIFO_t fifo_odr)
{
    LSM6DS3_ACC_GYRO_ODR_G_t gyro_odr;

    switch(fifo_odr) 
    {
          case LSM6DS3_ACC_GYRO_ODR_FIFO_10Hz:
            gyro_odr = LSM6DS3_ACC_GYRO_ODR_G_13Hz;
            break;
          
          case LSM6DS3_ACC_GYRO_ODR_FIFO_25Hz:
            gyro_odr = LSM6DS3_ACC_GYRO_ODR_G_26Hz;
            break;
          
          case LSM6DS3_ACC_GYRO_ODR_FIFO_50Hz:
            gyro_odr = LSM6DS3_ACC_GYRO_ODR_G_52Hz;
            break;
          
          case LSM6DS3_ACC_GYRO_ODR_FIFO_100Hz:
            gyro_odr = LSM6DS3_ACC_GYRO_ODR_G_104Hz;
            break;
          
          case LSM6DS3_ACC_GYRO_ODR_FIFO_200Hz:
            gyro_odr = LSM6DS3_ACC_GYRO_ODR_G_208Hz;
            break;
          
          case LSM6DS3_ACC_GYRO_ODR_FIFO_400Hz:
            gyro_odr = LSM6DS3_ACC_GYRO_ODR_G_416Hz;
            break;
          
          case LSM6DS3_ACC_GYRO_ODR_FIFO_800Hz:
            gyro_odr = LSM6DS3_ACC_GYRO_ODR_G_833Hz;
            break;
          
          case LSM6DS3_ACC_GYRO_ODR_FIFO_1600Hz:
            gyro_odr = LSM6DS3_ACC_GYRO_ODR_G_1660Hz;
            break;          
          
          default:
            gyro_odr = LSM6DS3_ACC_GYRO_ODR_G_POWER_DOWN;
    }

    return gyro_odr;
}


static LSM6DS3_ACC_GYRO_ODR_XL_t CC_LSM6DSX_Fifo2Accel_ODR(LSM6DS3_ACC_GYRO_ODR_FIFO_t fifo_odr)
{
    LSM6DS3_ACC_GYRO_ODR_XL_t accel_odr;

    switch(fifo_odr) 
    {
        case LSM6DS3_ACC_GYRO_ODR_FIFO_10Hz:
          accel_odr = LSM6DS3_ACC_GYRO_ODR_XL_13Hz;
          break;
    
        case LSM6DS3_ACC_GYRO_ODR_FIFO_25Hz:
          accel_odr = LSM6DS3_ACC_GYRO_ODR_XL_26Hz;
          break;
    
        case LSM6DS3_ACC_GYRO_ODR_FIFO_50Hz:
          accel_odr = LSM6DS3_ACC_GYRO_ODR_XL_52Hz;
          break;
    
        case LSM6DS3_ACC_GYRO_ODR_FIFO_100Hz:
          accel_odr = LSM6DS3_ACC_GYRO_ODR_XL_104Hz;
          break;
    
        case LSM6DS3_ACC_GYRO_ODR_FIFO_200Hz:
          accel_odr = LSM6DS3_ACC_GYRO_ODR_XL_208Hz;
          break;
    
        case LSM6DS3_ACC_GYRO_ODR_FIFO_400Hz:
          accel_odr = LSM6DS3_ACC_GYRO_ODR_XL_416Hz;
          break;
    
        case LSM6DS3_ACC_GYRO_ODR_FIFO_800Hz:
          accel_odr = LSM6DS3_ACC_GYRO_ODR_XL_833Hz;
          break;
    
        case LSM6DS3_ACC_GYRO_ODR_FIFO_1600Hz:
          accel_odr = LSM6DS3_ACC_GYRO_ODR_XL_1660Hz;
          break;
    
        case LSM6DS3_ACC_GYRO_ODR_FIFO_3300Hz:
          accel_odr = LSM6DS3_ACC_GYRO_ODR_XL_3330Hz;
          break;
    
        case LSM6DS3_ACC_GYRO_ODR_FIFO_6600Hz:
          accel_odr = LSM6DS3_ACC_GYRO_ODR_XL_6660Hz;
          break;
    
        case LSM6DS3_ACC_GYRO_ODR_FIFO_13300Hz:
          accel_odr = LSM6DS3_ACC_GYRO_ODR_XL_13330Hz;
          break;
    
        default:
          accel_odr = LSM6DS3_ACC_GYRO_ODR_XL_POWER_DOWN;
    }

    return accel_odr;

}    

u16_t LSM6DSX_ACC_GYRO_translate_ODR_Fifo(LSM6DS3_ACC_GYRO_ODR_FIFO_t value)
{
  u16_t odr_hz_val;
  
  switch(value) 
  {
      case LSM6DS3_ACC_GYRO_ODR_FIFO_10Hz:
        odr_hz_val = 10;
        break;

      case LSM6DS3_ACC_GYRO_ODR_FIFO_25Hz:
        odr_hz_val = 25;
        break;

      case LSM6DS3_ACC_GYRO_ODR_FIFO_50Hz:
        odr_hz_val = 50;
        break;

      case LSM6DS3_ACC_GYRO_ODR_FIFO_100Hz:
        odr_hz_val = 100;
        break;

      case LSM6DS3_ACC_GYRO_ODR_FIFO_200Hz:
        odr_hz_val = 200;
        break;

      case LSM6DS3_ACC_GYRO_ODR_FIFO_400Hz:
        odr_hz_val = 400;
        break;

      case LSM6DS3_ACC_GYRO_ODR_FIFO_800Hz:
        odr_hz_val = 800;
        break;

      case LSM6DS3_ACC_GYRO_ODR_FIFO_1600Hz:
        odr_hz_val = 1600;
        break;

      case LSM6DS3_ACC_GYRO_ODR_FIFO_3300Hz:
        odr_hz_val = 3300;
        break;

      case LSM6DS3_ACC_GYRO_ODR_FIFO_6600Hz:
        odr_hz_val = 6600;
        break;

      case LSM6DS3_ACC_GYRO_ODR_FIFO_13300Hz:
        odr_hz_val = 13300;
        break;

      default:
        odr_hz_val = 0;
  }

  return odr_hz_val;
}


void CC_LSM6DSX_FifoEnable(void)
{
    E_LSM6DSX_FIFO_TARGET_DEVICE eTargetDevice = Lsm6dsx_Fifo_Config.ControlTarget;
    LSM6DS3_ACC_GYRO_ODR_FIFO_t fifo_odr = Lsm6dsx_Fifo_Config.OdrLevel;

    
    CC_LSM6DSX_GyroPowerDown ();
    CC_LSM6DSX_AccelPowerDown();

    CC_LSM6DSX_FifoClean();

    LSM6DS3_ACC_GYRO_W_FIFO_MODE(0x00, LSM6DS3_ACC_GYRO_FIFO_MODE_BYPASS);

    CC_LSM6DSX_GyroPowerON (CC_LSM6DSX_Fifo2Gyro_ODR(fifo_odr));
    CC_LSM6DSX_AccelPowerON(CC_LSM6DSX_Fifo2Accel_ODR(fifo_odr));

    _CC_LSM6DSX_RegWrite(LSM6DS3_ACC_GYRO_FIFO_CTRL5, LSM6DS3_ACC_GYRO_ODR_FIFO_50Hz);


    switch (eTargetDevice)
    {
        case E_LSM6DSX_FIFO_CONTROL_ACCEL_GYRO:
             _CC_LSM6DSX_RegWrite(LSM6DS3_ACC_GYRO_FIFO_CTRL3,
                                 (LSM6DS3_ACC_GYRO_DEC_FIFO_G_NO_DECIMATION | LSM6DS3_ACC_GYRO_DEC_FIFO_XL_NO_DECIMATION));
             break;

        case E_LSM6DSX_FIFO_CONTROL_ACCEL:               
             LSM6DS3_ACC_GYRO_W_DEC_FIFO_XL(0x00, LSM6DS3_ACC_GYRO_DEC_FIFO_XL_NO_DECIMATION);
             break;

        case E_LSM6DSX_FIFO_CONTROL_GYRO:
             LSM6DS3_ACC_GYRO_W_DEC_FIFO_G(0x00, LSM6DS3_ACC_GYRO_DEC_FIFO_G_NO_DECIMATION);
             break;
    }

    _CC_LSM6DSX_RegWrite(LSM6DS3_ACC_GYRO_FIFO_CTRL5,
                        (fifo_odr | LSM6DS3_ACC_GYRO_FIFO_MODE_DYN_STREAM_2));
}

void CC_LSM6DSX_FifoDisable(E_LSM6DSX_FIFO_TARGET_DEVICE eTargetDevice)
//void CC_LSM6DSX_FifoDisable(void)
{
    //E_LSM6DSX_FIFO_TARGET_DEVICE eTargetDevice = Lsm6dsx_Fifo_Config.ControlTarget;
        
    switch (eTargetDevice)
    {
        case E_LSM6DSX_FIFO_CONTROL_ACCEL_GYRO:
             _CC_LSM6DSX_RegWrite(LSM6DS3_ACC_GYRO_FIFO_CTRL3,
                                 (LSM6DS3_ACC_GYRO_DEC_FIFO_G_DATA_NOT_IN_FIFO | LSM6DS3_ACC_GYRO_DEC_FIFO_XL_DATA_NOT_IN_FIFO));
             break;

        case E_LSM6DSX_FIFO_CONTROL_ACCEL:
             LSM6DS3_ACC_GYRO_W_DEC_FIFO_XL(0x00, LSM6DS3_ACC_GYRO_DEC_FIFO_XL_DATA_NOT_IN_FIFO);
             break;

        case E_LSM6DSX_FIFO_CONTROL_GYRO:
             LSM6DS3_ACC_GYRO_W_DEC_FIFO_G(0x00, LSM6DS3_ACC_GYRO_DEC_FIFO_G_DATA_NOT_IN_FIFO);
             break;
    }

    LSM6DS3_ACC_GYRO_W_FIFO_MODE(0x00, LSM6DS3_ACC_GYRO_FIFO_MODE_BYPASS);
}



E_LSM6DSX_FIFO_TARGET_DEVICE CC_LSM6DSX_Fifo_Get_Control_Target(void)
{
    return Lsm6dsx_Fifo_Config.ControlTarget;
}

bool CC_LSM6DSX_Fifo_Is_Configured(void)
{
    return Lsm6dsx_Fifo_Config.isConfigured;
}

void CC_LSM6DSX_Fifo_Configure(E_LSM6DSX_FIFO_TARGET_DEVICE control_target, LSM6DS3_ACC_GYRO_ODR_FIFO_t odr)
{
    Lsm6dsx_Fifo_Config.isConfigured = true;
    Lsm6dsx_Fifo_Config.ControlTarget = control_target;
    Lsm6dsx_Fifo_Config.OdrLevel = odr;
    Lsm6dsx_Fifo_Config.OdrHzVal = LSM6DSX_ACC_GYRO_translate_ODR_Fifo(odr);
    Lsm6dsx_Fifo_Config.SampleCount = 0;

}


bool CC_LSM6DSX_Fifo_Accel_Register(uint8_t handle, S_CC_LSM6DSX_Fifo_Settings *fifo_set)
{
    
    if(fifo_set->Odr > Lsm6dsx_Fifo_Config.OdrHzVal)
        return false;

    S_Lsm6dsx_Fifo_Handle[handle].Accel.isRegistered = true;
    S_Lsm6dsx_Fifo_Handle[handle].Accel.nSamples = 0;
    S_Lsm6dsx_Fifo_Handle[handle].Accel.nFifoDepth = fifo_set->nFifoDepth;
    S_Lsm6dsx_Fifo_Handle[handle].Accel.PeriodInHz = LSM6DSX_ACC_GYRO_translate_ODR_Fifo(fifo_set->Odr);
    S_Lsm6dsx_Fifo_Handle[handle].Accel.fifo_data = fifo_set->iSampleData;

    return true;
}


bool CC_LSM6DSX_Fifo_Gyro_Register(uint8_t handle, S_CC_LSM6DSX_Fifo_Settings *fifo_set)
{

    if(fifo_set->Odr > Lsm6dsx_Fifo_Config.OdrHzVal)
        return false;

    S_Lsm6dsx_Fifo_Handle[handle].Gyro.isRegistered = true;
    S_Lsm6dsx_Fifo_Handle[handle].Gyro.nSamples = 0;
    S_Lsm6dsx_Fifo_Handle[handle].Gyro.nFifoDepth = fifo_set->nFifoDepth;
    S_Lsm6dsx_Fifo_Handle[handle].Gyro.PeriodInHz = LSM6DSX_ACC_GYRO_translate_ODR_Fifo(fifo_set->Odr);
    S_Lsm6dsx_Fifo_Handle[handle].Gyro.fifo_data = fifo_set->iSampleData;    

    return true;
}

void CC_LSM6DSX_Fifo_Accel_UnRegister(uint8_t handle)
{
    S_Lsm6dsx_Fifo_Handle[handle].Accel.isRegistered = false;
    S_Lsm6dsx_Fifo_Handle[handle].Accel.nSamples = 0;
    S_Lsm6dsx_Fifo_Handle[handle].Accel.fifo_data = 0;

}

void CC_LSM6DSX_Fifo_Gyro_UnRegister(uint8_t handle)
{
    S_Lsm6dsx_Fifo_Handle[handle].Gyro.isRegistered = false;
    S_Lsm6dsx_Fifo_Handle[handle].Gyro.nSamples = 0;
    S_Lsm6dsx_Fifo_Handle[handle].Gyro.fifo_data = 0;
}


void CC_LSM6DSX_Fifo_Accel_Read_Done(uint8_t handle)
{
    S_Lsm6dsx_Fifo_Handle[handle].Accel.nSamples = 0;    
}

void CC_LSM6DSX_Fifo_Gyro_Read_Done(uint8_t handle)
{
    S_Lsm6dsx_Fifo_Handle[handle].Gyro.nSamples = 0;
}

uint32_t  CC_LSM6DSX_Fifo_Get_Accel_UnRead_Samples(uint8_t handle)
{
    return S_Lsm6dsx_Fifo_Handle[handle].Accel.nSamples;
}

uint32_t  CC_LSM6DSX_Fifo_Get_Gyro_UnRead_Samples(uint8_t handle)
{
    return S_Lsm6dsx_Fifo_Handle[handle].Gyro.nSamples;
}

void CC_LSM6DSX_Fifo_Update_Data(void)
{
    uint16_t fifo_len = 0;
    uint16_t data_index;
    int16_t mems_data[3];
    uint32_t fifo_nb_consumed = 0;

    LSM6DS3_ACC_GYRO_R_FIFONumOfEntries(0, &fifo_len);


    if(0x00 == fifo_len)
        return;


    fifo_len /= 3; // make x,y,z => fifo_len = 1


    while(fifo_nb_consumed < fifo_len)
    {

        // Need to process Gyro data first
        // Process Gyro data:    
        if(E_LSM6DSX_FIFO_CONTROL_GYRO == Lsm6dsx_Fifo_Config.ControlTarget
            || (E_LSM6DSX_FIFO_CONTROL_ACCEL_GYRO == Lsm6dsx_Fifo_Config.ControlTarget))
        {

            fifo_nb_consumed++;
            
            LSM6DS3_ACC_Get_Acceleration(mems_data, FIFO_EN); // 1 sample => x,y,z            

            //TracerInfo("gyro_x: 0x%02x\r\n", mems_data[0]);
            //TracerInfo("gyro_y: 0x%02x\r\n", mems_data[1]);
            //TracerInfo("gyro_z: 0x%02x\r\n", mems_data[2]);       


            for(uint8_t handle = 0; handle < E_SEN_USER_ID_TOTAL; handle++)
            {
                if(S_Lsm6dsx_Fifo_Handle[handle].Gyro.isRegistered)
                {
                    uint32_t sample_target = (Lsm6dsx_Fifo_Config.OdrHzVal / S_Lsm6dsx_Fifo_Handle[handle].Gyro.PeriodInHz);
                        
                    if(0x00 == (Lsm6dsx_Fifo_Config.SampleCount % sample_target))
                    {                
                        data_index = (S_Lsm6dsx_Fifo_Handle[handle].Gyro.nSamples * 3); // x,y,z

                        if(data_index < S_Lsm6dsx_Fifo_Handle[handle].Gyro.nFifoDepth)
                        {
                            memcpy(&S_Lsm6dsx_Fifo_Handle[handle].Gyro.fifo_data[data_index], mems_data, 0x06);

                            S_Lsm6dsx_Fifo_Handle[handle].Gyro.nSamples++;

                        }
                    }
                }
            }

        }


        
        // Process Accel data:
        if((E_LSM6DSX_FIFO_CONTROL_ACCEL == Lsm6dsx_Fifo_Config.ControlTarget) 
            || (E_LSM6DSX_FIFO_CONTROL_ACCEL_GYRO == Lsm6dsx_Fifo_Config.ControlTarget))
        {

            fifo_nb_consumed++;
            
            LSM6DS3_ACC_Get_Acceleration(mems_data, FIFO_EN); // 1 sample => x,y,z
        

            //TracerInfo("acc_x: 0x%02x\r\n", mems_data[0]);
            //TracerInfo("acc_y: 0x%02x\r\n", mems_data[1]);
            //TracerInfo("acc_z: 0x%02x\r\n", mems_data[2]);                

            
            for(uint8_t handle = 0; handle < E_SEN_USER_ID_TOTAL; handle++)
            {
                if(S_Lsm6dsx_Fifo_Handle[handle].Accel.isRegistered)
                {
         
                    uint32_t sample_target = (Lsm6dsx_Fifo_Config.OdrHzVal / S_Lsm6dsx_Fifo_Handle[handle].Accel.PeriodInHz);
                
                    if(0x00 == (Lsm6dsx_Fifo_Config.SampleCount % sample_target))
                    {                
                        data_index = (S_Lsm6dsx_Fifo_Handle[handle].Accel.nSamples * 3); // x,y,z

                        if(data_index < S_Lsm6dsx_Fifo_Handle[handle].Accel.nFifoDepth)
                        {
                            memcpy(&S_Lsm6dsx_Fifo_Handle[handle].Accel.fifo_data[data_index], mems_data, 0x06);

                            S_Lsm6dsx_Fifo_Handle[handle].Accel.nSamples++;

                        }
                    }
                    
                }
            }

        }


        Lsm6dsx_Fifo_Config.SampleCount++;

    }

}




////////////////////   Multi-configure judgement ///////////////////////////

static E_Accel_Gyro_Access_Mode JudgeAccelGyroAccessMode(void)
{
    uint8_t i;
    E_Accel_Gyro_Access_Mode curr_mode;
    E_Accel_Gyro_Access_Mode final_select_mode = E_ACC_GYRO_MODE_NOT_CONFIG;


    for(i = 0; i < E_SEN_USER_ID_TOTAL; i++)
    {
        curr_mode = g_Accel_Gyro_Settings[i].AccessMode;

        if(E_ACC_GYRO_MODE_NOT_CONFIG == curr_mode)
        {
            continue;
        }
        else
        {
            // check last mode
            if(E_ACC_GYRO_MODE_NOT_CONFIG == final_select_mode)
            {
                final_select_mode = curr_mode;
            
                continue;
            }
            else
            {
                if(final_select_mode != curr_mode)
                {
                    return E_ACC_GYRO_MODE_NOT_CONFIG;
                }
            }

        }

    }


    return final_select_mode;
    
}


static E_LSM6DSX_FIFO_TARGET_DEVICE JudgeAccelGyroFifoControlTarget(void) 
{
    uint8_t i;
    E_LSM6DSX_FIFO_TARGET_DEVICE curr_target;
    E_LSM6DSX_FIFO_TARGET_DEVICE final_select_target = E_LSM6DSX_FIFO_CONTROL_NONE; 

    for(i = 0; i < E_SEN_USER_ID_TOTAL; i++)
    {

        if(E_ACC_GYRO_MODE_FIFO != g_Accel_Gyro_Settings[i].AccessMode)
            continue;
    
        curr_target = g_Accel_Gyro_Settings[i].FifoTarget;

        // check current mode
        if(E_LSM6DSX_FIFO_CONTROL_NONE == curr_target)
        {
            continue;
        }
        else if(E_LSM6DSX_FIFO_CONTROL_ACCEL_GYRO == curr_target)
        {
            return E_LSM6DSX_FIFO_CONTROL_ACCEL_GYRO;
        }
        else
        {
            // check last mode
            if(E_LSM6DSX_FIFO_CONTROL_NONE == final_select_target)
            {
                final_select_target = curr_target;
            
                continue;
            }
            else
            {
                if(final_select_target != curr_target)
                {
                    return E_LSM6DSX_FIFO_CONTROL_ACCEL_GYRO;
                }
            }
        }
      
    }

    return final_select_target;
}


static LSM6DS3_ACC_GYRO_ODR_FIFO_t JudgeAccelGyroFifo_ODR(void) // Find the larger ODR
{
    uint8_t i;
    LSM6DS3_ACC_GYRO_ODR_FIFO_t curr_odr;
    LSM6DS3_ACC_GYRO_ODR_FIFO_t final_select_odr = LSM6DS3_ACC_GYRO_ODR_FIFO_10Hz;
        

    for(i = 0; i < E_SEN_USER_ID_TOTAL; i++)
    {
        
         if(E_ACC_GYRO_MODE_FIFO != g_Accel_Gyro_Settings[i].AccessMode)
             continue;

         if(E_LSM6DSX_FIFO_CONTROL_NONE == g_Accel_Gyro_Settings[i].FifoTarget)
             continue;


         // check current ODR:
         if(E_LSM6DSX_FIFO_CONTROL_ACCEL == g_Accel_Gyro_Settings[i].FifoTarget)
         {
            curr_odr = g_Accel_Gyro_Settings[i].S_Accel.FifoSet.Odr;
         }
         else if(E_LSM6DSX_FIFO_CONTROL_GYRO == g_Accel_Gyro_Settings[i].FifoTarget)
         {
             curr_odr = g_Accel_Gyro_Settings[i].S_Gyro.FifoSet.Odr;
         }
         else if(E_LSM6DSX_FIFO_CONTROL_ACCEL_GYRO == g_Accel_Gyro_Settings[i].FifoTarget)
         {
             // find the larger
             if(g_Accel_Gyro_Settings[i].S_Accel.FifoSet.Odr >= g_Accel_Gyro_Settings[i].S_Gyro.FifoSet.Odr)
                 curr_odr = g_Accel_Gyro_Settings[i].S_Accel.FifoSet.Odr;
             else
                 curr_odr = g_Accel_Gyro_Settings[i].S_Gyro.FifoSet.Odr;
         }

         
        if(curr_odr > final_select_odr)
            final_select_odr = curr_odr;       

    }


    return final_select_odr;
}




void Acc_Gyro_Dump_All_Settings(void)
{



    TracerInfo("Acc_Gyro_Dump_All_Settings\r\n");


    TracerInfo("isConfigured: %d\r\n", Lsm6dsx_Fifo_Config.isConfigured);
    TracerInfo("ControlTarget: %d\r\n", Lsm6dsx_Fifo_Config.ControlTarget);
    TracerInfo("OdrLevel: %d\r\n", Lsm6dsx_Fifo_Config.OdrLevel);
    TracerInfo("OdrHzVal: %d\r\n", Lsm6dsx_Fifo_Config.OdrHzVal);
    TracerInfo("SampleCount: %d\r\n", Lsm6dsx_Fifo_Config.SampleCount);


    for(uint8_t i = 0; i < E_SEN_USER_ID_TOTAL; i++)
    {
        TracerInfo("====== User ID: %d  =======\r\n", i);

        TracerInfo("AccessMode: %d\r\n", g_Accel_Gyro_Settings[i].AccessMode);
        TracerInfo("FifoTarget: %d\r\n", g_Accel_Gyro_Settings[i].FifoTarget);
        TracerInfo("PollTarget: %d\r\n", g_Accel_Gyro_Settings[i].PollTarget);

        TracerInfo("Accel\r\n");
        TracerInfo("Fifo settings:\r\n");        
        TracerInfo("nFifoDepth: %d Bytes\r\n", g_Accel_Gyro_Settings[i].S_Accel.FifoSet.nFifoDepth);
        TracerInfo("Odr: %d Hz\r\n", LSM6DSX_ACC_GYRO_translate_ODR_Fifo(g_Accel_Gyro_Settings[i].S_Accel.FifoSet.Odr));        
        TracerInfo("*iSampleData: 0x%x\r\n", (uint32_t)g_Accel_Gyro_Settings[i].S_Accel.FifoSet.iSampleData);
        TracerInfo("Polling settings:\r\n");        
        TracerInfo("Odr: 0x%02x Hz\r\n", g_Accel_Gyro_Settings[i].S_Accel.PollSet.Odr);        
        TracerInfo("*iSampleData: 0x%x\r\n", (uint32_t)g_Accel_Gyro_Settings[i].S_Accel.PollSet.iSampleData);
        

        TracerInfo("Gyro\r\n");
        TracerInfo("Fifo settings:\r\n");        
        TracerInfo("nFifoDepth: %d Bytes\r\n", g_Accel_Gyro_Settings[i].S_Gyro.FifoSet.nFifoDepth);
        TracerInfo("Odr: %d Hz\r\n", LSM6DSX_ACC_GYRO_translate_ODR_Fifo(g_Accel_Gyro_Settings[i].S_Gyro.FifoSet.Odr));        
        TracerInfo("*iSampleData: 0x%x\r\n", (uint32_t)g_Accel_Gyro_Settings[i].S_Gyro.FifoSet.iSampleData);
        TracerInfo("Polling settings:\r\n");        
        TracerInfo("Odr: 0x%02x Hz\r\n", g_Accel_Gyro_Settings[i].S_Gyro.PollSet.Odr);        
        TracerInfo("*iSampleData: 0x%x\r\n", (uint32_t)g_Accel_Gyro_Settings[i].S_Gyro.PollSet.iSampleData);       
    }

}



//////////////// Interface definition /////////////////////

E_Accel_Gyro_Manager_Status Acc_Gyro_Controller_Init(void)
{
    E_Accel_Gyro_Manager_Status ret = E_ACC_GYRO_SUCCESS;

    if(MEMS_ERROR == LSM6DS3_X_Init()) 
        return E_ACC_GYRO_ERROR_INIT_FAIL;

    return ret;
}


E_Accel_Gyro_Manager_Status Acc_Gyro_Controller_Configure(E_Sensor_User_ID UserID, void *Params)
{
    E_Accel_Gyro_Manager_Status ret = E_ACC_GYRO_SUCCESS;
    T_Accel_Gyro_Settings *myParams = (T_Accel_Gyro_Settings*)Params;
    E_Accel_Gyro_Access_Mode access_mode;    

    
    //if(true == CC_LSM6DSX_Fifo_Is_Configured())
      //  return E_SM_ERROR_REPEAT_CONFIG;

    memcpy(&g_Accel_Gyro_Settings[UserID], myParams, sizeof(T_Accel_Gyro_Settings));            
    
    access_mode = JudgeAccelGyroAccessMode();

    if(E_ACC_GYRO_MODE_FIFO == access_mode)
    {            
          E_LSM6DSX_FIFO_TARGET_DEVICE fifo_target;
                
          if(E_LSM6DSX_FIFO_CONTROL_NONE != (fifo_target = JudgeAccelGyroFifoControlTarget()))
          {
              LSM6DS3_ACC_GYRO_ODR_FIFO_t fifo_odr = JudgeAccelGyroFifo_ODR();
                                   
              CC_LSM6DSX_Fifo_Configure(fifo_target, fifo_odr);
          }
          else
              return E_ACC_GYRO_ERROR_INVALID_CONTROL_TARGET;

        E_LSM6DSX_FIFO_TARGET_DEVICE control_target; 
    
        control_target = g_Accel_Gyro_Settings[UserID].FifoTarget;//CC_LSM6DSX_Fifo_Get_Control_Target();
    
        if((E_LSM6DSX_FIFO_CONTROL_ACCEL == control_target)
           || (E_LSM6DSX_FIFO_CONTROL_ACCEL_GYRO == control_target))
        {
            CC_LSM6DSX_Fifo_Accel_Register(UserID, &g_Accel_Gyro_Settings[UserID].S_Accel.FifoSet);
        }
            
        if((E_LSM6DSX_FIFO_CONTROL_GYRO == control_target)
           || (E_LSM6DSX_FIFO_CONTROL_ACCEL_GYRO == control_target))
        {
            CC_LSM6DSX_Fifo_Gyro_Register(UserID, &g_Accel_Gyro_Settings[UserID].S_Gyro.FifoSet);
        }          

    }
    else if(E_ACC_GYRO_MODE_POLL == access_mode)
    {
          // do nothing
    }
    else
    {
          return E_ACC_GYRO_ERROR_INVALID_ACCESS_MODE;
    }

    return ret;
}

E_Accel_Gyro_Manager_Status Acc_Gyro_Controller_Start(E_Sensor_User_ID UserID)
{
    E_Accel_Gyro_Manager_Status ret = E_ACC_GYRO_SUCCESS;
    
    if(CC_LSM6DSX_Fifo_Is_Configured())
    {
        CC_LSM6DSX_FifoDisable(E_LSM6DSX_FIFO_CONTROL_ACCEL_GYRO); // disable all
        
        CC_LSM6DSX_FifoEnable();
                        
    }
    else // using polling mode:
    {
        
        if((LSM6DS3_POLL_SINGLE_MODE_ACCEL == g_Accel_Gyro_Settings[UserID].PollTarget))
                //&& (false == CC_LSM6DSX_Check_Accel_ON()))
        {
           
            CC_LSM6DSX_AccelPowerON((LSM6DS3_ACC_GYRO_ODR_XL_t)g_Accel_Gyro_Settings[UserID].S_Accel.PollSet.Odr);     
        }
        else if((LSM6DS3_POLL_SINGLE_MODE_GYRO == g_Accel_Gyro_Settings[UserID].PollTarget))
                //&& (false == CC_LSM6DSX_Check_Gyro_ON()))
        {
            CC_LSM6DSX_GyroPowerON ((LSM6DS3_ACC_GYRO_ODR_G_t)g_Accel_Gyro_Settings[UserID].S_Gyro.PollSet.Odr);
        }
        else if((LSM6DS3_POLL_DUAL_MODE == g_Accel_Gyro_Settings[UserID].PollTarget))
                //&& (false == CC_LSM6DSX_Check_Accel_ON())
                //&& (false == CC_LSM6DSX_Check_Gyro_ON()))
        {
               
            CC_LSM6DSX_AccelPowerON((LSM6DS3_ACC_GYRO_ODR_XL_t)g_Accel_Gyro_Settings[UserID].S_Accel.PollSet.Odr);
            CC_LSM6DSX_GyroPowerON ((LSM6DS3_ACC_GYRO_ODR_G_t)g_Accel_Gyro_Settings[UserID].S_Gyro.PollSet.Odr);
        }
        else
            return E_ACC_GYRO_ERROR_INVALID_CONTROL_TARGET; 
    }

    return ret;
}

E_Accel_Gyro_Manager_Status Acc_Gyro_Controller_GetData(E_Sensor_User_ID UserID, void* pSampleData, void *pDataSzInBytes)
{
    E_Accel_Gyro_Manager_Status ret = E_ACC_GYRO_SUCCESS;
    E_Accel_Gyro_Access_Mode access_mode;
    uint16_t accel_nSamples = 0;    
    uint16_t gyro_nSamples = 0; 

    access_mode = JudgeAccelGyroAccessMode();
                

    if(E_ACC_GYRO_MODE_FIFO == access_mode)
    {            
        E_LSM6DSX_FIFO_TARGET_DEVICE fifo_target;
        T_Accel_Gyro_Fifo_Data_Bytes *fifo_SzInBytes = (T_Accel_Gyro_Fifo_Data_Bytes *) pDataSzInBytes;
        T_Accel_Gyro_Fifo_Data *fifo_data = (T_Accel_Gyro_Fifo_Data*) pSampleData;

        fifo_SzInBytes->accel_SzInBytes = 0;
        fifo_SzInBytes->gyro_SzInBytes = 0;

        
//        if(NULL != pSampleData)
//            fifo_data = (T_Accel_Gyro_Fifo_Data*) pSampleData;

       
        CC_LSM6DSX_Fifo_Update_Data();

        fifo_target = g_Accel_Gyro_Settings[UserID].FifoTarget;//CC_LSM6DSX_Fifo_Get_Control_Target();

        
        if( (E_LSM6DSX_FIFO_CONTROL_ACCEL == fifo_target)
            || (E_LSM6DSX_FIFO_CONTROL_ACCEL_GYRO == fifo_target))
        {

            if(NULL != fifo_data->p_accel_data)
            {
                accel_nSamples = CC_LSM6DSX_Fifo_Get_Accel_UnRead_Samples(UserID);     

                fifo_SzInBytes->accel_SzInBytes = (accel_nSamples * 3 * 2); //16bits x,y,z

                fifo_data->accel_nSamples = accel_nSamples;

                memcpy(fifo_data->p_accel_data, g_Accel_Gyro_Settings[UserID].S_Accel.FifoSet.iSampleData, fifo_SzInBytes->accel_SzInBytes);                  

                CC_LSM6DSX_Fifo_Accel_Read_Done(UserID);

                //TracerInfo("Get acc sample(%d): %d\r\n", UserID, accel_nSamples);
                /*
                TracerInfo("Get acc sample[0]: %d\r\n", fifo_data->p_accel_data[0]);
                TracerInfo("Get acc sample[1]: %d\r\n", fifo_data->p_accel_data[1]);
                TracerInfo("Get acc sample[2]: %d\r\n", fifo_data->p_accel_data[2]); 
                TracerInfo("Get acc sample[3]: %d\r\n", fifo_data->p_accel_data[3]);
                TracerInfo("Get acc sample[4]: %d\r\n", fifo_data->p_accel_data[4]);
                TracerInfo("Get acc sample[5]: %d\r\n", fifo_data->p_accel_data[5]);                 
                */
            }
        }
            
        if((E_LSM6DSX_FIFO_CONTROL_GYRO == fifo_target)
            || (E_LSM6DSX_FIFO_CONTROL_ACCEL_GYRO == fifo_target))
        {

            if(NULL != fifo_data->p_gyro_data)
            {
                //TracerInfo("pGyro_addr3: 0x%x\r\n", fifo_data->p_gyro_data);

            
                gyro_nSamples = CC_LSM6DSX_Fifo_Get_Gyro_UnRead_Samples(UserID);    

                fifo_SzInBytes->gyro_SzInBytes = (gyro_nSamples * 3 * 2); //16bits x,y,z

                fifo_data->gyro_nSamples = gyro_nSamples;
                
                memcpy(fifo_data->p_gyro_data, g_Accel_Gyro_Settings[UserID].S_Gyro.FifoSet.iSampleData, fifo_SzInBytes->gyro_SzInBytes);

                CC_LSM6DSX_Fifo_Gyro_Read_Done(UserID);

                //TracerInfo("Get gyro sample(%d): %d\r\n", UserID, gyro_nSamples);
                //TracerInfo("Get gyro sample[0]: %d\r\n", g_Accel_Gyro_Settings[UserID].S_Gyro.FifoSet.iSampleData[0]);
                //TracerInfo("Get gyro sample[1]: %d\r\n", g_Accel_Gyro_Settings[UserID].S_Gyro.FifoSet.iSampleData[1]);
                //TracerInfo("Get gyro sample[2]: %d\r\n", g_Accel_Gyro_Settings[UserID].S_Gyro.FifoSet.iSampleData[2]);                

            }           

        }
                        
    }           
    else if(E_ACC_GYRO_MODE_POLL == access_mode)
    {

        //uint16_t dataLen = sizeof(T_Accel_Gyro_Hybrid_Data);

        
        LSM6DS3_ACC_GYRO_GetRawAccData(NULL, (i16_t*) ((T_Accel_Gyro_Hybrid_Data *) pSampleData)->accel_data);
        LSM6DS3_ACC_GYRO_GetRawGyroData16(NULL, (i16_t*) ((T_Accel_Gyro_Hybrid_Data *) pSampleData)->gyro_data);

    }

    return ret;
}




E_Accel_Gyro_Manager_Status Acc_Gyro_Controller_Shutdown(E_Sensor_User_ID UserID, bool power_off)
{
    E_Accel_Gyro_Manager_Status ret = E_ACC_GYRO_SUCCESS;
    
    //CC_LSM6DSX_Fifo_Reset();  

    if(E_SEN_USER_ID_TOTAL == UserID)
    { 
        for(uint8_t i = 0; i < E_SEN_USER_ID_TOTAL; i++)
        {
            CC_LSM6DSX_Fifo_Accel_UnRegister(i);
            CC_LSM6DSX_Fifo_Gyro_UnRegister(i);
        }
    }
    else
    {
        CC_LSM6DSX_Fifo_Accel_UnRegister(UserID);
        CC_LSM6DSX_Fifo_Gyro_UnRegister(UserID);
    }

    if(true == power_off)    
    {
        CC_LSM6DSX_FifoDisable(E_LSM6DSX_FIFO_CONTROL_ACCEL_GYRO);        
        
        CC_LSM6DSX_AccelPowerDown();
        CC_LSM6DSX_GyroPowerDown();    
    }

    return ret;
}


//////////////   interface of sensor manager  /////////////////

const struct sensor_manager_itfs acc_gyro_itf =
{
    Acc_Gyro_Controller_Init,
    Acc_Gyro_Controller_Configure,
    Acc_Gyro_Controller_Start,
    Acc_Gyro_Controller_GetData,
    Acc_Gyro_Controller_Shutdown
};


const struct sensor_manager_itfs* acc_gyro_sm_itf_get(void)
{
    return &acc_gyro_itf;
}

#endif
