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
*  adc.c
*
*  Project:
*  --------
*  cc6801
*
*  Description:
*  ------------
*  This is ADC driver
*
*  Author:
*  -------
*  CloudChip
*
*===========================================================================/
*  20170206 PAT initial version
******************************************************************************/
#include "cc6801_reg.h"

#include "adc.h"

static uint32_t g_dwaAdcOutputTable[] =
{
     527000,  534000,  542000,  549000,  556000,  564000,  571000,  578000,
     586000,  593000,  600000,  608000,  615000,  622000,  630000,  637000,
     644000,  652000,  659000,  666000,  674000,  681000,  688000,  696000,
     703000,  710000,  718000,  725000,  732000,  740000,  747000,  754000,
     762000,  769000,  776000,  784000,  791000,  798000,  806000,  813000,
     820000,  828000,  835000,  842000,  850000,  857000,  864000,  872000,
     879000,  886000,  894000,  901000,  908000,  916000,  923000,  930000,
     938000,  945000,  952000,  960000,  967000,  974000,  982000,  989000,
     996000, 1003000, 1011000, 1018000, 1025000, 1033000, 1040000, 1047000,
    1055000, 1062000, 1069000, 1077000, 1084000, 1091000, 1099000, 1106000,
    1113000, 1121000, 1128000, 1135000, 1143000, 1150000, 1157000, 1165000,
    1172000, 1179000, 1187000, 1194000, 1201000, 1209000, 1216000, 1223000,
    1231000, 1238000, 1245000, 1253000, 1260000, 1267000, 1275000, 1282000,
    1289000, 1297000, 1304000, 1311000, 1319000, 1326000, 1333000, 1341000,
    1348000, 1355000, 1363000, 1370000, 1377000, 1385000, 1392000, 1399000,
    1407000, 1414000, 1421000, 1429000, 1436000, 1443000, 1451000, 1458000
};

typedef enum
{
    ADC_VBR12_OUTPUT_1V208 = 0,
    ADC_VBR12_OUTPUT_1V222,
    ADC_VBR12_OUTPUT_1V237,
    ADC_VBR12_OUTPUT_1V252,
    ADC_VBR12_OUTPUT_1V150,
    ADC_VBR12_OUTPUT_1V165,
    ADC_VBR12_OUTPUT_1V180,
    ADC_VBR12_OUTPUT_1V193,
}E_AdcBGRSelect;

typedef struct
{
    uint32_t dwAdcMin;
    uint32_t dwAdcMax;
    uint32_t dwBatMin;
    uint32_t dwBatMax;
    uint32_t dwSlope;
    int16_t  iOffset;
} T_AdcParameter;

T_AdcParameter cc6801Adc =
{
    .dwAdcMin = 654800,  //uV
    .dwAdcMax = 1386000, //uV
    .dwBatMin = 1700000, //uV
    .dwBatMax = 3600000, //uV
};

cc6801_AdcCB g_fpAdcCB;

int cc6801_AdcSample(uint32_t *dwValue)
{
    uint32_t dwAdc = 0;

    dwAdc = regSCU->bf.adc;

    if (dwAdc > 0x7F)
        return CC_ERROR_INVALID_DATA;

    *dwValue = g_dwaAdcOutputTable[dwAdc];

    return CC_SUCCESS;
}

uint8_t cc6801_AdcBGRSelGet(void)
{
    return regSCU->bf.bgrSel;
}

void cc6801_AdcBGRSelSet(E_AdcBGRSelect bVBR12Out)
{
    regSCU->bf.bgrSel = (bVBR12Out & 0x07);
}

void cc6801_AdcModeSet(void)
{
    regSCU->bf.adc_temp = 0; //ADC Mode
}

void cc6801_AdcDisable(void)
{
    regSCU->bf.pdBgr = 1;
    regSCU->bf.pdAdc = 1;
}

void cc6801_AdcEnable(void)
{
    regSCU->bf.pdBgr = 0;
    regSCU->bf.pdAdc = 0;
}

uint32_t cc6801_Adc2BatVol(uint32_t dwAdcVol)
{
    return ((uint64_t)(dwAdcVol * cc6801Adc.dwSlope) / 1000) + cc6801Adc.iOffset;
}

void cc6801_AdcInit(cc6801_AdcCB fpAdcCB)
{
    cc6801Adc.dwSlope = ((uint64_t)((cc6801Adc.dwBatMax - cc6801Adc.dwBatMin) * 1000) / (cc6801Adc.dwAdcMax - cc6801Adc.dwAdcMin));
    cc6801Adc.iOffset = cc6801Adc.dwBatMin - ((uint64_t)(cc6801Adc.dwSlope * cc6801Adc.dwAdcMin) / 1000);

    g_fpAdcCB = fpAdcCB;
    cc6801_AdcModeSet();
    cc6801_AdcEnable();

    //TODO: Create ADC timer here
    //Timer period should be > 4ms (129*32Khz)

}
