



#include "jump_table.h"
#include "rwip_config.h"
#include "rf.h"
#include "nvds.h"
#include "arch.h"
#include "rwip_config.h"

#if (BLE_APP_PRESENT)
    #include "app.h"
#endif

#if 1//(NVDS_SUPPORT)
    #include "nvds.h"
#endif

#if (BLE_PROFILES)
    #include "prf.h"
#endif

#include "app_uart.h"
#include "rwip.h"
#include "gattc.h"
#include "l2cc.h"


uint32_t rwip_heap_non_ret[RWIP_CALC_HEAP_LEN(RWIP_HEAP_NON_RET_SIZE)];//  __attribute__((section("heap_mem_area_not_ret"), zero_init));
uint32_t rwip_heap_env[RWIP_CALC_HEAP_LEN(RWIP_HEAP_ENV_SIZE)]   __attribute__((section("heap_env_area"), zero_init));
uint32_t rwip_heap_msg[RWIP_CALC_HEAP_LEN(RWIP_HEAP_MSG_SIZE)]   __attribute__((section("heap_msg_area"), zero_init));
uint32_t rwip_heap_db[RWIP_CALC_HEAP_LEN(RWIP_HEAP_DB_SIZE)]     __attribute__((section("heap_db_area"), zero_init));


#ifdef CFG_JUMP_TABLE_2

const uint32_t volatile * const jump_table_base = ((uint32_t*)JUMP_TABLE_BASE_ADDR);


// jump table from RAM
uint32_t* jump_table2_base[] __attribute__((section("jump_table2_mem_area"))) =
{    
        /* 000 */ (uint32_t *) 0,
        /* 001 */ (uint32_t *) 0,
#if BLE_APP_PRESENT
        /* 002 */ (uint32_t *) appm_init,
#else
        /* 002 */ (uint32_t *) 0,
#endif
        /* 003 */ (uint32_t *) rf_init,
        /* 004 */ (uint32_t *) 0,
        /* 005 */ (uint32_t *) 0,
        /* 006 */ (uint32_t *) rwip_eif_get,
    #if (BLE_PROFILES)
        /* 007 */ (uint32_t *) prf_init,
        /* 008 */ (uint32_t *) prf_create,
        /* 009 */ (uint32_t *) prf_add_profile,
        /* 010 */ (uint32_t *) prf_cleanup,
        /* 011 */ (uint32_t *) prf_get_id_from_task,
        /* 012 */ (uint32_t *) prf_get_task_from_id,
    #else
        /* 007 */ (uint32_t *) 0,
        /* 008 */ (uint32_t *) 0,
        /* 009 */ (uint32_t *) 0,
        /* 010 */ (uint32_t *) 0,
        /* 011 */ (uint32_t *) 0,
        /* 012 */ (uint32_t *) 0,
    #endif
        /* 013 */ (uint32_t *) 0,
        /* 014 */ (uint32_t *) 0,
        /* 015 */ (uint32_t *) 0,
        /* 016 */ (uint32_t *) 0,
        /* 017 */ (uint32_t *) 0,
        /* 018 */ (uint32_t *) 0,
        /* 019 */ (uint32_t *) 0,
        /* 020 */ (uint32_t *) 0,
        /* 021 */ (uint32_t *) 0,
        /* 022 */ (uint32_t *) 0,
        /* 023 */ (uint32_t *) 0,
        /* 024 */ (uint32_t *) 0,
        /* 025 */ (uint32_t *) 0,
        /* 026 */ (uint32_t *) 0,
        /* 027 */ (uint32_t *) 0,
        /* 028 */ (uint32_t *) 0,
        /* 029 */ (uint32_t *) 0,
        /* 030 */ (uint32_t *) 0,
        /* 031 */ (uint32_t *) 0,
        /* 032 */ (uint32_t *) 0,
        /* 033 */ (uint32_t *) 0,
        /* 034 */ (uint32_t *) 0,
        /* 035 */ (uint32_t *) 0,
        /* 036 */ (uint32_t *) 0,
        /* 037 */ (uint32_t *) 0,
        /* 038 */ (uint32_t *) 0,
        /* 039 */ (uint32_t *) 0,
        /* 040 */ (uint32_t *) 0,
        /* 041 */ (uint32_t *) 0,
        /* 042 */ (uint32_t *) 0,
        /* 043 */ (uint32_t *) 0,
        /* 044 */ (uint32_t *) 0,
        /* 045 */ (uint32_t *) 0,
        /* 046 */ (uint32_t *) 0,
        /* 047 */ (uint32_t *) 0,
        /* 048 */ (uint32_t *) 0,
        /* 049 */ (uint32_t *) 0,
        /* 050 */ (uint32_t *) 0,
        /* 051 */ (uint32_t *) 0, 
        /* 052 */ (uint32_t *) 0, 
        /* 053 */ (uint32_t *) 0, 
        /* 054 */ (uint32_t *) 0, 
        /* 055 */ (uint32_t *) 0, 
        /* 056 */ (uint32_t *) 0, 
        /* 057 */ (uint32_t *) 0, 
        /* 058 */ (uint32_t *) 0, 
        /* 059 */ (uint32_t *) 0,
        /* 060 */ (uint32_t *) 0,    
        /* 061 */ (uint32_t *) &rwip_heap_env[0],
        /* 062 */ (uint32_t *) RWIP_HEAP_ENV_SIZE,
        /* 063 */ (uint32_t *) &rwip_heap_db[0],
        /* 064 */ (uint32_t *) RWIP_HEAP_DB_SIZE,
        /* 065 */ (uint32_t *) &rwip_heap_msg[0],
        /* 066 */ (uint32_t *) RWIP_HEAP_MSG_SIZE,
        /* 067 */ (uint32_t *) &rwip_heap_non_ret[0],
        /* 068 */ (uint32_t *) RWIP_HEAP_NON_RET_SIZE,



};

