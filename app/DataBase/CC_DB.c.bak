#ifdef DB_EN

#include "project.h"
#include "CC_DB.h"
#include "fds.h"
#include "fstorage.h"
#include <stdint.h>
#include "apptime.h"
#include "CC_AppSrvc_HeartRate.h"
#include "tracer.h"
#include "htpt_task.h"
#include "clock.h"
#include "string.h"
#include "htpt_task.h"

#define DB_MAGIC_CODE        0xA5804E37
#define RECID_RETURN(_id)    (0x0000|(_id))
#define GET_BIT(bit)    ((uint32_t)(0x00000001 << bit))

extern void CC_MainGet_CurrentTime(app_date_time_t *_stCurTime);
extern void CC_GetStaticCalibrationInfo(db_sys_static_gyro_offset_t *pData);
extern void CC_SetStaticCalibrationInfo(db_sys_static_gyro_offset_t *pData);
extern bool CC_Ble_Check_Is_Bonded(void);
extern void CC_DB_Force_Execute_Init(uint8_t init_type);
extern uint8_t CC_DB_Check_Init_Done(void);
//extern bool CC_Ble_Check_Is_Bond_Locked(void);


static volatile uint8_t _bWrite_Flag=0;
static volatile uint8_t _bUpdate_Flag=0;
static volatile uint8_t _bdeleteRec_Flag =0;
static volatile uint8_t _bdelete_Flag =0;
static volatile uint8_t _bgc_Flag =0;
static volatile uint32_t _wDBSystem_Save_Flag = 0;
static volatile uint8_t _bIsDBFull = 0;
S_DB_FILE_ID s_tDB_FileId;
S_DB_REC_ID s_tDB_RecId;
S_DB_REC_Num s_tDB_RecNum;


uint8_t CC_DB_Check_DB_Full(void)
{
    return _bIsDBFull;
}

static void _DB_Set_DB_Full(void)
{
    _bIsDBFull = 1;
}

static void _DB_Clr_DB_Full(void)
{
    _bIsDBFull = 0;
}

static void _DB_Fds_Evt_Handler(fds_evt_t const * const p_fds_evt)
{
    switch (p_fds_evt->id)
    {
        case FDS_EVT_INIT:
        if (p_fds_evt->result != FDS_SUCCESS)
        {
            // Initialization failed.
        }
        break;
        case FDS_EVT_WRITE:
        if (p_fds_evt->result == FDS_SUCCESS)
        {   
            _bWrite_Flag=1;
        }
        break;
        case FDS_EVT_UPDATE:
        if (p_fds_evt->result == FDS_SUCCESS)
        {
                _bUpdate_Flag=1;
        }
        break;
        case FDS_EVT_GC:
        if (p_fds_evt->result == FDS_SUCCESS)
        {
            _bgc_Flag = 1;
            //TracerInfo("[FDS] GC successfully\r\n");
        }            
        break;

        case FDS_EVT_DEL_RECORD:
        if (p_fds_evt->result == FDS_SUCCESS)
        {
            _bdeleteRec_Flag = 1;
            //TracerInfo("[FDS] File Rec deleted successfully\r\n");
        }
        break;
        
        case FDS_EVT_DEL_FILE:
        if (p_fds_evt->result == FDS_SUCCESS)
        {
            _bdelete_Flag = 1;
            //TracerInfo("[FDS] File deleted successfully\n");
        }    
        break;
        
        default:
        break;
    }
}

#if 0
void  _DB_FlieID_Init(void)
{
    memset(&s_tDB_FileId,0xff,sizeof(S_DB_FILE_ID));
}

void _DB_RecID_Init(void)
{
    memset(&s_tDB_RecId,0x00,sizeof(S_DB_REC_ID));
    memset(&s_tDB_RecNum,0x00,sizeof(S_DB_REC_ID));
}
#endif

void _DB_Update_ID_Info(uint8_t _bDayofWeek)
{

    s_tDB_FileId.wPedFileID = FILE_ID_PED_SUN | RECID_RETURN(_bDayofWeek);
    s_tDB_FileId.wHrmFileID = FILE_ID_HRM_SUN | RECID_RETURN(_bDayofWeek);
    s_tDB_FileId.wSleepFileID = FILE_ID_SLEEP_SUN | RECID_RETURN(_bDayofWeek);
    s_tDB_FileId.wSwimFileID = FILE_ID_SWIM_SUN | RECID_RETURN(_bDayofWeek);
#ifdef BatteryLog_EN
    s_tDB_FileId.wMixLogFileID = FILE_MixLog_ID_SUN | RECID_RETURN(_bDayofWeek);;
#endif            
  
}

uint16_t _DB_FileID_Get(eDB_Content_Op_t _eOpCode)
{
    uint16_t _wRetFileId;
    switch (_eOpCode)
    {
        case eSystem:
            ;
            break;
        case ePed:
            _wRetFileId = s_tDB_FileId.wPedFileID;
            break;
        case eHrm:
            _wRetFileId = s_tDB_FileId.wHrmFileID;
            break;
        case eSleep:
            _wRetFileId = s_tDB_FileId.wSleepFileID;
            break;
        case eSwim:
            _wRetFileId = s_tDB_FileId.wSwimFileID;
            break;
#ifdef BatteryLog_EN
        case eMixLog:
            _wRetFileId = s_tDB_FileId.wMixLogFileID;
            break;
#endif
        default:
            //invaild data;
            break;
    }
    return _wRetFileId;
}


#if 0
uint16_t _DB_RecID_Update(eDB_Content_Op_t _eOpCode) // no use
{
    uint16_t _wTmpRecId;
    switch (_eOpCode)
    {
        case eSystem:
            _wTmpRecId = s_tDB_RecId.wSystemRecID;
            break;
        case ePed:
            _wTmpRecId = ++s_tDB_RecId.wPedRecID;
            break;
        case eHrm:
            _wTmpRecId = ++s_tDB_RecId.wHrmRecID;
            break;
        case eSleep:
            _wTmpRecId = ++s_tDB_RecId.wSleepRecID;
            break;
        case eSwim:
            _wTmpRecId = ++s_tDB_RecId.wSwimRecID;
            break;
#ifdef BatteryLog_EN
        case eMixLog:
            _wTmpRecId = ++s_tDB_RecId.wMixLogRecID;
            break;
#endif    
        default:
            //invaild data;
            _wTmpRecId = 0;
            break;
    }
    return _wTmpRecId;
}
#endif

static ret_code_t _DB_Save_PedData(eDB_Content_Op_t _eOpCode,uint32_t *pWData,uint16_t _bWLen)
{
    ret_code_t ret;
    uint16_t _wFileId = _DB_FileID_Get(_eOpCode);
    uint16_t _wRecId;
    

    if(FDS_SUCCESS == (ret = CC_Fds_Read(_wFileId, REC_ID_RecordNum,
         &s_tDB_RecNum.wPedRecNum, &s_tDB_RecNum.wPedRecLen)))        
    {

         _wRecId = s_tDB_RecNum.wPedRecNum + 2; // offset:2


         TracerInfo(">>>save_rec_num = %d\r\n", _wRecId); 

         
         if (FDS_SUCCESS == (ret = CC_Fds_Write(_wFileId,_wRecId,pWData,_bWLen)))
         {    
             ++s_tDB_RecNum.wPedRecNum;
                               
             ret = CC_Fds_Update(_wFileId,REC_ID_RecordNum, &s_tDB_RecNum.wPedRecNum,sizeof(uint32_t)/4);
         }
         else
         {
             TracerInfo("_DB_Save_PedData  Write Data Fail \r\n");   
         }         
    }     


    if (ret == FDS_ERR_NOT_FOUND) 
    {
         s_tDB_RecNum.wPedRecNum = 1;
              
         ret = CC_Fds_Write(_wFileId,REC_ID_RecordNum, &s_tDB_RecNum.wPedRecNum,sizeof(uint32_t)/4);
    } 

    
    return ret;      

}


static ret_code_t _DB_Save_HrmData(eDB_Content_Op_t _eOpCode,uint32_t *pWData,uint16_t _bWLen)
{
    ret_code_t ret;

    uint16_t _wFileId = _DB_FileID_Get(_eOpCode); 
    uint16_t _wRecId;
    
    if(FDS_SUCCESS == (ret = CC_Fds_Read(_wFileId, REC_ID_RecordNum,
         &s_tDB_RecNum.wHrmRecNum, &s_tDB_RecNum.wHrmRecLen)))        
    {

         _wRecId = s_tDB_RecNum.wHrmRecNum + 2; // offset:2


         TracerInfo(">>>save_rec_num = %d\r\n", _wRecId); 

         
         if (FDS_SUCCESS == (ret = CC_Fds_Write(_wFileId,_wRecId,pWData,_bWLen)))
         {    
             ++s_tDB_RecNum.wHrmRecNum;
                               
             ret = CC_Fds_Update(_wFileId,REC_ID_RecordNum, &s_tDB_RecNum.wHrmRecNum,sizeof(uint32_t)/4);
         }
         else
         {
             TracerInfo("_DB_Save_HrmData Write Data Fail \r\n");   
         }         
    }     


    if (ret == FDS_ERR_NOT_FOUND) 
    {
         s_tDB_RecNum.wHrmRecNum = 1;
         
         ret = CC_Fds_Write(_wFileId,REC_ID_RecordNum, &s_tDB_RecNum.wHrmRecNum,sizeof(uint32_t)/4);
    } 

    
    return ret;    

}

static ret_code_t _DB_Save_SleepData(eDB_Content_Op_t _eOpCode,uint32_t *pWData,uint16_t _bWLen)
{
    ret_code_t ret;

    uint16_t _wFileId = _DB_FileID_Get(_eOpCode);
    uint16_t _wRecId;


    if(FDS_SUCCESS == (ret = CC_Fds_Read(_wFileId, REC_ID_RecordNum,
         &s_tDB_RecNum.wSleepRecNum, &s_tDB_RecNum.wSleepRecLen)))        
    {

         _wRecId = s_tDB_RecNum.wSleepRecNum + 2; // offset:2


         TracerInfo(">>>save_rec_num = %d\r\n", _wRecId); 

         
         if (FDS_SUCCESS == (ret = CC_Fds_Write(_wFileId,_wRecId,pWData,_bWLen)))
         {    
             ++s_tDB_RecNum.wSleepRecNum;
                               
             ret = CC_Fds_Update(_wFileId,REC_ID_RecordNum, &s_tDB_RecNum.wSleepRecNum,sizeof(uint32_t)/4);
         }
         else
         {
             TracerInfo("_DB_Save_SleepData Write Data Fail \r\n");   
         }         
    }     


    if (ret == FDS_ERR_NOT_FOUND) 
    {
         s_tDB_RecNum.wSleepRecNum = 1;
              
         ret = CC_Fds_Write(_wFileId,REC_ID_RecordNum, &s_tDB_RecNum.wSleepRecNum,sizeof(uint32_t)/4);
    } 


    return ret;   
 
}

static ret_code_t _DB_Save_SwimData(eDB_Content_Op_t _eOpCode,uint32_t *pWData,uint16_t _bWLen)
{
    ret_code_t ret;

    uint16_t _wFileId = _DB_FileID_Get(_eOpCode);
    uint16_t _wRecId;


    if(FDS_SUCCESS == (ret = CC_Fds_Read(_wFileId, REC_ID_RecordNum,
         &s_tDB_RecNum.wSwimRecNum, &s_tDB_RecNum.wSwimRecLen)))        
    {

         _wRecId = s_tDB_RecNum.wSwimRecNum + 2; // offset:2


         TracerInfo(">>>save_rec_num = %d\r\n", _wRecId); 

         
         if (FDS_SUCCESS == (ret = CC_Fds_Write(_wFileId,_wRecId,pWData,_bWLen)))
         {    
             ++s_tDB_RecNum.wSwimRecNum;
                               
             ret = CC_Fds_Update(_wFileId,REC_ID_RecordNum, &s_tDB_RecNum.wSwimRecNum,sizeof(uint32_t)/4);
         }
         else
         {
             TracerInfo("_DB_Save_SwimData Write Data Fail \r\n");   
         }         
    }     


    if (ret == FDS_ERR_NOT_FOUND) 
    {
         s_tDB_RecNum.wSwimRecNum = 1;
              
         ret = CC_Fds_Write(_wFileId,REC_ID_RecordNum, &s_tDB_RecNum.wSwimRecNum,sizeof(uint32_t)/4);
    } 


    return ret;   

}
#ifdef BatteryLog_EN
static ret_code_t _DB_Save_MixLogData(eDB_Content_Op_t _eOpCode,uint32_t *pWData,uint16_t _bWLen)
{
    ret_code_t ret;

    uint16_t _wFileId = _DB_FileID_Get(_eOpCode);
    uint16_t _wRecId;


    if(FDS_SUCCESS == (ret = CC_Fds_Read(_wFileId, REC_ID_RecordNum,
         &s_tDB_RecNum.wMixLogRecNum, &s_tDB_RecNum.wMixLogRecLen)))        
    {

         _wRecId = s_tDB_RecNum.wMixLogRecNum + 2; // offset:2


         TracerInfo(">>>save_rec_num = %d\r\n", _wRecId); 

         
         if (FDS_SUCCESS == (ret = CC_Fds_Write(_wFileId,_wRecId,pWData,_bWLen)))
         {    
             ++s_tDB_RecNum.wMixLogRecNum;
                               
             ret = CC_Fds_Update(_wFileId,REC_ID_RecordNum, &s_tDB_RecNum.wMixLogRecNum,sizeof(uint32_t)/4);
         }
         else
         {
             TracerInfo("_DB_Save_MixLogData Write Data Fail \r\n");   
         }         
    }     


    if (ret == FDS_ERR_NOT_FOUND) 
    {
         s_tDB_RecNum.wMixLogRecNum = 1;
              
         ret = CC_Fds_Write(_wFileId,REC_ID_RecordNum, &s_tDB_RecNum.wMixLogRecNum,sizeof(uint32_t)/4);
    } 


    return ret;      
    
}
#endif

