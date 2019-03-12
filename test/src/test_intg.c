/* Copyright (c) 2019 Cloudchip, Inc. All Rights Reserved.
 *
 * The information contained herein is property of Cloudchip, Inc.
 * Terms and conditions of usage are described in detail in CLOUDCHIP
 * STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information.
 * NO WARRANTY of ANY KIND is provided. This heading must NOT be removed
 * from the file.
 */

/*
 * INCLUDES
 ****************************************************************************************
 */
#include "test.h"

#if (TEST_INTEGRATION)
#include <string.h>
#include <stdio.h>

#include "ll.h"
#include "drvi_init.h"
#include "drvi_gpio.h"
#include "drvi_pwm.h"
#include "drvi_rtc.h"
#include "drvi_spi.h"
#include "drvi_wdt.h"
#include "drvi_wktm.h"
#include "rtc.h"
#include "tracer.h"
#include "project.h"
#include "test.h"
#include "test_aes.h"
#include "ppg_pah8002.h"

#ifdef TEST_INTG_RTC
    sprintf(cTmpBuf, "[%02d:%02d:%02d] ", time.hour, time.min, time.sec);
    strcat(buffer, cTmpBuf);
#endif
#ifdef TEST_INTG_SPI1
    sprintf(cTmpBuf, "[S1:%d]", count.spi1);
    strcat(buffer, cTmpBuf);
#endif
#ifdef TEST_INTG_SPI2
    sprintf(cTmpBuf, "[S2:%d]", count.spi2);
    strcat(buffer, cTmpBuf);
#endif
#ifdef TEST_INTG_I2C0
    sprintf(cTmpBuf, "[I0:%d]", count.i2c0);
    strcat(buffer, cTmpBuf);
#endif
#ifdef TEST_INTG_I2C1
    sprintf(cTmpBuf, "[I1:%d]", count.i2c1);
    strcat(buffer, cTmpBuf);
#endif
#ifdef TEST_INTG_GPIO
    sprintf(cTmpBuf, "[G:%d]", count.gpi);
    strcat(buffer, cTmpBuf);
#endif
#ifdef TEST_INTG_AES
    sprintf(cTmpBuf, "[A:%d]", count.aes);
    strcat(buffer, cTmpBuf);
#endif
#ifdef TEST_INTG_WDT
    sprintf(cTmpBuf, "[W:%d]", count.wdt);
    strcat(buffer, cTmpBuf);
#endif
#ifdef TEST_INTG_WKTM
    sprintf(cTmpBuf, "[T:%d]", count.wktm0);
    strcat(buffer, cTmpBuf);
#endif
#ifdef TEST_INTG_PWM
    sprintf(cTmpBuf, "[P:%d]", count.pwm0);
    strcat(buffer, cTmpBuf);
#endif
#ifdef TEST_INTG_UART
    sprintf(cTmpBuf, "[U:%d]", count.uart0_tx);
    strcat(buffer, cTmpBuf);
#endif

extern volatile uint32_t AES_INTR;
extern volatile uint32_t SPI1_M_INTR;
extern volatile uint32_t SPI2_M_INTR;
extern volatile uint32_t I2C0_M_INTR;
extern volatile uint32_t I2C1_M_INTR;
extern volatile uint32_t UART0_TXDM_INTR;

#define TEST_INTG_SPI1    1
#define TEST_INTG_SPI2    1
#define TEST_INTG_I2C0    1
#define TEST_INTG_I2C1    1
#define TEST_INTG_GPIO    1
#define TEST_INTG_AES     1
#define TEST_INTG_WDT     1
#define TEST_INTG_RTC     1
//#define TEST_INTG_WKTM    1
//#define TEST_INTG_PWM     1
#define TEST_INTG_UART    1

//record for each interface
struct module_exec_count
{
    uint32_t mem;
    uint32_t spi1;
    uint32_t spi2;
    uint32_t i2c0;
    uint32_t i2c1;
    uint32_t uart0_tx;
    uint32_t uart0_rx;
    uint32_t aes;
    uint32_t wdt;
    uint32_t eflash;
    uint32_t wktm0;
    uint32_t pwm0;
    uint32_t gpi;
};

struct module_exec_count count;

