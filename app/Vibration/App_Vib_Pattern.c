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


/******************************************************************************
 * INCLUDE FILE
 ******************************************************************************/
#include "App_Vib_pattern.h"
#include "App_Vib_Manager.h"

S_APP_VIB_PATTERN_CFG  s_tVibExample =
{
    .eExecMode = E_APP_VIB_OPT_NORMAL,
    .stAttr.bNumOfPattern = 3,
    .stAttr.eVibPattern[0].bRepeatCount = 1,
    .stAttr.eVibPattern[0].eVibOnOrOff.wVibON_ms = 200,
    .stAttr.eVibPattern[0].eVibOnOrOff.wVibOFF_ms = 200,

    .stAttr.eVibPattern[1].bRepeatCount = 1,
    .stAttr.eVibPattern[1].eVibOnOrOff.wVibON_ms = 200,
    .stAttr.eVibPattern[1].eVibOnOrOff.wVibOFF_ms = 200,

    .stAttr.eVibPattern[2].bRepeatCount = 1,
    .stAttr.eVibPattern[2].eVibOnOrOff.wVibON_ms = 200,
    .stAttr.eVibPattern[2].eVibOnOrOff.wVibOFF_ms = 2000,

    .eBehavior = E_APP_VIB_EXEC_ONESHOT,
};

S_APP_VIB_PATTERN_CFG s_tVib100msAndRepeat =
{
    .eExecMode = E_APP_VIB_OPT_NORMAL,
    .stAttr.bNumOfPattern = 1,
    .stAttr.eVibPattern[0].bRepeatCount = 1,
    .stAttr.eVibPattern[0].eVibOnOrOff.wVibON_ms = 300,
    .stAttr.eVibPattern[0].eVibOnOrOff.wVibOFF_ms =300,
    .eBehavior = E_APP_VIB_EXEC_REPEAT,
};


S_APP_VIB_PATTERN_CFG s_tVibAscendingAndRepeat =
{
    .eExecMode = E_APP_VIB_OPT_ASCENDING,
    .stAttr.bNumOfPattern = 3,
    .stAttr.eVibPattern[0].bRepeatCount = 1,
    .stAttr.eVibPattern[0].eVibOnOrOff.wVibON_ms = 200,
    .stAttr.eVibPattern[0].eVibOnOrOff.wVibOFF_ms = 200,

    .stAttr.eVibPattern[1].bRepeatCount = 1,
    .stAttr.eVibPattern[1].eVibOnOrOff.wVibON_ms = 200,
    .stAttr.eVibPattern[1].eVibOnOrOff.wVibOFF_ms = 200,

    .stAttr.eVibPattern[2].bRepeatCount = 1,
    .stAttr.eVibPattern[2].eVibOnOrOff.wVibON_ms = 200,
    .stAttr.eVibPattern[2].eVibOnOrOff.wVibOFF_ms = 2000,

    .eBehavior = E_APP_VIB_EXEC_REPEAT,
    .stVarAttr.wVar_TimeInterval = 400,
    .stVarAttr.eVar_Type = E_APP_VIB_VAR_REPEAT,
    .stVarAttr.bVar_StartLevel = 100,
    .stVarAttr.bVar_LevelInterval = 20,


};


S_APP_VIB_PATTERN_CFG s_tVibDescendingAndInfinite =
{
    .eExecMode = E_APP_VIB_OPT_DESCENDING,
    .stAttr.bNumOfPattern = 3,
    .stAttr.eVibPattern[0].bRepeatCount = 1,
    .stAttr.eVibPattern[0].bRepeatCount = 1,
    .stAttr.eVibPattern[0].eVibOnOrOff.wVibON_ms = 200,
    .stAttr.eVibPattern[0].eVibOnOrOff.wVibOFF_ms = 200,

    .stAttr.eVibPattern[1].bRepeatCount = 1,
    .stAttr.eVibPattern[1].eVibOnOrOff.wVibON_ms = 200,
    .stAttr.eVibPattern[1].eVibOnOrOff.wVibOFF_ms = 200,

    .stAttr.eVibPattern[2].bRepeatCount = 1,
    .stAttr.eVibPattern[2].eVibOnOrOff.wVibON_ms = 200,
    .stAttr.eVibPattern[2].eVibOnOrOff.wVibOFF_ms = 2000,


    .eBehavior = E_APP_VIB_EXEC_REPEAT,
    .stVarAttr.wVar_TimeInterval = 400,
    .stVarAttr.eVar_Type = E_APP_VIB_VAR_INFINITE,
    .stVarAttr.bVar_StartLevel = 100,
    .stVarAttr.bVar_LevelInterval = 20,
};