#if 0
ret_code_t _DB_WriteFirstBootCode(void)
{
    ret_code_t ret;
    uint32_t _dwTmpData = 0x00000000;
/*    app_date_time_t app_time;

    memset(&app_time, 0x00, sizeof(app_date_time_t));

    ret = CC_Fds_Write(FILE_ID_SYSTEM,FRIST_BOOT_RECID,(uint32_t*)&app_time,sizeof(app_date_time_t));
    if ( CC_SUCCESS!=ret)
    {
        return ret;
    }
    while (_bWrite_Flag == false)
    ;        
    return ret;
*/

    _dwTmpData = FIRST_BOOT_CODE;
    ret = CC_Fds_Write(FILE_ID_SYSTEM,FRIST_BOOT_RECID,&_dwTmpData,sizeof(uint32_t));
    if ( CC_SUCCESS!=ret)
    {
        return ret;
    }
    while (_bWrite_Flag == false)
    ;       
    return ret;

}
#endif

ret_code_t _db_Create_RecSpace(uint8_t i)
{
        //init DB each file REC_ID_RecordNum to 0
    ret_code_t ret =0;
    uint32_t _dwTmpData = 0x00000000;
    uint16_t _wTmpFileId = 0;
    
    _wTmpFileId = FILE_ID_PED_SUN | RECID_RETURN(i);
    
    if (CC_SUCCESS  != (ret = CC_Fds_Write(_wTmpFileId,REC_ID_RecordNum,&_dwTmpData,sizeof(uint32_t)/4)))
        return ret;

    TracerInfo("_db_Create_ File Rec 1  FileID=[0x%x] RecID=[0x%x] \r\n",_wTmpFileId,REC_ID_RecordNum);

     _wTmpFileId = FILE_ID_HRM_SUN | RECID_RETURN(i);
     
    if (CC_SUCCESS  != (ret = CC_Fds_Write(_wTmpFileId,REC_ID_RecordNum,&_dwTmpData,sizeof(uint32_t)/4)))
        return ret;
        
    TracerInfo("_db_Create_ File Rec 1  FileID=[0x%x] RecID=[0x%x] \r\n",_wTmpFileId,REC_ID_RecordNum);

     _wTmpFileId = FILE_ID_SLEEP_SUN | RECID_RETURN(i);

    if (CC_SUCCESS  != (ret = CC_Fds_Write(_wTmpFileId,REC_ID_RecordNum,&_dwTmpData,sizeof(uint32_t)/4)))
        return ret;

    TracerInfo("_db_Create_ File Rec 1  FileID=[0x%x] RecID=[0x%x] \r\n",_wTmpFileId,REC_ID_RecordNum);

     _wTmpFileId = FILE_ID_SWIM_SUN | RECID_RETURN(i);

    if (CC_SUCCESS  != (ret = CC_Fds_Write(_wTmpFileId,REC_ID_RecordNum,&_dwTmpData,sizeof(uint32_t)/4)))
        return ret;

    TracerInfo("_db_Create_ File Rec 1  FileID=[0x%x] RecID=[0x%x] \r\n",_wTmpFileId,REC_ID_RecordNum);
    
    #ifdef BatteryLog_EN
    _wTmpFileId = FILE_MixLog_ID_SUN| RECID_RETURN(i);
    
    if (CC_SUCCESS  != (ret = CC_Fds_Write(_wTmpFileId,REC_ID_RecordNum,&_dwTmpData,sizeof(uint32_t)/4)))
        return ret;

    TracerInfo("_db_Create_ File Rec 1  FileID=[0x%x] RecID=[0x%x] \r\n",_wTmpFileId,REC_ID_RecordNum);
    #endif
    
    return ret;
}
ret_code_t _DB_Delete_FileSpace(uint8_t _bDayofWeek)
{
    ret_code_t ret;
    
    ret = CC_FindFile_And_DeleteFile(ePed,_bDayofWeek);
    ret |= CC_FindFile_And_DeleteFile(eHrm,_bDayofWeek);
    ret |= CC_FindFile_And_DeleteFile(eSleep,_bDayofWeek);
    ret |= CC_FindFile_And_DeleteFile(eSwim,_bDayofWeek);
#ifdef BatteryLog_EN
    ret |= CC_FindFile_And_DeleteFile(eMixLog,_bDayofWeek);
#endif    
    return ret;
}

ret_code_t CC_DB_Create_OneDay_RecSpace(eDB_Content_Op_t id, uint8_t _bDayofWeek)
{
    ret_code_t ret =0;
    uint32_t _dwTmpData = 0x00000000;
    uint16_t _wTmpFileId = 0;
    
    _wTmpFileId = id | RECID_RETURN(_bDayofWeek);

    ret = CC_Fds_Write(_wTmpFileId, REC_ID_RecordNum, &_dwTmpData, sizeof(uint32_t)/4);

    return ret;
}

ret_code_t CC_DB_Delete_OneDay_File(eDB_Content_Op_t id, uint8_t _bDayofWeek)
{
    ret_code_t ret;    
    app_date_time_t curr_time;
    
    if(CC_SUCCESS != (ret = CC_FindFile_And_DeleteFile(id, _bDayofWeek)))
        return ret;

    if(CC_SUCCESS != (ret = CC_DB_Create_OneDay_RecSpace(id, _bDayofWeek)))
        return ret;

    CC_MainGet_CurrentTime(&curr_time);
    
    if(curr_time.dayofweek == _bDayofWeek)
    {      
        switch (id)
        {
            case ePed:
                s_tDB_RecNum.wPedRecNum = 0x00;
                   //s_tDB_RecId.wPedRecID = REC_ID_RecordNum;
                break;
            case eHrm:
                s_tDB_RecNum.wHrmRecNum = 0x00;
                    //s_tDB_RecId.wHrmRecID = REC_ID_RecordNum;
                break;
            case eSleep:
                s_tDB_RecNum.wSleepRecNum = 0x00;
                //s_tDB_RecId.wSleepRecID = REC_ID_RecordNum;
                break;
            case eSwim:
                s_tDB_RecNum.wSwimRecNum = 0x00;
                //s_tDB_RecId.wSwimRecID = REC_ID_RecordNum;
                break;
#ifdef BatteryLog_EN
            case eMixLog:
                s_tDB_RecNum.wMixLogRecNum = 0x00;
                //s_tDB_RecId.wMixLogRecID = REC_ID_RecordNum;
                break; 
#endif 
            default:    
            break;
            
        }
    }

    return CC_SUCCESS;
}

uint8_t CC_DB_Check_Magic_Code(void)
{
    db_sys_first_use_record_t db_time;
    uint16_t rSize;

    if(CC_SUCCESS == CC_Fds_Read(eSystem, DB_SYS_TIME,  (uint32_t*)&db_time, &rSize))
    {
        return (DB_MAGIC_CODE == db_time.magic_code);
    }

    return 0;
}

void CC_DB_Recover_System_Data(void)
{

    CC_Ble_General_Info_T GeneralInfo;
    CC_Ble_Clock_Alarm_T alarm;
    db_sys_notify_enabled_t notify_settings;
    CC_Ble_Unit_Info_T unit_info;
    db_sys_sleep_monitor_t _stSleepMonitorTimeSetting;
    db_sys_static_gyro_offset_t _stStaticSwimCalInfo;



    // General information:
    CC_BLE_Cmd_GetGeneralInfo(&GeneralInfo,false);
    CC_DB_Read_System_Data(DB_SYS_GENERAL_INFO, &GeneralInfo);
    CC_BLE_Cmd_SetGeneralInfo((uint8_t*)&GeneralInfo);

    // Clock alarm:
    CC_BLE_Cmd_GetClockAlarm(&alarm,false);
    CC_DB_Read_System_Data(DB_SYS_ALARM, &alarm);
    CC_BLE_Cmd_SetClockAlarm((uint8_t*)&alarm);

    // Notify settings:
    CC_BLE_Cmd_GetNotifySetting(&notify_settings);
    CC_DB_Read_System_Data(DB_SYS_NOTIFY, &notify_settings);
    CC_BLE_Cmd_SetNotifySetting((uint8_t*)&notify_settings);

    // Unit info:
    CC_BLE_Cmd_GetUnitInfo(&unit_info,false);
    CC_DB_Read_System_Data(DB_SYS_UNIT, &unit_info);
    CC_BLE_Cmd_SetUnitInfo((uint8_t*)&unit_info);

      // Sleep Monitor
    //CC_BLE_Cmd_GetSleepTimeSetting(&_stSleepMonitorTimeSetting);
    CC_DB_Read_System_Data(DB_SYS_SLEEP_MONITOR_TIME_SETTING, &_stSleepMonitorTimeSetting);
    CC_BLE_Cmd_SetSleepTimeSetting((uint8_t*)&_stSleepMonitorTimeSetting);

      // static swim cal info
    CC_DB_Read_System_Data(DB_SYS_STATIC_GYRO_OFFSET, &_stStaticSwimCalInfo);
    if ( false == _stStaticSwimCalInfo.wVaildFlag )
    {
        memset(&_stStaticSwimCalInfo, 0x00, sizeof(db_sys_static_gyro_offset_t));
    }
    CC_SetStaticCalibrationInfo(&_stStaticSwimCalInfo);
      
    // HR Setting
    #if 1 //Jason, clear HRS and 24 HR data to 0
    db_sys_hr_setting_t    _tHrSetting;
    memset(&_tHrSetting,0x00, sizeof(db_sys_hr_setting_t));
    CC_BLE_Cmd_SetHrSetting(&_tHrSetting);
    #else
    {
        db_sys_hr_setting_t    _tHrSetting;

        CC_DB_Read_System_Data(DB_SYS_HR_SETTING, &_tHrSetting);

        CC_AppSrv_HR_Set24HrPeriod(_tHrSetting.dw24HrPeriodicMeasurementTime, _tHrSetting.dw24HrOneMeasurementMaxTime);

        CC_BLE_Cmd_SetHrSetting(&_tHrSetting);
    }
    #endif
}

void CC_DB_Factory_System_DataReset(void)
{
  
    CC_Ble_General_Info_T GeneralInfo;
    CC_Ble_Clock_Alarm_T alarm;
    db_sys_notify_enabled_t notify_settings;
    CC_Ble_Unit_Info_T unit_info;
    db_sys_sleep_monitor_t _stSleepMontorTimeSetting;

    TracerInfo("CC_DB_Init_System_Data \r\n");
    //memset(&GeneralInfo, 0x00, sizeof(CC_Ble_General_Info_T));

    GeneralInfo.cHeight = 170;
    GeneralInfo.cWeight = 65;
    GeneralInfo.cAge = 30;
    GeneralInfo.cGender = CC_LIB_GENDER_MALE;
    GeneralInfo.cStride_Lenght = 70;
    GeneralInfo.cSwim_Pool_Size = eSWIM_50M;
    GeneralInfo.bBandLocation= 0; //Left hand
    GeneralInfo.bRestingHrMax= APPSRV_HRM_DEFAULT_RESTING_MAX; //Left hand
    GeneralInfo.bRestingHrMin= APPSRV_HRM_DEFAULT_RESTING_MIN; //Left hand
    GeneralInfo.bExerciseHrMax= APPSRV_HRM_DEFAULT_EXERCISE_MAX(30); //Left hand
    GeneralInfo.bExerciseHrMin= APPSRV_HRM_DEFAULT_EXERCISE_MIN(30); //Left hand
    

    memset(&alarm, 0x00, sizeof(CC_Ble_Clock_Alarm_T));
    alarm.cTotalNum = 4;
    memset(&notify_settings, 0x00, sizeof(db_sys_notify_enabled_t));
    memset(&unit_info, 0x00, sizeof(CC_Ble_Unit_Info_T));

    _stSleepMontorTimeSetting.start_time_hour = 22;
    _stSleepMontorTimeSetting.start_time_min = 0;
    _stSleepMontorTimeSetting.end_time_hour = 8;
    _stSleepMontorTimeSetting.end_time_min = 0;

    CC_BLE_Cmd_SetGeneralInfo((uint8_t*)&GeneralInfo);
    
    CC_BLE_Cmd_SetClockAlarm((uint8_t*)&alarm);
    CC_BLE_Cmd_SetNotifySetting((uint8_t*)&notify_settings);
    CC_BLE_Cmd_SetUnitInfo((uint8_t*)&unit_info);
    CC_BLE_Cmd_SetSleepTimeSetting((uint8_t*)&_stSleepMontorTimeSetting);

    // HR Setting
    #if 1 //Jason, clear HRS and 24 HR data to 0
    db_sys_hr_setting_t    _tHrSetting;
    memset(&_tHrSetting,0x00, sizeof(db_sys_hr_setting_t));
    CC_BLE_Cmd_SetHrSetting(&_tHrSetting);
    #else
    {
        db_sys_hr_setting_t    _tHrSetting;

        CC_DB_Read_System_Data(DB_SYS_HR_SETTING, &_tHrSetting);

        CC_AppSrv_HR_Set24HrPeriod(_tHrSetting.dw24HrPeriodicMeasurementTime, _tHrSetting.dw24HrOneMeasurementMaxTime);

        CC_BLE_Cmd_SetHrSetting(&_tHrSetting);
    }
    #endif

    //App set DB reset
    for (int _bDayofWeek =0;_bDayofWeek<7;_bDayofWeek++)
        _DB_Delete_FileSpace(_bDayofWeek);
    for (int _bDayofWeek =0;_bDayofWeek<7;_bDayofWeek++)
        _db_Create_RecSpace(_bDayofWeek);        

}

