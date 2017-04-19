/**
 ****************************************************************************************
 *
 * @file drvi_spi.h
 *
 * @brief Head file of drvi_spi.c
 *
 * Copyright (C) CloudChip 2017-2019
 *
 *
 ****************************************************************************************
 */
#ifndef _DRVI_SPI_H_
#define _DRVI_SPI_H_
#include "spim.h"


#define drvi_spi_init(a,b,c)                  cc_drv_spi_init(a,b,c)
#define drvi_spi_write(a,b,c)                 cc_drv_spi_write(a,b,c)
#define drvi_spi_read(a,b,c)                  cc_drv_spi_read(a,b,c)
#define drvi_spi_write_then_read(a,b,c,d,e)   cc_drv_spi_write_then_read(a,b,c,d,e)

















#endif //_DRVI_SPI_H_


