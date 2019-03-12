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
*
*
*  Project:
*  --------
*  CC6801
*
*  Description:
*  ------------
*  the description
*
*  Author:
*  -------
*  Blake   (cc-blake)
*
*===========================================================================
*
******************************************************************************/

/******************************************************************************
Head Block of The File
******************************************************************************/
#include "test.h"
#include "drvi_adc.h"
#include "cc6801_reg.h"
#include "drvi_clock.h"
#include "tracer.h"

#if (TEST_ADC)


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

void test_ADC(void)
{
    uint32_t volt;

    drvi_AdcInit();
    
    while(1)
    {
        drvi_AdcEnable();
        drvi_ClockDelayMs(10);
        drvi_AdcVbatt(&volt);
        //cc6801_AdcDisable();
        TracerInfo("Vbat = %12d\r\n", volt);
        
        drvi_ClockDelayMs(10);
    }
    
    
/*    
    while(0)
    {
        rd(SCU_PLLLOCK_REG, data);
        data |= 0x08;                   //internal resistor connect to GND
        wr(SCU_PLLLOCK_REG, data);
        drvi_ClockDelayUs(100);

        rd(SCU_PLLLOCK_REG, data);
        data &= ~0x03;                  //clear PD and PD_AD
        wr(SCU_PLLLOCK_REG, data);
        drvi_ClockDelayUs(100);

        rd(SCU_PLLLOCK_REG, data);
        data |= 0x01;                  //set PD = 1
        wr(SCU_PLLLOCK_REG, data);
        drvi_ClockDelayUs(10);

        rd(SCU_PLLLOCK_REG, data);
        data &= ~0x03;                  //clear PD and PD_AD
        wr(SCU_PLLLOCK_REG, data);
        drvi_ClockDelayUs(100);

        rd(SCU_PLLLOCK_REG, data);
        data |= 0x02;                  //set PD_AD = 1
        wr(SCU_PLLLOCK_REG, data);
        drvi_ClockDelayUs(100);

        rd(SCU_PLLLOCK_REG, data);
        data &= ~0x03;                  //clear PD and PD_AD
        wr(SCU_PLLLOCK_REG, data);
        drvi_ClockDelayUs(100);


        rd(SCU_PLLLOCK_REG, data);      //read data
        drvi_ClockDelayUs(100);
    }
*/

}

#endif //TEST_ADC