void CC_DB_Init_System_Data(void)
{
    CC_Ble_General_Info_T GeneralInfo;
    CC_Ble_Clock_Alarm_T alarm;
    db_sys_notify_enabled_t notify_settings;
    CC_Ble_Unit_Info_T unit_info;
    db_sys_sleep_monitor_t _stSleepMontorTimeSetting;
    db_sys_static_gyro_offset_t _stStaticSwimCalInfo;

    TracerInfo("CC_DB_Init_System_Data \r\n");
    //memset(&GeneralInfo, 0x00, sizeof(CC_Ble_General_Info_T));

    GeneralInfo.cHeight = 170;
    GeneralInfo.cWeight = 65;
    GeneralInfo.cAge = 30;
    GeneralInfo.cGender = CC_LIB_GENDER_MALE;
    GeneralInfo.cStride_Lenght = 70;
    GeneralInfo.cSwim_Pool_Size = eSWIM_50M;
    GeneralInfo.bBandLocation= 0; //Left hand

    GeneralInfo.bRestingHrMax= APPSRV_HRM_DEFAULT_RESTING_MAX; //Left hand
    GeneralInfo.bRestingHrMin= APPSRV_HRM_DEFAULT_RESTING_MIN; //Left hand
    GeneralInfo.bExerciseHrMax= APPSRV_HRM_DEFAULT_EXERCISE_MAX(30); //Left hand
    GeneralInfo.bExerciseHrMin= APPSRV_HRM_DEFAULT_EXERCISE_MIN(30); //Left hand

    memset(&alarm, 0x00, sizeof(CC_Ble_Clock_Alarm_T));
    memset(&notify_settings, 0x00, sizeof(db_sys_notify_enabled_t));
    memset(&unit_info, 0x00, sizeof(CC_Ble_Unit_Info_T));

    _stSleepMontorTimeSetting.start_time_hour = 22;
    _stSleepMontorTimeSetting.start_time_min = 0;
    _stSleepMontorTimeSetting.end_time_hour = 8;
    _stSleepMontorTimeSetting.end_time_min = 0;

    CC_BLE_Cmd_SetGeneralInfo((uint8_t*)&GeneralInfo);
    CC_BLE_Cmd_SetClockAlarm((uint8_t*)&alarm);
    CC_BLE_Cmd_SetNotifySetting((uint8_t*)&notify_settings);
    CC_BLE_Cmd_SetUnitInfo((uint8_t*)&unit_info);
    CC_BLE_Cmd_SetSleepTimeSetting((uint8_t*)&_stSleepMontorTimeSetting);


    memset(&_stStaticSwimCalInfo, 0x00, sizeof(db_sys_static_gyro_offset_t));
    CC_DB_Read_System_Data(DB_SYS_STATIC_GYRO_OFFSET, &_stStaticSwimCalInfo);
    if (false == _stStaticSwimCalInfo.wVaildFlag)
    {
        memset(&_stStaticSwimCalInfo, 0x00, sizeof(db_sys_static_gyro_offset_t));    
    }    
    CC_SetStaticCalibrationInfo(&_stStaticSwimCalInfo);
    
}

 ret_code_t CC_DB_Init_FileRec(uint8_t *_bFirstInit, uint8_t _bState)
{
    ret_code_t ret;

    if (DB_INIT_FROM_SYSTEM == _bState)
    {

        if(0x01 == CC_DB_Check_Magic_Code())
        {

            CC_DB_Recover_System_Data();
             *_bFirstInit = true;
        }
        else
        {
            for (int _bDayofWeek =0;_bDayofWeek<7;_bDayofWeek++)
                _DB_Delete_FileSpace(_bDayofWeek);
            for (int _bDayofWeek =0;_bDayofWeek<7;_bDayofWeek++)
                ret =_db_Create_RecSpace(_bDayofWeek);

            CC_DB_Init_System_Data();
            *_bFirstInit = false;
        }
    }
    else if(DB_INIT_FROM_APP == _bState)
    {
         //App set DB reset
         for (int _bDayofWeek =0;_bDayofWeek<7;_bDayofWeek++)
            _DB_Delete_FileSpace(_bDayofWeek);
         for (int _bDayofWeek =0;_bDayofWeek<7;_bDayofWeek++)
            ret =_db_Create_RecSpace(_bDayofWeek);
         
         CC_DB_Recover_System_Data();
             *_bFirstInit = false;
    }

    return ret;
}


ret_code_t CC_Save_Record(eDB_Content_Op_t _eOpCode,uint32_t *pWData,uint16_t _bWLen)
{
    ret_code_t ret;    

    //if((0 == CC_DB_Check_Init_Done()) || (0x00 == app_Get_Update_Time_Flag()) || (true == CC_Ble_Check_Is_Bond_Locked()))
    if((0 == CC_DB_Check_Init_Done()) || (0x00 == app_Get_Update_Time_Flag()))
        return FDS_ERR_NOT_INITIALIZED;


    switch (_eOpCode)
    {
        case eSystem:

            break;
        case ePed:
            ret = _DB_Save_PedData(_eOpCode, pWData, _bWLen/4);
            break;
        case eHrm:
            ret = _DB_Save_HrmData(_eOpCode, pWData, _bWLen/4);
            break;
        case eSleep:
            ret = _DB_Save_SleepData(_eOpCode, pWData, _bWLen/4);
            break;
        case eSwim:
            ret = _DB_Save_SwimData(_eOpCode, pWData, _bWLen/4);
            break;
#ifdef BatteryLog_EN
        case eMixLog:
            ret = _DB_Save_MixLogData(_eOpCode, pWData, _bWLen/4);
            break;
#endif
        default:
            break;
    }



    if(FDS_ERR_NO_SPACE_IN_FLASH == ret)
        _DB_Set_DB_Full();
    else if(FDS_SUCCESS == ret)
        _DB_Clr_DB_Full();

    return ret;
}

ret_code_t CC_Read_DayofRecoordLen(eDB_Content_Op_t _eOpCode,uint8_t _bDayofWeek, uint16_t *_wDataLen)
{
    ret_code_t ret;
    uint16_t _wFileId =0;
    uint32_t _wTmpData;
    uint16_t _wTmpLen;

    //if((0 == CC_DB_Check_Init_Done()) || (0x00 == app_Get_Update_Time_Flag()) || (true == CC_Ble_Check_Is_Bond_Locked()))
    if((0 == CC_DB_Check_Init_Done()) || (0x00 == app_Get_Update_Time_Flag()))
        return FDS_ERR_NOT_INITIALIZED;


    switch (_eOpCode)
    {
        case eSystem:
            break;
        case ePed:
        case eHrm:
        case eSleep:
        case eSwim:
#ifdef BatteryLog_EN
        case eMixLog:
#endif
        {
            _wFileId = _eOpCode | RECID_RETURN(_bDayofWeek);
            ret = CC_Fds_Read(_wFileId,REC_ID_RecordNum,&_wTmpData,&_wTmpLen);
            if (ret == FDS_SUCCESS)                    
            {
                *_wDataLen =(uint16_t) _wTmpData;
               
            }
            else
                TracerInfo("error read length = %d\r\n",ret);
        }    
            break;
        default:
            break;
    }
    return ret;

}

ret_code_t CC_Read_Record(eDB_Content_Op_t _eOpCode,uint8_t _bDayofWeek,uint16_t _wRecId ,uint32_t *_pRData, uint16_t *_wDataLen)
{
    ret_code_t ret;
    uint16_t _wFileId =0;
    uint16_t _wTmpRecId = _wRecId+REC_ID_RecordOffset;


    switch (_eOpCode)
    {
        case eSystem:
            break;
        case ePed:
        case eHrm:
        case eSleep:
        case eSwim:
#ifdef BatteryLog_EN
        case eMixLog:
#endif
        {
            _wFileId = _eOpCode | RECID_RETURN(_bDayofWeek);
            ret = CC_Fds_Read(_wFileId,_wTmpRecId,_pRData,_wDataLen);

        }    
            break;
        default:
            break;
    }
     return ret;

}

ret_code_t CC_FindFile_And_DeleteFile(eDB_Content_Op_t _eOpCode,uint8_t _bDayofWeek)
{
    fds_record_desc_t   record_desc;        
    fds_find_token_t    ftok;        
    //ret_code_t ret;
    uint16_t _wFileId = _eOpCode | RECID_RETURN(_bDayofWeek);
    ftok.page=0;        
    ftok.p_addr=NULL;    

    TracerInfo("CC_FindFile_And_DeleteFile FileID=[0x%x] \r\n",_wFileId);    
    // Loop and find records with same ID and rec key and mark them as deleted.         
    while (fds_record_find_in_file(_wFileId, &record_desc, &ftok) == FDS_SUCCESS)        
    {            
        _bdelete_Flag = false;
        
        if(FDS_SUCCESS == (fds_file_delete(_wFileId)))
        {
            while(_bdelete_Flag == false)
            {         
                 __WFE();
         
            }
        }
        //TracerInfo("Deleted record ID: %d \r\n",record_desc.record_id);        

    }        
    

    _DB_Clr_DB_Full();  
    
    return CC_SUCCESS;
}



ret_code_t CC_Fds_init (void)
{
    ret_code_t ret;

    ret = fds_init();
    if (ret != FDS_SUCCESS)
    {
        return ret;
    }
    
    ret = fds_register(_DB_Fds_Evt_Handler);
    if (ret != FDS_SUCCESS)
    {
        return ret;    
    }        
    return CC_SUCCESS;
}

void CC_Fds_Change_DayofWeek(uint8_t _bDayofWeek, uint8_t _IsReCreateOneDay)
{
    switch (_bDayofWeek)
    {
        case APP_WEEK_SUNDAY:
        case APP_WEEK_MONDAY:
        case APP_WEEK_TUESDAY:  
        case APP_WEEK_WEDNESDAY:
        case APP_WEEK_THURSDAY:    
        case APP_WEEK_FIRDAY:
        case APP_WEEK_SATURDAY:    
        {
            
            s_tDB_FileId.wPedFileID = FILE_ID_PED_SUN | RECID_RETURN(_bDayofWeek);
            s_tDB_FileId.wHrmFileID = FILE_ID_HRM_SUN | RECID_RETURN(_bDayofWeek);
            s_tDB_FileId.wSleepFileID = FILE_ID_SLEEP_SUN | RECID_RETURN(_bDayofWeek);
            s_tDB_FileId.wSwimFileID = FILE_ID_SWIM_SUN | RECID_RETURN(_bDayofWeek);
#ifdef BatteryLog_EN
            s_tDB_FileId.wMixLogFileID = FILE_MixLog_ID_SUN | RECID_RETURN(_bDayofWeek);;
#endif            

#if 0
            s_tDB_RecId.wPedRecID = REC_ID_RecordNum;
            s_tDB_RecId.wHrmRecID = REC_ID_RecordNum;
            s_tDB_RecId.wSleepRecID = REC_ID_RecordNum;
            s_tDB_RecId.wSwimRecID = REC_ID_RecordNum;
#ifdef BatteryLog_EN
            s_tDB_RecId.wMixLogRecID = REC_ID_RecordNum;
#endif
#endif
            if ( true ==_IsReCreateOneDay)  // _IsReCreateOneDay == true, day change to delete and create new one day file and rec
            {                                             // _IsReCreateOneDay == false, just for sync file ID and Rec ID (no use for now codebase)
            _DB_Delete_FileSpace(_bDayofWeek);
            _db_Create_RecSpace(_bDayofWeek);
            }    
        }   
                break;
                
         default:
            break;

            
    }
}

#include "drvi_eflash.h"

ret_code_t CC_Fds_Write(const uint16_t _wFileID, const uint16_t _wRecKey,uint32_t *pWData,uint16_t _bWLen)
{
    fds_record_t        record;
    fds_record_desc_t   record_desc;
    fds_record_chunk_t  record_chunk;
    ret_code_t ret;
        
    // Set up data.
    record_chunk.p_data         = pWData;
    record_chunk.length_words   = _bWLen;
    // Set up record.
    record.file_id              = _wFileID;
    record.key                  = _wRecKey;
    record.data.p_chunks       = &record_chunk;
    record.data.num_chunks   = 1;

    _bWrite_Flag  = false; 



    
    if(FDS_SUCCESS != (ret = fds_record_write(&record_desc, &record)))
    {
        TracerInfo("fds_record_write != FDS_SUCCESS fds_record_write: %d\r\n", ret);
        if(FDS_ERR_NO_SPACE_IN_FLASH == ret)
        {
             ret_code_t gc_ret;
                            
             _bgc_Flag = false;

             if(FDS_SUCCESS != (gc_ret = fds_gc()))
                  return gc_ret;
                        
             while (_bgc_Flag == false)
             {
                  __WFE();     
             }                              
        }
    
        return ret;
    }
    while (_bWrite_Flag == false)
    {
        __WFE();     
    }

    TracerInfo("Writing Record ID = %d \r\n",record_desc.record_id);

    return CC_SUCCESS;
}


