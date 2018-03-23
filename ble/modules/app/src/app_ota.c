/**
 ****************************************************************************************
 *
 * @file app_ota.c
 *
 * @brief Health Thermometer Application entry point
 *
 * Copyright (C) RivieraWaves 2009-2015
 *
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup APP
 * @{
 ****************************************************************************************
 */

#include "rwip_config.h"     // SW configuration

//#if (BLE_APP_OTA)

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "app_ota.h"                  
#include "app.h"                     // Application Definitions
#include "app_task.h"                // application task definitions
#include "otat_task.h"               // health thermometer functions
#include "co_bt.h"
#include "prf_types.h"
#include "prf_utils.h"
#include "arch.h"                    // Platform Definitions

#include "co_math.h"
#include "ke_timer.h"
#include "drvi_clock.h"
#include "sw_timer.h"
#include "tracer.h"
#include "crc32.h"
#include "app_ccps.h"
#include "ota_handling.h"
#ifdef CC_OTA_SELF_TEST
#include "scheduler.h"
#endif

/*
 * DEFINES
 ****************************************************************************************
 */
#ifdef BOOTLOADER
#define OTA_ENC_CODE_SIZE       20
#endif

#ifdef CC_OTA_SELF_TEST
#define OTA_TEST_NB_OF_CHUNK        (6)
#define OTA_TEST_IMAGE_SIZE (OTA_CHUNK_SIZE * OTA_TEST_NB_OF_CHUNK)
#endif


/*
 * LOCAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

#ifdef BOOTLOADER
// encryption:
const static uint8_t bEncKey[OTA_ENC_CODE_SIZE] = {0x86, 0x16, 0x9d, 0xda, 0x59, 0x48, 0x99, 0x2b, 0xcc, 0x4b, 0x23, 0x82, 0x37, 0xb7, 0xb6, 0xc2, 0xbb, 0x71, 0xf6, 0x1c};
static uint8_t bEncIndex;

// ota buffer:
uint8_t bOTAEncryptedDataInChunk[OTA_CHUNK_SIZE] __attribute__((aligned(4)));
uint8_t bOTADataInChunk[OTA_CHUNK_SIZE] __attribute__((aligned(4)));

// config. settings. from peer
S_App_OTA_PeerConfig sAppOTAPeerConfig;

static uint32_t dwOTADataOffset;
static bool blIsCheckCRCInChunk;
static bool blIsCheckCRCInFlash;
static bool blImageIsConfirmed;
#endif
static bool blIsReadytoReboot;



#ifdef CC_OTA_SELF_TEST
static uint8_t baOTATestImage[OTA_TEST_IMAGE_SIZE];
static uint32_t dwOTATestImageCRC;
#endif



/*
 * LOCAL FUNCTION DEFINITIONS
 ****************************************************************************************
 */


#ifdef CC_OTA_SELF_TEST
void   APP_OTA_SelfTest_EventHandler(S_AppSchedEvent *tEvent);


void    APP_OTA_PostEvent_SlaveRequest(uint8_t *data, uint16_t size)
{
    S_AppSchedEvent     _tEvent;
        
    _tEvent.eModuleID     = E_APP_SCHED_MODID_OTA_SELFTEST_MGR;
    _tEvent.bEventID      = E_APP_OTA_ST_EVENT_SLAVE_REQUEST;
    _tEvent.wDataByteSize = size;
    _tEvent.vpData = ((void *) malloc(_tEvent.wDataByteSize));

    memcpy(_tEvent.vpData, data, size);
    
    APP_SCHED_PostEvent(&_tEvent);
}
#endif


void app_ota_notify_send(uint8_t *tx_data, uint8_t length)
{
                                            
#ifdef CC_OTA_SELF_TEST
    APP_OTA_PostEvent_SlaveRequest(tx_data, length);    
#else
    app_ccps_notify_send(tx_data, length);
#endif
}


/*
 * GLOBAL FUNCTION DEFINITIONS
 ****************************************************************************************
 */


void app_ota_init(void)
{
    #ifdef BOOTLOADER
    blImageIsConfirmed = false;
    #endif
    
    ota_handling_init();
    
#ifdef CC_OTA_SELF_TEST

    APP_SCHED_RegEventHandler(E_APP_SCHED_MODID_OTA_SELFTEST_MGR, APP_OTA_SelfTest_EventHandler);


   for(uint32_t i = 0; i < OTA_TEST_IMAGE_SIZE; i++)
        baOTATestImage[i] = (i % 256);

    // calculate crc in file
    dwOTATestImageCRC = crc32_compute(baOTATestImage, OTA_TEST_IMAGE_SIZE, NULL);
#endif

}


static void APP_OTA_Reboot_Set(void)
{

    blIsReadytoReboot = true;
}


