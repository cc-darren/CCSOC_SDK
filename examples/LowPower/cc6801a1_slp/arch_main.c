/**
 ****************************************************************************************
 *
 * @file arch_main.c
 *
 * @brief Main loop of the application.
 *
 * Copyright (C) CloudChip 2017-2019
 *
 *
 ****************************************************************************************
 */


/*
 * INCLUDES
 ****************************************************************************************
 */
#include <string.h>
#include "ll.h"
#include "drvi_init.h"
//#include "sw_timer.h"
//#include "ak09912.h"
#include "hs.h"
#include "drvi_gpio.h"
#include "drvi_rtc.h"
#include "drvi_pwm.h"
#include "project.h"


extern void sys_InitMain(void);


extern uint32_t PeripheralOn;


void rtc(void) {
    T_Rtc sTime;

    drvi_RtcInit();

    sTime = drvi_RtcGetTime();
    sTime.sec += 1;

    drvi_AlarmSetTime(RTC_ALARM,*((T_Alarm *)&sTime));
    drvi_AlarmEnable(RTC_ALARM);
}


void wait32Kready(void)
{
    uint32_t dwRdata = 0;
    while ((dwRdata&0x80000000)==0)
    {
        rd(0x40000010,dwRdata);
    }
}

#define HS_INTR(inintr_en,inintr_id,exintr_level,exintr_en,exintr_id) ((inintr_en<<24) | (inintr_id<<16) | (exintr_level<<15) | (exintr_en<<8) | (exintr_id+2))



#define PUP_EXID_ENABLE     1
#define PUP_EXID_DISABLE    0
#define PUP_EXID_TRGGER_F   1
#define PUP_EXID_TRGGER_R   0
#define PUP_INID_ENABLE     1
#define PUP_INID_DISABLE    0

#define WAKEUPTIMERSEL (HS_PERIPHERAL_RTC)
#define ENTERLOWPOWER HS_PWRMODE_SLEEP