ret_code_t CC_Fds_Update(const uint16_t _wFileID, const uint16_t _wRecKey,uint32_t *pWData,uint16_t _bWLen)
{
    fds_record_t        record;
    //fds_flash_record_t  flash_record;
    fds_record_desc_t   record_desc;
    fds_record_chunk_t  record_chunk;
    fds_find_token_t    ftok ={0};//Important, make sure you zero init the ftok token
    ret_code_t ret;
    //uint32_t err_code;

    // Set up data.
    record_chunk.p_data         = pWData;
    record_chunk.length_words   = _bWLen;
    // Set up record.
    record.file_id              = _wFileID;
    record.key                  = _wRecKey;
    record.data.p_chunks       = &record_chunk;
    record.data.num_chunks   = 1;



    //while (fds_record_find(_wFileID, _wRecKey, &record_desc, &ftok) == FDS_SUCCESS)        
    if(fds_record_find(_wFileID, _wRecKey, &record_desc, &ftok) == FDS_SUCCESS)        
    {        
        _bUpdate_Flag = false;
        
        if(FDS_SUCCESS != (ret = fds_record_update(&record_desc, &record)))
        {
            if(FDS_ERR_NO_SPACE_IN_FLASH == ret)
            {
                 ret_code_t gc_ret;
                                   
                 _bgc_Flag = false;
            
                 if(FDS_SUCCESS != (gc_ret = fds_gc()))
                      return gc_ret;
                               
                 while (_bgc_Flag == false)
                 {
                      __WFE();     
                 }                              
            }
            
            return ret;
        }
        
        while (_bUpdate_Flag == false)
        {
           __WFE();     
        }
    }    
    
    return CC_SUCCESS;

}


ret_code_t CC_Fds_Read(const uint16_t _RFileID, const uint16_t _RRecKey,uint32_t *_pRData,uint16_t *_pRLen)
{
    fds_flash_record_t  flash_record;
    fds_record_desc_t   record_desc;
    fds_find_token_t    ftok ={0};//Important, make sure you zero init the ftok token
    //uint32_t *data;
    uint32_t err_code;



    //TracerInfo("Start searching... \r\n");
    // Loop until all records with the given key and file ID have been found.
    while (fds_record_find(_RFileID, _RRecKey, &record_desc, &ftok) == FDS_SUCCESS)
    {
        err_code = fds_record_open(&record_desc, &flash_record);
        if ( err_code != FDS_SUCCESS)
        {
            fds_record_close(&record_desc);
            return err_code;        
        }

        *_pRLen = flash_record.p_header->tl.length_words;    
        #if 0
        if(REC_ID_RecordNum == _RRecKey) // fixed by Chiu
            memcpy(_pRData,flash_record.p_data, sizeof(uint32_t));          
        else
        #endif   
            memcpy(_pRData,flash_record.p_data, sizeof(uint32_t)*(flash_record.p_header->tl.length_words));  

        TracerInfo("CC_Fds_Read Data  FileID=[0x%x] RecID=[0x%x] \r\n",_RFileID,_RRecKey);
        TracerInfo("CC_Fds_Read Read Flash Address = 0x%8x\r\n",(uint32_t)flash_record.p_data);
        err_code = fds_record_close(&record_desc);
        if (err_code != FDS_SUCCESS)
        {
            return err_code;    
        }
        return FDS_SUCCESS;
    }
   
    return FDS_ERR_NOT_FOUND;     
}

ret_code_t CC_Fds_Find_And_Delete (const uint16_t _FileID, const uint16_t _RecKey)
{    
    fds_record_desc_t   record_desc;        
    fds_find_token_t    ftok;            
    ret_code_t ret;
    ftok.page=0;        
    ftok.p_addr=NULL;        
    
    // Loop and find records with same ID and rec key and mark them as deleted.         
    while (fds_record_find(_FileID, _RecKey, &record_desc, &ftok) == FDS_SUCCESS)        
    {    
        _bdeleteRec_Flag = false;
        
        if(FDS_SUCCESS != (ret = fds_record_delete(&record_desc)))
            return ret;

        while (_bdeleteRec_Flag == false)
        {
            __WFE();     
        }
       
        TracerInfo("Deleted record ID: %d \r\n",record_desc.record_id);        

    }        

    return CC_SUCCESS;
}

void _DB_TEST_READ_SLEEP(void)
{
    cc6801_ClockDelayMs(1000);
    uint16_t total;
    db_sleep_t m_db_sleep;    
    uint16_t rsize;

    app_date_time_t curr_time;
     
    CC_MainGet_CurrentTime(&curr_time);
    

    CC_Read_DayofRecoordLen(eSleep, curr_time.dayofweek, &total);
    TracerInfo("total = %d\r\n",total);

     for (int i =1 ; i <=total; i++ )
     {
     
        CC_Read_Record(eSleep,curr_time.dayofweek , i, (uint32_t*)&m_db_sleep, &rsize);
        TracerInfo("=========[%d]=============\r\n",i);
        
        TracerInfo("total count = %d index=\r\n",total,i);

        
        TracerInfo("-----------------------------\r\n");
        TracerInfo("time y %d/%d/%d  \r\n",m_db_sleep.detect_time.year,
                                                              m_db_sleep.detect_time.month,
                                                              m_db_sleep.detect_time.day);
         
        TracerInfo("time h %d/%d/%d  \r\n",m_db_sleep.detect_time.hour,
                                                              m_db_sleep.detect_time.min,
                                                              m_db_sleep.detect_time.sec);

        

        TracerInfo("sleep state = %d\r\n",m_db_sleep.detect_time.sleep_state);
        TracerInfo("time period  = %d\r\n",m_db_sleep.period.period_min);
        TracerInfo("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\r\n");
     }  
     //NRF_LOG_FLUSH();
        
}

ret_code_t CC_DB_Check_And_Write(const uint16_t file_id, const uint16_t rec_key,uint32_t *wdata,uint16_t blen) // len in Bytes
{
    ret_code_t ret;
    uint16_t rlen;
    uint32_t rbuf[30];
    uint16_t wlen;

    wlen = blen/4; // in words(4Bytes)

    if(FDS_SUCCESS == (ret = CC_Fds_Read(file_id, rec_key, rbuf, &rlen)))
    {
        if (FDS_SUCCESS == (ret = CC_Fds_Find_And_Delete(file_id, rec_key)))
        {                    
            ret = CC_Fds_Write(file_id, rec_key, wdata, wlen);           
        }
    }


    if (ret == FDS_ERR_NOT_FOUND) 
    {
        ret = CC_Fds_Write(file_id, rec_key, wdata, wlen);      
    }

    return ret;
}


ret_code_t CC_DB_Save_System_Time(app_date_time_t *app_time)
{
    db_sys_first_use_record_t db_time;
    ret_code_t ret;

    db_time.year = (uint8_t)(app_time->year - 2000);
    db_time.month = app_time->month;
    db_time.day = app_time->day;
    db_time.hour = app_time->hours;
    db_time.min = app_time->minutes;
    db_time.dayofweek = app_time->dayofweek;
    db_time.magic_code = DB_MAGIC_CODE;    

    ret = CC_DB_Check_And_Write(eSystem, DB_SYS_TIME, (uint32_t*) &db_time, sizeof(db_sys_first_use_record_t));    

    return ret;
}

ret_code_t CC_DB_Read_System_Time(app_date_time_t *app_time)
{
    db_sys_first_use_record_t db_time;
    uint16_t rSize;
    ret_code_t ret;

    if(CC_SUCCESS == (ret = CC_Fds_Read(eSystem, DB_SYS_TIME,  (uint32_t*)&db_time, &rSize)))
    {
        app_time->year = db_time.year + 2000;    
        app_time->month = db_time.month;
        app_time->day = db_time.day;
        app_time->hours = db_time.hour;
        app_time->minutes = db_time.min;
        app_time->dayofweek = db_time.dayofweek;
    }

    return ret;
}

ret_code_t CC_DB_Save_System_General_Info(CC_Ble_General_Info_T *Info)
{

    db_sys_general_info_t db_Info;
    ret_code_t ret;

    db_Info.height = Info->cHeight; 
    db_Info.weight = Info->cWeight;
    db_Info.age = Info->cAge;
    db_Info.gender = Info->cGender;
    db_Info.stride_len = Info->cStride_Lenght;
    db_Info.swim_pool_size = Info->cSwim_Pool_Size;
    db_Info.bBandLocation= Info->bBandLocation;
    db_Info.bRestingHrMax= Info->bRestingHrMax;
    db_Info.bRestingHrMin= Info->bRestingHrMin;
    db_Info.bExerciseHrMax= Info->bExerciseHrMax;
    db_Info.bExerciseHrMin= Info->bExerciseHrMin;
      
    
    ret = CC_DB_Check_And_Write(eSystem, DB_SYS_GENERAL_INFO, (uint32_t*) &db_Info, sizeof(db_sys_general_info_t));

    return ret;
}


ret_code_t CC_DB_Read_System_General_Info(CC_Ble_General_Info_T *Info)
{
    db_sys_general_info_t db_Info;
    uint16_t rSize;
    ret_code_t ret;

    if(CC_SUCCESS == (ret = CC_Fds_Read(eSystem, DB_SYS_GENERAL_INFO,  (uint32_t*)&db_Info, &rSize)))
    {
        Info->cHeight = db_Info.height; 
        Info->cWeight = db_Info.weight;
        Info->cAge = db_Info.age;
        Info->cGender = (CC_Gender_t)db_Info.gender;
        Info->cStride_Lenght = db_Info.stride_len;
        Info->cSwim_Pool_Size = (eSWIM_LEN_SET_t)db_Info.swim_pool_size;
        Info->bBandLocation = db_Info.bBandLocation;
        Info->bRestingHrMax = db_Info.bRestingHrMax;
        Info->bRestingHrMin = db_Info.bRestingHrMin;;
        Info->bExerciseHrMax = db_Info.bExerciseHrMax;;
        Info->bExerciseHrMin = db_Info.bExerciseHrMin;;
    }

    return ret;
}

int CC_DB_Read_AGE(void)
{
    CC_Ble_General_Info_T tGeneralInfo;
    
    if (CC_SUCCESS == CC_DB_Read_System_General_Info(&tGeneralInfo))
    {
        return (tGeneralInfo.cAge);
    }
    
    return (-1);
}

ret_code_t CC_DB_Save_System_Alarm(CC_Ble_Clock_Alarm_T *alarm)
{
    
    db_sys_alarm_t db_alarm[4];
    ret_code_t ret;

    db_alarm[0].setting = alarm->cAlarmTime[0].cSetting;
    db_alarm[0].hour = alarm->cAlarmTime[0].cHour;
    db_alarm[0].min = alarm->cAlarmTime[0].cMinute;
    db_alarm[1].setting = alarm->cAlarmTime[1].cSetting;
    db_alarm[1].hour = alarm->cAlarmTime[1].cHour;
    db_alarm[1].min = alarm->cAlarmTime[1].cMinute;
    db_alarm[2].setting = alarm->cAlarmTime[2].cSetting;
    db_alarm[2].hour = alarm->cAlarmTime[2].cHour;
    db_alarm[2].min = alarm->cAlarmTime[2].cMinute;
    db_alarm[3].setting = alarm->cAlarmTime[3].cSetting;
    db_alarm[3].hour = alarm->cAlarmTime[3].cHour;
    db_alarm[3].min = alarm->cAlarmTime[3].cMinute;

    ret = CC_DB_Check_And_Write(eSystem, DB_SYS_ALARM, (uint32_t*) db_alarm, (sizeof(db_sys_alarm_t)*4));
    
    return ret;
}

ret_code_t CC_DB_Read_System_Alarm(CC_Ble_Clock_Alarm_T *alarm)
{
    db_sys_alarm_t db_alarm[4];
    uint16_t rSize;
    ret_code_t ret;

    if(CC_SUCCESS == (ret = CC_Fds_Read(eSystem, DB_SYS_ALARM,  (uint32_t*)db_alarm, &rSize)))
    {
        
        alarm->cAlarmTime[0].cSetting = db_alarm[0].setting;
        alarm->cAlarmTime[0].cHour = db_alarm[0].hour;
        alarm->cAlarmTime[0].cMinute = db_alarm[0].min;
        alarm->cAlarmTime[1].cSetting = db_alarm[1].setting;
        alarm->cAlarmTime[1].cHour = db_alarm[1].hour;
        alarm->cAlarmTime[1].cMinute = db_alarm[1].min;
        alarm->cAlarmTime[2].cSetting = db_alarm[2].setting;
        alarm->cAlarmTime[2].cHour = db_alarm[2].hour;
        alarm->cAlarmTime[2].cMinute = db_alarm[2].min;
        alarm->cAlarmTime[3].cSetting = db_alarm[3].setting;
        alarm->cAlarmTime[3].cHour = db_alarm[3].hour;
        alarm->cAlarmTime[3].cMinute = db_alarm[3].min;        
        alarm->cTotalNum = 4;
    }

    return ret;
}

ret_code_t CC_DB_Save_System_HR_Setting(db_sys_hr_setting_t *ptHrSetting)
{
    db_sys_hr_setting_t    _tHrSetting;

    _tHrSetting.eIsHrsEnabled  = CC_BLE_Cme_Get_HeartRateStrapMode();
    _tHrSetting.eIs24HrEnabled = CC_BLE_Cme_Get_24HourHeartRateMode();

    CC_AppSrv_HR_Get24HrPeriod(&_tHrSetting.dw24HrPeriodicMeasurementTime, &_tHrSetting.dw24HrOneMeasurementMaxTime);

    return (CC_DB_Check_And_Write(eSystem, DB_SYS_HR_SETTING, ((uint32_t *) &_tHrSetting), sizeof(db_sys_hr_setting_t)));
}

