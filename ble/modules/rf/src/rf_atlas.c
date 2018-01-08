/**
****************************************************************************************
*
* @file rf_atlas.c
*
* @brief Atlas radio initialization and specific functions
*
* Copyright (C) RivieraWaves 2009-2015
*
* $Rev: $
*
****************************************************************************************
*/

/**
****************************************************************************************
* @addtogroup RF_ATLAS
* @ingroup RF
* @brief Atlas Radio Driver
*
* This is the driver block for Atlas radio
* @{
****************************************************************************************
*/

/**
 *****************************************************************************************
 * INCLUDE FILES
 *****************************************************************************************
 */
#include <string.h>        // for memcpy
#include "co_utils.h"      // common utility definition
#include "co_math.h"       // common math functions
#include "co_endian.h"     // endian definitions
#include "rf.h"            // RF interface
#include "em_map.h"        // RF area mapping

#include "rwip.h"          // for RF API structure definition
#include "reg_blecore.h"   // ble core registers
#include "reg_ble_em_cs.h" // control structure definitions

#include "plf.h"           // Platform register

#include "drvi_gpio.h"

// Atlas register definitions and access functions
static uint32_t rf_atl_reg_rd (uint16_t addr);
static void rf_atl_reg_wr (uint16_t addr, uint32_t value);

#define REG_ATL_RD                rf_atl_reg_rd
#define REG_ATL_WR                rf_atl_reg_wr

// CONFIGURATIONs ==============================================================
#define CC6801_MPW
//#define TC4CC6801

//#define DCDC_MODE

/**
 ****************************************************************************************
 * DEFINES
 ****************************************************************************************
 **/

#define RF_EM_SPI_ADRESS        (EM_BASE_ADDR + EM_RF_SW_SPI_OFFSET)

#define ATL_SPIRD                   0x00
#define ATL_SPIWR                   0x80

#define ATL_MAX_BURST_SIZE          0x80
#define ICY1x_INIT_TBL_SIZE         0x89
#define ICY20_INIT_TBL_SIZE         0x98
#define ICY30_INIT_TBL_SIZE         0xA8
#define ICY31_INIT_TBL_SIZE         0xBD
#define ICY40_INIT_TBL_SIZE         0xC0
#define ICY62_INIT_TBL_SIZE         0xC0
#define ICY63_INIT_TBL_SIZE         0xC0

// TX max power
#define ATL_POWER_MAX               0x06


/**
****************************************************************************************
* MACROS
*****************************************************************************************
*/

/// IcyTRx EM Write Macro for HW driven SPI chained structures
#define RF_ICTRX_EM_BLE_WR(addr, val) \
    EM_BLE_WR((((uint32_t) (addr)) + REG_BLE_EM_CS_BASE_ADDR), (val))


// Max burst register
static uint8_t rf_atl_reg_buf[ATL_MAX_BURST_SIZE + 2]; // max burst size + buffer controls

/**
 ****************************************************************************************
 * GLOBAL VARIABLE DEFINITIONS
 *****************************************************************************************
 **/

/* Icytrx v0x13 static settings */
static const uint8_t RF_ATL_REG_TBL_13[ICY1x_INIT_TBL_SIZE] =
{
  0x22, 0x18, 0x01, 0x10,   0x00, 0x82, 0x28, 0x00,   0x55, 0xff, 0x65, 0x12,   0x67, 0x12, 0x00, 0xd6, //0x0*
  0x41, 0x76, 0x71, 0x02,   0x2d, 0x03, 0x80, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00, //0x1*
  0x00, 0xff, 0xfd, 0xfa,   0xf3, 0xe7, 0xd7, 0xc3,   0xae, 0x9a, 0x8b, 0x82,   0xec, 0x51, 0x8d, 0x82, //0x2*
  0x30, 0x00, 0x01, 0x00,   0x21, 0x74, 0xca, 0xfe,   0x20, 0x9a, 0x60, 0xe5,   0x03, 0x04, 0xd0, 0x26, //0x3*
  0x43, 0x60, 0x08, 0xfa,   0x00, 0xf0, 0x0c, 0x00,   0x26, 0x00, 0x66, 0x66,   0x20, 0xf0, 0x30, 0x66, //0x4*
  0x00, 0x43, 0x43, 0xa3,   0xa3, 0xb7, 0xb7, 0xff,   0x05, 0x00, 0x03, 0x00,   0x00, 0x00, 0x10, 0x21, //0x5*
  0x24, 0x03, 0x00, 0x00,   0x00, 0x00, 0x33, 0x0e,   0x37, 0xd9, 0x9d, 0x09,   0x37, 0x33, 0xb1, 0x88, //0x6*
  0x35, 0x7b, 0x03, 0x57,   0x34, 0x00, 0x10, 0x40,   0x39, 0x04, 0x80, 0xF0,   0x40, 0x9b, 0xa1, 0x09, //0x7*
  0x31, 0x04, 0x17, 0x80,   0x09, 0x00, 0x00, 0xf0,   0x00                                              //0x8*
};


/* Icytrx v0x14 static settings */
static const uint8_t RF_ATL_REG_TBL_14[ICY1x_INIT_TBL_SIZE] =
{
  0x22, 0x18, 0x01, 0x10,   0x00, 0x82, 0x28, 0x00,   0x55, 0xff, 0x65, 0x12,   0x67, 0x12, 0x00, 0xd6,  //0x0*
  0x41, 0x76, 0x71, 0x02,   0x2d, 0x03, 0x80, 0x00,   0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00,  //0x1*
  0x00, 0xff, 0xfd, 0xfa,   0xf3, 0xe7, 0xd7, 0xc3,   0xae, 0x9a, 0x8b, 0x82,   0xec, 0x51, 0x8d, 0x82,  //0x2*
  0x30, 0x00, 0x01, 0x00,   0x21, 0x74, 0xca, 0xfe,   0x20, 0x9a, 0x60, 0xe5,   0x03, 0x04, 0xd0, 0x26,  //0x3*
  0x43, 0x60, 0x08, 0xfa,   0x00, 0xf0, 0x0c, 0x00,   0x26, 0x00, 0x66, 0x66,   0x20, 0xf0, 0x30, 0x66,  //0x4*
  0x00, 0x43, 0x43, 0xa3,   0xa3, 0xb7, 0xb7, 0xff,   0x05, 0x00, 0x03, 0x00,   0x00, 0x00, 0x10, 0x21,  //0x5*
  0x24, 0x03, 0x00, 0x00,   0x00, 0x00, 0x31, 0x0e,   0x37, 0xd9, 0x9d, 0x09,   0x37, 0x33, 0xb1, 0x88,  //0x6*
  0x35, 0x7b, 0x03, 0x57,   0x34, 0x00, 0x10, 0x40,   0x39, 0x04, 0x80, 0xF0,   0x00, 0xb7, 0xa1, 0x09,  //0x7*
  0x31, 0x04, 0x17, 0x80,   0x09, 0x00, 0x00, 0xf0,   0x00                                               //0x8*
};


/* Icytrx v0x20 static settings */
static const uint8_t RF_ATL_REG_TBL_20[ICY20_INIT_TBL_SIZE] =
{ 0x22, 0x08, 0x00, 0x81,   0x80, 0x82, 0xff, 0x01,    0xaa, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x1e, //0x0*
  0x3c, 0x5a, 0x78, 0x02,   0x10, 0x88, 0x00, 0x00,    0x00, 0x00, 0x00, 0x00,   0x00, 0x23, 0x12, 0x14, //0x1*
  0x03, 0x20, 0x00, 0x00,   0x03, 0x00, 0xff, 0xf0,    0x0f, 0x00, 0x00, 0x00,   0xf5, 0x49, 0x17, 0x02, //0x2*
  0x00, 0x01, 0x01, 0x00,   0x00, 0x00, 0x00, 0x00,    0x00, 0x01, 0x02, 0x07,   0x10, 0x20, 0x37, 0x50, //0x3*
  0x66, 0x76, 0x7e, 0x2b,   0x11, 0xa0, 0x21, 0x74,    0xca, 0xfe, 0x3f, 0x05,   0x04, 0x57, 0x0b, 0x1b, //0x4*
  0x30, 0x44, 0x0b, 0xaa,   0x2a, 0x11, 0x17, 0x17,    0x08, 0x0e, 0x0c, 0x08,   0xc8, 0x54, 0x80, 0x00, //0x5*
  0x40, 0xa0, 0x42, 0x86,   0x15, 0x3d, 0x7a, 0xf4,    0xdb, 0xb6, 0x0d, 0x00,   0x22, 0x40, 0x42, 0x44, //0x6*
  0x14, 0x00, 0x00, 0x00,   0x73, 0x66, 0xb0, 0x99,    0x79, 0x07, 0x76, 0x47,   0x93, 0x78, 0x56, 0x00, //0x7*
  0x00, 0x02, 0x0f, 0xc3,   0x82, 0x06, 0x00, 0x00,    0x00, 0x07, 0x1f, 0x00,   0x00, 0x00, 0x00, 0x35, //0x8*
  0x80, 0xc3, 0x69, 0x44,   0x00, 0xf0, 0x00, 0x00                                                       //0x9*
};


