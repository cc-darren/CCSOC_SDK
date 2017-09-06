

#ifndef I2C_SENSOR_H__
#define I2C_SENSOR_H__

#include <stdint.h>

void i2c_init (void);
void i2c_data_read(uint8_t reg_addr, uint8_t *reg_data, uint8_t cnt);
void i2c_data_write(uint8_t reg_addr, uint8_t *reg_data);


#endif


