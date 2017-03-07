#include "addr_map.h"
#include "stdint.h"

//WDT register
static uint32_t  pre_scale = 1;           //8bit
static uint32_t  cre = 0;                 //1bit  Chip reset enable
//static uint32_t  time_load = 1;           //1bit // timer load
static uint32_t  stop = 0;                //1bit // 0:disable 1:enable
static uint32_t  intr_load = 0x00000050;     //32bit // WDT and Timer loader
static uint32_t  crc_load = 0x00000050;      //32bit // Chip reset counter loader
static uint32_t  int_en = 1;              //1bit // interrupt enable
static uint32_t  int_clr = 1;             //1bit //interrupt clear

extern uint32_t WDT_INTR;

int test_wdtrst(void)
{

   // WDT function
    //$display("@%t, %m MSG: ****** test name: init_test_CPU_BFM", $time);
    //wait (`TB_INIT_DONE);
    //$display("@%d, %m MSG: TB_INIT_DONEROM", $time);

    //repeat(4) @(posedge `CHIP_CLK48M);
    delayns(4);

#ifdef ROM_BOOT
  test_done();
#else

    //`CHIP_CPU_BFM.ahbwr( `WDT_ADDR_BASE+32'h08, 3'd0, 4'd2, intr_load);
    wr(WDT_ADDR_BASE+0x08, intr_load);
    //`CHIP_CPU_BFM.ahbwr( `WDT_ADDR_BASE+32'h0c, 3'd0, 4'd2, crc_load);
    wr(WDT_ADDR_BASE+0x0c, crc_load);
    //`CHIP_CPU_BFM.ahbwr( `WDT_ADDR_BASE+32'h04, 3'd0, 4'd2, 32'h0000_0000 | stop | pre_scale << 8 | cre <<2 | 1'b1 <<2);
    wr(WDT_ADDR_BASE+0x04, (0x00000000 | stop | pre_scale << 8 | cre <<2 | 1 <<2));
    //`CHIP_CPU_BFM.ahbwr( `WDT_ADDR_BASE+32'h00, 3'd0, 4'd2, 32'h0000_0000 | int_en);
    wr(WDT_ADDR_BASE+0x00, (0x00000000 | int_en));

    //wait(`WDT_INTR);
    while(!WDT_INTR);
    WDT_INTR = 0;

    //repeat(4)@(posedge `CHIP_CLK48M);
    delayns(4);
    //`CHIP_CPU_BFM.ahbwr( `WDT_ADDR_BASE+32'h00, 3'd0, 4'd2, 32'h0000_0000 | int_clr <<16 | 1'b0);
    wr(WDT_ADDR_BASE+0x00, (0x00000000 | int_clr <<16 | 0));

    //repeat(4)@(posedge `CHIP_CLK48M);
    delayns(4);
    //wait(~`WDT_INTR);


    //repeat(100000)@(posedge `CHIP_CLK48M);
    delayns(100000);

    //$finish;
#endif

    return 0;

}

