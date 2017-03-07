/**
 ****************************************************************************************
 *
 * @file int.c
 *
 * @brief Interrupt controller driver
 *
 * Copyright (C) PATTAYA 2017-2018
 *
 *
 ****************************************************************************************
 */
#include "compiler.h"
#include "register.h"
#include "ll.h"
//#include "uart.h"


void I2C_ISR(void)
{
	  volatile unsigned int test;

		test = *((volatile unsigned int *)(BLE_BASE+0x04));
		test++;
}

/**
 ****************************************************************************************
 * @brief A680 ISR dispatch function.
 *
 * This function is called whenever one or more interrupt occurs
 *
 * @return none
 ****************************************************************************************
 */
void A680_IRQHandler(void)
{

    volatile unsigned int int_status;
    unsigned int    mask;
    //uint8_t         ch;

    //GLOBAL_INT_DISABLE();

    //get vectors
    int_status = *((volatile unsigned int *)(BASEADDR_INT+0x40));

    while(int_status)
    {
        mask = int_status & (~int_status+1);
        switch(mask)
        {
            case 0x00000001: break;
            case 0x00000002: break;
            case 0x00000004: I2C_ISR();     break;
            case 0x00000008: break;
            case 0x00000010: break;
            case 0x00000020: break;
            case 0x00000040: break;
            case 0x00000080: break;
            case 0x00000100: break;
            case 0x00000200: break;
            case 0x00000400: break;
            case 0x00000800: break;
            case 0x00001000: break;
            case 0x00002000: break;
            case 0x00004000: break;
            case 0x00008000: break;
            case 0x00010000: break;
            case 0x00020000: break;
            case 0x00040000: break;
            case 0x00080000: break;
            case 0x00100000: break;
            case 0x00200000: break;
            case 0x00400000: break;
            case 0x00800000: break;
            case 0x01000000: break;
            case 0x02000000: break;
            case 0x04000000: break;
            case 0x08000000: break;
            case 0x10000000: break;
            case 0x20000000: break;
            case 0x40000000: break;
            case 0x80000000: break;
            default: break;
        }

        //Write 1 clear interrupt
        *((volatile unsigned int *)(BASEADDR_INT+0x04)) = mask;

        int_status &= ~mask;
    }

    //GLOBAL_INT_RESTORE();v
}



/**
 ****************************************************************************************
 * @brief Interrupt controller initialization
 *
 * This function is called by arch_main
 *
 * @return none
 ****************************************************************************************
 */
void intc_init(void)
{
    unsigned int rd_data;

    *((volatile unsigned int *)(BASEADDR_INT+0x3C)) = 0xFFFFFFFF;        //disable all

    //rd_data = *((volatile unsigned int *)(BASEADDR_TIMER0+0x0C));       //read clear Timer0 interrupt



    rd_data = *((volatile unsigned int *)(BASEADDR_INT+0x40));          //read clear interrupt status
    *((volatile unsigned int *)(BASEADDR_INT+0x40)) = rd_data;      //write 1 clear all interrupt

    //disallow ARM peripheral IRQ
    *((volatile unsigned int *)(NVIC_ISER0)) = 0x00;

    //allow interrupt to CPU
    //__enable_irq();

    //enable INT for peripheral
    //*((volatile unsigned int *)(BASEADDR_INT+0)) = 0x1FFDFFEE;        //only enable TIMER, GPIO1 and BLE
}