/* Icytrx v0x30 static settings */
static const uint8_t RF_ATL_REG_TBL_30[ICY30_INIT_TBL_SIZE] =
{ 0x22, 0x08, 0x01, 0x00,   0x00, 0x23, 0x0b, 0x14,    0x05, 0x20, 0x00, 0x10,   0x03, 0x43, 0xff, 0xff, // 0x0*
  0x0f, 0x00, 0x00, 0x42,   0x46, 0xc8, 0x1c, 0xc7,    0x15, 0x82, 0x00, 0x00,   0x01, 0x01, 0x72, 0x1c, // 0x1*
  0x07, 0x00, 0xa2, 0xff,   0x01, 0x55, 0x00, 0x00,    0x00, 0x00, 0x00, 0x00,   0x29, 0x41, 0x76, 0x71, // 0x2*
  0x03, 0x2d, 0x03, 0x80,   0x00, 0x55, 0x55, 0x55,    0x55, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00, // 0x3*
  0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x01,    0x02, 0x07, 0x10, 0x20,   0x37, 0x50, 0x66, 0x76, // 0x4*
  0x7e, 0x2b, 0x01, 0xa0,   0x21, 0x74, 0xca, 0xfe,    0x3f, 0x80, 0x00, 0x50,   0x0f, 0x1b, 0x30, 0x44, // 0x5*
  0x0b, 0xaa, 0x2a, 0x11,   0x1b, 0x17, 0x0b, 0x15,    0x0c, 0x0c, 0xe8, 0x5c,   0x84, 0x00, 0x20, 0x60, // 0x6*
  0xc0, 0x84, 0x31, 0x63,   0xc6, 0x8c, 0xdb, 0xb6,    0x0d, 0x00, 0x24, 0xb0,   0x42, 0xb4, 0x14, 0x00, // 0x7*
  0x00, 0x00, 0x73, 0x66,   0x72, 0x99, 0x75, 0x99,    0x97, 0x39, 0x72, 0x68,   0x25, 0x00, 0x00, 0x02, // 0x8*
  0x0f, 0xc2, 0xc2, 0x06,   0x00, 0x00, 0x00, 0x07,    0x33, 0x00, 0x00, 0x00,   0x00, 0x35, 0x80, 0xc3, // 0x9*
  0x80, 0x04, 0x00, 0xf0,   0x00, 0x03, 0x00
};

/* Icytrx v0x31 static settings */
static const uint8_t RF_ATL_REG_TBL_31[ICY31_INIT_TBL_SIZE] =
{ 0x22, 0x08, 0x00, 0x00,   0x00, 0x23, 0x0b, 0x14,    0x05, 0x20, 0x00, 0x10,   0x03, 0x00, 0xff, 0xff, // 0x0*
  0x0f, 0x00, 0x00, 0x80,   0x23, 0x82, 0x1b, 0xc7,    0x15, 0x82, 0x00, 0x00,   0x01, 0x01, 0x72, 0x1c, // 0x1*
  0x07, 0x00, 0x82, 0xff,   0x01, 0x55, 0x00, 0x00,    0x00, 0x00, 0x00, 0x00,   0x29, 0x41, 0x76, 0x71, // 0x2*
  0x03, 0x2d, 0x03, 0x80,   0x00, 0x55, 0x55, 0x55,    0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00, // 0x3*
  0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x01,    0x02, 0x07, 0x10, 0x20,   0x37, 0x50, 0x66, 0x76, // 0x4*
  0x7e, 0x2b, 0x10, 0xc0,   0x21, 0x74, 0xca, 0xfe,    0x3f, 0x00, 0x02, 0x50,   0x0f, 0x1b, 0x30, 0x44, // 0x5*
  0x0b, 0x55, 0x15, 0x11,   0x1b, 0x09, 0x0b, 0x15,    0x0c, 0xfe, 0x50, 0x5c,   0x84, 0x00, 0x00, 0x04, // 0x6*
  0x24, 0x30, 0x81, 0x11,   0x9c, 0x00, 0x00, 0x00,    0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0xdc, 0xc8, // 0x7*
  0x0d, 0x00, 0x00, 0x00,   0x00, 0x33, 0xc0, 0x22,    0x61, 0x01, 0x00, 0x00,   0x00, 0x73, 0x66, 0x72, // 0x8*
  0x99, 0x75, 0x9a, 0x77,   0x37, 0x72, 0x13, 0x68,    0x25, 0x00, 0x00, 0x08,   0x0f, 0xc2, 0xc2, 0x16, // 0x9*
  0x18, 0x00, 0x00, 0x07,   0x7f, 0x80, 0x00, 0x00,    0x00, 0x3a, 0xd0, 0xc3,   0x78, 0x04, 0x04, 0xf0, // 0xa*
  0x00, 0x03, 0x00, 0x4b,   0x11, 0x00, 0x00, 0x40,    0x00, 0x00, 0x86, 0x25,   0x00
};

/* Icytrx v0x40 static settings */
static  const uint8_t RF_ATL_REG_TBL_40[ICY40_INIT_TBL_SIZE] =
  {  0x22, 0x08, 0x00, 0x00,   0x00, 0x23, 0x0b, 0x14,    0x05, 0x20, 0x00, 0x10,   0x03, 0x00, 0xff, 0xff, // 0x0*
     0x0f, 0x00, 0x00, 0x00,   0x23, 0x82, 0x00, 0x00,    0x1b, 0xc7, 0x15, 0x82,   0x01, 0x01, 0x72, 0x1c, // 0x1*
     0x07, 0x00, 0x82, 0xff,   0x01, 0x55, 0x00, 0x00,    0x00, 0x00, 0x00, 0x00,   0x29, 0x41, 0x76, 0x71, // 0x2*
     0x03, 0x00, 0x00, 0x00,   0x2d, 0x03, 0x80, 0x00,    0x55, 0x55, 0x55, 0x00,   0x00, 0x00, 0x00, 0x00, // 0x3*
     0x00, 0x2b, 0x10, 0xc0,   0x00, 0x00, 0x00, 0x00,    0x00, 0x00, 0x01, 0x02,   0x07, 0x10, 0x20, 0x37, // 0x4*
     0x50, 0x66, 0x76, 0x7e,   0x21, 0x74, 0xca, 0xfe,    0x3f, 0x1b, 0x00, 0x02,   0x50, 0x0f, 0x30, 0x44, // 0x5*
     0x0b, 0x11, 0x55, 0x15,   0x1b, 0x0c, 0x09, 0x3a,    0x0b, 0x15, 0xfe, 0x50,   0x5c, 0x84, 0x00, 0x00, // 0x6*
     0x00, 0x00, 0x04, 0x24,   0x30, 0x81, 0x11, 0x9c,    0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00, // 0x7*
     0x00, 0x33, 0xc0, 0x22,   0xdc, 0xc8, 0x0d, 0x00,    0x00, 0x61, 0x01, 0x00,   0x00, 0x00, 0x73, 0x66, // 0x8*
     0x72, 0x99, 0x73, 0x9a,   0x77, 0x37, 0x72, 0x13,    0x00, 0x68, 0x25, 0x0f,   0x00, 0x00, 0x08, 0x7f, // 0x9*
     0x18, 0x00, 0x00, 0x80,   0xc2, 0xc2, 0x16, 0x80,    0x77, 0x00, 0x00, 0x00,   0x00, 0x35, 0x84, 0xc3, // 0xa*
     0x04, 0x04, 0xf0, 0x00,   0x03, 0x4b, 0x11, 0x00,    0x00, 0x40, 0x00, 0x00,   0x86, 0x25, 0x00, 0x03
 };

/* Icytrx v0x62 static settings - CS553 - 1Mbps table */ // Latest CSEM 0x62 + CEVA patch --- 12/07/2016 --CFGv4
static  const uint8_t RF_ATL_REG_TBL_62[ICY62_INIT_TBL_SIZE] =
{
  0x22, 0x08, 0x00, 0x00,   0x00, 0x23, 0x0b, 0x14,    0x09, 0x20, 0x00, 0x10,   0x03, 0x00, 0xff, 0xff, // 0x0*
    0x0f, 0x00, 0x00, 0x80,   0x23, 0x82, 0x1b, 0xc7,    0x15, 0x82, 0x00, 0x00,   0x01, 0x01, 0x72, 0x1c, // 0x1*
  0x07, 0x00, 0x82, 0xff,   0x01, 0x55, 0x00, 0x00,    0x00, 0x00, 0x00, 0x00,   0x29, 0x41, 0x76, 0x71, // 0x2*
  0x03, 0x2d, 0x03, 0x80,   0x00, 0x55, 0x55, 0x55,    0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00, // 0x3*
  0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x01,    0x02, 0x07, 0x10, 0x20,   0x37, 0x50, 0x66, 0x76, // 0x4*
  0x7e, 0x2b, 0x11, 0xa0,   0x21, 0x74, 0xca, 0xfe,    0x3f, 0x00, 0x04, 0x50,   0x3f, 0x1b, 0x30, 0x44, // 0x5*
    0x0b, 0x55, 0x15, 0x11,   0x19, 0x0b, 0x3a, 0x0b,    0x13, 0x0c, 0xfe, 0xb0,   0x64, 0x7e, 0x00, 0x00, // 0x6*
    0x04, 0x28, 0x40, 0x05,   0xca, 0x50, 0x95, 0xf2,    0x94, 0xb7, 0xfc, 0xe5,   0xbf, 0xff, 0x0e, 0x94, // 0x7*
    0xa2, 0x6d, 0x1b, 0x01,   0x71, 0x00, 0x33, 0xc0,    0x22, 0x61, 0x01, 0x00,   0x00, 0x00, 0x73, 0x66, // 0x8*
    0x38, 0x77, 0x74, 0x9a,   0x77, 0x66, 0x9f, 0x96,    0x06, 0x89, 0x57, 0x00,   0x00, 0x08, 0x0f, 0xc2, // 0x9*
    0xc2, 0x16, 0x18, 0x00,   0x00, 0x15, 0x25, 0x0e,    0x00, 0x00, 0x00, 0x35,   0x84, 0xc3, 0x90, 0x04, // 0xa*
    0x80, 0xf0, 0xd0, 0x03,   0xcb, 0x11, 0x00, 0x00,    0x40, 0x00, 0x00, 0x89,   0x33, 0x00, 0x09, 0x00
};

