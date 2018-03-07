
#include "ll.h"
#include "lowpower.h"
#include "drvi_lowpower.h"
#include "drvi_clock.h"


extern E_ClockSupported g_dwCurrentClock;

//uint32_t dwPDNCmdCnt;
//uint32_t dwDR0CmdCnt;
//uint32_t dwDR1CmdCnt;
//uint32_t dwDR2CmdCnt;

struct {
    uint32_t dwPin;
    uint32_t dwIF;
    BOOL bIsGetData;
    uint32_t * pBuf;
} WakeupPin[3];

typedef struct _DataRamCmd_t{
    uint32_t * pStartAddr;
    uint32_t dwCmdCnt;
} DataRamCmd_t;


IN_RET_RAM_BEGIN
uint32_t g_dwPeripheralState;
IN_RET_RAM_END



uint32_t cc6801_LowPower_SetWakeupSource(Lowpower_WakeUp_t SetID,uint32_t dwPinNum,BOOL bIsGetData,uint32_t * pBuf)
{
    if ((SetID>WAKEUP_DR2)||(dwPinNum>11)) {
        return FAIL;
    }
    
    //pin number is limited in 0~11
    WakeupPin[SetID].dwPin = dwPinNum;
    WakeupPin[SetID].bIsGetData = bIsGetData;
    WakeupPin[SetID].pBuf = pBuf;
    /*
    switch (dwPinNum)
    {
    #if (ACC_INT_PIN!=PIN_NULL)
    case ACC_INT_PIN:
        WakeupParameter[SetID].dwIF = ACC_IF;
        break;
    #endif
    #if (MAG_INT_PIN!=PIN_NULL)
    case MAG_INT_PIN:
        WakeupParameter[SetID].dwIF = MAG_IF;
        break;
    #endif
    #if (PPG_INT_PIN!=PIN_NULL)
    case PPG_INT_PIN:
        WakeupParameter[SetID].dwIF = PPG_IF;
        break;
    #endif
    #if (GYR_INT_PIN!=PIN_NULL)
    case GYR_INT_PIN:
        WakeupParameter[SetID].dwIF = GYR_IF;
        break;
    #endif
    default:
        WakeupParameter[SetID].dwIF = IF_NULL;
        break;
    }
    */
    return SUCCESS;
}

IN_RET_RAM_BEGIN
void EnterAndLeaveWFI(void)
{
    __wfi();
    drvi_ClockSysClkAdjust(g_dwCurrentClock);
}
IN_RET_RAM_END

void cc6801_LowPower_Sleep(Lowpower_PowerMode_t lpMode)
{
    DataRamCmd_t tPRDList;
    DataRamCmd_t tDrList[3];
IN_RET_RAM_BEGIN
    DataRamCmd_t tWakeupList;
    uint32_t mlpCmdList[256];
IN_RET_RAM_END


    uint32_t tmpdata,tmp_SCU_CLKCFG1_REG;
    

    //0x40002200, sleep mode, clock source and mode selection
    

    tmpdata |= (HS_CONFIG_ClrIntr|HS_CONFIG_RingOsc_Dis|HS_CONFIG_32KOsc_En|HS_CONFIG_32KOsc_Sel);
    wr(HS_CONFIG_REG,tmpdata);

    
    //0x40000014
    setbit(SCU_CLKCFG1_REG,SCU_CLKCFG1_REG_HSGatedClockEn);

    //0x40002200, sleep mode, clock source and mode selection
    if (lpMode == PWRMODE_NORMAL)
    {
        __wfi();
        return;
    }
    else
    {
        tmpdata = lpMode;
    }
    
    tmpdata |= (HS_CONFIG_ClrIntr|HS_CONFIG_RingOsc_Dis|HS_CONFIG_32KOsc_En|HS_CONFIG_32KOsc_Sel);

    wr(HS_CONFIG_REG,tmpdata);



    //must on before enter sleep : GPIO/SPI0/SPI1/SPI2
    cc6801_LowPower_PeripheralOn(PERIPHERAL_GPIO|PERIPHERAL_SPI0|PERIPHERAL_SPI1|PERIPHERAL_SPI2);

/*
    rd(SCU_CLKCFG1_REG,tmp_SCU_CLKCFG1_REG);
    dwPDNCmdCnt = 0;
    mlpCmdList[dwPDNCmdCnt] = SCU_CLKCFG1_REG;
    dwPDNCmdCnt++;
    mlpCmdList[dwPDNCmdCnt] = tmp_SCU_CLKCFG1_REG&(~0x00000001);
*/
//    HS_setHSCmd(Cmdset,CKGEN_SWRST_REG,tmpSWRST);
//    HS_setHSCmd(Cmdset,CKGEN_CLKEN_REG,tmpClkEN);
//    wr(HS_PSO_PDN_REG,(tmpPSO|0x80600000)); //0x80000000 this will be dones after all command. set bit31 before enter sleep and HS will clear this bit automatically
//                                            //0x00600000 set CPU to power off in sleep period
    
    
    
    
    GLOBAL_INT_STOP();
    //notify HS to act
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;

    EnterAndLeaveWFI();
    
    cc6801_ClockSysClkAdjust(g_dwCurrentClock);
    
    GLOBAL_INT_START();
    //for PWRMODE_DEEPSLEEP
    //    can be waked up by given interrupts
    //    wake up in SW reset mode
    //    the code below this part won't be executedd
    //    FW will start from bootrom when wake up
    //
    //for PWRMODE_HIBERNATE
    //    only reset pin can wake up device
    //    wakeup up in reset mode 
    //

    //notify HS to end
    //Move clear bit into wakeup function, so that wakeup setting can be done once
    //SCB->SCR &= (~SCB_SCR_SLEEPDEEP_Msk);

    // ----------------------------
    // wait HS back to normal state
    do  {
        delayns(10);
        rd(HS_STS_REG,tmpdata);
    } while((tmpdata&0x0000000F) != HS_STATE_NORMAL);

    //0x40000020
    //set clear intr bit
    setbit(HS_CONFIG_REG,HS_CONFIG_ClrIntr);

    //0x40000014
    regSCU->bf.hsGatedClk    = 0; //disable Hardware sequencer, HS related register must be filled before HS clock off
}



