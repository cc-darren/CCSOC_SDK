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

/******************************************************************************
*  Filename:
*  ---------
*  touch.h
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
#ifndef _TOUCH_H_
#define _TOUCH_H_

#include "project.h"

/******************************************************************************
Head Block of The File
******************************************************************************/
#ifndef TOUCH_SUPPORTED_FINGERS
  #define TOUCH_SUPPORTED_FINGERS 1     //default support 1 finger
#endif

/******************************************************************************
Declaration of External Variables & Functions
******************************************************************************/

/******************************************************************************
Declaration of data structure
******************************************************************************/
typedef enum
{
    F_DOWN      = 0,
    F_UP        = 1,
    F_CONTACT   = 2,
    F_NONE      = 3
}E_fState;

typedef enum
{
    //Actually, one finger gesture and two finger gesture must be separted.
    //Currently, we don't need two finger gesture
    //Also, focal touchpanel, the first touchpanel we supported, doesn't separate one/two gesture
    //Just follow focal's design now.
    G_MOVE_UP       = 0x10,
    G_MOVE_RIGHT    = 0x14,
    G_MOVE_DOWN     = 0x18,
    G_MOVE_LEFT     = 0x1C,
    G_ZOOM_IN       = 0x48,
    G_ZOMM_OUT      = 0x49,
    G_NONE          = 0x00,
    G_PALM          = 0xF0,
    G_SINGLE_TAP    = 0x20,
    G_DOUBLE_TAP    = 0x21,
    G_TAP_N_HOLD    = 0x22
}E_gesture;

typedef struct
{
    uint16_t wX;
    uint16_t wY;
    uint8_t  bID;   //first touch = 0, second touch = 1, and so on
    E_fState bState;
}T_oneFinger;

typedef struct
{
    uint8_t     bTouchNum;
    E_gesture   bGesture;
    T_oneFinger tF[TOUCH_SUPPORTED_FINGERS];
}T_fingerInfo;

/******************************************************************************
Declaration of Global Variables & Functions
******************************************************************************/

/******************************************************************************
Declaration of static Global Variables & Functions
******************************************************************************/
void touch_init(T_callback);
bool touch_getFingerDataIfReady(void);


#endif //_TOUCH_H_




