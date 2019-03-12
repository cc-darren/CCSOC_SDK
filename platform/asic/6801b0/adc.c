/******************************************************************************
*  Copyright 2019, CloudChip, Inc.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of CloudChip, Inc. (C) 2019
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

int cc6801_AdcVbatt(uint32_t *dwValue)
{
    uint32_t dwAdc = 0;

    dwAdc = (regSCU->bf.adc & 0x7F);

    //Following definitions come from CC6801 analog datasheet (TRAK_ADC7)
    //all calculation value are already multipled by 1000000
    //O_DAC_LOW=0.527 volt, O_DAC_HIGH=1.458 volt
    //ADC_7BIT_MINIMUM=0, ADC_7BIT_MAXIMUM=127
    //interpolation (1.458-0.527)/127 = (volt-0.527)/ADC
    //volt = ADC*0.0073307 + 0.527
    *dwValue = dwAdc*7330+527000;

#if defined(USE_CC6801_ADC) && (USE_CC6801_ADC==TRUE)
  #if (ADC_EXTERNAL_R1_OHM == LIION_BATTERY)
    //Vbat = ADC_voltage / R2 * (R1+R2)
    //     = ADC_voltage / ADC_INTERNAL_R2_OHM * (ADC_EXTERNAL_R1_OHM + ADC_INTERNAL_R2_OHM)
    //     = ADC_voltage / 1000000 * (2000000 + 1000000)
    //     = ADC_voltage / 1000000 * 3000000
    //     = ADC_voltage * (3000000/1000000)
    //     = ADC_voltage * 3
    *dwValue *= 3;
  #elif (ADC_EXTERNAL_R1_OHM == COIN_BATTERY)
    //Vbat = ADC_voltage / R2 * (R1+R2)
    //     = ADC_voltage / ADC_INTERNAL_R2_OHM * (ADC_EXTERNAL_R1_OHM + ADC_INTERNAL_R2_OHM)
    //     = ADC_voltage / 1000000 * (1500000 + 1000000)
    //     = ADC_voltage / 1000000 * 2500000
    //     = ADC_voltage * (2500000/1000000)
    //     = ADC_voltage * 2.5
    //     = ADC_voltage * (2 + 0.5)
    //     = (ADC_voltage * 2) + (ADC_voltage / 2)
    *dwValue = (*dwValue<<1) + (*dwValue>>1);
  #endif
#endif

    return CC_SUCCESS;
}

E_AdcBGRSelect cc6801_AdcBGRSelGet(void)
{
    return (E_AdcBGRSelect)regSCU->bf.bgrSel;
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

    //finally, disable internal resistor divider to avoid reading wrong ADC in buffer
    regSCU->bf.en_ext = 0;
}

void cc6801_AdcEnable(void)
{
    //first, enable internal resistor divider to make voltage stable before reading it
    regSCU->bf.en_ext = 1;

    regSCU->bf.pdBgr = 0;
    regSCU->bf.pdAdc = 0;
}

void cc6801_AdcInit(void)
{
    //ADC read interval should be > 4ms (129*32Khz)

    cc6801_AdcModeSet();
    cc6801_AdcBGRSelSet(ADC_VBR12_OUTPUT_1V208);
    cc6801_AdcDisable();
}
