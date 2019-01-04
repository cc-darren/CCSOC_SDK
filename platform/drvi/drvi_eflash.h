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

#ifndef _DRVI_EFLASH_H_
#define _DRVI_EFLASH_H_

#include "eflash.h"


__forceinline void drvi_EflashInit(void)
{
    cc6801_EflashInit();
}

__forceinline void drvi_EflashFlush(void)
{
    cc6801_EflashFlush();
}
__forceinline BOOL drvi_EflashEraseALL(void)
{
    return cc6801_EflashEraseALL();
}
__forceinline BOOL drvi_EflashErasePage(uint32_t dwEflashAdr)
{
    return cc6801_EflashErasePage(dwEflashAdr);
}
__forceinline void drvi_EflashProgram(uint32_t dwEflashAdr,unsigned char * pBufAdr,uint32_t dwBufSize)
{
    cc6801_EflashProgram(dwEflashAdr,pBufAdr,dwBufSize);
}
__forceinline void drvi_EflashRegisterCallback(fpEflash_Callback fpCB)
{
    cc6801_EflashRegisterCallback(fpCB);
}
__forceinline void drvi_SecurityProtect(void)
{
    cc6801_SecurityProtect();
}
/**@brief SoC Events. */
enum E_DRVI_EFLASH_EVTS
{
  E_DRVI_EFLASH_OPERATION_SUCCESS,              /**< Event indicating that the ongoing flash operation has completed successfully. */
  E_DRVI_EFLASH_OPERATION_ERROR,                /**< Event indicating that the ongoing flash operation has timed out with an error. */
  E_DRVI_EFLASH_NUMBER_OF_EVTS
};
#endif