bool APP_OTA_RebootIsReady(void)
{
    
    return blIsReadytoReboot;
}




/*
 * OTA Response
 ****************************************************************************************
 */

static void APP_OTA_Start_Rsp(uint8_t result)
{
    
    S_App_CC_Messages tx_msg;                    
    S_CCPS_OTA_START_RSP tx_rsp;


    // Generic Ack for CMD_START
    tx_msg.len      = SIZE_OF_CC_MSG_HDR;
    tx_msg.type     = E_CCPS_FTYPE_OTA;
    tx_msg.op       = E_CCPS_OPCODE_OTA_CMD_START | 0x80;    

    app_ota_notify_send((uint8_t*)&tx_msg, (tx_msg.len + SIZE_OF_CC_MSG_LEN)); 


    // RSP_START
    tx_rsp.result   = result;

    tx_msg.len      = sizeof(S_CCPS_OTA_START_RSP) + SIZE_OF_CC_MSG_HDR;
    tx_msg.type     = E_CCPS_FTYPE_OTA;
    tx_msg.op       = E_CCPS_OPCODE_OTA_RSP_START;    
    memcpy(tx_msg.data, &tx_rsp, sizeof(S_CCPS_OTA_START_RSP));

    app_ota_notify_send((uint8_t*)&tx_msg, (tx_msg.len + SIZE_OF_CC_MSG_LEN)); 
}


static void APP_OTA_Stop_Rsp(void)
{
    
    S_App_CC_Messages tx_msg;                    

    // Generic Ack for CMD_STOP
    tx_msg.len      = SIZE_OF_CC_MSG_HDR;
    tx_msg.type     = E_CCPS_FTYPE_OTA;
    tx_msg.op       = E_CCPS_OPCODE_OTA_CMD_STOP | 0x80;    

    app_ota_notify_send((uint8_t*)&tx_msg, (tx_msg.len + SIZE_OF_CC_MSG_LEN)); 
}


static void APP_OTA_Reconnect_Rsp(void)
{
    
    S_App_CC_Messages tx_msg;                    

    // Generic Ack for CMD_RECONNECT
    tx_msg.len      = SIZE_OF_CC_MSG_HDR;
    tx_msg.type     = E_CCPS_FTYPE_OTA;
    tx_msg.op       = E_CCPS_OPCODE_OTA_CMD_RECONNECT | 0x80;    

    app_ota_notify_send((uint8_t*)&tx_msg, (tx_msg.len + SIZE_OF_CC_MSG_LEN)); 
}


#ifdef BOOTLOADER
static void APP_OTA_Config_Rsp(uint8_t  result,      S_App_OTA_LocalConfig  *pLocalConfig)
{
    
    S_App_CC_Messages tx_msg;                    
    S_CCPS_OTA_CONFIG_RSP tx_rsp;


    // Generic Ack for CMD_CONFIG
    tx_msg.len      = SIZE_OF_CC_MSG_HDR;
    tx_msg.type     = E_CCPS_FTYPE_OTA;
    tx_msg.op       = E_CCPS_OPCODE_OTA_CMD_CONFIG | 0x80;    

    app_ota_notify_send((uint8_t*)&tx_msg, (tx_msg.len + SIZE_OF_CC_MSG_LEN)); 


    // RSP_CONFIG
    tx_rsp.result           = result;

    if(E_CCPS_OTA_CONFIG_RSP_SUCCESS == result)
    {
        tx_rsp.signuature       = pLocalConfig->signature;
        tx_rsp.chunk_size       = pLocalConfig->chunk_size;
        tx_rsp.chunk_offset     = pLocalConfig->chunk_offset;
    }
    else
    {
        tx_rsp.signuature       = 0;
        tx_rsp.chunk_size       = 0;
        tx_rsp.chunk_offset     = 0;
    }
    
    tx_msg.len      = sizeof(S_CCPS_OTA_CONFIG_RSP) + SIZE_OF_CC_MSG_HDR;
    tx_msg.type     = E_CCPS_FTYPE_OTA;
    tx_msg.op       = E_CCPS_OPCODE_OTA_RSP_CONFIG;    
    memcpy(tx_msg.data, &tx_rsp, sizeof(S_CCPS_OTA_CONFIG_RSP));

    app_ota_notify_send((uint8_t*)&tx_msg, (tx_msg.len + SIZE_OF_CC_MSG_LEN)); 
}