#ifdef TEST_INTG_RTC
static T_Rtc time = {0};
static uint32_t dwUartEn = 0;
#endif

extern void sys_InitMain(void);

#ifdef TEST_INTG_GPIO
void gpio_callback(void)
{
    count.gpi++;
}
#endif
#ifdef TEST_INTG_WDT
static void test_WdtHandler(void *event)
{
    count.wdt++;
}
#endif
#ifdef TEST_INTG_PWM
void pwm_callback(void *para)
{
    static int iPwmEn = 0;

    if (iPwmEn < 1000)
        drvi_pwmStop(PWM_0);
    else if (iPwmEn < 1200)
        drvi_pwmStart(PWM_0);
    else
        iPwmEn = 0;

    iPwmEn++;
    count.pwm0++;
}
#endif
#ifdef TEST_INTG_WKTM
void timer_callback(void *para)
{
    drvi_wktmCounterClear(WKTM_PORT_0);
    count.wktm0++;
}
#endif
#ifdef TEST_INTG_RTC
void intg_RtcHandler(void *para)
{
    time = drvi_RtcGetTime();
    dwUartEn = 1;
}
#endif

void intg_Logger(char *buffer)
{
    char cTmpBuf[16] = {0};

    memset(buffer, 0x00, 16);
#ifdef TEST_INTG_RTC
    sprintf(cTmpBuf, "[%02d:%02d:%02d] ", time.hour, time.min, time.sec);
    strcat(buffer, cTmpBuf);
#endif
#ifdef TEST_INTG_SPI1
    //TracerInfo("ACC X[%d]Y[%d]Z[%d]\r\n", acc_buf[0], acc_buf[4], acc_buf[8]);
    sprintf(cTmpBuf, "[S1:%d]", count.spi1);
    strcat(buffer, cTmpBuf);
#endif
#ifdef TEST_INTG_SPI2
    sprintf(cTmpBuf, "[S2:%d]", count.spi2);
    strcat(buffer, cTmpBuf);
#endif
#ifdef TEST_INTG_I2C0
    //TracerInfo("MAG X[%d]Y[%d]Z[%d]\r\n", mag_buf[0], mag_buf[2], mag_buf[4]);
    sprintf(cTmpBuf, "[I0:%d]", count.i2c0);
    strcat(buffer, cTmpBuf);
#endif
#ifdef TEST_INTG_I2C1
    sprintf(cTmpBuf, "[I1:%d]", count.i2c1);
    strcat(buffer, cTmpBuf);
#endif
#ifdef TEST_INTG_GPIO
    sprintf(cTmpBuf, "[G:%d]", count.gpi);
    strcat(buffer, cTmpBuf);
#endif
#ifdef TEST_INTG_AES
    sprintf(cTmpBuf, "[A:%d]", count.aes);
    strcat(buffer, cTmpBuf);
#endif
#ifdef TEST_INTG_WDT
    sprintf(cTmpBuf, "[W:%d]", count.wdt);
    strcat(buffer, cTmpBuf);
#endif
#ifdef TEST_INTG_WKTM
    sprintf(cTmpBuf, "[T:%d]", count.wktm0);
    strcat(buffer, cTmpBuf);
#endif
#ifdef TEST_INTG_PWM
    sprintf(cTmpBuf, "[P:%d]", count.pwm0);
    strcat(buffer, cTmpBuf);
#endif
#ifdef TEST_INTG_UART
    sprintf(cTmpBuf, "[U:%d]", count.uart0_tx);
    strcat(buffer, cTmpBuf);
#endif

    strcat(buffer, "\r\n");
}

/*
 * MAIN FUNCTION
 ****************************************************************************************
 */

