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
    uint8_t  bAccBuf[6];
    uint32_t dwTestCntr = 0;
    
    //Must be first in main()
    sys_InitMain();

    //start interrupt handling
    GLOBAL_INT_START(); 

    drvi_initialize();

    ACC_Init();
    
    TracerInfo("== CC6801 - SPI1 - Start ==\r\n");

    /*******************************/
    /****** Application Start ******/
    /*******************************/
    while(1)
    {
        ACC_GetRawAccData((i16_t *)bAccBuf);
        
        TracerInfo("[S1:%03d] acc_x:0x%02x 0x%02x, acc_y:0x%02x 0x%02x, acc_z:0x%02x 0x%02x\r\n", 
                    ++dwTestCntr, 
                    bAccBuf[1], 
                    bAccBuf[0], 
                    bAccBuf[3], 
                    bAccBuf[2], 
                    bAccBuf[5], 
                    bAccBuf[4]);
        
        cc6801_ClockDelayMs(1000);        
    }
        
    //return(0);
}
