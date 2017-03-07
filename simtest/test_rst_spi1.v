
`define SPIDATA 32'h1234
`define SPIDATA_1 32'habcd

reg [1:0] 	int_en = 2'b11;
reg		dual_bank = 1'b0;
reg		redu_en=1'b0;
reg [31:0]	rdata;
reg [31:0]	data;
reg [31:0]	addr=32'h8000_6000; 
integer		i;

wire spim_clk ;//= topsys.chip.GPIO[4];
wire spim_cs  ;//= topsys.chip.GPIO[5];
wire spim_d   ;//= topsys.chip.GPIO[6];
reg [31:0] spi_data = 0;

reg [5:0] spim_cnt = 0;

// TX byte swap
wire tx_byte_swap = 0;

//always @(posedge spim_clk) begin
//   if(spim_cs) 
//     spim_cnt <= 6'h0;
//   else if(~spim_cs) 
//     spim_cnt <= spim_cnt + 1;
//    else 
//     spim_cnt <= spim_cnt; 
// end
//
//
//always @(posedge spim_clk) begin
//  if(~spim_cs && ~spim_cnt[5]) begin
//    case(spim_cnt[4:0]) 
//      5'd0: spi_data[7] <= spim_d;
//      5'd1: spi_data[6] <= spim_d;
//      5'd2: spi_data[5] <= spim_d;
//      5'd3: spi_data[4] <= spim_d;
//      5'd4: spi_data[3] <= spim_d;
//      5'd5: spi_data[2] <= spim_d;
//      5'd6: spi_data[1] <= spim_d;
//      5'd7: spi_data[0] <= spim_d;
//      5'd8: spi_data[15] <= spim_d;
//      5'd9: spi_data[14] <= spim_d;
//      5'd10: spi_data[13] <= spim_d;
//      5'd11: spi_data[12] <= spim_d;
//      5'd12: spi_data[11] <= spim_d;
//      5'd13: spi_data[10] <= spim_d;
//      5'd14: spi_data[9] <= spim_d;
//      5'd15: spi_data[8] <= spim_d;
//      5'd16: spi_data[23] <= spim_d;
//      5'd17: spi_data[22] <= spim_d;
//      5'd18: spi_data[21] <= spim_d;
//      5'd19: spi_data[20] <= spim_d;
//      5'd20: spi_data[19] <= spim_d;
//      5'd21: spi_data[18] <= spim_d;
//      5'd22: spi_data[17] <= spim_d;
//      5'd23: spi_data[16] <= spim_d;
//      5'd24: spi_data[31] <= spim_d;
//      5'd25: spi_data[30] <= spim_d;
//      5'd26: spi_data[29] <= spim_d;
//      5'd27: spi_data[28] <= spim_d;
//      5'd28: spi_data[27] <= spim_d;
//      5'd29: spi_data[26] <= spim_d;
//      5'd30: spi_data[25] <= spim_d;
//      5'd31: spi_data[24] <= spim_d;
//      default spi_data <= 32'h0;
//     endcase
//  end
//
//end


initial begin
    $display("@%t, %m MSG: ****** test name: init_test_CPU_BFM", $time);
    wait (`TB_INIT_DONE);
    $display("@%d, %m MSG: TB_INIT_DONEROM", $time);

    @(posedge `CHIP_CLK48M);
 `ifdef SPI1_MODE_4
  force spim_clk = topsys.chip.GPIO[4];
  force spim_cs  = topsys.chip.GPIO[5];
  force spim_d   = topsys.chip.GPIO[6];
  set_port_spim1_4();
  `elsif SPI1_MODE_16
  force spim_clk = topsys.chip.GPIO[16];
  force spim_cs  = topsys.chip.GPIO[17];
  force spim_d   = topsys.chip.GPIO[18];
  set_port_spim1_16();
  `else
  force spim_clk = topsys.chip.GPIO[28];
  force spim_cs  = topsys.chip.GPIO[29];
  force spim_d   = topsys.chip.GPIO[30];
  set_port_spim1_28();
  `endif  

  `CHIP_CPU_BFM.ahbwr(`SYSRAM_ADDR_BASE+32'h00, 3'd0, 4'd2, `SPIDATA);
  `CHIP_CPU_BFM.ahbwr(`SYSRAM_ADDR_BASE+32'h04, 3'd0, 4'd2, 32'h00);

  // SPI1 master setting 
  // program cpha/cpol == 3'b00 , cs = 2'b11
  `CHIP_CPU_BFM.ahbwr(`SPI1_ADDR_BASE+32'h04, 3'd0, 4'd2, 32'h0100_0400); 
  // program write starting address
  `CHIP_CPU_BFM.ahbwr(`SPI1_ADDR_BASE+32'h0c, 3'd0, 4'd2, `SYSRAM_ADDR_BASE+32'h1000); 
  // program read starting address
  `CHIP_CPU_BFM.ahbwr(`SPI1_ADDR_BASE+32'h10, 3'd0, 4'd2, `SYSRAM_ADDR_BASE); 
  // program interrupt enable
  `CHIP_CPU_BFM.ahbwr(`SPI1_ADDR_BASE+32'h00, 3'd0, 4'd2, 32'h003); 

  //enable SPI write_only mode (op_mode =0,swap=1, w=1byte)
  `CHIP_CPU_BFM.ahbwr(`SPI1_ADDR_BASE+32'h08, 3'd0, 4'd2, 32'h0100_0001 | tx_byte_swap <<18); 

   topsys.s_spi_bfm.s_spi_bfm[1].genblk1.s_spi_bfm.spi_s_wr(2 , tx_byte_swap, `SPIDATA);
  // wait interrupt
  wait(`SPI1_M_INTR);
  
  `CHIP_CPU_BFM.ahbwr(`SPI1_ADDR_BASE+32'h00, 3'd0, 4'd2, 32'h1_0001); 
  // clear interrupt
