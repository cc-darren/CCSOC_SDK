#include "stdint.h"
#include "addr_map.h"

//reg [1:0] 	int_en = 2'b11;
//reg		dual_bank = 1'b0;
//reg		redu_en=1'b0;
static uint32_t rdata;              //32bit
//reg [31:0]	data;
//reg [31:0]	addr=32'h8000_6000; 
//integer		i;

//wire dmic_clk = topsys.chip.GPIO[0];
//reg  dmic_din = 0;
//
//always @(posedge dmic_clk) begin
// //#2 dmic_din = $random & 1'b1;
// #2 dmic_din = dmic_din ^ 1'b1;
// //#1 dmic_din = ~dmic_din & 1'b1;
//end

extern uint32_t DMIC_INTR;
extern void set_port_dmic0(void);
extern int rd_compare(uint32_t addr, uint32_t val, uint32_t compare_val);

//0: success; others: fail
int test_dmic(void) {

    //$display("@%t, %m MSG: ****** test name: init_test_CPU_BFM", $time);
    //wait (`TB_INIT_DONE);
    //$display("@%d, %m MSG: TB_INIT_DONEROM", $time);

    //@(posedge `CHIP_CLK48M);
    delayns(1);

    set_port_dmic0();

//  force topsys.chip.core_domain0.dmic_top_wrap.dmic_din = dmic_din;
//  force topsys.chip.GPIO[1] = dmic_din;
    // DMIC setting 
    //  interrupt enable
    //`CHIP_CPU_BFM.ahbwr(`DMIC_ADDR_BASE, 3'd0, 4'd2, 32'h0_0001);
    wr(DMIC_ADDR_BASE, 0x00001); 
    // lr_avail = left_channel
    //`CHIP_CPU_BFM.ahbwr(`DMIC_ADDR_BASE+32'h04, 3'd0, 4'd2, 32'h0); 
    wr(DMIC_ADDR_BASE+0x04, 0x0); 
    // dma_byte_num
    //`CHIP_CPU_BFM.ahbwr(`DMIC_ADDR_BASE+32'h08, 3'd0, 4'd2, 32'hc); 
    wr(DMIC_ADDR_BASE+0x08, 0xc); 
    // dma_start_addr
    //`CHIP_CPU_BFM.ahbwr(`DMIC_ADDR_BASE+32'h0c, 3'd0, 4'd2, `SYSRAM_ADDR_BASE); 
    wr(DMIC_ADDR_BASE+0x0c, SYSRAM_TEST_ADDR_BASE); 
    // dma_end_addr
    //`CHIP_CPU_BFM.ahbwr(`DMIC_ADDR_BASE+32'h10, 3'd0, 4'd2, `SYSRAM_ADDR_BASE+32'h50); 
    wr(DMIC_ADDR_BASE+0x10, SYSRAM_TEST_ADDR_BASE+0x50); 

    // enable dmic
    //`CHIP_CPU_BFM.ahbwr(`DMIC_ADDR_BASE+32'h14, 3'd0, 4'd2, 32'h1);
    wr(DMIC_ADDR_BASE+0x14, 0x1); 

    //topsys.dmic_bfm.dmic_l(1'b0);
    
    //wait(topsys.chip.core_domain0.dmic_top_wrap.dmic_int);
    while(!DMIC_INTR);
    DMIC_INTR = 0;
    // disable dmic
    //`CHIP_CPU_BFM.ahbwr(`DMIC_ADDR_BASE+32'h14, 3'd0, 4'd2, 32'h0); 
    wr(DMIC_ADDR_BASE+0x14, 0x0);

    //repeat(100)@(posedge `CHIP_CLK48M);
    delayns(100);
    //`CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h00, 3'd0, 4'd2, rdata, 1, 32'hc0895e81, 1'b0);
    rd_compare(SYSRAM_TEST_ADDR_BASE+0x00, rdata, 0xc0895e81);
    //`CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h04, 3'd0, 4'd2, rdata, 1, 32'h8484d609, 1'b0);
    rd_compare(SYSRAM_TEST_ADDR_BASE+0x04, rdata, 0x8484d609);
    //`CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h08, 3'd0, 4'd2, rdata, 1, 32'hb1f05663, 1'b0);
    rd_compare(SYSRAM_TEST_ADDR_BASE+0x08, rdata, 0xb1f05663);
    //`CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h0c, 3'd0, 4'd2, rdata, 1, 32'h06b97b0d, 1'b0);
    rd_compare(SYSRAM_TEST_ADDR_BASE+0x0c, rdata, 0x06b97b0d);
  //  `CHIP_CPU_BFM.ahbrd( 32'h6_0010, 3'd0, 4'd2, rdata, 1, 32'hb5d1_ef7e, 1'b0);
 //   `CHIP_CPU_BFM.ahbrd( 32'h6_0014, 3'd0, 4'd2, rdata, 1, 32'h1dff_e4af, 1'b0);
 //   `CHIP_CPU_BFM.ahbrd( 32'h6_0018, 3'd0, 4'd2, rdata, 1, 32'haadc_9beb, 1'b0);
 //   `CHIP_CPU_BFM.ahbrd( 32'h6_001c, 3'd0, 4'd2, rdata, 1, 32'h404_c160, 1'b0);
 //   `CHIP_CPU_BFM.ahbrd( 32'h6_0020, 3'd0, 4'd2, rdata, 1, 32'h6a09_3dbc, 1'b0);
 //   `CHIP_CPU_BFM.ahbrd( 32'h6_0024, 3'd0, 4'd2, rdata, 1, 32'h3bb5_69a4, 1'b0);
 //
    //disable interrupt
    //`CHIP_CPU_BFM.ahbwr(`DMIC_ADDR_BASE, 3'd0, 4'd2, 32'h0_0100);

    //wait(~topsys.chip.core_domain0.dmic_top_wrap.dmic_int);
    
    //repeat(5000)@(posedge `CHIP_CLK48M);    
    //$finish;

    return 0;
}
