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
*  asic_options.h
*
*  Project:
*  --------
*  All
*
*  Description:
*  ------------
*  find and include corresopnding asic options header file
*
*  Author:
*  -------
*  PPP   (CC-pattaya)
*
*===========================================================================
*
******************************************************************************/
#ifndef _ASIC_OPTIONS_H_
#define _ASIC_OPTIONS_H_

#define STRINGIZE_AUX(a) #a
#define STRINGIZE(a) STRINGIZE_AUX(a)
#define CAT_AUX(a, b) a##b
#define CAT(a, b) CAT_AUX(a, b)
#define FIND_ASIC(file) STRINGIZE(CAT(file, _options.h))
#include FIND_ASIC(ASIC)



#define UseInterface(_type,_id)   Interface_##_type##_id



#endif  //_ASIC_OPTIONS_H_
