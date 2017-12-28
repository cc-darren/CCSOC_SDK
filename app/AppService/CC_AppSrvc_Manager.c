#ifdef APP_SERV_MGR_EN

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "project.h"
#include "CC_Sensor_Manager.h"
#include "CC_AppSrvc_Manager.h"
#include "Acc_Gyro_Controller.h" 
#include "Mag_Controller.h"
#include "CC_HRM_drv.h"
#include "error.h"
#include "tracer.h"
#include "clock.h"

#define IS_SRV_ACTIVED(type)  ((E_APP_SRV_ST_ACTIVE == b_app_srv_state[type]) ? true : false)
#define IS_SRV_SUSPENDED(type)  ((E_APP_SRV_ST_SUSPEND == b_app_srv_state[type]) ? true : false)

static E_App_Srv_State b_app_srv_state[E_APP_SRV_ID_TOTAL];







static E_App_Srv_Err_Code AppSrv_Suspend(E_App_Srv_ID type)
{

    E_App_Srv_Err_Code ret_code = E_APP_SRV_ERR_NONE;

    b_app_srv_state[type] = E_APP_SRV_ST_SUSPEND;

    switch(type)
    {
        case E_APP_SRV_ID_HRM:
            CC_SenMgr_Stop_HRM();
            break;
        case E_APP_SRV_ID_PEDO:            
            CC_SenMgr_Stop_Pedometer();
            break;
        case E_APP_SRV_ID_SWIM:            
            CC_SenMgr_Stop_Swim();
            break;
        default:
            ret_code = E_APP_SRV_ERR_TYPE;
            break;
    }
   

    return ret_code; 
}

//static E_App_Srv_Err_Code AppSrv_Resume(E_App_Srv_ID type)
static E_App_Srv_Err_Code AppSrv_Resume(void)
{

    E_App_Srv_Err_Code ret_code = E_APP_SRV_ERR_NONE;

#ifdef SRV_MGR_TEST_CASE_4
    if(IS_SRV_SUSPENDED(E_APP_SRV_ID_SWIM))
    {
        TracerInfo("CC_AppSrv_Sensor_Resume: %d\r\n", (E_APP_SRV_ID_SWIM));
        if(E_SEN_ERROR_NONE == CC_SenMgr_Start_Swim())            
            b_app_srv_state[E_APP_SRV_ID_HRM] = E_APP_SRV_ST_ACTIVE;        
        else
            ret_code = E_APP_SRV_ERR_RESUME_FAIL;
    }      

#endif

#if 0
    if(IS_SRV_SUSPENDED(type))
    {
        TracerInfo("CC_AppSrv_Sensor_Resume: %d!\r\n", type);
        CC_SenMgr_Start_Swim();

        b_app_srv_state[type] = E_APP_SRV_ST_ACTIVE;        
    }      
    else
    {
        TracerInfo("E_APP_SRV_ERR_CANT_RESUME_FROM_ACTIVE: %d!\r\n", type);
        
        ret_code = E_APP_SRV_ERR_CANT_RESUME_FROM_ACTIVE;

    }
#endif

#if 0
    if(IS_SRV_SUSPENDED(E_APP_SRV_ID_HRM))
    {
        // resume hrm
        TracerInfo("CC_AppSrv_Sensor_Resume: %d!\r\n", E_APP_SRV_ID_HRM);
        CC_SenMgr_Start_HRM();

        b_app_srv_state[E_APP_SRV_ID_HRM] = E_APP_SRV_ST_ACTIVE;        
    }
    else
        ret_code = E_APP_SRV_ERR_CANT_RESUME_FROM_ACTIVE;

    if(IS_SRV_SUSPENDED(E_APP_SRV_ID_PEDO))
    {
        // resume pedo
        TracerInfo("CC_AppSrv_Sensor_Resume: %d!\r\n", E_APP_SRV_ID_PEDO);
        CC_SenMgr_Start_Pedometer();

        b_app_srv_state[E_APP_SRV_ID_PEDO] = E_APP_SRV_ST_ACTIVE;        
    }
    else
        ret_code = E_APP_SRV_ERR_CANT_RESUME_FROM_ACTIVE;        

    if(IS_SRV_SUSPENDED(E_APP_SRV_ID_SWIM))
    {
        // resume swim
        TracerInfo("CC_AppSrv_Sensor_Resume: %d!\r\n", E_APP_SRV_ID_SWIM);
        CC_SenMgr_Start_Swim();

        b_app_srv_state[E_APP_SRV_ID_SWIM] = E_APP_SRV_ST_ACTIVE;        
    }    
    else
        ret_code = E_APP_SRV_ERR_CANT_RESUME_FROM_ACTIVE;        
#endif
    return ret_code; 
}








