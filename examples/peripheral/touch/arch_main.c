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

/*
 * INCLUDES
 ****************************************************************************************
 */
#include <string.h>
#include "ll.h"
#include "drvi_init.h"
#include "tracer.h"
#include "project.h"
#include "touch.h"


extern T_fingerInfo fg;

extern void sys_InitMain(void);

/*
 * MAIN FUNCTION
 ****************************************************************************************
 */

int main(void)
{
    //Must be first in main()
    sys_InitMain();

    //start interrupt handling
    GLOBAL_INT_START(); 

    drvi_initialize();

    TracerInfo("== CC6801 Touch Test Start ==\r\n");

    /*******************************/
    /****** Application Start ******/
    /*******************************/
    
    touch_init(NULL);
    
    //case1: polling
    while(1)
    {
        if(touch_getFingerDataIfReady())
        {
            TracerInfo("#%-2d ges=%-2x F1=%-5d,%-5d", \
                        fg.bTouchNum, fg.bGesture, fg.tF[0].wX, fg.tF[0].wY);
          #if (TOUCH_SUPPORTED_FINGERS == 2)
            TracerInfo(" F2=%-5d,%-5d", fg.tF[1].wX, fg.tF[1].wY);
          #endif
            TracerInfo("\r");
        }
    }
    
    //case2: interrupt callback
    //       When touch interrupt is triggered(GPIO interrupt), callback function will be scheduled.
    //       When callback func is called, callback func calls touch_getFingerDataIfReady()
    //       This example can't demo case2 because there is no scheduling.

}

