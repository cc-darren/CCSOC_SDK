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
*  touch_ft6x36.h
*
*  Project:
*  --------
*  CC6801
*
*  Description:
*  ------------
*  This is touch driver of focal ft6x36 series
*
*  Author:
*  -------
*  PPP   (cc-pattaya)
*
*===========================================================================
*
******************************************************************************/
#ifndef _TOUCH_FT6X36_H_
#define _TOUCH_FT6X36_H_

#include <stdint.h>
#include "global.h"

/******************************************************************************
Head Block of The File
******************************************************************************/
#define TOUCH_ModuleInit(x)             ft6x36_init(x)
#define TOUCH_FillFingerInfo()          ft6x36_fillFingerInfo()
#define TOUCH_FingerDataReadDone()      ft6x36_FingerDataReadDone()
#define TOUCH_isFingerDataReady()       ft6x36_isFingerDataReady()

/******************************************************************************
Declaration of External Variables & Functions
******************************************************************************/

/******************************************************************************
Declaration of data structure
******************************************************************************/


/******************************************************************************
Declaration of Global Variables & Functions
******************************************************************************/

/******************************************************************************
Declaration of static Global Variables & Functions
******************************************************************************/
void ft6x36_init(T_callback);
void ft6x36_fillFingerInfo(void);
void ft6x36_FingerDataReadDone(void);
bool ft6x36_isFingerDataReady(void);

#endif //_TOUCH_FT6X36_H_