ret_code_t CC_DB_Read_System_HR_Setting(db_sys_hr_setting_t *ptHrSetting)
{
    uint16_t               _wSize    = 0;
    ret_code_t             _tRetCode = 0;

    db_sys_hr_setting_t    _tDbHrSetting;
    
    if (CC_SUCCESS == (_tRetCode = CC_Fds_Read(eSystem, DB_SYS_HR_SETTING, ((uint32_t *) &_tDbHrSetting), &_wSize)))
    {
        ptHrSetting->eIsHrsEnabled                 = _tDbHrSetting.eIsHrsEnabled;
        ptHrSetting->eIs24HrEnabled                = _tDbHrSetting.eIs24HrEnabled;
        ptHrSetting->dw24HrPeriodicMeasurementTime = _tDbHrSetting.dw24HrPeriodicMeasurementTime;
        ptHrSetting->dw24HrOneMeasurementMaxTime   = _tDbHrSetting.dw24HrOneMeasurementMaxTime;
    }
    
    return _tRetCode;
}

ret_code_t CC_DB_Save_System_Notify(db_sys_notify_enabled_t *notify)
{
    db_sys_notify_enabled_t db_notify;
    ret_code_t ret;

    db_notify.incomming_call_en = notify->incomming_call_en;
    db_notify.incomming_sms_en    = notify->incomming_sms_en;
    db_notify.longsit_en = notify->longsit_en;
    db_notify.lifearm_en = notify->lifearm_en;

    ret = CC_DB_Check_And_Write(eSystem, DB_SYS_NOTIFY, (uint32_t*) &db_notify, sizeof(db_sys_notify_enabled_t));
    
    //Jason, no save HRS and 24 HR data.
    //CC_DB_Save_System_HR_Setting(NULL);    // TBD: to be refined to have independent handler...

    return ret;
}


ret_code_t CC_DB_Read_System_Notify(db_sys_notify_enabled_t *notify)
{
    db_sys_notify_enabled_t db_notify;
    uint16_t rSize;
    ret_code_t ret;

    if(CC_SUCCESS == (ret = CC_Fds_Read(eSystem, DB_SYS_NOTIFY,  (uint32_t*)&db_notify, &rSize)))
    {
        notify->incomming_call_en = db_notify.incomming_call_en;
        notify->incomming_sms_en = db_notify.incomming_sms_en;
        notify->longsit_en = db_notify.longsit_en;
        notify->lifearm_en = db_notify.lifearm_en;
    }

    return ret;
}

ret_code_t CC_DB_Save_System_Unit(CC_Ble_Unit_Info_T *unit)
{
    db_sys_unit_t db_unit;
    ret_code_t ret;

    db_unit.length = unit->cUnitLength;
    db_unit.weight = unit->cUnitWeight;    
        
    ret = CC_DB_Check_And_Write(eSystem, DB_SYS_UNIT, (uint32_t*) &db_unit, sizeof(db_sys_unit_t));
    

    return ret;
}


ret_code_t CC_DB_Read_System_Unit(CC_Ble_Unit_Info_T *unit)
{
    db_sys_unit_t db_unit;
    uint16_t rSize;
    ret_code_t ret;

    if(CC_SUCCESS == (ret = CC_Fds_Read(eSystem, DB_SYS_UNIT,  (uint32_t*)&db_unit, &rSize)))
    {
        unit->cUnitLength = db_unit.length;
        unit->cUnitWeight = db_unit.weight;        
    }

    return ret;
}

ret_code_t CC_DB_Save_Sleep_Info(db_sys_sleep_monitor_t *_pData)
{
    ret_code_t ret;
    ret = CC_DB_Check_And_Write(eSystem, DB_SYS_SLEEP_MONITOR_TIME_SETTING, (uint32_t*) _pData, sizeof(db_sys_sleep_monitor_t));
    return ret;
}

ret_code_t CC_DB_Read_Sleep_Info(db_sys_sleep_monitor_t *_pData)
{  
    ret_code_t ret;
    uint16_t rSize;
    if(CC_SUCCESS == (ret = CC_Fds_Read(eSystem, DB_SYS_SLEEP_MONITOR_TIME_SETTING,  (uint32_t*)_pData, &rSize)))
    {
        return ret;
    }
    
    return ret;
}

ret_code_t CC_DB_Save_System_Static_Gyro_Offset(db_sys_static_gyro_offset_t *_pData)
{

    ret_code_t ret;
    ret = CC_DB_Check_And_Write(eSystem, DB_SYS_STATIC_GYRO_OFFSET, (uint32_t*) _pData, sizeof(db_sys_static_gyro_offset_t));
    return ret;
}

ret_code_t CC_DB_Read_System_Static_Gyro_Offset(db_sys_static_gyro_offset_t *_pData)
{
    ret_code_t ret;
    uint16_t rSize;
    if(CC_SUCCESS == (ret = CC_Fds_Read(eSystem, DB_SYS_STATIC_GYRO_OFFSET,  (uint32_t*)_pData, &rSize)))
    {
        return ret;
    }

    return ret;
}

ret_code_t CC_DB_Save_System_Reserved(uint32_t id)
{
    db_sys_device_info_t db_id;
    ret_code_t ret;

    db_id.id = id;
        
    ret = CC_DB_Check_And_Write(eSystem, DB_SYS_RSVD, (uint32_t*) &db_id, sizeof(db_sys_device_info_t));

    return ret;

}

ret_code_t CC_DB_Read_System_Reserved(uint32_t *id)
{
    db_sys_device_info_t db_id;
    uint16_t rSize;
    ret_code_t ret;

    if(CC_SUCCESS == (ret = CC_Fds_Read(eSystem, DB_SYS_RSVD,  (uint32_t*)&db_id, &rSize)))
    {
        *id = db_id.id;
    }

    return ret;
}


#if 0
ret_code_t CC_DB_Save_System_Dyna_Gyro_Offset()
{
/*
    ret_code_t ret;
        
    ret = CC_DB_Check_And_Write(eSystem, DB_SYS_DYNA_GYRO_OFFSET, (uint32_t*) db_id, sizeof(db_sys_device_info_t));


    return ret;
*/
    return 0;
}
ret_code_t CC_DB_Read_System_Dyna_Gyro_Offset()
{
    return 0;
}
#endif

void CC_DB_System_Save_Request(eDB_System_Record_ID_t sys_id)
{
    _wDBSystem_Save_Flag |= GET_BIT(sys_id);
}


static ret_code_t CC_DB_Save_System_Data(eDB_System_Record_ID_t sys_id, void *data)
{
            
        ret_code_t ret;
    
    
        switch(sys_id)
        {
            case DB_SYS_TIME: 
                ret = CC_DB_Save_System_Time((app_date_time_t*) data);
                break;
    
            case DB_SYS_GENERAL_INFO: 
                ret = CC_DB_Save_System_General_Info((CC_Ble_General_Info_T *)data);
                break;
                
            case DB_SYS_ALARM: 
                ret = CC_DB_Save_System_Alarm((CC_Ble_Clock_Alarm_T *)data);
                break;
    
            case DB_SYS_NOTIFY: 
                ret = CC_DB_Save_System_Notify((db_sys_notify_enabled_t *)data);
                break;
    
            case DB_SYS_UNIT: 
                ret = CC_DB_Save_System_Unit((CC_Ble_Unit_Info_T *) data);
                break;

            case DB_SYS_SLEEP_MONITOR_TIME_SETTING:
                ret = CC_DB_Save_Sleep_Info((db_sys_sleep_monitor_t *)data);
                break;

            case DB_SYS_RSVD: 
                ret = CC_DB_Save_System_Reserved((uint32_t) data);
                break;
    
            case DB_SYS_DYNA_GYRO_OFFSET: 
                 //ret = CC_DB_Save_System_Dyna_Gyro_Offset(data);
                break;
    
            case DB_SYS_STATIC_GYRO_OFFSET: 
                ret = CC_DB_Save_System_Static_Gyro_Offset(data);
                break;
    
            default:
                break;
        }
    
        return ret;

}


ret_code_t CC_DB_Read_System_Data(eDB_System_Record_ID_t sys_id, void *data)
{
            
        ret_code_t ret;
    
    
        switch(sys_id)
        {
            case DB_SYS_TIME: 
                ret = CC_DB_Read_System_Time((app_date_time_t*) data);
                break;
    
            case DB_SYS_GENERAL_INFO: 
                ret = CC_DB_Read_System_General_Info((CC_Ble_General_Info_T *)data);
                break;
                
            case DB_SYS_ALARM: 
                ret = CC_DB_Read_System_Alarm((CC_Ble_Clock_Alarm_T *)data);
                break;
    
            case DB_SYS_HR_SETTING: 
                ret = CC_DB_Read_System_HR_Setting((db_sys_hr_setting_t *)data);
                break;

            case DB_SYS_NOTIFY: 
                ret = CC_DB_Read_System_Notify((db_sys_notify_enabled_t *)data);
                break;
    
            case DB_SYS_UNIT: 
                ret = CC_DB_Read_System_Unit((CC_Ble_Unit_Info_T *) data);
                break;

            case DB_SYS_SLEEP_MONITOR_TIME_SETTING:
                ret = CC_DB_Read_Sleep_Info((db_sys_sleep_monitor_t *)data);
                break;

            case DB_SYS_RSVD: 
                ret = CC_DB_Read_System_Reserved((uint32_t*) data);
                break;
    
            case DB_SYS_DYNA_GYRO_OFFSET: 
                //ret = CC_DB_Read_System_Dyna_Gyro_Offset(data);
                break;
    
            case DB_SYS_STATIC_GYRO_OFFSET: 
                ret = CC_DB_Read_System_Static_Gyro_Offset(data);
                break;
            
            default:
                break;
        }
    
        return ret;

}


void CC_DB_System_Check_Request_And_Save(void)
{

    if(0 != (_wDBSystem_Save_Flag & GET_BIT(DB_SYS_TIME)))
    {
        app_date_time_t systime = app_getSysTime();

        if(CC_SUCCESS == CC_DB_Save_System_Data(DB_SYS_TIME, (app_date_time_t*) &systime))    
            _wDBSystem_Save_Flag &= ~ GET_BIT(DB_SYS_TIME);
    }

    if(0 != (_wDBSystem_Save_Flag & GET_BIT(DB_SYS_GENERAL_INFO)))
    {
        CC_Ble_General_Info_T GeneralInfo;

        CC_BLE_Cmd_GetGeneralInfo(&GeneralInfo,false);

        if(CC_SUCCESS == CC_DB_Save_System_Data(DB_SYS_GENERAL_INFO, (CC_Ble_General_Info_T *)&GeneralInfo))    
            _wDBSystem_Save_Flag &= ~ GET_BIT(DB_SYS_GENERAL_INFO);
    }

    if(0 != (_wDBSystem_Save_Flag & GET_BIT(DB_SYS_ALARM)))
    {
        CC_Ble_Clock_Alarm_T alarm;

        CC_BLE_Cmd_GetClockAlarm(&alarm,false);

        if(CC_SUCCESS == CC_DB_Save_System_Data(DB_SYS_ALARM, (CC_Ble_Clock_Alarm_T *)&alarm))
            _wDBSystem_Save_Flag &= ~ GET_BIT(DB_SYS_ALARM);
    }

    if(0 != (_wDBSystem_Save_Flag & GET_BIT(DB_SYS_NOTIFY)))
    {
        db_sys_notify_enabled_t notify_settings;

        CC_BLE_Cmd_GetNotifySetting(&notify_settings);

        if(CC_SUCCESS == CC_DB_Save_System_Data(DB_SYS_NOTIFY, (db_sys_notify_enabled_t *)&notify_settings))
            _wDBSystem_Save_Flag &= ~ GET_BIT(DB_SYS_NOTIFY);
    }

    if(0 != (_wDBSystem_Save_Flag & GET_BIT(DB_SYS_UNIT)))
    {
        CC_Ble_Unit_Info_T unit_info;

        CC_BLE_Cmd_GetUnitInfo(&unit_info,false);

        if(CC_SUCCESS == CC_DB_Save_System_Data(DB_SYS_UNIT, (CC_Ble_Unit_Info_T *) &unit_info))
            _wDBSystem_Save_Flag &= ~ GET_BIT(DB_SYS_UNIT);

    }


    if(0 != (_wDBSystem_Save_Flag & GET_BIT(DB_SYS_SLEEP_MONITOR_TIME_SETTING)))
    {
        db_sys_sleep_monitor_t _Sleep_Time_Period;
             CC_BLE_Cmd_GetSleepTimeSetting(&_Sleep_Time_Period,false);
        if(CC_SUCCESS == CC_DB_Save_System_Data(DB_SYS_SLEEP_MONITOR_TIME_SETTING, (db_sys_sleep_monitor_t *) &_Sleep_Time_Period))
            _wDBSystem_Save_Flag &= ~ GET_BIT(DB_SYS_SLEEP_MONITOR_TIME_SETTING);
    }

    if(0 != (_wDBSystem_Save_Flag & GET_BIT(DB_SYS_STATIC_GYRO_OFFSET)))
    {
        db_sys_static_gyro_offset_t _stCal_Data;
        CC_GetStaticCalibrationInfo(&_stCal_Data);
        if(CC_SUCCESS == CC_DB_Save_System_Data(DB_SYS_STATIC_GYRO_OFFSET, (db_sys_static_gyro_offset_t *) &_stCal_Data))
            _wDBSystem_Save_Flag &= ~ GET_BIT(DB_SYS_STATIC_GYRO_OFFSET);
    }


/*    
    if(0 != (_wDBSystem_Save_Flag & GET_BIT(DB_SYS_RSVD)))
    {
        if(CC_SUCCESS == CC_DB_Save_System_Data(DB_SYS_RSVD, (uint32_t) data))
            _wDBSystem_Save_Flag &= ~ GET_BIT(DB_SYS_RSVD);
    }

    if(0 != (_wDBSystem_Save_Flag & GET_BIT(DB_SYS_DYNA_GYRO_OFFSET)))
    {
        _wDBSystem_Save_Flag &= ~ GET_BIT(DB_SYS_DYNA_GYRO_OFFSET);
    }

*/
}
#ifdef BatteryLog_EN
void _DB_TEST_WRITE_BATTERYLOG(void)
{
#if 1
     db_mixlog_t m_db_battery;
     ret_code_t ret;                
    app_date_time_t curr_time;
     
    CC_MainGet_CurrentTime(&curr_time);
     m_db_battery.time.month = curr_time.month;
     m_db_battery.time.day = curr_time.day;
     m_db_battery.time.hour = 5;
     m_db_battery.time.min = 59;

     for(int i=0;i<=360;i++)
     {
        m_db_battery.battery[i].BatteryLevel = 100;
        m_db_battery.battery[i].BatteryADCValue = 775;
     }

     if(FDS_SUCCESS != (ret = CC_Save_Record(eMixLog, (uint32_t*)&m_db_battery, sizeof(db_mixlog_t))))
        TracerInfo("err1: 0x%x\r\n", ret);


     m_db_battery.time.month = curr_time.month;
     m_db_battery.time.day = curr_time.day;
     m_db_battery.time.hour = 11;
     m_db_battery.time.min = 59;

     for(int i=0;i<=360;i++)
     {
        m_db_battery.battery[i].BatteryLevel = 99;
        m_db_battery.battery[i].BatteryADCValue = 774;
     }

     if(FDS_SUCCESS != (ret = CC_Save_Record(eMixLog, (uint32_t*)&m_db_battery, sizeof(db_mixlog_t))))
        TracerInfo("err1: 0x%x\r\n", ret);


         m_db_battery.time.month = curr_time.month;
     m_db_battery.time.day = curr_time.day;
     m_db_battery.time.hour = 17;
     m_db_battery.time.min = 59;

     for(int i=0;i<=360;i++)
     {
        m_db_battery.battery[i].BatteryLevel = 98;
        m_db_battery.battery[i].BatteryADCValue = 773;
     }

     if(FDS_SUCCESS != (ret = CC_Save_Record(eMixLog, (uint32_t*)&m_db_battery, sizeof(db_mixlog_t))))
        TracerInfo("err1: 0x%x\r\n", ret);

     m_db_battery.time.month = curr_time.month;
     m_db_battery.time.day = curr_time.day;
     m_db_battery.time.hour = 23;
     m_db_battery.time.min = 59;

     for(int i=0;i<=360;i++)
     {
        m_db_battery.battery[i].BatteryLevel = 97;
        m_db_battery.battery[i].BatteryADCValue = 772;
     }

     if(FDS_SUCCESS != (ret = CC_Save_Record(eMixLog, (uint32_t*)&m_db_battery, sizeof(db_mixlog_t))))
        TracerInfo("err1: 0x%x\r\n", ret);

#endif
}
    
