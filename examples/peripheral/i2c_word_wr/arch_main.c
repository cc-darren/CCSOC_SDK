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

/*
 * INCLUDES
 ****************************************************************************************
 */
#include <stdio.h>
#include <string.h>

#include "ll.h"
#include "drvi_init.h"
#include "drvi_i2c.h"
#include "tracer.h"
#include "project.h"

T_I2cDevice g_tSgtl5000Dev =
{
    .bBusNum = AUDIO_IF_ID,
    .bAddr = SGTL5000_SLAVE_ADDRESS,
};

extern void sys_InitMain(void);

int I2cWordRead(uint16_t wReg, uint16_t *wVal)
{
    int iRet;
    uint8_t bBuf[2];

    //send i2c device address first
    bBuf[0] = (wReg >> 8) & 0xFF;
    bBuf[1] = wReg & 0xFF;
    iRet = drvi_I2cWrite(&g_tSgtl5000Dev, bBuf, sizeof(bBuf));
    if (iRet)
        return iRet;

    //Read data from i2c
    iRet = drvi_I2cRead(&g_tSgtl5000Dev, bBuf, sizeof(bBuf));
    if (iRet != CC_SUCCESS)
        return CC_ERROR_INTERNAL;

    *wVal = ((bBuf[0] << 8) | bBuf[1]);

    return iRet;
}

int I2cWordWrite(uint16_t wReg, uint16_t wVal)
{
    uint8_t bBuf[4];
    int iRet;

    bBuf[0] = (wReg >> 8) & 0xFF;
    bBuf[1] = wReg & 0xFF;
    bBuf[2] = (wVal >> 8) & 0xFF;
    bBuf[3] = wVal & 0xFF;

    iRet = drvi_I2cWrite(&g_tSgtl5000Dev, (uint8_t *)bBuf, sizeof(bBuf));
    if (iRet != CC_SUCCESS)
        return CC_ERROR_INTERNAL;

    return iRet;
}


/*
 * MAIN FUNCTION
 ****************************************************************************************
 */

int main(void)
{
    int ret;
    uint16_t wId;

    //Must be first in main()
    sys_InitMain();

    //start interrupt handling
    GLOBAL_INT_START();

    drvi_initialize();

    TracerInfo("== CC6801 Start ==\r\n");

    /*******************************/
    /****** Application Start ******/
    /*******************************/

    ret = I2cWordRead(0x0000, &wId);
    if (ret)
        TracerInfo("Error reading chip id %d\r\n", ret);

    TracerInfo("Audio Codec SGTL5000 Chip ID 0x%X\r\n", wId);

    ret = I2cWordWrite(0x0030, 0x40FC);
    if (ret)
        TracerInfo("Error %d, write 0x0030\r\n", ret);

    ret = I2cWordWrite(0x0002, 0x0021);
    if (ret)
        TracerInfo("Error %d, write 0x0002\r\n", ret);

    ret = I2cWordWrite(0x0024, 0x0101);
    if (ret)
        TracerInfo("Error %d, write 0x0024\r\n", ret);

    ret = I2cWordWrite(0x000A, 0x0010);
    if (ret)
        TracerInfo("Error %d, write 0x000A\r\n", ret);

    ret = I2cWordWrite(0x0006, 0x0130);
    if (ret)
        TracerInfo("Error %d, write 0x0006\r\n", ret);

    ret = I2cWordWrite(0x000E, 0x0000);
    if (ret)
        TracerInfo("Error %d, write 0x000E\r\n", ret);

    ret = I2cWordWrite(0x0022, 0x3A3A);
    if (ret)
        TracerInfo("Error %d, write 0x0022\r\n", ret);

    while(1)
    {
    }
}

