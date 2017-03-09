#include "stdint.h"
#include "addr_map.h"

//#define EFLASH_TEST
//#define TEST_I2C_WR
//#define TEST_RST_SPI0
//#define TEST_RST_SPI1
//#define TEST_RST_SPI2
//#define TEST_I2S_M_PORT
//#define TEST_PWM0
//#define TEST_PWM1
//#define TEST_UART_RX_IRT_BYTE0
//#define TEST_UART1_8BIT_DW_MODE
//#define TEST_UART2_8BIT_DW_MODE
//#define TEST_OTP_AES
//#define TEST_WDT
//#define TEST_GPIO
//#define TEST_DMIC
//#define TEST_DMU_PERF
//#define DHRYSTONE
//#define TEST_WDTRST
#define TEST_EF_DMA_WR
//#define TEST_EF_RD


extern void test_init(void);
extern void eFlash_test(void);
extern int test_i2c_wr(void);
extern int test_rst_spi0(void);
extern int test_rst_spi1(void);
extern int test_rst_spi2(void);
extern int test_i2s_m_port(void);
extern int test_pwm0(void);
extern int test_pwm1(void);
extern int test_uart_rx_irt_byte0(void);
extern int test_uart1_8bit_dw_mode(void);
extern int test_uart2_8bit_dw_mode(void);
extern int test_otp_aes(void);
extern int test_wdt(void);
extern int test_gpio(void);
extern int test_dmic(void);
extern int test_dmu_perf(void);
extern void dhry_main(void);
extern int test_wdtrst(void);
extern int test_eflash_dma_wr(void);
extern int test_eflash_rd(void);

void testbed(void)
{
    
#ifndef BYP_TEST_INIT
    test_init();
#endif

#ifdef EFLASH_TEST
    eFlash_test();
#endif

#ifdef TEST_I2C_WR
    test_i2c_wr();
#endif

#ifdef TEST_RST_SPI0
    test_rst_spi0();
#endif

#ifdef TEST_RST_SPI1
    test_rst_spi1();
#endif

#ifdef TEST_RST_SPI2
    test_rst_spi2();
#endif

#ifdef TEST_I2S_M_PORT
    test_i2s_m_port();
#endif

#ifdef TEST_PWM0
    test_pwm0();
#endif

#ifdef TEST_PWM1
    test_pwm1();
#endif

#ifdef TEST_UART_RX_IRT_BYTE0
    test_uart_rx_irt_byte0();
#endif

#ifdef TEST_UART1_8BIT_DW_MODE
    test_uart1_8bit_dw_mode();
#endif

#ifdef TEST_UART2_8BIT_DW_MODE
    test_uart2_8bit_dw_mode();
#endif

#ifdef TEST_OTP_AES
    test_otp_aes();
#endif

#ifdef TEST_WDT
    test_wdt();
#endif

#ifdef TEST_GPIO
    test_gpio();
#endif

#ifdef TEST_DMIC
    test_dmic();
#endif

#ifdef TEST_DMU_PERF
    test_dmu_perf();
#endif

#ifdef DHRYSTONE
    wr(CKGEN_CLKEN_REG, 0x0);   //disable all IPs clock
    dhry_main();
    wr(CKGEN_CLKEN_REG, 0x10);  //enable GPIO clock
    delayns(10);
    wr(CKGEN_SWRST_REG, 0x0);
    wr(CKGEN_SWRST_REG, 0x10);  //reset GPIO
    delayns(10);
#endif

#ifdef TEST_WDTRST
    test_wdtrst();
#endif

#ifdef TEST_EF_DMA_WR
    test_eflash_dma_wr();
#endif

#ifdef TEST_EF_RD
    test_eflash_rd();
#endif

    wr(GPIO_INTR_PRIO_REG, 0x1);    // notify sim to finish
}
