/**
 ****************************************************************************************
 *
 * @file drvi_wktm.c
 *
 * @brief HW timer interface for all SOC.
 *
 * Copyright (C) CloudChip 2017-2019
 *
 *
 ****************************************************************************************
 */
#include "project.h"
#include "drvi_wktm.h"

#if defined(WKTM_INUSE) && WKTM_INUSE
void drvi_wktmInit(void)
{
    #if defined(WKTM0_INUSE) && WKTM0_INUSE
    cc6801_wktmInit(WKTM_0);
    #endif
    
    #if defined(WKTM1_INUSE) && WKTM1_INUSE
    cc6801_wktmInit(WKTM_1);
    #endif
}
#endif

