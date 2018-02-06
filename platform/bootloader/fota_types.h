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

/**@file
 *
 * @defgroup sdk_nrf_dfu_types DFU types
 * @{
 * @ingroup  sdk_nrf_dfu
 */

#ifndef NRF_DFU_TYPES_H__
#define NRF_DFU_TYPES_H__

#include <stdint.h>
#include <stddef.h>

#define PAGE_SIZE_IN_WORDS  (1024)
#define NRF_DFU_SETTINGS_VERSION 1

/** @brief The size that must be reserved for the MBR when a softdevice is written to flash.
This is the offset where the first byte of the softdevice hex file is written.*/
#define MBR_SIZE                (0x1000)


#define INIT_COMMAND_MAX_SIZE       256     /**< Maximum size of the init command stored in dfu_settings. */


/** @brief  Size of a flash codepage. This value is used for calculating the size of the reserved
 *          flash space in the bootloader region. It is checked against NRF_UICR->CODEPAGESIZE
 *          at run time to ensure that the region is correct.
 */
#define CODE_PAGE_SIZE            (PAGE_SIZE_IN_WORDS*sizeof(uint32_t))


/** @brief  Maximum size of a data object.*/
#define DATA_OBJECT_MAX_SIZE           (CODE_PAGE_SIZE * 4)


/** @brief  Page location of the bootloader settings address.
 */
#define BOOTLOADER_SETTINGS_ADDRESS     (0x1003F000UL)


/** @brief  Size of the flash space reserved for application data.
 */
//#ifndef DFU_APP_DATA_RESERVED
//#define DFU_APP_DATA_RESERVED               CODE_PAGE_SIZE * 3
//#endif


/** @brief Total size of the region between the SoftDevice and the bootloader.
 */
//#ifndef DFU_APP_DATA_RESERVED
//#define DFU_REGION_TOTAL_SIZE               ((0x10040000) - CODE_REGION_1_START)
//#endif


/** @brief Size of the Code Region 0, found in the UICR.CLEN0 register.
 *
 * @details This value is identical to the start of Code Region 1. This value is used for
 *          compilation safety, because the linker will fail if the application expands
 *          into the bootloader. At run time, the bootloader uses the value found in UICR.CLEN0.
 */

//#ifndef CODE_REGION_1_START
//#define CODE_REGION_1_START                 0x10010000
//#endif

#define NRF_DFU_CURRENT_BANK_0 0x00
#define NRF_DFU_CURRENT_BANK_1 0x01

#define NRF_DFU_BANK_LAYOUT_DUAL   0x00
#define NRF_DFU_BANK_LAYOUT_SINGLE 0x01


/** @brief DFU bank state codes.
 *
 * @details The DFU bank state indicates the content of a bank:
 *          A valid image of a certain type or an invalid image.
 */

#define NRF_DFU_BANK_INVALID     0x00 /**< Invalid image. */
#define NRF_DFU_BANK_VALID_APP   0x01 /**< Valid application. */
#define NRF_DFU_BANK_VALID_SD    0xA5 /**< Valid SoftDevice. */
#define NRF_DFU_BANK_VALID_BL    0xAA /**< Valid bootloader. */
#define NRF_DFU_BANK_VALID_SD_BL 0xAC /**< Valid SoftDevice and bootloader. */


/** @brief Description of a single bank. */
#pragma pack(4)
typedef struct
{
    uint32_t                image_size;         /**< Size of the image in the bank. */
    uint32_t                image_crc;          /**< CRC of the image. If set to 0, the CRC is ignored. */
    uint32_t                bank_code;          /**< Identifier code for the bank. */
} nrf_dfu_bank_t;

/**@brief DFU progress.
 *
 * Be aware of the difference between objects and firmware images. A firmware image consists of multiple objects, each of a maximum size @ref DATA_OBJECT_MAX_SIZE.
 */
typedef struct
{
    uint32_t command_size;              /**< The size of the current init command stored in the DFU settings. */
    uint32_t command_offset;            /**< The offset of the currently received init command data. The offset will increase as the init command is received. */
    uint32_t command_crc;               /**< The calculated CRC of the init command (calculated after the transfer is completed). */

    uint32_t data_object_size;          /**< The size of the last object created. Note that this size is not the size of the whole firmware image.*/

    uint32_t firmware_image_crc;        /**< CRC value of the current firmware (continuously calculated as data is received). */
    uint32_t firmware_image_crc_last;   /**< The CRC of the last executed object. */
    uint32_t firmware_image_offset;     /**< The offset of the current firmware image being transferred. Note that this offset is the offset in the entire firmware image and not only the current object. */
    uint32_t firmware_image_offset_last;/**< The offset of the last executed object from the start of the firmware image. */
} dfu_progress_t;


/**@brief DFU settings for application and bank data.
 */
typedef struct
{
    uint32_t            crc;                /**< CRC for the stored DFU settings, not including the CRC itself. If 0xFFFFFFF, the CRC has never been calculated. */
    uint32_t            settings_version;   /**< Version of the currect dfu settings struct layout.*/
    uint32_t            app_version;        /**< Version of the last stored application. */
    uint32_t            bootloader_version; /**< Version of the last stored bootloader. */

    uint32_t            bank_layout;        /**< Bank layout: single bank or dual bank. This value can change. */
    uint32_t            bank_current;       /**< The bank that is currently used. */

    nrf_dfu_bank_t      bank_0;             /**< Bank 0. */
    nrf_dfu_bank_t      bank_1;             /**< Bank 1. */

    uint32_t            write_offset;       /**< Write offset for the current operation. */
    uint32_t            sd_size;            /**< SoftDevice size (if combined BL and SD). */

    dfu_progress_t      progress;           /**< Current DFU progress. */

    uint32_t            enter_buttonless_dfu;
    uint8_t             init_command[INIT_COMMAND_MAX_SIZE];  /**< Buffer for storing the init command. */
} nrf_dfu_settings_t;
#pragma pack() // revert pack settings


#endif // NRF_DFU_TYPES_H__

/** @} */
