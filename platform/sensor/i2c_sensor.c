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
#include "i2c.h"
#include "i2c_sensor.h"
#include "ak09912.h"


#define I2C0_INSTANCE_INDEX      0
#define I2C0_CONFIG_FREQUENCY    CC_I2C_FREQ_100K
#define I2C0_CONFIG_ADDRESS      AK09912_MAG_MEMS_I2C_ADDRESS

static const cc_drv_i2c_t m_i2c_sensor = CC_DRV_I2C_INSTANCE(0);
static volatile bool i2c_rx_done;  /**< Flag used to indicate that i2c instance completed the Rx transfer. */
static volatile bool i2c_tx_done;  /**< Flag used to indicate that i2c instance completed the Tx transfer. */


/**
 * @brief i2c events handler.
 */
void i2c_handler(cc_drv_i2c_evt_t const * p_event, void * p_context)
{

    switch(p_event->type)
    {
        case CC_DRV_I2C_RX_DONE:
            i2c_rx_done = true;
            break;
        case CC_DRV_I2C_TX_DONE:
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
    const cc_drv_i2c_config_t i2c_sensor_config = {
       .address            = I2C0_CONFIG_ADDRESS,
       .frequency          = I2C0_CONFIG_FREQUENCY,
    };

    cc_drv_i2c_init(&m_i2c_sensor, &i2c_sensor_config, i2c_handler, NULL);
}

void i2c_data_read(uint8_t reg_addr, uint8_t *reg_data, uint8_t cnt)
{
    //send i2c device address first
    i2c_tx_done = false;
    cc_drv_i2c_tx(&m_i2c_sensor, &reg_addr, sizeof(reg_addr), true);

    #if 0
    while(!i2c_tx_done)
    {
        __WFE();
    }
    i2c_tx_done = false;
    #endif
    //Read data from i2c
    i2c_rx_done = false;
    cc_drv_i2c_rx(&m_i2c_sensor, reg_data, cnt, false);
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
    cc_drv_i2c_tx(&m_i2c_sensor, reg, sizeof(reg), false);
    #if 0
    while(!i2c_tx_done)
    {
        __WFE();
    }
    i2c_tx_done = false;
    #endif
}

