/**
 ****************************************************************************************
 *
 * @file bleuarts.h
 *
 * @brief Header file - CloudChip Proprietary Services.
 *
 * Copyright (C) CloudChip 2018
 *
 *
 ****************************************************************************************
 */

#ifndef BLEUARTS_H_
#define BLEUARTS_H_

/**
 ****************************************************************************************
 * @addtogroup BLEUARTS  Profile 
 * @ingroup BLEUARTS
 * @brief BLEUARTS Profile 
 *
 *
 *
 * @{
 ****************************************************************************************
 */


/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "rwip_config.h"
#include "project.h"
#if (BLE_BLEUARTS_SERVER)
#include <stdint.h>
#include <stdbool.h>
#include "prf_types.h"
#include "prf_utils.h"

#include "gap.h"

/*
 * MACROS
 ****************************************************************************************
 */
#define BLEUARTS_IS_FEATURE_SUPPORTED(feat, bit_mask) (((feat & bit_mask) == bit_mask))

#define BLEUARTS_HANDLE(idx) (bleuarts_att_hdl_get(bleuarts_env, (idx)))

#define BLEUARTS_IDX(hdl)    (bleuarts_att_idx_get(bleuarts_env, (hdl)))

#define BLEUARTS_GACK(bOpcode)    (0x80 | bOpcode)    /* RSP: generic ACK */

/*
 * DEFINES
 ****************************************************************************************
 */
///Maximum number of BLEUARTS task instances
#define BLEUARTS_IDX_MAX    (1)

#ifdef CFG_DLE_EN 
#define BLEUARTS_REPORT_MAX_LEN              (244)
#else
#define BLEUARTS_REPORT_MAX_LEN              (20)
#endif

#define BLEUARTS_IND_NTF_CFG_MAX_LEN          (2)



//#define BLEUARTS_REPORT_MASK               (0x003F) 

#define BLEUARTS_REPORT_ATT_NB             (BLEUARTS_IDX_NB)     


#pragma push
#pragma pack(1)

typedef struct 
{
    uint16_t    wLength;
    uint8_t     bType;
    uint8_t     bOpCode;
}   S_CcpsGenericHeader;

#pragma pop

#define BLEUARTS_PKT_HEADER_LENGTH_SIZE    ( sizeof(((S_CcpsGenericHeader *) 0)->wLength) )
#define BLEUARTS_PKT_HEADER_TYPE_SIZE      ( sizeof(((S_CcpsGenericHeader *) 0)->bType  ) )
#define BLEUARTS_PKT_HEADER_OPCODE_SIZE    ( sizeof(((S_CcpsGenericHeader *) 0)->bOpCode) )

#define BLEUARTS_PKT_HEADER_LENGTH_POSITION    (0)
#define BLEUARTS_PKT_HEADER_TYPE_POSITION      (BLEUARTS_PKT_HEADER_LENGTH_POSITION + BLEUARTS_PKT_HEADER_LENGTH_SIZE)
#define BLEUARTS_PKT_HEADER_OPCODE_POSITION    (BLEUARTS_PKT_HEADER_TYPE_POSITION   + BLEUARTS_PKT_HEADER_TYPE_SIZE  )


typedef struct
{
    uint16_t    wMtuOfPeer;    // max bytes per packet from peer
    uint16_t    wMtuOfSelf;    // max bytes per packet to send out
}   S_CcpsPktCB;

typedef struct
{
    uint16_t    wPktTotalSize;
    uint16_t    wWriteIndex;
    uint8_t    *pbData;
}   S_CcpsRcvPktCB;


/// Possible states of the BLEUARTS task
enum bleuarts_state
{
    /// Idle state
    BLEUARTS_IDLE,
    /// Busy state
    BLEUARTS_BUSY,

    /// Number of defined states.
    BLEUARTS_STATE_MAX
};

///Attributes database elements

enum otas_att_db_list
{
    BLEUARTS_IDX_SVC,
	
	//---RX---
    BLEUARTS_IDX_RX_CHAR,
    BLEUARTS_IDX_RX_VAL,

	//---TX---
	BLEUARTS_IDX_TX_CHAR,
	BLEUARTS_IDX_TX_VAL,
	BLEUARTS_IDX_TX_NTF_CFG,

