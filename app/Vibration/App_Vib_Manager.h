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



#ifndef _APP_VIB_MGR_
#define _APP_VIB_MGR_

/******************************************************************************
 * INCLUDE FILE
 ******************************************************************************/

 /******************************************************************************
 * DEFINITION / CONSTANT / ENUM / TYPE
 ******************************************************************************/
#define VIB_PATTERN_LEN_MAX         (15)

#define VIB_SWITCH_ON_INTERVAL_MAX  (10000)
#define VIB_SWITCH_ON_INTERVAL_MIN  (1)

#define VIB_SWITCH_OFF_INTERVAL_MAX  (10000)
#define VIB_SWITCH_OFF_INTERVAL_MIN  (1)

#define VIB_VARIABLE_INTERVAL_MAX  (10000)
#define VIB_VARIABLE_INTERVAL_MIN  (1)


#define VIB_VAR_LEVEL_LMT_MAX          (100)
#define VIB_VAR_LEVEL_LMT_MIN          (40)

#define VIB_VAR_LEVEL_INTERVAL_MAX         (60)

typedef enum
{
    E_APP_VIB_RET_SUCESS,
    E_APP_VIB_RET_MODE_ERROR,
    E_APP_VIB_RET_EXEC_BEHAVIOR_ERROR,
    E_APP_VIB_RET_PATTERN_NUM_ERROR,
    E_APP_VIB_RET_SWITCH_LIMITED_ERROR,
    E_APP_VIB_RET_EXEC_COUNT_ERROR,
    E_APP_VIB_RET_INTERVAL_ERROR,
    E_APP_VIB_RET_VAR_LEVEL_INTERVAL_ERROR,
    E_APP_VIB_RET_VAR_TYPE_ERROR,
    E_APP_VIB_RET_VAR_LEVEL_LIMIT_ERROR,
    E_APP_VIB_RET_NO_FUNC,
}E_APP_VIB_RESULT;

typedef enum
{
    E_APP_VIB_OPT_NORMAL = 0x01,
    E_APP_VIB_OPT_ASCENDING,
    E_APP_VIB_OPT_DESCENDING,
    E_APP_VIB_MODE_INVAILD = 0xff,
}E_APP_VIB_OPERATORS_MODE;

typedef enum
{
    E_APP_VIB_EXEC_ONESHOT =0x01,
    E_APP_VIB_EXEC_REPEAT,
    E_APP_VIB_EXEC_INVAILD =0xff
}E_APP_VIB_EXEC_BEHAVIOR;

typedef enum
{
    E_APP_SCHED_VIB_TIMER_OPT_TIMEOUT = 0x01,
    E_APP_SCHED_VIB_TIMER_EXECINTERVAL_TIMEOUT,
    E_APP_SCHED_VIB_TIMER_DUTYCYCLECHANGE_TIMEOUT,
} E_APP_SCHED_VIB_TIMER_TIMEOUT;

typedef enum
{
    E_APP_VIB_VAR_INFINITE   = 0x01,
    E_APP_VIB_VAR_REPEAT,
    E_APP_VIB_VAR_INVAILD,

}E_APP_VIB_VAR_TYPE;

typedef struct
{
    uint16_t wVibON_ms;
    uint16_t wVibOFF_ms;

}S_APP_VIB_VIB_SWITCH;

typedef struct
{
    uint8_t                     bRepeatCount;
    S_APP_VIB_VIB_SWITCH        eVibOnOrOff;
}S_APP_VIB_PATTERN_CFG_UNIT;

typedef struct
{
    uint16_t                    wVar_TimeInterval;
    uint8_t                     bVar_LevelInterval;
    E_APP_VIB_VAR_TYPE          eVar_Type;
    int8_t                      bVar_StartLevel;
}E_APP_VIB_VARIABLE_ATTR;


typedef struct
{
    int8_t                      bNumOfPattern;
    S_APP_VIB_PATTERN_CFG_UNIT      eVibPattern[VIB_PATTERN_LEN_MAX];
}S_APP_VIB_ATTR;

typedef struct
{
    uint16_t                    wOpt_Ms;
    E_APP_VIB_OPERATORS_MODE    eExecMode;
    S_APP_VIB_ATTR              stAttr;
    E_APP_VIB_EXEC_BEHAVIOR     eBehavior;
    E_APP_VIB_VARIABLE_ATTR     stVarAttr;
}S_APP_VIB_PATTERN_CFG;


