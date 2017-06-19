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

#define I2C_ERR_NONE        0x00
#define I2C_ERR_NO_ACK      0x01
#define I2C_ERR_COLLIDED    0x02

volatile uint32_t I2C0_M_INTR = 0;
volatile uint32_t I2C1_M_INTR = 0;

static int msg_err = 0;

void I2C0_M_IRQHandler(void)
{
    regI2C0->bf.dma_done_intr = 1;

    //regI2C0->ms_resync_done_intr = 1;
    if (regI2C0->bf.i2c_cd_err_intr)
    {
        msg_err |= I2C_ERR_COLLIDED;
        regI2C0->bf.i2c_cd_err_intr = 1;
    }

    if (regI2C0->bf.i2c_err_ack_intr)
    {
        msg_err |= I2C_ERR_NO_ACK;
        regI2C0->bf.i2c_err_ack_intr = 1;
    }

    if (regI2C0->bf.dma_done_intr)
        regI2C0->bf.dma_done_intr = 1;

    I2C0_M_INTR = 1;
}

void I2C1_M_IRQHandler(void)
{
    regI2C1->bf.dma_done_intr = 1;

    //regI2C1->ms_resync_done_intr = 1;
    if (regI2C1->bf.i2c_cd_err_intr)
    {
        msg_err |= I2C_ERR_COLLIDED;
        regI2C1->bf.i2c_cd_err_intr = 1;
    }

    if (regI2C1->bf.i2c_err_ack_intr)
    {
        msg_err |= I2C_ERR_NO_ACK;
        regI2C1->bf.i2c_err_ack_intr = 1;
    }

    if (regI2C1->bf.dma_done_intr)
        regI2C1->bf.dma_done_intr = 1;

    I2C1_M_INTR = 1;
}

__STATIC_INLINE void cc_i2c_frequency_set(U_regI2C      * p_i2c,
                                           cc_i2c_frequency_t frequency)
{
    switch (frequency)
    {
        case CC_I2C_FREQ_100K:
          p_i2c->bf.ms_prescaler = 154;
          p_i2c->bf.cfg_i2c_mask = 3;
          break;
        case CC_I2C_FREQ_400K:
          p_i2c->bf.ms_prescaler = 30;
          p_i2c->bf.cfg_i2c_mask = 3;
          break;
        default:
          break;
    }
}

void cc_i2c_int_disable(cc_drv_i2c_t const * const p_instance)
{
    //p_instance->bf.ms_resync_intr_en = 0;
    p_instance->p_reg->bf.i2c_cd_err_intr_en = 0;
    p_instance->p_reg->bf.i2c_err_ack_intr_en = 0;
    p_instance->p_reg->bf.dma_done_intr_en = 0;
}

void cc_i2c_int_enable(cc_drv_i2c_t const * const p_instance)
{
    //p_instance->p_reg->bf.ms_resync_intr_en = 1;
    p_instance->p_reg->bf.i2c_cd_err_intr_en = 1;
    p_instance->p_reg->bf.i2c_err_ack_intr_en = 1;
    p_instance->p_reg->bf.dma_done_intr_en = 1;

    //p_instance->p_reg->bf.ms_resync_done_intr = 1;
    p_instance->p_reg->bf.i2c_cd_err_intr = 1;
    p_instance->p_reg->bf.i2c_err_ack_intr = 1;
    p_instance->p_reg->bf.dma_done_intr = 1;
}

static int i2c0_xfer(U_regI2C * p_i2c)
{
    msg_err = I2C_ERR_NONE;

    NVIC_EnableIRQ(I2C0_M_IRQn);

    p_i2c->bf.dma_enable = 1;

    while(!I2C0_M_INTR);
    I2C0_M_INTR = 0;

    NVIC_DisableIRQ(I2C0_M_IRQn);

    if (msg_err == I2C_ERR_NONE)
        return CC_SUCCESS;

    //Error occurs, reset I2C bus
    if ((msg_err == I2C_ERR_COLLIDED) || (msg_err == I2C_ERR_NO_ACK))
        return CC_ERROR_INTERNAL;

    return CC_ERROR_BUSY;
}

static int i2c1_xfer(U_regI2C * p_i2c)
{
    msg_err = I2C_ERR_NONE;

    NVIC_EnableIRQ(I2C1_M_IRQn);

    p_i2c->bf.dma_enable = 1;

    while(!I2C1_M_INTR);
    I2C1_M_INTR = 0;

    NVIC_DisableIRQ(I2C1_M_IRQn);

    if (msg_err == I2C_ERR_NONE)
        return CC_SUCCESS;

    //Error occurs, reset I2C bus
    if ((msg_err == I2C_ERR_COLLIDED) || (msg_err == I2C_ERR_NO_ACK))
        return CC_ERROR_INTERNAL;

    return CC_ERROR_BUSY;
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
                               uint8_t const             * p_data,
                               uint32_t                    length,
                               bool                        xfer_pending,
                               bool                        is_tx)
{
    U_regI2C *reg = p_instance->p_reg;
    int  intance = p_instance->instance_id;
    int  (*i2c_xfer)(U_regI2C * p_i2c);

    switch (intance)
    {
        case 0:
            i2c_xfer = i2c0_xfer;
            break;
        case 1:
            i2c_xfer = i2c1_xfer;
            break;
        default:
            break;
    }

    if (is_tx)
    {
        p_instance->p_reg->bf.dma_str_raddr = (uint32_t)p_data;
        p_instance->p_reg->bf.wdata_byte_num = length-1;
        p_instance->p_reg->bf.rdata_byte_num = length-1;
        p_instance->p_reg->bf.op_mode = 0;
    }
    else
    {
        p_instance->p_reg->bf.dma_str_waddr = (uint32_t)p_data;
        p_instance->p_reg->bf.wdata_byte_num = length-1;
        p_instance->p_reg->bf.rdata_byte_num = length-1;
        p_instance->p_reg->bf.op_mode = 1;
    }

    p_instance->p_reg->bf.dbus_burst = 0;

    return i2c_xfer(reg);
}

int cc_drv_i2c_init(cc_drv_i2c_t const * const  p_instance,
                            cc_drv_i2c_config_t const * p_config,
                            cc_drv_i2c_evt_handler_t    event_handler,
                            void *                       p_context)
{
    cc_i2c_frequency_set(p_instance->p_reg, p_config->frequency);

    p_instance->p_reg->bf.cfg_core_select = 1;

    p_instance->p_reg->bf.ms_slave_addr = p_config->address;
    p_instance->p_reg->bf.ms_word_addr = 0;
    p_instance->p_reg->bf.ms_no_stop = 0;
    p_instance->p_reg->bf.ms_addr_en = 0;
    p_instance->p_reg->bf.ms_addr_16bit = 0;

    cc_i2c_int_enable(p_instance);

    return CC_SUCCESS;
}


void cc_drv_i2c_uninit(cc_drv_i2c_t const * const p_instance)
{
}

int cc_drv_i2c_tx(cc_drv_i2c_t const * const p_instance,
                          uint8_t const *             p_data,
                          uint32_t                    length,
                          bool                        xfer_pending)
{
    return i2c_transfer(p_instance,
                        p_data, length,
                        xfer_pending, true);
}


int cc_drv_i2c_rx(cc_drv_i2c_t const * const p_instance,
                          uint8_t *                   p_data,
                          uint32_t                    length,
                          bool                        xfer_pending)
{
    return i2c_transfer(p_instance,
                        p_data, length,
                        xfer_pending, false);
}
