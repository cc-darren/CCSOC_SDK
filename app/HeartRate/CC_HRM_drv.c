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

#include "project.h"
#include "CC_HRM_drv.h"
#include "pah8002_reg_ir2g1_.h"
#include "pah8002_api_c.h"
#include "ppg_pah8002.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

//#include "LSM6DS3_ACC_GYRO_driver.h"
#include "cc_db.h"
#include "CC_AppSrvc_HeartRate.h"
#include "clock.h"
#include "tracer.h"
#ifdef APP_SERV_MGR_EN
#include "CC_Sensor_Manager.h"
#endif

#ifdef HRM_EN


#define TOTAL_CHANNELS_FOR_ALG  3

//#define MEMS_ZERO  //Default Accelerometer data are all zero 
#define MEMS_SAMPLES_PER_CH_READ     50//20
#define MEMS_SAMPLES_PER_READ        (TOTAL_CHANNELS* MEMS_SAMPLES_PER_CH_READ)
//#define PPG_MODE_ONLY 


extern void CC_SetHrmDisplayHrmDataPageEvent(void);
extern uint8_t CC_MainGet_HrmData(void);
extern uint8_t CC_MainGet_HrmDataFlag(void);
extern eHrmOp_State CC_GetHrmStatus(void);
extern void CC_MainSet_HrmData(uint8_t _bHrmData);
extern void CC_SetHrmStatus(eHrmOp_State _eState);
extern void CC_MainGet_CurrentTime(app_date_time_t *_stCurTime);
extern eStete_t CC_BLE_Cme_Get_HeartRateStrapMode(void);

 
enum{ 
  SUSPEND_MODE = 0, 
  TOUCH_MODE, 
  NORMAL_MODE, 
  NORMAL_LONG_ET_MODE, 
  STRESS_MODE,  
  NONE, 
}; 

static uint8_t pah8002_ppg_data[HEART_RATE_MODE_SAMPLES_PER_READ * 4] ; 
//static uint8_t _touch_flag = 0 ; 
static volatile uint8_t _pah8002_interrupt = 0 ; 
static pah8002_data_t _pah8002_data; 
static uint32_t _timestamp = 0 ; 
#ifdef MEMS_ZERO 
static int16_t _mems_data[HEART_RATE_MODE_SAMPLES_PER_READ * 3] ; 
#else
static bool   hrm_mems_enabled = false;
static int16_t _mems_data[MEMS_SAMPLES_PER_READ*2]; // reserved double size for over sampling
static uint8_t hrm_mems_index = 0;

#endif 

static uint8_t _ir_dac = 0 ;
static uint8_t _ir_expo = 0 ; 
//static uint8_t _chip_id = 0 ; 
static uint8_t _pah8002_alg_local_buffer[12200];
static void *_pah8002_alg_buffer = NULL; 
//static volatile uint32_t tick_cnt; 
uint32_t sys_tick; 

static int touch_cnt = 0;
static int no_touch_cnt = 0 ;
static uint8_t touch_sts_output = 1 ; 

/*
void SysTick_Handler(void)
{
    tick_cnt++;
}


uint32_t get_sys_tick(void)
{
    return tick_cnt;
}
*/



static bool pah8002_wakeup(void)
{
    int retry = 0;
    int success = 0;
    uint8_t data = 0;
    int result = 0;

    do
    {
        result = pah8002_write_reg(0x7f, 0x00);
        if (result != MEMS_SUCCESS)
            continue;

        result = pah8002_read_reg(0, &data);
        if (result != MEMS_SUCCESS)
            continue;

        if(data == 0x02)
            success++;
        else
            success = 0 ;

        if(success >=1)
            break;
        retry ++;
    }while(retry <=20);

    if(data != 0x02)
    {
        TracerInfo("_WakeupSensor Fail id\r\n");
        TracerInfo("pah8002_check retry %d \r\n", retry);
        return false;
    }

    pah8002_write_reg(0x7f, 0x02); 
    pah8002_write_reg(0x70, 0x00); 
    
    return true;
}


