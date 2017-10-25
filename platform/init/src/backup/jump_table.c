
#include "jump_table.h"
#include "rwip_config.h"
#include "rf.h"
#include "nvds.h"
#include "app_uart.h"



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
    	/* 018 */ (const uint32_t *) 0,//0x00016350,
		/* 019 */ (const uint32_t *) 0,//0x00016408,
		/* 020 */ (const uint32_t *) 0,//0x00016528,
		/* 021 */ (const uint32_t *) 0,//0x00016580,
		/* 022 */ (const uint32_t *) 0,
		/* 023 */ (const uint32_t *) 0,//0x00016680,
		/* 024 */ (const uint32_t *) 0,//0x00016360,
		/* 025 */ (const uint32_t *) 0,//0x00016398,
		/* 026 */ (const uint32_t *) 0,//0x00016680,
		/* 027 */ (const uint32_t *) 0,//0x000163b8,
		/* 028 */ (const uint32_t *) 0,//0x0001677c,
		/* 029 */ (const uint32_t *) 0,//0x000167b8,
		/* 030 */ (const uint32_t *) 0,//0x000168f8,
		/* 031 */ (const uint32_t *) 0,//0x000169f4,
		/* 032 */ (const uint32_t *) 0,//0x000167f4,
		/* 033 */ (const uint32_t *) 0,//0x00016888,
		/* 034 */ (const uint32_t *) 0,//0x00016b58,
		/* 035 */ (const uint32_t *) 0,//0x00016a34,
		/* 036 */ (const uint32_t *) 0,//0x00016f90,
		/* 037 */ (const uint32_t *) 0,//0x00016c08,
		/* 038 */ (const uint32_t *) 0,//0x00016db0,
		/* 039 */ (const uint32_t *) 0,//0x00016f30,
		/* 040 */ (const uint32_t *) 0,
		/* 041 */ (const uint32_t *) 0,
		/* 042 */ (const uint32_t *) 0,
		/* 043 */ (const uint32_t *) 0,
		/* 044 */ (const uint32_t *) 0,
		/* 045 */ (const uint32_t *) 0,
		/* 046 */ (const uint32_t *) 0,

};