void _DB_TEST_READ_BATTERYLOG(void)
{
#if 0
    cc6801_ClockDelayMs(1000);
    uint16_t total;
    db_mixlog_t m_db_battery;
    uint16_t rsize;

    app_date_time_t curr_time;
     
    CC_MainGet_CurrentTime(&curr_time);

    CC_Read_DayofRecoordLen(eMixLog, curr_time.dayofweek, &total);
    TracerInfo("total = %d\r\n",total);

     for (int i =1 ; i <=total; i++ )
     {
        CC_Read_Record(eMixLog,curr_time.dayofweek , i, (uint32_t*)&m_db_battery, &rsize);
        TracerInfo("=========[%d]=============\r\n",i);
        TracerInfo("time y %d/%d/%d %d \r\n",m_db_battery.time.month,
                                                              m_db_battery.time.day,
                                                              m_db_battery.time.hour,
                                                              m_db_battery.time.min);
         
        TracerInfo("battery level %d/%d \r\n",m_db_battery.battery[0].BatteryLevel,
                                                              m_db_battery.battery[0].BatteryLevel);


        
        TracerInfo("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\r\n");
     }  
     //NRF_LOG_FLUSH();
#endif

}
#endif

extern app_sys_time_t m_SystemTime;//for test

void _DB_TEST_WRITE_Hrm(void)
    {
    db_heartrate_t m_db_hrm;
    app_date_time_t curr_time;
    ret_code_t ret;                
    //uint8_t dayofweek;
    //static uint32_t _bTemp = 0;

    m_SystemTime.is_updated = true; 
    CC_MainGet_CurrentTime(&curr_time);
    m_db_hrm.time.year = (uint8_t)curr_time.year-2000;
    m_db_hrm.time.month = curr_time.month;
    m_db_hrm.time.day = curr_time.day;
    m_db_hrm.time.hour= curr_time.hours;
    m_db_hrm.time.min = curr_time.minutes;
    m_db_hrm.data.hr = 100;

    if(FDS_SUCCESS != (ret = CC_Save_Record(eHrm, (uint32_t*)&m_db_hrm, sizeof(db_heartrate_t))))
        TracerInfo("err1: 0x%x\r\n", ret);

    
    m_db_hrm.time.year = (uint8_t)curr_time.year-2000;
    m_db_hrm.time.month = curr_time.month;
    m_db_hrm.time.day = curr_time.day;
    m_db_hrm.time.hour= curr_time.hours+1;
    m_db_hrm.time.min = curr_time.minutes+1;
    m_db_hrm.data.hr = 100;

    if(FDS_SUCCESS != (ret = CC_Save_Record(eHrm, (uint32_t*)&m_db_hrm, sizeof(db_heartrate_t))))
        TracerInfo("err1: 0x%x\r\n", ret);

    
    }

void _DB_TEST_READ_Hrm(void)
{
    cc6801_ClockDelayMs(1000);
    uint16_t total;
    db_heartrate_t   hrm_db_data;        
    uint16_t rsize;

    app_date_time_t curr_time;
     
    CC_MainGet_CurrentTime(&curr_time);

    CC_Read_DayofRecoordLen(eHrm, curr_time.dayofweek, &total);
    TracerInfo("total = %d\r\n",total);
     for (int i =1 ; i <=total; i++ )
     {
        CC_Read_Record(eHrm,curr_time.dayofweek , i, (uint32_t*)&hrm_db_data, &rsize);
        TracerInfo("=========[%d]=============\r\n",i);
        TracerInfo("time y %d/%d/%d  \r\n",hrm_db_data.time.year,
                                                              hrm_db_data.time.month,
                                                              hrm_db_data.time.day);
         
        TracerInfo("time h %d/%d \r\n",hrm_db_data.time.hour,
                                                              hrm_db_data.time.min);

        TracerInfo("HR = %d\r\n",hrm_db_data.data.hr);    

        
        TracerInfo("total count = %d\r\n",i);
        TracerInfo("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\r\n");
    }
     //NRF_LOG_FLUSH();
}

void _DB_TEST_READ_Swim(void)
{
    cc6801_ClockDelayMs(1000);
    uint16_t total;
    db_swimming_t   swimming_db_data;        
    uint16_t rsize;

    app_date_time_t curr_time;
     
    CC_MainGet_CurrentTime(&curr_time);
     

    CC_Read_DayofRecoordLen(eSwim, curr_time.dayofweek, &total);
    TracerInfo("total = %d\r\n",total);

     for (int i =1 ; i <=total; i++ )
     {
        CC_Read_Record(eSwim,curr_time.dayofweek , i, (uint32_t*)&swimming_db_data, &rsize);
        TracerInfo("=========[%d]=============\r\n",i);
        TracerInfo("total count = %d\r\n",swimming_db_data.section.total_recordcount);
        TracerInfo("section_num  = %d\r\n",swimming_db_data.section.section_num);
        TracerInfo("poolsize= %d\r\n",swimming_db_data.section.uInfo.bInfo.swim_pool_sizes);
        TracerInfo("type = %d\r\n",swimming_db_data.section.uInfo.bInfo.swimming_type);
        
        TracerInfo("-----------------------------\r\n");
        TracerInfo("time y %d/%d/%d  \r\n",swimming_db_data.start_time.year,
                                                              swimming_db_data.start_time.month,
                                                              swimming_db_data.start_time.day);
         
        TracerInfo("time h %d/%d/%d  \r\n",swimming_db_data.start_time.hour,
                                                              swimming_db_data.start_time.min,
                                                              swimming_db_data.start_time.sec);

        TracerInfo("lap  = %d\r\n",swimming_db_data.start_time.lap_number);    

        
        TracerInfo("total count = %d\r\n",swimming_db_data.endofperiod.swimming_count);
        TracerInfo("section_num  = %d\r\n",swimming_db_data.endofperiod.time_period);
        TracerInfo("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\r\n");
     }  
     //NRF_LOG_FLUSH();
        
}

void _DB_TEST_WRITE_Swim(void)
{
    
    do
    {
        db_swimming_t m_db_swim;
        app_date_time_t curr_time;
        uint8_t i;
        ret_code_t ret;                
        uint8_t dayofweek;

        static uint32_t _bTemp = 0;
        
        CC_MainGet_CurrentTime(&curr_time);
             

        for(dayofweek = 0; dayofweek <1 ; dayofweek++)
        {
            for(i = 0; i < 59; i++)            
            {
               
                m_db_swim.section.total_recordcount  = i+1;
                m_db_swim.section.uInfo.bInfo.swim_pool_sizes = eSWIM_50M;
                m_db_swim.section.uInfo.bInfo.swimming_type = 4;
                m_db_swim.section.section_num = 1;

                m_db_swim.start_time.year = (uint8_t)curr_time.year-2000;
                m_db_swim.start_time.month = curr_time.month;
                m_db_swim.start_time.day = curr_time.day;
                m_db_swim.start_time.hour= curr_time.hours;

                m_db_swim.start_time.min= curr_time.minutes+(_bTemp/60);
                m_db_swim.start_time.sec= curr_time.seconds+(_bTemp%60);
                
                m_db_swim.start_time.lap_number = i+1;
               
                m_db_swim.endofperiod.swimming_count = 20;
                m_db_swim.endofperiod.time_period = 30;
                
                ++_bTemp;
            if(FDS_SUCCESS != (ret = CC_Save_Record(eSwim, (uint32_t*)&m_db_swim, sizeof(db_swimming_t))))
            TracerInfo("err1: 0x%x\r\n", ret);
            }
            
        }
    }while(0);
    //NRF_LOG_FLUSH();
        
    
}
void _DB_TEST_Read_Hrm(void)
{
    cc6801_ClockDelayMs(1000);
    uint16_t total;
                db_heartrate_t hr_db_data;
            
            uint16_t rsize;

     CC_Read_DayofRecoordLen(eHrm, 2, &total);
     TracerInfo("total = %d\r\n",total);

     for (int i =1 ; i <=total; i++ )
     {
        CC_Read_Record(eHrm, 2, i, (uint32_t*)&hr_db_data, &rsize);
        TracerInfo("HR = %d\r\n",hr_db_data.data.hr);
        TracerInfo("size = %d\r\n",rsize);
     }    
}

void _DB_TEST_WRITE_SLEEP(void)
{
    do
    {
        db_sleep_t m_db_sleep;
        app_date_time_t curr_time;
        ret_code_t ret;                
        uint8_t dayofweek;
        CC_MainGet_CurrentTime(&curr_time);
    
        for(dayofweek = 0; dayofweek <1 ; dayofweek++)
        {
          
                //m_db_sleep.section.total_recordcount  = i+1;

                m_db_sleep.detect_time.year = (uint8_t)curr_time.year-2000;
                m_db_sleep.detect_time.month = curr_time.month;
                m_db_sleep.detect_time.day = curr_time.day;
                m_db_sleep.detect_time.hour= curr_time.hours;
                m_db_sleep.detect_time.min= curr_time.minutes;
                m_db_sleep.detect_time.sec= curr_time.seconds;
                
                m_db_sleep.detect_time.sleep_state = 1;
               
                m_db_sleep.period.period_min= 10;
                 
                 if(FDS_SUCCESS != (ret = CC_Save_Record(eSleep, (uint32_t*)&m_db_sleep, sizeof(db_sleep_t))))
                TracerInfo("err1: 0x%x\r\n", ret);


                m_db_sleep.detect_time.year = (uint8_t)curr_time.year-2000;
                m_db_sleep.detect_time.month = curr_time.month;
                m_db_sleep.detect_time.day = curr_time.day;
                m_db_sleep.detect_time.hour= m_db_sleep.detect_time.hour+1;
                m_db_sleep.detect_time.min= curr_time.minutes;
                m_db_sleep.detect_time.sec= curr_time.seconds;
                
                m_db_sleep.detect_time.sleep_state = 2;
               
                m_db_sleep.period.period_min= 60;
                 
                 if(FDS_SUCCESS != (ret = CC_Save_Record(eSleep, (uint32_t*)&m_db_sleep, sizeof(db_sleep_t))))
                TracerInfo("err1: 0x%x\r\n", ret);

                m_db_sleep.detect_time.year = (uint8_t)curr_time.year-2000;
                m_db_sleep.detect_time.month = curr_time.month;
                m_db_sleep.detect_time.day = curr_time.day;
                m_db_sleep.detect_time.hour= m_db_sleep.detect_time.hour+1;
                m_db_sleep.detect_time.min= curr_time.minutes;
                m_db_sleep.detect_time.sec= curr_time.seconds;
                
                m_db_sleep.detect_time.sleep_state = 1;
               
                m_db_sleep.period.period_min= 60;
                 
                 if(FDS_SUCCESS != (ret = CC_Save_Record(eSleep, (uint32_t*)&m_db_sleep, sizeof(db_sleep_t))))
                TracerInfo("err1: 0x%x\r\n", ret);
 
                m_db_sleep.detect_time.year = (uint8_t)curr_time.year-2000;
                m_db_sleep.detect_time.month = curr_time.month;
                m_db_sleep.detect_time.day = curr_time.day;
                m_db_sleep.detect_time.hour= m_db_sleep.detect_time.hour+1;
                m_db_sleep.detect_time.min= curr_time.minutes;
                m_db_sleep.detect_time.sec= curr_time.seconds;
                
                m_db_sleep.detect_time.sleep_state = 0;
               
                m_db_sleep.period.period_min= 60;
                 
                 if(FDS_SUCCESS != (ret = CC_Save_Record(eSleep, (uint32_t*)&m_db_sleep, sizeof(db_sleep_t))))
                TracerInfo("err1: 0x%x\r\n", ret);


        }
    }while(0);

    
}



