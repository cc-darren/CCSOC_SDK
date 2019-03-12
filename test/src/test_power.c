
#include <stdio.h>
#include "hs.h"
#include "project.h"
#include "pwm.h"
#include "gpio.h"
#include "rtc.h"
#include "tracer.h"
#include "drvi_clock.h"

extern uint32_t dwRTC_wake;
extern uint32_t PeripheralOn;


void rtc(void) {
    T_Rtc rtcTime;
    T_Alarm alarmTime;

    cc6801_RtcInit();

    rtcTime = cc6801_RtcGetTime();
    
    alarmTime.sec = rtcTime.sec + 10;
    alarmTime.min = rtcTime.min;
    alarmTime.hour = rtcTime.hour;
    alarmTime.h24 = rtcTime.h24;
    alarmTime.day = rtcTime.day;
    alarmTime.month = rtcTime.month;
    
    cc6801_AlarmSetTime(RTC_ALARM,alarmTime);
    cc6801_AlarmEnable(RTC_ALARM);
}

//clk select, use 32K if rtc enabled.
//            use 40K for lower power requested.


#define HS_INTR(inintr_en,inintr_id,exintr_level,exintr_en,exintr_id) ((inintr_en<<24) | (inintr_id<<16) | (exintr_level<<15) | (exintr_en<<8) | (exintr_id+2))



#define PUP_EXID_ENABLE     1
#define PUP_EXID_DISABLE    0
#define PUP_EXID_TRGGER_F   1
#define PUP_EXID_TRGGER_R   0
#define PUP_INID_ENABLE     1
#define PUP_INID_DISABLE    0


//#define WAKEUPTIMERSEL (HS_PERIPHERAL_RTC)
#define WAKEUPTIMERSEL (HS_PERIPHERAL_WKT1)

void clockingGPIO(uint32_t gpio, uint32_t count)
{
  #if (!TEST_HIBERNATE)
    uint32_t base = GPIO_PAD_OUT_REG;
    
    if (gpio > 31)
    {
        gpio -= 32;
        base = GPIO1_PAD_OUT_REG;
    }
    
    gpio = (1<<gpio);
    
    for(uint32_t i=0;i<count;i++)
    {
        clrbit(base,gpio);
        drvi_ClockDelayUs(10);
        setbit(base,gpio);
        drvi_ClockDelayUs(10);
        clrbit(base,gpio);
    }
  #endif
}

void dumpreg(uint32_t start, uint32_t stop)
{
  #if (!TEST_HIBERNATE)
    for(uint32_t addr=start;addr<=stop;addr+=0x10)
    {
        TracerInfo("0x%8x | %08x %08x %08x %08x\r\n",addr, *(uint32_t*)addr, *(uint32_t*)(addr+4), *(uint32_t*)(addr+8), *(uint32_t*)(addr+12));
    }
    delayns(10000);
  #endif
}

