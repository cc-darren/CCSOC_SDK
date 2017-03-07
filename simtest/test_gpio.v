
reg [1:0] 	int_en = 2'b11;
reg		dual_bank = 1'b0;
reg		redu_en=1'b0;
reg [31:0]	rdata;
reg [31:0]	data;
reg [31:0]	addr=32'h8000_6000; 
integer		i;
reg [15:0]      ws_cnt;
reg [31:0]      i2s_tx_data;
reg [5:0]       tx_cnt;

initial begin

    $display("@%t, %m MSG: ****** test name: init_test_CPU_BFM", $time);
    wait (`TB_INIT_DONE);
    $display("@%d, %m MSG: TB_INIT_DONEROM", $time);

    // single edge trigger
    $display("===== GPIO single edge trigger ,active high =====");
    // GPIO test
    // GPIO[31:0]
    `CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h08, 3'd0, 4'd2, 32'h0000_0000); 
    // GPIO[47:32] 
    `CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h34, 3'd0, 4'd2, 32'h0000_0000); 
    // high level active
    `CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h0c, 3'd0, 4'd2, 32'h0000_0000);
    `CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h38, 3'd0, 4'd2, 32'h0000_0000);
    // GPIO inerrupt enable
    `CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h10, 3'd0, 4'd2, 32'hffff_f1fc);
    `CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h3c, 3'd0, 4'd2, 32'h0000_ffff);
    // 0 :level trigger 1: single edge trigger
    `CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h20, 3'd0, 4'd2, 32'hffff_f1fc);
    `CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h4c, 3'd0, 4'd2, 32'h0000_ffff);
    `CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h18, 3'd0, 4'd2, 32'hffff_f1fc);
    `CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h44, 3'd0, 4'd2, 32'h0000_ffff);

    repeat(500)@(posedge `CHIP_CLK48M);    

    wait(`GPIO_INTR);

    `CHIP_CPU_BFM.ahbrd( `GPIO_ADDR_BASE+32'h14, 3'd0, 4'd2, rdata, 1, 32'hffff_f1fc, 1'b0);
    `CHIP_CPU_BFM.ahbrd( `GPIO_ADDR_BASE+32'h40, 3'd0, 4'd2, rdata, 1, 32'h0000_ffff, 1'b0);
    // clear
    `CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h14, 3'd0, 4'd2, 32'hffff_f1fc);
    `CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h40, 3'd0, 4'd2, 32'h0000_ffff);
    wait(~`GPIO_INTR);
    `CHIP_CPU_BFM.ahbrd( `GPIO_ADDR_BASE+32'h14, 3'd0, 4'd2, rdata, 1, 32'h0000_0000, 1'b0);
    `CHIP_CPU_BFM.ahbrd( `GPIO_ADDR_BASE+32'h40, 3'd0, 4'd2, rdata, 1, 32'h0000_0000, 1'b0);
    // disable interrupt 
    `CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h10, 3'd0, 4'd2, 32'h0000_0000);
    `CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h3c, 3'd0, 4'd2, 32'h0000_0000);
    `CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h18, 3'd0, 4'd2, 32'h0000_0000);
    `CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h44, 3'd0, 4'd2, 32'h0000_0000);

    repeat(5000)@(posedge `CHIP_CLK48M);    
     
    // low leve active
    $display("===== GPIO single edge trigger ,active low =====");
    force topsys.chip.GPIO[47:0] = 48'h0000_0000_0000;

    // GPIO test
    `CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h08, 3'd0, 4'd2, 32'h0000_0000);
    `CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h34, 3'd0, 4'd2, 32'h0000_0000);
    // high level active
    `CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h0c, 3'd0, 4'd2, 32'hffff_ffff);
    `CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h38, 3'd0, 4'd2, 32'h0000_ffff);
    // GPIO inerrupt enable
    `CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h10, 3'd0, 4'd2, 32'hffff_f1fc);
    `CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h3c, 3'd0, 4'd2, 32'h0000_ffff);
    // 0 :level trigger 1: single edge trigger
    `CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h20, 3'd0, 4'd2, 32'hffff_f1fc);
    `CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h4c, 3'd0, 4'd2, 32'h0000_ffff);
    `CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h18, 3'd0, 4'd2, 32'hffff_f1fc);
    `CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h44, 3'd0, 4'd2, 32'h0000_ffff);

    repeat(500)@(posedge `CHIP_CLK48M);    

    wait(`GPIO_INTR);

    `CHIP_CPU_BFM.ahbrd( `GPIO_ADDR_BASE+32'h14, 3'd0, 4'd2, rdata, 1, 32'hffff_f1fc, 1'b0);
    `CHIP_CPU_BFM.ahbrd( `GPIO_ADDR_BASE+32'h40, 3'd0, 4'd2, rdata, 1, 32'h0000_ffff, 1'b0);
    // clear
    `CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h14, 3'd0, 4'd2, 32'hffff_f1fc);
    `CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h40, 3'd0, 4'd2, 32'h0000_ffff);
    wait(~`GPIO_INTR);
    `CHIP_CPU_BFM.ahbrd( `GPIO_ADDR_BASE+32'h14, 3'd0, 4'd2, rdata, 1, 32'h0000_0000, 1'b0);
    `CHIP_CPU_BFM.ahbrd( `GPIO_ADDR_BASE+32'h40, 3'd0, 4'd2, rdata, 1, 32'h0000_0000, 1'b0);

    `CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h10, 3'd0, 4'd2, 32'h0000_0000);
    `CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h3c, 3'd0, 4'd2, 32'h0000_0000);
    `CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h18, 3'd0, 4'd2, 32'h0000_0000);
    `CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h44, 3'd0, 4'd2, 32'h0000_0000);

    repeat(1000)@(posedge `CHIP_CLK48M);    

    // level edge trigger
    $display("===== GPIO level edge trigger ,active high =====");
    // GPIO test
    `CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h08, 3'd0, 4'd2, 32'h0000_0000);
    `CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h34, 3'd0, 4'd2, 32'h0000_0000);
    // high level active
    `CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h0c, 3'd0, 4'd2, 32'h0000_0000);
    `CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h38, 3'd0, 4'd2, 32'h0000_0000);
    // GPIO inerrupt enable
    `CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h10, 3'd0, 4'd2, 32'hffff_f1fc);
    `CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h3c, 3'd0, 4'd2, 32'h0000_ffff);
    // 0 :level trigger 1: single edge trigger
    `CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h20, 3'd0, 4'd2, 32'h0000_0000);
    `CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h4c, 3'd0, 4'd2, 32'h0000_0000);
    `CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h18, 3'd0, 4'd2, 32'hffff_f1fc);
    `CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h44, 3'd0, 4'd2, 32'h0000_ffff);

    repeat(50)@(posedge `CHIP_CLK48M);    
    
    force topsys.chip.GPIO[47:0] = 48'hffff_ffff_f1fc;
    repeat(1)@(posedge `CHIP_CLK48M);    
    force topsys.chip.GPIO[47:0] = 48'h0000_0000_0000;

    repeat(100)@(posedge `CHIP_CLK48M);    

    wait(`GPIO_INTR);

    `CHIP_CPU_BFM.ahbrd( `GPIO_ADDR_BASE+32'h14, 3'd0, 4'd2, rdata, 1, 32'hffff_f1fc, 1'b0);
    `CHIP_CPU_BFM.ahbrd( `GPIO_ADDR_BASE+32'h40, 3'd0, 4'd2, rdata, 1, 32'h0000_ffff, 1'b0);
    // clear
    `CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h14, 3'd0, 4'd2, 32'hffff_f1fc);
    `CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h40, 3'd0, 4'd2, 32'h0000_ffff);
    wait(~`GPIO_INTR);
    `CHIP_CPU_BFM.ahbrd( `GPIO_ADDR_BASE+32'h14, 3'd0, 4'd2, rdata, 1, 32'h0000_0000, 1'b0);
    `CHIP_CPU_BFM.ahbrd( `GPIO_ADDR_BASE+32'h40, 3'd0, 4'd2, rdata, 1, 32'h0000_0000, 1'b0);
    // disable interrupt 
    `CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h10, 3'd0, 4'd2, 32'h0000_0000);
    `CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h3c, 3'd0, 4'd2, 32'h0000_0000);

    `CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h18, 3'd0, 4'd2, 32'h0000_0000);
    `CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h44, 3'd0, 4'd2, 32'h0000_0000);

    repeat(100)@(posedge `CHIP_CLK48M);    
    force topsys.chip.GPIO[47:0] = 48'hffff_ffff_f1fc;

    // level edge trigger , low active
    $display("===== GPIO level edge trigger ,active low =====");
    // clear
    `CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h14, 3'd0, 4'd2, 32'hffff_f1fc);
    `CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h40, 3'd0, 4'd2, 32'h0000_ffff);
    // GPIO test
    `CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h08, 3'd0, 4'd2, 32'h0000_0000);
    `CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h34, 3'd0, 4'd2, 32'h0000_0000);
    // high level active
    `CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h0c, 3'd0, 4'd2, 32'hffff_f1fc);
    `CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h38, 3'd0, 4'd2, 32'h0000_ffff);
    // 0 :level trigger 1: single edge trigger
    `CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h20, 3'd0, 4'd2, 32'h0000_0000);
    `CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h4c, 3'd0, 4'd2, 32'h0000_0000);
    `CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h18, 3'd0, 4'd2, 32'hffff_f1fc);
    `CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h44, 3'd0, 4'd2, 32'h0000_ffff);
    // GPIO inerrupt enable
    `CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h10, 3'd0, 4'd2, 32'hffff_f1fc);
    `CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h3c, 3'd0, 4'd2, 32'h0000_ffff);

    repeat(50)@(posedge `CHIP_CLK48M);    
    
    force topsys.chip.GPIO[47:0] = 48'h0000_0000_0000;
    repeat(1)@(posedge `CHIP_CLK48M);    
    force topsys.chip.GPIO[47:0] = 48'hffff_ffff_f1fc;

    repeat(100)@(posedge `CHIP_CLK48M);    

    wait(`GPIO_INTR);

    `CHIP_CPU_BFM.ahbrd( `GPIO_ADDR_BASE+32'h14, 3'd0, 4'd2, rdata, 1, 32'hffff_f1fc, 1'b0);
    `CHIP_CPU_BFM.ahbrd( `GPIO_ADDR_BASE+32'h40, 3'd0, 4'd2, rdata, 1, 32'h0000_ffff, 1'b0);
    // clear
    `CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h14, 3'd0, 4'd2, 32'hffff_f1fc);
    `CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h40, 3'd0, 4'd2, 32'h0000_ffff);
    wait(~`GPIO_INTR);
    `CHIP_CPU_BFM.ahbrd( `GPIO_ADDR_BASE+32'h14, 3'd0, 4'd2, rdata, 1, 32'h0000_0000, 1'b0);
    `CHIP_CPU_BFM.ahbrd( `GPIO_ADDR_BASE+32'h40, 3'd0, 4'd2, rdata, 1, 32'h0000_0000, 1'b0);
    // disable interrupt 
    `CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h10, 3'd0, 4'd2, 32'h0000_0000);
    `CHIP_CPU_BFM.ahbwr( `GPIO_ADDR_BASE+32'h3c, 3'd0, 4'd2, 32'h0000_0000);



    repeat(5000)@(posedge `CHIP_CLK48M);    
    $finish;
end

