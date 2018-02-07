#ifndef _CC_DB_H_
#define _CC_DB_H_
#include "project.h"

#include <stdint.h>


#define BLE_DB_PED_COMMAND_ID       0xFA
#define BLE_DB_HRM_COMMAND_ID       0xFB
#define BLE_DB_SLEEP_COMMAND_ID     0xFC
#define BLE_DB_SWIM_COMMAND_ID      0xFD
#ifdef BatteryLog_EN
#define BLE_DB_BATLEV_COMMAND_ID   0xE0
#endif



#define FIRST_BOOT_CODE               0x5566AACC
#define FRIST_BOOT_RECID              0x0001

#define REC_ID_RecordNum                0x0001
#define REC_ID_RecordOffset            0x0001

#define FILE_ID_SYSTEM                  0x1000

#define FILE_ID_PED_SUN                 0x2000
#define FILE_ID_PED_MON                 0x2001
#define FILE_ID_PED_TUE                 0x2002
#define FILE_ID_PED_WED                 0x2003
#define FILE_ID_PED_THU                 0x2004
#define FILE_ID_PED_FRI                 0x2005
#define FILE_ID_PED_SAT                 0x2006

#define FILE_ID_HRM_SUN                 0x3000
#define FILE_ID_HRM_MON                 0x3001
#define FILE_ID_HRM_TUE                 0x3002
#define FILE_ID_HRM_WED                 0x3003
#define FILE_ID_HRM_THU                 0x3004
#define FILE_ID_HRM_FRI                 0x3005
#define FILE_ID_HRM_SAT                 0x3006

#define FILE_ID_SLEEP_SUN               0x4000
#define FILE_ID_SLEEP_MON               0x4001
#define FILE_ID_SLEEP_TUE               0x4002
#define FILE_ID_SLEEP_WED               0x4003
#define FILE_ID_SLEEP_THU               0x4004
#define FILE_ID_SLEEP_FRI               0x4005
#define FILE_ID_SLEEP_SAT               0x4006

#define FILE_ID_SWIM_SUN                0x5000
#define FILE_ID_SWIM_MON                0x5001
#define FILE_ID_SWIM_TUE                0x5002
#define FILE_ID_SWIM_WED                0x5003
#define FILE_ID_SWIM_THU                0x5004
#define FILE_ID_SWIM_FRI                0x5005
#define FILE_ID_SWIM_SAT                0x5006

#ifdef BatteryLog_EN
#define FILE_MixLog_ID_SUN                          0x6000
#define FILE_MixLog_ID_MON                          0x6001
#define FILE_MixLog_ID_TUE                          0x6002
#define FILE_MixLog_ID_WED                          0x6003
#define FILE_MixLog_ID_THU                         0x6004
#define FILE_MixLog_ID_FRI                          0x6005
#define FILE_MixLog_ID_SAT                          0x6006
#endif
typedef struct
{
    uint16_t wSystemFileID;
    uint16_t wPedFileID;
    uint16_t wHrmFileID;
    uint16_t wSleepFileID;
    uint16_t wSwimFileID;
    #ifdef BatteryLog_EN
    uint16_t wMixLogFileID;
    #endif
}S_DB_FILE_ID __attribute__((aligned(4)));


typedef struct
{
    uint16_t wSystemRecID;
    uint16_t wPedRecID;
    uint16_t wHrmRecID;
    uint16_t wSleepRecID;
    uint16_t wSwimRecID;
    #ifdef BatteryLog_EN
    uint16_t wMixLogRecID;
    #endif
}S_DB_REC_ID __attribute__((aligned(4)));

typedef struct
{
    uint32_t wPedRecNum;
    uint16_t wPedRecLen;
    uint32_t wHrmRecNum;
    uint16_t wHrmRecLen;
    uint32_t wSleepRecNum;
    uint16_t wSleepRecLen;
    uint32_t wSwimRecNum;
    uint16_t wSwimRecLen;
    #ifdef BatteryLog_EN
    uint32_t wMixLogRecNum;
    uint16_t wMixLogRecLen;
    #endif
}S_DB_REC_Num __attribute__((aligned(4)));

typedef enum 
{
    eSystem=0x1000,
    ePed=0x2000,
    eHrm=0x3000,
    eSleep=0x4000,
    eSwim=0x5000,
    eMixLog=0x6000
}eDB_Content_Op_t;


typedef enum 
{
    DB_SYS_TIME = 0x01,
    DB_SYS_GENERAL_INFO,
    DB_SYS_ALARM,
    DB_SYS_HR_SETTING,
    DB_SYS_NOTIFY,
    DB_SYS_UNIT,
    DB_SYS_LONGSIT_TIME_SETTING,
    DB_SYS_DYNA_GYRO_OFFSET,
    DB_SYS_STATIC_GYRO_OFFSET,
    DB_SYS_GOAL_ACHIEVED,
    DB_SYS_RSVD,
}eDB_System_Record_ID_t;