int main(void)
{
    setbit(SCU_ICACHE_REG, ((1<<17) | (1<<18)) );
    regCKGEN->dw.clkEn   = 0xFFFFFFFF; //wr(CKGEN_CLKEN_REG,     0xFFFFFFFF);
    regAOCKGEN->clkEn    = 0xFFFFFFFF; //wr(AO_CKGEN_CLKEN_REG,  0xFFFFFFFF);
    wr(SCU_PSOCTRL_REG,     0x00000000);
    wr(SCU_ISOCTRL_REG,     0x00000000);
    wr(CKGEN_SWRST_REG,     0xFFFFFFFF);
    regAOCKGEN->swReset  = 0xFFFFFFFF;

    sys_InitMain();

    drvi_initialize();


    //start interrupt handling
	
    GLOBAL_INT_START();

    
    wait32Kready();

    while(1)
    {
        cc_hs_clst_t prdcmdlst;
        cc_hs_clst_t dr0cmdlst;
        cc_hs_clst_t dr1cmdlst;
        cc_hs_clst_t dr2cmdlst;
        cc_hs_clst_t dr3cmdlst;
        uint32_t tmpdata;

        wr(GPIO1_INTR_TYPE_REG, 0);


        /*--------------------------------------------------------------*/
        /*-----    prepare for sleep case     --------------------------*/
        /*--------------------------------------------------------------*/
        //PeripheralOn = (HS_PERIPHERAL_GPIO |HS_PERIPHERAL_PWM1);
        //PeripheralOn = (HS_PERIPHERAL_CLKGEN); //ckgen has to be on with rtc and ble alive
        PeripheralOn = 0;

    #if (WAKEUPTIMERSEL & HS_PERIPHERAL_RTC)

        PeripheralOn |= HS_PERIPHERAL_RTC ;
        // rtc init and test
        rtc();

    #elif (WAKEUPTIMERSEL & HS_PERIPHERAL_WKT1)

        PeripheralOn |= HS_PERIPHERAL_WKT1 ;

        //wktm1 init
        regWKTM1->bf.clear = 1;           //clear WKT counter
        regWKTM1->bf.intSts = 1;          //clear interrupt
        regWKTM1->dw.highCounter = 0;     
        regWKTM1->dw.lowCounter = 0x400;  //set count
        regWKTM1->bf.pwmTimerSel = 1;     //select Timer
        regWKTM1->bf.repeat = 1;          //default one-shot
        regWKTM1->bf.pwmOutEn = 0;        //this is timer, no PWM output

        regWKTM1->bf.prescaler = 0;       //default to highest clock

        regWKTM1->bf.intEn = 1;           //enable interrupt;
        regWKTM1->bf.clear = 0;
        regWKTM1->bf.enable = 1;

        NVIC_ClearPendingIRQ(WKTM1_IRQn);
        NVIC_EnableIRQ(WKTM1_IRQn);
    #endif

    #if (WAKEUPTIMERSEL & HS_PERIPHERAL_WDT)

        PeripheralOn |= HS_PERIPHERAL_WDT ;

        #if 0 
        //reset chip in sleep
        regWDT->dw.intCounter = 0x400;
        regWDT->dw.rstCounter = 0x200;
        #else 
        //HS should kick WDT
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


        if (ENTERLOWPOWER==HS_PWRMODE_HIBERNATE) {
            PeripheralOn = 0;
        } else {
            PeripheralOn |= (HS_PERIPHERAL_BLE|HS_PERIPHERAL_BLEPHY|HS_PERIPHERAL_CLKGEN|HS_PERIPHERAL_GPIO);
        }
    
        /*

        if HS_PERIPHERAL_BLEPHY is off during sleep period,
        FW has to select SCU clock selection to 2M or 32K before shut off sequence (clk reset/clk disable/iso/pso)

        or just keep the clk to 48M if HS_PERIPHERAL_BLEPHY isn't off
        */




        // enable HS, #0x40000014
        setbit(SCU_CLKCFG1_REG,SCU_CLKCFG1_REG_HSGatedClk);


    #ifdef FPGA
        //0x40002214 set wakeup interrupt set 0
        wr(HS_DR0_INTR_REG,HS_INTR( 0, 0, 0, 0, 0));
        drvi_DisableIrq(0);


        //0x40002220 set wakeup interrupt set 1
        wr(HS_DR1_INTR_REG,HS_INTR(0,0,0,PUP_EXID_DISABLE,(0)));
        drvi_DisableIrq(0);
    #else
        //0x40002214 set wakeup interrupt set 0
        wr(HS_DR0_INTR_REG,HS_INTR(PUP_INID_ENABLE,SPI1_M_IRQn,PUP_EXID_TRGGER_F,PUP_EXID_ENABLE,5));
        drvi_DisableIrq(5);

        //0x40002220 set wakeup interrupt set 1
        wr(HS_DR1_INTR_REG,HS_INTR(PUP_INID_ENABLE,I2C0_M_IRQn,PUP_EXID_TRGGER_R,PUP_EXID_ENABLE,6));
        drvi_DisableIrq(6);
    
        //0x4000222C set wakeup interrupt set 2
        wr(HS_DR2_INTR_REG,HS_INTR(PUP_INID_ENABLE,I2C1_M_IRQn,PUP_EXID_TRGGER_R,PUP_EXID_ENABLE,9));
        drvi_DisableIrq(9);
    #endif

        //** PRD ** power down seqence for HS
        HS_BeginCmdList(&prdcmdlst,0x20029000,PeripheralOn,HS_CMDLIST_PRD);
//        HS_EndCmdList(&tmpcmdlst);


        /*---------------------------------------
        because there is no read commands for HS commands
        peripheral in Dr0/Dr1/Dr2 should have same setting or it might cause another perpheral off
        ---------------------------------------*/
        PeripheralOn |= (HS_PERIPHERAL_SPI1|HS_PERIPHERAL_I2C0|HS_PERIPHERAL_I2C1);

        //** DR0 ** prepare for dr0 power up seq
        tmpdata = 0x20029200;
        HS_BeginCmdList(&dr0cmdlst,tmpdata,PeripheralOn,HS_CMDLIST_PUP0);
        //enable SPI 1 read
        HS_setHSCmd(&dr0cmdlst,SPI1_INTR_REG    ,0x00000003);
        HS_setHSCmd(&dr0cmdlst,SPI1_CTRL_REG    ,0x01000403);
        HS_setHSCmd(&dr0cmdlst,SPI1_DMA_WR_REG  ,(tmpdata+0x100));
        HS_setHSCmd(&dr0cmdlst,(tmpdata+0x100)   ,0x000000A8);
        HS_setHSCmd(&dr0cmdlst,SPI1_DMA_RD_REG  ,(tmpdata+0x110));
        HS_setHSCmd(&dr0cmdlst,SPI1_DMA_CTRL_REG,0x1020500);

        //HS_EndCmdList(&tmpcmdlst);

        NVIC_ClearPendingIRQ(SPI1_M_IRQn);
        NVIC_EnableIRQ(SPI1_M_IRQn);


        //** DR1 **prepare for dr1 power up seq
        tmpdata = 0x20029400;
        HS_BeginCmdList(&dr1cmdlst,tmpdata,PeripheralOn,HS_CMDLIST_PUP1);
        //enable I2C0 read
        HS_setHSCmd(&dr1cmdlst,(I2C0_ADDR_BASE + 0x00),0x0000000F);
        HS_setHSCmd(&dr1cmdlst,(I2C0_ADDR_BASE + 0x08),0x900F8000);
        HS_setHSCmd(&dr1cmdlst,(I2C0_ADDR_BASE + 0x0C),(tmpdata+0x100));
        HS_setHSCmd(&dr1cmdlst,(tmpdata+0x100)         ,0x00000062);
        HS_setHSCmd(&dr1cmdlst,(I2C0_ADDR_BASE + 0x10),(tmpdata+0x110));
        HS_setHSCmd(&dr1cmdlst,(I2C0_ADDR_BASE + 0x14),0x00780002);
        HS_setHSCmd(&dr1cmdlst,(I2C0_ADDR_BASE + 0x18),0x00620004);

        //HS_EndCmdList(&tmpcmdlst);

        NVIC_ClearPendingIRQ(I2C0_M_IRQn);
        NVIC_EnableIRQ(I2C0_M_IRQn);

        //** DR2 **prepare for dr2 power up seq
        tmpdata = 0x20029600;
        HS_BeginCmdList(&dr2cmdlst,tmpdata,PeripheralOn,HS_CMDLIST_PUP2);
        //enable I2C1 read
        HS_setHSCmd(&dr2cmdlst,(I2C1_ADDR_BASE + 0x00),0x0000000F);
        HS_setHSCmd(&dr2cmdlst,(I2C1_ADDR_BASE + 0x08),0x900F8000);
        HS_setHSCmd(&dr2cmdlst,(I2C1_ADDR_BASE + 0x0C),(tmpdata+0x100));
        HS_setHSCmd(&dr2cmdlst,(tmpdata+0x100)         ,0x00000094);
        HS_setHSCmd(&dr2cmdlst,(I2C1_ADDR_BASE + 0x10),(tmpdata+0x110));
        HS_setHSCmd(&dr2cmdlst,(I2C1_ADDR_BASE + 0x14),0x00780002);
        HS_setHSCmd(&dr2cmdlst,(I2C1_ADDR_BASE + 0x18),0x00940004);

        //HS_EndCmdList(&tmpcmdlst);

        NVIC_ClearPendingIRQ(I2C1_M_IRQn);
        NVIC_EnableIRQ(I2C1_M_IRQn);


        //renew PeripheralOn for wakeup setting
        PeripheralOn |= (HS_PERIPHERAL_BLEPHY); //BLEPHY must be enabled after wakeup or high frequency will not exist.

        PeripheralOn |= (HS_PERIPHERAL_GPIO );  //enable GPIO to make GPIO1_INTR_PRIO_REG work

        //** DR3 wakeup **prepare wake up peripheral settings
        tmpdata = 0x20029800;
        HS_BeginCmdList(&dr3cmdlst,tmpdata,PeripheralOn,HS_CMDLIST_PUP3);
        //HS_EndCmdList(&tmpcmdlst);

        //----------------------------------------------------------------
        //all cmdlst has been completed.
        //update them to make new clock setting.
        dr0cmdlst.hs_clst_cnt += 6;
        dr1cmdlst.hs_clst_cnt += 6;
        dr2cmdlst.hs_clst_cnt += 6;
        dr3cmdlst.hs_clst_cnt += 6;
        
        //--------dr0--------------
        dr0cmdlst.hs_clst_cnt -= 6;
        HS_setHSCmd(&dr0cmdlst,HS_DR1_PUP_REG,( ((dr1cmdlst.hs_clst_cnt-26)<<24) | ((dr1cmdlst.hs_clst_pos+104)&0x0003FFFF) ));
        HS_setHSCmd(&dr0cmdlst,HS_DR2_PUP_REG,( ((dr2cmdlst.hs_clst_cnt-26)<<24) | ((dr2cmdlst.hs_clst_pos+104)&0x0003FFFF) ));
        HS_setHSCmd(&dr0cmdlst,(uint32_t)(&(regHS->dwDr3Pup)),( ((dr3cmdlst.hs_clst_cnt-26)<<24) | ((dr3cmdlst.hs_clst_pos+104)&0x0003FFFF) ));
        //--------dr1--------------
        dr1cmdlst.hs_clst_cnt -= 6;
        HS_setHSCmd(&dr1cmdlst,HS_DR0_PUP_REG,( ((dr0cmdlst.hs_clst_cnt-26)<<24) | ((dr0cmdlst.hs_clst_pos+104)&0x0003FFFF) ));
        HS_setHSCmd(&dr1cmdlst,HS_DR2_PUP_REG,( ((dr2cmdlst.hs_clst_cnt-26)<<24) | ((dr2cmdlst.hs_clst_pos+104)&0x0003FFFF) ));
        HS_setHSCmd(&dr1cmdlst,(uint32_t)(&(regHS->dwDr3Pup)),( ((dr3cmdlst.hs_clst_cnt-26)<<24) | ((dr3cmdlst.hs_clst_pos+104)&0x0003FFFF) ));
        //--------dr2--------------
        dr2cmdlst.hs_clst_cnt -= 6;
        HS_setHSCmd(&dr2cmdlst,HS_DR0_PUP_REG,( ((dr0cmdlst.hs_clst_cnt-26)<<24) | ((dr0cmdlst.hs_clst_pos+104)&0x0003FFFF) ));
        HS_setHSCmd(&dr2cmdlst,HS_DR1_PUP_REG,( ((dr1cmdlst.hs_clst_cnt-26)<<24) | ((dr1cmdlst.hs_clst_pos+104)&0x0003FFFF) ));
        HS_setHSCmd(&dr2cmdlst,(uint32_t)(&(regHS->dwDr3Pup)),( ((dr3cmdlst.hs_clst_cnt-26)<<24) | ((dr3cmdlst.hs_clst_pos+104)&0x0003FFFF) ));
        
        //----------------------------------------------------------------


        //notify HS will monitor this bit to start hand shake with CPU
        SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;

        GLOBAL_INT_STOP();
    
        //========================================================================================
        //0x40002200, sleep mode, select clock source and mode
        //  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - 
        wr(HS_CONFIG_REG,(HS_CONFIG_ClrIntr|HS_CONFIG_2M_Disable|HS_CONFIG_32K_Enable|HS_CONFIG_WDT_INTR_CLR_EN|ENTERLOWPOWER));
        //========================================================================================


        //wr(HS_PMU_DCDC_REG,0x16200370);

        __wfi();

        //drvi_ClockSysClkAdjust();

        //notify HS to end
        SCB->SCR &= (~SCB_SCR_SLEEPDEEP_Msk);

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

        GLOBAL_INT_START();

        wr(GPIO1_INTR_PRIO_REG,1);
    }
}

