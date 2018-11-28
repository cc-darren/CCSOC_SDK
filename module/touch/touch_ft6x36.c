/* Copyright (c) 2018 Cloudchip, Inc. All Rights Reserved.
 *
 * The information contained herein is property of Cloudchip, Inc.
 * Terms and conditions of usage are described in detail in CLOUDCHIP
 * STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information.
 * NO WARRANTY of ANY KIND is provided. This heading must NOT be removed
 * from the file.
 */

/******************************************************************************
*  Filename:
*  ---------
*  touch_ft6x36.c
*
*  Project:
*  --------
*  CC6801
*
*  Description:
*  ------------
*  This is touch driver of focal ft6x36 series.
*
*  Author:
*  -------
*  PPP   (cc-pattaya)
*
*===========================================================================
*
******************************************************************************/

/******************************************************************************
Head Block of The File
******************************************************************************/
#include <stdint.h>
//#include "touch_ft6x36.h"
#include "drvi_i2c.h"
#include "drvi_gpio.h"
#include "touch.h"

#define REG00_WORKING_MODE      0x00
#define REG00_FACTORY_MODE      0x04
#define REG80_TOUCH_THRESHOLD   0x04
#define REG85_FILTER_COEFF      0x04
#define REG86_ACTIVE_MODE       0x00
#define REG86_MONITOR_MODE      0x01
#define REG87_TIME_TO_MONITOR   0x0A
#define REG88_ACTIVE_MODE_RR    0x10
#define REG89_MONITOR_MODE_RR   0x19
#define REG91_ROTATE_ANGLE_MIN  0x0A
#define REG92_LR_GESTURE_OFFSET 0x19
#define REG93_UD_GESTURE_OFFSET 0x19
#define REG94_LR_GESTURE_DST    0x19
#define REG95_UD_GESTURE_DST    0x19
#define REG96_ZOOM_GESTURE_DST  0x32
#define REGA4_INT_POLLING       0x00
#define REGA4_INT_TRIGGER       0x01
#define REGA5_POWER_MODE        0x00    //?
#define REGBC_OPERATING_MODE    0x01    //?

#define FINGER_INFO_READ_START  0x01

#if (TOUCH_SUPPORTED_FINGERS == 1)
  #define FINGER_INFO_READ_SIZE  0x08
#elif (TOUCH_SUPPORTED_FINGERS == 2)
  #define FINGER_INFO_READ_SIZE  0x0E
#else
  #error Must define "TOUCH_SUPPORTED_FINGERS"
#endif

/******************************************************************************
Declaration of External Variables & Functions
******************************************************************************/
extern T_fingerInfo fg;

/******************************************************************************
Declaration of data structure
******************************************************************************/
typedef struct
{
    uint8_t a;  //register address
    uint8_t d;  //register default value
    uint16_t padding;   //add padding for struct DWord alignment for cc6801 I2C DMA access
}T_Ft6x36CtrlReg;

__align(4) T_Ft6x36CtrlReg ctrlTable[] =
{
    {0x00,REG00_WORKING_MODE},
    {0x80,REG80_TOUCH_THRESHOLD},
    {0x85,REG85_FILTER_COEFF},
    {0x86,REG86_ACTIVE_MODE},
    {0x87,REG87_TIME_TO_MONITOR},
    {0x88,REG88_ACTIVE_MODE_RR},
    {0x89,REG89_MONITOR_MODE_RR},
    {0x91,REG91_ROTATE_ANGLE_MIN},
    {0x92,REG92_LR_GESTURE_OFFSET},
    {0x93,REG93_UD_GESTURE_OFFSET},
    {0x94,REG94_LR_GESTURE_DST},
    {0x95,REG95_UD_GESTURE_DST},
    {0x96,REG96_ZOOM_GESTURE_DST},
    {0xA4,REGA4_INT_TRIGGER},
    {0xA5,REGA5_POWER_MODE},
    {0xBC,REGBC_OPERATING_MODE}
};
    
/******************************************************************************
Declaration of Global Variables & Functions
******************************************************************************/
volatile bool blFingerDataReady = 0;
T_callback  fpIntCallback = NULL;
/******************************************************************************
Declaration of static Global Variables & Functions
******************************************************************************/
__forceinline int ft6x36_readReg(uint8_t *pTxData, uint16_t wTxLen, uint8_t *pRxData, uint16_t wRxLen)
{
    T_I2cDevice dev =
    {
        .bBusNum    = TOUCH_IF_ID,
        .bAddr      = TOUCH_FT6X36_SLAVE_ADDR,
    };

    return drvi_I2cWriteThenRead(&dev, pTxData, wTxLen, pRxData, wRxLen);
}

__forceinline int ft6x36_writeReg(uint8_t *pTxData, uint16_t wTxLen)
{
    T_I2cDevice dev =
    {
        .bBusNum    = TOUCH_IF_ID,
        .bAddr      = TOUCH_FT6X36_SLAVE_ADDR,
    };

    return drvi_I2cWrite(&dev, pTxData, wTxLen);
}

void ft6x36_IntCallback(void)
{
    blFingerDataReady = TRUE;
    
    //if MMI or other touch application need to schedule I2C reading,
    //call callback to do it
    if(fpIntCallback) fpIntCallback(NULL);
}

__forceinline void ft6x36_FingerDataReadDone(void)
{
    blFingerDataReady = FALSE;
}

__forceinline bool ft6x36_isFingerDataReady(void)
{
    return blFingerDataReady;
}
    
void ft6x36_init(T_callback fpGpioIsrCallback)
{
    //register GPIO as interrupt
    ft6x36_FingerDataReadDone();
    drvi_RequestIrq(TOUCH2D_INT_PIN, ft6x36_IntCallback, IRQ_TYPE_LEVEL_LOW);
    drvi_EnableIrq(TOUCH2D_INT_PIN);
    fpIntCallback = fpGpioIsrCallback;

    //write default value to touch sensor
    for(uint32_t i=0;i<(sizeof(ctrlTable)/sizeof(T_Ft6x36CtrlReg));i++)
    {
        ft6x36_writeReg((uint8_t *)&ctrlTable[i], 2);
    }  
}

void ft6x36_fillFingerInfo(void)
{
    __align(4) uint8_t fingerInfo[FINGER_INFO_READ_SIZE];
    uint8_t bStartAddr = FINGER_INFO_READ_START;
    
    ft6x36_readReg(&bStartAddr, 1, fingerInfo, FINGER_INFO_READ_SIZE);
    
    fg.bTouchNum = fingerInfo[1] & 0x0F;
    fg.bGesture = (E_gesture)fingerInfo[0];
    fg.tF[0].bID = (fingerInfo[4] >> 4);
    fg.tF[0].bState = (E_fState)(fingerInfo[2] >> 6);
    fg.tF[0].wX = (((fingerInfo[2] & 0x0F) << 8) | fingerInfo[3]);
    fg.tF[0].wY = (((fingerInfo[4] & 0x0F) << 8) | fingerInfo[5]);
  #if (TOUCH_SUPPORTED_FINGERS == 2)
    fg.tF[1].bID = (fingerInfo[10] >> 4);
    fg.tF[1].bState = (E_fState)(fingerInfo[8] >> 6);
    fg.tF[1].wX = (((fingerInfo[ 8] & 0x0F) << 8) | fingerInfo[ 9]);
    fg.tF[1].wY = (((fingerInfo[10] & 0x0F) << 8) | fingerInfo[11]);    
  #endif  
}