static bool pah8002_sw_reset(void)
{
    if(MEMS_SUCCESS != pah8002_write_reg(0xe1, 0))  //write 0 to trigger Software Reset 
    { 
        TracerInfo("pah8002_sw_reset Fail \r\n");
        return false; 
    }       

    cc6801_ClockDelayMs(5); 
    
    return true;
}

static bool pah8002_enter_suspend_mode(void) 
{ 
    int i = 0 ; 

    //TracerInfo("pah8002_enter_suspend_mode \r\n");    
    pah8002_wakeup(); 
    pah8002_sw_reset(); 

    for(i = 0; i < SUSPEND_REG_ARRAY_SIZE;i++) 
    { 
        if ( pah8002_write_reg(suspend_register_array[i][0], 
                            suspend_register_array[i][1]) != 0 ) 
        { 
            return false; 
        } 
    }   

    return true; 
} 

bool pah8002_sensor_init(void)
{
    uint32_t open_size = 0; 

    // Setting algorithm buffer
    //Algorithm initialization 
    _pah8002_data.frame_count = 0 ; 
    _pah8002_data.nf_ppg_channel = TOTAL_CHANNELS_FOR_ALG; 
    _pah8002_data.nf_ppg_per_channel = HEART_RATE_MODE_SAMPLES_PER_CH_READ; 
    _pah8002_data.ppg_data = (int32_t *)pah8002_ppg_data; 

#ifdef MEMS_ZERO 
    memset(_mems_data, 0, sizeof(_mems_data)); 
    _pah8002_data.mems_data = _mems_data; 
    _pah8002_data.nf_mems = HEART_RATE_MODE_SAMPLES_PER_CH_READ; 
    for (int i = 0; i<(HEART_RATE_MODE_SAMPLES_PER_READ * 3);i++)
        _mems_data[i] = 0x0001;
#else
    memset(_mems_data, 0, sizeof(_mems_data)); 
    _pah8002_data.mems_data = _mems_data; 
    _pah8002_data.nf_mems = MEMS_SAMPLES_PER_CH_READ; 
#endif 

    // Allocate heap memory
    open_size = pah8002_query_open_size(); 
    TracerInfo("pah8002 heap size: %d\r\n", open_size);
    //_pah8002_alg_buffer = malloc(open_size);         
    _pah8002_alg_buffer = _pah8002_alg_local_buffer; // NO USE HEAP space!!!
    
    if(pah8002_open(_pah8002_alg_buffer) != MSG_SUCCESS)
        return false; 

    if (MSG_SUCCESS != pah8002_set_param(PAH8002_PARAM_IDX_GSENSOR_MODE, 2))
        return false;

    if (MSG_SUCCESS != pah8002_set_param(PAH8002_PARAM_IDX_HAS_IR_CH, 2))   
        return false;


    cc6801_ClockDelayMs(300);

    if (false == pah8002_wakeup())
        return false;

    if (false == pah8002_sw_reset())
        return false;

    for(int i = 0; i < INIT_PPG_REG_ARRAY_SIZE;i++) 
    {
        if ( pah8002_write_reg( init_ppg_register_array[i][0], 
            init_ppg_register_array[i][1]) != MEMS_SUCCESS ) 
        { 
            return false; 
        } 
    }

    pah8002_write_reg(0x7f, 0x00);         //Bank0 
    pah8002_read_reg(0x0D, &_ir_expo);  // IR Exposure Time 
    pah8002_write_reg(0x7f, 0x01);         //Bank1 
    pah8002_read_reg(0xBA, &_ir_dac);   //IR Led DAC 
    
//    TracerInfo("<<< pah8002_enter_normal_mode ir_dac %x, ir_expo %x\r\n", _ir_dac, _ir_expo); 

    pah8002_write_reg(0x7f, 0x01);
    
    // Adjust fifo size
    pah8002_write_reg(0xea, (HEART_RATE_MODE_SAMPLES_PER_READ + 1)); 

    //enable sensor, TG enable. REQTIMER_ENABLE
    pah8002_write_reg(0xd5, 1);  

    _timestamp = sys_tick = Hrm_get_sys_tick(); // 1ms tick

#ifndef MEMS_ZERO 
        hrm_mems_enabled = true;
    
	#ifdef FIFO_MODE_EN		
        #ifndef APP_SERV_MGR_EN  // not defined
            CC_LSM6DSX_Fifo_Accel_Register(MEMS_FIFO_USER_HRM, _mems_data, MEMS_SAMPLES_PER_READ);
        #endif
	#else
            hrm_mems_index = 0;
	#endif
    
#endif	

    return true;
}