uint32_t CC_Save_Record(eDB_Content_Op_t _eOpCode,uint32_t *pWData,uint16_t _bWLen);

uint32_t CC_Read_DayofRecoordLen(eDB_Content_Op_t _eOpCode,uint8_t _bDayofWeek, uint16_t *_wDataLen);
uint32_t CC_Read_Record(eDB_Content_Op_t _eOpCode,uint8_t _bDayofWeek,uint16_t _wRecId ,uint32_t *_pRData, uint16_t *_wDataLen);
uint32_t CC_Update_Record(void);
uint32_t CC_Delete_Record(void);
uint32_t CC_FindFile_And_DeleteFile(eDB_Content_Op_t _eOpCode,uint8_t _bDayofWeek);
uint32_t CC_DB_Init_FileRec(uint8_t *_bFirstInit, uint8_t _bState);
uint32_t CC_DB_Delete_OneDay_File(eDB_Content_Op_t id, uint8_t _bDayofWeek);
uint8_t CC_DB_Check_DB_Full(void);

uint32_t CC_Fds_init (void);
uint32_t CC_Fds_Write(const uint16_t _wFileID, const uint16_t _wRecKey,uint32_t *pWData,uint16_t _bWLen);
uint32_t CC_Fds_Update(const uint16_t _wFileID, const uint16_t _wRecKey,uint32_t *pWData,uint16_t _bWLen);
uint32_t CC_Fds_Read(const uint16_t _wFileID, const uint16_t _wRecKey,uint32_t *_pRData,uint16_t *_pRLen);
uint32_t CC_Fds_Find_And_Delete (const uint16_t _FileID, const uint16_t _RecKey);
void CC_DB_System_Save_Request(eDB_System_Record_ID_t sys_id);
void CC_DB_System_Check_Request_And_Save(void);
uint32_t CC_DB_Read_System_Data(eDB_System_Record_ID_t sys_id, void *data);

void CC_Fds_Change_DayofWeek(uint8_t _bDayofWeek, uint8_t _IsReCreateOneDay);
uint8_t CC_DB_Check_Magic_Code(void);
void CC_DB_Factory_System_DataReset(void);

void _DB_TEST(void);
void _DB_TEST_Write_System_DB(void);
void _DB_TEST_Read_System_DB(void);
void _DB_TEST_WRITE_BATTERYLOG(void);
void _DB_TEST_READ_BATTERYLOG(void);
void _DB_TEST_READ_SLEEP(void);
void _DB_TEST_WRITE_SLEEP(void);

void _DB_TEST_WRITE_Swim(void);
void _DB_TEST_READ_Swim(void);
void _DB_TEST_Write_System_DB(void);
void _DB_TEST_Read_System_DB(void);
uint8_t _DB_TEST_Full_DB(void);
void CC_Fds_Change_DayofWeek(uint8_t _bDayofWeek, uint8_t _IsReCreateOneDay);
void _DB_Update_ID_Info(uint8_t _bDayofWeek);




#define FOLDER_MixLog_ID_NUM1						  	0x9000
#define FOLDER_MixLog_ID_NUM2                         	0x9001
#define FOLDER_MixLog_ID_NUM3                         	0x9002
#define FOLDER_MixLog_ID_NUM4                          	0x9003
#define FOLDER_MixLog_ID_NUM5                         	0x9004
#define FILE_ID_INIT									(0x0001) 
#define FILE_ID_REC_OFFSET            					(0x0001)


typedef struct
{

    uint32_t wMiscfile_1_Num;
    uint16_t wMiscfile_1_Len;
	
    uint32_t wMiscfile_2_Num;
    uint16_t wMiscfile_2_Len;
	
	uint32_t wMiscfile_3_Num;
    uint16_t wMiscfile_3_Len;
	
    uint32_t wMiscfile_4_Num;
    uint16_t wMiscfile_4_Len;
	
	uint32_t wMiscfile_5_Num;
    uint16_t wMiscfile_5_Len;
	
}S_DB_FILE_Num __attribute__((aligned(4)));

typedef enum
{
    eFOLDER1=0x9000,
    eFOLDER2=0x9001,
    eFOLDER3=0x9002,
	eFOLDER4=0x9003,
	eFOLDER5=0x9004
}eDB_FOLDER_OP_t;


uint32_t CC_DB_Init_Folder(void);
uint32_t CC_DB_Save_File(eDB_FOLDER_OP_t _eOpCode,uint32_t *pWData,uint16_t _wLen); //_wLen: in words
uint32_t CC_DB_Read_File(eDB_FOLDER_OP_t _eOpCode,uint16_t _wFileId ,uint32_t *_pRData, uint16_t *_wDataLen);// _wDataLen: in words
uint32_t CC_DB_Read_Folder_NumOfFile(eDB_FOLDER_OP_t _eOpCode, uint16_t *_wDataLen);
uint32_t CC_DB_Reset_One_Folder(eDB_FOLDER_OP_t _eOpCode);

#endif
