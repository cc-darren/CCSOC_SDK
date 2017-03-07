
reg [1:0] 	int_en = 2'b11;
reg		dual_bank = 1'b0;
reg		redu_en=1'b0;
reg [31:0]	rdata;
reg [31:0]	data;
reg [31:0]	addr=32'h8000_6000; 
integer		i;

//WDT register
reg [7:0] pre_scale = 8'h1;
reg       cre = 1'b0;  // Chip reset enable
reg       time_load = 1'b1; // timer load
reg       stop = 1'b0;      // 0:disable 1:enable
reg [31:0] intr_load = 32'h0000_0050;  // WDT and Timer loader
reg [31:0] crc_load = 32'h0000_0050; // Chip reset counter loader
reg      int_en = 1'b1;  // interrupt enable
reg      int_clr = 1'b1;  //interrupt clear

initial begin

   // WDT function
    $display("@%t, %m MSG: ****** test name: init_test_CPU_BFM", $time);
    wait (`TB_INIT_DONE);
    $display("@%d, %m MSG: TB_INIT_DONEROM", $time);

    repeat(4) @(posedge `CHIP_CLK48M);
`ifdef ROM_BOOT
  test_done();
`else
    
    `CHIP_CPU_BFM.ahbwr( `WDT_ADDR_BASE+32'h08, 3'd0, 4'd2, intr_load);
    `CHIP_CPU_BFM.ahbwr( `WDT_ADDR_BASE+32'h0c, 3'd0, 4'd2, crc_load);
    `CHIP_CPU_BFM.ahbwr( `WDT_ADDR_BASE+32'h04, 3'd0, 4'd2, 32'h0000_0000 | stop | pre_scale << 8 | cre <<2 | 1'b1 <<2);
    `CHIP_CPU_BFM.ahbwr( `WDT_ADDR_BASE+32'h00, 3'd0, 4'd2, 32'h0000_0000 | int_en);

//    `CHIP_CPU_BFM.ahbrd( `I2S_ADDR_BASE+32'h00, 3'd0, 4'd2, rdata, 1, 32'h17_0109, 1'b0);

   // wait(topsys.chip.core_domain0.watchdog_top_wrap.watchdog_top.watchdog_int);
    wait(`WDT_INTR);

    repeat(4)@(posedge `CHIP_CLK48M);    
    `CHIP_CPU_BFM.ahbwr( `WDT_ADDR_BASE+32'h00, 3'd0, 4'd2, 32'h0000_0000 | int_clr <<16 | 1'b0);

    repeat(4)@(posedge `CHIP_CLK48M);    
    wait(~`WDT_INTR);
    //wait(~topsys.chip.core_domain0.watchdog_top_wrap.watchdog_top.watchdog_int);

 //   repeat(100)@(posedge `CHIP_CLK48M);    
 //   
 //   // Timer function
 //   `CHIP_CPU_BFM.ahbwr( `WDT_ADDR_BASE+32'h04, 3'd0, 4'd2, 32'h0000_0000 | 1'b1 | pre_scale << 8 | cre <<2 | time_load <<1);
 //   `CHIP_CPU_BFM.ahbwr( `WDT_ADDR_BASE+32'h04, 3'd0, 4'd2, 32'h0000_0000 | stop | pre_scale << 8 | cre <<2 | time_load <<1);
 //   `CHIP_CPU_BFM.ahbwr( `WDT_ADDR_BASE+32'h00, 3'd0, 4'd2, 32'h0000_0000 | int_en);
 //   
 //   wait(`WDT_INTR);
 //   //wait(topsys.chip.core_domain0.watchdog_top_wrap.watchdog_top.watchdog_int);

 //   repeat(4)@(posedge `CHIP_CLK48M);    
 //   `CHIP_CPU_BFM.ahbwr( `WDT_ADDR_BASE+32'h00, 3'd0, 4'd2, 32'h0000_0000 | int_clr <<16 | 1'b0);

 //   repeat(4)@(posedge `CHIP_CLK48M);    
 //   wait(~`WDT_INTR);
 //   //wait(~topsys.chip.core_domain0.watchdog_top_wrap.watchdog_top.watchdog_int);
 //  
 //   `CHIP_CPU_BFM.ahbwr( `WDT_ADDR_BASE+32'h04, 3'd0, 4'd2, 32'h0000_0000 | 1'b1 | pre_scale << 8 | cre <<2 | time_load <<1);

    repeat(100000)@(posedge `CHIP_CLK48M);    
    $finish;
`endif
end

