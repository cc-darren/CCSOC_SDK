/******************************************************************************
*  Copyright 2017, Cloudchip, Inc.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Cloudchip, Inc. (C) 2017
******************************************************************************/

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
    #if defined(BOOTLOADER) && BOOTLOADER
        #include "project_bootloader_FPGA.h"
    #elif defined(VENUS_WRISTBAND) && VENUS_WRISTBAND
        #include "project_FPGA_Venus.h"
    #else
        #include "project_FPGA.h"
    #endif
#elif defined(ASIC) && ASIC
    #include "project_ASIC.h"
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
