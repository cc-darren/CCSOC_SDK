/***********************************************************************/
/*  This file is part of the ARM Toolchain package                     */
/*  Copyright (c) 2010 Keil - An ARM Company. All rights reserved.     */
/***********************************************************************/
/*                                                                     */
/*  FlashDev.C:  Flash Programming Functions adapted                   */
/*               for New Device 256kB Flash                            */
/*                                                                     */
/***********************************************************************/

#include "FlashOS.H"        // FlashOS Structures


#include "drvi_eflash.h"

/* 
   Mandatory Flash Programming Functions (Called by FlashOS):
                int Init        (unsigned long adr,   // Initialize Flash
                                 unsigned long clk,
                                 unsigned long fnc);
                int UnInit      (unsigned long fnc);  // De-initialize Flash
                int EraseSector (unsigned long adr);  // Erase Sector Function
                int ProgramPage (unsigned long adr,   // Program Page Function
                                 unsigned long sz,
                                 unsigned char *buf);

   Optional  Flash Programming Functions (Called by FlashOS):
                int BlankCheck  (unsigned long adr,   // Blank Check
                                 unsigned long sz,
                                 unsigned char pat);
                int EraseChip   (void);               // Erase complete Device
      unsigned long Verify      (unsigned long adr,   // Verify Function
                                 unsigned long sz,
                                 unsigned char *buf);

       - BlanckCheck  is necessary if Flash space is not mapped into CPU memory space
       - Verify       is necessary if Flash space is not mapped into CPU memory space
       - if EraseChip is not provided than EraseSector for all sectors is called
*/



/*
 *  Initialize Flash Programming Functions
 *    Parameter:      adr:  Device Base Address
 *                    clk:  Clock Frequency (Hz)
 *                    fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */
int Init (unsigned long adr, unsigned long clk, unsigned long fnc) {
    /* Add your Code */
    if (fnc != 3) {
        drvi_EflashInit();
    }
  return (0);                                  // Finished without Errors
}

/*
 *  De-Initialize Flash Programming Functions
 *    Parameter:      fnc:  Function Code (1 - Erase, 2 - Program, 3 - Verify)
 *    Return Value:   0 - OK,  1 - Failed
 */

int UnInit (unsigned long fnc) {
    /* Add your Code */
    if (fnc==2) {
    #if (defined EF_PROTECTED)
        drvi_SecurityProtect();
    #endif
    }
    drvi_EflashFlush();
    
    return (0);                                  // Finished without Errors
}


/*
 *  Erase complete Flash Memory
 *    Return Value:   0 - OK,  1 - Failed
 */
int EraseChip (void) {

    /* Add your Code */
    drvi_EflashEraseALL();
    return (0);                                  // Finished without Errors
}


/*
 *  Erase Sector in Flash Memory
 *    Parameter:      adr:  Sector Address
 *    Return Value:   0 - OK,  1 - Failed
 */

int EraseSector (unsigned long adr) {

    /* Add your Code */
    EraseChip();
    return (0);                                  // Finished without Errors
}


/*
 *  Program Page in Flash Memory
 *    Parameter:      adr:  Page Start Address
 *                    sz:   Page Size
 *                    buf:  Page Data
 *    Return Value:   0 - OK,  1 - Failed
 */

int ProgramPage (unsigned long adr, unsigned long sz, unsigned char *buf) {
    /* Add your Code */

    drvi_EflashProgram(adr,buf,sz);
    return (0);                                  // Finished without Errors
}

//int  BlankCheck (unsigned long adr,unsigned long sz,unsigned char pat)
/*
int BlankCheck (unsigned long adr, unsigned long sz, unsigned char pat)
{
    return 0;
}
*/

unsigned long Verify (unsigned long adr,   // Verify Function
                      unsigned long sz,
                      unsigned char *buf)
{
    uint32_t ii,dwdata1,dwdata2;

    for (ii=0;ii<sz;ii+=4) {
        dwdata1 = *((uint32_t *)adr);
        dwdata2 = *((uint32_t *)buf);
        if (dwdata1 != dwdata2) {
            break;
        }
    }
    return (adr+ii);
}







