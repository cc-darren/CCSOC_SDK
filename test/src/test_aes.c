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
*
*
*  Project:
*  --------
*  CC6801
*
*  Description:
*  ------------
*  the description
*
*  Author:
*  -------
*  Blake   (cc-blake)
*
*===========================================================================
*
******************************************************************************/

/******************************************************************************
Head Block of The File
******************************************************************************/
#include "test.h"

#if (TEST_AES)

#include <stdio.h>
#include <string.h>

#include "global.h"
#include "cc6801_reg.h"

#include "test_aes.h"

#define AES_DATA_SIZE 64

/******************************************************************************
Declaration of External Variables & Functions
******************************************************************************/

/******************************************************************************
Declaration of data structure
******************************************************************************/
static uint32_t dwEncryptData[AES_DATA_SIZE] = {0};
static uint32_t dwDecryptData[AES_DATA_SIZE] = {0};
static uint32_t dwAesResult[AES_DATA_SIZE] = {0};

/******************************************************************************
Declaration of Global Variables & Functions
******************************************************************************/

/******************************************************************************
Declaration of static Global Variables & Functions
******************************************************************************/
static int g_iIndex = 0;
static enum drvi_aes_mode g_EMode;
//extern uint32_t AES_TEST_START;



void TEST_AesInit(void)
{
    for (g_iIndex=0; g_iIndex<AES_DATA_SIZE; g_iIndex++)
        dwDecryptData[g_iIndex] = g_iIndex;

    g_iIndex = 8;
    g_EMode = DRVI_AES_MODE_ECB;
}

void TEST_AesSingleMode(void)
{
    int iResult = 0;

    g_iIndex *= 2;
    g_EMode++;

    if (g_iIndex > AES_DATA_SIZE)
      g_iIndex = 16;

    if (g_EMode > DRVI_AES_MODE_LAST)
      g_EMode = DRVI_AES_MODE_ECB;

    do
    {
        memset(dwEncryptData, 0, g_iIndex);
        memset(dwAesResult, 0, g_iIndex);

        if (drvi_aes_init(g_EMode))
        {
            //AES_TEST_START = 1;
            break;
        }
        drvi_aes_encrypt((uint32_t)dwDecryptData, (uint32_t)dwEncryptData, g_iIndex);
        if (drvi_aes_init(g_EMode))
        {
            //AES_TEST_START = 1;
            break;
        }
        drvi_aes_decrypt((uint32_t)dwEncryptData, (uint32_t)dwAesResult, g_iIndex);

        iResult = memcmp((const void *)dwDecryptData, (const void *)dwAesResult, g_iIndex);

        if (iResult)
        {
            printf("AES test error, mode=%d, size=%d\r\n", g_EMode, g_iIndex);
        }
        else
        {
            printf("AES test pass, mode=%d, size=%d\r\n", g_EMode, g_iIndex);
        }
    } while(0);

}

void TEST_AesLoop(int iCount)
{
    int iIndex = 0;
    int iResult = 0;
    enum drvi_aes_mode E_Mode;

    for (iIndex=0; iIndex<AES_DATA_SIZE; iIndex++)
        dwDecryptData[iIndex] = iIndex;

    while(iCount>0)
    {
        for (iIndex=16; iIndex<=AES_DATA_SIZE; iIndex*=2)
        {
            for (E_Mode=DRVI_AES_MODE_ECB; E_Mode<=DRVI_AES_MODE_LAST; E_Mode++)
            {
                do
                {
                    memset(dwEncryptData, 0, iIndex);
                    memset(dwAesResult, 0, iIndex);

                    if (drvi_aes_init(E_Mode))
                        break;
                    drvi_aes_encrypt((uint32_t)dwDecryptData, (uint32_t)dwEncryptData, iIndex);
                    if (drvi_aes_init(E_Mode))
                        break;
                    drvi_aes_decrypt((uint32_t)dwEncryptData, (uint32_t)dwAesResult, iIndex);

                    iResult = memcmp((const void *)dwDecryptData, (const void *)dwAesResult, iIndex);

                    if (iResult)
                    {
                        printf("AES test error, mode=%d, size=%d\r\n", E_Mode, iIndex);
                    }
                } while(0);

            }
        }

        iCount--;
        if (!(iCount%100)) printf("%d times Pass!\r\n", iCount);
    }
}
#endif //TEST_AES