void pah8002_sensor_deinit(void)
{
#ifndef MEMS_ZERO    
    hrm_mems_enabled = false;
#endif    
    pah8002_enter_suspend_mode(); 
    pah8002_close(); 

    if (_pah8002_alg_buffer) 
    { 
        free(_pah8002_alg_buffer); 
        _pah8002_alg_buffer = NULL; 
    } 

}

static uint32_t pah8002_update_timestamp(void)
{
    uint32_t new_timestamp = 0;
        
    sys_tick = Hrm_get_sys_tick();
    new_timestamp = sys_tick - _timestamp; 
    _timestamp = sys_tick;     

    return new_timestamp;
}

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

static uint8_t pah8002_get_touch_flag_ppg_mode() 
{ 
    int32_t *s = (int32_t *)pah8002_ppg_data ; 
    int32_t ch0 ; 
    int32_t ch1 ; 
    int64_t ir_rawdata; 
    int i; 

#define TouchDetection_Upper_TH (600) 
#define TouchDetection_Lower_TH (512) 


#define TouchDetection_Count_TH (3)        //(3+1)*50ms = 200ms 
#define NoTouchDetection_Count_TH (3)      //(3+1)*50ms = 200ms 

    for(i=0; i<HEART_RATE_MODE_SAMPLES_PER_READ; i+=TOTAL_CHANNELS) 
    { 
        ch0 = *s; 
        ch1 = *(s+1); 
        ir_rawdata = ch0 - ch1 ; 
        ir_rawdata = (ir_rawdata * _ir_dac * _ir_expo)>>20 ; 

        if( ir_rawdata > TouchDetection_Upper_TH) 
        { 
            touch_cnt++; 
            no_touch_cnt = 0; 
        } 
        else if( ir_rawdata < TouchDetection_Lower_TH) 
        { 
            no_touch_cnt++; 
            touch_cnt = 0 ; 
        } 
        else 
        { 
            touch_cnt = 0 ; 
            no_touch_cnt = 0; 
        }   

        s+=TOTAL_CHANNELS; 
    }  

    if(touch_cnt > TouchDetection_Count_TH) 
    { 
        touch_sts_output = 1; 
    } 
    else if( no_touch_cnt > NoTouchDetection_Count_TH) 
    { 
        touch_sts_output = 0; 
    } 
    return touch_sts_output;             
} 



void pah8002_log(void) 
{ 
    int i = 0 ; 
    uint32_t *ppg_data = (uint32_t *)_pah8002_data.ppg_data ; 
    int16_t *mems_data = _pah8002_data.mems_data ;     
    //TracerInfo("Time stamp, %d \r\n", _timestamp); 
    TracerInfo("Frame Count, %d \r\n", _pah8002_data.frame_count); 
    TracerInfo("Time, %d \r\n", _pah8002_data.time); 
    TracerInfo("PPG, %d, %d, ", _pah8002_data.touch_flag, _pah8002_data.nf_ppg_per_channel); 
    //TracerInfo("PPG_GSENSOR_RAW_DATA,");
    for(i=0; i<_pah8002_data.nf_ppg_channel * _pah8002_data.nf_ppg_per_channel; i++) 
    { 
        TracerInfo("%d, ", *ppg_data); 
        ppg_data ++; 
    } 
    TracerInfo("\r\n"); 
    TracerInfo("MEMS, %d, ", _pah8002_data.nf_mems); 
    //TracerInfo("MEMS\r\n"); 
    for(i=0; i<_pah8002_data.nf_mems*3; i++) 
    { 
        TracerInfo("%d, ", *mems_data); 
        mems_data ++; 
    } 
    TracerInfo("\r\n");  
} 




