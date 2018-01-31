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
#include "global.h"

void cc6801_I2sInit(T_callback handler);
void cc6801_I2sTxConfig(uint32_t dwDmaBufStart, uint32_t dwDmaBufEnd);
void cc6801_I2sRxConfig(uint32_t dwDmaBufStart, uint32_t dwDmaBufEnd);
void cc6801_I2sStart(uint16_t dwTxByte, uint16_t dwRxByte);
void cc6801_I2sStop(void);















#endif //_I2S_H_


