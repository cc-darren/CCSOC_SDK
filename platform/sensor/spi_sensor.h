/******************************************************************************
*  Copyright 2017, CloudChip, Inc.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of CloudChip, Inc. (C) 2017
******************************************************************************/

#ifndef SPI_SENSOR_H__
#define SPI_SENSOR_H__

#include <stdint.h>

void spi_init (uint8_t spi_id);
void spi_data_read(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data, uint8_t cnt, uint8_t spi_id);
void spi_data_write(uint8_t dev_addr, uint8_t reg_addr, uint8_t *reg_data, uint8_t cnt, uint8_t spi_id);


#endif