/* Icytrx v0x63 static settings - Latest CSEM 0x63 + CEVA patch --- 12/15/2017  */ 
static  const uint8_t RF_ATL_REG_TBL_63[ICY63_INIT_TBL_SIZE] =
{	
    0x22, 0x08, 0x00, 0x00,   0x00, 0x22, 0x0b, 0x14,    0x09, 0x20, 0x00, 0x10,   0x03, 0x00, 0xff, 0xff, // 0x0*
    0x0f, 0x00, 0x00, 0x80,   0x23, 0x82, 0x1b, 0xc7,    0x15, 0x82, 0x00, 0x00,   0x01, 0x01, 0x72, 0x1c, // 0x1*
    0x07, 0x00, 0x82, 0xff,   0x01, 0x55, 0x00, 0x00,    0x00, 0x00, 0x00, 0x00,   0x29, 0x41, 0x76, 0x71, // 0x2*
    0x03, 0x2d, 0x03, 0x80,   0x00, 0x55, 0x55, 0x55,    0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x00, // 0x3*
    0x00, 0x00, 0x00, 0x00,   0x00, 0x00, 0x00, 0x01,    0x02, 0x07, 0x10, 0x20,   0x37, 0x50, 0x66, 0x76, // 0x4*
    0x7e, 0x2b, 0x11, 0xa0,   0x21, 0x74, 0xca, 0xfe,    0x3f, 0x00, 0x04, 0x50,   0x2f, 0x1b, 0x30, 0x44, // 0x5*
    0x0b, 0x55, 0x15, 0x11,   0x19, 0x0b, 0x38, 0x0b,    0x12, 0x2c, 0xfe, 0xb0,   0x40, 0x6f, 0x00, 0x00, // 0x6* 
    0x04, 0x28, 0x40, 0x05,   0x4a, 0x52, 0x90, 0xaa,    0x54, 0xb5, 0xea, 0xd5,   0x2f, 0x7f, 0x0e, 0x5c, // 0x7*
    0xa2, 0x6c, 0xec, 0x01,   0x71, 0x00, 0x33, 0xc1,    0x22, 0x61, 0x01, 0x00,   0x00, 0x00, 0x73, 0x66, // 0x8*
    0x38, 0x77, 0x74, 0x9a,   0x77, 0x66, 0x9f, 0x95,    0x06, 0x89, 0x47, 0x00,   0x00, 0x08, 0x0f, 0xc2, // 0x9*
    0xc2, 0x16, 0x18, 0x00,   0x00, 0x11, 0x25, 0x0f,    0x00, 0x00, 0x00, 0x15,   0x20, 0xc3, 0x50, 0x04, // 0xa*
    0x80, 0xf0, 0xd0, 0x03,   0xc3, 0x11, 0x00, 0x00,    0x40, 0x00, 0x00, 0x8e,   0x33, 0x00, 0x19, 0xf1, // 0xb* 
};

/* Frequency Table for IcyTRx 0x13/0x14 */
static const uint32_t RF_ATL_FREQ_TBL[EM_RF_FREQ_TABLE_LEN/4] =
{
  [0x00] = 0x82810da7,   // 2404 MHz
  [0x01] = 0x82819630,   // 2406 MHz
  [0x02] = 0x82821eb8,   // 2408 MHz
  [0x03] = 0x8282a741,   // 2410 MHz
  [0x04] = 0x82832fc9,   // 2412 MHz
  [0x05] = 0x8283b852,   // 2414 MHz
  [0x06] = 0x828440da,   // 2416 MHz
  [0x07] = 0x8284c963,   // 2418 MHz
  [0x08] = 0x828551ec,   // 2420 MHz
  [0x09] = 0x8285da74,   // 2422 MHz
  [0x0A] = 0x828662fd,   // 2426 MHz
  [0x0B] = 0x8287740e,   // 2428 MHz
  [0x0C] = 0x8287fc96,   // 2430 MHz
  [0x0D] = 0x8288851f,   // 2432 MHz
  [0x0E] = 0x82890da7,   // 2434 MHz
  [0x0F] = 0x82899630,   // 2436 MHz
  [0x10] = 0x828a1eb8,   // 2438 MHz
  [0x11] = 0x828aa741,   // 2440 MHz
  [0x12] = 0x828b2fc9,   // 2442 MHz
  [0x13] = 0x828bb852,   // 2444 MHz
  [0x14] = 0x828c40da,   // 2446 MHz
  [0x15] = 0x828cc963,   // 2448 MHz
  [0x16] = 0x828d51ec,   // 2450 MHz
  [0x17] = 0x828dda74,   // 2452 MHz
  [0x18] = 0x828e62fd,   // 2454 MHz
  [0x19] = 0x828eeb85,   // 2456 MHz
  [0x1A] = 0x828f740e,   // 2458 MHz
  [0x1B] = 0x828ffc96,   // 2460 MHz
  [0x1C] = 0x8290851f,   // 2462 MHz
  [0x1D] = 0x82910da7,   // 2464 MHz
  [0x1E] = 0x82919630,   // 2466 MHz
  [0x1F] = 0x82921eb8,   // 2468 MHz
  [0x20] = 0x8292a741,   // 2470 MHz
  [0x21] = 0x82932fc9,   // 2472 MHz
  [0x22] = 0x8293b852,   // 2474 MHz
  [0x23] = 0x829440da,   // 2476 MHz
  [0x24] = 0x8294c963,   // 2478 MHz
  [0x25] = 0x8280851f,   // 2402 MHz
  [0x26] = 0x8286eb85,   // 2426 MHz
  [0x27] = 0x829551ec    // 2480 MHz
};

/* Frequency Table for IcyTRx65 v0x20*/
static const uint32_t RF_ATL_FREQ_TBL20[EM_RF_FREQ_TABLE_LEN/4] =
{
  [0x00] = 0x821638e3,    // 2404MHz
  [0x01] = 0x8216aaaa,    // 2406MHz
  [0x02] = 0x82171c71,    // 2408MHz
  [0x03] = 0x82178e38,    // 2410MHz
  [0x04] = 0x82180000,    // 2412MHz
  [0x05] = 0x821871c7,    // 2414MHz
  [0x06] = 0x8218e38e,    // 2416MHz
  [0x07] = 0x82195555,    // 2418MHz
  [0x08] = 0x8219c71c,    // 2420MHz
  [0x09] = 0x821a38e3,    // 2422MHz
  [0x0a] = 0x821aaaaa,    // 2424MHz
  [0x0b] = 0x821b8e38,    // 2428MHz
  [0x0c] = 0x821c0000,    // 2430MHz
  [0x0d] = 0x821c71c7,    // 2432MHz
  [0x0e] = 0x821ce38e,    // 2434MHz
  [0x0f] = 0x821d5555,    // 2436MHz
  [0x10] = 0x821dc71c,    // 2438MHz
  [0x11] = 0x821e38e3,    // 2440MHz
  [0x12] = 0x821eaaaa,    // 2442MHz
  [0x13] = 0x821f1c71,    // 2444MHz
  [0x14] = 0x821f8e38,    // 2446MHz
  [0x15] = 0x82200000,    // 2448MHz
  [0x16] = 0x822071c7,    // 2450MHz
  [0x17] = 0x8220e38e,    // 2452MHz
  [0x18] = 0x82215555,    // 2454MHz
  [0x19] = 0x8221c71c,    // 2456MHz
  [0x1a] = 0x822238e3,    // 2458MHz
  [0x1b] = 0x8222aaaa,    // 2460MHz
  [0x1c] = 0x82231c71,    // 2462MHz
  [0x1d] = 0x82238e38,    // 2464MHz
  [0x1e] = 0x82240000,    // 2466MHz
  [0x1f] = 0x822471c7,    // 2468MHz
  [0x20] = 0x8224e38e,    // 2470MHz
  [0x21] = 0x82255555,    // 2472MHz
  [0x22] = 0x8225c71c,    // 2474MHz
  [0x23] = 0x822638e3,    // 2476MHz
  [0x24] = 0x8226aaaa,    // 2478MHz
  [0x25] = 0x8215c71c,    // 2402MHz
  [0x26] = 0x821b1c71,    // 2426MHz
  [0x27] = 0x82271c71,    // 2480MHz
};

/* Frequency Table for IcyTRx65 v0x30 - CS1*/  /// ### TODO -> JPL To populate correctly
static const uint32_t RF_ATL_FREQ_TBL30[EM_RF_FREQ_TABLE_LEN/4] =
{
  [0x00] = 0x821638e3,    // 2404MHz
  [0x01] = 0x8216aaaa,    // 2406MHz
  [0x02] = 0x82171c71,    // 2408MHz
  [0x03] = 0x82178e38,    // 2410MHz
  [0x04] = 0x82180000,    // 2412MHz
  [0x05] = 0x821871c7,    // 2414MHz
  [0x06] = 0x8218e38e,    // 2416MHz
  [0x07] = 0x82195555,    // 2418MHz
  [0x08] = 0x8219c71c,    // 2420MHz
  [0x09] = 0x821a38e3,    // 2422MHz
  [0x0a] = 0x821aaaaa,    // 2424MHz
  [0x0b] = 0x821b8e38,    // 2428MHz
  [0x0c] = 0x821c0000,    // 2430MHz
  [0x0d] = 0x821c71c7,    // 2432MHz
  [0x0e] = 0x821ce38e,    // 2434MHz
  [0x0f] = 0x821d5555,    // 2436MHz
  [0x10] = 0x821dc71c,    // 2438MHz
  [0x11] = 0x821e38e3,    // 2440MHz
  [0x12] = 0x821eaaaa,    // 2442MHz
  [0x13] = 0x821f1c71,    // 2444MHz
  [0x14] = 0x821f8e38,    // 2446MHz
  [0x15] = 0x82200000,    // 2448MHz
  [0x16] = 0x822071c7,    // 2450MHz
  [0x17] = 0x8220e38e,    // 2452MHz
  [0x18] = 0x82215555,    // 2454MHz
  [0x19] = 0x8221c71c,    // 2456MHz
  [0x1a] = 0x822238e3,    // 2458MHz
  [0x1b] = 0x8222aaaa,    // 2460MHz
  [0x1c] = 0x82231c71,    // 2462MHz
  [0x1d] = 0x82238e38,    // 2464MHz
  [0x1e] = 0x82240000,    // 2466MHz
  [0x1f] = 0x822471c7,    // 2468MHz
  [0x20] = 0x8224e38e,    // 2470MHz
  [0x21] = 0x82255555,    // 2472MHz
  [0x22] = 0x8225c71c,    // 2474MHz
  [0x23] = 0x822638e3,    // 2476MHz
  [0x24] = 0x8226aaaa,    // 2478MHz
  [0x25] = 0x8215c71c,    // 2402MHz
  [0x26] = 0x821b1c71,    // 2426MHz
  [0x27] = 0x82271c71,    // 2480MHz
};

