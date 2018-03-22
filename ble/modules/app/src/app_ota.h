/**
 ****************************************************************************************
 *
 * @file app_ota.h
 *
 * @brief Cloudchip OTA Application entry point
 *
 * Copyright (C) RivieraWaves 2009-2015
 *
 *
 ****************************************************************************************
 */

#ifndef APP_OTA_H_
#define APP_OTA_H_

/**
 ****************************************************************************************
 * @addtogroup APP
 * @ingroup RICOW
 *
 * @brief Cloudchip OTA Application entry point.
 *
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwip_config.h"     // SW configuration

//#if (BLE_APP_OTA)

#include <stdint.h>          // Standard Integer Definition
#include "ke_task.h"
#include "gattc_task.h"
#include "app_ccps.h"

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

extern struct app_env_tag app_env;


typedef enum		
{			
	E_CCPS_OPCODE_OTA_CMD_START 	    = 0x00,	    /* CMD: start OTA */
	E_CCPS_OPCODE_OTA_RSP_START 	    = 0x01,	    /* RSP: respond result */
	E_CCPS_OPCODE_OTA_CMD_CONFIG	    = 0x02,	    /* CMD: cfg, e.g. fw info */
	E_CCPS_OPCODE_OTA_RSP_CONFIG	    = 0x03,	    /* RSP: same as previous */
	E_CCPS_OPCODE_OTA_CMD_OFFSET	    = 0x04,	    /* CMD: chunk offset, resume */
	E_CCPS_OPCODE_OTA_RSP_OFFSET	    = 0x05,	    /* RSP: respond result */
	E_CCPS_OPCODE_OTA_CMD_CRC   	    = 0x06,	    /* CMD: CRC commands */
	E_CCPS_OPCODE_OTA_RSP_CRC   	    = 0x07,	    /* RSP: respond CRC */
    E_CCPS_OPCODE_OTA_CMD_CRC_CONFIRM   = 0x08,     /* CMD: confirm CRC */
    E_CCPS_OPCODE_OTA_CMD_STOP          = 0x09,     /* CMD: OTA ends w SUCC/FAIL */    
    E_CCPS_OPCODE_OTA_CMD_RECONNECT     = 0x0A,     /* CMD: request reconnection*/
    
}	E_CcpsOpcodeOTA;		


typedef enum
{
    E_CCPS_OTA_START_RSP_SUCCESS        = 0x00,
    E_CCPS_OTA_START_RSP_RECONNECT      = 0x01,
    E_CCPS_OTA_START_RSP_NOT_SUPPORT    = 0xFE,
    E_CCPS_OTA_START_RSP_FAIL           = 0xFF,

}   E_CcpsOTAStartResultRsp;


typedef enum
{
    E_CCPS_OTA_ENC_REQ_DISABLED    = 0x00,
    E_CCPS_OTA_ENC_REQ_ENABLED     = 0x01,

}   E_CcpsOTAConfigEncReq;


typedef enum
{
    E_CCPS_OTA_CONFIG_RSP_SUCCESS           = 0x00,
    E_CCPS_OTA_CONFIG_RSP_NOT_ACCEPTED      = 0xFF,

}   E_CcpsOTAConfigRsp;


typedef enum
{
    E_CCPS_OTA_CHUNK_OFFSET_RSP_OK              = 0x00,
    E_CCPS_OTA_CHUNK_OFFSET_RSP_NOT_ACCEPTED    = 0x01,

}   E_CcpsOTAChunkOffsetResultRsp;


typedef enum
{
    E_CCPS_OTA_CRC_REQ_CHUNK   = 0x00,
    E_CCPS_OTA_CRC_REQ_FILE    = 0x01,

}   E_CcpsOTACRCAreaReq;


typedef enum
{
    E_CCPS_OTA_CRC_CONFIRM_OK   = 0x00,
    E_CCPS_OTA_CRC_CONFIRM_FAIL = 0x01,

}   E_CcpsOTACRCCofirmReq;


typedef enum
{
    E_CCPS_OTA_STOP_OTA_SUCCESS   = 0x00,
    E_CCPS_OTA_STOP_OTA_FAIL      = 0x01,

}   E_CcpsOTAStopOTAResult;

   

#pragma push
#pragma pack(1)

typedef struct
{
     uint8_t     result;
} S_CCPS_OTA_START_RSP;


typedef struct
{
     uint32_t   signature;
     uint32_t   file_size;
     uint32_t   file_crc;
     uint32_t   prog_addr;    
     uint8_t    enc;
} S_CCPS_OTA_CONFIG_REQ;


