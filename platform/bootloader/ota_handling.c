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


//#include "global.h"
#include "project.h"
#include "drvi_eflash.h"
#include "ota_handling.h"
#include "rwip.h"
#include "tracer.h"
#include "drvi_clock.h"


#define ENTER_OTA_MODE_CODE         (0x404F5441)  // "@OTA"
#define OTA_IMAGE_VALID_CODE        (0x5613C648)
#define FlASH_PAGE_SIZE             (2048)        // in Byte
#define ADDRESS_OF_BL_SETTINGS      (0x1003E800)
#define END_OF_OTA_ADDRESS          (ADDRESS_OF_BL_SETTINGS)
#define DEFAULT_ADDRESS_OF_APP      (0x1000A000)


static S_App_OTA_LocalConfig sAppOTALocalConfig;


volatile bool bWaitForFlashDoneFlag = false;


void  ota_handling_init(void)
{

    drvi_EflashInit();

    ota_update_bl_settings();
}


void  ota_update_bl_settings(void)
{

    //drvi_EflashFlush();

    memcpy(&sAppOTALocalConfig, (uint8_t*)ADDRESS_OF_BL_SETTINGS, sizeof(S_App_OTA_LocalConfig));

    if((sAppOTALocalConfig.chunk_size < 256) || (98304 < sAppOTALocalConfig.chunk_size))
        sAppOTALocalConfig.chunk_size = OTA_CHUNK_SIZE; // if over spec, set the default size

}


void  ota_flash_write_bl_settings(void)
{

    drvi_EflashErasePage(ADDRESS_OF_BL_SETTINGS);
    drvi_EflashProgram(ADDRESS_OF_BL_SETTINGS, (uint8_t*)&sAppOTALocalConfig, sizeof(S_App_OTA_LocalConfig));
    drvi_EflashFlush();
}



void  ota_set_mode_enabled(void)
{

     sAppOTALocalConfig.enter_ota_mode = ENTER_OTA_MODE_CODE;

     ota_flash_write_bl_settings();

}