static void APP_OTA_Chunk_Offset_Rsp(uint8_t result)
{
    
    S_App_CC_Messages tx_msg;                    
    S_CCPS_OTA_CHUNK_OFFSET_RSP tx_rsp;


    // Generic Ack for CMD_OFFSET
    tx_msg.len      = SIZE_OF_CC_MSG_HDR;
    tx_msg.type     = E_CCPS_FTYPE_OTA;
    tx_msg.op       = E_CCPS_OPCODE_OTA_CMD_OFFSET | 0x80;    

    app_ota_notify_send((uint8_t*)&tx_msg, (tx_msg.len + SIZE_OF_CC_MSG_LEN)); 

    // RSP_OFFSET
    tx_rsp.result   = result;

    tx_msg.len      = sizeof(S_CCPS_OTA_CHUNK_OFFSET_RSP) + SIZE_OF_CC_MSG_HDR;
    tx_msg.type     = E_CCPS_FTYPE_OTA;
    tx_msg.op       = E_CCPS_OPCODE_OTA_RSP_OFFSET;    
    memcpy(tx_msg.data, &tx_rsp, sizeof(S_CCPS_OTA_CHUNK_OFFSET_RSP));

    app_ota_notify_send((uint8_t*)&tx_msg, (tx_msg.len + SIZE_OF_CC_MSG_LEN)); 
}


static void APP_OTA_CRC_Rsp(uint32_t crc)
{
    
    S_App_CC_Messages tx_msg;                    
    S_CCPS_OTA_CRC_RSP tx_rsp;


    // Generic Ack for CMD_CRC
    tx_msg.len      = SIZE_OF_CC_MSG_HDR;
    tx_msg.type     = E_CCPS_FTYPE_OTA;
    tx_msg.op       = E_CCPS_OPCODE_OTA_CMD_CRC | 0x80;    

    app_ota_notify_send((uint8_t*)&tx_msg, (tx_msg.len + SIZE_OF_CC_MSG_LEN)); 

    // RSP_CRC
    tx_rsp.crc      = crc;

    tx_msg.len      = sizeof(S_CCPS_OTA_CRC_RSP) + SIZE_OF_CC_MSG_HDR;
    tx_msg.type     = E_CCPS_FTYPE_OTA;
    tx_msg.op       = E_CCPS_OPCODE_OTA_RSP_CRC;    
    memcpy(tx_msg.data, &tx_rsp, sizeof(S_CCPS_OTA_CRC_RSP));

    app_ota_notify_send((uint8_t*)&tx_msg, (tx_msg.len + SIZE_OF_CC_MSG_LEN)); 
}


static void APP_OTA_CRC_Confirm_Rsp(void)
{
    
    S_App_CC_Messages tx_msg;                      

    // Generic Ack for CMD_CRC_CONFIRM
    tx_msg.len      = SIZE_OF_CC_MSG_HDR;
    tx_msg.type     = E_CCPS_FTYPE_OTA;
    tx_msg.op       = E_CCPS_OPCODE_OTA_CMD_CRC_CONFIRM | 0x80;    

    app_ota_notify_send((uint8_t*)&tx_msg, (tx_msg.len + SIZE_OF_CC_MSG_LEN)); 
}
#endif





/*
 * OTA Handler
 ****************************************************************************************
 */


