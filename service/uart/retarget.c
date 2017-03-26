/******************************************************************************
*  Copyright 2017, Cloud Chip, Inc.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Cloud Chip, Inc. (C) 2017
******************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include "app_uart.h"
#include "global.h"

struct __FILE
{
    int handle;
};

FILE __stdout;
FILE __stdin;

int fputc(int ch, FILE * p_file)
{
    UNUSED_PARAMETER(p_file);

    UNUSED_VARIABLE(app_uart_put((uint8_t)ch));
    return ch;
}

int fgetc(FILE * p_file)
{
    uint8_t input;
    while (app_uart_get(&input) == CC_ERROR_NOT_FOUND)
    {
        // No implementation needed.
    }
    return input;
}
