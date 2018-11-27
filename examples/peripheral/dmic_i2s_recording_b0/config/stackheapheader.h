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

/******************************************************************************
*  Filename:
*  ---------
*  stackheapheader.h
*
*  Project:
*  --------
*  cc6801
*
*  Description:
*  ------------
*  System stack and heap size.
*  Stack/heap size may be different from projects.
*  Cadence/Speed projects may have smaller size and Wristband/Watch projects may have bigger size.
*
*  Author:
*  -------
*  PPP   (CC-pattaya)
*
*===========================================================================
*
******************************************************************************/

#ifndef _STACKHEAPHEADER_H_
#define _STACKHEAPHEADER_H_

#define SYS_STACK_SIZE      0x800
#define SYS_HEAP_SIZE       0x004

//;*********************
//;   App image header *
//;*********************
//Image FW version format: 0xAABBBCCC, AA=major, BBB=minor, CCC=update
#define IMG_FW_MAJOR        0x01000000  
#define IMG_FW_MINOR        0x00001000
#define IMG_FW_UPDATE       0x00000001

#define IMG_ID              "6801"
#define IMG_FW_VERSION      (IMG_FW_MAJOR+IMG_FW_MINOR+IMG_FW_UPDATE)
#define IMG_FLAG1           0xFFFFFFFF
#define IMG_FLAG2           0xFFFFFFFF

//;****************
//;   OTA Block   *
//;****************
#define OTA_ID              "XXXX"
#define OTA_FLAG            0x5613C648
#define OTA_START_ADDR      0x00000000
#define OTA_FILE_SIZE       0x00000000

//;****************
//;   BLE Block   *
//;****************
#define BLE_ID              "BLE_"
#define BLE_FLAG            0x00000000
#define BLE_RANDOM_KEY      0x12345678


#endif //_STACKHEAPHEADER_H_


