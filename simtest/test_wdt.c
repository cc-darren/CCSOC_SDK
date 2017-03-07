#include "stdint.h"
#include "addr_map.h"

static uint32_t int_en = 3;                                             //2bit
//reg		dual_bank = 1'b0;
//reg		redu_en=1'b0;
//reg [31:0]	rdata;
//reg [31:0]	data;
//reg [31:0]	addr=32'h8000_6000; 
//integer		i;

//WDT register
static uint32_t pre_scale = 0x1;                                        //8bit
static uint32_t cre = 0;  // Chip reset enable                          //1bit
static uint32_t time_load = 1; // timer load                            //1bit
static uint32_t stop = 0;      // 0:disable 1:enable                    //1bit
static uint32_t intr_load = 0x00000050;  // WDT and Timer loader        //32bit
static uint32_t crc_load = 0x00000020; // Chip reset counter loader     //32bit
//reg      int_en = 1'b1;  // interrupt enable
//reg      int_clr = 1'b1;  //interrupt clear

extern uint32_t WDT_INTR;

//0: success; others: fail
int test_wdt(void) {

    // WDT function
    //$display("@%t, %m MSG: ****** test name: init_test_CPU_BFM", $time);
    //wait (`TB_INIT_DONE);
    //$display("@%d, %m MSG: TB_INIT_DONEROM", $time);

    //repeat(4) @(posedge `CHIP_CLK48M);
    delayns(4);

    //`CHIP_CPU_BFM.ahbwr( `WDT_ADDR_BASE+32'h08, 3'd0, 4'd2, intr_load);
    wr(WDT_ADDR_BASE+0x08, intr_load);
    //`CHIP_CPU_BFM.ahbwr( `WDT_ADDR_BASE+32'h0c, 3'd0, 4'd2, crc_load);
    wr(WDT_ADDR_BASE+0x0c, crc_load);
    //`CHIP_CPU_BFM.ahbwr( `WDT_ADDR_BASE+32'h04, 3'd0, 4'd2, 32'h0000_0000 | stop | pre_scale << 8 | cre <<2);
    wr(WDT_ADDR_BASE+0x04, (0x00000000 | stop | pre_scale << 8 | cre <<2));
    //`CHIP_CPU_BFM.ahbwr( `WDT_ADDR_BASE+32'h00, 3'd0, 4'd2, 32'h0000_0000 | int_en);
    wr(WDT_ADDR_BASE+0x00, (0x00000000 | int_en));

//    `CHIP_CPU_BFM.ahbrd( `I2S_ADDR_BASE+32'h00, 3'd0, 4'd2, rdata, 1, 32'h17_0109, 1'b0);

   // wait(topsys.chip.core_domain0.watchdog_top_wrap.watchdog_top.watchdog_int);
    //wait(`WDT_INTR);
    while(!WDT_INTR);
    WDT_INTR = 0;

    //repeat(4)@(posedge `CHIP_CLK48M);
    delayns(4);
    //`CHIP_CPU_BFM.ahbwr( `WDT_ADDR_BASE+32'h00, 3'd0, 4'd2, 32'h0000_0000 | int_clr <<16 | 1'b0);

    //repeat(4)@(posedge `CHIP_CLK48M);    
    delayns(4);
    //wait(~`WDT_INTR);
    //wait(~topsys.chip.core_domain0.watchdog_top_wrap.watchdog_top.watchdog_int);

    //repeat(100)@(posedge `CHIP_CLK48M);
    delayns(100);
    
    // Timer function
    //`CHIP_CPU_BFM.ahbwr( `WDT_ADDR_BASE+32'h04, 3'd0, 4'd2, 32'h0000_0000 | 1'b1 | pre_scale << 8 | cre <<2 | time_load <<1);
    wr(WDT_ADDR_BASE+0x04, (0x00000000 | 1 | pre_scale << 8 | cre <<2 | time_load <<1));
    //`CHIP_CPU_BFM.ahbwr( `WDT_ADDR_BASE+32'h04, 3'd0, 4'd2, 32'h0000_0000 | stop | pre_scale << 8 | cre <<2 | time_load <<1);
    wr(WDT_ADDR_BASE+0x04, (0x00000000 | stop | pre_scale << 8 | cre <<2 | time_load <<1));
    //`CHIP_CPU_BFM.ahbwr( `WDT_ADDR_BASE+32'h00, 3'd0, 4'd2, 32'h0000_0000 | int_en);
    wr(WDT_ADDR_BASE+0x00, (0x00000000 | int_en));

    //wait(`WDT_INTR);
    while(!WDT_INTR);
    WDT_INTR = 0;
    //wait(topsys.chip.core_domain0.watchdog_top_wrap.watchdog_top.watchdog_int);

    //repeat(4)@(posedge `CHIP_CLK48M);    
    delayns(4);
    //`CHIP_CPU_BFM.ahbwr( `WDT_ADDR_BASE+32'h00, 3'd0, 4'd2, 32'h0000_0000 | int_clr <<16 | 1'b0);

    //repeat(4)@(posedge `CHIP_CLK48M);    
    delayns(4);
    //wait(~`WDT_INTR);
    //wait(~topsys.chip.core_domain0.watchdog_top_wrap.watchdog_top.watchdog_int);
   
    //`CHIP_CPU_BFM.ahbwr( `WDT_ADDR_BASE+32'h04, 3'd0, 4'd2, 32'h0000_0000 | 1'b1 | pre_scale << 8 | cre <<2 | time_load <<1);
    wr(WDT_ADDR_BASE+0x04, (0x00000000 | 1 | pre_scale << 8 | cre <<2 | time_load <<1));

    //repeat(5000)@(posedge `CHIP_CLK48M);    
    //$finish;

    return 0;
}