void testsleep(uint32_t SleepMode)
{
    cc_hs_clst_t tmpcmdlst;
    uint32_t tmpdata;

    /*--------------------------------------------------------------*/
    /*-----    prepare for sleep case     --------------------------*/
    /*--------------------------------------------------------------*/
    //PeripheralOn = (HS_PERIPHERAL_GPIO |HS_PERIPHERAL_PWM1);
    //PeripheralOn = (HS_PERIPHERAL_CLKGEN); //ckgen has to be on with rtc and ble alive
    //PeripheralOn = HS_PERIPHERAL_UART0;

    #if (WAKEUPTIMERSEL & HS_PERIPHERAL_RTC)
    
        PeripheralOn |= HS_PERIPHERAL_RTC ;
        // rtc init and test
        rtc();
        regWKTM1->bf.enable = 0;
        regWKTM1->bf.intEn = 0;
        regWKTM1->bf.intSts = 1;
        wr(0x40002400,0x00010000);
    #elif (WAKEUPTIMERSEL & HS_PERIPHERAL_WKT1)
        
        PeripheralOn |= HS_PERIPHERAL_WKT1 ;
        //wktm1 init

        regCKGEN->bf.wktm1SwRst = 1;
        regWKTM1->bf.clear = 1;           //clear WKT counter
        drvi_ClockDelayMs(1);             //must wait until counter cleared
        regWKTM1->bf.intSts = 1;          //clear interrupt
        regWKTM1->dw.highCounter = 0;     
        regWKTM1->dw.lowCounter = 0x100;  //set count
        regWKTM1->bf.pwmTimerSel = 1;     //select Timer
        regWKTM1->bf.repeat = 0;          //1:one-shot, 0:repeat
        regWKTM1->bf.pwmOutEn = 0;        //this is timer, no PWM output

        regWKTM1->bf.prescaler = 32;       //default to highest clock

        regWKTM1->bf.intEn = 1;           //enable interrupt;
        regWKTM1->bf.clear = 0;
        regWKTM1->bf.enable = 1;

        //dumpreg(WKTM1_ADDR_BASE, 0x40000714);
        NVIC_ClearPendingIRQ(WKTM1_IRQn);
        NVIC_EnableIRQ(WKTM1_IRQn);
    #endif

    #if (WAKEUPTIMERSEL & HS_PERIPHERAL_WDT)
        
        PeripheralOn |= HS_PERIPHERAL_WDT ;
        #if 0 //reset chip in sleep
        regWDT->dw.intCounter = 0x400;
        regWDT->dw.rstCounter = 0x200;
        #else //HS should kick WDT
        regWDT->dw.intCounter = 0x200;
        regWDT->dw.rstCounter = 0x400;
        #endif

        regWDT->bf.prescaler = 1;     //default to highest clock
        regWDT->bf.timerSel = 0;      //select watchdog
        
        regWDT->bf.wdtEn = 1;
        regWDT->bf.wdtEn = 0;

        regWDT->bf.rstEn = 1;         //enable chip reset when reach reset count

        regWDT->bf.intSts = 1;        //clear interrupt;
        regWDT->bf.intEn = 1;         //default enable interrupt;

        NVIC_ClearPendingIRQ(WDT_IRQn);
        NVIC_EnableIRQ(WDT_IRQn);
        
    #endif

    //clockingGPIO(36,3);
    //clockingGPIO(37,3);
    //clockingGPIO(38,3);
    //clockingGPIO(39,3);

    if (SleepMode==HS_PWRMODE_HIBERNATE)
    {
        PeripheralOn = 0;
        //PeripheralOn = HS_PERIPHERAL_GPIO;
    }
    else
    {
        //PeripheralOn |= (HS_PERIPHERAL_BLE|HS_PERIPHERAL_BLEPHY|HS_PERIPHERAL_CLKGEN|HS_PERIPHERAL_GPIO);
        
        //Note: turn off HS_PERIPHERAL_BLE to gain 3uA
        //**Note: GPIO module can be turned off but FW can't access GPIO registers anymore to avoid system hang
        
        //PeripheralOn |= (HS_PERIPHERAL_BLEPHY|HS_PERIPHERAL_CLKGEN);    //both slp&dslp current 10uA (ret Off)
        //PeripheralOn |= (HS_PERIPHERAL_CLKGEN);     //both slp&dslp current 1.1mA(ret Off), can't sleep
                                                    //if system swtich to 2M before wfi, system can sleep and wakeup "once", current is 25uA(ret On)
        
        //PeripheralOn |= (HS_PERIPHERAL_BLEPHY);     //both slp&dslp current 0.9mA(ret Off)
                                                    //if system switch to 2M before wfi, system can sleep and wakeup "once", current is 25uA(ret On)
        
        //PeripheralOn |= 0;                          //both slp&dslp current 0.9mA(ret Off)
                                                    //if system switch to 2M before wfi, system can sleep and wakeup "once", current is 25uA(ret On)

        PeripheralOn |= (HS_PERIPHERAL_GPIO);
    }
    
    /*
    
    if HS_PERIPHERAL_BLEPHY is off during sleep period,
    FW has to select SCU clock selection to 2M or 32K before shut off sequence (clk reset/clk disable/iso/pso)
    
    or just keep the clk to 48M if HS_PERIPHERAL_BLEPHY isn't off
    */




    // enable HS, #0x40000014
    setbit(SCU_CLKCFG1_REG,SCU_CLKCFG1_REG_HSGatedClk);
    //regSCU->bf.dctrlGatedClk = 1; //enable data ram(retention ram), it has been enabled in bootrom


#ifdef FPGA
    //0x40002214 set wakeup interrupt set 0
    wr(HS_DR0_INTR_REG,HS_INTR( 0, 0, 0, 0, 0));
    //cc6801_disable_irq(0);


    //0x40002220 set wakeup interrupt set 1
    wr(HS_DR1_INTR_REG,HS_INTR(0,0,0,PUP_EXID_DISABLE,(0)));
    //cc6801_disable_irq(0);
#else
    
    //0x40002214 set wakeup interrupt set 0
//    wr(HS_DR0_INTR_REG,HS_INTR(PUP_INID_DISABLE,SPI1_M_IRQn,PUP_EXID_TRGGER_F,PUP_EXID_DISABLE,0));
    //cc6801_disable_irq(0);

    //0x40002220 set wakeup interrupt set 1
//    wr(HS_DR1_INTR_REG,HS_INTR(PUP_INID_DISABLE,I2C0_M_IRQn,PUP_EXID_TRGGER_R,PUP_EXID_DISABLE,0));
    //cc6801_disable_irq(0);
    
    
    //0x4000222C set wakeup interrupt set 2
//    wr(HS_DR2_INTR_REG,HS_INTR(PUP_INID_DISABLE,I2C1_M_IRQn,PUP_EXID_TRGGER_R,PUP_EXID_DISABLE,0));
    //cc6801_disable_irq(0);
#endif

    //** PRD ** power down seqence for HS
    HS_BeginCmdList(&tmpcmdlst,0x20029000,PeripheralOn,HS_CMDLIST_PRD);
    HS_EndCmdList(&tmpcmdlst);


    /*---------------------------------------
    because there is no read commands for HS commands
    peripheral in Dr0/Dr1/Dr2 should have same setting or it might cause another perpheral off
    ---------------------------------------*/
    //PeripheralOn |= (HS_PERIPHERAL_SPI1|HS_PERIPHERAL_I2C0|HS_PERIPHERAL_I2C1);

    //** DR0 ** prepare for dr0 power up seq
    tmpdata = 0x20029100;
    HS_BeginCmdList(&tmpcmdlst,tmpdata,PeripheralOn,HS_CMDLIST_PUP0);
    //enable SPI 1 read
    rd(SCU_CLKCFG1_REG,tmpdata);
    HS_setHSCmd(&tmpcmdlst,SCU_CLKCFG1_REG,tmpdata); 

    HS_setHSCmd(&tmpcmdlst,SPI1_INTR_REG    ,0x00000003);
    HS_setHSCmd(&tmpcmdlst,SPI1_CTRL_REG    ,0x01000403);
    HS_setHSCmd(&tmpcmdlst,SPI1_DMA_WR_REG  ,(tmpdata+0x70));
    HS_setHSCmd(&tmpcmdlst,(tmpdata+0x70)   ,0x000000A8);
    HS_setHSCmd(&tmpcmdlst,SPI1_DMA_RD_REG  ,(tmpdata+0x80));
    HS_setHSCmd(&tmpcmdlst,SPI1_DMA_CTRL_REG,0x1020500);

    HS_EndCmdList(&tmpcmdlst);
    
//    NVIC_ClearPendingIRQ(SPI1_M_IRQn);
//    NVIC_EnableIRQ(SPI1_M_IRQn);


    //** DR1 **prepare for dr1 power up seq
    tmpdata = 0x20029200;
    HS_BeginCmdList(&tmpcmdlst,tmpdata,PeripheralOn,HS_CMDLIST_PUP1);
    //enable I2C0 read
    rd(SCU_CLKCFG1_REG,tmpdata);
    HS_setHSCmd(&tmpcmdlst,SCU_CLKCFG1_REG,tmpdata); 

    HS_setHSCmd(&tmpcmdlst,(I2C0_ADDR_BASE + 0x00),0x0000000F);
    HS_setHSCmd(&tmpcmdlst,(I2C0_ADDR_BASE + 0x08),0x900F8000);
    HS_setHSCmd(&tmpcmdlst,(I2C0_ADDR_BASE + 0x0C),(tmpdata+0x70));
    HS_setHSCmd(&tmpcmdlst,(tmpdata+0x70)         ,0x00000062);
    HS_setHSCmd(&tmpcmdlst,(I2C0_ADDR_BASE + 0x10),(tmpdata+0x80));
    HS_setHSCmd(&tmpcmdlst,(I2C0_ADDR_BASE + 0x14),0x00780002);
    HS_setHSCmd(&tmpcmdlst,(I2C0_ADDR_BASE + 0x18),0x00620004);

    HS_EndCmdList(&tmpcmdlst);

//    NVIC_ClearPendingIRQ(I2C0_M_IRQn);
//    NVIC_EnableIRQ(I2C0_M_IRQn);

    //** DR2 **prepare for dr2 power up seq
    tmpdata = 0x20029300;
    HS_BeginCmdList(&tmpcmdlst,tmpdata,PeripheralOn,HS_CMDLIST_PUP2);
    //enable I2C1 read
    rd(SCU_CLKCFG1_REG,tmpdata);
    HS_setHSCmd(&tmpcmdlst,SCU_CLKCFG1_REG,tmpdata); 

    HS_setHSCmd(&tmpcmdlst,(I2C1_ADDR_BASE + 0x00),0x0000000F);
    HS_setHSCmd(&tmpcmdlst,(I2C1_ADDR_BASE + 0x08),0x900F8000);
    HS_setHSCmd(&tmpcmdlst,(I2C1_ADDR_BASE + 0x0C),(tmpdata+0x70));
    HS_setHSCmd(&tmpcmdlst,(tmpdata+0x70)         ,0x00000094);
    HS_setHSCmd(&tmpcmdlst,(I2C1_ADDR_BASE + 0x10),(tmpdata+0x80));
    HS_setHSCmd(&tmpcmdlst,(I2C1_ADDR_BASE + 0x14),0x00780002);
    HS_setHSCmd(&tmpcmdlst,(I2C1_ADDR_BASE + 0x18),0x00940004);

    HS_EndCmdList(&tmpcmdlst);

//    NVIC_ClearPendingIRQ(I2C1_M_IRQn);
//    NVIC_EnableIRQ(I2C1_M_IRQn);


    //renew PeripheralOn for wakeup setting
    PeripheralOn |= (HS_PERIPHERAL_BLEPHY|HS_PERIPHERAL_CLKGEN); //BLEPHY must be enabled after wakeup or high frequency will not exist.

    //PeripheralOn |= (HS_PERIPHERAL_GPIO );  //enable GPIO to make GPIO1_INTR_PRIO_REG work
    //PeripheralOn |= (HS_PERIPHERAL_AES |HS_PERIPHERAL_PWM1);   //HS_PERIPHERAL_AES  switch on to test AES below

    //** DR3 wakeup **prepare wake up peripheral settings
    tmpdata = 0x20029400;
    HS_BeginCmdList(&tmpcmdlst,tmpdata,PeripheralOn,HS_CMDLIST_PUP3);
//HS_setHSCmd(&tmpcmdlst,GPIO1_PAD_OUT_REG,0x00000020); 
    
    //restore clock setting 
    rd(SCU_PLLCFG_REG,tmpdata);
    HS_setHSCmd(&tmpcmdlst,SCU_PLLCFG_REG,tmpdata);

    rd(SCU_CLKCFG0_REG,tmpdata);
    HS_setHSCmd(&tmpcmdlst,SCU_CLKCFG0_REG,tmpdata);

    rd(SCU_CLKCFG1_REG,tmpdata);
    HS_setHSCmd(&tmpcmdlst,SCU_CLKCFG1_REG,tmpdata);

    rd(CKGEN_CFG1_REG,tmpdata);
    HS_setHSCmd(&tmpcmdlst,CKGEN_CFG1_REG,tmpdata);

    rd(CKGEN_CFG2_REG,tmpdata);
    HS_setHSCmd(&tmpcmdlst,CKGEN_CFG2_REG,tmpdata);

    rd(CKGEN_CFG3_REG,tmpdata);
    HS_setHSCmd(&tmpcmdlst,CKGEN_CFG3_REG,tmpdata);

    rd(CKGEN_CFG4_REG,tmpdata);
    HS_setHSCmd(&tmpcmdlst,CKGEN_CFG4_REG,tmpdata);    

    rd(EF_TIMING0_REG,tmpdata);
    HS_setHSCmd(&tmpcmdlst,EF_TIMING0_REG,tmpdata);

    rd(EF_TIMING1_REG,tmpdata);
    HS_setHSCmd(&tmpcmdlst,EF_TIMING1_REG,tmpdata);

    rd(EF_TIMING2_REG,tmpdata);
    HS_setHSCmd(&tmpcmdlst,EF_TIMING2_REG,tmpdata);

    rd(EF_TIMING3_REG,tmpdata);
    HS_setHSCmd(&tmpcmdlst,EF_TIMING3_REG,tmpdata);

    rd(EF_TIMING4_REG,tmpdata);
    HS_setHSCmd(&tmpcmdlst,EF_TIMING4_REG,tmpdata);

    rd(EF_TIMING5_REG,tmpdata);
    HS_setHSCmd(&tmpcmdlst,EF_TIMING5_REG,tmpdata);

    rd(EF_TIMING6_REG,tmpdata);
    HS_setHSCmd(&tmpcmdlst,EF_TIMING6_REG,tmpdata);

    HS_EndCmdList(&tmpcmdlst);



    //notify HS will monitor this bit to start hand shake with CPU
    SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;


    
//setbit(0x40000028,0x000003F8);

    //========================================================================================
    //0x40002200, sleep mode, clock source and mode selection
    //  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - 
    //wr(HS_CONFIG_REG,(HS_CONFIG_ClrIntr|HS_CONFIG_2M_Disable|HS_CONFIG_32K_Disable|HS_CONFIG_WDT_INTR_CLR_EN|SleepMode|0x0000F000));
    wr(HS_CONFIG_REG,(HS_CONFIG_ClrIntr|HS_CONFIG_2M_Disable|HS_CONFIG_32K_Enable|HS_CONFIG_WDT_INTR_CLR_EN|SleepMode|0x00000000));
    regSCU->dw.clkConfig0 &= ~0x00000060;
    
    //*** Note ***
    //The combination must be "HS_CONFIG_2M_Disable|HS_CONFIG_32K_Enable", then we can get lowest current (7uA in hibernate)
    //why???
    //*** Note ***
    
    //========================================================================================

    if(HS_PWRMODE_SLEEP == SleepMode)
    {
        regHS->dwRetRamCtrl = 0x000000FF;
    }
    else if(HS_PWRMODE_DEEPSLEEP == SleepMode)
    {
        regHS->dwRetRamCtrl = 0x00000000;
    }
    else if(HS_PWRMODE_HIBERNATE == SleepMode)
    {
        regHS->dwRetRamCtrl = 0x0000FFFF;
        
        regHS->dwPdnLdo = (1<<27);
        
    }

        //In hibernate mode, FW must switch system clock to 2M or 32K before go to sleep
        regSCU->dw.clkConfig1 &= ~0x03;  //switch to 2M     ->7.7uA
        //regSCU->dw.clkConfig1 |=  0x02;  //switch to 32K    ->7.7uA (same as 2M clk)
    
    //dumpreg(HS_ADDR_BASE, 0x4000224C);
    //dumpreg(WKTM1_ADDR_BASE, 0x40000714);
    
        GLOBAL_INT_STOP();
    
//    wr(HS_PMU_DCDC_REG,0x16200370);

    #if 0
        //use wfi for sleep mode testing
        __wfi();
    #else
        //only for deep sleep
        extern void Dslp_Handler(void);
        Dslp_Handler();
    #endif


    regSCU->dw.clkConfig1 |= 0x01;
    
    //notify HS to end
    SCB->SCR &= (~SCB_SCR_SLEEPDEEP_Msk);    
    
//clrbit(0x40000028,0x000003F8);
    // ----------------------------
    // wait HS back to normal state
    do 
    {
        delayns(10);
        rd(HS_STS_REG,tmpdata);
    } while((tmpdata&0x0000000F) != HS_STATE_NORMAL);


    //0x40002200 clear HS interrupt status
    setbit(HS_CONFIG_REG,HS_CONFIG_ClrIntr);

    //0x40000014 set HS gated clock to stop HS.
    clrbit(SCU_CLKCFG1_REG,SCU_CLKCFG1_REG_HSGatedClk) ; //disable Hardware sequencer, HS related register must be filled before HS clock off

    
//wr(GPIO1_PAD_OUT_REG,0x00000040);

    //clockingGPIO(39,5);

    GLOBAL_INT_START();
    
}




void wait32Kready(void)
{
    uint32_t dwRdata = 0;
    while ((dwRdata&0x80000000)==0)
    {
        rd(0x40000010,dwRdata);
    } 
    
}


void test_power(void)
{
    wait32Kready();

    //wr(GPIO1_INTR_TYPE_REG, 0);

  #if TEST_SLEEP
    testsleep(HS_PWRMODE_SLEEP);
  #elif TEST_DEEPSLEEP
    testsleep(HS_PWRMODE_DEEPSLEEP);
  #elif TEST_HIBERNATE
    testsleep(HS_PWRMODE_HIBERNATE);
  #endif

    //wr(GPIO1_INTR_PRIO_REG,1);
}

