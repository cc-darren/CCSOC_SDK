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

#include "global.h"
#include "drvi_i2c.h"
#include "i2c_sensor.h"
#include "ak09912.h"


#define I2C0_INSTANCE_INDEX      0
#define I2C0_CONFIG_FREQUENCY    CC_I2C_FREQ_100K
#define I2C0_CONFIG_ADDRESS      AK09912_MAG_MEMS_I2C_ADDRESS

static volatile bool i2c_rx_done;  /**< Flag used to indicate that i2c instance completed the Rx transfer. */
static volatile bool i2c_tx_done;  /**< Flag used to indicate that i2c instance completed the Tx transfer. */


/**
 * @brief i2c events handler.
 */
void i2c_handler(T_I2cEvent const * p_event)
{

    switch(p_event->eType)
    {
        case DRVI_I2C_READ_DONE:
            i2c_rx_done = true;
            break;
        case DRVI_I2C_WRITE_DONE:
            i2c_tx_done = true;
            break;
        default:
            break;
    }
}

/**
 * @brief i2c initialization.
 */
void i2c_init (void)
{
    T_I2cDevice i2c_sensor_config = {
       .bBusNum          = I2C0_INSTANCE_INDEX,
       .bAddr            = I2C0_CONFIG_ADDRESS,
    };

    drvi_I2cDeviceRegister(&i2c_sensor_config);
}

void i2c_data_read(uint8_t reg_addr, uint8_t *reg_data, uint8_t cnt)
{
    //send i2c device address first
    i2c_tx_done = false;
    drvi_I2cWrite(I2C0_INSTANCE_INDEX, &reg_addr, sizeof(reg_addr));

    #if 0
    while(!i2c_tx_done)
    {
        __WFE();
    }
    i2c_tx_done = false;
    #endif
    //Read data from i2c
    i2c_rx_done = false;
    drvi_I2cRead(I2C0_INSTANCE_INDEX, reg_data, cnt);
    //APP_ERROR_CHECK(err_code);
    #if 0
    while(!i2c_rx_done)
    {
        __WFE();
    }
    i2c_rx_done = false;
    #endif
}

void i2c_data_write(uint8_t reg_addr, uint8_t *reg_data)
{
    uint8_t reg[2];
    reg[0] = reg_addr;
    reg[1] = *reg_data;

    i2c_tx_done = false;
    drvi_I2cWrite(I2C0_INSTANCE_INDEX, reg, sizeof(reg));
    #if 0
    while(!i2c_tx_done)
    {
        __WFE();
    }
    i2c_tx_done = false;
    #endif
}