/* Frequency Table for IcyTRx65 v0x31 - CS2*/
static const uint32_t RF_ATL_FREQ_TBL31[EM_RF_FREQ_TABLE_LEN/4] =
{
  [0x00] = 0x821638e3,    // 2404MHz
  [0x01] = 0x8216aaaa,    // 2406MHz
  [0x02] = 0x82171c71,    // 2408MHz
  [0x03] = 0x82178e38,    // 2410MHz
  [0x04] = 0x82180000,    // 2412MHz
  [0x05] = 0x821871c7,    // 2414MHz
  [0x06] = 0x8218e38e,    // 2416MHz
  [0x07] = 0x82195555,    // 2418MHz
  [0x08] = 0x8219c71c,    // 2420MHz
  [0x09] = 0x821a38e3,    // 2422MHz
  [0x0a] = 0x821aaaaa,    // 2424MHz
  [0x0b] = 0x821b8e38,    // 2428MHz
  [0x0c] = 0x821c0000,    // 2430MHz
  [0x0d] = 0x821c71c7,    // 2432MHz
  [0x0e] = 0x821ce38e,    // 2434MHz
  [0x0f] = 0x821d5555,    // 2436MHz
  [0x10] = 0x821dc71c,    // 2438MHz
  [0x11] = 0x821e38e3,    // 2440MHz
  [0x12] = 0x821eaaaa,    // 2442MHz
  [0x13] = 0x821f1c71,    // 2444MHz
  [0x14] = 0x821f8e38,    // 2446MHz
  [0x15] = 0x82200000,    // 2448MHz
  [0x16] = 0x822071c7,    // 2450MHz
  [0x17] = 0x8220e38e,    // 2452MHz
  [0x18] = 0x82215555,    // 2454MHz
  [0x19] = 0x8221c71c,    // 2456MHz
  [0x1a] = 0x822238e3,    // 2458MHz
  [0x1b] = 0x8222aaaa,    // 2460MHz
  [0x1c] = 0x82231c71,    // 2462MHz
  [0x1d] = 0x82238e38,    // 2464MHz
  [0x1e] = 0x82240000,    // 2466MHz
  [0x1f] = 0x822471c7,    // 2468MHz
  [0x20] = 0x8224e38e,    // 2470MHz
  [0x21] = 0x82255555,    // 2472MHz
  [0x22] = 0x8225c71c,    // 2474MHz
  [0x23] = 0x822638e3,    // 2476MHz
  [0x24] = 0x8226aaaa,    // 2478MHz
  [0x25] = 0x8215c71c,    // 2402MHz
  [0x26] = 0x821b1c71,    // 2426MHz
  [0x27] = 0x82271c71,    // 2480MHz
};

/* Frequency Table for IcyTRx55 v0x40 - GCS1*/
static const uint32_t RF_ATL_FREQ_TBL40[EM_RF_FREQ_TABLE_LEN/4] =
{
  [0x00] = 0x821638e3,    // 2404MHz
  [0x01] = 0x8216aaaa,    // 2406MHz
  [0x02] = 0x82171c71,    // 2408MHz
  [0x03] = 0x82178e38,    // 2410MHz
  [0x04] = 0x82180000,    // 2412MHz
  [0x05] = 0x821871c7,    // 2414MHz
  [0x06] = 0x8218e38e,    // 2416MHz
  [0x07] = 0x82195555,    // 2418MHz
  [0x08] = 0x8219c71c,    // 2420MHz
  [0x09] = 0x821a38e3,    // 2422MHz
  [0x0a] = 0x821aaaaa,    // 2424MHz
  [0x0b] = 0x821b8e38,    // 2428MHz
  [0x0c] = 0x821c0000,    // 2430MHz
  [0x0d] = 0x821c71c7,    // 2432MHz
  [0x0e] = 0x821ce38e,    // 2434MHz
  [0x0f] = 0x821d5555,    // 2436MHz
  [0x10] = 0x821dc71c,    // 2438MHz
  [0x11] = 0x821e38e3,    // 2440MHz
  [0x12] = 0x821eaaaa,    // 2442MHz
  [0x13] = 0x821f1c71,    // 2444MHz
  [0x14] = 0x821f8e38,    // 2446MHz
  [0x15] = 0x82200000,    // 2448MHz
  [0x16] = 0x822071c7,    // 2450MHz
  [0x17] = 0x8220e38e,    // 2452MHz
  [0x18] = 0x82215555,    // 2454MHz
  [0x19] = 0x8221c71c,    // 2456MHz
  [0x1a] = 0x822238e3,    // 2458MHz
  [0x1b] = 0x8222aaaa,    // 2460MHz
  [0x1c] = 0x82231c71,    // 2462MHz
  [0x1d] = 0x82238e38,    // 2464MHz
  [0x1e] = 0x82240000,    // 2466MHz
  [0x1f] = 0x822471c7,    // 2468MHz
  [0x20] = 0x8224e38e,    // 2470MHz
  [0x21] = 0x82255555,    // 2472MHz
  [0x22] = 0x8225c71c,    // 2474MHz
  [0x23] = 0x822638e3,    // 2476MHz
  [0x24] = 0x8226aaaa,    // 2478MHz
  [0x25] = 0x8215c71c,    // 2402MHz
  [0x26] = 0x821b1c71,    // 2426MHz
  [0x27] = 0x82271c71,    // 2480MHz
};

/* Frequency Table for IcyTRx55 v0x62 - CS553 */
static const uint32_t RF_ATL_FREQ_TBL62[EM_RF_FREQ_TABLE_LEN/4] =
{
  [0x00] = 0x821638e3,    // 2404MHz
  [0x01] = 0x8216aaaa,    // 2406MHz
  [0x02] = 0x82171c71,    // 2408MHz
  [0x03] = 0x82178e38,    // 2410MHz
  [0x04] = 0x82180000,    // 2412MHz
  [0x05] = 0x821871c7,    // 2414MHz
  [0x06] = 0x8218e38e,    // 2416MHz
  [0x07] = 0x82195555,    // 2418MHz
  [0x08] = 0x8219c71c,    // 2420MHz
  [0x09] = 0x821a38e3,    // 2422MHz
  [0x0a] = 0x821aaaaa,    // 2424MHz
  [0x0b] = 0x821b8e38,    // 2428MHz
  [0x0c] = 0x821c0000,    // 2430MHz
  [0x0d] = 0x821c71c7,    // 2432MHz
  [0x0e] = 0x821ce38e,    // 2434MHz
  [0x0f] = 0x821d5555,    // 2436MHz
  [0x10] = 0x821dc71c,    // 2438MHz
  [0x11] = 0x821e38e3,    // 2440MHz
  [0x12] = 0x821eaaaa,    // 2442MHz
  [0x13] = 0x821f1c71,    // 2444MHz
  [0x14] = 0x821f8e38,    // 2446MHz
  [0x15] = 0x82200000,    // 2448MHz
  [0x16] = 0x822071c7,    // 2450MHz
  [0x17] = 0x8220e38e,    // 2452MHz
  [0x18] = 0x82215555,    // 2454MHz
  [0x19] = 0x8221c71c,    // 2456MHz
  [0x1a] = 0x822238e3,    // 2458MHz
  [0x1b] = 0x8222aaaa,    // 2460MHz
  [0x1c] = 0x82231c71,    // 2462MHz
  [0x1d] = 0x82238e38,    // 2464MHz
  [0x1e] = 0x82240000,    // 2466MHz
  [0x1f] = 0x822471c7,    // 2468MHz
  [0x20] = 0x8224e38e,    // 2470MHz
  [0x21] = 0x82255555,    // 2472MHz
  [0x22] = 0x8225c71c,    // 2474MHz
  [0x23] = 0x822638e3,    // 2476MHz
  [0x24] = 0x8226aaaa,    // 2478MHz
  [0x25] = 0x8215c71c,    // 2402MHz
  [0x26] = 0x821b1c71,    // 2426MHz
  [0x27] = 0x82271c71,    // 2480MHz
};

/* Frequency Table for IcyTRx55 v0x63 - CC6801 - Used same as 0x62 */
static const uint32_t RF_ATL_FREQ_TBL63[EM_RF_FREQ_TABLE_LEN/4] =
{
  [0x00] = 0x821638e3,    // 2404MHz
  [0x01] = 0x8216aaaa,    // 2406MHz
  [0x02] = 0x82171c71,    // 2408MHz
  [0x03] = 0x82178e38,    // 2410MHz
  [0x04] = 0x82180000,    // 2412MHz
  [0x05] = 0x821871c7,    // 2414MHz
  [0x06] = 0x8218e38e,    // 2416MHz
  [0x07] = 0x82195555,    // 2418MHz
  [0x08] = 0x8219c71c,    // 2420MHz
  [0x09] = 0x821a38e3,    // 2422MHz
  [0x0a] = 0x821aaaaa,    // 2424MHz
  [0x0b] = 0x821b8e38,    // 2428MHz
  [0x0c] = 0x821c0000,    // 2430MHz
  [0x0d] = 0x821c71c7,    // 2432MHz
  [0x0e] = 0x821ce38e,    // 2434MHz
  [0x0f] = 0x821d5555,    // 2436MHz
  [0x10] = 0x821dc71c,    // 2438MHz
  [0x11] = 0x821e38e3,    // 2440MHz
  [0x12] = 0x821eaaaa,    // 2442MHz
  [0x13] = 0x821f1c71,    // 2444MHz
  [0x14] = 0x821f8e38,    // 2446MHz
  [0x15] = 0x82200000,    // 2448MHz
  [0x16] = 0x822071c7,    // 2450MHz
  [0x17] = 0x8220e38e,    // 2452MHz
  [0x18] = 0x82215555,    // 2454MHz
  [0x19] = 0x8221c71c,    // 2456MHz
  [0x1a] = 0x822238e3,    // 2458MHz
  [0x1b] = 0x8222aaaa,    // 2460MHz
  [0x1c] = 0x82231c71,    // 2462MHz
  [0x1d] = 0x82238e38,    // 2464MHz
  [0x1e] = 0x82240000,    // 2466MHz
  [0x1f] = 0x822471c7,    // 2468MHz
  [0x20] = 0x8224e38e,    // 2470MHz
  [0x21] = 0x82255555,    // 2472MHz
  [0x22] = 0x8225c71c,    // 2474MHz
  [0x23] = 0x822638e3,    // 2476MHz
  [0x24] = 0x8226aaaa,    // 2478MHz
  [0x25] = 0x8215c71c,    // 2402MHz
  [0x26] = 0x821b1c71,    // 2426MHz
  [0x27] = 0x82271c71,    // 2480MHz
};

