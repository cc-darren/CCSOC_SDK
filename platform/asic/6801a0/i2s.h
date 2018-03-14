/**
 ****************************************************************************************
 *
 * @file i2s.h
 *
 * @brief Head file of i2s.c
 *
 * Copyright (C) CloudChip 2017-2019
 *
 *
 ****************************************************************************************
 */
#ifndef _I2S_H_
#define _I2S_H_
#include <stdio.h>
#include "global.h"
#include "cc6801_reg.h"

#define CC6801_I2S_BIT_DISABLE          0
#define CC6801_I2S_BIT_ENABLE           1

#define CC6801_I2S_BITS_8               7
#define CC6801_I2S_BITS_16              15
#define CC6801_I2S_BITS_20              19
#define CC6801_I2S_BITS_24              23
#define CC6801_I2S_BITS_28              27
#define CC6801_I2S_BITS_32              31

#define CC6801_I2S_BIT_FORMAT_I2S       1
#define CC6801_I2S_BIT_FORMAT_LJM       2
#define CC6801_I2S_BIT_FORMAT_RJM       3

#define CC6801_I2S_BIT_ALIGN_24BE       0
#define CC6801_I2S_BIT_ALIGN_24LE       1
#define CC6801_I2S_BIT_ALIGN_16LE       2
#define CC6801_I2S_BIT_ALIGN_8LE        3

// 0x00 - I2S Clock Control(I2SCLKCTL)
#define CC6801_I2S_WSRES_SHIFT          16
#define CC6801_I2S_WSRES_MASK           (0x1F << CC6801_I2S_WSRES_SHIFT)
#define CC6801_I2S_WSRES_8              (CC6801_I2S_BITS_8  << CC6801_I2S_WSRES_SHIFT)
#define CC6801_I2S_WSRES_16             (CC6801_I2S_BITS_16 << CC6801_I2S_WSRES_SHIFT)
#define CC6801_I2S_WSRES_20	            (CC6801_I2S_BITS_20 << CC6801_I2S_WSRES_SHIFT)
#define CC6801_I2S_WSRES_24	            (CC6801_I2S_BITS_24 << CC6801_I2S_WSRES_SHIFT)
#define CC6801_I2S_WSRES_28	            (CC6801_I2S_BITS_28 << CC6801_I2S_WSRES_SHIFT)
#define CC6801_I2S_WSRES_32	            (CC6801_I2S_BITS_32 << CC6801_I2S_WSRES_SHIFT)

#define CC6801_I2S_CLKDIV_SHIFT         8
#define CC6801_I2S_CLKDIV_MASK          (0xFF << CC6801_I2S_CLKDIV_SHIFT)

#define CC6801_I2S_BIT_SLAVE            0
#define CC6801_I2S_BIT_MASTER           1
#define CC6801_I2S_MS_SHIFT             3
#define CC6801_I2S_MS_MASK              (0x1 << CC6801_I2S_MS_SHIFT)
#define CC6801_I2S_MS_SLAVE             (CC6801_I2S_BIT_SLAVE  << CC6801_I2S_MS_SHIFT)
#define CC6801_I2S_MS_MASTER            (CC6801_I2S_BIT_MASTER << CC6801_I2S_MS_SHIFT)

#define CC6801_I2S_CLKEN_SHIFT          0
#define CC6801_I2S_CLKEN_MASK           (0x1 << CC6801_I2S_CLKEN_SHIFT)
#define CC6801_I2S_CLKEN_DISABLE        (CC6801_I2S_BIT_DISABLE  << CC6801_I2S_CLKEN_SHIFT)
#define CC6801_I2S_CLKEN_ENABLE         (CC6801_I2S_BIT_ENABLE << CC6801_I2S_CLKEN_SHIFT)

// 0x08 - I2S Tx/Rx Control(I2STXCTL/I2SRXCTRL)
#define CC6801_I2S_ALIGN_SHIFT          14
#define CC6801_I2S_ALIGN_MASK           (0x3 << CC6801_I2S_ALIGN_SHIFT)
#define CC6801_I2S_ALIGN_24BE           (CC6801_I2S_BIT_ALIGN_24BE  << CC6801_I2S_ALIGN_SHIFT)
#define CC6801_I2S_ALIGN_24LE           (CC6801_I2S_BIT_ALIGN_24LE << CC6801_I2S_ALIGN_SHIFT)
#define CC6801_I2S_ALIGN_16LE           (CC6801_I2S_BIT_ALIGN_16LE << CC6801_I2S_ALIGN_SHIFT)
#define CC6801_I2S_ALIGN_8LE            (CC6801_I2S_BIT_ALIGN_8LE << CC6801_I2S_ALIGN_SHIFT)

#define CC6801_I2S_RES_SHIFT            2
#define CC6801_I2S_RES_MASK             (0x1F << CC6801_I2S_RES_SHIFT)
#define CC6801_I2S_RES_8                (CC6801_I2S_BITS_8  << CC6801_I2S_RES_SHIFT)
#define CC6801_I2S_RES_16               (CC6801_I2S_BITS_16 << CC6801_I2S_RES_SHIFT)
#define CC6801_I2S_RES_20               (CC6801_I2S_BITS_20 << CC6801_I2S_RES_SHIFT)
#define CC6801_I2S_RES_24               (CC6801_I2S_BITS_24 << CC6801_I2S_RES_SHIFT)

#define CC6801_I2S_MOD_SHIFT            0
#define CC6801_I2S_MOD_MASK             (0x3 << CC6801_I2S_MOD_SHIFT)
#define CC6801_I2S_MOD_I2S              (CC6801_I2S_BIT_FORMAT_I2S << CC6801_I2S_MOD_SHIFT)
#define CC6801_I2S_MOD_RJM              (CC6801_I2S_BIT_FORMAT_RJM << CC6801_I2S_MOD_SHIFT)
#define CC6801_I2S_MOD_LJM              (CC6801_I2S_BIT_FORMAT_LJM << CC6801_I2S_MOD_SHIFT)

typedef struct S_DaiHwParams T_DaiHwParams;

void cc6801_I2sInit(T_callback handler);
int cc6801_I2sSetFormat(uint16_t wFmt);
int cc6801_I2sHwParams(T_DaiHwParams *tpParams);
void cc6801_I2sTxConfig(uint32_t dwDmaBufStart, uint32_t dwDmaBufEnd);
void cc6801_I2sRxConfig(uint32_t dwDmaBufStart, uint32_t dwDmaBufEnd);
void cc6801_I2sStart(uint8_t bTxByte, uint8_t bRxByte);
void cc6801_I2sStop(void);















#endif //_I2S_H_


