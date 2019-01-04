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

/******************************************************************************
*  Filename:
*  ---------
*  w25q80dv.h
*
*  Project:
*  --------
*  CC6801
*
*  Description:
*  ------------
*  This is flash driver of winbond w25q80dv
*
*  Author:
*  -------
*  PPP   (cc-pattaya)
*
*===========================================================================
*
******************************************************************************/
#ifndef _FLASH_W25Q80DV_H_
#define _FLASH_W25Q80DV_H_

#include <stdint.h>
#include "global.h"

/******************************************************************************
Head Block of The File
******************************************************************************/
#define FLASH_Init()                   winbond_Init()
#define FLASH_Read(x,y,z)              winbond_Read(x,y,z)
#define FLASH_Write(x,y,z)             winbond_Write(x,y,z)
#define FLASH_Erase(x,y)               winbond_Erase(x,y)

#if defined(MODULE_FLASH) && (MODULE_FLASH==FLASH_W25Q256JV)
#define WINBOND_BLOCK_NUM       512
#endif

#if defined(MODULE_FLASH) && (MODULE_FLASH==FLASH_W25Q80DV)
#define WINBOND_BLOCK_NUM       16
#endif

#define WINBOND_BLOCK_SIZE      65536
#define WINBOND_SECTOR_SIZE     4096
#define WINBOND_PAGE_SIZE       128
#define WINBOND_FLASH_SIZE      (WINBOND_BLOCK_NUM*WINBOND_BLOCK_SIZE)

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
int winbond_Init(void);
int winbond_Read(uint8_t *pBuff, uint32_t dwFlashAddr, uint32_t dwSize);
int winbond_Write(uint32_t dwFlashAddr, uint8_t *pBuff, uint32_t dwSize);
int winbond_Erase(uint32_t dwFlashAddr, uint32_t dwSectorCount);

#endif //_FLASH_W25Q80DV_H_




