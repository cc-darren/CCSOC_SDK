/**
 ****************************************************************************************
 *
 * @file drvi_aes.h
 *
 * @brief Head file of drvi_aes.c
 *
 * Copyright (C) CloudChip 2017-2019
 *
 *
 ****************************************************************************************
 */
#ifndef _DRVI_AES_H_
#define _DRVI_AES_H_
#include "aes.h"

#define drvi_aes_init(mode)                    cc6801_aes_init(mode)
#define drvi_aes_encrypt(in,out,size)          cc6801_aes_encrypt(in,out,size)
#define drvi_aes_decrypt(in,out,size)          cc6801_aes_decrypt(in,out,size)


enum drvi_aes_mode {
    DRVI_AES_MODE_ECB = 0,
    DRVI_AES_MODE_CBC,
    DRVI_AES_MODE_OFB,
    DRVI_AES_MODE_CFB,
    DRVI_AES_MODE_CTR,
    DRVI_AES_MODE_CMAC,
    DRVI_AES_MODE_LAST,
};












#endif //_DRVI_AES_H_


