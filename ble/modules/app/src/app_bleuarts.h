/**
 ****************************************************************************************
 *
 * @file app_bleuarts.h
 *
 * @brief CloudChip Proprietary Services Application entry point
 *
 * Copyright (C) RivieraWaves 2009-2015
 *
 *
 ****************************************************************************************
 */

#ifndef APP_BLEUARTS_H_
#define APP_BLEUARTS_H_

/**
 ****************************************************************************************
 * @addtogroup APP
 * @ingroup RICOW
 *
 * @brief CloudChip Proprietary Services Application entry point.
 *
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "rwip_config.h"     // SW configuration

#if (BLE_APP_BLEUARTS)

#include <stdint.h>          // Standard Integer Definition
#include "ke_task.h"
#include "gattc_task.h"

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

extern struct app_env_tag app_env;

/// CloudChip Proprietary Services application environment structure
/*
struct app_bleuarts_env_tag
{

};
*/

#define SIZE_OF_CC_MSG_LEN       (sizeof(((S_App_CC_Messages *)0)->len))
#define SIZE_OF_CC_MSG_TYPE      (sizeof(((S_App_CC_Messages *)0)->type))
#define SIZE_OF_CC_MSG_OP        (sizeof(((S_App_CC_Messages *)0)->op))
#define SIZE_OF_CC_MSG_HDR       (SIZE_OF_CC_MSG_TYPE + SIZE_OF_CC_MSG_OP)


#pragma push
#pragma pack(1)

typedef struct 
{
    uint16_t len;
    uint8_t  type;
    uint8_t  op;
    uint8_t  data[300]; 
} S_App_CC_Messages;

#pragma pop



typedef enum
{	
	E_BLEUARTS_FTYPE_SYSTEM 	= 0x00,	    /* System CMD/RSP */
	E_BLEUARTS_FTYPE_OTA    	= 0x10,	    /* OTA CMD/RSP */
	E_BLEUARTS_FTYPE_PED    	= 0x20,	    /* Pedometer CMD/RSP */
	E_BLEUARTS_FTYPE_HR     	= 0x21,	    /* Heart Rate CMD/RSP */
	E_BLEUARTS_FTYPE_SLEEP  	= 0x22,	    /* Sleep Monitor CMD/RSP */
	E_BLEUARTS_FTYPE_LIFTARM	= 0x23,	    /* Lift Arm CMD/RSP */
	E_BLEUARTS_FTYPE_SWIM   	= 0x24,	    /* Swim CMD/RSP */
}	E_CcpsFunctionType;


typedef enum		
{
    E_BLEUARTS_OPCODE_SYS_CMD_READ 	= 0x00,	    /* CMD: read specific data */
    E_BLEUARTS_OPCODE_SYS_RSP_READ 	= 0x01,	    /* RSP: respond data */
    E_BLEUARTS_OPCODE_SYS_CMD_WRITE	= 0x02,	    /* CMD: write settings */
    E_BLEUARTS_OPCODE_SYS_RSP_WRITE	= 0x03,	    /* RSP: respond results */
    E_BLEUARTS_OPCODE_SYS_NTF_DATA 	= 0x04,	    /* NTF: report live data */    
}   E_CcpsOpcodeSystem;

/*
 * GLOBAL VARIABLES DECLARATIONS
 ****************************************************************************************
 */

/// CloudChip Proprietary Service Application environment
//extern struct app_bleuarts_env_tag app_bleuarts_env;

/// Table of message handlers
extern const struct ke_state_handler app_bleuarts_table_handler;

/*
 * FUNCTIONS DECLARATION
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 *
 * CloudChip Proprietary Services Functions
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Initialize BLEUARTS Application
 ****************************************************************************************
 */
void app_bleuarts_init(void);

/**
 ****************************************************************************************
 * @brief Add a BLEUARTS instance in the DB
 ****************************************************************************************
 */
void app_bleuarts_add_bleuartss(void);

/**
 ****************************************************************************************
 * @brief Enable the CloudChip Proprietary Services profile
 ****************************************************************************************
 */
void app_bleuarts_enable_prf(uint8_t);



/**
 ****************************************************************************************
 * @brief Notify user defined data
 *
 * @param[in] tx_data    Pointer to the data that would be sent by notifiction packets
 * @param[in] length     Length of the data
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
void app_bleuarts_notify_send(uint8_t *tx_data, uint8_t length);

/**
 ****************************************************************************************
 * @brief Indicate user defined data
 *
 * @param[in] tx_data    Pointer to the data that would be sent by indication packets
 * @param[in] length     Length of the data
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
void app_bleuarts_indicate_send(uint8_t *tx_data, uint8_t length);


/**
 ****************************************************************************************
 * @brief Read Response via "Report characteristic"
 *
 * @param[in] tx_data    Pointer to the data that would be sent by read response packets
 * @param[in] length     Length of the data
 * @return If the message was consumed or not.
 ****************************************************************************************
 */
void bleuarts_packet_send_reply_handler(uint8_t *tx_data, uint8_t *length);



#endif //(BLE_APP_BLEUARTS)

/// @} APP

#endif // APP_BLEUARTS_H_