/* VCO sub-band init value Table for IcyTRx / All 0xFF before VCO calibration */
static uint8_t RF_ATL_VCO_TBL[EM_RF_VCO_TABLE_LEN] =
{
  [0x00] = 0xFF,   // 2404 MHz
  [0x01] = 0xFF,   // 2406 MHz
  [0x02] = 0xFF,   // 2408 MHz
  [0x03] = 0xFF,   // 2410 MHz
  [0x04] = 0xFF,   // 2412 MHz
  [0x05] = 0xFF,   // 2414 MHz
  [0x06] = 0xFF,   // 2416 MHz
  [0x07] = 0xFF,   // 2418 MHz
  [0x08] = 0xFF,   // 2420 MHz
  [0x09] = 0xFF,   // 2422 MHz
  [0x0A] = 0xFF,   // 2426 MHz
  [0x0B] = 0xFF,   // 2428 MHz
  [0x0C] = 0xFF,   // 2430 MHz
  [0x0D] = 0xFF,   // 2432 MHz
  [0x0E] = 0xFF,   // 2434 MHz
  [0x0F] = 0xFF,   // 2436 MHz
  [0x10] = 0xFF,   // 2438 MHz
  [0x11] = 0xFF,   // 2440 MHz
  [0x12] = 0xFF,   // 2442 MHz
  [0x13] = 0xFF,   // 2444 MHz
  [0x14] = 0xFF,   // 2446 MHz
  [0x15] = 0xFF,   // 2448 MHz
  [0x16] = 0xFF,   // 2450 MHz
  [0x17] = 0xFF,   // 2452 MHz
  [0x18] = 0xFF,   // 2454 MHz
  [0x19] = 0xFF,   // 2456 MHz
  [0x1A] = 0xFF,   // 2458 MHz
  [0x1B] = 0xFF,   // 2460 MHz
  [0x1C] = 0xFF,   // 2462 MHz
  [0x1D] = 0xFF,   // 2464 MHz
  [0x1E] = 0xFF,   // 2466 MHz
  [0x1F] = 0xFF,   // 2468 MHz
  [0x20] = 0xFF,   // 2470 MHz
  [0x21] = 0xFF,   // 2472 MHz
  [0x22] = 0xFF,   // 2474 MHz
  [0x23] = 0xFF,   // 2476 MHz
  [0x24] = 0xFF,   // 2478 MHz
  [0x25] = 0xFF,   // 2402 MHz
  [0x26] = 0xFF,   // 2426 MHz
  [0x27] = 0xFF    // 2480 MHz
};

/* IcyTRx dynamic register */ 
enum
{
    ICY1x_2x_CENTER_FREQ  = 0x2C,
    ICY3x_CENTER_FREQ     = 0x16,
    ICY_FSM_MODE          = 0xC0,
    ICY_TIMING            = 0x6d,
    ICY2x_SW_CAP_FSM      = 0x88,
    ICY30_TIMING          = 0x7b,
    ICY30_SW_CAP_FSM      = 0x96,
    ICY31_SW_CAP_FSM      = 0xa2,
    ICY31_TIMING          = 0x8B,
    ICY40_CENTER_FREQ     = 0x18,
    ICY40_SW_CAP_FSM      = 0xa2,
    ICY40_TIMING          = 0x8C,
    ICY62_CENTER_FREQ     = 0x16,
    ICY62_SW_CAP_FSM      = 0xa4,
    ICY62_TIMING          = 0x86,
    ICY62_SB_OFFSET       = 0xB2,
    ICY63_CENTER_FREQ     = 0x16, //same as 0x62
    ICY63_SW_CAP_FSM      = 0xa4, //same as 0x62
    ICY63_TIMING          = 0x86, //same as 0x62
    ICY63_SB_OFFSET       = 0xB2  //same as 0x62
};

/**
 ****************************************************************************************
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief SPI access
 ***************************************************************************************
 */
static void rf_atl_spi_tf(void)
{
    //launch SPI transfer
    ble_spigo_setf(1);

    //wait for transfer to be completed
    while (!ble_spicomp_getf());
}

/**
 ****************************************************************************************
 * @brief Atlas specific read access
 *
 * @param[in] addr    register address
 *
 * @return uint32_t value
 *****************************************************************************************
 */
static uint32_t rf_atl_reg_rd (uint16_t addr)
{
    // Next Pointr to 0x0
    rf_atl_reg_buf[0] = (uint8_t)(0);  // Next Pointer set to 0x0000 to stop the SPI Chained access
    rf_atl_reg_buf[1] = (uint8_t)(0);

    //copy control and number of u32 to send
    rf_atl_reg_buf[2] = (uint8_t)(ATL_SPIRD + 1);

    //copy address
    rf_atl_reg_buf[3] = addr;

    memcpy((void *)RF_EM_SPI_ADRESS, rf_atl_reg_buf, 4);

    //do the transfer
    rf_atl_spi_tf();

    return (uint32_t)(*((uint8_t *)(RF_EM_SPI_ADRESS + 4)));
}

/**
 ****************************************************************************************
 * @brief Atlas specific write access
 *
 * @param[in] addr    register address
 * @param[in] value   value to write
 *
 * @return uint32_t value
 ****************************************************************************************
 */
static void rf_atl_reg_wr (uint16_t addr, uint32_t value)
{
    rf_atl_reg_buf[0] = (uint8_t)(0);  // Next Pointer set to 0x0000 to stop the SPI Chained access
    rf_atl_reg_buf[1] = (uint8_t)(0);

    //inversion for EM reading by U8 on ATL SPI side
    //copy control and number of u32 to send
    rf_atl_reg_buf[2] = (uint8_t)(ATL_SPIWR + 1);

    //copy address
    rf_atl_reg_buf[3] = (uint8_t)(addr);

    //on old implementations (BT core 3.0, BLE core 1.0) swap the data
    rf_atl_reg_buf[4]= (uint8_t)value;

    memcpy((void *)RF_EM_SPI_ADRESS, rf_atl_reg_buf, 5);

    //do the transfer
    rf_atl_spi_tf();
}

/**
 ****************************************************************************************
 * @brief Atlas specific read access
 *
 * @param[in] addr    register address
 * @param[in] size    transfer size
 * @param[in] data    pointer to the data array
 *
 * @return uint32_t value
 ****************************************************************************************
 **/
static void rf_atl_reg_burst_wr (uint16_t addr, uint8_t size, uint8_t *data)
{
    rf_atl_reg_buf[0] = (uint8_t)(0);  // Next Pointer set to 0x0000 to stop the SPI Chained access
    rf_atl_reg_buf[1] = (uint8_t)(0);

    //copy control and number of u8 to send
    rf_atl_reg_buf[2] = (uint8_t)(ATL_SPIWR + size);

    //copy address
    rf_atl_reg_buf[3] = (uint8_t)(addr);

    for(int i =0;i < size + 2;i++)
    {
        rf_atl_reg_buf[i + 4] = *(data + i);
    }

    memcpy((void *)RF_EM_SPI_ADRESS, rf_atl_reg_buf, 4 + size);

    //do the transfer
    rf_atl_spi_tf();
}

/**
 *****************************************************************************************
 * @brief Init RF sequence after reset.
 *****************************************************************************************
 */
static void rf_reset(void)
{
}

/**
 ****************************************************************************************
 * @brief ISR to be called in BLE ISR routine when RF Interrupt occurs.
 *****************************************************************************************
 */
static void rf_force_agc_enable(bool en)
{
}

/**
 *****************************************************************************************
 * @brief Get TX power in dBm from the index in the control structure
 *
 * @param[in] txpwr_idx  Index of the TX power in the control structure
 * @param[in] modulation Modulation: 1 or 2 or 3 MBPS
 *
 * @return The TX power in dBm
 *
 *****************************************************************************************
 */
static uint8_t rf_txpwr_dbm_get(uint8_t txpwr_idx, uint8_t modulation)
{
    // Power table should be provided
    return(0);
}

/**
 *****************************************************************************************
 * @brief Sleep function for Atlas RF.
 *****************************************************************************************
 */
static void rf_sleep(void)
{
    #if defined(CFG_BLE)
    ble_deepslcntl_set(ble_deepslcntl_get() |
                      BLE_DEEP_SLEEP_ON_BIT |    // RW BLE Core sleep
                      BLE_RADIO_SLEEP_EN_BIT |   // Radio sleep
                      BLE_OSC_SLEEP_EN_BIT);     // Oscillator sleep
    #endif // CFG_BLE
}

/**
 *****************************************************************************************
 * @brief Convert RSSI to dBm
 *
 * @param[in] rssi_reg RSSI read from the HW registers
 *
 * @return The converted RSSI
 *
 *****************************************************************************************
 */
static int8_t rf_rssi_convert (uint8_t rssi_reg)
{
    uint8_t RssidBm = 0; 

    RssidBm = ((rssi_reg) >> 1) - 118;

    return(RssidBm);
}

#define REG_WRITE(addr, data)    *((uint32_t *) addr) = data
#define REG_READ(addr, data)     data = *((uint32_t *) addr)

static void _spi_go(void)
{
    volatile uint32_t    _dwSpiComp = 0;

    REG_WRITE(0x40004070, 0x00000001);    /* SPIGO */
  
    while (1)    /* wait SPI for COMPLETE */
    {
        REG_READ(0x40004070, _dwSpiComp);

        if (_dwSpiComp & 0x00000002)
            break;
    }
}

void    _delay_100us(void)
{
    volatile uint64_t    _lDelayCount = 0;

    for (_lDelayCount = 0; _lDelayCount < 100; _lDelayCount++)
    {
    }
}

void    _delay_300us(void)
{
    volatile uint64_t    _lDelayCount = 0;

    for (_lDelayCount = 0; _lDelayCount < 300; _lDelayCount++)
    {
    }
}