#else // Disable Jump table 1 from flash 





#if (BLE_CENTRAL && BLE_CHNL_ASSESS)
/// Default Channel Assessment Timer duration (1s - Multiple of 10ms)
#define LLM_UTIL_CH_ASSES_DFLT_TIMER_DUR     (100)
/// Default Channel Reassessment Timer duration (Multiple of Channel Assessment Timer duration)
#define LLM_UTIL_CH_ASSES_DFLT_REASS_CNT     (8)
/// Default Minimal Threshold
#define LLM_UTIL_CH_ASSES_DFLT_MIN_THR       (-10)
#endif // (BLE_CENTRAL && BLE_CHNL_ASSESS)



/*
 * Deep sleep threshold. Application specific. Control if during deep sleep the system RAM will be powered off and if OTP copy will be required.
 ****************************************************************************************
*/
#if (DEEP_SLEEP)
/// Sleep Duration Value in periodic wake-up mode
#define MAX_SLEEP_DURATION_PERIODIC_WAKEUP      0x0320  // 0.5s
/// Sleep Duration Value in external wake-up mode
#define MAX_SLEEP_DURATION_EXTERNAL_WAKEUP      0x3E80  //10s
#endif //DEEP_SLEEP

#if 0
/// Kernel Message Heap
#define RWIP_HEAP_MSG_SIZE         (  BT_HEAP_MSG_SIZE_      + \
                                    BLE_HEAP_MSG_SIZE_     + \
                                    BLEHL_HEAP_MSG_SIZE_      )

/// Number of link in kernel environment
#define KE_NB_LINK_IN_HEAP_ENV   4


/// Size of Environment heap
//#define RWIP_HEAP_ENV_SIZE         10

( BT_HEAP_ENV_SIZE_         + \
                                     ( BLE_HEAP_ENV_SIZE_      + \
                                       BLEHL_HEAP_ENV_SIZE_ )    \
                                     * KE_NB_LINK_IN_HEAP_ENV )

/// Size of Attribute database heap
#define RWIP_HEAP_DB_SIZE         (  BLEHL_HEAP_DB_SIZE  )

/// Size of non retention heap - 512 bytes per ble link plus 4096 bytes for data throughput should be sufficient and should be tuned
#if (BLE_EMB_PRESENT || BLE_HOST_PRESENT)
#define RWIP_HEAP_NON_RET_SIZE    (( 512 * BLE_CONNECTION_MAX ) + 4096)
#else
#define RWIP_HEAP_NON_RET_SIZE    ( 1024 )
#endif
#endif

uint32_t volatile *const jump_table2_base = ((uint32_t*)JUMP_TABLE_2_BASE_ADDR);


void _func_dummy(void)
{
    // do nothing
}


void lld_sleep_compensate(void);
void lld_sleep_init(void);
uint32_t lld_sleep_us_2_lpcycles(uint32_t us); // rwip_us_2_lpcycles
uint32_t lld_sleep_lpcycles_2_us(uint32_t lpcycles); // rwip_sleep_lpcycles_2_us



