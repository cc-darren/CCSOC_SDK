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

#ifndef _PPG_PAH8002_H_
#define _PPG_PAH8002_H_

#include <stdint.h>

void pah8002_twi_init (void);
void pah8002_twi_deinit(void);

// return 0 if the function is successful.
uint8_t pah8002_write_reg(uint8_t addr, uint8_t data);
uint8_t pah8002_read_reg(uint8_t addr, uint8_t *data);
uint8_t pah8002_burst_read_reg(uint8_t addr, uint8_t *data, uint32_t rx_size);

#endif
