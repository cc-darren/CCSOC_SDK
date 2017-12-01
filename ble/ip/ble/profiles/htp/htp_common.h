/**
 ****************************************************************************************
 *
 * @file htp_common.h
 *
 * @brief Header File - Health Thermometer Profile common types.
 *
 * Copyright (C) RivieraWaves 2009-2016
 *
 *
 ****************************************************************************************
 */


#ifndef _HTP_COMMON_H_
#define _HTP_COMMON_H_

/**
 ****************************************************************************************
 * @addtogroup HTP Health Thermometer Profile
 * @ingroup PROFILE
 * @brief Health Thermometer Profile
 *****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include "prf_types.h"

/*
 * DEFINES
 ****************************************************************************************
 */

/// Out of range error code.
#define HTP_OUT_OF_RANGE_ERR_CODE        (0x80)

///Temperature Measurement Flags field bit values
enum htp_temp_flag
{
    /// Temperature Units Flag - Celsius
    HTP_FLAG_CELSIUS              = 0x00,
    /// Temperature Units Flag - Fahrenheit
    HTP_FLAG_FAHRENHEIT,
    /// Time Stamp Flag
    HTP_FLAG_TIME,
    /// Temperature Type Flag
    HTP_FLAG_TYPE                 = 0x04,
};

/// Temperature Type Description
enum htp_temp_type
{
    /// Armpit
    HTP_TYPE_ARMPIT                   = 0x01,
    /// Body (general)
    HTP_TYPE_BODY                     = 0x02,
    /// Ear (usually ear lobe)
    HTP_TYPE_EAR                      = 0x03,
    /// Finger
    HTP_TYPE_FINGER                   = 0x04,
    /// Gastro-intestinal Tract
    HTP_TYPE_GASTRO_INTESTINAL_TRACT  = 0x05,
    /// Mouth
    HTP_TYPE_MOUTH                    = 0x06,
    /// Rectum
    HTP_TYPE_RECTUM                   = 0x07,
    /// Toe
    HTP_TYPE_TOE                      = 0x08,
    /// Tympanum (ear drum)
    HTP_TYPE_TYMPANUM                 = 0x09,
};

/// Temprature type: intermdiary/stable
enum htp_temp_info
{
    /// intermediate temperature measurement
    HTP_TEMP_INTERM         = (0x00),
    /// stable temperature measurement
    HTP_TEMP_STABLE         = (0x01),
};
        /*
 * STRUCTURES
 ****************************************************************************************
 */

///Temperature Measurement Value Structure
struct htp_temp_meas
{
    /// Temperature value
    uint32_t temp;
    /// Time stamp
    struct prf_date_time time_stamp;
    /// Flag (@see enum htp_temp_flag)
    uint8_t  flags;
    /// Type (@see enum htp_temp_type)
    uint8_t  type;
};


///Step Measurement Value Structure
#pragma pack(push) 
#pragma pack(1)
struct htp_period_meas
{
/*    
    char        eHeader;
    uint8_t     eArray[19];
    char        elen;
*/    
    
    uint8_t vid;
    uint8_t type;
    uint8_t update_step_flag;
    uint32_t step;
    uint8_t type2;
    uint8_t update_hr_flag;
    uint16_t hr;
    uint32_t calorie;

};

struct htp_history_meas
{
    uint8_t vid;
    uint8_t type;
    uint16_t total;
    uint16_t rec_index;
    uint8_t eArray[14];
};


struct htp_swim_meas
{
    uint8_t     vid;
    uint8_t     type;
    uint8_t     is_Swim_En;
    uint8_t     style_type;
    uint32_t    dwSwimCnt;  
    uint32_t    dwSwimLap;
    uint32_t    dwTimestamp;
};

#pragma pack(pop)

/// @} htp_common

#endif /* _HTP_COMMON_H_ */
