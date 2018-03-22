/**
 ****************************************************************************************
 *
 * @file ccps.h
 *
 * @brief Header file - CloudChip Proprietary Services.
 *
 * Copyright (C) CloudChip 2018
 *
 *
 ****************************************************************************************
 */

#ifndef CCPS_H_
#define CCPS_H_

/**
 ****************************************************************************************
 * @addtogroup CCPS  Profile 
 * @ingroup CCPS
 * @brief CCPS Profile 
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
#if (BLE_CCPS_SERVER)
#include <stdint.h>
#include <stdbool.h>
#include "prf_types.h"
#include "prf_utils.h"

#include "gap.h"

/*
 * MACROS
 ****************************************************************************************
 */
#define CCPS_IS_FEATURE_SUPPORTED(feat, bit_mask) (((feat & bit_mask) == bit_mask))

#define CCPS_HANDLE(idx) (ccps_att_hdl_get(ccps_env, (idx)))

#define CCPS_IDX(hdl)    (ccps_att_idx_get(ccps_env, (hdl)))
/*
 * DEFINES
 ****************************************************************************************
 */
///Maximum number of CCPS task instances
#define CCPS_IDX_MAX    (1)

#ifdef CFG_DLE_EN 
#define CCPS_REPORT_MAX_LEN              (244)
#else
#define CCPS_REPORT_MAX_LEN              (20)
#endif

#define CCPS_IND_NTF_CFG_MAX_LEN          (2)



//#define CCPS_REPORT_MASK               (0x003F) 

#define CCPS_REPORT_ATT_NB             (CCPS_IDX_NB)     


/// Possible states of the CCPS task
enum ccps_state
{
    /// Idle state
    CCPS_IDLE,
    /// Busy state
    CCPS_BUSY,

    /// Number of defined states.
    CCPS_STATE_MAX
};

///Attributes database elements

enum otas_att_db_list
{
    CCPS_IDX_SVC,
    CCPS_IDX_REPORT_CHAR,
    CCPS_IDX_REPORT_VAL,
    CCPS_IDX_REPORT_NTF_CFG,

    CCPS_IDX_IMAGE_CHAR,
    CCPS_IDX_IMAGE_VAL,

    CCPS_IDX_NB,
};

/// ongoing operation information
struct ccps_op
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


///CCPS Profile Environment Variable
struct ccps_env_tag
{
    /// profile environment
    prf_env_t prf_env;
    /// On-going operation
    struct ccps_op * operation;
    /// Service Start Handle
    uint16_t shdl;
    /// Database configuration
    uint16_t features;

    /// Notification and indication configuration of peer devices.
    uint8_t ntf_ind_cfg[BLE_CONNECTION_MAX];
    /// State of different task instances
    ke_state_t state[CCPS_IDX_MAX];
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
uint8_t ccps_get_valid_rge_offset(uint16_t features);

/**
 ****************************************************************************************
 * @brief Retrieve CCPS service profile interface
 *
 * @return CCPS service profile interface
 ****************************************************************************************
 */
const struct prf_task_cbs* ccps_prf_itf_get(void);


/**
 ****************************************************************************************
 * @brief  This function fully manage notification and indication of health thermometer
 *         to peer(s) device(s) according to on-going operation requested by application:
 *            - Modification of Intermediate Temperature
 *            - Indicate to a known device that Temperature Measured has change
 *            - Indicate to a known device that Measure Interval has change
 ****************************************************************************************
 */
void ccps_exe_operation(void);



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
uint8_t ccps_update_ntf_ind_cfg(uint8_t conidx, uint8_t cfg, uint16_t valid_val, uint16_t value);


/**
 ****************************************************************************************
 * @brief Retrieve attribute handle from attribute index
 *
 * @param[in] ccps_env   Environment variable
 * @param[in] att_idx    Attribute Index
 *
 * @return attribute Handle
 ****************************************************************************************
 */
uint16_t ccps_att_hdl_get(struct ccps_env_tag* ccps_env, uint8_t att_idx);

/**
 ****************************************************************************************
 * @brief Retrieve attribute index from attribute handle
 *
 * @param[in] ccps_env   Environment variable
 * @param[in] handle     Attribute Handle
 *
 * @return attribute Index
 ****************************************************************************************
 */
uint8_t ccps_att_idx_get(struct ccps_env_tag* ccps_env, uint16_t handle);

/*
 * TASK DESCRIPTOR DECLARATIONS
 ****************************************************************************************
 */
extern const struct ke_state_handler ccps_default_handler;



#endif //BLE_HT_THERMOM

/// @} CCPS

#endif // CCPS_H_
