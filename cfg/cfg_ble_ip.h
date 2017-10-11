#ifndef _CC_SDK_BLE_IP_CONFIG_
    #define _CC_SDK_BLE_IP_CONFIG_

// <<< Use Configuration Wizard in Context Menu >>>\n

//BLE is always supported
#define CFG_BLE    1

// <h> BLE IP ARCH CONFIGURATION
#define CFG_BLE_APP

//==========================================================
// <q> EMB   - BLE Embedded Sub-block
#ifndef CFG_EMB
    #define CFG_EMB    1
#endif

// <q> HOST - BLE HOST Sub-block
#ifndef CFG_HOST
    #define CFG_HOST    1
#endif

//#define CFG_ALLROLES
//#define CFG_CENTRAL
#define CFG_PERIPHERAL
//#define CFG_BROADCASTER
//#define CFG_OBSERVER

#define CFG_CON    8 
#define CFG_SLEEP

#define CFG_CHNL_ASSESS

#define CFG_AHITL
#define CFG_HCITL
//#define CFG_NVDS
#define CFG_HW_AUDIO

#define CFG_SEC_CON 
#define CFG_APP_SEC

#define CFG_ATTC
#define CFG_ATTS

#define CFG_RF_ATLAS

#define CFG_PRF
#define CFG_NB_PRF    5
#define CFG_PRF_HTPT
#define CFG_PRF_BASS
#define CFG_PRF_DISS
//#define CFG_PRF_CSCPS

#define CFG_APP
#define CFG_APP_HT
#define BLE_APP_BATT  1
#define CFG_APP_DIS    1
//#define CFG_APP_CSCS    1

#define CFG_EXT_DB
//#define CFG_DBG




#define flash_read(a,b,c,d,e)
#define flash_write(a,b,c,d,e)
#define flash_erase(a,b,c,d)
#define flash_identify(a,b)


// <<< end of configuration section >>>
#endif //_CC_SDK_BLE_IP_CONFIG_
