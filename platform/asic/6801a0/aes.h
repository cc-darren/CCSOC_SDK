/**
 ****************************************************************************************
 *
 * @file aes.h
 *
 * @brief Head file of aes.c
 *
 * Copyright (C) CloudChip 2017-2019
 *
 *
 ****************************************************************************************
 */
#ifndef _AES_H_
#define _AES_H_
#include "global.h"
#include "drvi_aes.h"


typedef enum {
    CC6801_AES_MODE_ECB           = 0x00000001,
    CC6801_AES_MODE_CBC           = 0x00000002,
    CC6801_AES_MODE_CFB           = 0x00000004,
    CC6801_AES_MODE_OFB           = 0x00000008,
    CC6801_AES_MODE_CTR           = 0x00000010,
    CC6801_AES_MODE_ERROR         = 0x000000FF,
}cc6801_aes_mode;




int cc6801_aes_init(uint32_t mode);
void cc6801_aes_encrypt(uint32_t in_addr, uint32_t out_addr, int size);
void cc6801_aes_decrypt(uint32_t in_addr, uint32_t out_addr, int size);













#endif //_AES_H_


