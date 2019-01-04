/* Copyright (c) 2019 Cloudchip, Inc. All Rights Reserved.
 *
 * The information contained herein is property of Cloudchip, Inc.
 * Terms and conditions of usage are described in detail in CLOUDCHIP
 * STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information.
 * NO WARRANTY of ANY KIND is provided. This heading must NOT be removed
 * from the file.
 */

/******************************************************************************
*  Filename:
*  ---------
*  touch.c
*
*  Project:
*  --------
*  CC6801
*
*  Description:
*  ------------
*  This is touch service for CC FW platform. It's a interface between touch driver and MMI.
*
*  Author:
*  -------
*  PPP   (cc-pattaya)
*
*===========================================================================
*
******************************************************************************/

/******************************************************************************
Head Block of The File
******************************************************************************/
#include <stdint.h>
#include <string.h>
#include "touch.h"


/******************************************************************************
Declaration of External Variables & Functions
******************************************************************************/

/******************************************************************************
Declaration of data structure
******************************************************************************/


/******************************************************************************
Declaration of Global Variables & Functions
******************************************************************************/
__align(4) T_fingerInfo fg;

/******************************************************************************
Declaration of static Global Variables & Functions
******************************************************************************/
void touch_init(T_callback fpGpioIsrCallback)
{
    TOUCH_ModuleInit(fpGpioIsrCallback);
    memset(&fg, 0, sizeof(fg));
}

bool touch_getFingerDataIfReady(void)
{
    if(TOUCH_isFingerDataReady())
    {
        TOUCH_FillFingerInfo();
        TOUCH_FingerDataReadDone();
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}



