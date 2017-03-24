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

#include "cc_delay.h"

/*lint --e{438} "Variable not used" */
void cc_delay_ms(uint32_t volatile number_of_ms)
{
    while(number_of_ms != 0)
    {
        number_of_ms--;
        nrf_delay_us(999);
    }
}