void APP_OTA_MsgHandler(S_App_CC_Messages *msg)
{


    //TracerInfo("Rx OTA Messge Type: 0x%x\r\n", msg->op ); 


    switch(msg->op)
    {
        
        case E_CCPS_OPCODE_OTA_CMD_START:      
        {
            
            TracerInfo("\r\nRx OTA CMD_START\r\n"); 

            //app_ota_init();
            
            #ifdef BOOTLOADER
            
            APP_OTA_Start_Rsp(E_CCPS_OTA_START_RSP_SUCCESS);           
            
            #else           
            
            ota_set_mode_enabled();  
               
            APP_OTA_Start_Rsp(E_CCPS_OTA_START_RSP_RECONNECT);            
            #endif

        }break;


        #ifdef BOOTLOADER
        case E_CCPS_OPCODE_OTA_CMD_CONFIG:
        {
            
            S_CCPS_OTA_CONFIG_REQ  *ptData = (S_CCPS_OTA_CONFIG_REQ *) msg->data;
            S_App_OTA_LocalConfig  *pLocalConfig;


            TracerInfo("\r\nRx OTA CMD_CONFIG\r\n"); 

            
            ota_get_local_config(&pLocalConfig);

            // save remote parameters in RAM
            sAppOTAPeerConfig.signature   = ptData->signature;
            sAppOTAPeerConfig.file_size   = ptData->file_size;
            sAppOTAPeerConfig.file_crc    = ptData->file_crc;            
            sAppOTAPeerConfig.prog_addr   = ptData->prog_addr;   
            sAppOTAPeerConfig.enc         = ptData->enc;         

            TracerInfo("File Info:\r\n");
            TracerInfo("signature: 0x%04x\r\n", sAppOTAPeerConfig.signature);
            TracerInfo("file_size: 0x%04x\r\n", sAppOTAPeerConfig.file_size);
            TracerInfo("file_crc: 0x%04x\r\n",  sAppOTAPeerConfig.file_crc);
            TracerInfo("prog_addr: 0x%04x\r\n", sAppOTAPeerConfig.prog_addr);            
            TracerInfo("enc: %d\r\n",           sAppOTAPeerConfig.enc);


          
            if(true == ota_check_config_is_ok(&sAppOTAPeerConfig))
            {                        
                APP_OTA_Config_Rsp(E_CCPS_OTA_CONFIG_RSP_SUCCESS, pLocalConfig);             
            }
            else
            {
                APP_OTA_Config_Rsp(E_CCPS_OTA_CONFIG_RSP_NOT_ACCEPTED, pLocalConfig);  
            }

        }break;


        case E_CCPS_OPCODE_OTA_CMD_OFFSET:
        {
            
            S_CCPS_OTA_CHUNK_OFFSET_CMD *ptData = (S_CCPS_OTA_CHUNK_OFFSET_CMD *) msg->data;


            TracerInfo("\r\nRx OTA CMD_OFFSET\r\n");


            sAppOTAPeerConfig.chunk_offset = ptData->chunk_offset; 


            if(0 == ptData->chunk_offset)
            {
                //TracerInfo("ota_save_all_peer_config\r\n");
                

                ota_save_all_peer_config(&sAppOTAPeerConfig);

                ota_clr_image_valid_state();

                ota_flash_erase_fw_space();    

                TracerInfo("prog_addr: 0x%04x\r\n",    sAppOTAPeerConfig.prog_addr);         
                TracerInfo("file_size: 0x%04x\r\n",    sAppOTAPeerConfig.file_size);
                TracerInfo("file_crc: 0x%04x\r\n",     sAppOTAPeerConfig.file_crc);
                TracerInfo("signature: 0x%04x\r\n",    sAppOTAPeerConfig.signature);
                TracerInfo("chunk_offset: 0x%04x\r\n", sAppOTAPeerConfig.chunk_offset);              
                TracerInfo("enc: %d\r\n",              sAppOTAPeerConfig.enc);                
            }
            else
            {
                TracerInfo("chunk_offset: 0x%04x\r\n", sAppOTAPeerConfig.chunk_offset);
            }

            
            
            dwOTADataOffset  = 0;
            bEncIndex   = (ota_get_prog_image_offset(sAppOTAPeerConfig.chunk_offset) % OTA_ENC_CODE_SIZE);   

           
            // Assume OK:
            APP_OTA_Chunk_Offset_Rsp(E_CCPS_OTA_CHUNK_OFFSET_RSP_OK);          
            
        }break;


        case E_CCPS_OPCODE_OTA_CMD_CRC:
        {
            
            S_CCPS_OTA_CRC_CMD *ptData = (S_CCPS_OTA_CRC_CMD *) msg->data;


            TracerInfo("\r\nRx OTA CMD_CRC\r\n");
            

            if(E_CCPS_OTA_CRC_REQ_CHUNK == ptData->area)
            {
                uint32_t crc, data_size;                    

                blIsCheckCRCInChunk = true;

                data_size = dwOTADataOffset;

                if(0x01 == sAppOTAPeerConfig.enc)
                {
                    crc = crc32_compute(bOTAEncryptedDataInChunk, data_size, NULL);
                }
                else
                {
                    crc = crc32_compute(bOTADataInChunk, data_size, NULL);
                }

                TracerInfo("CRC in Chunk: 0x%04x, data_offset: %d\r\n", crc, dwOTADataOffset);
                

                APP_OTA_CRC_Rsp(crc); 

            }
            else if(E_CCPS_OTA_CRC_REQ_FILE == ptData->area)
            {

                uint32_t crc;
                
                blIsCheckCRCInFlash = true;
                
                crc = ota_get_file_crc();
                
                TracerInfo("CRC in Flash: 0x%04x\r\n", crc);
                

                APP_OTA_CRC_Rsp(crc);
                
            }
           
            

        }break;


        case E_CCPS_OPCODE_OTA_CMD_CRC_CONFIRM:
        {


            S_CCPS_OTA_CRC_CONFIRM_REQ *ptData = (S_CCPS_OTA_CRC_CONFIRM_REQ *) msg->data;


            TracerInfo("\r\nRx OTA CMD_CRC_CONFIRM\r\n");
            

            if(E_CCPS_OTA_CRC_CONFIRM_OK == ptData->result)
            {

                if(true == blIsCheckCRCInChunk)
                {
                    
                    uint32_t data_size = dwOTADataOffset;
                    
                    blIsCheckCRCInChunk = false;              

                    ota_save_chunk_offset(sAppOTAPeerConfig.chunk_offset);
                    
                    ota_flash_write_chunk_data(bOTADataInChunk, data_size);

                                        
                    APP_OTA_CRC_Confirm_Rsp();      
                    
                }                
                else if(true == blIsCheckCRCInFlash)
                {
                
                    blIsCheckCRCInFlash = false;

                    blImageIsConfirmed = true;
                    
                    APP_OTA_CRC_Confirm_Rsp();
                                
                    TracerInfo("Check CRC is ok in flash\r\n");
                    
                }
                else
                {
                    TracerInfo("ERROR: No CRC should be confirmed!\r\n");
                }

            }
            else
            {
                // crc confirm fail
                TracerInfo("CRC error has been informed by Master !\r\n");

            }

        }break;
            
        #endif // end of #ifdef BOOTLOADER


        case E_CCPS_OPCODE_OTA_CMD_STOP:
        {
            
            TracerInfo("\r\nRx OTA CMD_STOP\r\n");
           
            #ifdef BOOTLOADER
            S_CCPS_OTA_STOP_OTA_CMD *ptData = (S_CCPS_OTA_STOP_OTA_CMD *) msg->data;
            
            if(E_CCPS_OTA_STOP_OTA_SUCCESS == ptData->result)
            {
                
                TracerInfo("OTA is ok, ready to reboot...\r\n");

                if(true == blImageIsConfirmed)
                {
                    blImageIsConfirmed = false;
                                     
                    ota_set_image_valid_state();
                }

            }
            else
            {
                TracerInfo("OTA is fail!! ready to reboot...\r\n");

                ota_clr_local_config();

            }
            #endif

            APP_OTA_Reboot_Set();
                
            APP_OTA_Stop_Rsp();            
            
        }break;


        case E_CCPS_OPCODE_OTA_CMD_RECONNECT:
        {

            TracerInfo("\r\nRx OTA CMD_RECONNECT\r\n");

            APP_OTA_Reboot_Set();
            
            APP_OTA_Reconnect_Rsp();

        }break;


        default:
        
            TracerInfo("ERROR! Unsupport OP Code: 0x%x\r\n", msg->op);
            
            break;
    }


}



