#include "stdint.h"
#include "addr_map.h"

//reg [1:0] 	int_en = 2'b11;
//reg		dual_bank = 1'b0;
//reg		redu_en=1'b0;
static uint32_t rdata;              //32bit
//reg [31:0]	data;
//reg [31:0]	addr=32'h8000_6000; 
//integer		i;
//reg [15:0]      ws_cnt;
//reg [31:0]      i2s_tx_data;
//reg [5:0]       tx_cnt;

extern uint32_t GPIO_INTR;
extern int rd_compare(uint32_t addr, uint32_t val, uint32_t compare_val);

//0: success; others: fail
int test_gpio(void) {

    //$display("@%t, %m MSG: ****** test name: init_test_CPU_BFM", $time);
    //wait (`TB_INIT_DONE);
    //$display("@%d, %m MSG: TB_INIT_DONEROM", $time);

    // single edge trigger
    //$display("===== GPIO single edge trigger ,active high =====");
    // GPIO test
    // GPIO[31:0]
    //`CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h08, 3'd0, 4'd2, 32'h0000_0000);
    wr(GPIO_ADDR_BASE+0x08, 0x00000000); 
    // GPIO[47:32] 
    //`CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h34, 3'd0, 4'd2, 32'h0000_0000);
    wr(GPIO_ADDR_BASE+0x34, 0x00000000);
    // high level active
    //`CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h0c, 3'd0, 4'd2, 32'h0000_0000);
    wr(GPIO_ADDR_BASE+0x0c, 0x00000000);
    //`CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h38, 3'd0, 4'd2, 32'h0000_0000);
    wr(GPIO_ADDR_BASE+0x38, 0x00000000);
    // GPIO inerrupt enable
    //`CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h10, 3'd0, 4'd2, 32'hffff_f1fc);
    wr(GPIO_ADDR_BASE+0x10, 0xfffff1fc);
    //`CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h3c, 3'd0, 4'd2, 32'h0000_ffff);
    wr(GPIO_ADDR_BASE+0x3c, 0x0000ffff);
    // 0 :level trigger 1: single edge trigger
    //`CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h20, 3'd0, 4'd2, 32'hffff_f1fc);
    wr(GPIO_ADDR_BASE+0x20, 0xfffff1fc);
    //`CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h4c, 3'd0, 4'd2, 32'h0000_ffff);
    wr(GPIO_ADDR_BASE+0x4c, 0x0000ffff);
    //`CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h18, 3'd0, 4'd2, 32'hffff_f1fc);
    wr(GPIO_ADDR_BASE+0x18, 0xfffff1fc);
    //`CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h44, 3'd0, 4'd2, 32'h0000_ffff);
    wr(GPIO_ADDR_BASE+0x44, 0x0000ffff);

    //repeat(500)@(posedge `CHIP_CLK48M);
    delayns(50);  

    //wait(`GPIO_INTR);
    while(!GPIO_INTR);
    GPIO_INTR = 0;

    //`CHIP_CPU_BFM.ahbrd( `GPIO_ADDR_BASE+32'h14, 3'd0, 4'd2, rdata, 1, 32'hffff_f1fc, 1'b0);
    rd_compare(GPIO_ADDR_BASE+0x14, rdata, 0xfffff1fc);
    //`CHIP_CPU_BFM.ahbrd( `GPIO_ADDR_BASE+32'h40, 3'd0, 4'd2, rdata, 1, 32'h0000_ffff, 1'b0);
    rd_compare(GPIO_ADDR_BASE+0x40, rdata, 0x0000ffff);
    // clear
    //`CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h14, 3'd0, 4'd2, 32'hffff_f1fc);
    wr(GPIO_ADDR_BASE+0x14, 0xfffff1fc);
    //`CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h40, 3'd0, 4'd2, 32'h0000_ffff);
    wr(GPIO_ADDR_BASE+0x40, 0x0000ffff);
    //wait(~`GPIO_INTR);
    //`CHIP_CPU_BFM.ahbrd( `GPIO_ADDR_BASE+32'h14, 3'd0, 4'd2, rdata, 1, 32'h0000_0000, 1'b0);
    rd_compare(GPIO_ADDR_BASE+0x14, rdata, 0x00000000);
    //`CHIP_CPU_BFM.ahbrd( `GPIO_ADDR_BASE+32'h40, 3'd0, 4'd2, rdata, 1, 32'h0000_0000, 1'b0);
    rd_compare(GPIO_ADDR_BASE+0x40, rdata, 0x00000000);
    // disable interrupt 
    //`CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h10, 3'd0, 4'd2, 32'h0000_0000);
    //`CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h3c, 3'd0, 4'd2, 32'h0000_0000);
    //`CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h18, 3'd0, 4'd2, 32'h0000_0000);
    wr(GPIO_ADDR_BASE+0x18, 0x00000000);
    //`CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h44, 3'd0, 4'd2, 32'h0000_0000);
    wr(GPIO_ADDR_BASE+0x44, 0x00000000);

    //repeat(5000)@(posedge `CHIP_CLK48M);    
    delayns(100);
     
    // low leve active
    //$display("===== GPIO single edge trigger ,active low =====");
    //force topsys.chip.GPIO[47:0] = 48'h0000_0000_0000;

    // GPIO test
    //`CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h08, 3'd0, 4'd2, 32'h0000_0000);
    wr(GPIO_ADDR_BASE+0x08, 0x00000000);
    //`CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h34, 3'd0, 4'd2, 32'h0000_0000);
    wr(GPIO_ADDR_BASE+0x34, 0x00000000);
    // high level active
    //`CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h0c, 3'd0, 4'd2, 32'hffff_ffff);
    wr(GPIO_ADDR_BASE+0x0c, 0xffffffff);
    //`CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h38, 3'd0, 4'd2, 32'h0000_ffff);
    wr(GPIO_ADDR_BASE+0x38, 0x0000ffff);
    // GPIO inerrupt enable
    //`CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h10, 3'd0, 4'd2, 32'hffff_f1fc);
    wr(GPIO_ADDR_BASE+0x10, 0xfffff1fc);
    //`CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h3c, 3'd0, 4'd2, 32'h0000_ffff);
    wr(GPIO_ADDR_BASE+0x3c, 0x0000ffff);
    // 0 :level trigger 1: single edge trigger
    //`CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h20, 3'd0, 4'd2, 32'hffff_f1fc);
    wr(GPIO_ADDR_BASE+0x20, 0xfffff1fc);
    //`CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h4c, 3'd0, 4'd2, 32'h0000_ffff);
    wr(GPIO_ADDR_BASE+0x4c, 0x0000ffff);
    //`CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h18, 3'd0, 4'd2, 32'hffff_f1fc);
    wr(GPIO_ADDR_BASE+0x18, 0xfffff1fc);
    //`CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h44, 3'd0, 4'd2, 32'h0000_ffff);
    wr(GPIO_ADDR_BASE+0x44, 0x0000ffff);

    //repeat(500)@(posedge `CHIP_CLK48M);
    delayns(50);

    //wait(`GPIO_INTR);
    while(!GPIO_INTR);
    GPIO_INTR = 0;

    //`CHIP_CPU_BFM.ahbrd( `GPIO_ADDR_BASE+32'h14, 3'd0, 4'd2, rdata, 1, 32'hffff_f1fc, 1'b0);
    rd_compare(GPIO_ADDR_BASE+0x14, rdata, 0xfffff1fc);
    //`CHIP_CPU_BFM.ahbrd( `GPIO_ADDR_BASE+32'h40, 3'd0, 4'd2, rdata, 1, 32'h0000_ffff, 1'b0);
    rd_compare(GPIO_ADDR_BASE+0x40, rdata, 0x0000ffff);
    // clear
    //`CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h14, 3'd0, 4'd2, 32'hffff_f1fc);
    wr(GPIO_ADDR_BASE+0x14, 0xfffff1fc);
    //`CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h40, 3'd0, 4'd2, 32'h0000_ffff);
    wr(GPIO_ADDR_BASE+0x40, 0x0000ffff);
    //wait(~`GPIO_INTR);
    //`CHIP_CPU_BFM.ahbrd( `GPIO_ADDR_BASE+32'h14, 3'd0, 4'd2, rdata, 1, 32'h0000_0000, 1'b0);
    rd_compare(GPIO_ADDR_BASE+0x14, rdata, 0x00000000);
    //`CHIP_CPU_BFM.ahbrd( `GPIO_ADDR_BASE+32'h40, 3'd0, 4'd2, rdata, 1, 32'h0000_0000, 1'b0);
    rd_compare(GPIO_ADDR_BASE+0x40, rdata, 0x00000000);

    //`CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h10, 3'd0, 4'd2, 32'h0000_0000);
    //`CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h3c, 3'd0, 4'd2, 32'h0000_0000);
    //`CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h18, 3'd0, 4'd2, 32'h0000_0000);
    wr(GPIO_ADDR_BASE+0x18, 0x00000000);
    //`CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h44, 3'd0, 4'd2, 32'h0000_0000);
    wr(GPIO_ADDR_BASE+0x44, 0x00000000);

    //repeat(1000)@(posedge `CHIP_CLK48M);
    delayns(100);

    // level edge trigger
    //$display("===== GPIO level edge trigger ,active high =====");
    // GPIO test
    //`CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h08, 3'd0, 4'd2, 32'h0000_0000);
    wr(GPIO_ADDR_BASE+0x08, 0x00000000);
    //`CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h34, 3'd0, 4'd2, 32'h0000_0000);
    wr(GPIO_ADDR_BASE+0x34, 0x00000000);
    // high level active
    //`CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h0c, 3'd0, 4'd2, 32'h0000_0000);
    wr(GPIO_ADDR_BASE+0x0c, 0x00000000);
    //`CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h38, 3'd0, 4'd2, 32'h0000_0000);
    wr(GPIO_ADDR_BASE+0x38, 0x00000000);
    // GPIO inerrupt enable
    //`CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h10, 3'd0, 4'd2, 32'hffff_f1fc);
    wr(GPIO_ADDR_BASE+0x10, 0xfffff1fc);
    //`CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h3c, 3'd0, 4'd2, 32'h0000_ffff);
    wr(GPIO_ADDR_BASE+0x3c, 0x0000ffff);
    // 0 :level trigger 1: single edge trigger
    //`CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h20, 3'd0, 4'd2, 32'h0000_0000);
    wr(GPIO_ADDR_BASE+0x20, 0x00000000);
    //`CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h4c, 3'd0, 4'd2, 32'h0000_0000);
    wr(GPIO_ADDR_BASE+0x4c, 0x00000000);
    //`CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h18, 3'd0, 4'd2, 32'hffff_f1fc);
    wr(GPIO_ADDR_BASE+0x18, 0xfffff1fc);
    //`CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h44, 3'd0, 4'd2, 32'h0000_ffff);
    wr(GPIO_ADDR_BASE+0x44, 0x0000ffff);

    //repeat(50)@(posedge `CHIP_CLK48M);
    delayns(50);
    
    //force topsys.chip.GPIO[47:0] = 48'hffff_ffff_f1fc;
    //repeat(1)@(posedge `CHIP_CLK48M);    
    //force topsys.chip.GPIO[47:0] = 48'h0000_0000_0000;

    //repeat(100)@(posedge `CHIP_CLK48M);    
    delayns(10);

    //wait(`GPIO_INTR);
    while(!GPIO_INTR);
    GPIO_INTR = 0;

    //`CHIP_CPU_BFM.ahbrd( `GPIO_ADDR_BASE+32'h14, 3'd0, 4'd2, rdata, 1, 32'hffff_f1fc, 1'b0);
    rd_compare(GPIO_ADDR_BASE+0x14, rdata, 0xfffff1fc);
    //`CHIP_CPU_BFM.ahbrd( `GPIO_ADDR_BASE+32'h40, 3'd0, 4'd2, rdata, 1, 32'h0000_ffff, 1'b0);
    rd_compare(GPIO_ADDR_BASE+0x40, rdata, 0x0000ffff);
    // clear
    //`CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h14, 3'd0, 4'd2, 32'hffff_f1fc);
    wr(GPIO_ADDR_BASE+0x14, 0xfffff1fc);
    //`CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h40, 3'd0, 4'd2, 32'h0000_ffff);
    wr(GPIO_ADDR_BASE+0x40, 0x0000ffff);
    //wait(~`GPIO_INTR);
    //`CHIP_CPU_BFM.ahbrd( `GPIO_ADDR_BASE+32'h14, 3'd0, 4'd2, rdata, 1, 32'h0000_0000, 1'b0);
    rd_compare(GPIO_ADDR_BASE+0x14, rdata, 0x00000000);
    //`CHIP_CPU_BFM.ahbrd( `GPIO_ADDR_BASE+32'h40, 3'd0, 4'd2, rdata, 1, 32'h0000_0000, 1'b0);
    rd_compare(GPIO_ADDR_BASE+0x40, rdata, 0x00000000);
    // disable interrupt 
    //`CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h10, 3'd0, 4'd2, 32'h0000_0000);
    //`CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h3c, 3'd0, 4'd2, 32'h0000_0000);
    //`CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h18, 3'd0, 4'd2, 32'h0000_0000);
    wr(GPIO_ADDR_BASE+0x18, 0x00000000);
    //`CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h44, 3'd0, 4'd2, 32'h0000_0000);
    wr(GPIO_ADDR_BASE+0x44, 0x00000000);

    //repeat(100)@(posedge `CHIP_CLK48M);
    delayns(100);
    //force topsys.chip.GPIO[47:0] = 48'hffff_ffff_f1fc;

    // level edge trigger , low active
    //$display("===== GPIO level edge trigger ,active low =====");
    // clear
    //`CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h14, 3'd0, 4'd2, 32'hffff_f1fc);
    wr(GPIO_ADDR_BASE+0x14, 0xfffff1fc);
    //`CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h40, 3'd0, 4'd2, 32'h0000_ffff);
    wr(GPIO_ADDR_BASE+0x40, 0x0000ffff);
    // GPIO test
    //`CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h08, 3'd0, 4'd2, 32'h0000_0000);
    wr(GPIO_ADDR_BASE+0x08, 0x00000000);
    //`CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h34, 3'd0, 4'd2, 32'h0000_0000);
    wr(GPIO_ADDR_BASE+0x34, 0x00000000);
    // high level active
    //`CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h0c, 3'd0, 4'd2, 32'hffff_f1fc);
    wr(GPIO_ADDR_BASE+0x0c, 0xfffff1fc);
    //`CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h38, 3'd0, 4'd2, 32'h0000_ffff);
    wr(GPIO_ADDR_BASE+0x38, 0x0000ffff);
    // 0 :level trigger 1: single edge trigger
    //`CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h20, 3'd0, 4'd2, 32'h0000_0000);
    wr(GPIO_ADDR_BASE+0x20, 0x00000000);
    //`CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h4c, 3'd0, 4'd2, 32'h0000_0000);
    wr(GPIO_ADDR_BASE+0x4c, 0x00000000);
    //`CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h18, 3'd0, 4'd2, 32'hffff_f1fc);
    wr(GPIO_ADDR_BASE+0x18, 0xfffff1fc);
    //`CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h44, 3'd0, 4'd2, 32'h0000_ffff);
    wr(GPIO_ADDR_BASE+0x44, 0x0000ffff);
    // GPIO inerrupt enable
    //`CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h10, 3'd0, 4'd2, 32'hffff_f1fc);
    wr(GPIO_ADDR_BASE+0x10, 0xfffff1fc);
    //`CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h3c, 3'd0, 4'd2, 32'h0000_ffff);
    wr(GPIO_ADDR_BASE+0x3c, 0x0000ffff);

    //repeat(50)@(posedge `CHIP_CLK48M);    
    delayns(50);

    //force topsys.chip.GPIO[47:0] = 48'h0000_0000_0000;
    //repeat(1)@(posedge `CHIP_CLK48M);    
    //force topsys.chip.GPIO[47:0] = 48'hffff_ffff_f1fc;

    //repeat(100)@(posedge `CHIP_CLK48M);    

    //wait(`GPIO_INTR);
    while(!GPIO_INTR);
    GPIO_INTR = 0;

    //`CHIP_CPU_BFM.ahbrd( `GPIO_ADDR_BASE+32'h14, 3'd0, 4'd2, rdata, 1, 32'hffff_f1fc, 1'b0);
    rd_compare(GPIO_ADDR_BASE+0x14, rdata, 0xfffff1fc);
    //`CHIP_CPU_BFM.ahbrd( `GPIO_ADDR_BASE+32'h40, 3'd0, 4'd2, rdata, 1, 32'h0000_ffff, 1'b0);
    rd_compare(GPIO_ADDR_BASE+0x40, rdata, 0x0000ffff);
    // clear
    //`CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h14, 3'd0, 4'd2, 32'hffff_f1fc);
    wr(GPIO_ADDR_BASE+0x14, 0xfffff1fc);
    //`CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h40, 3'd0, 4'd2, 32'h0000_ffff);
    wr(GPIO_ADDR_BASE+0x40, 0x0000ffff);
    //wait(~`GPIO_INTR);
    //`CHIP_CPU_BFM.ahbrd( `GPIO_ADDR_BASE+32'h14, 3'd0, 4'd2, rdata, 1, 32'h0000_0000, 1'b0);
    rd_compare(GPIO_ADDR_BASE+0x14, rdata, 0x00000000);
    //`CHIP_CPU_BFM.ahbrd( `GPIO_ADDR_BASE+32'h40, 3'd0, 4'd2, rdata, 1, 32'h0000_0000, 1'b0);
    rd_compare(GPIO_ADDR_BASE+0x40, rdata, 0x00000000);
    // disable interrupt 
    //`CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h10, 3'd0, 4'd2, 32'h0000_0000);
    //`CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h3c, 3'd0, 4'd2, 32'h0000_0000);


    //repeat(5000)@(posedge `CHIP_CLK48M);    
    //$finish;

    return 0;
}
