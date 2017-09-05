/******************************************************************************
*  Copyright 2017, Cloudchip, Inc.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Cloudchip, Inc. (C) 2017
******************************************************************************/

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



#define UseInterface(_type,_id) _Interface_##_type##_id##_
#define GetInterfaceType(_func_IF) (_func_IF & 0xF0)
#define GetInterfaceID(_func_IF)   (_func_IF & 0x0F)


#endif  //_ASIC_OPTIONS_H_