    BLEUARTS_IDX_NB,
};

/// ongoing operation information
struct bleuarts_op
{
     /// Operation
     uint8_t op;
     /// Cursor on connection
     uint8_t cursor;
     /// Handle of the attribute to indicate/notify
     uint16_t handle;
     /// Task that request the operation that should receive completed message response
     uint16_t dest_id;
     /// Packed notification/indication data size
     uint8_t length;
     /// used to know on which device interval update has been requested, and to prevent
     /// indication to be triggered on this connection index
     uint8_t conidx;
     /// Packed notification/indication data
     uint8_t data[__ARRAY_EMPTY];
};


///BLEUARTS Profile Environment Variable
struct bleuarts_env_tag
{
    /// profile environment
    prf_env_t prf_env;
    /// On-going operation
    struct bleuarts_op * operation;
    /// Service Start Handle
    uint16_t shdl;
    /// Database configuration
    uint16_t features;

    /// Notification and indication configuration of peer devices.
    uint8_t ntf_ind_cfg[BLE_CONNECTION_MAX];
    /// State of different task instances
    ke_state_t state[BLEUARTS_IDX_MAX];

    S_CcpsRcvPktCB   tRcvReportPktCB;
};

/*
 * GLOBAL VARIABLE DECLARATIONS
 ****************************************************************************************
 */

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Compute the offset of the valid range descriptor.
 * The Measurement Interval characteristic has two optional descriptors. In the database,
 * the Client Characteristic Configuration descriptor will always be placed just after the
 * characteristic value. Thus, this function checks if the CCC descriptor has been added.
 * @return     0 if Measurement Interval Char. is not writable (no Valid Range descriptor)
 *             1 if Measurement Interval Char. doesn't support indications (no CCC descriptor)
 *             2 otherwise
 ****************************************************************************************
 */
uint8_t bleuarts_get_valid_rge_offset(uint16_t features);

/**
 ****************************************************************************************
 * @brief Retrieve BLEUARTS service profile interface
 *
 * @return BLEUARTS service profile interface
 ****************************************************************************************
 */
const struct prf_task_cbs* bleuarts_prf_itf_get(void);


/**
 ****************************************************************************************
 * @brief  This function fully manage notification and indication of health thermometer
 *         to peer(s) device(s) according to on-going operation requested by application:
 *            - Modification of Intermediate Temperature
 *            - Indicate to a known device that Temperature Measured has change
 *            - Indicate to a known device that Measure Interval has change
 ****************************************************************************************
 */
void bleuarts_exe_operation(void);



/**
 ****************************************************************************************
 * @brief Update Notification, Indication configuration
 *
 * @param[in] conidx    Connection index
 * @param[in] cfg       Indication configuration flag
 * @param[in] valid_val Valid value if NTF/IND enable
 * @param[in] value     Value set by peer device.
 *
 * @return status of configuration update
 ****************************************************************************************
 */
uint8_t bleuarts_update_ntf_ind_cfg(uint8_t conidx, uint8_t cfg, uint16_t valid_val, uint16_t value);


/**
 ****************************************************************************************
 * @brief Retrieve attribute handle from attribute index
 *
 * @param[in] bleuarts_env   Environment variable
 * @param[in] att_idx    Attribute Index
 *
 * @return attribute Handle
 ****************************************************************************************
 */
uint16_t bleuarts_att_hdl_get(struct bleuarts_env_tag* bleuarts_env, uint8_t att_idx);

/**
 ****************************************************************************************
 * @brief Retrieve attribute index from attribute handle
 *
 * @param[in] bleuarts_env   Environment variable
 * @param[in] handle     Attribute Handle
 *
 * @return attribute Index
 ****************************************************************************************
 */
uint8_t bleuarts_att_idx_get(struct bleuarts_env_tag* bleuarts_env, uint16_t handle);

/*
 * TASK DESCRIPTOR DECLARATIONS
 ****************************************************************************************
 */
extern const struct ke_state_handler bleuarts_default_handler;



#endif //BLE_BLEUARTS_SERVER

/// @} BLEUARTS

#endif // BLEUARTS_H_