//   wait(topsys.chip.core_domain0.u0_spi_m_top_wrap.spi_m_top.spi_m_int);
  wait(~`SPI1_M_INTR);

  force topsys.s_spi_bfm.s_spi_bfm[1].genblk1.s_spi_bfm.spi_s_wr.count[10:0] = 11'd0;
  release topsys.s_spi_bfm.s_spi_bfm[1].genblk1.s_spi_bfm.spi_s_wr.count[10:0];

    repeat(50)@(posedge `CHIP_CLK48M);    
  // reset spi0
  `CHIP_CPU_BFM.ahbwr(`CKGEN_SWRST_REG, 3'd0, 4'd2, 32'hffff_bfff); 
    repeat(500)@(posedge `CHIP_CLK48M);    
  `CHIP_CPU_BFM.ahbwr(`CKGEN_SWRST_REG, 3'd0, 4'd2, 32'hffff_ffff); 


  `CHIP_CPU_BFM.ahbwr(`SYSRAM_ADDR_BASE+32'h00, 3'd0, 4'd2, `SPIDATA_1);
  `CHIP_CPU_BFM.ahbwr(`SYSRAM_ADDR_BASE+32'h04, 3'd0, 4'd2, 32'h00);

  // SPI1 master setting 
  // program cpha/cpol == 3'b00 , cs = 2'b11
  `CHIP_CPU_BFM.ahbwr(`SPI1_ADDR_BASE+32'h04, 3'd0, 4'd2, 32'h0100_0400); 
  // program write starting address
  `CHIP_CPU_BFM.ahbwr(`SPI1_ADDR_BASE+32'h0c, 3'd0, 4'd2, `SYSRAM_ADDR_BASE+32'h1000); 
  // program read starting address
  `CHIP_CPU_BFM.ahbwr(`SPI1_ADDR_BASE+32'h10, 3'd0, 4'd2, `SYSRAM_ADDR_BASE); 
  // program interrupt enable
  `CHIP_CPU_BFM.ahbwr(`SPI1_ADDR_BASE+32'h00, 3'd0, 4'd2, 32'h003); 

  //enable SPI write_only mode (op_mode =0,swap=1, w=1byte)
  `CHIP_CPU_BFM.ahbwr(`SPI1_ADDR_BASE+32'h08, 3'd0, 4'd2, 32'h0100_0001 | tx_byte_swap <<18); 

   topsys.s_spi_bfm.s_spi_bfm[1].genblk1.s_spi_bfm.spi_s_wr(2 , tx_byte_swap, `SPIDATA_1);
  // wait interrupt
  wait(`SPI1_M_INTR);
  
  `CHIP_CPU_BFM.ahbwr(`SPI1_ADDR_BASE+32'h00, 3'd0, 4'd2, 32'h1_0001); 
  // clear interrupt
//   wait(topsys.chip.core_domain0.u0_spi_m_top_wrap.spi_m_top.spi_m_int);
  wait(~`SPI1_M_INTR);

    repeat(5000)@(posedge `CHIP_CLK48M);    
    $finish;
end