void _Radio55nmPmuInit(void)
{
    #if defined(TC4CC6801)
        drvi_GpioWrite(GPIO_PIN_0, 1); //DCDC SPI
    
        #ifdef DCDC_MODE
            /****Turn-ON the DCDC and put it in DCDC mode****/
            REG_WRITE(0x40004070, 0x00880000);
            REG_WRITE(0x40004074, 0x04031000); 
            REG_WRITE(0x40007000, 0x08811008); // address = 0x08
            REG_WRITE(0x40007004, 0x0000007F); // if it�s not set, the circuit drains 250mA
            REG_WRITE(0x40007008, 0x00810000); // address = 0x00
            REG_WRITE(0x4000700C, 0x00000007); //Used the mode register to set the dcdc in DCDC buck mode
            _spi_go();
        #else  // LDO
            /****Turn-ON the DCDC and put it in LDO mode****/
            REG_WRITE(0x40004070, 0x00880000);
            REG_WRITE(0x40004074, 0x04031000); 
            REG_WRITE(0x40007000, 0x08811008); // address = 0x08
            REG_WRITE(0x40007004, 0x0000007F); // if it�s not set, the circuit drains 250mA
            REG_WRITE(0x40007008, 0x00810000); // address = 0x00
            REG_WRITE(0x4000700C, 0x00000002); //Used the mode register to set the dcdc in LDO mode
            _spi_go();    
        #endif    
    
        _delay_300us();    //ADD 300us delay//This delay lets the LDO to settle
        _delay_300us();
    
        /****Turn-ON the VDDD (PTAT and BGref are turned ON too)****/
        REG_WRITE(0x40004070, 0x00880000);
        REG_WRITE(0x40004074, 0x04031010);
        REG_WRITE(0x40007010, 0x03810000); // address = 0x03
        REG_WRITE(0x40007014, 0x00000006); // Icytrx_pmu enables - vddd, bandgap and the ptat are enabled
        _spi_go();
    
        _delay_100us();    //ADD 100us delay// This lets the vddd to start-up
    
        /****Trimming VDDD to 1.2V******/
        REG_WRITE(0x40004070, 0x00880000);
        REG_WRITE(0x40004074, 0x04031010);
        REG_WRITE(0x40007010, 0x0B810000); // address = 0x0B // write access
        REG_WRITE(0x40007014, 0x000000C8); // Trimming vddd to 1.0 V and enable vddd transient
        _spi_go();
    
        _delay_100us();    //ADD 100us delay// This lets the vddd to settle
    
        drvi_GpioWrite(GPIO_PIN_0, 0); //Radio SPI
    #endif  //(TC4CC6801)
    
    REG_WRITE(0x40004070, 0x00880000);
    REG_WRITE(0x40004074, 0x04031000);
    REG_WRITE(0x40007000, 0xE1810000); // address = 0xe1
    REG_WRITE(0x40007004, 0x000000ff); // Turning on all the LDOs
    //   REG_WRITE(0x40007008, 0xE2811010); // address = 0xe2
    //   REG_WRITE(0x4000700c, 0x0000004a); // trimming value LDO_syn , LDO_pa
    //   REG_WRITE(0x40007010, 0xE3810000); // address = 0xe3
    //   REG_WRITE(0x40007014, 0x00000029); // trimming value for LDO_a
    _spi_go();
}

/**
 ****************************************************************************************
 * RADIO FUNCTION INTERFACE
 ****************************************************************************************
 **/