#ifdef BOOTLOADER
static void ota_decrypt_data(uint8_t *buf, uint32_t size)
{
    
    for(uint32_t i = 0; i < size; i++)
    {

        bEncIndex %= OTA_ENC_CODE_SIZE;

        buf[i] ^= bEncKey[bEncIndex];
        buf[i] -= bEncIndex;

        bEncIndex++;

    }

}


void APP_OTA_Image_Write(const uint8_t *image_data, uint16_t data_size)
{


    if((dwOTADataOffset + data_size) <= OTA_CHUNK_SIZE)
    {

        TracerInfo("Rx Raw Data, data_offset: %d\r\n", dwOTADataOffset);     
        

        if(0x01 == sAppOTAPeerConfig.enc)
        {
            uint8_t *decry_data = (uint8_t*) malloc(data_size);

            if(decry_data == NULL)
            {
                TracerInfo("HEAP Size is not enough!\r\n");
                
                return;
            }

            TracerInfo("Decrypt Image...\r\n");
            
            memcpy(&bOTAEncryptedDataInChunk[dwOTADataOffset], image_data, data_size);               
            
            memcpy(decry_data, image_data, data_size);

            ota_decrypt_data(decry_data, data_size);

            memcpy(&bOTADataInChunk[dwOTADataOffset], decry_data, data_size);

            free(decry_data);
        }
        else
            memcpy(&bOTADataInChunk[dwOTADataOffset], image_data, data_size);         


        #if 0
        for(uint16_t i = 0; i < data_size; i++)
        {            
            TracerInfo("0x%02x, ",image_data[i]);
        }

        TracerInfo("\r\n");
        #endif
        
        dwOTADataOffset += data_size;
    
    }
    else
    {
        // Error!!
        TracerInfo("ERROR! Data_Offset > Max of Chunk Size: %d\r\n", dwOTADataOffset + data_size);
    }

}
#endif




#ifdef BOOTLOADER
#ifdef CC_OTA_SELF_TEST


/*
 * OTA Self-Test Code
 ****************************************************************************************
 */

