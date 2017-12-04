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
*  adc_mcp3421.c
*
*  Project:
*  --------
*  cc6801
*
*  Description:
*  ------------
*  Driver for the Microchip mcp3421 chip
*  This driver exports the value of analog input voltage to application, the
*  voltage unit is nV.
*
*  Author:
*  -------
*  CloudChip
*
*===========================================================================/
*  20171206 Blake initial version
******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "project.h"

#if (MODULE_ADC == ADC_MCP_MCP3421)
/* Imported macro ------------------------------------------------------------*/
#if (ADC_IF_TYPE == IF_I2C)
    #include "drvi_i2c.h"
    #define MCP3421_IF_Read(rbuf,rlen)                          drvi_I2cRead(ADC_IF_ID,rbuf,rlen)
    #define MCP3421_IF_Write(buf,len)                           drvi_I2cWrite(ADC_IF_ID,buf,len)
#else
    #define MCP3421_IF_Read(rbuf,rlen)
    #define MCP3421_IF_Write(buf,len)
#endif

/* Private macro -------------------------------------------------------------*/
#define MCP3421_IF_ADDRESS 0x68

/* Masks */
#define MCP3421_CHANNEL_MASK    0x60
#define MCP3421_PGA_MASK    0x03
#define MCP3421_SRATE_MASK  0x0C
#define MCP3421_SRATE_240   0x0
#define MCP3421_SRATE_60    0x1
#define MCP3421_SRATE_15    0x2
#define MCP3421_SRATE_3 0x3
#define MCP3421_PGA_1   0
#define MCP3421_PGA_2   1
#define MCP3421_PGA_4   2
#define MCP3421_PGA_8   3
#define MCP3421_CONT_SAMPLING   0x10

#define MCP3421_CHANNEL(config) (((config) & MCP3421_CHANNEL_MASK) >> 5)
#define MCP3421_PGA(config) ((config) & MCP3421_PGA_MASK)
#define MCP3421_SAMPLE_RATE(config) (((config) & MCP3421_SRATE_MASK) >> 2)

#define MCP3421_CHANNEL_VALUE(value) (((value) << 5) & MCP3421_CHANNEL_MASK)
#define MCP3421_PGA_VALUE(value) ((value) & MCP3421_PGA_MASK)
#define MCP3421_SAMPLE_RATE_VALUE(value) ((value << 2) & MCP3421_SRATE_MASK)

static const int g_iMcp3421Scales[4][4] = {
    { 1000000, 500000, 250000, 125000 },
    { 250000,  125000, 62500,  31250  },
    { 62500,   31250,  15625,  7812   },
    { 15625,   7812,   3906,   1953   } };

/* sample rates to integer conversion table */
static const int g_iMcp3421SampleRates[4] = {
    [MCP3421_SRATE_240] = 240,
    [MCP3421_SRATE_60] = 60,
    [MCP3421_SRATE_15] = 15,
    [MCP3421_SRATE_3] = 3 };

/* sample rates to sign extension table */
static const int g_iMcp3421SignExtend[4] = {
    [MCP3421_SRATE_240] = 11,
    [MCP3421_SRATE_60] = 13,
    [MCP3421_SRATE_15] = 15,
    [MCP3421_SRATE_3] = 17 };

/* Client data (each client gets its own) */
struct S_Mcp3421Dev {
    uint8_t bId;
    uint8_t bConfig;
    uint8_t bPga;
};

struct S_Mcp3421Dev g_sAdcDev;

static inline int32_t SignExtend32(uint32_t dwValue, int iIndex)
{
    uint8_t bShift = 31 - iIndex;
    return (int32_t)(dwValue << bShift) >> bShift;
}

static int MCP3421UpdateConfig(uint8_t bNewConfig)
{
    int iRet;

    iRet = MCP3421_IF_Write(&bNewConfig, 1);
    if (iRet > 0) {
        g_sAdcDev.bConfig = bNewConfig;
        iRet = 0;
    }

    return iRet;
}