const uint32_t* const jump_table_base[] __attribute__((section("jump_table_mem_area"))) =
{
    #if BLE_APP_PRESENT
        /* 000 */ (const uint32_t *) TASK_APP,
    #else
        /* 000 */ (const uint32_t *) TASK_AHI,
    #endif

        /* 001 */ (const uint32_t *) _func_dummy,

    #if BLE_APP_PRESENT
        /* 002 */ (const uint32_t *) appm_init,
    #else
        /* 002 */ (const uint32_t *) _func_dummy,
    #endif

        /* 003 */ (const uint32_t *) rf_init,
        /* 004 */ (const uint32_t *) _func_dummy,
        /* 005 */ (const uint32_t *) _func_dummy,
        /* 006 */ (const uint32_t *) rwip_eif_get,

    #if (BLE_PROFILES)
        /* 007 */ (const uint32_t *) prf_init,
        /* 008 */ (const uint32_t *) prf_create,
        /* 009 */ (const uint32_t *) prf_add_profile,
        /* 010 */ (const uint32_t *) prf_cleanup,
        /* 011 */ (const uint32_t *) prf_get_id_from_task,
        /* 012 */ (const uint32_t *) prf_get_task_from_id,
    #else
        /* 007 */ (const uint32_t *) _func_dummy,
        /* 008 */ (const uint32_t *) _func_dummy,
        /* 009 */ (const uint32_t *) _func_dummy,
        /* 010 */ (const uint32_t *) _func_dummy,
        /* 011 */ (const uint32_t *) _func_dummy,
        /* 012 */ (const uint32_t *) _func_dummy,
    #endif

        /* 013 */ (const uint32_t *) 0,
        /* 014 */ (const uint32_t *) app_nvds_init,
        /* 015 */ (const uint32_t *) app_nvds_get,
        /* 016 */ (const uint32_t *) app_nvds_put,
        /* 017 */ (const uint32_t *) app_nvds_del,
        /* 018 */ (const uint32_t *) 0,
        /* 019 */ (const uint32_t *) 0,
        /* 020 */ (const uint32_t *) 0,
        /* 021 */ (const uint32_t *) 0,
        /* 022 */ (const uint32_t *) 0,
        /* 023 */ (const uint32_t *) 0,
        /* 024 */ (const uint32_t *) 0,
        /* 025 */ (const uint32_t *) 0,
        /* 026 */ (const uint32_t *) 0,
        /* 027 */ (const uint32_t *) 0,
        /* 028 */ (const uint32_t *) 0,
        /* 029 */ (const uint32_t *) 0,
        /* 030 */ (const uint32_t *) 0,
        /* 031 */ (const uint32_t *) 0,
        /* 032 */ (const uint32_t *) 0,
        /* 033 */ (const uint32_t *) 0,
        /* 034 */ (const uint32_t *) 0,
        /* 035 */ (const uint32_t *) 0,
        /* 036 */ (const uint32_t *) 0,
        /* 037 */ (const uint32_t *) 0,
        /* 038 */ (const uint32_t *) 0,
        /* 039 */ (const uint32_t *) 0,
        /* 040 */ (const uint32_t *) 0,
        /* 041 */ (const uint32_t *) 0,
        /* 042 */ (const uint32_t *) 0,
        /* 043 */ (const uint32_t *) 0,
        /* 044 */ (const uint32_t *) 0,
        /* 045 */ (const uint32_t *) 0,
        /* 046 */ (const uint32_t *) 0,
        /* 047 */ (const uint32_t *) 0,
#if (BLE_CENTRAL && BLE_CHNL_ASSESS)        
        /* 048 */ (const uint32_t *) LLM_UTIL_CH_ASSES_DFLT_TIMER_DUR,
        /* 049 */ (const uint32_t *) LLM_UTIL_CH_ASSES_DFLT_REASS_CNT,
        /* 050 */ (const uint32_t *) LLM_UTIL_CH_ASSES_DFLT_MIN_THR,
#else
        /* 048 */ (const uint32_t *) 0,
        /* 049 */ (const uint32_t *) 0,
        /* 050 */ (const uint32_t *) 0,
#endif
        /* 051 */ (const uint32_t *) 0, // lld_evt_init_func
        /* 052 */ (const uint32_t *) 0, // lld_init_func
        /* 053 */ (const uint32_t *) 0, // lld_test_stop_func
        /* 054 */ (const uint32_t *) 0, // lld_test_mode_tx_func
        /* 055 */ (const uint32_t *) 0, // lld_test_mode_rx_func
        /* 056 */ (const uint32_t *) 0, // llm_encryption_done_func
        /* 057 */ (const uint32_t *) 0, // ke_task_init_func
        /* 058 */ (const uint32_t *) 0, // ke_timer_init_func
#if (DEEP_SLEEP)        
        /* 059 */ (const uint32_t *) MAX_SLEEP_DURATION_PERIODIC_WAKEUP,
        /* 060 */ (const uint32_t *) MAX_SLEEP_DURATION_EXTERNAL_WAKEUP,
#else
        /* 059 */ (const uint32_t *) 0,
        /* 060 */ (const uint32_t *) 0,
#endif        
        /* 061 */ (const uint32_t *) &rwip_heap_env[0],
        /* 062 */ (const uint32_t *) RWIP_HEAP_ENV_SIZE,
        /* 063 */ (const uint32_t *) &rwip_heap_db[0],
        /* 064 */ (const uint32_t *) RWIP_HEAP_DB_SIZE,
        /* 065 */ (const uint32_t *) &rwip_heap_msg[0],
        /* 066 */ (const uint32_t *) RWIP_HEAP_MSG_SIZE,
        /* 067 */ (const uint32_t *) &rwip_heap_non_ret[0],
        /* 068 */ (const uint32_t *) RWIP_HEAP_NON_RET_SIZE,
};

#endif


