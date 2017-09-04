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
*  project_FPGA.h
*
*  Project:
*  --------
*  FPGA
*
*  Description:
*  ------------
*  FW configuration for FPGA
*
*  Author:
*  -------
*  PPP   (CC-pattaya)
*
*===========================================================================
*
******************************************************************************/

#ifndef _PROJECT_FPGA_H_
#define _PROJECT_FPGA_H_

/******************************************************************************
Head Block of The File
******************************************************************************/
// Sec 0: Comment block of the file

// Sec 1: Include File

// Sec 2: Constant Definitions, Imported Symbols, miscellaneous


#define XTAL_MHZ                48      //uint: MHz
#define SYSTEM_CLOCK_MHZ        32      //unit: MHz


//#define DHRY_TEST               1
#define MSC_CLOCK               1
#define CLK_TCK                 32000		//HW timer clock is 32KHz for Dhrystone measurement
#define Number_Of_Runs          1000000


/**************************************************
*
*   Choose ASIC
*   (pick one ASIC from asic_supported.h)
*
***************************************************/
#define ASIC                    CC6801

//find and include project options from the asic you chose
#include "asic_options.h"


/************Add sub-options of this ASIC****************/

//define SPI clock
//define I2C clock
//define UART baudrate, parity, stop bits, flow control ON/OFF
//IO pin mux
//Timer 32K or high clock
//WDT timeout time
//RTC default date/time
//PWM duty cycle



/**************************************************
*
*   Choose ACC sensor and config ACC interface
*   (pick one ACC sensor from sensor_supported.h)
*
***************************************************/
#define SENSOR_ACC              ACC_ST_LSM303C
#define ACC_IF                  SPI0

/**************************************************
*
*   Choose MAG sensor and config MAG interface
*   (pick one MAG sensor from sensor_supported.h)
*
***************************************************/
#define SENSOR_MAG              MAG_ST_LSM303C
#define MAG_IF                  SPI1

/**************************************************
*
*   Choose GYRO sensor and config GYRO interface
*   (pick one GYRO sensor from sensor_supported.h)
*
***************************************************/
#define SENSOR_GYR              GYR_ST_LSM6DSL
#define GYR_IF                  I2C0

/**************************************************
*
*   Choose OLED model and config OLED interface
*
***************************************************/
#define OLED_IF                 SPI2




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

#endif // _PROJECT_FPGA_H_