typedef struct
{
     uint8_t    result;
     uint32_t   signuature;
     uint32_t   chunk_size;
     uint32_t   chunk_offset;   
} S_CCPS_OTA_CONFIG_RSP;


typedef struct
{
     uint32_t   chunk_offset;
} S_CCPS_OTA_CHUNK_OFFSET_CMD;  


typedef struct
{
     uint8_t   result;
} S_CCPS_OTA_CHUNK_OFFSET_RSP;  



typedef struct
{
     uint8_t   area;
  
} S_CCPS_OTA_CRC_CMD;  


typedef struct
{
     uint32_t   crc;
  
} S_CCPS_OTA_CRC_RSP;  


typedef struct
{
     uint8_t   result;
  
} S_CCPS_OTA_CRC_CONFIRM_REQ; 


typedef struct
{
     uint8_t    result;
  
} S_CCPS_OTA_STOP_OTA_CMD;  


#if 0
// Cloudchip OTA Operation format:
// For Master: 
typedef struct
{
    uint8_t     op;
    uint8_t     cmd;
} S_App_CC_OP_OTA_CMD;


typedef struct
{
    uint8_t     op;
    uint32_t    prog_addr;
    uint32_t    file_size;
    uint32_t    file_crc;
    uint32_t    signature;
    uint8_t     enc;
    uint32_t    chunk_size;
} S_App_CC_OP_FILE_INFO;


typedef struct
{
    uint8_t     op;
    uint32_t    chunk_offset;
} S_App_CC_OP_CHUNK_OFFSET;


typedef struct
{
    uint8_t     op;
    uint8_t     raw_data[256];  // for temp
} S_App_CC_OP_RAW_DATA;


// For Slave:
typedef struct
{
    uint8_t     op;
    uint32_t    prog_addr;
    uint32_t    file_size;
    uint32_t    file_crc;
    uint32_t    signature;
    uint32_t    chunk_size;
    uint32_t    chunk_offset;
    uint8_t     enc;
} S_App_CC_OP_OTA_INFO;


typedef struct
{
    uint8_t     op;
    uint8_t     result;
} S_App_CC_OP_OTA_RSP;



typedef struct
{
    uint8_t     op;
    uint32_t    crc;
} S_App_CC_OP_OTA_CRC ;
#endif


typedef struct
{
    uint32_t    prog_addr;
    uint32_t    file_size;
    uint32_t    file_crc;
    uint32_t    signature;
    uint32_t    chunk_offset;    
    uint8_t     enc;
} S_App_OTA_PeerConfig  ;



#ifdef CC_OTA_SELF_TEST

typedef enum
{   
    E_APP_OTA_ST_EVENT_MASTER_REQUEST,
    E_APP_OTA_ST_EVENT_SLAVE_REQUEST,

}   E_App_OTA_SelfTest_EventID;


typedef enum
{
    // from Master
    E_APP_OTA_ST_MODE_OTA_CMD,
    E_APP_OTA_ST_MODE_FILE_INFO,
    E_APP_OTA_ST_MODE_CHUNK_OFFSET,
    E_APP_OTA_ST_MODE_RAW_DATA,

    // from Slave
    E_APP_OTA_ST_MODE_OTA_INFO,
    E_APP_OTA_ST_MODE_OTA_RSP,
    E_APP_OTA_ST_MODE_OTA_CRC,    

}   E_App_OTA_SelfTest_Mode;

/*
// NOT used 
typedef struct
{
    uint8_t    eOTASelfTestMode;
    uint8_t    baData[300]; // for test
}   S_App_OTA_SelfTest_Req;
*/
#endif


#pragma pop // end of unligned structure

/*
 * GLOBAL VARIABLES DECLARATIONS
 ****************************************************************************************
 */



/// Table of message handlers
extern const struct ke_state_handler app_ota_table_handler;

/*
 * FUNCTIONS DECLARATION
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 *
 * Cloudchip OTA Application Functions
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Initialize Cloudchip OTA Application
 ****************************************************************************************
 */
void app_ota_init(void);

/**
 ****************************************************************************************
 * @brief Increase temp_type by 1
 ****************************************************************************************
 */
//void app_ota_notify_send(uint8_t *tx_data);
void app_ota_notify_send(uint8_t *tx_data, uint8_t length);
void app_ota_ble_disconnect(void * p_context);

void APP_OTA_MsgHandler(S_App_CC_Messages* msg);
void APP_OTA_Image_Write(const uint8_t *raw_data, uint16_t data_size);
bool APP_OTA_RebootIsReady(void);

#ifdef CC_OTA_SELF_TEST
void APP_OTA_Self_Test(void);
#endif


//#endif //(BLE_APP_OTA)

/// @} APP

#endif // APP_OTA_H_
