
reg [1:0] 	int_en = 2'b11;
reg		dual_bank = 1'b0;
reg		redu_en=1'b0;
reg [31:0]	rdata;
reg [31:0]	data;
reg [31:0]	addr=32'h8000_6000; 
integer		i;

//wire dmic_clk = topsys.chip.GPIO[0];
//reg  dmic_din = 0;
//
//always @(posedge dmic_clk) begin
// //#2 dmic_din = $random & 1'b1;
// #2 dmic_din = dmic_din ^ 1'b1;
// //#1 dmic_din = ~dmic_din & 1'b1;
//end


initial begin
    $display("@%t, %m MSG: ****** test name: init_test_CPU_BFM", $time);
    wait (`TB_INIT_DONE);
    $display("@%d, %m MSG: TB_INIT_DONEROM", $time);

    @(posedge `CHIP_CLK48M);

  set_port_dmic0();

//  force topsys.chip.core_domain0.dmic_top_wrap.dmic_din = dmic_din;
//  force topsys.chip.GPIO[1] = dmic_din;
  // DMIC setting 
  //  interrupt enable
  `CHIP_CPU_BFM.ahbwr(`DMIC_ADDR_BASE, 3'd0, 4'd2, 32'h0_0001); 
  // lr_avail = left_channel
  `CHIP_CPU_BFM.ahbwr(`DMIC_ADDR_BASE+32'h04, 3'd0, 4'd2, 32'h0); 
  // dma_byte_num
  `CHIP_CPU_BFM.ahbwr(`DMIC_ADDR_BASE+32'h08, 3'd0, 4'd2, 32'hc); 
  // dma_start_addr
  `CHIP_CPU_BFM.ahbwr(`DMIC_ADDR_BASE+32'h0c, 3'd0, 4'd2, `SYSRAM_ADDR_BASE); 
  // dma_end_addr
  `CHIP_CPU_BFM.ahbwr(`DMIC_ADDR_BASE+32'h10, 3'd0, 4'd2, `SYSRAM_ADDR_BASE+32'h50); 

  // enable dmic
  `CHIP_CPU_BFM.ahbwr(`DMIC_ADDR_BASE+32'h14, 3'd0, 4'd2, 32'h1); 
    
  topsys.dmic_bfm.dmic_l(1'b0);

   wait(topsys.chip.core_domain0.dmic_top_wrap.dmic_int);
  // disable dmic
  `CHIP_CPU_BFM.ahbwr(`DMIC_ADDR_BASE+32'h14, 3'd0, 4'd2, 32'h0); 

    repeat(100)@(posedge `CHIP_CLK48M);    
    `CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h00, 3'd0, 4'd2, rdata, 1, 32'hc0895e81, 1'b0);
    `CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h04, 3'd0, 4'd2, rdata, 1, 32'h8484d609, 1'b0);
    `CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h08, 3'd0, 4'd2, rdata, 1, 32'hb1f05663, 1'b0);
    `CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h0c, 3'd0, 4'd2, rdata, 1, 32'h06b97b0d, 1'b0);
  //  `CHIP_CPU_BFM.ahbrd( 32'h6_0010, 3'd0, 4'd2, rdata, 1, 32'hb5d1_ef7e, 1'b0);
 //   `CHIP_CPU_BFM.ahbrd( 32'h6_0014, 3'd0, 4'd2, rdata, 1, 32'h1dff_e4af, 1'b0);
 //   `CHIP_CPU_BFM.ahbrd( 32'h6_0018, 3'd0, 4'd2, rdata, 1, 32'haadc_9beb, 1'b0);
 //   `CHIP_CPU_BFM.ahbrd( 32'h6_001c, 3'd0, 4'd2, rdata, 1, 32'h404_c160, 1'b0);
 //   `CHIP_CPU_BFM.ahbrd( 32'h6_0020, 3'd0, 4'd2, rdata, 1, 32'h6a09_3dbc, 1'b0);
 //   `CHIP_CPU_BFM.ahbrd( 32'h6_0024, 3'd0, 4'd2, rdata, 1, 32'h3bb5_69a4, 1'b0);
 //
   //disable interrupt
  `CHIP_CPU_BFM.ahbwr(`DMIC_ADDR_BASE, 3'd0, 4'd2, 32'h0_0100); 

   wait(~topsys.chip.core_domain0.dmic_top_wrap.dmic_int);

    repeat(5000)@(posedge `CHIP_CLK48M);    
    $finish;
end

