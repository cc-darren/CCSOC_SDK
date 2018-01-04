#ifdef APP_SERV_MGR_EN

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "project.h"
#include "CC_AppSrvc_Manager.h"
#include "CC_Sensor_Manager.h"
#include "PPG_Controller.h"
#include "CC_HRM_drv.h"
//#include "pah8002_comm.h"
#include "pah8002_api_c.h"
//#include "CC_drv_gpio.h"


#include "error.h"
#include "tracer.h"


static bool pah8002_cmp_ppg_checksum(uint32_t * ppg_data, uint8_t *cks)
{
    uint32_t *s = ppg_data;
    uint32_t cks_cal = *s; 
    uint32_t cks_rx = *((uint32_t *)cks) ; 
    uint32_t i ; 
    
    //for(i = 1; i < HEART_RATE_MODE_SAMPLES_PER_READ; i++) 
    for(i = 1; i < HEART_RATE_MODE_SAMPLES_PER_READ/4; i++) // only for 60 Bytes
    { 
        cks_cal = cks_cal ^ (*(s+i)) ; 
    } 
    
    if(cks_cal != cks_rx) 
    {
        TracerInfo("checksum error %x != %x\r\n",cks_cal, cks_rx);
        return true;
    }
    else
        return false;
}




//////////////// Interface definition /////////////////////

E_PPG_Manager_Status PPG_Controller_Init(void)
{
    E_PPG_Manager_Status ret = E_PPG_SUCCESS;


    pah8002_twi_init(); 
    
    drvi_GpioWrite(HRM_RST_PIN, 1);     //power on hrm   
    
    if(MSG_SUCCESS == pah8002_sensor_init())
        return E_PPG_ERROR_INIT_FAIL;

   
    return ret;
}


E_PPG_Manager_Status PPG_Controller_Configure(E_App_Srv_ID UserID, void *Params)
{
    E_PPG_Manager_Status ret = E_PPG_SUCCESS;

    // do nothing, no settings need to config.

    return ret;
}



E_PPG_Manager_Status PPG_Controller_Start(E_App_Srv_ID UserID)
{
    E_PPG_Manager_Status ret = E_PPG_SUCCESS;

    pah8002_write_reg(0xd5, 1);

    return ret;
}


E_PPG_Manager_Status PPG_Controller_GetData(E_App_Srv_ID UserID, void* pSampleData, void *pDataSzInBytes)
{
    E_PPG_Manager_Status ret = E_PPG_SUCCESS;
    static uint8_t pah8002_ppg_data[HEART_RATE_MODE_SAMPLES_PER_READ * 4] ; // for test!!!!
	//uint16_t* pdataBytes = (uint16_t*) pDataSzInBytes;
	uint8_t cks[4];
    uint8_t int_req = 0; 

    *(uint16_t*) pDataSzInBytes = 0;


    pah8002_write_reg(0x7f, 0x02);
    pah8002_read_reg(0x73, &int_req); // check interrupt status


    if((int_req & 0x08) != 0) 
    {
         TracerInfo("fifo underflow\r\n"); 
         //return E_PPG_ERROR_FIFO_UNDERFLOW;
    }   
    
    if((int_req & 0x04) != 0) 
    {
         TracerInfo("fifo overflow\r\n"); 
         //return E_PPG_ERROR_FIFO_OVERFLOW;
    }   

    if((int_req & 0x01) != 0) // check fifo interrupt
    {   
        
          for(uint8_t i = 0; i < 4; i++)
          {
               pah8002_write_reg(0x7f, 0x03);
               pah8002_burst_read_reg((i*HEART_RATE_MODE_SAMPLES_PER_READ), ((uint8_t*)pSampleData + i*HEART_RATE_MODE_SAMPLES_PER_READ), HEART_RATE_MODE_SAMPLES_PER_READ);
            
               pah8002_write_reg(0x7f, 0x02);
               pah8002_burst_read_reg(0x80, cks, 4);
            
               pah8002_cmp_ppg_checksum((uint32_t*)((uint8_t*)pSampleData + i*HEART_RATE_MODE_SAMPLES_PER_READ), cks);
          }

          pah8002_write_reg(0x75, 0x01);
          pah8002_write_reg(0x75, 0x00);
          

          //*pdataBytes = HEART_RATE_MODE_SAMPLES_PER_READ*4;
          *(uint16_t*) pDataSzInBytes = HEART_RATE_MODE_SAMPLES_PER_READ*4;
    }
    else
        return E_PPG_ERROR_DATA_NOT_READY; 
    

    return ret;
}


E_PPG_Manager_Status PPG_Controller_Shutdown(E_App_Srv_ID UserID, bool power_off)
{
    E_PPG_Manager_Status ret = E_PPG_SUCCESS;

    if(true == power_off)      
    {        
        pah8002_sensor_deinit();
        
         drvi_GpioWrite(HRM_RST_PIN, 0);
        
        pah8002_twi_deinit();

    }
    
    return ret;
}


//////////////   interface of sensor manager  /////////////////

const struct sensor_manager_itfs ppg_itf =
{
    PPG_Controller_Init,
    PPG_Controller_Configure,
    PPG_Controller_Start,
    PPG_Controller_GetData,
    PPG_Controller_Shutdown
};


const struct sensor_manager_itfs* ppg_sm_itf_get(void)
{
    return &ppg_itf;
}

#endif
