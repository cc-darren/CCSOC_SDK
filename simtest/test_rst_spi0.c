#include "stdint.h"
#include "addr_map.h"

#define SPI0_MODE_0

#define SPIDATA     0x1234
#define SPIDATA_0   0xabcd

//reg [1:0] 	int_en = 2'b11;
//reg		dual_bank = 1'b0;
//reg		redu_en=1'b0;
//reg [31:0]	rdata;
//reg [31:0]	data;
//reg [31:0]	addr=32'h8000_6000; 
//integer		i;

//wire spim_clk;// = topsys.chip.GPIO[0];
//wire spim_cs ;// = topsys.chip.GPIO[1];
//wire spim_d  ;// = topsys.chip.GPIO[2];
//reg [31:0] spi_data = 0;

//reg [5:0] spim_cnt = 0;

// TX byte swap
static uint32_t tx_byte_swap = 0;

extern uint32_t SPI0_M_INTR;
extern void set_port_spim0_0(void);
extern void set_port_spim0_12(void);
extern void set_port_spim0_24(void);

//0: success; others: fail
int test_rst_spi0(void) {
    int ret = 0;

    //$display("@%t, %m MSG: ****** test name: init_test_CPU_BFM", $time);
    //wait (`TB_INIT_DONE);
    //$display("@%d, %m MSG: TB_INIT_DONEROM", $time);

    //@(posedge `CHIP_CLK48M);
    delayns(1);
    // spi_m port0
    #ifdef SPI0_MODE_0
    //force spim_clk = topsys.chip.GPIO[0];
    //force spim_cs  = topsys.chip.GPIO[1];
    //force spim_d   = topsys.chip.GPIO[2];
    set_port_spim0_0();
    // spi_m port12
    #elif defined(SPI0_MODE_12)
    //force spim_clk = topsys.chip.GPIO[12];
    //force spim_cs  = topsys.chip.GPIO[13];
    //force spim_d   = topsys.chip.GPIO[14];
    set_port_spim0_12();
    #else
    // spi_m port24
    //force spim_clk = topsys.chip.GPIO[24];
    //force spim_cs  = topsys.chip.GPIO[25];
    //force spim_d   = topsys.chip.GPIO[26];
    set_port_spim0_24();
    #endif

    //`CHIP_CPU_BFM.ahbwr(`SYSRAM_TEST_ADDR_BASE+32'h00, 3'd0, 4'd2, `SPIDATA);
    wr(SYSRAM_TEST_ADDR_BASE+0x00, SPIDATA);
    //`CHIP_CPU_BFM.ahbwr(`SYSRAM_TEST_ADDR_BASE+32'h04, 3'd0, 4'd2, 32'h00);
    wr(SYSRAM_TEST_ADDR_BASE+0x04, 0x00);

    // SPI0 master setting 
    // program cpha/cpol == 3'b00 , cs = 2'b11
    //`CHIP_CPU_BFM.ahbwr(`SPI0_ADDR_BASE+32'h04, 3'd0, 4'd2, 32'h0100_0400);
    wr(SPI0_ADDR_BASE+0x04, 0x01000400);
    // program write starting address
    //`CHIP_CPU_BFM.ahbwr(`SPI0_ADDR_BASE+32'h0c, 3'd0, 4'd2, `SYSRAM_TEST_ADDR_BASE+32'h1000);
    wr(SPI0_ADDR_BASE+0x0c, SYSRAM_TEST_ADDR_BASE+0x1000);
    // program read starting address
    //`CHIP_CPU_BFM.ahbwr(`SPI0_ADDR_BASE+32'h10, 3'd0, 4'd2, `SYSRAM_TEST_ADDR_BASE);
    wr(SPI0_ADDR_BASE+0x10, SYSRAM_TEST_ADDR_BASE);
    // program interrupt enable
    //`CHIP_CPU_BFM.ahbwr(`SPI0_ADDR_BASE+32'h00, 3'd0, 4'd2, 32'h003);
    wr(SPI0_ADDR_BASE+0x00, 0x003);
    //enable SPI write_only mode (op_mode =0,swap=1, w=1byte)
    //`CHIP_CPU_BFM.ahbwr(`SPI0_ADDR_BASE+32'h08, 3'd0, 4'd2, 32'h0100_0001 | tx_byte_swap <<18);
    wr(SPI0_ADDR_BASE+0x08, 0x01000001 | tx_byte_swap <<18);

    //topsys.s_spi_bfm.s_spi_bfm[0].genblk1.s_spi_bfm.spi_s_wr(2 , tx_byte_swap, `SPIDATA);
    // wait interrupt
    //wait(`SPI0_M_INTR);
    while(!SPI0_M_INTR);
    SPI0_M_INTR = 0;
    //wait(topsys.chip.core_domain0.u0_spi_m_top_wrap.spi_m_top.spi_m.spi_m_int);  
    // $display("spi master data = %h", spi_data);

    //`CHIP_CPU_BFM.ahbwr(`SPI0_ADDR_BASE+32'h00, 3'd0, 4'd2, 32'h1_0001);
    //wr(SPI0_ADDR_BASE+0x00, 0x00010001);
    // clear interrupt
    //wait(~`SPI0_M_INTR);

    //force topsys.s_spi_bfm.s_spi_bfm[0].genblk1.s_spi_bfm.spi_s_wr.count[10:0] = 11'd0;
    //release topsys.s_spi_bfm.s_spi_bfm[0].genblk1.s_spi_bfm.spi_s_wr.count[10:0];

    //repeat(50)@(posedge `CHIP_CLK48M);
    delayns(50);
    // reset spi0
    //`CHIP_CPU_BFM.ahbwr(`CKGEN_SWRST_REG, 3'd0, 4'd2, 32'hffff_7fff);
    wr(CKGEN_SWRST_REG, 0xffff7fff);
    //repeat(500)@(posedge `CHIP_CLK48M);    
    delayns(500);
    //`CHIP_CPU_BFM.ahbwr(`CKGEN_SWRST_REG, 3'd0, 4'd2, 32'hffff_ffff);
    wr(CKGEN_SWRST_REG, 0xffffffff);

    //`CHIP_CPU_BFM.ahbwr(`SYSRAM_TEST_ADDR_BASE+32'h00, 3'd0, 4'd2, `SPIDATA_0);
    wr(SYSRAM_TEST_ADDR_BASE+0x00, SPIDATA_0);
    //`CHIP_CPU_BFM.ahbwr(`SYSRAM_TEST_ADDR_BASE+32'h04, 3'd0, 4'd2, 32'h00);
    wr(SYSRAM_TEST_ADDR_BASE+0x04, 0x00);
    // SPI0 master setting 
    // program cpha/cpol == 3'b00 , cs = 2'b11
    //`CHIP_CPU_BFM.ahbwr(`SPI0_ADDR_BASE+32'h04, 3'd0, 4'd2, 32'h0100_0400);
    wr(SPI0_ADDR_BASE+0x04, 0x01000400); 
    // program write starting address
    //`CHIP_CPU_BFM.ahbwr(`SPI0_ADDR_BASE+32'h0c, 3'd0, 4'd2, `SYSRAM_TEST_ADDR_BASE+32'h1000); 
    wr(SPI0_ADDR_BASE+0x0c, SYSRAM_TEST_ADDR_BASE+0x1000); 
    // program read starting address
    //`CHIP_CPU_BFM.ahbwr(`SPI0_ADDR_BASE+32'h10, 3'd0, 4'd2, `SYSRAM_TEST_ADDR_BASE); 
    wr(SPI0_ADDR_BASE+0x10, SYSRAM_TEST_ADDR_BASE); 
    // program interrupt enable
    //`CHIP_CPU_BFM.ahbwr(`SPI0_ADDR_BASE+32'h00, 3'd0, 4'd2, 32'h003); 
    wr(SPI0_ADDR_BASE+0x00, 0x003); 
    //enable SPI write_only mode (op_mode =0,swap=1, w=1byte)
    //`CHIP_CPU_BFM.ahbwr(`SPI0_ADDR_BASE+32'h08, 3'd0, 4'd2, 32'h0100_0001 | tx_byte_swap <<18); 
    wr(SPI0_ADDR_BASE+0x08, 0x01000001 | tx_byte_swap <<18); 

    //topsys.s_spi_bfm.s_spi_bfm[0].genblk1.s_spi_bfm.spi_s_wr(2 , tx_byte_swap, `SPIDATA_0);
    // wait interrupt
    //wait(`SPI0_M_INTR);
    while(!SPI0_M_INTR);
    SPI0_M_INTR = 0;
    //wait(topsys.chip.core_domain0.u0_spi_m_top_wrap.spi_m_top.spi_m.spi_m_int);  
    // $display("spi master data = %h", spi_data);

    //`CHIP_CPU_BFM.ahbwr(`SPI0_ADDR_BASE+32'h00, 3'd0, 4'd2, 32'h1_0001); 
    //wr(SPI0_ADDR_BASE+0x00, 0x00010001); 
    // clear interrupt
    //wait(~`SPI0_M_INTR);

    //repeat(5000)@(posedge `CHIP_CLK48M);    
    //$finish;

    return ret;
}
