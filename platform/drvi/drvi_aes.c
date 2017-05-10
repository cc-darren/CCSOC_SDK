/**
 ****************************************************************************************
 *
 * @file drvi_aes.c
 *
 * @brief HW AES interface for all SOC.
 *
 * Copyright (C) CloudChip 2017-2019
 *
 *
 ****************************************************************************************
 */
#include "global.h"
#include "drvi_aes.h"
#include "cc6801_reg.h"

#ifdef TEST_AES

#include <stdio.h>
#include <string.h>

static uint32_t encrypt_data[4096] = {0};
static uint32_t decrypt_data[4096] = {0};
static uint32_t aes_result[4096] = {0};

void aes_start_test(void)
{
    int i = 0;
    int result = 0;
    enum drvi_aes_mode mode;
    int count = 0;

    for (i=0; i<4096; i++)
        decrypt_data[i] = i;

    while(1)
    {
        for (i=16; i<=4096; i*=2)
        {
            for (mode=DRVI_AES_MODE_ECB; mode<=DRVI_AES_MODE_LAST; mode++)
            {
                do
                {
                    memset(encrypt_data, 0, i);
                    memset(aes_result, 0, i);

                    if (drvi_aes_init(mode))
                        break;
                    drvi_aes_encrypt((uint32_t)decrypt_data, (uint32_t)encrypt_data, i);
                    if (drvi_aes_init(mode))
                        break;
                    drvi_aes_decrypt((uint32_t)encrypt_data, (uint32_t)aes_result, i);

                    result = memcmp((const void *)decrypt_data, (const void *)aes_result, i);

                    if (result)
                    {
                        printf("AES test error, mode=%d, size=%d\r\n", mode, i);
                    }
                } while(0);

            }
        }

        count++;
        if (!(count%100)) printf("%d times Pass!\r\n", cycle);
    }
}
#endif
