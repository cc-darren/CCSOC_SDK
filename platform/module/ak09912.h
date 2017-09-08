/*******************************************************************************
* File Name          : AK09912_MAG_driver.h
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __AK09912_MAG_DRIVER__H
#define __AK09912_MAG_DRIVER__H

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
#ifndef _MAG_SHARE_TYPE_
#define _MAG_SHARE_TYPE_
#define MAG_Init()                              AK09912_MAG_Init()
#define MAG_GetMagRaw(buf)                      AK09912_MAG_GetMagRaw(buf)
#define MAG_GetMagRawBurst(buf, buf_size)       AK09912_MAG_GetMagRawBurst(buf, buf_size)
#endif //_MAG_SHARE_TYPE_
//these could change accordingly with the architecture

#ifndef __ARCHDEP__TYPES
#define __ARCHDEP__TYPES
typedef unsigned char u8_t;
typedef unsigned short int u16_t;
typedef short int i16_t;
typedef signed char i8_t;
#endif /*__ARCHDEP__TYPES*/

typedef u8_t AK09912_MAG_Int1PinConf_t;
typedef u8_t AK09912_MAG_Int2PinConf_t;
typedef u8_t AK09912_MAG_Int1Conf_t;
typedef u8_t AK09912_MAG_Axis_t;

/* Exported common structure --------------------------------------------------------*/

#ifndef __SHARED__TYPES
#define __SHARED__TYPES

typedef enum {
  MEMS_SUCCESS				=		0x01,
  MEMS_ERROR				=		0x00	
} status_t;

typedef enum {
  MEMS_ENABLE				=		0x01,
  MEMS_DISABLE				=		0x00	
} State_t;

typedef u8_t IntPinConf_t;
typedef u8_t Axis_t;
typedef u8_t IntConf_t;

#endif /*__SHARED__TYPES*/

#ifndef __AK09912__SHARED
#define __AK09912__SHARED

typedef struct {
  i16_t AXIS_X;
  i16_t AXIS_Y;
  i16_t AXIS_Z;
} AxesRaw_t;

typedef struct {
  i8_t AXIS_X;
  i8_t AXIS_Y;
  i8_t AXIS_Z;
} AxesAdj_t;

#endif


typedef enum { 
	AK09912_MAG_NO_OVERFLOW =	0x00,
	AK09912_MAG_OVERFLOW 	=	0x04
} AK09912_MAG_HOFL_t;

typedef enum { 
	AK09912_MAG_TEMP_EN_DISABLE =		 0x00,
	AK09912_MAG_TEMP_EN_ENABLE 	=	 0x80
} AK09912_MAG_TEMP_EN_t;

typedef enum { 
	AK09912_MAG_NSF_DISABLE =	 0x00,
	AK09912_MAG_NSF_LOW =		 0x20,
	AK09912_MAG_NSF_MIDDLE =	 0x40,
	AK09912_MAG_NSF_HIGH= 		 0x60
} AK09912_MAG_NSF_t;

typedef enum { 
	AK09912_MAG_POWER_DOWN  =	 0x00,
	AK09912_MAG_SINGLE      =	 0x01,
	AK09912_MAG_DO_10_Hz 	=	 0x02,
	AK09912_MAG_DO_20_hZ 	=	 0x04,
	AK09912_MAG_DO_50_Hz 	=	 0x06,
	AK09912_MAG_DO_100_Hz 	=	 0x08,
	AK09912_MAG_EXT_TRIG 	=	 0x0A,
	AK09912_MAG_SELF_TEST   =	 0x10,
	AK09912_MAG_ROM_ACCESS  =	 0x1F
} AK09912_MAG_MODE_t;

typedef enum { 
	AK09912_MAG_SOFT_RST_DISABLE =		 0x00,
	AK09912_MAG_SOFT_RST_ENABLE =		 0x01
} AK09912_MAG_SOFT_RST_t;

typedef enum { 
	AK09912_MAG_I2C_ENALBE =	 0x00,
	AK09912_MAG_I2C_DISABLE =	 0x1B
} AK09912_MAG_I2CDIS_t;

typedef enum { 
	AK09912_MAG_I2C_HS_DISABLE =	 0x00,
	AK09912_MAG_I2C_HS_ENABLE =	 0x80
} AK09912_MAG_I2CHS_EN_t;


typedef enum { 
	AK09912_MAG_DOR_DRDY_DEFAULT 	=  	 0x00, 
	AK09912_MAG_DRDY    =	 0x01,
	AK09912_MAG_DOR     =	 0x02
} AK09912_MAG_DOR_DRDY_t;


/* Registers Name ------------------------------------------------------------------------*/
#define     AK09912_MAG_WHO_AM_I    0x04
#define     AKM_COMPANY_ID    0x48