int TEST_IntgInit(void)
{
    T_RtcCallback tRtcHandler;

#ifdef TEST_INTG_SPI1
    ACC_Init();
#endif
#ifdef TEST_INTG_SPI2
    oled_Init();
    oled_HardReset();
#endif
#ifdef TEST_INTG_I2C0
    MAG_Init();
#endif
#ifdef TEST_INTG_I2C1
    pah8002_twi_init();
#endif
#ifdef TEST_INTG_GPIO
    drvi_RequestIrq(GPIO_PIN_3, gpio_callback, IRQ_TYPE_EDGE_FALLING);
    drvi_EnableIrq(GPIO_PIN_3);
#endif
#ifdef TEST_INTG_AES
    drvi_aes_init(DRVI_AES_MODE_ECB);
#endif
#ifdef TEST_INTG_WDT
    drvi_WdtRegisterCallback(2000, test_WdtHandler);
    drvi_WdtEnable();
#endif
#ifdef TEST_INTG_RTC
    drvi_RtcInit();
    tRtcHandler.day = NULL;
    tRtcHandler.hour = NULL;
    tRtcHandler.min = NULL;
    tRtcHandler.sec = intg_RtcHandler;
    drvi_RtcRegisterCallback(tRtcHandler);
#endif
#ifdef TEST_INTG_WKTM
    drvi_wktmRegisterCallback(WKTM_PORT_0, timer_callback);
    drvi_wktmStart(WKTM_PORT_0);
    drvi_wktmCounterLoad(WKTM_PORT_0, 30000);
    NVIC_EnableIRQ(WKTM0_IRQn);
#endif
#ifdef TEST_INTG_PWM
    drvi_pwmRegisterCallback(PWM_0, pwm_callback);
    drvi_pwmStop(PWM_0);
    drvi_pwmLoadPrescaler(PWM_0, 0);
    drvi_pwmDuty(PWM_0, 50);
    drvi_pwmStart(PWM_0);
    NVIC_EnableIRQ(PWM0_IRQn);
#endif
#ifdef TEST_INTG_UART
    //UART0_TXDM_INTR = 1;
#endif
    return 0;
}

void TEST_Intg(void)
{
#ifdef TEST_INTG_SPI1
    uint8_t bAccBuf[6];
#endif
#ifdef TEST_INTG_I2C0
    uint8_t bMagBuf;
#endif
#ifdef TEST_INTG_GPIO
    uint32_t dwGpioLoop = 0;
#endif
#ifdef TEST_INTG_AES
    uint8_t bEncryptData[16] = {0};
    uint8_t bDecryptData[16] = {0};
#endif
#ifdef TEST_INTG_UART
    uint8_t bUartTxBuf[1024];
#endif

    //Must be first in main()
    sys_InitMain();

    //start interrupt handling
    GLOBAL_INT_START();

    drvi_initialize();

//    TracerInfo("== CC6801 Start ==\r\n");

    /*******************************/
    /****** Application Start ******/
    /*******************************/

    //TEST_Main();
    TEST_IntgInit();

    while(1)
    {
#ifdef TEST_INTG_SPI1
        if (!SPI1_M_INTR)
        {
            ACC_GetRawAccData((i16_t *)bAccBuf);
            count.spi1++;
        }
#endif
#ifdef TEST_INTG_SPI2
        if (!SPI2_M_INTR)
        {
            oled_DisplayOff();
            count.spi2++;
        }
#endif
#ifdef TEST_INTG_I2C0
        if (!I2C0_M_INTR)
        {
            AK09912_MAG_ReadReg(AK09912_MAG_HXL, &bMagBuf);
            count.i2c0++;
        }
#endif
#ifdef TEST_INTG_I2C1
        if (!I2C1_M_INTR)
        {
            pah8002_write_reg(0x7f, 0x00);
            count.i2c1++;
        }
#endif
#ifdef TEST_INTG_GPIO
        if (((dwGpioLoop >> 3) << 3) == dwGpioLoop)
        {
            drvi_GpioWrite(GPIO_PIN_2, 0);
            drvi_GpioWrite(GPIO_PIN_2, 1);
        }

        dwGpioLoop++;
#endif
#ifdef TEST_INTG_AES
        if (!AES_INTR)
        {
            drvi_aes_init(DRVI_AES_MODE_ECB);
            drvi_aes_encrypt((uint32_t)bDecryptData, (uint32_t)bEncryptData, 16);
            count.aes++;
        }
#endif
#ifdef TEST_INTG_UART
        if (dwUartEn)
        {
            //UART0_TXDM_INTR = 0;
            dwUartEn = 0;
            intg_Logger((char *)bUartTxBuf);
            TracerInfo("%s", bUartTxBuf);
            count.uart0_tx++;
        }
#endif
    }
}

#endif //TEST_INTEGRATION