static void pah8002_task(void)
{
        uint8_t ret; 
        uint8_t cks[4]; 
        uint8_t int_req = 0; 
        int16_t hr_trust_level = 0;
        float _fpHr;


        pah8002_write_reg(0x7f, 0x02);
        pah8002_read_reg(0x73, &int_req); // check interrupt status


        if( (int_req & 0x08) != 0) 
        {
             TracerInfo("fifo underflow\r\n"); 
        }

        if( (int_req & 0x04) != 0) 
        {
             TracerInfo("fifo overflow\r\n"); 
        }

        if( (int_req & 0x02) != 0) // check touch interrupt
        {
             // touch interput
        }

        if( (int_req & 0x01) != 0) // check fifo interrupt
        {
             // filo data ready                      

             for(uint8_t i = 0; i < 4; i++)
             {
                 pah8002_write_reg(0x7f, 0x03);
                 pah8002_burst_read_reg((i*HEART_RATE_MODE_SAMPLES_PER_READ), &pah8002_ppg_data[i*HEART_RATE_MODE_SAMPLES_PER_READ], HEART_RATE_MODE_SAMPLES_PER_READ);
                 
                 pah8002_write_reg(0x7f, 0x02);
                 pah8002_burst_read_reg(0x80, cks, 4);
                 
                 pah8002_cmp_ppg_checksum((uint32_t*)&pah8002_ppg_data[i*HEART_RATE_MODE_SAMPLES_PER_READ], cks);
             }

             pah8002_write_reg(0x75, 0x01);
             pah8002_write_reg(0x75, 0x00);

           	// process algorithm
#ifdef MEMS_ZERO 
#else       
            //TracerInfo("mems_index(%d)\r\n",hrm_mems_index);

    #ifdef FIFO_MODE_EN	

        #ifdef APP_SERV_MGR_EN	
            uint32_t  size_in_bytes;

            if(E_SEN_ERROR_NONE != CC_SenMgr_Acc_GetData(E_SEN_USER_ID_HRM, _pah8002_data.mems_data, &size_in_bytes))
                 TracerInfo("CC_SenMgr_Acc_GetData fail!\r\n");

            _pah8002_data.nf_mems = size_in_bytes/(2*3);

            TracerInfo("nf_mems: %d\r\n", _pah8002_data.nf_mems);
                
        #else    
            _pah8002_data.nf_mems = CC_LSM6DSX_Fifo_Get_Accel_UnRead_Samples(MEMS_FIFO_USER_HRM);

            memcpy(_pah8002_data.mems_data, _mems_data, (_pah8002_data.nf_mems*2));

            CC_LSM6DSX_Fifo_Accel_Read_Done(MEMS_FIFO_USER_HRM);
        #endif
            //for(uint8_t i=0; i< (_pah8002_data.nf_mems);i++)
                //TracerInfo("mems(%f)\r\n", _mems_data[i]); 

#ifdef FORCE_HRS_TEST_EN

            TracerInfo( "ACC_Data[0] %d\r\n",_pah8002_data.mems_data[0]);
            TracerInfo( "ACC_Data[1] %d\r\n",_pah8002_data.mems_data[1]);
            TracerInfo( "ACC_Data[2] %d\r\n",_pah8002_data.mems_data[2]);      

            TracerInfo( "PPG_Data[0] %d\r\n",_pah8002_data.ppg_data[0]);
            TracerInfo( "PPG_Data[1] %d\r\n",_pah8002_data.ppg_data[1]);
            TracerInfo( "PPG_Data[2] %d\r\n",_pah8002_data.ppg_data[2]);   
            
#endif


            //TracerInfo("nf_mems(%d)\r\n", _pah8002_data.nf_mems); 
    #else
            TracerInfo("hr_idx(%d)\r\n", hrm_mems_index); 
            _pah8002_data.nf_mems = hrm_mems_index/3;

            memcpy(_pah8002_data.mems_data, _mems_data, MEMS_SAMPLES_PER_READ*2);

            hrm_mems_index = 0;
            hrm_mems_enabled = true;	
            //TracerInfo("get gyro sample number:%d\r\n",CC_LSM6DSX_FifoGetUnReadData());
    #endif
#endif 
            _pah8002_data.time = pah8002_update_timestamp();

            //TracerInfo("time: %d\r\n",_pah8002_data.time);

            _pah8002_data.touch_flag = pah8002_get_touch_flag_ppg_mode(); ; 

            //TracerInfo("sys_tick	%d\r\n", _pah8002_data.time); 
            //pah8002_log(); 

            ret = pah8002_entrance(&_pah8002_data); 

            //TracerInfo("ret = %d\r\n",ret); 
            if((ret & 0x0f) != 0) 
            { 
                 switch(ret) //check error status 
                 { 
                    case MSG_ALG_NOT_OPEN: 
                        TracerInfo("Algorithm is not initialized.\r\n"); 
                       	break; 
                    case MSG_MEMS_LEN_TOO_SHORT: 
                        TracerInfo("MEMS data length is shorter than PPG data length.\r\n"); 
                       	break; 
                    case MSG_NO_TOUCH: 
                        TracerInfo("PPG is no touch.\r\n"); 
                       	break; 
                    case MSG_PPG_LEN_TOO_SHORT: 
                        TracerInfo("PPG data length is too short.\r\n"); 
                       	break; 
                    case MSG_FRAME_LOSS: 
                        TracerInfo("Frame count is not continuous.\r\n"); 
                       	break; 
                    default:
                        TracerInfo("ret = %d\r\n",ret); 
                       	break;
                    } 
            } 
            else
            {

                if((ret & 0xf0) == MSG_HR_READY) 
                { 

                    pah8002_get_hr(&_fpHr) ; 

                    TracerInfo("HR = %d\r\n", (int16_t)(_fpHr)); 

                    pah8002_get_signal_grade(&hr_trust_level);
                  
                    //TracerInfo("HR Trust Level = %d\r\n", hr_trust_level);
                    CC_AppSrv_HR_DataReport(((int16_t) _fpHr), hr_trust_level);    //TBD: should use call-back func by registration instead
                   
                } 
                _pah8002_data.frame_count++; 
            }    
            CC_AppSrv_HR_StatusReport(ret);                  

        }    


}