E_App_Srv_Err_Code CC_AppSrv_Manager_Init(void)
{
    E_App_Srv_Err_Code ret_code = E_APP_SRV_ERR_NONE;
    
    for(uint8_t i = 0; i < E_APP_SRV_ID_TOTAL; i++)
    {
        b_app_srv_state[i] = E_APP_SRV_ST_IDLE;
    
}

    return ret_code;
}


E_App_Srv_Err_Code CC_AppSrv_Manager_Start(E_App_Srv_ID type)
{
    E_App_Srv_Err_Code ret_code = E_APP_SRV_ERR_NONE;
    
    switch(type)
    {
        case E_APP_SRV_ID_HRM:

            if(IS_SRV_ACTIVED(E_APP_SRV_ID_SWIM))
            {
                //AppSrv_Suspend(E_APP_SRV_ID_HRM);

                return E_APP_SRV_ERR_CONFLICT;
            }

            if(E_SEN_ERROR_NONE != CC_SenMgr_Start_HRM())
                return E_APP_SRV_ERR_START_FAIL;

            break;
        case E_APP_SRV_ID_PEDO:            
            
#if 0 // not used resume here           
            if(IS_SRV_ACTIVED(E_APP_SRV_ID_SWIM))
                AppSrv_Suspend(E_APP_SRV_ID_SWIM);
#endif
            if(IS_SRV_ACTIVED(E_APP_SRV_ID_SWIM))
            {
                //AppSrv_Suspend(E_APP_SRV_ID_PEDO);

                return E_APP_SRV_ERR_CONFLICT;
            }

            if(E_SEN_ERROR_NONE != CC_SenMgr_Start_Pedometer())
                return E_APP_SRV_ERR_START_FAIL;
            
            break;
        case E_APP_SRV_ID_SWIM:  // the highest priority!          
#if 0
            if(IS_SRV_ACTIVED(E_APP_SRV_ID_HRM))
                AppSrv_Suspend(E_APP_SRV_ID_HRM);

            if(IS_SRV_ACTIVED(E_APP_SRV_ID_PEDO))
                AppSrv_Suspend(E_APP_SRV_ID_PEDO);
#endif

#if 1
            if(IS_SRV_ACTIVED(E_APP_SRV_ID_HRM)
                || IS_SRV_ACTIVED(E_APP_SRV_ID_PEDO))
                return E_APP_SRV_ERR_CONFLICT;
#endif                

            if(E_SEN_ERROR_NONE != CC_SenMgr_Start_Swim())
                return E_APP_SRV_ERR_START_FAIL;
            
            break;
        default:
            ret_code = E_APP_SRV_ERR_TYPE;
            break;
    }


    b_app_srv_state[type] = E_APP_SRV_ST_ACTIVE;


    return ret_code;
}


E_App_Srv_Err_Code CC_AppSrv_Manager_Stop(E_App_Srv_ID type)
{

    E_App_Srv_Err_Code ret_code = E_APP_SRV_ERR_NONE;

    //TracerInfo("CC_AppSrv_Manager_Stop: %d!\r\n", type);


    switch(type)
    {
        case E_APP_SRV_ID_HRM:
            
            if(E_SEN_ERROR_NONE != CC_SenMgr_Stop_HRM())
               return E_APP_SRV_ERR_STOP_FAIL; 
            
            break;
        case E_APP_SRV_ID_PEDO:            

            if(E_SEN_ERROR_NONE != CC_SenMgr_Stop_Pedometer())
               return E_APP_SRV_ERR_STOP_FAIL; 
            
            break;
        case E_APP_SRV_ID_SWIM:            

            if(E_SEN_ERROR_NONE != CC_SenMgr_Stop_Swim())
               return E_APP_SRV_ERR_STOP_FAIL; 
            
            break;
        default:
            
            ret_code = E_APP_SRV_ERR_TYPE;
            
            break;
    }



    b_app_srv_state[type] = E_APP_SRV_ST_IDLE;

    AppSrv_Resume();

    return ret_code;
}






