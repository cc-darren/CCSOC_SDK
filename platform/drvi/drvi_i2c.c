/* Copyright (c) 2019 Cloudchip, Inc. All Rights Reserved.
 *
 * The information contained herein is property of Cloudchip, Inc.
 * Terms and conditions of usage are described in detail in CLOUDCHIP
 * STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information.
 * NO WARRANTY of ANY KIND is provided. This heading must NOT be removed 
 * from the file.
 */
 
#include "drvi_i2c.h"

#if (defined I2C_INUSE && (I2C_INUSE))
int32_t drvi_I2cInit(void)
{
    int32_t iResult = 0;

#if defined I2C0_INUSE && (I2C0_INUSE)
    do
    {
        iResult = cc6801_I2cInit(I2C_0);
        if(iResult)
        {
            //Todo: When RTOS is enabled, interrupt must be disabled during initialization.
            //      So, tracer function can't be enabled.
            //      We need to implement UART direct output function
            //TracerErr("I2C%d initial error\r\n", I2C_0);
            break;
        }

        iResult = cc6801_I2cFreqSet(I2C_0, I2C0_CLOCK);
            //Todo: When RTOS is enabled, interrupt must be disabled during initialization.
            //      So, tracer function can't be enabled.
            //      We need to implement UART direct output function
            //if(iResult)
            //    TracerErr("I2C%d clock set error\r\n", I2C_0);
    } while(0);
#endif

#if defined I2C1_INUSE && (I2C1_INUSE)
    do
    {
        iResult = cc6801_I2cInit(I2C_1);
        if(iResult)
        {
            //Todo: When RTOS is enabled, interrupt must be disabled during initialization.
            //      So, tracer function can't be enabled.
            //      We need to implement UART direct output function
            //TracerErr("I2C%d initial error\r\n", I2C_1);
            break;
        }

        iResult = cc6801_I2cFreqSet(I2C_1, I2C1_CLOCK);
            //Todo: When RTOS is enabled, interrupt must be disabled during initialization.
            //      So, tracer function can't be enabled.
            //      We need to implement UART direct output function
            //if (iResult)
            //    TracerErr("I2C%d clock set error\r\n", I2C_1);
    } while(0);
#endif

    return iResult;
}

#if (!(defined(CC6801B0) || defined(CC6801B1)))
int32_t drvi_I2cWriteThenRead(T_I2cDevice *tpDev,
                          uint8_t const *pTxData, uint16_t wTxLen,
                          uint8_t *pRxData, uint16_t wRxLen)
{
    int32_t iResult = 0;

    do
    {
        iResult = cc6801_I2cWrite(tpDev, pTxData, wTxLen);
        if(CC_SUCCESS != iResult)
        {
            break;
        }

        iResult = cc6801_I2cRead(tpDev, pRxData, wRxLen);
    } while(0);

    return iResult;
}
#endif
#endif
