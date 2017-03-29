/******************************************************************************
*  Copyright 2017, Cloud Chip, Inc.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Cloud Chip, Inc. (C) 2017
******************************************************************************/
#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include "ll.h"
#include "compiler.h"
#include "error.h"
#include "utility.h"

#define FALSE               (0)
#define TRUE                (1)
#define SUCCESS             TRUE
#define FAIL                FALSE

typedef void (*T_callback)(void* p);

#endif //_GLOBAL_H_
