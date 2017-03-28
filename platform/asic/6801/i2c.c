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

/******************************************************************************
*  Filename:
*  ---------
*  i2c.c
*
*  Project:
*  --------
*  cc6801
*
*  Description:
*  ------------
*  This is I2C driver
*
*  Author:
*  -------
*  CloudChip
*
*===========================================================================/
*  20170206 PAT initial version
******************************************************************************/
#include "global.h"
#include "cc6801_reg.h"
#include "i2c.h"

volatile uint32_t I2C0_M_INTR = 0;
volatile uint32_t I2C1_M_INTR = 0;

/**@brief Generic function for handling I2C interrupt
 *
 * @param[in]  p_reg         Pointer to instance register structure.
 * @param[in]  instance_id   Index of instance.
 */
__STATIC_INLINE void cc_drv_i2c_int_handler(S_regI2C * p_reg, uint32_t instance_id)
{
    // clear interrupt
    p_reg->dma_done_intr = 1;
    //p_reg->ms_resync_done_intr = 1;
    p_reg->i2c_cd_err_intr = 1;
    p_reg->i2c_err_ack_intr = 1;
    p_reg->dma_done_intr = 1;
}


void I2C0_M_IRQHandler(void)
{
    cc_drv_i2c_int_handler(CC_I2C0, I2C0_INSTANCE_INDEX);
    I2C0_M_INTR = 1;
}

void I2C1_M_IRQHandler(void)
{
    wr(I2C1_ADDR_BASE+0, 0x00010007);   // clear interrupt
}

__STATIC_INLINE void cc_i2c_frequency_set(S_regI2C      * p_i2c,
                                           cc_i2c_frequency_t frequency)
{
    switch (frequency)
    {
        case CC_I2C_FREQ_100K:
          p_i2c->ms_prescaler = 232;
          p_i2c->cfg_i2c_mask = 3;
          break;
        case CC_I2C_FREQ_400K:
          p_i2c->ms_prescaler = 52;
          p_i2c->cfg_i2c_mask = 3;
          break;
        default:
          break;
    }
}

__STATIC_INLINE void cc_i2c_int_enable(S_regI2C * p_i2c, uint32_t int_mask)
{
    p_i2c->ms_resync_intr_en = 1;
    p_i2c->i2c_cd_err_intr_en = 1;
    p_i2c->i2c_err_ack_intr_en = 1;
    p_i2c->dma_done_intr_en = 1;
}


/**
 * @brief Function for transferring data.
 *
 * @note Transmission will be stopped when error or timeout occurs.
 *
 * @param[in] p_instance      I2C.
 * @param[in] address         Address of specific slave device (only 7 LSB).
 * @param[in] p_data          Pointer to a receive buffer.
 * @param[in] length          Number of bytes to be received.
 * @param[in] xfer_pending    After a specified number of bytes transmission will be
 *                            suspended (if xfer_pending is set) or stopped (if not)
 * @param[in] is_tx           Indicate transfer direction (true for master to slave transmission).
 *
 * @retval  CC_SUCCESS        If the procedure was successful.
 * @retval  CC_ERROR_BUSY     Driver is not ready for new transfer.
 * @retval  CC_ERROR_INTERNAL CC_I2C_EVENTS_ERROR or timeout has occured (only in blocking mode).
 */
static int i2c_transfer(cc_drv_i2c_t const * const p_instance,
                               uint8_t                     address,
                               uint8_t const             * p_data,
                               uint32_t                    length,
                               bool                        xfer_pending,
                               bool                        is_tx)
{
    if (is_tx)
    {
        p_instance->p_reg->dma_str_waddr = (uint32_t)p_data;
        p_instance->p_reg->wdata_byte_num = length;
    }
    else
    {
        p_instance->p_reg->dma_str_raddr = (uint32_t)p_data;
        p_instance->p_reg->rdata_byte_num = length;
    }

    p_instance->p_reg->dbus_burst = 0;
    p_instance->p_reg->dma_enable = 1;

    while(!I2C0_M_INTR);

    return CC_SUCCESS;
}

int cc_drv_i2c_init(cc_drv_i2c_t const * const  p_instance,
                            cc_drv_i2c_config_t const * p_config,
                            cc_drv_i2c_evt_handler_t    event_handler,
                            void *                       p_context)
{
    cc_i2c_frequency_set(p_instance->p_reg, p_config->frequency);

    p_instance->p_reg->ms_word_addr = 0;
    p_instance->p_reg->ms_slave_addr = 0x1F;
    p_instance->p_reg->ms_no_stop = 0;
    p_instance->p_reg->ms_addr_en = 0;
    p_instance->p_reg->ms_addr_16bit = 0;

		//TODO: I2C GPIO setting
    //NRF_GPIO->PIN_CNF[p_config->scl] = SCL_PIN_CONF;
    //NRF_GPIO->PIN_CNF[p_config->sda] = SDA_PIN_CONF;
    //nrf_i2c_pins_set(p_instance->p_reg, p_config->scl, p_config->sda);

    return CC_SUCCESS;
}


void cc_drv_i2c_uninit(cc_drv_i2c_t const * const p_instance)
{
}


void cc_drv_i2c_enable(cc_drv_i2c_t const * const p_instance)
{
    p_instance->p_reg->ms_resync_done_intr = 1;
    p_instance->p_reg->i2c_cd_err_intr = 1;
    p_instance->p_reg->i2c_err_ack_intr = 1;
    p_instance->p_reg->dma_done_intr = 1;

    cc_i2c_int_enable(p_instance->p_reg, 0x7);
}

int cc_drv_i2c_tx(cc_drv_i2c_t const * const p_instance,
                          uint8_t                     address,
                          uint8_t const *             p_data,
                          uint32_t                    length,
                          bool                        xfer_pending)
{
    return i2c_transfer(p_instance, address,
                        p_data, length,
                        xfer_pending, true);
}


int cc_drv_i2c_rx(cc_drv_i2c_t const * const p_instance,
                          uint8_t                     address,
                          uint8_t *                   p_data,
                          uint32_t                    length,
                          bool                        xfer_pending)
{
    return i2c_transfer(p_instance, address,
                        p_data, length,
                        xfer_pending, false);
}