void   APP_OTA_SelfTest_EventHandler(S_AppSchedEvent *tEvent)
{
    
    //TracerInfo(  "[APP_OTA_SelfTest_EventHandler] event: %d, argv: 0x%08X\r\n"
    //           , tEvent->bEventID
    //           , ((uint32_t) tEvent->vpData)                            );


    switch (tEvent->bEventID)
    {
        
         case E_APP_OTA_ST_EVENT_MASTER_REQUEST:
         {
            
             S_App_CC_Messages *ptAppCCMessage = tEvent->vpData;            

             TracerInfo("Event from Master\r\n");

             TracerInfo("Length: %d, type: 0x%02x, op: 0x%02x\r\n", ptAppCCMessage->len, ptAppCCMessage->type, ptAppCCMessage->op);

             if(ptAppCCMessage->len >= SIZE_OF_CC_MSG_HDR) 
             {

                 for(uint16_t i = 0; i < (ptAppCCMessage->len - SIZE_OF_CC_MSG_HDR); i++)
                 {
                    TracerInfo("0x%02x, ", ptAppCCMessage->data[i]);
                 }
                 /*
                 for(uint16_t i = 0; i < tEvent->wDataByteSize; i++)
                 {
                     TracerInfo("0x%02x, ", (uint8_t)tEvent->vpData[i]);
                 }
                 */
                 TracerInfo("\r\n");

                 APP_OTA_MsgHandler(ptAppCCMessage);
             }

             break;
         }
         
         case E_APP_OTA_ST_EVENT_SLAVE_REQUEST:
         {

             S_App_CC_Messages *ptAppCCMessage = tEvent->vpData;

             TracerInfo("Event from Slave\r\n");

             TracerInfo("Length: %d, type: 0x%02x, op: 0x%02x\r\n", ptAppCCMessage->len, ptAppCCMessage->type, ptAppCCMessage->op);

             if(ptAppCCMessage->len >= SIZE_OF_CC_MSG_HDR) 
             {
                 for(uint16_t i = 0; i < (ptAppCCMessage->len - SIZE_OF_CC_MSG_HDR); i++)
                 {
                     TracerInfo("0x%02x, ", ptAppCCMessage->data[i]);
                 }
             }
             
             TracerInfo("\r\n");
                          
             break;
         }
         
    }
    
}


void    APP_OTA_PostEvent_MasterRequest(uint8_t *data, uint16_t size)
{
    
    S_AppSchedEvent     _tEvent;
   
    
    _tEvent.eModuleID     = E_APP_SCHED_MODID_OTA_SELFTEST_MGR;
    _tEvent.bEventID      = E_APP_OTA_ST_EVENT_MASTER_REQUEST;
    _tEvent.wDataByteSize = size;
    _tEvent.vpData = ((void *) malloc(_tEvent.wDataByteSize));

    memcpy(_tEvent.vpData, data, size);
    
    APP_SCHED_PostEvent(&_tEvent);


    #if 0 // debug:
    TracerInfo("Master Request:\r\n");        
    for(uint16_t i = 0; i < size; i++)
    {
        TracerInfo("0x%02x, ", data[i]);
    }

    TracerInfo("\r\n");    
    #endif
}