void _DB_TEST(void)
{
#if 0    
    uint16_t fileid = 0x0001;
    uint16_t recid = 0x0001;
    uint32_t pData[2] = {0x11111111,0x22222222};

    uint32_t pData_Up[2] = {0x66666666,0x77777777};
    uint32_t pData_Up1[3] = {0x88888888,0x99999999,0x12121212};

    uint16_t wLen =0;
    uint32_t _pReadData[2];
    uint32_t _pReadData1[3];
     uint32_t _pReadData_Up1[3];


    //NRF_LOG_FLUSH();

    

    if (CC_SUCCESS ==CC_Fds_Find_And_Delete(fileid,recid))
    {
        TracerInfo("_DB_TEST Delete Success \r\n");    
    }
    else
    {
        TracerInfo("_DB_TEST Delete Failed \r\n");    
    }

   
    if (CC_SUCCESS == CC_Fds_Read(fileid,recid,_pReadData1,&wLen))
    {
        TracerInfo("_DB_TEST Read Success \r\n");    
        TracerInfo("Data=0x%8x \r\n",((uint32_t *) _pReadData1)[0]);
        TracerInfo("Data=0x%8x \r\n",((uint32_t *) _pReadData1)[1]);
        TracerInfo("Data=0x%8x \r\n",((uint32_t *) _pReadData1)[2]);
        TracerInfo("_DB_TEST Read Len=[%d] \r\n",wLen);
    }
    else
    {
        TracerInfo("_DB_TEST Read Failed \r\n");    
    }

    _bWrite_Flag = 0;
    if (CC_SUCCESS ==CC_Fds_Write(fileid,recid,pData,sizeof(pData)/(sizeof(uint32_t))))
    {
        TracerInfo("_DB_TEST Write Success \r\n");    
    }
    else
    {
        TracerInfo("_DB_TEST Write Failed \r\n");    
    }
    while(_bWrite_Flag == 0)
    ;    
    
    
    if (CC_SUCCESS == CC_Fds_Read(fileid,recid,_pReadData1,&wLen))
    {
        TracerInfo("_DB_TEST Read Success \r\n");    
        TracerInfo("Data=0x%8x \r\n",((uint32_t *) _pReadData1)[0]);
        TracerInfo("Data=0x%8x \r\n",((uint32_t *) _pReadData1)[1]);
        TracerInfo("Data=0x%8x \r\n",((uint32_t *) _pReadData1)[2]);
        TracerInfo("_DB_TEST Read Len=[%d] \r\n",wLen);          
    }
    else
    {
        TracerInfo("_DB_TEST Read Failed \r\n");    
    }

    _bUpdate_Flag = 0;


    if (CC_SUCCESS ==CC_Fds_Update(fileid,recid,pData_Up,sizeof(pData_Up)/(sizeof(uint32_t))))
    {
        TracerInfo("_DB_TEST Update Success \r\n");    
    }
    else
    {
        TracerInfo("_DB_TEST Update Failed \r\n");    
    }

    while(_bUpdate_Flag == 0)
    ;  
    if (CC_SUCCESS == CC_Fds_Read(fileid,recid,_pReadData1,&wLen))
    {
        TracerInfo("_DB_TEST Read Success \r\n");    
        TracerInfo("Data=0x%8x \r\n",((uint32_t *) _pReadData1)[0]);
        TracerInfo("Data=0x%8x \r\n",((uint32_t *) _pReadData1)[1]);
        TracerInfo("Data=0x%8x \r\n",((uint32_t *) _pReadData1)[2]);
        TracerInfo("_DB_TEST Read Len=[%d] \r\n",wLen);    
    }
    else
    {
        TracerInfo("_DB_TEST Read Failed \r\n");    
    }
#endif
    #if 0
    CC_Fds_Write(fileid,recid+1,pData1,sizeof(pData1)/(sizeof(uint32_t)));
    
    for (int j= 0;j<2;j++)
    {
        
        
        TracerInfo("Data=0x%8x \r\n",((uint32_t *) _pReadData1)[0]);
        TracerInfo("Data=0x%8x \r\n",((uint32_t *) _pReadData1)[1]);
        TracerInfo("Data=0x%8x \r\n",((uint32_t *) _pReadData1)[2]);
        TracerInfo("_DB_TEST Read Len=[%d] \r\n",wLen);
        //
        //
    }    

    
    CC_Fds_Update(fileid,recid,pData_Up,sizeof(pData_Up)/(sizeof(uint32_t)));
    CC_Fds_Update(fileid,recid+1,pData_Up1,sizeof(pData_Up1)/(sizeof(uint32_t)));
    
    for (int j= 0;j<2;j++)
    {
        CC_Fds_FindFinle_And_Delete(fileid);
        CC_Fds_Read(fileid,recid+j,_pReadData_Up1,&wLen);
        TracerInfo("Data=0x%8x \r\n",((uint32_t *) _pReadData_Up1)[0]);
        TracerInfo("Data=0x%8x \r\n",((uint32_t *) _pReadData_Up1)[1]);
        TracerInfo("Data=0x%8x \r\n",((uint32_t *) _pReadData_Up1)[2]);
        TracerInfo("_DB_TEST Read Len=[%d] \r\n",wLen);
        
        //CC_Fds_Find_And_Delete(fileid,recid+j);
    }    
 
    
    #endif
    //NRF_LOG_FLUSH();
    
#endif
}




void _DB_TEST_Write_System_DB(void)
{
#if 0
            db_heartrate_t m_db_hrm;    
            db_pedometer_t m_db_pedo;
            app_date_time_t curr_time;
            uint8_t i;
            ret_code_t ret; 
            //uint8_t dayofweek;
            app_date_time_t sys_time;
            CC_Ble_General_Info_T _eGeneralInfo;
            CC_Ble_Clock_Set_T cAlarmTime[4];
            db_sys_notify_enabled_t  _cBleNotifySettings;
            CC_Ble_Unit_Info_T _eUnitInfo;

            CC_MainGet_CurrentTime(&curr_time);

            TracerInfo("dayofweek(%d) \r\n",curr_time.dayofweek);

            m_SystemTime.is_updated = true; 



            db_sys_first_use_record_t db_time;

            sys_time.year = 2017;
            sys_time.month = 4;
            sys_time.day = 3;
            sys_time.hours = 23;
            sys_time.minutes = 10;
            sys_time.seconds = 5;
            sys_time.dayofweek = 2;

            //if(FDS_SUCCESS != (ret = CC_Fds_Write(eSystem, DB_SYS_TIME, (uint32_t*) &db_time, sizeof(db_sys_first_use_record_t))))
            if(FDS_SUCCESS != (ret = CC_DB_Save_System_Data(DB_SYS_TIME, (uint32_t*) &sys_time)))
            {
                TracerInfo("err0: 0x%x\r\n", ret);
            }




            _eGeneralInfo.cAge = 20;
            _eGeneralInfo.cGender = 0x01;
            _eGeneralInfo.cHeight = 170;
            _eGeneralInfo.cStride_Lenght = 10;
            _eGeneralInfo.cSwim_Pool_Size = 30;
            _eGeneralInfo.cWeight = 50;

            //if(FDS_SUCCESS != (ret = CC_Fds_Write(eSystem, DB_SYS_GENERAL_INFO, (uint32_t*) &_eGeneralInfo, sizeof(CC_Ble_General_Info_T))))
            if(FDS_SUCCESS != (ret = CC_DB_Save_System_Data(DB_SYS_GENERAL_INFO, (uint32_t*) &_eGeneralInfo)))
            {
                TracerInfo("err1: 0x%x\r\n", ret);            
            }



            cAlarmTime[0].cSetting = 0x81;
            cAlarmTime[0].cHour = 1;
            cAlarmTime[0].cMinute = 10;
            cAlarmTime[1].cSetting = 0x82;
            cAlarmTime[1].cHour = 2;
            cAlarmTime[1].cMinute = 20;
            cAlarmTime[2].cSetting = 0x83;
            cAlarmTime[2].cHour = 3;
            cAlarmTime[2].cMinute = 30;
            cAlarmTime[3].cSetting = 0x84;
            cAlarmTime[3].cHour = 4;
            cAlarmTime[3].cMinute = 40;            

            //if(FDS_SUCCESS != (ret = CC_Fds_Write(eSystem, DB_SYS_ALARM, (uint32_t*) &cAlarmTime, sizeof(CC_Ble_Clock_Set_T)*4)))
            if(FDS_SUCCESS != (ret = CC_DB_Save_System_Data(DB_SYS_ALARM, (uint32_t*) &cAlarmTime)))
            {
                TracerInfo("err2: 0x%x\r\n", ret);        
            }




            _cBleNotifySettings.incomming_call_en = 1;
            _cBleNotifySettings.incomming_sms_en = 0;
            _cBleNotifySettings.lifearm_en = 1;
            _cBleNotifySettings.longsit_en = 0;

            //if(FDS_SUCCESS != (ret = CC_Fds_Write(eSystem, DB_SYS_NOTIFY, (uint32_t*) &_cBleNotifySettings, sizeof(db_sys_notify_enabled_t))))
            if(FDS_SUCCESS != (ret = CC_DB_Save_System_Data(DB_SYS_NOTIFY, (uint32_t*) &_cBleNotifySettings)))
            {
                TracerInfo("err3: 0x%x\r\n", ret);    
            }
        

            _eUnitInfo.cUnitLength = 10;
            //_eUnitInfo.cUnitSwimLen = 20;
            _eUnitInfo.cUnitWeight = 30;

            //if(FDS_SUCCESS != (ret = CC_Fds_Write(eSystem, DB_SYS_UNIT, (uint32_t*) &_eUnitInfo, sizeof(CC_Ble_Unit_Info_T))))
            if(FDS_SUCCESS != (ret = CC_DB_Save_System_Data(DB_SYS_UNIT, (uint32_t*) &_eUnitInfo)))
            {
                TracerInfo("err4: 0x%x\r\n", ret);    
            }

            CC_MainGet_CurrentTime(&curr_time);
    
            //if(FDS_SUCCESS != (ret = CC_FindFile_And_DeleteFile(eHrm, APP_WEEK_TUESDAY)))
                //TracerInfo("err0: 0x%x\r\n", ret);
    
            //for(dayofweek = 0; dayofweek < 7; dayofweek++)
            //{
                for(i = 0; i < 10; i++)
                {
                    m_db_hrm.time.year = curr_time.year - 2000;
                    m_db_hrm.time.month = curr_time.month;
                    m_db_hrm.time.day = curr_time.day;//+dayofweek;
                    m_db_hrm.time.hour = curr_time.hours;
                    m_db_hrm.time.min = curr_time.minutes+i;
                    m_db_hrm.data.hr = 65+i;
    
                    TracerInfo("init_year=%d \r\n",m_db_hrm.time.year);
                    TracerInfo("init_month=%d \r\n",m_db_hrm.time.month);
                    TracerInfo("init_day=%d \r\n",m_db_hrm.time.day);
                    TracerInfo("init_hour=%d \r\n",m_db_hrm.time.hour);
                    TracerInfo("init_min=%d \r\n",m_db_hrm.time.min);
                    TracerInfo("init_hr=%d \r\n",m_db_hrm.data.hr);    
                    TracerInfo("init_dayofweek=%d \r\n",curr_time.dayofweek);
    
                    if(FDS_SUCCESS != (ret = CC_Save_Record(eHrm, (uint32_t*)&m_db_hrm, sizeof(db_heartrate_t))))
                        TracerInfo("err1: 0x%x\r\n", ret);
    
    
                    m_db_pedo.time.year = curr_time.year - 2000;
                    m_db_pedo.time.month = curr_time.month;
                    m_db_pedo.time.day = curr_time.day;//+dayofweek;
                    m_db_pedo.time.hour = curr_time.hours;
                    m_db_pedo.time.min = curr_time.minutes+i;
                    m_db_pedo.ped_state.state = 1;
                    m_db_pedo.ped_single.count = 0x1122;
                    m_db_pedo.endoftime.hour = 1;
                    m_db_pedo.endoftime.min = 20;
        
                    TracerInfo("pedometer_init:\r\n");
                    TracerInfo("init_year=%d \r\n",m_db_pedo.time.year);
                    TracerInfo("init_month=%d \r\n",m_db_pedo.time.month);
                    TracerInfo("init_day=%d \r\n",m_db_pedo.time.day);
                    TracerInfo("init_hour=%d \r\n",m_db_pedo.time.hour);
                    TracerInfo("init_min=%d \r\n",m_db_pedo.time.min);
                    TracerInfo("init_state=%d \r\n",m_db_pedo.ped_state.state);    
                    TracerInfo("init_singler=%d \r\n",m_db_pedo.ped_single.count);    
                    TracerInfo("init_end_hour=%d \r\n",m_db_pedo.endoftime.hour); 
                    TracerInfo("init_end_min=%d \r\n",m_db_pedo.endoftime.min);    
                    TracerInfo("init_dayofweek=%d \r\n",curr_time.dayofweek);
    
                    if(FDS_SUCCESS != (ret = CC_Save_Record(ePed, (uint32_t*)&m_db_pedo, sizeof(db_pedometer_t))))
                        TracerInfo("err2: 0x%x\r\n", ret);
    
                }
 #endif               
            //}
    /*
            for(uint8_t dayofweek = 0; dayofweek < 7; dayofweek++)
            {
                uint16_t len;
                CC_Read_DayofRecoordLen(eHrm, dayofweek, &len);
                
                TracerInfo("dayofweek(%d)=%d \r\n",dayofweek, len);
            }
    */
        
}