/******************************************************************************
 * EXPORTED FUNCTION
 ******************************************************************************/

/******************************************************************************
 * [FUNCTION] APP_VibMgr_Init
 *     Initialize VIB MGR
 * [ARGUMENT] None.
 * [RETURN  ] None.
 ******************************************************************************/
extern void APP_VibMgr_Init(void);

/******************************************************************************
 * LOCAL FUNCTION > _Vib_Mgr_RegEventData
 *     Set an pattern of vibration manager. Vibration manager will handle the
 *     pattern.
 * [ARGUMENT]
 *     <in> S_APP_VIB_PATTERN_CFG: a pointer to the struct where below info. specified,
 *            .wOpt_Ms          : Total operation time. Module will auto calculate
 *                                the time form vibration ON / OFF time.
 *            .eExecMode        : ID of the operation mode
 *            .stAttr           : Attribute of pattern number and vibration
 *                                ON / OFF time interval
 *               .bNumOfPattern : Set total pattern numbers for vibration ON / OFF
 *               .eVibPattern   : Set ON / OFF time and execute count.
 *                 .bRepeatCount    : Set how many time to operation this ON / OFF pattern
 *                 .wVibON_ms   : Set vibration "ON" time interval
 *                 .wVibOFF_ms  : Set vibration "OFF" time interval
 *            .eBehavior        : ID of the exec behavior.
 *                                Execute "ONESHOT" or "REPEAT" to operation time end
 *            .stVarAttr        : Vibration level will increment or decreasement.
 *                                Only support E_APP_VIB_OPT_ASCENDING and E_APP_VIB_OPT_DESCENDING.
 *               .wVar_TimeInterval : Set time interval. Module will adjust vibration level
 *               .bVar_LevelInterval: adjust this vibration level interval from Time interval timeout.
 *               .eVar_Type         : define on E_APP_VIB_VAR_TYPE.
 *                                    "E_APP_VIB_VAR_INFINITE" Vibration level to limit will module keep it.
 *                                    "E_APP_VIB_VAR_REPEAT" Vibration level to limit will repeat to start level
 *               .bVar_StartLevel   : Set the start vibration level
 * [HIGHLIGHT]
 *     1. Vibration ON / OFF time
 *         maximum set on "VIB_SWITCH_ON_INTERVAL_MAX",
 *         minimum, set on "VIB_SWITCH_ON_INTERVAL_MIN"
 *     2. Var_timeInterval maximum set on "VIB_VARIABLE_INTERVAL_MAX",
 *                         minimum set on "VIB_VARIABLE_INTERVAL_MIN"
 *     4. bVar_StartLevel  maximum varibable level is  "VIB_VAR_LEVEL_LMT_MAX"
 *                         minimum variable level is "VIB_VAR_LEVEL_LMT_MIN"
 *     5. bVar_LevelInterval maximum is "VIB_VAR_LEVEL_INTERVAL_MAX"
 *                           minimum will keep the original level on it.
 *
 * [RETURN  ]
 *     The return code to specify the result of the function call, listed of enum "VIB_SWITCH_ON_INTERVAL_MIN"
 *     E_APP_VIB_RESULT
 ******************************************************************************/
extern E_APP_VIB_RESULT APP_VibMgr_StartService(S_APP_VIB_PATTERN_CFG *_EventData);


/******************************************************************************
 * [FUNCTION] APP_VibMgr_StopService
 *     Call this scheduler to stop all events and reset all parameters.
 * [ARGUMENT] None.
 * [RETURN  ] None.
 ******************************************************************************/
extern void APP_VibMgr_StopService(void);

/******************************************************************************
 * [FUNCTION] APP_VibMgr_RegEventHandler
 *     Register the event handler of a module.
 * [ARGUMENT]
 *     <in> ptAppEvent: a pointer to the struct where below info. specified,
 *            ._stEvent: ID of a module to handle events
 * [RETURN  ] None
 ******************************************************************************/
extern void APP_VibMgr_RegEventHandler(S_VenusEvent *_stEvent);

#endif //_CC_PWM_SRV_
