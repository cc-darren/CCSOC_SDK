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
*  project.h
*
*  Project:
*  --------
*  all project
*
*  Description:
*  ------------
*  include defined project header file
*
*  Author:
*  -------
*  PPP   (CC-pattaya)
*
*===========================================================================
*
******************************************************************************/

#ifndef _PROJECT_H_
#define _PROJECT_H_

/******************************************************************************
Head Block of The File
******************************************************************************/
// Sec 0: Comment block of the file

// Sec 1: Include File
#include "global.h"
#include "asic_supported.h"
#include "module_supported.h"

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous

#if defined(FPGA) && FPGA
    #include "config_FPGA.h"
#elif defined(EVB) && EVB
    #include "config_EVB.h"
#elif defined(EVB605A) && EVB605A
    #include "config_EVB605A.h"
#elif defined(EVB605A_IOdown) && EVB605A_IOdown
    #include "config_EVB605A_IOdown.h"
#elif defined(EVB607A) && EVB607A
    #include "config_EVB607A.h"
#elif defined(EVB_QFN60L) && EVB_QFN60L
    #include "config_EVB_QFN60L.h"
#elif defined(DVK) && DVK
    #include "config_DVK.h"
#elif defined(DVK403B) && DVK403B
    #include "config_DVK403B.h"
#elif defined(DVK_DC404A) && DVK_DC404A
    #include "config_DVK403B_DC404A.h"
#elif defined(DVK_DC405A) && DVK_DC405A
    #include "config_DVK403B_DC405A.h"
#elif defined(ASIC) && ASIC
    #include "config_ASIC.h"
#else
    #error "There is no defined project. Please select one project!!"
#endif

#include "option_check.h"

/******************************************************************************
Declaration of External Variables & Functions
******************************************************************************/
// Sec 3: declaration of external variable

// Sec 4: declaration of external function prototype

/******************************************************************************
Declaration of data structure
******************************************************************************/
// Sec 5: structure, uniou, enum, linked list

/******************************************************************************
Declaration of Global Variables & Functions
******************************************************************************/
// Sec 6: declaration of global variable

// Sec 7: declaration of global function prototype

/******************************************************************************
Declaration of static Global Variables & Functions
******************************************************************************/
// Sec 8: declaration of static global variable

// Sec 9: declaration of static function prototype

/******************************************************************************
// Sec 10: C Functions
******************************************************************************/

#endif // _PROJECT_H_