void CC_HRM_PPG_INTR_ISR(void) 
{ 
    _pah8002_interrupt = 1 ; 
} 


uint8_t CC_HRM_PPG_INIT(void)
{
    //TracerInfo("CC_HRM_PPG_INIT \r\n");

    if (eHRM_Off == CC_GetHrmStatus())
    {
        CC_AppSrv_HR_StartSystemTick();
        
        pah8002_twi_init(); 
        
        drvi_GpioWrite(HRM_RST_PIN, 1);     //power on hrm 
        if (false == pah8002_sensor_init())
            return false;
        
        drvi_EnableIrq(HRM_INT_PIN);
            
        CC_SetHrmStatus(eHRM_On);  

        cc6801_ClockDelayMs(5);

        return true;
    }
    
    return false;
}

uint8_t CC_HRM_PPG_DEINIT(void)
{
    //TracerInfo("CC_HRM_PPG_DEINIT \r\n");

    if (eHRM_On == CC_GetHrmStatus())
    {
        CC_AppSrv_HR_StopSystemTick();
    
        drvi_DisableIrq(HRM_INT_PIN);
        
        pah8002_sensor_deinit();
        
        drvi_GpioWrite(HRM_RST_PIN, 0); 
        pah8002_twi_deinit();
        CC_SetHrmStatus(eHRM_Off);

        cc6801_ClockDelayMs(5);

        return true;
    }
    
    return false;
}


uint8_t CC_HRM_PPG_PROC(void)
{

    if(_pah8002_interrupt == 1) 
    {
        _pah8002_interrupt = 0; 

        if (false == pah8002_wakeup())
            return false;

        pah8002_task();
        
    }

    return true;
}

#ifndef MEMS_ZERO 
void CC_HRM_Get_Gsensor(int16_t *data)
{

    if(hrm_mems_enabled == true)
    {
        memcpy(&_mems_data[hrm_mems_index], data, 6);

        hrm_mems_index += 3;

        if(hrm_mems_index >= sizeof(_mems_data))
            hrm_mems_enabled = false;
    }
}
#endif


#endif


