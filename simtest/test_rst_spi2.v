
`define SPIDATA 32'h1234
`define SPIDATA_2 32'habcd

reg [1:0] 	int_en = 2'b11;
reg		dual_bank = 1'b0;
reg		redu_en=1'b0;
reg [31:0]	rdata;
reg [31:0]	data;
reg [31:0]	addr=32'h8000_6000; 
integer		i;

wire spim_clk ;//= topsys.chip.GPIO[8];
wire spim_cs  ;//= topsys.chip.GPIO[9];
wire spim_d   ;//= topsys.chip.GPIO[10];
reg [31:0] spi_data = 0;

reg [5:0] spim_cnt = 0;

// TX byte swap
wire tx_byte_swap = 0;


initial begin
    $display("@%t, %m MSG: ****** test name: init_test_CPU_BFM", $time);
    wait (`TB_INIT_DONE);
    $display("@%d, %m MSG: TB_INIT_DONEROM", $time);

    @(posedge `CHIP_CLK48M);
 `ifdef SPI2_PORT_8
  force spim_clk = topsys.chip.GPIO[8];
  force spim_cs  = topsys.chip.GPIO[9];
  force spim_d   = topsys.chip.GPIO[10];
  set_port_spim2_8();
 `else
  force spim_clk = topsys.chip.GPIO[20];
  force spim_cs  = topsys.chip.GPIO[21];
  force spim_d   = topsys.chip.GPIO[22];
  set_port_spim2_20();
  `endif

  `CHIP_CPU_BFM.ahbwr(`SYSRAM_ADDR_BASE+32'h00, 3'd0, 4'd2, `SPIDATA);
  `CHIP_CPU_BFM.ahbwr(`SYSRAM_ADDR_BASE+32'h04, 3'd0, 4'd2, 32'h00);

  // SPI2 master setting 
  // program cpha/cpol == 3'b00 , cs = 2'b11
  `CHIP_CPU_BFM.ahbwr(`SPI2_ADDR_BASE+32'h04, 3'd0, 4'd2, 32'h0100_0400); 
  // program write starting address
  `CHIP_CPU_BFM.ahbwr(`SPI2_ADDR_BASE+32'h0c, 3'd0, 4'd2, `SYSRAM_ADDR_BASE+32'h1000); 
  // program read starting address
  `CHIP_CPU_BFM.ahbwr(`SPI2_ADDR_BASE+32'h10, 3'd0, 4'd2, `SYSRAM_ADDR_BASE); 
  // program interrupt enable
  `CHIP_CPU_BFM.ahbwr(`SPI2_ADDR_BASE+32'h00, 3'd0, 4'd2, 32'h003); 

  //enable SPI write_only mode (op_mode =0,swap=1, w=1byte)
  `CHIP_CPU_BFM.ahbwr(`SPI2_ADDR_BASE+32'h08, 3'd0, 4'd2, 32'h0100_0001 | tx_byte_swap <<18); 

   topsys.s_spi_bfm.s_spi_bfm[2].genblk1.s_spi_bfm.spi_s_wr(2 , tx_byte_swap, `SPIDATA);
  // wait interrupt
    wait(`SPI2_M_INTR);
 // wait(topsys.chip.core_domain0.u2_spi_m_top_wrap.spi_m_top.spi_m.spi_m_int);  
  
  `CHIP_CPU_BFM.ahbwr(`SPI2_ADDR_BASE+32'h00, 3'd0, 4'd2, 32'h1_0001); 
  // clear interrupt
//   wait(topsys.chip.core_domain0.u0_spi_m_top_wrap.spi_m_top.spi_m_int);
   wait(~`SPI2_M_INTR);

  force topsys.s_spi_bfm.s_spi_bfm[2].genblk1.s_spi_bfm.spi_s_wr.count[10:0] = 11'd0;
  release topsys.s_spi_bfm.s_spi_bfm[2].genblk1.s_spi_bfm.spi_s_wr.count[10:0];

    repeat(50)@(posedge `CHIP_CLK48M);    
  // reset spi0
  `CHIP_CPU_BFM.ahbwr(`CKGEN_SWRST_REG, 3'd0, 4'd2, 32'hffff_dfff); 
    repeat(500)@(posedge `CHIP_CLK48M);    
  `CHIP_CPU_BFM.ahbwr(`CKGEN_SWRST_REG, 3'd0, 4'd2, 32'hffff_ffff); 


  `CHIP_CPU_BFM.ahbwr(`SYSRAM_ADDR_BASE+32'h00, 3'd0, 4'd2, `SPIDATA_2);
  `CHIP_CPU_BFM.ahbwr(`SYSRAM_ADDR_BASE+32'h04, 3'd0, 4'd2, 32'h00);

  // SPI2 master setting 
  // program cpha/cpol == 3'b00 , cs = 2'b11
  `CHIP_CPU_BFM.ahbwr(`SPI2_ADDR_BASE+32'h04, 3'd0, 4'd2, 32'h0100_0400); 
  // program write starting address
  `CHIP_CPU_BFM.ahbwr(`SPI2_ADDR_BASE+32'h0c, 3'd0, 4'd2, `SYSRAM_ADDR_BASE+32'h1000); 
  // program read starting address
  `CHIP_CPU_BFM.ahbwr(`SPI2_ADDR_BASE+32'h10, 3'd0, 4'd2, `SYSRAM_ADDR_BASE); 
  // program interrupt enable
  `CHIP_CPU_BFM.ahbwr(`SPI2_ADDR_BASE+32'h00, 3'd0, 4'd2, 32'h003); 

  //enable SPI write_only mode (op_mode =0,swap=1, w=1byte)
  `CHIP_CPU_BFM.ahbwr(`SPI2_ADDR_BASE+32'h08, 3'd0, 4'd2, 32'h0100_0001 | tx_byte_swap <<18); 

   topsys.s_spi_bfm.s_spi_bfm[2].genblk1.s_spi_bfm.spi_s_wr(2 , tx_byte_swap, `SPIDATA_2);
  // wait interrupt
    wait(`SPI2_M_INTR);
 // wait(topsys.chip.core_domain0.u2_spi_m_top_wrap.spi_m_top.spi_m.spi_m_int);  
  
  `CHIP_CPU_BFM.ahbwr(`SPI2_ADDR_BASE+32'h00, 3'd0, 4'd2, 32'h1_0001); 
  // clear interrupt
//   wait(topsys.chip.core_domain0.u0_spi_m_top_wrap.spi_m_top.spi_m_int);
   wait(~`SPI2_M_INTR);

    repeat(5000)@(posedge `CHIP_CLK48M);    
    $finish;
end

