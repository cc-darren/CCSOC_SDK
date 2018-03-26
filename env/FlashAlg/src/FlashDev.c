/***********************************************************************/
/*  This file is part of the ARM Toolchain package                     */
/*  Copyright (c) 2010 Keil - An ARM Company. All rights reserved.     */	
/***********************************************************************/
/*                                                                     */
/*  FlashDev.C:  Device Description for New Device Flash               */
/*                                                                     */
/***********************************************************************/

#include "FlashOS.H"        // FlashOS Structures


/*------------------------------------------------------*/
/*    begin build options                               */
/*------------------------------------------------------*/


#define BuildVersion 0.72

#define strASIC CC6801
/*------------------------------------------------------*/
/*    end build options                                 */
/*------------------------------------------------------*/


#if (defined ARMCM4_FP)
    #define TARGETCPU M4f
#elif (defined ARMCM0P)
    #define TARGETCPU M0
#else
  #error device not specified!
#endif


#if (defined EF_PROTECTED)
    #define strPROTECT (protect)
#else
    #define strPROTECT (no protect)
#endif



#define STRINGIZE_AUX(a) #a
#define STRINGIZE(a) STRINGIZE_AUX(a)
#define CAT4(a,b,c,d) a##_##b##_##c##d
#define DEVNAME(s_asicname,s_cpuname,s_version,s_protection) STRINGIZE(CAT4(s_asicname,s_cpuname,s_version,s_protection))

struct FlashDevice const FlashDevice  =  {
   FLASH_DRV_VERS,             // Driver Version, do not modify!
   DEVNAME(strASIC,TARGETCPU,BuildVersion,strPROTECT),   // Device Name 
   ONCHIP,                     // Device Type
   0x10000000,                 // Device Start Address
   0x00040000,                 // Device Size in Bytes (256kB)
   2048,                       // Programming Page Size
   0,                          // Reserved, must be 0
   0xFF,                       // Initial Content of Erased Memory
   20000,                       // Program Page Timeout 5000 mSec
   20000,                       // Erase Sector Timeout 5000 mSec

// Specify Size and Address of Sectors
   0x00040000, 0x00000000,         // 
   SECTOR_END
};


#define FL_NSECT    1
