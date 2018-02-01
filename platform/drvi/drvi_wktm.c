/* Copyright (c) 2018 Cloudchip, Inc. All Rights Reserved.
 *
 * The information contained herein is property of Cloudchip, Inc.
 * Terms and conditions of usage are described in detail in CLOUDCHIP
 * STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information.
 * NO WARRANTY of ANY KIND is provided. This heading must NOT be removed 
 * from the file.
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

