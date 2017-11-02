/******************************************************************************
*  Copyright 2017, CloudChip, Inc.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of CloudChip, Inc. (C) 2017
******************************************************************************/

/******************************************************************************
*  Filename:
*  ---------
*  aes.c
*
*  Project:
*  --------
*  cc6801
*
*  Description:
*  ------------
*  This is AES driver
*
*  Author:
*  -------
*  CloudChip
*
*===========================================================================/
*  20170206 PAT initial version
******************************************************************************/
#include "aes.h"
#include "cc6801_reg.h"

volatile uint32_t AES_INTR = 0;

void AES_IRQHandler(void)
{
    regAES->bf.intSts = 1;
    regAES->bf.intEn = 1;
    AES_INTR = 1;
}

void cc6801_aes_encrypt(uint32_t in_addr, uint32_t out_addr, int size)
{
    regAES->bf.decrypt = 0;
    regAES->bf.dmaRdAddr = in_addr;
    regAES->bf.dmaWrAddr = out_addr;
    regAES->bf.rByteNum = size-1;
    regAES->bf.wByteNum = size-1;

    NVIC_EnableIRQ(AES_IRQn);
    regAES->bf.dmaEn = 1;

    while(!AES_INTR);
    AES_INTR = 0;

    NVIC_DisableIRQ(AES_IRQn);
}

void cc6801_aes_decrypt(uint32_t in_addr, uint32_t out_addr, int size)
{
    regAES->bf.decrypt = 1;
    regAES->bf.dmaRdAddr = in_addr;
    regAES->bf.dmaWrAddr = out_addr;
    regAES->bf.rByteNum = size-1;
    regAES->bf.wByteNum = size-1;

    NVIC_EnableIRQ(AES_IRQn);
    regAES->bf.dmaEn = 1;

    while(!AES_INTR);
    AES_INTR = 0;

    NVIC_DisableIRQ(AES_IRQn);
}

static int cc6801_aes_set_mode(uint32_t mode)
{
    cc6801_aes_mode bc_mode;

    switch (mode)
    {
        case DRVI_AES_MODE_ECB:
            bc_mode = CC6801_AES_MODE_ECB;
            break;

        case DRVI_AES_MODE_CBC:
            bc_mode = CC6801_AES_MODE_CBC;
            break;

        case DRVI_AES_MODE_OFB:
            bc_mode = CC6801_AES_MODE_OFB;
            break;

        case DRVI_AES_MODE_CFB:
            bc_mode = CC6801_AES_MODE_CFB;
            break;

        case DRVI_AES_MODE_CTR:
            bc_mode = CC6801_AES_MODE_CTR;
            break;

        case DRVI_AES_MODE_CMAC:
        default:
            return CC_ERROR_INVALID_DATA;
    }

    regAES->bf.bcMode = bc_mode;

    return CC_SUCCESS;
}


int cc6801_aes_init(uint32_t mode)
{

    regCKGEN->bf.aesSwRst = 0;
    regCKGEN->bf.aesSwRst = 1;

    regAES->bf.intEn = 1;         //default enable interrupt;
    regAES->bf.keySize = 0;

    regAES->bf.dbusBurst = 0;
    regAES->bf.opMode = 2;

    return cc6801_aes_set_mode(mode);
}