#define CC6801_PERIPHERAL_ON(module_name) { if (dwPeripheral & PERIPHERAL_##module_name## )    \
                                               { *dwTmpPSO &= (~ ISO_PSO_##module_name## );     \
                                                 *dwTmpClkEN |= CLKEN_##module_name## ;         \
                                                 *dwTmpSWRST |= SWRST_##module_name## ; }       \
                                          }
void GetPeripheralOnValue(uint32_t dwPeripheral,uint32_t * dwTmpPSO,uint32_t * dwTmpClkEN,uint32_t * dwTmpSWRST)
{
    CC6801_PERIPHERAL_ON(WDT);
    CC6801_PERIPHERAL_ON(RTC);
    CC6801_PERIPHERAL_ON(PWM0);
    CC6801_PERIPHERAL_ON(PWM1);
    CC6801_PERIPHERAL_ON(WKT0);
    CC6801_PERIPHERAL_ON(WKT1);
    CC6801_PERIPHERAL_ON(SPI0);
    CC6801_PERIPHERAL_ON(SPI1);
    CC6801_PERIPHERAL_ON(SPI2);
    CC6801_PERIPHERAL_ON(UART0);
    CC6801_PERIPHERAL_ON(UART1);
    CC6801_PERIPHERAL_ON(UART2);
    CC6801_PERIPHERAL_ON(I2C0);
    CC6801_PERIPHERAL_ON(I2C1);
    CC6801_PERIPHERAL_ON(I2S);
    CC6801_PERIPHERAL_ON(DMIC);
    CC6801_PERIPHERAL_ON(BLE);
    CC6801_PERIPHERAL_ON(CCU);
    CC6801_PERIPHERAL_ON(AES);
    CC6801_PERIPHERAL_ON(DMU);
    CC6801_PERIPHERAL_ON(GPIO);
    CC6801_PERIPHERAL_ON(TESTLOGIC);
    CC6801_PERIPHERAL_ON(BLEPHY);
}

void cc6801_LowPower_PeripheralOn(uint32_t dwPeripheral)
{
    uint32_t dwPSO,dwClkEN,dwSWRST;

    //update pheripheral state
    g_dwPeripheralState |= dwPeripheral;



    rd(SCU_PSOCTRL_REG,dwPSO);
    rd(CKGEN_CLKEN_REG,dwClkEN);
    rd(CKGEN_SWRST_REG,dwSWRST);

    GetPeripheralOnValue(dwPeripheral,&dwPSO,&dwClkEN,&dwSWRST);

    wr(CKGEN_CLKEN_REG,dwClkEN);
    wr(SCU_PSOCTRL_REG,dwPSO  );
    wr(SCU_ISOCTRL_REG,dwPSO  );
    wr(CKGEN_SWRST_REG,dwSWRST);
}


#define CC6801_PERIPHERAL_OFF(module_name) { if (dwPeripheral & PERIPHERAL_##module_name## )    \
                                                { *dwTmpPSO   |= ISO_PSO_##module_name## ;       \
                                                  *dwTmpClkEN &= (~CLKEN_##module_name##) ;      \
                                                  *dwTmpSWRST &= (~SWRST_##module_name##) ; }    \
                                           }
void GetPeripheralOFFValue(uint32_t dwPeripheral,uint32_t * dwTmpPSO,uint32_t * dwTmpClkEN,uint32_t * dwTmpSWRST)
{
    CC6801_PERIPHERAL_OFF(WDT);
    CC6801_PERIPHERAL_OFF(RTC);
    CC6801_PERIPHERAL_OFF(PWM0);
    CC6801_PERIPHERAL_OFF(PWM1);
    CC6801_PERIPHERAL_OFF(WKT0);
    CC6801_PERIPHERAL_OFF(WKT1);
    CC6801_PERIPHERAL_OFF(SPI0);
    CC6801_PERIPHERAL_OFF(SPI1);
    CC6801_PERIPHERAL_OFF(SPI2);
    CC6801_PERIPHERAL_OFF(UART0);
    CC6801_PERIPHERAL_OFF(UART1);
    CC6801_PERIPHERAL_OFF(UART2);
    CC6801_PERIPHERAL_OFF(I2C0);
    CC6801_PERIPHERAL_OFF(I2C1);
    CC6801_PERIPHERAL_OFF(I2S);
    CC6801_PERIPHERAL_OFF(DMIC);
    CC6801_PERIPHERAL_OFF(BLE);
    CC6801_PERIPHERAL_OFF(CCU);
    CC6801_PERIPHERAL_OFF(AES);
    CC6801_PERIPHERAL_OFF(DMU);
    CC6801_PERIPHERAL_OFF(GPIO);
    CC6801_PERIPHERAL_OFF(TESTLOGIC);
    CC6801_PERIPHERAL_OFF(BLEPHY);
}
void cc6801_LowPower_PeripheralOff(uint32_t dwPeripheral)
{
    uint32_t dwPSO,dwClkEN,dwSWRST;

    //update pheripheral state
    g_dwPeripheralState &= (~dwPeripheral);


    rd(SCU_PSOCTRL_REG,dwPSO);
    rd(CKGEN_CLKEN_REG,dwClkEN);
    rd(CKGEN_SWRST_REG,dwSWRST);
    
    GetPeripheralOFFValue(dwPeripheral,&dwPSO,&dwClkEN,&dwSWRST);

    wr(CKGEN_SWRST_REG,dwSWRST);
    wr(CKGEN_CLKEN_REG,dwClkEN);
    wr(SCU_ISOCTRL_REG,dwPSO  );
    wr(SCU_PSOCTRL_REG,dwPSO  );
}

void cc6801_LowPower_PowerOn(void)
{
    
    uint32_t dwTmp;
    rd(PMU_MODE_RADIOEN,dwTmp);
    dwTmp &=(~PMU_MODE_MASK);
    dwTmp |= PMU_MODE_ACTIVE;
    setbit(PMU_MODE_RADIOEN,dwTmp);     //mode register is set.
    drvi_ClockDelayMs(1);

    dwTmp &=(~PMU_RADIOEN_MASK); 
    dwTmp |= (PMU_RADIOEN_BGP|PMU_RADIOEN_LDOVDDD);
    setbit(PMU_MODE_RADIOEN,dwTmp);     //disable all icytrx
    drvi_ClockDelayUs(100);

    setbit(PMU_CUCTRL,vd3_adc_en);                  //val = 0x10000000
    drvi_ClockDelayUs(100);

    setbit(PMU_CUCTRL,vddio_postdrv_pd1_en);        //val = 0x30000000
    setbit(PMU_CUCTRL,ldo_pd0dig_en);               //val = 0x31000000
    drvi_ClockDelayUs(100);

    setbit(PMU_CUCTRL,ldo_pd1ana_en);               //val = 0x37500000
    drvi_ClockDelayUs(100);
}

void cc6801_LowPower_PowerOff(void)
{
    uint32_t dwTmp;
    rd(PMU_MODE_RADIOEN,dwTmp);
    dwTmp &=(~PMU_MODE_MASK);
    dwTmp |= PMU_MODE_SLEEP;
    setbit(PMU_MODE_RADIOEN,dwTmp);     //mode register is set.
    drvi_ClockDelayMs(1);
    
    dwTmp &=(~PMU_RADIOEN_MASK); 
    dwTmp |= PMU_RADIOEN_PTATOFF;
    setbit(PMU_MODE_RADIOEN,dwTmp);     //disable all icytrx
    drvi_ClockDelayUs(100);
    
    clrbit(PMU_CUCTRL,ldo_pd1ana_en);
    drvi_ClockDelayUs(100);
    
    
    clrbit(PMU_CUCTRL,vddio_postdrv_pd1_en);
    drvi_ClockDelayUs(100);
    
    clrbit(PMU_CUCTRL,vd3_adc_en);
    drvi_ClockDelayUs(100);

}
