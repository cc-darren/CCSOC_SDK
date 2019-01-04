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
#include "altitude.h"
uint32_t gSeaLevelPerssure=101325;
int32_t gPerssureOffset=0;


// table for altitude, matrix[0] means: "the pressure at sea level" - "the pressure at 200*0m"
//                     matrix[6] means: "the pressure at sea level" - "the pressure at 200*6m"
// altitude range is 0~10000m, pressure unit is pa
const uint32_t M_ALTDIF[51] = {     0,  2375,  4715,  7005,  9245, 11455, 13605, 15725,
                                17805, 19835, 21825, 23785, 25695, 27575, 29415, 31215,
                                32985, 34705, 36405, 38065, 39685, 41275, 42835, 44355,
                                45845, 47305, 48735, 50135, 51495, 52835, 54145, 55425,
                                56675, 57895, 59095, 60265, 61405, 62525, 63615, 64685,
                                65725, 66745, 67735, 68705, 69655, 70585, 71485, 72365,
                                73225, 74065, 74885
                               };



#if (defined PRESSURE_INT_PIN) && (PRESSURE_INT_PIN)
uint8_t ALT_StartDevice(fpGpiIrqHandler tCB)
{
    //init pressure sensor
    if (CC_SUCCESS!=PRESSURE_Init()) {
        return CC_ERROR_NOT_FOUND;
    }
    drvi_RequestIrq(PRESSURE_INT_PIN, tCB, IRQ_TYPE_LEVEL_HIGH);
    drvi_EnableIrq(PRESSURE_INT_PIN);
    //enable interrupt and data
    PRESSURE_Enable();

    return CC_SUCCESS;
}
#else
uint8_t ALT_StartDevice(void)
{
    //init pressure sensor
    if (CC_SUCCESS!=PRESSURE_Init()) {
        return CC_ERROR_NOT_FOUND;
    }
    //enable interrupt and data
    PRESSURE_Enable();

    return CC_SUCCESS;
}

BOOL ALT_CheckFIFORdy(void)
{
    uint8_t isRdy;
    PRESSURE_CheckFIFORdy(&isRdy);

    if (isRdy)
        return TRUE;
    else
        return FALSE;
}

#endif

void ALT_StopDevice(void)
{
    #if (defined PRESSURE_INT_PIN) && (PRESSURE_INT_PIN)
    drvi_DisableIrq(PRESSURE_INT_PIN);
    #endif
    PRESSURE_Disable();
}

uint32_t ALT_CalculateAltitdue(uint32_t dwInPressure)
{
    uint32_t i,dwVal;
    uint32_t dwAltitude;

    dwAltitude = 0;
    if (gSeaLevelPerssure>dwInPressure) {
        dwVal = gSeaLevelPerssure - dwInPressure;
        /* tranformation herer */
        for (i=1;i<51;i++) {
            if (dwVal>=M_ALTDIF[i]) {
                dwAltitude += 200;
            } else {
                break;
            }
        }
        dwAltitude = dwAltitude + 200*(dwVal-M_ALTDIF[i-1])/(M_ALTDIF[i]-M_ALTDIF[i-1]);
    }
    return dwAltitude;
}

//get atmosphere in pa
uint32_t ALT_GetAtmosPressure(void)
{
    uint32_t dwRawPressure;
    PRESSURE_GetRawPressure(&dwRawPressure);
    dwRawPressure =(uint32_t) ((int32_t)dwRawPressure + gPerssureOffset);
    return (dwRawPressure);


}

void ALT_CalibrateWithSeaLevelPressure(uint32_t dwPressure)
{
    gSeaLevelPerssure = dwPressure;
}


void ALT_CalibrateWithCurrentPressure(int32_t dwCurPressure)
{
    uint32_t dwMeasurePressure;
    PRESSURE_GetRawPressure(&dwMeasurePressure);
    gPerssureOffset = (int32_t)(dwCurPressure-dwMeasurePressure);
}

void ALT_CalibrateWithCurrentHeight(uint32_t dwCurHeight)
{
    uint32_t i,dwpos,dwAsumePressureDiff;
    uint32_t dwMeasurePressure;
    i = dwCurHeight/200;
    dwpos = dwCurHeight%200;
    dwAsumePressureDiff = (dwpos*(M_ALTDIF[i+1]-M_ALTDIF[i])/200) + M_ALTDIF[i];
    dwMeasurePressure  = ALT_GetAtmosPressure();

    gSeaLevelPerssure = dwMeasurePressure+dwAsumePressureDiff;
}

void ALT_CalibrateWithCurrentPressureAndHeight(uint32_t dwCurPressure,uint32_t dwCurHeight)
{

    uint32_t dwMeasurePressure;
    PRESSURE_GetRawPressure(&dwMeasurePressure);
    gPerssureOffset = (int32_t)(dwCurPressure-dwMeasurePressure);

    uint32_t i,dwpos,dwAsumePressureDiff;
    i = dwCurHeight/200;
    dwpos = dwCurHeight%200;
    dwAsumePressureDiff = (dwpos*(M_ALTDIF[i+1]-M_ALTDIF[i])/200) + M_ALTDIF[i];

    gSeaLevelPerssure = dwCurPressure+dwAsumePressureDiff;
}
