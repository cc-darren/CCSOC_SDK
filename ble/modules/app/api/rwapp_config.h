/**
 ****************************************************************************************
 *
 * @file rwapp_config.h
 *
 * @brief Application configuration definition
 *
 * Copyright (C) RivieraWaves 2009-2016
 *
 ****************************************************************************************
 */


#ifndef _RWAPP_CONFIG_H_
#define _RWAPP_CONFIG_H_

/**
 ****************************************************************************************
 * @addtogroup app
 * @brief Application configuration definition
 *
 * @{
 ****************************************************************************************
 */


/*
 * INCLUDE FILES
 ****************************************************************************************
 */

/*
 * DEFINES
 ****************************************************************************************
 */

/******************************************************************************************/
/* -------------------------   BLE APPLICATION SETTINGS      -----------------------------*/
/******************************************************************************************/

/// Health Thermometer Application
#if defined(CFG_APP_HT)
#define BLE_APP_HT           1
#else // defined(CFG_APP_HT)
#define BLE_APP_HT           0
#endif // defined(CFG_APP_HT)

/// HID Application
#if defined(CFG_APP_HID)
#define BLE_APP_HID          1
#else // defined(CFG_APP_HID)
#define BLE_APP_HID          0
#endif // defined(CFG_APP_HID)

/// DIS Application
#if defined(CFG_APP_DIS)
#define BLE_APP_DIS          1
#else // defined(CFG_APP_DIS)
#define BLE_APP_DIS          0
#endif // defined(CFG_APP_DIS)

/// Time Application
#if defined(CFG_APP_TIME)
#define BLE_APP_TIME         1
#else // defined(CFG_APP_TIME)
#define BLE_APP_TIME         0
#endif // defined(CFG_APP_TIME)

/// Battery Service Application
/*
#if (BLE_APP_HID)
#define BLE_APP_BATT          1
#else
#define BLE_APP_BATT          0
#endif // (BLE_APP_HID)
*/
/// Security Application
#if (defined(CFG_APP_SEC) || BLE_APP_HID || defined(BLE_APP_AM0))
#define BLE_APP_SEC          1
#else // defined(CFG_APP_SEC)
#define BLE_APP_SEC          0
#endif // defined(CFG_APP_SEC)


/// OTA
#if defined(CFG_APP_OTA)
#define BLE_APP_OTA           1
#else // defined(CFG_APP_OTA)
#define BLE_APP_OTA           0
#endif // defined(CFG_APP_OTA)


/// CCPS
#if defined(CFG_APP_CCPS)
#define BLE_APP_CCPS           1
#else // defined(CFG_APP_CCPS)
#define BLE_APP_CCPS           0
#endif // defined(CFG_APP_CCPS)

/// BLEUARTS
#if defined(CFG_APP_BLEUARTS)
#define BLE_APP_BLEUARTS           1
#else // defined(CFG_APP_BLEUARTS)
#define BLE_APP_BLEUARTS           0
#endif // defined(CFG_APP_BLEUARTS)


/// @} rwapp_config

#endif /* _RWAPP_CONFIG_H_ */