//////////////  Test Code /////////////////////////

#ifdef SRV_MGR_TEST_EN



void CC_AppSrv_Sensor_Test(void)
{
    int16_t hrm_acc_data[MEMS_FIFO_SIZE];
    uint32_t hrm_acc_size = 0;
    //uint8_t hrm_ppg_data[HEART_RATE_MODE_SAMPLES_PER_READ * 4];
    uint32_t hrm_ppg_data[HEART_RATE_MODE_SAMPLES_PER_READ];
    uint32_t hrm_ppg_size = 0;   

    int16_t pedo_acc_data[MEMS_FIFO_SIZE];
    uint32_t pedo_acc_size = 0;
    
    int16_t swim_acc_data[MEMS_FIFO_SIZE];
    int16_t swim_gyro_data[MEMS_FIFO_SIZE];    
    uint32_t swim_acc_size = 0;
    uint32_t swim_gyro_size = 0;

    AxesRaw_t mag_data;
    uint32_t mag_size = 0;  

    uint32_t get_data_size = 0;
    uint32_t get_data2_size = 0;    

    TracerInfo("[SRV_TC-1]: CC_AppSrv_Manager_Init()\r\n");

    CC_AppSrv_Manager_Init();


#ifdef SRV_MGR_TEST_CASE_1

    #define TEST_CASE_ROUND_NB  10


    for(uint8_t round = 0; round < TEST_CASE_ROUND_NB; round++)
    {

        TracerInfo("[SRV_TC-1]: Test Round: %d\r\n", round);
        

        if(E_APP_SRV_ERR_NONE != CC_AppSrv_Manager_Start(E_APP_SRV_ID_PEDO))
        {
            TracerInfo("[SRV_TC-1]: CC_AppSrv_Manager_Start fail!\r\n");
            
            while(1)
                ;
        }

        get_data_size = 0;

        while(get_data_size < MEMS_FIFO_SIZE) // get data until fifo full!
        {

            if(E_APP_SRV_ERR_NONE == CC_SenMgr_Acc_GetData(E_APP_SRV_ID_PEDO, pedo_acc_data, &pedo_acc_size))
            {
                if(0x00 != pedo_acc_size)
                {
                    TracerInfo("[SRV_TC-1]: pedo_accel_size: %d\r\n", pedo_acc_size);
                
                    for(uint16_t i = 0; i < pedo_acc_size/sizeof(int16_t); i+=3)
                    {                                       
                        TracerInfo("pedo_accel_X: %d\r\n", pedo_acc_data[i]);
                        TracerInfo("pedo_accel_Y: %d\r\n", pedo_acc_data[i+1]);
                        TracerInfo("pedo_accel_Z: %d\r\n", pedo_acc_data[i+2]);        
                    }
                        
                    TracerInfo("\r\n");            

                    
                                        
                }            

                get_data_size += pedo_acc_size;            
           }

        }


        TracerInfo("[SRV_TC-1]: Pedo get data done.\r\n");

        TracerInfo("[SRV_TC-1]: CC_AppSrv_Manager_Stop()\r\n");

        if(E_APP_SRV_ERR_NONE != CC_AppSrv_Manager_Stop(E_APP_SRV_ID_PEDO))
        {
            TracerInfo("[SRV_TC-1]: CC_AppSrv_Manager_Stop fail!\r\n");
        
            while(1)
                ;
        }

        if(E_APP_SRV_ERR_NONE != CC_SenMgr_Acc_GetData(E_APP_SRV_ID_PEDO, pedo_acc_data, &pedo_acc_size))
        {
            TracerInfo("[SRV_TC-1]: CC_SenMgr_Acc_GetData() don't get any data after stopping.\r\n");
        }
        else
        {
            TracerInfo("[SRV_TC-1]: Acc Data should not get here!! \r\n");
        }

        
        
    }


    TracerInfo("[SRV_TC-1]: SRV Test Case 1 pass!\r\n");

    
    

    while(1)
        ;


#elif defined(SRV_MGR_TEST_CASE_2)

#define TEST_CASE_ROUND_NB  10


    for(uint8_t round = 0; round < TEST_CASE_ROUND_NB; round++)
    {

        TracerInfo("[SRV_TC-2]: Test Round: %d\r\n", round);
        

        if(E_APP_SRV_ERR_NONE != CC_AppSrv_Manager_Start(E_APP_SRV_ID_HRM))
        {
            TracerInfo("[SRV_TC-2]: CC_AppSrv_Manager_Start fail!\r\n");
            
            while(1)
                ;
        }

        get_data_size = 0;
        get_data2_size = 0;

        while((get_data_size < MEMS_FIFO_SIZE)
            || (get_data2_size < MEMS_FIFO_SIZE))// get data until fifo full!
        {

            if(E_APP_SRV_ERR_NONE == CC_SenMgr_Acc_GetData(E_APP_SRV_ID_HRM, hrm_acc_data, &hrm_acc_size))
            {
                if(0x00 != hrm_acc_size)
                {                
                    for(uint16_t i = 0; i < hrm_acc_size/sizeof(int16_t); i+=3)
                    {                                       
                        TracerInfo("hrm_accel_X: %d\r\n", hrm_acc_data[i]);
                        TracerInfo("hrm_accel_Y: %d\r\n", hrm_acc_data[i+1]);
                        TracerInfo("hrm_accel_Z: %d\r\n", hrm_acc_data[i+2]);        
                    }
                        
                    TracerInfo("\r\n");            

                    
                                        
                }            

                get_data_size += hrm_acc_size;            
           }

            
             if(E_APP_SRV_ERR_NONE == CC_SenMgr_PPG_GetData(E_APP_SRV_ID_HRM, hrm_ppg_data, &hrm_ppg_size))
             {
                 if(0x00 != hrm_ppg_size)
                 {
                     TracerInfo("[SRV_TC-2]: hrm_ppg_size: %d\r\n", hrm_ppg_size);
                 
                     for(uint16_t i = 0; i < hrm_ppg_size/sizeof(uint32_t); i+=3)
                     {                                       
                         TracerInfo("hrm_ppg_X: %d\r\n", hrm_ppg_data[i]);
                         TracerInfo("hrm_ppg_Y: %d\r\n", hrm_ppg_data[i+1]);
                         TracerInfo("hrm_ppg_Z: %d\r\n", hrm_ppg_data[i+2]);        

                         
                                                           
                     }
                         
                     TracerInfo("\r\n");                                   
                 }            
            
                 get_data2_size += hrm_ppg_size;            
            }

        }


        TracerInfo("[SRV_TC-2]: Pedo get data done.\r\n");

        TracerInfo("[SRV_TC-2]: CC_AppSrv_Manager_Stop()\r\n");

        if(E_APP_SRV_ERR_NONE != CC_AppSrv_Manager_Stop(E_APP_SRV_ID_HRM))
        {
            TracerInfo("[SRV_TC-2]: CC_AppSrv_Manager_Stop fail!\r\n");
        
            while(1)
                ;
        }

        if(E_APP_SRV_ERR_NONE != CC_SenMgr_Acc_GetData(E_APP_SRV_ID_HRM, pedo_acc_data, &pedo_acc_size))
        {
            TracerInfo("[SRV_TC-2]: CC_SenMgr_Acc_GetData() don't get any data after stopping.\r\n");
        }
        else
        {
            TracerInfo("[SRV_TC-2]: Acc Data should not get here!! size\r\n");
        }

        if(E_APP_SRV_ERR_NONE != CC_SenMgr_PPG_GetData(E_APP_SRV_ID_HRM, hrm_ppg_data, &hrm_ppg_size))
        {
            TracerInfo("[SRV_TC-2]: CC_SenMgr_PPG_GetData() don't get any data after stopping.\r\n");
        }
        else
        {
            TracerInfo("[SRV_TC-2]: PPG Data should not get here!! \r\n");
        }
        

        
        
    }


    TracerInfo("[SRV_TC-2]: SRV Test Case 2 pass!\r\n");

    
    

    while(1)
        ;



#elif defined(SRV_MGR_TEST_CASE_3)

    #define TEST_CASE_ROUND_NB  10


    for(uint8_t round = 0; round < TEST_CASE_ROUND_NB; round++)
    {

        TracerInfo("[SRV_TC-3]: Test Round: %d\r\n", round);
        

        if(E_APP_SRV_ERR_NONE != CC_AppSrv_Manager_Start(E_APP_SRV_ID_SWIM))
        {
            TracerInfo("[SRV_TC-3]: CC_AppSrv_Manager_Start fail!\r\n");
            
            while(1)
                ;
        }

        get_data_size = 0;
        get_data2_size = 0;

        while((get_data_size < MEMS_FIFO_SIZE) 
            || (get_data2_size < MEMS_FIFO_SIZE))// get data until fifo full!
        {

            if(E_APP_SRV_ERR_NONE == CC_SenMgr_Acc_GetData(E_APP_SRV_ID_SWIM, swim_acc_data, &swim_acc_size))
            {
                if(0x00 != swim_acc_size)
                {
                    TracerInfo("[SRV_TC-3]: swim_acc_size: %d\r\n", swim_acc_size);
                
                    for(uint16_t i = 0; i < swim_acc_size/sizeof(int16_t); i+=3)
                    {                                       
                        TracerInfo("swim_accel_X: %d\r\n", swim_acc_data[i]);
                        TracerInfo("swim_accel_Y: %d\r\n", swim_acc_data[i+1]);
                        TracerInfo("swim_accel_Z: %d\r\n", swim_acc_data[i+2]);        
                    }
                        
                    TracerInfo("\r\n");            

                    
                                        
                }            

                get_data_size += swim_acc_size;            
           }


           if(E_APP_SRV_ERR_NONE == CC_SenMgr_Gyro_GetData(E_APP_SRV_ID_SWIM, swim_gyro_data, &swim_gyro_size))
           {
               if(0x00 != swim_gyro_size)
               {
                   TracerInfo("[SRV_TC-3]: swim_gyro_size: %d\r\n", swim_gyro_size);
                 
                   for(uint16_t i = 0; i < swim_gyro_size/sizeof(int16_t); i+=3)
                   {                                       
                       TracerInfo("swim_gyro_X: %d\r\n", swim_gyro_data[i]);
                       TracerInfo("swim_gyro_Y: %d\r\n", swim_gyro_data[i+1]);
                       TracerInfo("swim_gyro_Z: %d\r\n", swim_gyro_data[i+2]);        
                   }
                         
                   TracerInfo("\r\n");            
            
                   
                                       
               }            
            
               get_data2_size += swim_gyro_size;            
           }


           if(E_APP_SRV_ERR_NONE == CC_SenMgr_Mag_GetData(E_APP_SRV_ID_SWIM, (int16_t*)&mag_data, &mag_size))
           {
               if(0x00 != mag_size)
               {
                   TracerInfo("[SRV_TC-3]: swim_mag_size: %d\r\n", mag_size);
                 
                   TracerInfo("swim_mag_X: %d\r\n", mag_data.AXIS_X);
                   TracerInfo("swim_mag_Y: %d\r\n", mag_data.AXIS_Y);
                   TracerInfo("swim_mag_Z: %d\r\n", mag_data.AXIS_Z);        
                         
                   TracerInfo("\r\n");            
            
                   
                                       
               }            
            
           }

        }


        TracerInfo("[SRV_TC-3]: Pedo/Gyro get data done.\r\n");

        TracerInfo("[SRV_TC-3]: CC_AppSrv_Manager_Stop()\r\n");

        if(E_APP_SRV_ERR_NONE != CC_AppSrv_Manager_Stop(E_APP_SRV_ID_SWIM))
        {
            TracerInfo("[SRV_TC-3]: CC_AppSrv_Manager_Stop fail!\r\n");
        
            while(1)
                ;
        }

        if(E_APP_SRV_ERR_NONE != CC_SenMgr_Acc_GetData(E_APP_SRV_ID_SWIM, swim_acc_data, &swim_acc_size))
        {
            TracerInfo("[SRV_TC-3]: CC_SenMgr_Acc_GetData() don't get any data after stopping.\r\n");
        }
        else
        {
            TracerInfo("[SRV_TC-3]: Acc Data should not get here!! size\r\n");
        }

        if(E_APP_SRV_ERR_NONE != CC_SenMgr_Gyro_GetData(E_APP_SRV_ID_SWIM, swim_gyro_data, &swim_gyro_size))
        {
            TracerInfo("[SRV_TC-3]: CC_SenMgr_Gyro_GetData() don't get any data after stopping.\r\n");
        }
        else
        {
            TracerInfo("[SRV_TC-3]: Gyro Data should not get here!! size\r\n");
        }

        if(E_APP_SRV_ERR_NONE != CC_SenMgr_Mag_GetData(E_APP_SRV_ID_SWIM, (int16_t*)&mag_data, &mag_size))
        {
            TracerInfo("[SRV_TC-3]: CC_SenMgr_Mag_GetData() don't get any data after stopping.\r\n");
        }
        else
        {
            TracerInfo("[SRV_TC-3]: Mag Data should not get here!! \r\n");
        }

        
        
    }


    TracerInfo("[SRV_TC-3]: SRV Test Case 3 pass!\r\n");

    
    

    while(1)
        ;



#elif defined(SRV_MGR_TEST_CASE_4)


    TracerInfo("[SRV_TC-4]: CC_AppSrv_Manager_Start\r\n");

    if(E_APP_SRV_ERR_NONE != CC_AppSrv_Manager_Start(E_APP_SRV_ID_SWIM))
    {
        TracerInfo("[SRV_TC-3]: CC_AppSrv_Manager_Start fail!\r\n");
        
        while(1)
            ;
    }


    cc6801_ClockDelayMs(100);

    TracerInfo("[SRV_TC-4]: Get Mag:\r\n");
    
    

    if(E_APP_SRV_ERR_NONE == CC_SenMgr_Mag_GetData(E_APP_SRV_ID_SWIM, (int16_t*)&mag_data, &mag_size))
    {
         if(0x00 != mag_size)
         {
              TracerInfo("[SRV_TC-4]: swim_mag_size: %d\r\n", mag_size);
              
              TracerInfo("swim_mag_X: %d\r\n", mag_data.AXIS_X);
              TracerInfo("swim_mag_Y: %d\r\n", mag_data.AXIS_Y);
              TracerInfo("swim_mag_Z: %d\r\n", mag_data.AXIS_Z);                              
              TracerInfo("\r\n");            
         
              
                                  
         }            
         
    }


    TracerInfo("[SRV_TC-4]: Suspend SWIM\r\n");
    AppSrv_Suspend(E_APP_SRV_ID_SWIM);


    TracerInfo("[SRV_TC-4]: Get Mag\r\n");

    if(E_APP_SRV_ERR_NONE != CC_SenMgr_Mag_GetData(E_APP_SRV_ID_SWIM, (int16_t*)&mag_data, &mag_size))
    {
         TracerInfo("[SRV_TC-4]: CC_SenMgr_Mag_GetData() don't get any data after suspend.\r\n");
    }
    else
    {
         TracerInfo("[SRV_TC-4]: Mag Data should not get here!! \r\n");
    }
     

    TracerInfo("[SRV_TC-4]: Resume SWIM\r\n");
    if(E_APP_SRV_ERR_NONE != AppSrv_Resume())
    {
        TracerInfo("[SRV_TC-4]: Resume Fail!\r\n");
        
                

        while(1)
            ;
    }


    cc6801_ClockDelayMs(100);

    TracerInfo("[SRV_TC-4]: Get Mag:\r\n");
    
    
    

    if(E_APP_SRV_ERR_NONE == CC_SenMgr_Mag_GetData(E_APP_SRV_ID_SWIM, (int16_t*)&mag_data, &mag_size))
    {
        if(0x00 != mag_size)
        {
            TracerInfo("[SRV_TC-4]: swim_mag_size: %d\r\n", mag_size);
          
            TracerInfo("swim_mag_X: %d\r\n", mag_data.AXIS_X);
            TracerInfo("swim_mag_Y: %d\r\n", mag_data.AXIS_Y);
            TracerInfo("swim_mag_Z: %d\r\n", mag_data.AXIS_Z);        
                  
            TracerInfo("\r\n");            
     
            
                                
        }            
     
    }
    else
    {
        TracerInfo("[SRV_TC-4]: Can't get SWIM data!!\r\n");
        
                  
    }



    if(E_APP_SRV_ERR_NONE != CC_AppSrv_Manager_Stop(E_APP_SRV_ID_SWIM))
    {
          TracerInfo("[SRV_TC-2]: CC_AppSrv_Manager_Stop fail!\r\n");
      
          while(1)
              ;
    }


    TracerInfo("[SRV_TC-4]: SRV Test Case 4 pass!\r\n");

    
    

    while(1)
        ;      

#else


#define CC_APPSRV_SENSOR_TEST_MODE     1


#if defined (CC_APPSRV_SENSOR_TEST_MODE) && (CC_APPSRV_SENSOR_TEST_MODE == 2U) 
    // TEST1,  for Swim
    if(E_APP_SRV_ERR_NONE != CC_AppSrv_Manager_Start(E_APP_SRV_ID_SWIM))
    {
        TracerInfo("CC_AppSrv_Manager_Start fail!\r\n");
            
        while(1)
           ;
    }
#endif


#if 0 // for PEDO    
    if(E_APP_SRV_ERR_NONE != CC_AppSrv_Manager_Start(E_APP_SRV_ID_PEDO))
    {
        TracerInfo("CC_AppSrv_Manager_Start fail!\r\n");
    
        while(1)
            ;
    }
#endif

#if 1 // for HRM
    if(E_APP_SRV_ERR_NONE != CC_AppSrv_Manager_Start(E_APP_SRV_ID_HRM))
    {
        TracerInfo("CC_AppSrv_Manager_Start fail!\r\n");
        
        while(1)
            ;
    }
#endif

#if defined (CC_APPSRV_SENSOR_TEST_MODE) && (CC_APPSRV_SENSOR_TEST_MODE == 1U)
        // TEST2,  for Swim
        if(E_APP_SRV_ERR_NONE != CC_AppSrv_Manager_Start(E_APP_SRV_ID_SWIM))
        {
            TracerInfo("CC_AppSrv_Manager_Start fail!\r\n");
        
            while(1)
                ;
        }
#endif


//    CC_AppSrv_Manager_Stop(E_APP_SRV_ID_SWIM);  // test!!!!!!!!!!!!!!


    Acc_Gyro_Dump_All_Settings();


    while(1)
    {
        static uint32_t test_mode_change_cnt = 0;
        static uint8_t test_mode = 0;

        TracerInfo("test mode: %d\r\n", test_mode);     

        if(test_mode_change_cnt == 1000) // about 10s
        {     
            test_mode = 1;
            
            CC_AppSrv_Manager_Stop(E_APP_SRV_ID_SWIM); 

            test_mode_change_cnt++;

            // only Swim, hrm/pedo are suspend
        }
        else if(test_mode_change_cnt == 2000) // about 20s
        {

            test_mode = 2;

            CC_AppSrv_Manager_Stop(E_APP_SRV_ID_HRM); 

            test_mode_change_cnt++;

            // Only Pedo
        }        
        else if(test_mode_change_cnt == 3000) // about 30s
        {

            test_mode = 3;

            CC_AppSrv_Manager_Stop(E_APP_SRV_ID_PEDO);

            test_mode_change_cnt++;

            test_mode_change_cnt = 0xffffffff; // stop mode change

            // close all
        }                
        else if(test_mode_change_cnt == 0xffffffff)
        {
            test_mode = 100;
            // do nothing;
        }
        else
            test_mode_change_cnt++;


#if 1 // for HRM    
        if(E_APP_SRV_ERR_NONE == CC_SenMgr_PPG_GetData(E_APP_SRV_ID_HRM, hrm_ppg_data, &hrm_ppg_size))
        {
            //TracerInfo("CC_SenMgr_Acc_GetData fail!\r\n");
        
            //while(1)
            //    ;

            if(0x00 != hrm_ppg_size)
            {
                TracerInfo("hrm_acc_data:\r\n");        

                for(uint16_t i = 0; i < hrm_ppg_size/sizeof(uint32_t); i+=3)
                {
                    TracerInfo("hrm_ppg_ch1: %d\r\n", hrm_ppg_data[i]);        
                    TracerInfo("hrm_ppg_ch2: %d\r\n", hrm_ppg_data[i+1]);
                    TracerInfo("hrm_ppg_ch3: %d\r\n", hrm_ppg_data[i+2]);                    
                }
                

                TracerInfo("\r\n");            
            }            
        }


#endif



        
#if 1 // for PEDO    
        if(E_APP_SRV_ERR_NONE == CC_SenMgr_Acc_GetData(E_APP_SRV_ID_PEDO, pedo_acc_data, &pedo_acc_size))
        {
            //TracerInfo("CC_SenMgr_Acc_GetData fail!\r\n");
        
            //while(1)
            //    ;

            if(0x00 != pedo_acc_size)
            {
                TracerInfo("pedo_acc_data:\r\n");        

                for(uint16_t i = 0; i < pedo_acc_size/sizeof(int16_t); i+=3)
                {
                    if( (pedo_acc_size < 6) 
                        || (0 != (pedo_acc_size % 6)))
                         TracerInfo("Error Size: %d\r\n", pedo_acc_size);
                
                
                    TracerInfo("pedo_accel_X: %d\r\n", pedo_acc_data[i]);
                    TracerInfo("pedo_accel_Y: %d\r\n", pedo_acc_data[i+1]);
                    TracerInfo("pedo_accel_Z: %d\r\n", pedo_acc_data[i+2]);        
                }
                

                TracerInfo("\r\n");            
            }            
        }


#endif
        

#if 1 // for Swim
        // ACCEL:
        if(E_APP_SRV_ERR_NONE == CC_SenMgr_Acc_GetData(E_APP_SRV_ID_SWIM, swim_acc_data, &swim_acc_size))
        {
            //TracerInfo("CC_SenMgr_Acc_GetData fail!\r\n");

            //while(1)
            //    ;

            if(0x00 != swim_acc_size)
            {
                TracerInfo("swim_acc_data:\r\n");        
            
                for(uint16_t i = 0; i < swim_acc_size/sizeof(int16_t); i+=3)
                {
                    if( (swim_acc_size < 6) 
                        || (0 != (swim_acc_size % 6)))
                         TracerInfo("Error Size: %d\r\n", swim_acc_size);
                
                
                    TracerInfo("swim_accel_X: %d\r\n", swim_acc_data[i]);
                    TracerInfo("swim_accel_Y: %d\r\n", swim_acc_data[i+1]);
                    TracerInfo("swim_accel_Z: %d\r\n", swim_acc_data[i+2]);        
                }
                
            
                TracerInfo("\r\n");            
            }
            
        }




        // Gyro:
        if(E_APP_SRV_ERR_NONE == CC_SenMgr_Gyro_GetData(E_APP_SRV_ID_SWIM, swim_gyro_data, &swim_gyro_size))
        {
            //TracerInfo("CC_SenMgr_Acc_GetData fail!\r\n");

            //while(1)
             //   ;

            if(0x00 != swim_gyro_size)
            {
                TracerInfo("swim_gyro_data:\r\n");        

                for(uint16_t i = 0; i < swim_gyro_size/sizeof(int16_t); i+=3)
                {
                    if( (swim_gyro_size < 6) 
                        || (0 != (swim_gyro_size % 6)))
                         TracerInfo("Error Size: %d\r\n", swim_gyro_size);
                
                
                    TracerInfo("swim_gyro_X: %d\r\n", swim_gyro_data[i]);
                    TracerInfo("swim_gyro_Y: %d\r\n", swim_gyro_data[i+1]);
                    TracerInfo("swim_gyro_Z: %d\r\n", swim_gyro_data[i+2]);        
                }
                

                TracerInfo("\r\n");            
            }            
        }




        // Mag:
        if(E_APP_SRV_ERR_NONE == CC_SenMgr_Mag_GetData(E_APP_SRV_ID_SWIM, (int16_t*)&mag_data, &mag_size))
        {
            //TracerInfo("CC_SenMgr_Acc_GetData fail!\r\n");

            //while(1)
              //  ;
              
            if(0x00 != mag_size)
            {
                TracerInfo("swim_mag_data:\r\n");        
               
                TracerInfo("mag_data_X: %d\r\n", mag_data.AXIS_X);
                TracerInfo("mag_data_Y: %d\r\n", mag_data.AXIS_Y);
                TracerInfo("mag_data_Z: %d\r\n", mag_data.AXIS_Z);                  

                TracerInfo("\r\n");            
            }                 
        }

     

#endif
        cc6801_ClockDelayMs(10);

        if (true == NRF_LOG_PROCESS())
        {
            
            continue;         
        }
        

    }
    
    

    if(E_APP_SRV_ERR_NONE != CC_AppSrv_Manager_Stop(E_APP_SRV_ID_PEDO))
    {
        TracerInfo("CC_AppSrv_Manager_Stop fail!\r\n");
    
        while(1)
            ;
    }



#endif
    
}


#endif

#endif
