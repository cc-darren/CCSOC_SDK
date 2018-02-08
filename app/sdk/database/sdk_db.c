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
*  sdk_db.c
*
*  Project:
*  --------
*  cc6801
*
*  Description:
*  ------------
*  Emulator of Database
*
*  Author:
*  -------
*  CloudChip
*
*===========================================================================/
*  20180125 SAMUEL initial version
******************************************************************************/

/******************************************************************************
 * INCLUDE FILE
 ******************************************************************************/

#ifdef DB_EN
#include "project.h"
#include "cc_db.h"
#include "tracer.h"
#include "sdk_db.h"
#include "string.h"
#include "clock.h"
#include "sw_timer.h"




 /* GLOBAL FUNCTION DEFINITIONS
 ****************************************************************************************
 */

#define FILE1_SIZE_WORDS    10
#define FILE2_SIZE_WORDS    10
#define FILE3_SIZE_WORDS    10
#define FILE4_SIZE_WORDS    10
#define FILE5_SIZE_WORDS    10

#define READ_SIZE_WORDS    10



void sdk_db_test_code(void)
{
    uint32_t err_code;
    uint32_t file1_raw[FILE1_SIZE_WORDS];
    uint32_t file2_raw[FILE2_SIZE_WORDS];
    uint32_t file3_raw[FILE3_SIZE_WORDS];
    uint32_t file4_raw[FILE4_SIZE_WORDS];
    uint32_t file5_raw[FILE5_SIZE_WORDS];
    uint32_t _wReadData[READ_SIZE_WORDS];    
    uint16_t file_totol;
    uint16_t data_words;
    
    TracerInfo("\nSDK EXAMPLE >> DATABASE starts ==========\r\n");

    // init data for 5 files:

    // file 1: 0 ~ 9
    for(uint8_t i = 0; i < FILE1_SIZE_WORDS; i++)
        file1_raw[i] = i;

    // file 2: 9 ~ 0
    for(uint8_t i = 0; i < FILE2_SIZE_WORDS; i++)
        file2_raw[i] = FILE2_SIZE_WORDS - i - 1;

    // file 3: repeat 0x55555555
    for(uint8_t i = 0; i < FILE3_SIZE_WORDS; i++)
        file3_raw[i] = 0x55555555;

    // file 4: repeat 0xAAAAAAAA
    for(uint8_t i = 0; i < FILE4_SIZE_WORDS; i++)
        file4_raw[i] = 0xAAAAAAAA;

    // file 5: repeat 0x55AA55AA
    for(uint8_t i = 0; i < FILE5_SIZE_WORDS; i++)
        file5_raw[i] = 0x55AA55AA;



    // Init. DB
    if(CC_SUCCESS != (err_code = CC_DB_Init_Folder()))
        TracerInfo("CC_DB_Init_Folder fail: 0x%x\r\n", err_code);


    // Remove Old data in Flash memory:
    CC_DB_Reset_One_Folder(eFOLDER1);
    CC_DB_Reset_One_Folder(eFOLDER2);
    CC_DB_Reset_One_Folder(eFOLDER3);
    CC_DB_Reset_One_Folder(eFOLDER4);
    CC_DB_Reset_One_Folder(eFOLDER5);


    // Save files:
    TracerInfo("Save 1 file to each folder...\r\n");
    CC_DB_Save_File(eFOLDER1, (uint32_t*)file1_raw, FILE1_SIZE_WORDS);
    CC_DB_Save_File(eFOLDER2, (uint32_t*)file2_raw, FILE2_SIZE_WORDS);
    CC_DB_Save_File(eFOLDER3, (uint32_t*)file3_raw, FILE3_SIZE_WORDS);
    CC_DB_Save_File(eFOLDER4, (uint32_t*)file4_raw, FILE4_SIZE_WORDS);
    CC_DB_Save_File(eFOLDER5, (uint32_t*)file5_raw, FILE5_SIZE_WORDS);


    // Dump all files:
    for(uint16_t fd_num = 0; fd_num < 5; fd_num++)
    {
        CC_DB_Read_Folder_NumOfFile((eDB_FOLDER_OP_t)(fd_num+eFOLDER1), &file_totol);
        
        TracerInfo("Folder%d include %d files\r\n", fd_num+1, file_totol);


        for(uint16_t file_num = 0; file_num < file_totol; file_num++)
        {
            
            CC_DB_Read_File((eDB_FOLDER_OP_t)(fd_num+eFOLDER1), (file_num+FILE_ID_REC_OFFSET), (uint32_t*) _wReadData, &data_words);

            TracerInfo("Dump Data File: %d with Size: %d words\r\n", (file_num+FILE_ID_REC_OFFSET), data_words);

            for(uint8_t i = 0; i < data_words; i++)
            {
                TracerInfo("0x%x, ", _wReadData[i]);
            }

            TracerInfo("\r\n");
        }
    }



    // Reset folder 1:
    TracerInfo("Reset Folder1...\r\n");
    CC_DB_Reset_One_Folder(eFOLDER1);


    // Save 5 files to Folder 1
    TracerInfo("Save 5 files to Folder1...\r\n");
    CC_DB_Save_File(eFOLDER1, (uint32_t*)file1_raw, FILE1_SIZE_WORDS);
    CC_DB_Save_File(eFOLDER1, (uint32_t*)file2_raw, FILE2_SIZE_WORDS);
    CC_DB_Save_File(eFOLDER1, (uint32_t*)file3_raw, FILE3_SIZE_WORDS);
    CC_DB_Save_File(eFOLDER1, (uint32_t*)file4_raw, FILE4_SIZE_WORDS);
    CC_DB_Save_File(eFOLDER1, (uint32_t*)file5_raw, FILE5_SIZE_WORDS);


    CC_DB_Read_Folder_NumOfFile(eFOLDER1, &file_totol);
    
    TracerInfo("Folder1 include %d files:\r\n", file_totol);


    for(uint16_t file_num = 0; file_num < file_totol; file_num++)
    {
        CC_DB_Read_File(eFOLDER1, (file_num+FILE_ID_REC_OFFSET), (uint32_t*)_wReadData, &data_words);
    
        for(uint8_t i = 0; i < data_words; i++)
            TracerInfo("0x%x, ", _wReadData[i]);

        TracerInfo("\r\n");
    }

    TracerInfo("\nSDK EXAMPLE >> DATABASE done ==========\r\n");
}


#endif

