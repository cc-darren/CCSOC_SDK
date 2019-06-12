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

/*
 * INCLUDES
 ****************************************************************************************
 */
#include <stdio.h>
#include "ll.h"
#include "drvi_init.h"
#include "tracer.h"
#include "project.h"

extern void sys_InitMain(void);
extern void cc6801_ClockDelayMs(volatile uint32_t);
/*
 * MAIN FUNCTION
 ****************************************************************************************
 */
int main(void)
{
    AxesRaw_t   stMagBuf;    
    uint32_t    dwTestCntr = 0;
    
    //Must be first in main()
    sys_InitMain();

    //start interrupt handling
    GLOBAL_INT_START(); 

    drvi_initialize();
    
    MAG_Init();
    
    TracerInfo("== CC6801 - I2C0 - Start ==\r\n");

    /*******************************/
    /****** Application Start ******/
    /*******************************/
    while(1)
    {             
        AK09912_MAG_GetMagRaw(&stMagBuf);
        TracerInfo("[I0:%04d] mag_x:0x%x, mag_y:0x%x, mag_z:0x%x\r\n", ++dwTestCntr, stMagBuf.AXIS_X, stMagBuf.AXIS_Y, stMagBuf.AXIS_Z);
        cc6801_ClockDelayMs(1000);         
    }
    
    //return(0);
}
