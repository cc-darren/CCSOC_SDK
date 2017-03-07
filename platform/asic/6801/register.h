#ifndef REGISTER_H
/**
 ****************************************************************************************
 *
 * @file register.h
 *
 * @brief Include all A680 register define
 *
 * Copyright (C) PATTAYA 2016-2018
 *
 *
 ****************************************************************************************
 */

#define FPGA_ARRY_BASE          0x40010000
#define BLE_BASE                0x40020000
#define AHB_REG_BASE            0x40040000
#define BASEADDR_GPIO           0x50500000
#define BASEADDR_TIMER0         0x50600000
#define BASEADDR_INT            0x40000000
#define NVIC_ISER0              0xE000E100
#define VTOR_REG                0xE000ED08

// FPGA
#define FPGA_FIFO               (FPGA_ARRY_BASE + 0x404)
#define FPGA_CTRL               (FPGA_ARRY_BASE + 0x38)
#define FPGA_STATUS             (FPGA_ARRY_BASE + 0x3C)


#endif // REGISTER_H