#define 	AK09912_AKM_ID      0x00
#define 	AK09912_MAG_ID      0x01
#define 	AK09912_MAG_ST1     0x10
#define 	AK09912_MAG_HXL		0x11
#define 	AK09912_MAG_HXH		0x12
#define 	AK09912_MAG_HYL		0x13
#define 	AK09912_MAG_HYH		0x14
#define 	AK09912_MAG_HZL		0x15
#define 	AK09912_MAG_HZH		0x16
#define 	AK09912_MAG_TMPS    0x17
#define 	AK09912_MAG_ST2     0x18
#define 	AK09912_MAG_CNTL1   0x30
#define 	AK09912_MAG_CNTL2   0x31
#define 	AK09912_MAG_CNTL3   0x32
#define 	AK09912_MAG_I2CDIS  0x36
#define 	AK09912_MAG_ASAX    0x60
#define 	AK09912_MAG_ASAY    0x61
#define 	AK09912_MAG_ASAZ    0x62

/* Exported macro ------------------------------------------------------------*/

#ifndef __SHARED__MACROS

#define __SHARED__MACROS
#define ValBit(VAR,Place)         (VAR & (1<<Place))
#define BIT(x) ( (x) )

#endif /*__SHARED__MACROS*/

/* Exported constants --------------------------------------------------------*/

#ifndef __SHARED__CONSTANTS
#define __SHARED__CONSTANTS

#define MEMS_SET                                        0x01
#define MEMS_RESET                                      0x00

#endif /*__SHARED__CONSTANTS*/


#define AKM_RST_MASK (1<<AKM_RST_PIN)
#define AKM_RST_INV_MASK AKM_RST_MASK

#define AKM_RST_CONFIGURE(akm_rst_mask) do { uint32_t pin;                  \
                                  for (pin = 0; pin < 32; pin++) \
                                      if ( (akm_rst_mask) & (1 << pin) )   \
                                          nrf_gpio_cfg_output(pin); } while (0)

#define AKM_RST_OFF(akm_rst_mask) do {  NRF_GPIO->OUTSET = (akm_rst_mask) & (AKM_RST_MASK & AKM_RST_INV_MASK); \
                            NRF_GPIO->OUTCLR = (akm_rst_mask) & (AKM_RST_MASK & ~AKM_RST_INV_MASK); } while (0)

#define AKM_RST_ON(akm_rst_mask) do {  NRF_GPIO->OUTCLR = (akm_rst_mask) & (AKM_RST_MASK & AKM_RST_INV_MASK); \
                           NRF_GPIO->OUTSET = (akm_rst_mask) & (AKM_RST_MASK & ~AKM_RST_INV_MASK); } while (0)

/************** I2C Address *****************/

#define AK09912_MAG_MEMS_I2C_ADDRESS         0x0C

/* Exported functions --------------------------------------------------------*/

/**********Sensor Configuration Functions***********/
status_t AK09912_MAG_SetMode(AK09912_MAG_MODE_t ov);
status_t AK09912_MAG_TemperatureEN(AK09912_MAG_TEMP_EN_t ov);
status_t AK09912_MAG_GetST1Stat(u8_t *value);
status_t AK09912_MAG_GetST2Stat(u8_t *value);
status_t AK09912_MAG_MeasOverFlow(AK09912_MAG_HOFL_t *value);
status_t AK09912_MAG_MeasOverRun(AK09912_MAG_DOR_DRDY_t *value);
status_t AK09912_MAG_Data_Ready(AK09912_MAG_DOR_DRDY_t *value);

/***************Utility Functions****************/
status_t AK09912_MAG_SoftReset(AK09912_MAG_SOFT_RST_t ov);
status_t AK09912_MAG_I2CDisable(AK09912_MAG_I2CDIS_t ov);
status_t AK09912_MAG_I2CHS_En(AK09912_MAG_I2CHS_EN_t ov);

/****************Reading Functions*****************/
status_t AK09912_MAG_GetMagRaw(AxesRaw_t* buff);
status_t AK09912_MAG_GetMagRawBurst(u8_t* buff, u8_t buffer_size);
status_t AK09912_MAG_GetTemperatureRaw(u8_t* buff);

/*********************Generic*********************/
u8_t AK09912_MAG_ReadReg(u8_t Reg, u8_t* Data);
u8_t AK09912_MAG_WriteReg(u8_t Reg, u8_t Data);

status_t AK09912_MAG_Init(void);
status_t AK09912_MAG_SLEEP(void);


#endif /* __AK09912_MAG_H */

/******************* (C) COPYRIGHT 2013 STMicroelectronics *****END OF FILE****/