void APP_OTA_Self_Test(void)
{

    S_App_CC_Messages tx_msg;                    
    uint16_t delayMS;

    TracerInfo("======  CC_OTA_Self_Test Start!  ======\r\n");


    
    // Send OTA_CMD: Start OTA

    TracerInfo("\r\n=== [Step 1.1]: Start OTA ===\r\n");
    

    tx_msg.len      = SIZE_OF_CC_MSG_HDR; 
    tx_msg.type     = E_CCPS_FTYPE_OTA;
    tx_msg.op       = E_CCPS_OPCODE_OTA_CMD_START;

    APP_OTA_PostEvent_MasterRequest((uint8_t*)&tx_msg, (tx_msg.len + SIZE_OF_CC_MSG_LEN));


    delayMS = 1000;
    
    do
    {
        APP_SCHED_RunScheduler();

        cc6801_ClockDelayMs(1);
    }while (delayMS--);
    


    // Send OTA_CMD: Start OTA

    TracerInfo("\r\n=== [Step 1.2]: Start OTA ===\r\n");
    
    tx_msg.len      = SIZE_OF_CC_MSG_HDR; 
    tx_msg.type     = E_CCPS_FTYPE_OTA;
    tx_msg.op       = E_CCPS_OPCODE_OTA_CMD_START;

    APP_OTA_PostEvent_MasterRequest((uint8_t*)&tx_msg, (tx_msg.len + SIZE_OF_CC_MSG_LEN));


    delayMS = 1000;
    
    do
    {
        APP_SCHED_RunScheduler();

        cc6801_ClockDelayMs(1);
    }while (delayMS--);


    

    // Send OTA_CONFIG_REQ
    TracerInfo("\r\n=== [Step 2]: Send OTA_CONFIG_REQ ===\r\n");
    
    S_CCPS_OTA_CONFIG_REQ tx_config;


    tx_config.signature  = 0x12345678;
    tx_config.file_size  = OTA_TEST_IMAGE_SIZE;
    tx_config.file_crc   = dwOTATestImageCRC;    
    tx_config.prog_addr  = 0x10025800;
    tx_config.enc        = false;

    TracerInfo("OTA Config from Master:\r\n");
    TracerInfo("signature: 0x%04x\r\n", tx_config.signature);
    TracerInfo("file_size: 0x%04x\r\n", tx_config.file_size);
    TracerInfo("file_crc: 0x%04x\r\n",  tx_config.file_crc);    
    TracerInfo("prog_addr: 0x%04x\r\n", tx_config.prog_addr);
    TracerInfo("enc: %d\r\n",           tx_config.enc);

    
    tx_msg.len      = sizeof(S_CCPS_OTA_CONFIG_REQ) + SIZE_OF_CC_MSG_HDR;
    tx_msg.type     = E_CCPS_FTYPE_OTA;
    tx_msg.op       = E_CCPS_OPCODE_OTA_CMD_CONFIG;
    memcpy(tx_msg.data, &tx_config, sizeof(S_CCPS_OTA_CONFIG_REQ));

    APP_OTA_PostEvent_MasterRequest((uint8_t*)&tx_msg, (tx_msg.len + SIZE_OF_CC_MSG_LEN));


    delayMS = 1000;
    
    do
    {
        APP_SCHED_RunScheduler();

        cc6801_ClockDelayMs(1);
    }while (delayMS--);


    S_CCPS_OTA_CRC_CMD          tx_crc_area;     
    S_CCPS_OTA_CRC_CONFIRM_REQ  tx_crc_confirm;


    for(uint32_t chunk_cnt = 0; chunk_cnt < OTA_TEST_NB_OF_CHUNK; chunk_cnt++)
    {
 
        TracerInfo("\r\n=== chunk_cnt: %d ===\r\n", chunk_cnt);



        // Send CHUNK_OFFSET from offset 0
        TracerInfo("\r\n=== [Step 3]: Send CHUNK_OFFSET ===\r\n");
        
        S_CCPS_OTA_CHUNK_OFFSET_CMD tx_chunk_size;

        tx_chunk_size.chunk_offset  = chunk_cnt; // 0;

        tx_msg.len      = sizeof(S_CCPS_OTA_CHUNK_OFFSET_CMD) + SIZE_OF_CC_MSG_HDR;
        tx_msg.type     = E_CCPS_FTYPE_OTA;
        tx_msg.op       = E_CCPS_OPCODE_OTA_CMD_OFFSET;
        memcpy(tx_msg.data, &tx_chunk_size, sizeof(S_CCPS_OTA_CHUNK_OFFSET_CMD));

        APP_OTA_PostEvent_MasterRequest((uint8_t*)&tx_msg, (tx_msg.len + SIZE_OF_CC_MSG_LEN));


        delayMS = 1000;
        
        do
        {
            APP_SCHED_RunScheduler();

            cc6801_ClockDelayMs(1);
        }while (delayMS--);    




        // Send RAW_DATA
        TracerInfo("\r\n=== [Step 4]: Send RAW_DATA ===\r\n");
        #if 0    
        S_App_CC_OP_RAW_DATA    tx_raw_data;

        tx_raw_data.op          = E_APP_OTA_RAW_DATA;
        memcpy(tx_raw_data.raw_data, baOTATestImage, OTA_CHUNK_SIZE);


        tx_msg.len      = OTA_CHUNK_SIZE + SIZE_OF_CC_MSG_TYPE + 1;   // +1: raw_data->op 
        tx_msg.type     = E_CCPS_FTYPE_OTA;
        memcpy(tx_msg.data, &tx_raw_data, (OTA_CHUNK_SIZE + 1));      // +1: raw_data->op 

        APP_OTA_PostEvent_MasterRequest((uint8_t*)&tx_msg, (tx_msg.len + SIZE_OF_CC_MSG_LEN));
        #endif

        APP_OTA_Image_Write(baOTATestImage, OTA_CHUNK_SIZE);

        delayMS = 1000;
        
        do
        {
            APP_SCHED_RunScheduler();

            cc6801_ClockDelayMs(1);
        }while (delayMS--);



        // Send OTA_CMD: CRC Check in Chunk
        TracerInfo("\r\n=== [Step 5]: Send OTA_CMD: Check CRC in Chunk ===\r\n");
    
      
        tx_crc_area.area  = E_CCPS_OTA_CRC_REQ_CHUNK;

        tx_msg.len      = sizeof(S_CCPS_OTA_CRC_CMD) + SIZE_OF_CC_MSG_HDR;
        tx_msg.type     = E_CCPS_FTYPE_OTA;
        tx_msg.op       = E_CCPS_OPCODE_OTA_CMD_CRC;
        memcpy(tx_msg.data, &tx_crc_area, sizeof(S_CCPS_OTA_CRC_CMD));

        APP_OTA_PostEvent_MasterRequest((uint8_t*)&tx_msg, (tx_msg.len + SIZE_OF_CC_MSG_LEN));


        delayMS = 1000;
        
        do
        {
            APP_SCHED_RunScheduler();

            cc6801_ClockDelayMs(1);
        }while (delayMS--);



        // Send OTA_CMD: CRC OK!
        TracerInfo("\r\n=== [Step 6]: Send OTA_CMD: CRC OK! ===\r\n");
        
        tx_crc_confirm.result  = E_CCPS_OTA_CRC_CONFIRM_OK;


        tx_msg.len      = sizeof(S_CCPS_OTA_CRC_CONFIRM_REQ) + SIZE_OF_CC_MSG_HDR;
        tx_msg.type     = E_CCPS_FTYPE_OTA;
        tx_msg.op       = E_CCPS_OPCODE_OTA_CMD_CRC_CONFIRM;
        memcpy(tx_msg.data, &tx_crc_confirm, sizeof(S_CCPS_OTA_CRC_CONFIRM_REQ));

        APP_OTA_PostEvent_MasterRequest((uint8_t*)&tx_msg, (tx_msg.len + SIZE_OF_CC_MSG_LEN));


        delayMS = 1000;
        
        do
        {
            APP_SCHED_RunScheduler();

            cc6801_ClockDelayMs(1);
        }while (delayMS--);    

  
    }
    

    // Send OTA_CMD: CRC check in Flash
    TracerInfo("\r\n=== [Step 7]: Send OTA_CMD: Check CRC in Flash ===\r\n");
    
    tx_crc_area.area  = E_CCPS_OTA_CRC_REQ_FILE;


    tx_msg.len      = sizeof(S_CCPS_OTA_CRC_CMD) + SIZE_OF_CC_MSG_HDR;
    tx_msg.type     = E_CCPS_FTYPE_OTA;
    tx_msg.op       = E_CCPS_OPCODE_OTA_CMD_CRC;
    memcpy(tx_msg.data, &tx_crc_area, sizeof(S_CCPS_OTA_CRC_CMD));

    APP_OTA_PostEvent_MasterRequest((uint8_t*)&tx_msg, (tx_msg.len + SIZE_OF_CC_MSG_LEN));



    delayMS = 1000;
    
    do
    {
        APP_SCHED_RunScheduler();

        cc6801_ClockDelayMs(1);
    }while (delayMS--);



    // Send OTA_CMD: CRC OK!
    TracerInfo("\r\n=== [Step 8]: Send OTA_CMD: CRC OK! ===\r\n");
    

    tx_crc_confirm.result  = E_CCPS_OTA_CRC_CONFIRM_OK;


    tx_msg.len      = sizeof(S_CCPS_OTA_CRC_CONFIRM_REQ) + SIZE_OF_CC_MSG_HDR;
    tx_msg.type     = E_CCPS_FTYPE_OTA;
    tx_msg.op       = E_CCPS_OPCODE_OTA_CMD_CRC_CONFIRM;
    memcpy(tx_msg.data, &tx_crc_confirm, sizeof(S_CCPS_OTA_CRC_CONFIRM_REQ));

    APP_OTA_PostEvent_MasterRequest((uint8_t*)&tx_msg, (tx_msg.len + SIZE_OF_CC_MSG_LEN));


    delayMS = 1000;
    
    do
    {
        APP_SCHED_RunScheduler();

        cc6801_ClockDelayMs(1);
    }while (delayMS--);    



    // Send OTA_STOP:
    TracerInfo("\r\n=== [Step 9]: Send OTA_STOP ===\r\n");
    
    S_CCPS_OTA_STOP_OTA_CMD  tx_ota_stop;

    tx_ota_stop.result  = E_CCPS_OTA_CRC_CONFIRM_OK;


    tx_msg.len      = sizeof(S_CCPS_OTA_STOP_OTA_CMD) + SIZE_OF_CC_MSG_HDR;
    tx_msg.type     = E_CCPS_FTYPE_OTA;
    tx_msg.op       = E_CCPS_OPCODE_OTA_CMD_STOP;
    memcpy(tx_msg.data, &tx_ota_stop, sizeof(S_CCPS_OTA_STOP_OTA_CMD));

    APP_OTA_PostEvent_MasterRequest((uint8_t*)&tx_msg, (tx_msg.len + SIZE_OF_CC_MSG_LEN));


    delayMS = 1000;
    
    do
    {
        APP_SCHED_RunScheduler();

        cc6801_ClockDelayMs(1);
    }while (delayMS--);    

    TracerInfo("======  CC_OTA_Self_Test Done!!  ======\r\n");
}

#endif
#endif

//#endif //BLE_APP_OTA

/// @} APP
