/**
 ****************************************************************************************
 *
 * @file drvi_i2s.h
 *
 * @brief Head file of drvi_i2s.c
 *
 * Copyright (C) CloudChip 2017-2019
 *
 *
 ****************************************************************************************
 */
#ifndef _DRVI_I2S_H_
#define _DRVI_I2S_H_
#include "project.h"
#include "i2s.h"

__forceinline void drvi_I2sInit(T_callback *handler)
{
    cc6801_I2sInit(handler);
}

__forceinline void drvi_I2sTxConfig(uint32_t dwDmaBufStart, uint32_t dwDmaBufEnd)
{
    cc6801_I2sTxConfig(dwDmaBufStart, dwDmaBufEnd);
}

__forceinline void drvi_I2sRxConfig(uint32_t dwDmaBufStart, uint32_t dwDmaBufEnd)
{
    cc6801_I2sRxConfig(dwDmaBufStart, dwDmaBufEnd);
}

__forceinline void drvi_I2sStart(uint16_t dwTxByte, uint16_t dwRxByte)
{
    cc6801_I2sStart(dwTxByte, dwRxByte);
}

__forceinline void drvi_I2sStop(void)
{
    cc6801_I2sStop();
}

#endif //_DRVI_I2S_H_
