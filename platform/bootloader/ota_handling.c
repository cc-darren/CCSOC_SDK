/* Copyright (c) 2016 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

//#include "global.h"
#include "project.h"
#include "drvi_eflash.h"
#include "crc32.h"
#include "ota_handling.h"
#include "tracer.h"


#define ENTER_OTA_MODE_CODE         (0x404F5441)  // "@OTA"
#define OTA_IMAGE_VALID_CODE        (0x5613C648)     
#define FlASH_PAGE_SIZE             (2048)        // in Byte
//#define ADDRESS_OF_USER_DATA        (0x10039800)
#define ADDRESS_OF_USER_DATA        (0x1003E800)
#define ADDRESS_OF_BL_SETTINGS      (0x1003E800)
#define END_OF_OTA_ADDRESS          (ADDRESS_OF_USER_DATA)



static S_App_OTA_LocalConfig sAppOTALocalConfig;


volatile bool bWaitForFlashDoneFlag = false;


static void  ota_flash_write_done_callback(uint32_t sys_evt)
{

    bWaitForFlashDoneFlag = false;
}


static void  ota_flash_wait_flag_set(void)
{
    bWaitForFlashDoneFlag = true;
}

static void  ota_flash_write_wait_done(void)
{

    while(true == bWaitForFlashDoneFlag)
        ;
}



void  ota_handling_init(void)
{


    drvi_EflashRegisterCallback(ota_flash_write_done_callback);

    drvi_EflashInit();

    ota_update_bl_settings();    
}


void  ota_update_bl_settings(void)
{

    drvi_EflashFlush(); 

    memcpy(&sAppOTALocalConfig, (uint8_t*)ADDRESS_OF_BL_SETTINGS, sizeof(S_App_OTA_LocalConfig));

    if((sAppOTALocalConfig.chunk_size < 256) || (98304 < sAppOTALocalConfig.chunk_size))
        sAppOTALocalConfig.chunk_size = OTA_CHUNK_SIZE; // if over spec, set the default size

}


void  ota_flash_write_bl_settings(void)
{


    //TracerInfo("ota_flash_write_bl_settings...\r\n");

    drvi_EflashRegisterCallback(ota_flash_write_done_callback);
    

    ota_flash_wait_flag_set();

    drvi_EflashErasePage(ADDRESS_OF_BL_SETTINGS);

    ota_flash_write_wait_done();


    ota_flash_wait_flag_set();

    drvi_EflashProgram(ADDRESS_OF_BL_SETTINGS, (uint8_t*)&sAppOTALocalConfig, sizeof(S_App_OTA_LocalConfig));

    ota_flash_write_wait_done();


    drvi_EflashFlush();

    
}



void  ota_set_mode_enabled(void)
{   
    
     sAppOTALocalConfig.enter_ota_mode = ENTER_OTA_MODE_CODE;

     ota_flash_write_bl_settings();

}



#ifdef BOOTLOADER


bool  ota_verify_and_get_app_address(uint32_t *start_address)
{

    drvi_EflashInit();
    
    ota_update_bl_settings();

    *start_address = sAppOTALocalConfig.prog_addr;


    if(sAppOTALocalConfig.enter_ota_mode == ENTER_OTA_MODE_CODE)
        return false;

    if(sAppOTALocalConfig.image_is_valid != OTA_IMAGE_VALID_CODE)
        return false;

    if(sAppOTALocalConfig.file_crc != ota_get_file_crc())
        return false;

    if((sAppOTALocalConfig.file_size + sAppOTALocalConfig.prog_addr) > END_OF_OTA_ADDRESS)
        return false;
    

    return true;
}


void  ota_set_mode_disabled(void)
{   
    
     sAppOTALocalConfig.enter_ota_mode = 0;

     ota_flash_write_bl_settings();

}

bool  ota_mode_is_enabled(void)
{
    
    return (ENTER_OTA_MODE_CODE == sAppOTALocalConfig.enter_ota_mode);

}

bool  ota_get_image_valid_state(void)
{
   
    return sAppOTALocalConfig.image_is_valid;
}


void  ota_clr_image_valid_state(void)
{

    sAppOTALocalConfig.image_is_valid   =   0;


    ota_flash_write_bl_settings();
}


void  ota_set_image_valid_state(void)
{

    sAppOTALocalConfig.image_is_valid   =   OTA_IMAGE_VALID_CODE;


    ota_flash_write_bl_settings();    
}


bool  ota_check_config_is_ok(S_App_OTA_PeerConfig *peer_config)
{


    if((peer_config->prog_addr % FlASH_PAGE_SIZE) != 0)
    {
        TracerInfo("Programmed start address: 0x%04x is not alignement to page size: 0x%04x\r\n", peer_config->prog_addr, FlASH_PAGE_SIZE);

        return false;
    }
    
    if((peer_config->file_size + peer_config->prog_addr) > END_OF_OTA_ADDRESS)
    {
        TracerInfo("Programmed image 0x%04x over 0x%04x\r\n", (peer_config->file_size + peer_config->prog_addr), END_OF_OTA_ADDRESS);
        
        return false; 
    }


    return true;
}


void  ota_save_all_peer_config(S_App_OTA_PeerConfig       *peer_config)
{

    sAppOTALocalConfig.image_is_valid = false;
    sAppOTALocalConfig.prog_addr      = peer_config->prog_addr;
    sAppOTALocalConfig.file_size      = peer_config->file_size;
    sAppOTALocalConfig.file_crc       = peer_config->file_crc;
    sAppOTALocalConfig.signature      = peer_config->signature;
    sAppOTALocalConfig.chunk_offset   = peer_config->chunk_offset;


    ota_flash_write_bl_settings();

}


void  ota_save_peer_chunk_offset(uint32_t chunk_offset)
{

    sAppOTALocalConfig.chunk_offset   = chunk_offset;

    ota_flash_write_bl_settings();

}


void  ota_clr_local_config(void)
{

    sAppOTALocalConfig.prog_addr      = 0;
    sAppOTALocalConfig.file_size      = 0;
    sAppOTALocalConfig.file_crc       = 0;
    sAppOTALocalConfig.signature      = 0;
    sAppOTALocalConfig.chunk_offset   = 0;


    ota_flash_write_bl_settings();

}


void  ota_get_local_config(S_App_OTA_LocalConfig **local_config)
{

    *local_config = &sAppOTALocalConfig;
}


void  ota_save_chunk_offset(uint32_t offset)
{

    sAppOTALocalConfig.chunk_offset   = offset;


    ota_flash_write_bl_settings();
}


uint32_t ota_get_prog_image_offset(void)
{

    sAppOTALocalConfig.chunk_size = OTA_CHUNK_SIZE;

    return sAppOTALocalConfig.chunk_offset * sAppOTALocalConfig.chunk_size;
}



bool  ota_flash_erase_fw_space(void)
{

    uint32_t start_addr = sAppOTALocalConfig.prog_addr;
    uint32_t num_pages;;


    num_pages = (END_OF_OTA_ADDRESS - sAppOTALocalConfig.prog_addr) / FlASH_PAGE_SIZE;

    TracerInfo("Erase page, start addr: 0x%04x, page size: %d\r\n", start_addr, num_pages);


    for(uint32_t page_offset = 0; page_offset < num_pages; page_offset++)
    {                 

         ota_flash_wait_flag_set();

         drvi_EflashErasePage(start_addr + (page_offset * FlASH_PAGE_SIZE));

         ota_flash_write_wait_done();

    }
   
    drvi_EflashFlush();


    return true;
}


void  ota_flash_write_chunk_data(uint8_t *pdata, uint32_t data_size)
{

    uint32_t prog_flash_addr;

    sAppOTALocalConfig.chunk_size = OTA_CHUNK_SIZE;

    prog_flash_addr = sAppOTALocalConfig.chunk_offset * sAppOTALocalConfig.chunk_size + sAppOTALocalConfig.prog_addr;

    TracerInfo("Call flash driver, flash addr: 0x%04x, data_size: %d\r\n", prog_flash_addr, data_size);    

    
    ota_flash_wait_flag_set();

    drvi_EflashProgram(prog_flash_addr, pdata, data_size);

    ota_flash_write_wait_done();

    drvi_EflashFlush();


}



uint32_t ota_get_file_crc(void)
{

    uint32_t crc;

    crc = crc32_compute((const uint8_t*)sAppOTALocalConfig.prog_addr, sAppOTALocalConfig.file_size, NULL);

    return crc;
}

#endif

