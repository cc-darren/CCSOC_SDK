#include "stdint.h"
#include "addr_map.h"

#define SPIDATA     0x1234
#define SPIDATA_2   0xabcd

//reg [1:0] 	int_en = 2'b11;
//reg		dual_bank = 1'b0;
//reg		redu_en=1'b0;
//reg [31:0]	rdata;
//reg [31:0]	data;
//reg [31:0]	addr=32'h8000_6000; 
//integer		i;

//wire spim_clk ;//= topsys.chip.GPIO[8];
//wire spim_cs  ;//= topsys.chip.GPIO[9];
//wire spim_d   ;//= topsys.chip.GPIO[10];
//reg [31:0] spi_data = 0;

//reg [5:0] spim_cnt = 0;

// TX byte swap
static uint32_t tx_byte_swap = 0;

extern uint32_t SPI2_M_INTR;
extern void set_port_spim2_8(void);
extern void set_port_spim2_20(void);

//0: success; others: fail
int test_rst_spi2(void) {
    int ret = 0;

    //$display("@%t, %m MSG: ****** test name: init_test_CPU_BFM", $time);
    //wait (`TB_INIT_DONE);
    //$display("@%d, %m MSG: TB_INIT_DONEROM", $time);

    //@(posedge `CHIP_CLK48M);
    delayns(1);
    #ifdef SPI2_PORT_8
    //force spim_clk = topsys.chip.GPIO[8];
    //force spim_cs  = topsys.chip.GPIO[9];
    //force spim_d   = topsys.chip.GPIO[10];
    set_port_spim2_8();
    #else
    //force spim_clk = topsys.chip.GPIO[20];
    //force spim_cs  = topsys.chip.GPIO[21];
    //force spim_d   = topsys.chip.GPIO[22];
    set_port_spim2_20();
    #endif
    
    //`CHIP_CPU_BFM.ahbwr(`SYSRAM_TEST_ADDR_BASE+32'h00, 3'd0, 4'd2, `SPIDATA);
    wr(SYSRAM_TEST_ADDR_BASE+0x00, SPIDATA);
    //`CHIP_CPU_BFM.ahbwr(`SYSRAM_TEST_ADDR_BASE+32'h04, 3'd0, 4'd2, 32'h00);
    wr(SYSRAM_TEST_ADDR_BASE+0x04, 0x00);
    
    // SPI2 master setting 
    // program cpha/cpol == 3'b00 , cs = 2'b11
    //`CHIP_CPU_BFM.ahbwr(`SPI2_ADDR_BASE+32'h04, 3'd0, 4'd2, 32'h0100_0400); 
    wr(SPI2_ADDR_BASE+0x04, 0x01000400); 
    // program write starting address
    //`CHIP_CPU_BFM.ahbwr(`SPI2_ADDR_BASE+32'h0c, 3'd0, 4'd2, `SYSRAM_TEST_ADDR_BASE+32'h1000); 
    wr(SPI2_ADDR_BASE+0x0c, SYSRAM_TEST_ADDR_BASE+0x1000); 
    // program read starting address
    //`CHIP_CPU_BFM.ahbwr(`SPI2_ADDR_BASE+32'h10, 3'd0, 4'd2, `SYSRAM_TEST_ADDR_BASE); 
    wr(SPI2_ADDR_BASE+0x10, SYSRAM_TEST_ADDR_BASE); 
    // program interrupt enable
    //`CHIP_CPU_BFM.ahbwr(`SPI2_ADDR_BASE+32'h00, 3'd0, 4'd2, 32'h003); 
    wr(SPI2_ADDR_BASE+0x00, 0x003); 
    
    //enable SPI write_only mode (op_mode =0,swap=1, w=1byte)
    //`CHIP_CPU_BFM.ahbwr(`SPI2_ADDR_BASE+32'h08, 3'd0, 4'd2, 32'h0100_0001 | tx_byte_swap <<18); 
	wr(SPI2_ADDR_BASE+0x08, 0x01000001 | tx_byte_swap <<18); 
    
    //topsys.s_spi_bfm.s_spi_bfm[2].genblk1.s_spi_bfm.spi_s_wr(2 , tx_byte_swap, `SPIDATA);
    // wait interrupt
    //wait(`SPI2_M_INTR);
    while(!SPI2_M_INTR);
    SPI2_M_INTR = 0;
    // wait(topsys.chip.core_domain0.u2_spi_m_top_wrap.spi_m_top.spi_m.spi_m_int);  
    
    //`CHIP_CPU_BFM.ahbwr(`SPI2_ADDR_BASE+32'h00, 3'd0, 4'd2, 32'h1_0001); 
    //wr(SPI2_ADDR_BASE+0x00, 0x00010001); 
    // clear interrupt
    //   wait(topsys.chip.core_domain0.u0_spi_m_top_wrap.spi_m_top.spi_m_int);
    //wait(~`SPI2_M_INTR);
    
    //force topsys.s_spi_bfm.s_spi_bfm[2].genblk1.s_spi_bfm.spi_s_wr.count[10:0] = 11'd0;
//    release topsys.s_spi_bfm.s_spi_bfm[2].genblk1.s_spi_bfm.spi_s_wr.count[10:0];
    
    //repeat(50)@(posedge `CHIP_CLK48M);    
    delayns(50);
    // reset spi0
    //`CHIP_CPU_BFM.ahbwr(`CKGEN_SWRST_REG, 3'd0, 4'd2, 32'hffff_dfff); 
    wr(CKGEN_SWRST_REG, 0xffffdfff); 
    //repeat(500)@(posedge `CHIP_CLK48M);    
    delayns(500);
    //`CHIP_CPU_BFM.ahbwr(`CKGEN_SWRST_REG, 3'd0, 4'd2, 32'hffff_ffff); 
    wr(CKGEN_SWRST_REG, 0xffffffff); 

    
    //`CHIP_CPU_BFM.ahbwr(`SYSRAM_TEST_ADDR_BASE+32'h00, 3'd0, 4'd2, `SPIDATA_2);
    wr(SYSRAM_TEST_ADDR_BASE+0x00, SPIDATA_2);
    //`CHIP_CPU_BFM.ahbwr(`SYSRAM_TEST_ADDR_BASE+32'h04, 3'd0, 4'd2, 32'h00);
    wr(SYSRAM_TEST_ADDR_BASE+0x04, 0x00);

    // SPI2 master setting 
    // program cpha/cpol == 3'b00 , cs = 2'b11
    //`CHIP_CPU_BFM.ahbwr(`SPI2_ADDR_BASE+32'h04, 3'd0, 4'd2, 32'h0100_0400); 
    wr(SPI2_ADDR_BASE+0x04, 0x01000400);
    // program write starting address
    //`CHIP_CPU_BFM.ahbwr(`SPI2_ADDR_BASE+32'h0c, 3'd0, 4'd2, `SYSRAM_TEST_ADDR_BASE+32'h1000);
    wr(SPI2_ADDR_BASE+0x0c, SYSRAM_TEST_ADDR_BASE+0x1000);  
    // program read starting address
    //`CHIP_CPU_BFM.ahbwr(`SPI2_ADDR_BASE+32'h10, 3'd0, 4'd2, `SYSRAM_TEST_ADDR_BASE); 
    wr(SPI2_ADDR_BASE+0x10, SYSRAM_TEST_ADDR_BASE); 
    // program interrupt enable
    //`CHIP_CPU_BFM.ahbwr(`SPI2_ADDR_BASE+32'h00, 3'd0, 4'd2, 32'h003); 
    wr(SPI2_ADDR_BASE+0x00, 0x003); 
    
    //enable SPI write_only mode (op_mode =0,swap=1, w=1byte)
    //`CHIP_CPU_BFM.ahbwr(`SPI2_ADDR_BASE+32'h08, 3'd0, 4'd2, 32'h0100_0001 | tx_byte_swap <<18); 
    wr(SPI2_ADDR_BASE+0x08, 0x01000001 | tx_byte_swap <<18); 

    //topsys.s_spi_bfm.s_spi_bfm[2].genblk1.s_spi_bfm.spi_s_wr(2 , tx_byte_swap, `SPIDATA_2);
    // wait interrupt
    //wait(`SPI2_M_INTR);
    while(!SPI2_M_INTR);
    SPI2_M_INTR = 0;
    // wait(topsys.chip.core_domain0.u2_spi_m_top_wrap.spi_m_top.spi_m.spi_m_int);  
    
    //`CHIP_CPU_BFM.ahbwr(`SPI2_ADDR_BASE+32'h00, 3'd0, 4'd2, 32'h1_0001); 
    //wr(SPI2_ADDR_BASE+0x00, 0x00010001); 
    // clear interrupt
    //   wait(topsys.chip.core_domain0.u0_spi_m_top_wrap.spi_m_top.spi_m_int);
    //wait(~`SPI2_M_INTR);
    
    //repeat(5000)@(posedge `CHIP_CLK48M);    
    //$finish;

    return ret;
}