void _DB_TEST_Read_System_DB(void)
{
#if 0
                //uint32_t rdata[3];
                uint16_t rec_len;
                uint16_t wsize;
                //db_heartrate_t rdata[20];
                //db_pedometer_t rdata[20];
                uint32_t rdata[20][20];
                //uint32_t rdata[20];
                uint8_t i;
                ret_code_t ret;

                app_date_time_t sys_time;
                CC_Ble_General_Info_T _eGeneralInfo;
                CC_Ble_Clock_Set_T cAlarmTime[4];
                db_sys_notify_enabled_t  _cBleNotifySettings;
                CC_Ble_Unit_Info_T _eUnitInfo;



                if(FDS_SUCCESS == (ret = CC_DB_Read_System_Data(DB_SYS_TIME, &sys_time)))
                {
                    TracerInfo("year=%d \r\n",sys_time.year);
                    TracerInfo("month=%d \r\n",sys_time.month);
                    TracerInfo("day=%d \r\n",sys_time.day);
                    TracerInfo("hour=%d \r\n",sys_time.hours);
                    TracerInfo("min=%d \r\n",sys_time.minutes);
                    TracerInfo("sec=%d \r\n",sys_time.seconds);    
                    TracerInfo("dayofweek=%d \r\n",sys_time.dayofweek);                        
                }
                else
                    TracerInfo("err6: 0x%x\r\n", ret);    


                if(FDS_SUCCESS == (ret = CC_DB_Read_System_Data(DB_SYS_GENERAL_INFO, &_eGeneralInfo)))
                {

                    TracerInfo("height=%d \r\n",_eGeneralInfo.cHeight);
                    TracerInfo("weight=%d \r\n",_eGeneralInfo.cWeight);
                    TracerInfo("age=%d \r\n",_eGeneralInfo.cAge);    
                    TracerInfo("gender=%d \r\n",_eGeneralInfo.cGender);
                    TracerInfo("length=%d \r\n",_eGeneralInfo.cStride_Lenght);
                    TracerInfo("size=%d \r\n",_eGeneralInfo.cSwim_Pool_Size);
                }
                else
                    TracerInfo("err7: 0x%x\r\n", ret);    


                if(FDS_SUCCESS == (ret = CC_DB_Read_System_Data(DB_SYS_ALARM, &cAlarmTime)))
                {

                    TracerInfo("setting[0]=%d \r\n",cAlarmTime[0].cSetting);
                    TracerInfo("hour[0]=%d \r\n",cAlarmTime[0].cHour);
                    TracerInfo("min[0]=%d \r\n",cAlarmTime[0].cMinute);    
                    TracerInfo("setting[1]=%d \r\n",cAlarmTime[1].cSetting);
                    TracerInfo("hour[1]=%d \r\n",cAlarmTime[1].cHour);
                    TracerInfo("min[1]=%d \r\n",cAlarmTime[1].cMinute);    
                    TracerInfo("setting[2]=%d \r\n",cAlarmTime[2].cSetting);
                    TracerInfo("hour[2]=%d \r\n",cAlarmTime[2].cHour);
                    TracerInfo("min[2]=%d \r\n",cAlarmTime[2].cMinute);    
                    TracerInfo("setting[3]=%d \r\n",cAlarmTime[3].cSetting);
                    TracerInfo("hour[3]=%d \r\n",cAlarmTime[3].cHour);
                    TracerInfo("min[3]=%d \r\n",cAlarmTime[3].cMinute);                        
                }
                else
                    TracerInfo("err8: 0x%x\r\n", ret);    


                if(FDS_SUCCESS == (ret = CC_DB_Read_System_Data(DB_SYS_NOTIFY, &_cBleNotifySettings)))
                {

                    TracerInfo("call=%d \r\n",_cBleNotifySettings.incomming_call_en);
                    TracerInfo("sms=%d \r\n",_cBleNotifySettings.incomming_sms_en);
                    TracerInfo("longsit=%d \r\n",_cBleNotifySettings.longsit_en);    
                    TracerInfo("liftarm=%d \r\n",_cBleNotifySettings.lifearm_en);                        
                }
                else
                    TracerInfo("err9: 0x%x\r\n", ret);                    



                if(FDS_SUCCESS == (ret = CC_DB_Read_System_Data(DB_SYS_UNIT, &_eUnitInfo)))
                {

                    TracerInfo("length=%d \r\n",_eUnitInfo.cUnitLength);
                    TracerInfo("weight=%d \r\n",_eUnitInfo.cUnitWeight);
                }
                else
                    TracerInfo("err10: 0x%x\r\n", ret);                    
#endif
#if 0
// Dump System DB:
    ret_code_t    ret;
    app_date_time_t    sys_time;
    CC_Ble_General_Info_T GeneralInfo;
    CC_Ble_Clock_Alarm_T alarm;
    db_sys_notify_enabled_t notify_settings;
    CC_Ble_Unit_Info_T unit_info;

    // System time:
    //app_date_time_t systime = app_getSysTime();

    if(FDS_SUCCESS == (ret = CC_DB_Read_System_Data(DB_SYS_TIME, &sys_time)))
    {
        TracerInfo("year=%d \r\n",sys_time.year);
        TracerInfo("month=%d \r\n",sys_time.month);
        TracerInfo("day=%d \r\n",sys_time.day);
        TracerInfo("hour=%d \r\n",sys_time.hours);
        TracerInfo("min=%d \r\n",sys_time.minutes);
        TracerInfo("sec=%d \r\n",sys_time.seconds);    
        TracerInfo("dayofweek=%d \r\n",sys_time.dayofweek);                        
    }
    else
        TracerInfo("err6: 0x%x\r\n", ret);    


    // General information:
    //CC_BLE_Cmd_GetGeneralInfo(&GeneralInfo);                
    if(FDS_SUCCESS == (ret = CC_DB_Read_System_Data(DB_SYS_GENERAL_INFO, &GeneralInfo)))
    {
        TracerInfo("height=%d \r\n",GeneralInfo.cHeight);
        TracerInfo("weight=%d \r\n",GeneralInfo.cWeight);
        TracerInfo("age=%d \r\n",GeneralInfo.cAge);    
        TracerInfo("gender=%d \r\n",GeneralInfo.cGender);
        TracerInfo("length=%d \r\n",GeneralInfo.cStride_Lenght);
        TracerInfo("size=%d \r\n",GeneralInfo.cSwim_Pool_Size);
    }
    else
        TracerInfo("err7: 0x%x\r\n", ret);    
    

    // Clock alarm:
    //CC_BLE_Cmd_GetClockAlarm(&alarm);

    if(FDS_SUCCESS == (ret = CC_DB_Read_System_Data(DB_SYS_ALARM, &alarm)))
    {
    
        TracerInfo("setting[0]=0x%x \r\n",alarm.cAlarmTime[0].cSetting);
        TracerInfo("hour[0]=%d \r\n",alarm.cAlarmTime[0].cHour);
        TracerInfo("min[0]=%d \r\n",alarm.cAlarmTime[0].cMinute);    
        TracerInfo("setting[1]=0x%x \r\n",alarm.cAlarmTime[1].cSetting);
        TracerInfo("hour[1]=%d \r\n",alarm.cAlarmTime[1].cHour);
        TracerInfo("min[1]=%d \r\n",alarm.cAlarmTime[1].cMinute);    
        TracerInfo("setting[2]=0x%x \r\n",alarm.cAlarmTime[2].cSetting);
        TracerInfo("hour[2]=%d \r\n",alarm.cAlarmTime[2].cHour);
        TracerInfo("min[2]=%d \r\n",alarm.cAlarmTime[2].cMinute);    
        TracerInfo("setting[3]=0x%x \r\n",alarm.cAlarmTime[3].cSetting);
        TracerInfo("hour[3]=%d \r\n",alarm.cAlarmTime[3].cHour);
        TracerInfo("min[3]=%d \r\n",alarm.cAlarmTime[3].cMinute);                        
    }
    else
        TracerInfo("err8: 0x%x\r\n", ret);    

    
    // Notify settings:
    //CC_BLE_Cmd_GetNotifySetting(&notify_settings);
    if(FDS_SUCCESS == (ret = CC_DB_Read_System_Data(DB_SYS_NOTIFY, &notify_settings)))
    {
    
        TracerInfo("call=%d \r\n",notify_settings.incomming_call_en);
        TracerInfo("sms=%d \r\n",notify_settings.incomming_sms_en);
        TracerInfo("longsit=%d \r\n",notify_settings.longsit_en); 
        TracerInfo("liftarm=%d \r\n",notify_settings.lifearm_en);                     
    }
    else
        TracerInfo("err9: 0x%x\r\n", ret);                    

    // Unit info:
    //CC_BLE_Cmd_GetUnitInfo(&unit_info);            

    if(FDS_SUCCESS == (ret = CC_DB_Read_System_Data(DB_SYS_UNIT, &unit_info)))
    {
        TracerInfo("length=%d \r\n",unit_info.cUnitLength);
        TracerInfo("weight=%d \r\n",unit_info.cUnitWeight);
    }
    else
        TracerInfo("err10: 0x%x\r\n", ret);
#endif

}


uint8_t _DB_TEST_Full_DB(void)
{
#if 0    
    db_heartrate_t m_db_hrm;
    app_date_time_t curr_time;
    ret_code_t ret;        
    uint16_t total;
      static uint8_t local_hr;
    //uint8_t dayofweek;
    //static uint32_t _bTemp = 0;
    CC_MainGet_CurrentTime(&curr_time);
    m_db_hrm.time.year = (uint8_t)curr_time.year-2000;
    m_db_hrm.time.month = curr_time.month;
    m_db_hrm.time.day = curr_time.day;
    m_db_hrm.time.hour= curr_time.hours;
    m_db_hrm.time.min = curr_time.minutes;
    m_db_hrm.time.sec = curr_time.seconds;
    m_db_hrm.data.hr = local_hr++;
    static uint8_t lock_auto_write_db = 0;

   // NRF_LOG_FLUSH();


//    while(1)
    {
        if(!CC_DB_Check_DB_Full() && (lock_auto_write_db == 0))    
        {
            if(FDS_SUCCESS != (ret = CC_Save_Record(eHrm, (uint32_t*)&m_db_hrm, sizeof(db_heartrate_t))))           
            {
                   //uint16_t rlen;
                 //db_heartrate_t rdata[10];

                 TracerInfo("savd hrm fail_ret:%d\r\n",ret);
         
                 ret = CC_Read_DayofRecoordLen(eHrm, curr_time.dayofweek, &total);
                 TracerInfo("ret = %d, total = %d\r\n",ret, total);
                
                /*
                 for(int i = 0; i < total; i++)                     
                 {
                      TracerInfo("index: %d\r\n", i);     
                     
                      CC_Read_Record(eHrm, curr_time.dayofweek, i+2, (uint32_t*)&rdata[0], &rlen);
                     
                      TracerInfo("year=%d \r\n",rdata[0].time.year);
                      TracerInfo("month=%d \r\n",rdata[0].time.month);
                      TracerInfo("day=%d \r\n",rdata[0].time.day);
                      TracerInfo("hour=%d \r\n",rdata[0].time.hour);
                      TracerInfo("min=%d \r\n",rdata[0].time.min);
                      TracerInfo("sec=%d \r\n",rdata[0].time.sec);
                      TracerInfo("hr=%d \r\n",rdata[0].data.hr);
                 }
                */
                /*
                 ret = CC_DB_Delete_OneDay_File(eHrm, curr_time.dayofweek);
                 TracerInfo("delete : ret = %d\r\n",ret);

                 ret = CC_Read_DayofRecoordLen(eHrm, curr_time.dayofweek, &total);
                 TracerInfo("ret5 = %d, total = %d\r\n",ret, total);
                */
                 lock_auto_write_db = 1;
            }
              //      break;

            return 0;
        }       
        else
        {
            //ret = CC_Read_DayofRecoordLen(eHrm, curr_time.dayofweek, &total);

            //TracerInfo("ret2 = %d, total = %d\r\n",ret, total);     

        }
    }

#endif  
    return 1;

}



