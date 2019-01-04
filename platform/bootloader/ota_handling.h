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

#ifndef _OTA_HANDLING_H_
#define _OTA_HANDLING_H_

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "app_ota.h"


#define OTA_CHUNK_SIZE          2048     



typedef struct
{

    uint32_t    enter_ota_mode;
    uint32_t    image_is_valid;    
    uint32_t    prog_addr;
    uint32_t    signature;
    uint32_t    file_size;
    uint32_t    file_crc;
    uint32_t    chunk_offset;
    uint32_t    chunk_size; 
    #ifdef FS_CC_MODE_EN
    uint32_t    fs_magic_no;    
    #endif
} S_App_OTA_LocalConfig;




void  ota_handling_init(void);
void  ota_update_bl_settings(void);
void  ota_set_mode_enabled(void);
#if defined(CC6801B0) && defined(FPGA)
void  ota_erase_user_data_space(void);
#endif

#ifdef BOOTLOADER
bool  ota_verify_and_get_app_address(uint32_t *start_address);
void  ota_set_mode_disabled(void);
bool  ota_mode_is_enabled(void);
void  ota_set_image_valid_state(void);
void  ota_clr_image_valid_state(void);
bool  ota_check_config_is_ok(S_App_OTA_PeerConfig  *peer_config);
void  ota_save_all_peer_config(S_App_OTA_PeerConfig  *peer_config);
void  ota_save_peer_chunk_offset(uint32_t chunk_offset);
void  ota_clr_local_config(void);
void  ota_get_local_config(S_App_OTA_LocalConfig **local_config);
bool  ota_flash_erase_fw_space(void);
void  ota_save_chunk_offset(uint32_t offset);
void  ota_flash_write_chunk_data(uint8_t *pdata, uint32_t data_size);
void  ota_time_tick_create(void);
uint32_t  ota_get_prog_image_offset(uint32_t chunk_offset);
uint32_t  ota_get_file_crc(void);
#endif



#endif // _OTA_HANDLING_H_

/** @} */

