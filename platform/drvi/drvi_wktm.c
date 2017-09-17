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
#include "drvi_wktm.h"

void drvi_wktmInit(void)
{
    #if defined(_WKTM0_INUSE_) && _WKTM0_INUSE_
    cc6801_wktmInit(WKTM_0);
    #endif
    
    #if defined(_WKTM1_INUSE_) && _WKTM1_INUSE_
    cc6801_wktmInit(WKTM_1);
    #endif
}