void rf_init(struct rwip_rf_api *api)
{
   	volatile uint32_t icy_version = 0; // Default version is Atlas
	uint8_t idx = 0;

	#if !defined(RP_HWSIM_BYPASS)
	uint8_t vco_sub_read = 0;
	#endif // RP_HWSIM_BYPASS

  #if defined(CC6801_MPW) || defined(TC4CC6801)
    _Radio55nmPmuInit();
  #endif

   // Initialize the RF driver API structure
  api->reg_rd = rf_atl_reg_rd;
  api->reg_wr = rf_atl_reg_wr;
  api->txpwr_dbm_get = rf_txpwr_dbm_get;
  api->txpwr_max = ATL_POWER_MAX;
  api->sleep = rf_sleep;
  api->reset = rf_reset;
  api->force_agc_enable = rf_force_agc_enable;
  api->rssi_convert = rf_rssi_convert;

  // RF Selection
  ble_xrfsel_setf(0x03);

  // Set pointer SW SPI Drive access Pointer
  ble_spiptr_setf(EM_RF_SW_SPI_OFFSET);

  // Detect the RF version
  icy_version = rf_atl_reg_rd(0xFF);

  // Select proper sub-version of IcyTRx Radio Controller / Need BLE Core xls update 1st
  switch(icy_version)
    {
    case(0x30):  // CS1
      ble_subversion_setf(0x1);
      // Set Platform RF selection register
      plf_rf_interface_sel(RF_INTF_V3X_V4X);
    break;
    case(0x31):  // CS2
      ble_subversion_setf(0x2);
      // Set Platform RF selection register
      plf_rf_interface_sel(RF_INTF_V3X_V4X);
	break;
    case(0x40):  // GCS1
      ble_subversion_setf(0x3);
      // Set Platform RF selection register
      plf_rf_interface_sel(RF_INTF_V3X_V4X);
    case(0x62):// CS553
      ble_subversion_setf(0x4);
      // Set Platform RF selection register
      plf_rf_interface_sel(RF_INTF_V3X_V4X);
    break;
    case(0x63):// Used same as the 0x62
      ble_subversion_setf(0x4);
      // Set Platform RF selection register
      plf_rf_interface_sel(RF_INTF_V3X_V4X);
    break;
    default:     // v0x1- to 0x2-
      ble_subversion_setf(0x0);
      // Reset Platform RF selection register
      plf_rf_interface_sel(RF_INTF_V1X_V2X);
    break;
    }

  // Tx/Rx Power Up - Sync Position - JEF Select
  switch(icy_version)
  {
  case(0x13):
    ble_rtrip_delay_setf(0x30); // Stable 30 Sept. 2014
    ble_rxpwrup_setf(0x60);
	ble_txpwrdn_setf(0x0F);
	ble_txpwrup_setf(0x27);
	ble_sync_position_setf(0x0);
	ble_sync_pulse_mode_setf(0x1);
	ble_jef_select_setf(0x1);
    ble_rfrxtmda_setf(0x1);
	break;
  case(0x14):
    ble_rtrip_delay_setf(0x34); // Stable 30 Sept. 2014
    ble_rxpwrup_setf(0x60);
	ble_txpwrdn_setf(0x0F);
	ble_txpwrup_setf(0x27);
	ble_sync_position_setf(0x0);
	ble_sync_pulse_mode_setf(0x1);
	ble_jef_select_setf(0x1);
    ble_rfrxtmda_setf(0x1);
	break;
  case(0x20):
    ble_rtrip_delay_setf(0x26); // Stable May 5 2014 - Realigned with OT
    ble_rxpwrup_setf(0x6B);
	ble_txpwrdn_setf(0x0F);
	ble_txpwrup_setf(0x1B);
	ble_sync_position_setf(0x0);
	ble_sync_pulse_mode_setf(0x1);
	ble_jef_select_setf(0x1);
    ble_rfrxtmda_setf(0x1);
	break;
  case(0x30):
    ble_rtrip_delay_setf(0x34);  // Stable May 21 2015
    ble_rxpwrup_setf(0x6b);
    ble_txpwrdn_setf(0x0f);
    ble_txpwrup_setf(0x2b);
    ble_sync_position_setf(0x0);
    ble_sync_pulse_mode_setf(0x1);
    ble_jef_select_setf(0x1);
    ble_rfrxtmda_setf(0x1);
	break;
  case(0x31):
	ble_rtrip_delay_setf(0x3E);  // Stable May 19 2015
	ble_rxpwrup_setf(0x6b);
	ble_txpwrdn_setf(0x0f);
	ble_txpwrup_setf(0x29);
	ble_sync_position_setf(0x0);
	ble_sync_pulse_mode_setf(0x1);
	ble_jef_select_setf(0x1);
	ble_rfrxtmda_setf(0x1);
	break;
  case(0x40):
    ble_rtrip_delay_setf(0x3E);  // Stable Nov 2015
    ble_rxpwrup_setf(0x6b);
    ble_txpwrdn_setf(0x0f);
    ble_txpwrup_setf(0x29);
    ble_sync_position_setf(0x0);
    ble_sync_pulse_mode_setf(0x1);
    ble_jef_select_setf(0x1);
    ble_rfrxtmda_setf(0x1);
    break;
  case(0x62):// To be verified
	ble_rtrip_delay_setf(0x3E);
	ble_rxpwrup_setf(0x6b);
	ble_txpwrdn_setf(0x0f);
	ble_txpwrup_setf(0x29);
	ble_sync_position_setf(0x0);
	ble_sync_pulse_mode_setf(0x1);
	ble_jef_select_setf(0x1);
	ble_rfrxtmda_setf(0x1);
        break;
  case(0x63):// Used same as 0x62
	ble_rtrip_delay_setf(0x3E);
	ble_rxpwrup_setf(0x6b);
	ble_txpwrdn_setf(0x0f);
	ble_txpwrup_setf(0x29);
	ble_sync_position_setf(0x0);
	ble_sync_pulse_mode_setf(0x1);
	ble_jef_select_setf(0x1);
	ble_rfrxtmda_setf(0x1);
        break;
  default:
   ASSERT_ERR(0);
  break;
  }

  // IcyTRx Register Initialization
  switch(icy_version)
  {
  case(0x13):
	  rf_atl_reg_burst_wr(0x00, 0x70, (uint8_t *) &RF_ATL_REG_TBL_13[0]);
	  rf_atl_reg_burst_wr(0x70, 0x18, (uint8_t *) &RF_ATL_REG_TBL_13[0x70]);
	break;
  case(0x14):
		rf_atl_reg_burst_wr(0x00, 0x70, (uint8_t *) &RF_ATL_REG_TBL_14[0]);
		rf_atl_reg_burst_wr(0x70, 0x18, (uint8_t *) &RF_ATL_REG_TBL_14[0x70]);
	break;
  case(0x20):
		rf_atl_reg_burst_wr(0x00, 0x70, (uint8_t *) &RF_ATL_REG_TBL_20[0]);
		rf_atl_reg_burst_wr(0x70, 0x27, (uint8_t *) &RF_ATL_REG_TBL_20[0x70]);
	break;
  case(0x30):
		rf_atl_reg_burst_wr(0x00, 0x70, (uint8_t *) &RF_ATL_REG_TBL_30[0]);
		rf_atl_reg_burst_wr(0x70, 0x37, (uint8_t *) &RF_ATL_REG_TBL_30[0x70]);
	break;
  case(0x31):
		rf_atl_reg_burst_wr(0x00, 0x70, (uint8_t *) &RF_ATL_REG_TBL_31[0]);
		rf_atl_reg_burst_wr(0x70, 0x4C, (uint8_t *) &RF_ATL_REG_TBL_31[0x70]);
	break;
  case(0x40):
        rf_atl_reg_burst_wr(0x00, 0x70, (uint8_t *) &RF_ATL_REG_TBL_40[0]);
        rf_atl_reg_burst_wr(0x70, 0x4E, (uint8_t *) &RF_ATL_REG_TBL_40[0x70]);
        rf_atl_reg_wr(0xBE, RF_ATL_REG_TBL_40[0xBE]);
    break;
  case(0x62): // CS553
       rf_atl_reg_burst_wr(0x00, 0x70, (uint8_t *) &RF_ATL_REG_TBL_62[0]);
       rf_atl_reg_burst_wr(0x70, 0x50, (uint8_t *) &RF_ATL_REG_TBL_62[0x70]);
    break;
  case(0x63): //Used same as 0x62
       rf_atl_reg_burst_wr(0x00, 0x70, (uint8_t *) &RF_ATL_REG_TBL_63[0]);
       rf_atl_reg_burst_wr(0x70, 0x50, (uint8_t *) &RF_ATL_REG_TBL_63[0x70]);
    break;
  default:
    ASSERT_ERR(0);
  break;
  }

  // Frequency Table Programming
  uint32_t *ft_ptr=(uint32_t *)(EM_BASE_ADDR+EM_FT_OFFSET);
  for(idx=0; idx < EM_RF_FREQ_TABLE_LEN/4; idx++)
  {
    switch(icy_version)
    {
    case(0x13):
      *ft_ptr++=RF_ATL_FREQ_TBL[idx];
	  break;
    case(0x14):
      *ft_ptr++=RF_ATL_FREQ_TBL[idx];
	  break;
    case(0x20):
      *ft_ptr++=RF_ATL_FREQ_TBL20[idx];
	  break;
    case(0x30):
      *ft_ptr++=RF_ATL_FREQ_TBL30[idx];
      break;
    case(0x31):
      *ft_ptr++=RF_ATL_FREQ_TBL31[idx];
	  break;
    case(0x40):
      *ft_ptr++=RF_ATL_FREQ_TBL40[idx];
      break;
    case(0x62):
      *ft_ptr++=RF_ATL_FREQ_TBL62[idx];
      break;
    case(0x63)://Used same as the 0x62
      *ft_ptr++=RF_ATL_FREQ_TBL63[idx];
      break;
    default:
      ASSERT_ERR(0);
    break;
    }
  }
  
  // VCO sub-band table Reset
  uint8_t *vco_ptr=(uint8_t *)(EM_BASE_ADDR+EM_FT_OFFSET+EM_RF_FREQ_TABLE_LEN);
  for(idx=0; idx < EM_RF_VCO_TABLE_LEN; idx++)
    {
    *vco_ptr++=RF_ATL_VCO_TBL[idx];
    }

  #if !defined(RP_HWSIM_BYPASS)
  // VCO Sub-band Calibration process / bypassed in HW simulations
  switch(icy_version)
  {
  case(0x20):
    rf_atl_reg_wr(ICY_TIMING,0x90);
	break;
  case(0x30):
    rf_atl_reg_wr(ICY30_TIMING,0xB0);
    break;
  case(0x31):
	rf_atl_reg_wr(ICY31_TIMING,0x81);
	break;
  case(0x40):
    //rf_atl_reg_wr(ICY40_TIMING,0x81);
    break;
  case(0x62):
    rf_atl_reg_wr(ICY62_SB_OFFSET,0x00);
    break;
  case(0x63)://Used same as 0x62 
    rf_atl_reg_wr(ICY63_SB_OFFSET,0x00);
    break;
  default:
  break;
  }

  for(idx=0; idx < EM_RF_VCO_TABLE_LEN; idx++)
    {
    // Initialize VCO sub Table pointer in EM
    uint8_t *ptr_vcosub=(uint8_t *)(EM_BASE_ADDR+EM_FT_OFFSET+EM_RF_FREQ_TABLE_LEN+idx);

    // Program PLLCNTL values / take value from RF_ATL_FREQ_TBL
    switch(icy_version)
    {
    case(0x20):
      rf_atl_reg_burst_wr(ICY1x_2x_CENTER_FREQ, 4, (uint8_t *) &RF_ATL_FREQ_TBL20[idx]);
	  break;
    case(0x30):
      rf_atl_reg_burst_wr(ICY3x_CENTER_FREQ, 4, (uint8_t *) &RF_ATL_FREQ_TBL30[idx]);
	  break;
    case(0x31):
      rf_atl_reg_burst_wr(ICY3x_CENTER_FREQ, 4, (uint8_t *) &RF_ATL_FREQ_TBL31[idx]);
    break;
    case(0x40):
       rf_atl_reg_burst_wr(ICY40_CENTER_FREQ, 4, (uint8_t *) &RF_ATL_FREQ_TBL40[idx]);
    break;
    case(0x62):
       rf_atl_reg_burst_wr(ICY62_CENTER_FREQ, 4, (uint8_t *) &RF_ATL_FREQ_TBL62[idx]);
    break;
    case(0x63): //Used same as 0x62
       rf_atl_reg_burst_wr(ICY63_CENTER_FREQ, 4, (uint8_t *) &RF_ATL_FREQ_TBL63[idx]);
    break;
    default:
      rf_atl_reg_burst_wr(ICY1x_2x_CENTER_FREQ, 4, (uint8_t *) &RF_ATL_FREQ_TBL[idx]);
    break;
    }

    // TxEN + Enable VCO Calibration in RF for v0x1-,  v0x2- and CS1 / @0xC0 write 0x6
    // RxEN + Enable VCO Calibration in RF for CS2 / @0xC0 write 0x3
    switch(icy_version)
    {
    case(0x31):
       rf_atl_reg_wr(ICY_FSM_MODE,0x03);
       break;
    case(0x62):
       rf_atl_reg_wr(ICY_FSM_MODE,0x02);
       break;
    case(0x63)://Used same as 0x62
       rf_atl_reg_wr(ICY_FSM_MODE,0x02);
       break;
    default:
       rf_atl_reg_wr(ICY_FSM_MODE,0x06);
       break;
    }

    // Poll on @0xC0 until it is equal to 0x0
    while (rf_atl_reg_rd(ICY_FSM_MODE) != 0x00);

    // WARNING !! Problem of SW dynamic resolved by adding this loop in order to prevent from erasing VCO table @ index 0
    for(uint32_t cpt=0;cpt<1000;cpt++);

    // Store VCO sub-band read value
    switch(icy_version)
      {
      case(0x30):
        vco_sub_read = rf_atl_reg_rd((uint16_t)ICY30_SW_CAP_FSM);
        // Invert half byte as Rx VCO sub-band programming is using bit [7:4]
        *ptr_vcosub = (vco_sub_read << 4) | (vco_sub_read >> 4);
      break;
      case(0x31):
        vco_sub_read = rf_atl_reg_rd((uint16_t)ICY31_SW_CAP_FSM);
        *ptr_vcosub = vco_sub_read;
      break;
      case(0x40):
        vco_sub_read = rf_atl_reg_rd((uint16_t)ICY40_SW_CAP_FSM);
        *ptr_vcosub = (vco_sub_read << 4) | (vco_sub_read >> 4);
      break;
      case(0x62):
        vco_sub_read = rf_atl_reg_rd((uint16_t)ICY62_SW_CAP_FSM);
        *ptr_vcosub = vco_sub_read;
      break;
      case(0x63)://Used same as 0x62
        vco_sub_read = rf_atl_reg_rd((uint16_t)ICY63_SW_CAP_FSM);
        *ptr_vcosub = vco_sub_read;
      break;
      default:
        vco_sub_read = rf_atl_reg_rd((uint16_t)ICY2x_SW_CAP_FSM);
        // Invert half byte as Rx VCO sub-band programming is using bit [7:4]
        *ptr_vcosub = (vco_sub_read << 4) | (vco_sub_read >> 4);
      break;
      }
    }

    switch(icy_version)
    {
    case(0x20):
      rf_atl_reg_wr(ICY_TIMING,0x40);
	break;
    case(0x30):
      rf_atl_reg_wr(ICY30_TIMING,0xB0);
    break;
    case(0x31):
      rf_atl_reg_wr(ICY31_TIMING,0x00);
    break;
    case(0x40):
     // rf_atl_reg_wr(ICY40_TIMING,0xB0);
	break;
    case(0x62):
      rf_atl_reg_wr(ICY62_SB_OFFSET,0xD0);
    break;
    case(0x63)://Used same as 0x62
      rf_atl_reg_wr(ICY63_SB_OFFSET,0xD0);
    break;
    default:
    break;
    }

  #endif // RP_HWSIM_BYPASS

  /* *************************************************************************************** */
  /* Initialize HW SPI Chains Pointers  */
  /* *************************************************************************************** */
  uint16_t txonptr  = (EM_RF_HW_SPI_OFFSET);
  uint16_t txoffptr = (0);
  uint16_t rxonptr  = (EM_RF_HW_SPI_OFFSET+16);
  uint16_t rxoffptr = (EM_RF_HW_SPI_OFFSET+16+32);
  uint16_t rssiptr  = (EM_RF_HW_SPI_OFFSET+16+32+8);

  /* TxOn Sequence start pointer */
  ble_txonptr_setf(txonptr);

  /* TxOff Sequence start pointer */
  ble_txoffptr_setf(txoffptr);

  /* RxOn Sequence start pointer */
  ble_rxonptr_setf(rxonptr);

  /* RxOff Sequence start pointer */
  ble_rxoffptr_setf(rxoffptr);

  /* RSSI Sequence start pointer */
  ble_rssiptr_setf(rssiptr);

  /* *************************************************************************************** */
  /* Initialize HW SPI Tx On Chained list  -> 2 structures of 8 bytes*/
  /* *************************************************************************************** */
  /*   TxON Access 1 -> PLL word
               -> Next Pointer = txonptr+0x8
               -> Write Address @0x2C / 4 byte for v0x1- to v0x2
               -> Write Address @0x16 / 4 byte for CS1 and CS2
               -> Write data = 0x00000000 -> Will be replaced by PLL word value from table above */
  RF_ICTRX_EM_BLE_WR(txonptr,     txonptr+0x8);
  switch(icy_version)
  {
  case(0x30):   // CS1
	RF_ICTRX_EM_BLE_WR(txonptr+0x2, 0x1684);
  break;
  case(0x31):   // CS2
	RF_ICTRX_EM_BLE_WR(txonptr+0x2, 0x1684);
  break;
  case(0x40):   // GCS1
    RF_ICTRX_EM_BLE_WR(txonptr+0x2, 0x1884);
  break;
  case(0x62):   // CS553
    RF_ICTRX_EM_BLE_WR(txonptr+0x2, 0x1684);
  break;
  case(0x63):   // Used same as 0x62
    RF_ICTRX_EM_BLE_WR(txonptr+0x2, 0x1684);
  break;
  default:
	RF_ICTRX_EM_BLE_WR(txonptr+0x2, 0x2C84);
  break;
  }
  RF_ICTRX_EM_BLE_WR(txonptr+0x4, 0x0000);
  RF_ICTRX_EM_BLE_WR(txonptr+0x6, 0x0000);

  /*   TxON Access 2 -> Tx enable
               -> Next Pointer = 0x0000 -> end of Sequence
               -> Write Address @0xC0 / 1 byte / Valid for all RF version at the moment
               -> Write data = 0x07 */
  RF_ICTRX_EM_BLE_WR(txonptr+0x8, 0x0000);
  RF_ICTRX_EM_BLE_WR(txonptr+0xA, 0xC081);
  RF_ICTRX_EM_BLE_WR(txonptr+0xC, 0x0007);
  RF_ICTRX_EM_BLE_WR(txonptr+0xE, 0x0000);

  /* *************************************************************************************** */
  /* Initialize HW SPI Tx Off Chained list -> Nothing here at the moment */
  /* *************************************************************************************** */

  /* *************************************************************************************** */
  /* Initialize HW SPI Rx On Chained list -> 4 structures of 8 bytes */
  /* *************************************************************************************** */
  /* Initialize HW SPI Rx On Chained list  */
  /*   RxON Access 1 -> PLL word
               -> Next Pointer = rxonptr+0x8
               -> Write Address @0x2C / 4 byte for v0x1- to v0x2
               -> Write Address @0x16 / 4 byte for CS1 and CS2
                -> Write data = 0x00000000 -> Will be replaced by PLL word value from table in EM*/
  RF_ICTRX_EM_BLE_WR(rxonptr,      rxonptr+0x8);
  switch(icy_version)
  {
  case(0x30):   // CS1
	RF_ICTRX_EM_BLE_WR(rxonptr+0x2, 0x1684);
  break;
  case(0x31):   // CS2
	RF_ICTRX_EM_BLE_WR(rxonptr+0x2, 0x1684);
  break;
  case(0x40):   // GCS1
    RF_ICTRX_EM_BLE_WR(rxonptr+0x2, 0x1884);
  break;
  case(0x62):   // CS553
    RF_ICTRX_EM_BLE_WR(rxonptr+0x2, 0x1684);
  break;
  case(0x63):   // Used same as the 0x62
    RF_ICTRX_EM_BLE_WR(rxonptr+0x2, 0x1684);
  break;
  default:
	RF_ICTRX_EM_BLE_WR(rxonptr+0x2, 0x2C84);
  break;
  }
  RF_ICTRX_EM_BLE_WR(rxonptr+0x4,  0x0000);
  RF_ICTRX_EM_BLE_WR(rxonptr+0x6,  0x0000);

  /*   RxON Access 2 -> VCO SW Cap (v0x1- and v0x2-) or VCO SW Cap auto Calibration (v0x3-)
               -> Next Pointer = rxonptr+0x10
               -> Write Address @0x88 / 1 byte / for v0x1- to v0x2-
               -> Write Address @0x96 / 1 byte / for CS1
               -> Write Address @0xA2 / 1 byte / for CS2 -> replaced below
               -> Write data = 0x00 -> will be replaced by VCO sub-band value from table in EM */
  RF_ICTRX_EM_BLE_WR(rxonptr+0x8,  rxonptr+0x10);
  switch(icy_version)
  {
  case(0x30):   // CS1
	RF_ICTRX_EM_BLE_WR(rxonptr+0xA,  0x9681);
    RF_ICTRX_EM_BLE_WR(rxonptr+0xC,  0x0000);
  break;
  case(0x31):   // CS2
	RF_ICTRX_EM_BLE_WR(rxonptr+0xA,  0xA281);
    RF_ICTRX_EM_BLE_WR(rxonptr+0xC,  0x0000);
  break;
  case(0x40):   // GCS1
    RF_ICTRX_EM_BLE_WR(rxonptr+0xA,  0xA281);
    RF_ICTRX_EM_BLE_WR(rxonptr+0xC,  0x0000);
  break;
  case(0x62):   // CS553
    RF_ICTRX_EM_BLE_WR(rxonptr+0xA,  0xA481);
    RF_ICTRX_EM_BLE_WR(rxonptr+0xC,  0x0000);
  break;
  case(0x63):   // Used same as the 0x62
    RF_ICTRX_EM_BLE_WR(rxonptr+0xA,  0xA481);
    RF_ICTRX_EM_BLE_WR(rxonptr+0xC,  0x0000);
  break;
  default:
	RF_ICTRX_EM_BLE_WR(rxonptr+0xA,  0x8881);
	RF_ICTRX_EM_BLE_WR(rxonptr+0xC,  0x0000);
  break;
  }

  RF_ICTRX_EM_BLE_WR(rxonptr+0xE,  0x0000);

  /*   RxON Access 3 -> Rx enable
               -> Next Pointer = 0x0000 -> rxonptr+0x18
               -> Write Address @0xC0 / 1 byte / Valid for all RF version at the moment
               -> Write data = 0x01 (for v0x1- or v0x2-) or 0x03 (for CS2) */
  RF_ICTRX_EM_BLE_WR(rxonptr+0x10, rxonptr+0x18);
  RF_ICTRX_EM_BLE_WR(rxonptr+0x12, 0xC081);
  switch(icy_version)
  {
  case(0x31):   // CS2
	RF_ICTRX_EM_BLE_WR(rxonptr+0x14, 0x0001);
  break;
  case(0x40):   // GCS1
    RF_ICTRX_EM_BLE_WR(rxonptr+0x14, 0x0001);
  break;
  case(0x62):   // CS553
    RF_ICTRX_EM_BLE_WR(rxonptr+0x14, 0x0001);
  break;
  case(0x63):   // Used samae as the 0x62
    RF_ICTRX_EM_BLE_WR(rxonptr+0x14, 0x0001);
  break;
  default:
	RF_ICTRX_EM_BLE_WR(rxonptr+0x14, 0x0001);
  break;
  }
  RF_ICTRX_EM_BLE_WR(rxonptr+0x16, 0x0000);
    
  /*   RxON Access 4 -> Access Address Write
               -> Next Pointer = 0x0000 -> end of Sequence
               -> Write Address @0x0F / 4 byte / for version v0x1- to v0x2-
               -> Write Address @0x2C / 4 byte / for CS1 and CS2
               -> Write data = 0x00 -> will be replaced by 32bit Access Address by the Radio Controller */
  RF_ICTRX_EM_BLE_WR(rxonptr+0x18, 0x0000);
  switch(icy_version)
  {
  case(0x30):   // CS1
	RF_ICTRX_EM_BLE_WR(rxonptr+0x1A, 0x2C84);
  break;
  case(0x31):   // CS2
	RF_ICTRX_EM_BLE_WR(rxonptr+0x1A, 0x2C84);
  break;
  case(0x40):   // GCS1
    RF_ICTRX_EM_BLE_WR(rxonptr+0x1A, 0x2C84);
  break;
  case(0x62):   // CS553
    RF_ICTRX_EM_BLE_WR(rxonptr+0x1A, 0x2C84);
  break;
  case(0x63):   // Used same as the 0x62
    RF_ICTRX_EM_BLE_WR(rxonptr+0x1A, 0x2C84);
  break;
  default:
    RF_ICTRX_EM_BLE_WR(rxonptr+0x1A, 0x0F84);
  break;
  }
  RF_ICTRX_EM_BLE_WR(rxonptr+0x1C, 0x0000);
  RF_ICTRX_EM_BLE_WR(rxonptr+0x1E, 0x0000);

  /* *************************************************************************************** */
  /* Initialize HW SPI Rx Off Chained list -> 1 structure of 8 bytes */
  /* *************************************************************************************** */
  /*    RxOFF Access 1  -> Rx Disable
                -> Next Pointer = 0x0000 -> end of Sequence
                -> Write Address @0xC0 / 1 byte
                -> Write data = 0x08 */

  RF_ICTRX_EM_BLE_WR(rxoffptr,     0x0000);
  RF_ICTRX_EM_BLE_WR(rxoffptr+0x2, 0xC081);
  RF_ICTRX_EM_BLE_WR(rxoffptr+0x4, 0x0008);
  RF_ICTRX_EM_BLE_WR(rxoffptr+0x6, 0x0000);

  /* *************************************************************************************** */
  /* Initialize HW SPI RSSI Chained list  -> 1 structure of 8 bytes*/
  /* *************************************************************************************** */
  /*    RSSI Access   -> RSSI read
               -> Next Pointer = 0x0000 -> end of the sequence
               -> Write Address @0xCA / 1 byte / Valid for all RF version at the moment
               -> Write data = 0x000000 -> Read data to replace this / provided to Packet Controller */
  RF_ICTRX_EM_BLE_WR(rssiptr,     0x0000);
  RF_ICTRX_EM_BLE_WR(rssiptr+0x2, 0xCA01);
  RF_ICTRX_EM_BLE_WR(rssiptr+0x4, 0x0000);
  RF_ICTRX_EM_BLE_WR(rssiptr+0x6, 0x0000);

};
///@} RF_ATLAS
