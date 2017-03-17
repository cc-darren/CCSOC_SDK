/**
 ****************************************************************************************
 *
 * @file arch_main.c
 *
 * @brief Main loop of the application.
 *
 * Copyright (C) CloudChip 2017-2019
 *
 *
 ****************************************************************************************
 */


/*
 * INCLUDES
 ****************************************************************************************
 */
#include <string.h>
#include "ll.h" 


extern void intc_init(void);
extern void testbed(void);
/*
 * MAIN FUNCTION
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief RW main function.
 *
 * This function is called right after the booting process has completed.
 *
 * @return status   exit status
 ****************************************************************************************
 */
void rw_main(void)
{
    /*
     ************************************************************************************
     * Platform initialization
     ************************************************************************************
     */
    //intc_init();



    /*
     ************************************************************************************
     * RW SW stack initialization
     ************************************************************************************
     */


    
    

    // finally start interrupt handling
    GLOBAL_INT_START();

    /*
     ************************************************************************************
     * Main loop
     ************************************************************************************
     */

    while(1)
    {
        uint32_t i;
        uint32_t size = 0x100;
        uint32_t *romAddr   = (uint32_t*)0x00000E0;
        //uint32_t *eFlashAddr= (uint32_t*)0x10030000;
        uint32_t *sRamAddr  = (uint32_t*)0x20008000;
        uint32_t *dRamAddr  = (uint32_t*)0x20011000;

        //memset(sRamAddr, 0x55, size);
        memcpy(sRamAddr, romAddr, size);
        //memcpy(sRamAddr, eFlashAddr, size);
        memcpy(dRamAddr, sRamAddr, size);
        
        for(i=0;i<(size>>2);i++)
        {
            if(*(dRamAddr+i) != *(sRamAddr+i))
            {
                *((volatile unsigned int *)(0x40000220)) = i;
            }
        }
        
        //finish testing
        *((volatile unsigned int *)(0x4000020C)) = 1;
        
        
        WFI();
    }
}

int main(void)
{
    //drvi_initialize();
    
    rw_main();

    while(1);
    //return(0);
}

