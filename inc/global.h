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

#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include <stdbool.h>
#include "ARMCM.h"
#include "ll.h"
#include "compiler.h"
#include "error.h"
#define malloc(size)		pvPortMalloc(size)
#define free(ptr)			vPortFree(ptr)
#define UINT8               unsigned char           // 1 byte
#define UINT16              unsigned short          // 2 bytes
#define UINT32              unsigned long           // 4 bytes
#define INT8                char
#define INT16               short
#define INT32               long
#define BOOL                UINT8
#define FALSE               (0)
#define TRUE                (1)
#define SUCCESS             TRUE
#define FAIL                FALSE
#define ENABLE              TRUE
#define DISABLE             FALSE

typedef void (*T_callback)(void* p);
typedef uint32_t T_IF_ID;                   //peripheral Interface ID

#endif //_GLOBAL_H_
