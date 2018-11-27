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

/* Includes ------------------------------------------------------------------*/
#include "project.h"

#if (MODULE_PRESSURE == PRESSURE_NULL)
/******************************************************************
*   place all function and private variables in this part
******************************************************************/
#define PRESSURE_IF_Read(tbuf,tlen,rbuf,rlen)
#define PRESSURE_IF_Write(buf,len)

#endif //#if (MODULE_PRESSURE == PRESSURE_NULL)