static int MCP3421Read(int *piValue, uint8_t *pbConfig)
{
    int iRet = 0;
    uint8_t bSampleRate = MCP3421_SAMPLE_RATE(g_sAdcDev.bConfig);
    uint8_t baBuf[4] = {0, 0, 0, 0};
    uint32_t dwTemp;

    if (bSampleRate == MCP3421_SRATE_3) {
        iRet = MCP3421_IF_Read(baBuf, 4);
        dwTemp = baBuf[0] << 16 | baBuf[1] << 8 | baBuf[2];
        *pbConfig = baBuf[3];
    } else {
        iRet = MCP3421_IF_Read(baBuf, 3);
        dwTemp = baBuf[0] << 8 | baBuf[1];
        *pbConfig = baBuf[2];
    }

    *piValue = SignExtend32(dwTemp, g_iMcp3421SignExtend[bSampleRate]);

    return iRet;
}

static int MCP3421ReadChannel(int *piValue)
{
    uint8_t bConfig;

    return MCP3421Read(piValue, &bConfig);
}

int MCP3421ReadSacle(int *piVal)
{
    uint8_t bSampleRate = MCP3421_SAMPLE_RATE(g_sAdcDev.bConfig);
    uint8_t bPga      = MCP3421_PGA(g_sAdcDev.bConfig);

    *piVal = g_iMcp3421Scales[bSampleRate][bPga];

    return CC_SUCCESS;
}

int MCP3421ReadSampleRate(int *piVal)
{
    *piVal = g_iMcp3421SampleRates[MCP3421_SAMPLE_RATE(g_sAdcDev.bConfig)];
    return CC_SUCCESS;
}

int MCP3421ReadRaw(int *piVal)
{
    int iErr;

    iErr = MCP3421ReadChannel(piVal);
    if (iErr < 0)
        return CC_ERROR_INTERNAL;

    return CC_SUCCESS;
}

int MCP3421WriteSampleFreq(int iVal)
{
    uint8_t bTemp;
    uint8_t bConfig = g_sAdcDev.bConfig;

    switch (iVal) {
        case 240:
            bTemp = MCP3421_SRATE_240;
            break;
        case 60:
            bTemp = MCP3421_SRATE_60;
            break;
        case 15:
            bTemp = MCP3421_SRATE_15;
            break;
        case 3:
            bTemp = MCP3421_SRATE_3;
            break;
        default:
            return CC_ERROR_INTERNAL;
    }

    bConfig &= ~MCP3421_CHANNEL_MASK;
    bConfig &= ~MCP3421_SRATE_MASK;
    bConfig |= MCP3421_SAMPLE_RATE_VALUE(bTemp);

    return MCP3421UpdateConfig(bConfig);
}

int MCP3421WritePga(int iVal)
{
    uint8_t bConfig = g_sAdcDev.bConfig;
    uint8_t bSampleRate = MCP3421_SAMPLE_RATE(bConfig);
    uint8_t bIdx;

    for (bIdx = 0; bIdx < ARRAY_SIZE(g_iMcp3421Scales[0]); bIdx++) {
        if (iVal == g_iMcp3421Scales[bSampleRate][bIdx]) {
            g_sAdcDev.bPga = bIdx;

            bConfig &= ~MCP3421_CHANNEL_MASK;
            bConfig |= MCP3421_CHANNEL_VALUE(0);
            bConfig &= ~MCP3421_PGA_MASK;
            bConfig |= MCP3421_PGA_VALUE(g_sAdcDev.bPga);

            return MCP3421UpdateConfig(bConfig);
        }
    }

    return CC_ERROR_INTERNAL;
}

int MCP3421Init(void)
{
    uint8_t bConfig;

    #if (ADC_IF_TYPE == IF_I2C)
    T_I2cDevice AdcDevice = {
       .bBusNum          = ADC_IF_TYPE,
       .bAddr            = MCP3421_IF_ADDRESS,
    };

    drvi_I2cDeviceRegister(&AdcDevice);
    #endif

    g_sAdcDev.bId = AdcDevice.bBusNum;

    /* meaningful default configuration */
    bConfig = (MCP3421_CONT_SAMPLING
        | MCP3421_CHANNEL_VALUE(0)
        | MCP3421_PGA_VALUE(MCP3421_PGA_1)
        | MCP3421_SAMPLE_RATE_VALUE(MCP3421_SRATE_240));
    MCP3421UpdateConfig(bConfig);

    return 0;
}

#endif //#if (MODULE_ADC == ADC_MCP_MCP3421)
