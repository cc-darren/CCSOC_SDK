
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
// I2s mode 
// 2'b01 : I2s format
// 2'b10 : Left-justified
// 2'b11 : Right-justified
reg [1:0]       mode = 2'b10;

initial begin


    $display("@%t, %m MSG: ****** test name: init_test_CPU_BFM", $time);
    wait (`TB_INIT_DONE);
    $display("@%d, %m MSG: TB_INIT_DONEROM", $time);

`ifdef ROM_BOOT
    force topsys.chip.GPIO[4] = topsys.chip.GPIO[8];
    force topsys.chip.GPIO[5] = topsys.chip.GPIO[9];
    force topsys.chip.GPIO[7] = topsys.chip.GPIO[10];

    repeat(100) @(posedge `CHIP_CLK48M);

    wait(~`I2S_TXDMA_INTR);
    repeat(100) @(posedge `CHIP_CLK48M);
    wait(~`I2S_RXDMA_INTR);

    wait(`CHIP_CORE0.gpio_top_wrap.gpio_top.gpio.gpio_int_polarity==32'd1);
    $display("@%d, %m MSG: test finish", $time);

    // test error count
    if (`CHIP_CORE0.gpio_top_wrap.gpio_top.gpio.gpio_int_type!=0) begin
        $display("@%t, %m ERROR: error count=%0d", $time, `CHIP_CORE0.gpio_top_wrap.gpio_top.gpio.gpio_int_type);
    end
    else 
    $display("@%d, %m MSG: test error count=0", $time);
    
    repeat(5000)@(posedge `CHIP_CLK48M);    
    $finish;

`else
    `ifdef I2S_PORT_8
    // set_port_i2s_m8
    force topsys.chip.GPIO[4] = topsys.chip.GPIO[8];
    force topsys.chip.GPIO[5] = topsys.chip.GPIO[9];
    force topsys.chip.GPIO[7] = topsys.chip.GPIO[10];
    set_port_i2s_m8();
    set_port_i2s_s4();
    $display(" ===== I2S master PORT 8 test =====");
   `elsif I2S_PORT_0
    force topsys.chip.GPIO[4] = topsys.chip.GPIO[0];
    force topsys.chip.GPIO[5] = topsys.chip.GPIO[1];
    force topsys.chip.GPIO[3] = topsys.chip.GPIO[2];
    set_port_i2s_m0();
    set_port_i2s_s4();
    $display(" ===== I2S master PORT 0 test =====");
    `elsif I2S_PORT_4
    force topsys.chip.GPIO[8] = topsys.chip.GPIO[4];
    force topsys.chip.GPIO[9] = topsys.chip.GPIO[5];
    force topsys.chip.GPIO[7] = topsys.chip.GPIO[6];
    set_port_i2s_m4();
    set_port_i2s_s8();
    $display(" ===== I2S master PORT 4 test =====");
   `elsif I2S_PORT_12
    force topsys.chip.GPIO[4] = topsys.chip.GPIO[12];
    force topsys.chip.GPIO[5] = topsys.chip.GPIO[13];
    force topsys.chip.GPIO[7] = topsys.chip.GPIO[14];
    set_port_i2s_m12();
    set_port_i2s_s4();
    $display(" ===== I2S master PORT 12 test =====");
    `elsif I2S_PORT_16
    force topsys.chip.GPIO[8] = topsys.chip.GPIO[16];
    force topsys.chip.GPIO[9] = topsys.chip.GPIO[17];
    force topsys.chip.GPIO[11] = topsys.chip.GPIO[18];
    set_port_i2s_m16();
    set_port_i2s_s8();
    $display(" ===== I2S master PORT 16 test =====");
    `elsif I2S_PORT_20
    force topsys.chip.GPIO[4] = topsys.chip.GPIO[20];
    force topsys.chip.GPIO[5] = topsys.chip.GPIO[21];
    force topsys.chip.GPIO[7] = topsys.chip.GPIO[22];
    set_port_i2s_m20();
    set_port_i2s_s4();
    $display(" ===== I2S master PORT 20 test =====");
    `elsif I2S_PORT_24
    force topsys.chip.GPIO[4] = topsys.chip.GPIO[24];
    force topsys.chip.GPIO[5] = topsys.chip.GPIO[25];
    force topsys.chip.GPIO[7] = topsys.chip.GPIO[26];
    set_port_i2s_m24();
    set_port_i2s_s4();
    $display(" ===== I2S master PORT 24 test =====");
    `elsif I2S_PORT_28
    force topsys.chip.GPIO[8] = topsys.chip.GPIO[28];
    force topsys.chip.GPIO[9] = topsys.chip.GPIO[29];
    force topsys.chip.GPIO[11] = topsys.chip.GPIO[30];
    set_port_i2s_m28();
    set_port_i2s_s8();
    $display(" ===== I2S master PORT 28 test =====");
    `elsif I2S_PORT_32
    force topsys.chip.GPIO[8] = topsys.chip.GPIO[32];
    force topsys.chip.GPIO[9] = topsys.chip.GPIO[33];
    force topsys.chip.GPIO[11] = topsys.chip.GPIO[34];
    set_port_i2s_m32();
    set_port_i2s_s8();
    $display(" ===== I2S master PORT 32 test =====");
    `elsif I2S_PORT_36
    force topsys.chip.GPIO[8] = topsys.chip.GPIO[36];
    force topsys.chip.GPIO[9] = topsys.chip.GPIO[37];
    force topsys.chip.GPIO[11] = topsys.chip.GPIO[38];
    set_port_i2s_m36();
    set_port_i2s_s8();
    $display(" ===== I2S master PORT 36 test =====");
    `elsif I2S_PORT_40
    force topsys.chip.GPIO[8] = topsys.chip.GPIO[40];
    force topsys.chip.GPIO[9] = topsys.chip.GPIO[41];
    force topsys.chip.GPIO[11] = topsys.chip.GPIO[42];
    set_port_i2s_m40();
    set_port_i2s_s8();
    $display(" ===== I2S master PORT 40 test =====");
    `else
    force topsys.chip.GPIO[8] = topsys.chip.GPIO[44];
    force topsys.chip.GPIO[9] = topsys.chip.GPIO[45];
    force topsys.chip.GPIO[11] = topsys.chip.GPIO[46];
    set_port_i2s_m44();
    set_port_i2s_s8();
    $display(" ===== I2S master PORT 44 test =====");
    `endif

    // I2S TX data
    `CHIP_CPU_BFM.ahbwr( `SYSRAM_ADDR_BASE+32'h00, 3'd0, 4'd2, 32'h0123_4567);
    `CHIP_CPU_BFM.ahbwr( `SYSRAM_ADDR_BASE+32'h04, 3'd0, 4'd2, 32'h89ab_cdef);
    `CHIP_CPU_BFM.ahbwr( `SYSRAM_ADDR_BASE+32'h08, 3'd0, 4'd2, 32'hfedc_ba98);
    `CHIP_CPU_BFM.ahbwr( `SYSRAM_ADDR_BASE+32'h0c, 3'd0, 4'd2, 32'h7654_3210);
    `CHIP_CPU_BFM.ahbwr( `SYSRAM_ADDR_BASE+32'h10, 3'd0, 4'd2, 32'h1111_1111);
    `CHIP_CPU_BFM.ahbwr( `SYSRAM_ADDR_BASE+32'h14, 3'd0, 4'd2, 32'h2222_2222);
    `CHIP_CPU_BFM.ahbwr( `SYSRAM_ADDR_BASE+32'h18, 3'd0, 4'd2, 32'h3333_3333);
    `CHIP_CPU_BFM.ahbwr( `SYSRAM_ADDR_BASE+32'h1c, 3'd0, 4'd2, 32'h4444_4444);
    `CHIP_CPU_BFM.ahbwr( `SYSRAM_ADDR_BASE+32'h20, 3'd0, 4'd2, 32'h5555_5555);
    `CHIP_CPU_BFM.ahbwr( `SYSRAM_ADDR_BASE+32'h24, 3'd0, 4'd2, 32'h6666_6666);
    `CHIP_CPU_BFM.ahbwr( `SYSRAM_ADDR_BASE+32'h28, 3'd0, 4'd2, 32'h7777_7777);
    `CHIP_CPU_BFM.ahbwr( `SYSRAM_ADDR_BASE+32'h2c, 3'd0, 4'd2, 32'h8888_8888);
    `CHIP_CPU_BFM.ahbwr( `SYSRAM_ADDR_BASE+32'h30, 3'd0, 4'd2, 32'h9999_9999);
    `CHIP_CPU_BFM.ahbwr( `SYSRAM_ADDR_BASE+32'h34, 3'd0, 4'd2, 32'haaaa_aaaa);
    `CHIP_CPU_BFM.ahbwr( `SYSRAM_ADDR_BASE+32'h38, 3'd0, 4'd2, 32'hbbbb_bbbb);
    `CHIP_CPU_BFM.ahbwr( `SYSRAM_ADDR_BASE+32'h3c, 3'd0, 4'd2, 32'hcccc_cccc);
    `CHIP_CPU_BFM.ahbwr( `SYSRAM_ADDR_BASE+32'h40, 3'd0, 4'd2, 32'hdddd_dddd);
    `CHIP_CPU_BFM.ahbwr( `SYSRAM_ADDR_BASE+32'h44, 3'd0, 4'd2, 32'heeee_eeee);
    `CHIP_CPU_BFM.ahbwr( `SYSRAM_ADDR_BASE+32'h48, 3'd0, 4'd2, 32'hffff_ffff);
    `CHIP_CPU_BFM.ahbwr( `SYSRAM_ADDR_BASE+32'h4c, 3'd0, 4'd2, 32'ha5a5_a5a5);
  //  `CHIP_CPU_BFM.ahbwr( 32'h0006_0080, 3'd0, 4'd2, 32'h0123_4567);
  //  `CHIP_CPU_BFM.ahbwr( 32'h0006_0084, 3'd0, 4'd2, 32'h89ab_cdef);
  //  `CHIP_CPU_BFM.ahbwr( 32'h0006_0088, 3'd0, 4'd2, 32'hfedc_ba98);
  //  `CHIP_CPU_BFM.ahbwr( 32'h0006_008c, 3'd0, 4'd2, 32'h7654_3210);
    repeat(4) @(posedge `CHIP_CLK48M);

    //program interrupt enable  0: RX 1: TX
    `CHIP_CPU_BFM.ahbwr( `I2S_ADDR_BASE+32'h20, 3'd0, 4'd2, 32'h0000_0003);
    repeat(4) @(posedge `CHIP_CLK48M);

    //program dam_byte_num [15:8] TX byte [7:0] R byte : 0 > 1byte ~ FF > 256byte          
    // TX 16byte RX 12byte   
    `CHIP_CPU_BFM.ahbwr( `I2S_ADDR_BASE+32'h24, 3'd0, 4'd2, 32'h0000_0c0c);
    `CHIP_CPU_BFM.ahbrd( `I2S_ADDR_BASE+32'h24, 3'd0, 4'd2, rdata, 1, 32'h0000_0c0c, 1'b0);
    repeat(4) @(posedge `CHIP_CLK48M);

    //program dma_rx_start_addr
    `CHIP_CPU_BFM.ahbwr( `I2S_ADDR_BASE+32'h28, 3'd0, 4'd2, `SYSRAM_ADDR_BASE+32'h1000);
  //  `CHIP_CPU_BFM.ahbrd( `I2S_ADDR_BASE+32'h28, 3'd0, 4'd2, rdata, 1, 32'h6_1000, 1'b0);
    repeat(4) @(posedge `CHIP_CLK48M);

    //program dma_rx_end_addr
    `CHIP_CPU_BFM.ahbwr( `I2S_ADDR_BASE+32'h2c, 3'd0, 4'd2, `SYSRAM_ADDR_BASE+32'h104c);
  //  `CHIP_CPU_BFM.ahbrd( `I2S_ADDR_BASE+32'h2c, 3'd0, 4'd2, rdata, 1, 32'h6_100c, 1'b0);
    repeat(4) @(posedge `CHIP_CLK48M);

    //program dma_tx_start_addr
    `CHIP_CPU_BFM.ahbwr( `I2S_ADDR_BASE+32'h34, 3'd0, 4'd2, `SYSRAM_ADDR_BASE);
  //  `CHIP_CPU_BFM.ahbrd( `I2S_ADDR_BASE+32'h34, 3'd0, 4'd2, rdata, 1, 32'h6000, 1'b0);
    repeat(4) @(posedge `CHIP_CLK48M);

    //program dma_tx_end_addr
    `CHIP_CPU_BFM.ahbwr( `I2S_ADDR_BASE+32'h38, 3'd0, 4'd2, `SYSRAM_ADDR_BASE+32'h004c);
  //  `CHIP_CPU_BFM.ahbrd( `I2S_ADDR_BASE+32'h38, 3'd0, 4'd2, rdata, 1, 32'h6_000c, 1'b0);
    repeat(4) @(posedge `CHIP_CLK48M);

    //read RX addr in RAM location
    `CHIP_CPU_BFM.ahbwr( `I2S_ADDR_BASE+32'h40, 3'd0, 4'd2, 32'h0);
 //   `CHIP_CPU_BFM.ahbrd( `I2S_ADDR_BASE+32'h40, 3'd0, 4'd2, rdata, 1, 32'h0, 1'b0);
    repeat(4) @(posedge `CHIP_CLK48M);

    //read TX addr in RAM location
    `CHIP_CPU_BFM.ahbwr( `I2S_ADDR_BASE+32'h44, 3'd0, 4'd2, 32'h0);
  //  `CHIP_CPU_BFM.ahbrd( `I2S_ADDR_BASE+32'h44, 3'd0, 4'd2, rdata, 1, 32'h0, 1'b0);
    repeat(4) @(posedge `CHIP_CLK48M);

    //set Master Mode
    `CHIP_CPU_BFM.ahbwr( `I2S_ADDR_BASE+32'h00, 3'd0, 4'd2, 32'h17_0109);
    `CHIP_CPU_BFM.ahbrd( `I2S_ADDR_BASE+32'h00, 3'd0, 4'd2, rdata, 1, 32'h17_0109, 1'b0);
    repeat(4) @(posedge `CHIP_CLK48M);

    `CHIP_CPU_BFM.ahbwr( `I2S_ADDR_BASE+32'h0c, 3'd0, 4'd2, 32'hffff_ff00);
    repeat(4) @(posedge `CHIP_CLK48M);

//    `CHIP_CPU_BFM.ahbwr( `I2S_ADDR_BASE+32'h10, 3'd0, 4'd2, 32'h0);
//    repeat(4) @(posedge `CHIP_CLK48M);

//    `CHIP_CPU_BFM.ahbwr( `I2S_ADDR_BASE+32'h0c, 3'd0, 4'd2, 32'h0);
//    repeat(4) @(posedge `CHIP_CLK48M);

    `CHIP_CPU_BFM.ahbwr( `I2S_ADDR_BASE+32'h10, 3'd0, 4'd2, 32'hffff_ff00);
    repeat(4) @(posedge `CHIP_CLK48M);


    //*********end i2s_ip programming*************
    if(mode == 2'b01) begin
     $display(" ===== I2s foramt =====");
     end
    else if(mode == 2'b10) begin
     $display(" ===== Left justified =====");
      end
    else if(mode == 2'b11) begin
     $display(" ===== Right justified =====");
    end
    else begin
     $display(" ===== TX/RX disabled =====");
     end
     
    //***************enable DMA*******************
    //program I2S-IP to start TX
    // threshold [18:16] = 4
    // [13] TXFIFOENL = 1 [12] TXFIFIENR = 1
    // [11] TXLD = 1 DMA enable
    // [10] TXRD = 1 DMA enable
    // [7] TXER =1 int enable
    // [6:2] = 00000
    // [1:0] = 10 Left-J   >  01:I2S format 11 Right-J 
    `CHIP_CPU_BFM.ahbwr( `I2S_ADDR_BASE+32'h08, 3'd0, 4'd2, {32'h4_3cdc + mode});
    `CHIP_CPU_BFM.ahbrd( `I2S_ADDR_BASE+32'h08, 3'd0, 4'd2, rdata, 1, {32'h4_3cdc + mode}, 1'b0);
    repeat(4) @(posedge `CHIP_CLK48M);

    //program I2S-IP to start RX
    `CHIP_CPU_BFM.ahbwr( `I2S_ADDR_BASE+32'h04, 3'd0, 4'd2, {32'h4_3cdc + mode});
    `CHIP_CPU_BFM.ahbrd( `I2S_ADDR_BASE+32'h04, 3'd0, 4'd2, rdata, 1, {32'h4_3cdc + mode}, 1'b0);

    //start DBUS RX-DMA
    `CHIP_CPU_BFM.ahbwr( `I2S_ADDR_BASE+32'h30, 3'd0, 4'd2, 32'h1);
    `CHIP_CPU_BFM.ahbrd( `I2S_ADDR_BASE+32'h30, 3'd0, 4'd2, rdata, 1, 32'h1, 1'b0);

    //start DBUS TX-DMA
    `CHIP_CPU_BFM.ahbwr( `I2S_ADDR_BASE+32'h3c, 3'd0, 4'd2, 32'h1);
    `CHIP_CPU_BFM.ahbrd( `I2S_ADDR_BASE+32'h3c, 3'd0, 4'd2, rdata, 1, 32'h1, 1'b0);

    wait(`I2S_TXDMA_INTR);
    repeat(20)@(posedge `CHIP_CLK48M);
    repeat(1)@(posedge `CHIP_CLK48M);

    repeat(10)@(posedge `CHIP_CLK48M);
    repeat(2000)@(posedge `CHIP_CLK48M);
    

    wait(`I2S_RXDMA_INTR);
    // end I2S -RX 
    `CHIP_CPU_BFM.ahbwr( `I2S_ADDR_BASE+32'h04, 3'd0, 4'd2, 32'h7_00de);
    `CHIP_CPU_BFM.ahbwr( `I2S_ADDR_BASE+32'h30, 3'd0, 4'd2, 32'h0);

    // end I2S -TX 
    `CHIP_CPU_BFM.ahbwr( `I2S_ADDR_BASE+32'h08, 3'd0, 4'd2, 32'h4_00de);
    `CHIP_CPU_BFM.ahbwr( `I2S_ADDR_BASE+32'h20, 3'd0, 4'd2, 32'h3_0000);
    `CHIP_CPU_BFM.ahbwr( `I2S_ADDR_BASE+32'h3c, 3'd0, 4'd2, 32'h0);

    `CHIP_CPU_BFM.ahbwr( `I2S_ADDR_BASE+32'h0, 3'd0, 4'd2, 32'h17_0101);
    
    wait(~`I2S_TXDMA_INTR);
    wait(~`I2S_RXDMA_INTR);
    repeat(100)@(posedge `CHIP_CLK48M);
    // Flush TX fifo   
    `CHIP_CPU_BFM.ahbwr( `I2S_ADDR_BASE+32'h3c, 3'd0, 4'd2, 32'h0100);
    repeat(20)@(posedge `CHIP_CLK48M);
    `CHIP_CPU_BFM.ahbwr( `I2S_ADDR_BASE+32'h3c, 3'd0, 4'd2, 32'h0);

    // read RX fifo 32'h6_1000
    if(mode == 2'b10) begin  // Left-justified
    `CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h1000, 3'd0, 4'd2, rdata, 1, 32'h123_4500, 1'b0);
    `CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h1004, 3'd0, 4'd2, rdata, 1, 32'h89ab_cd00, 1'b0);
    `CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h1008, 3'd0, 4'd2, rdata, 1, 32'hfedc_ba00, 1'b0);
    `CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h100c, 3'd0, 4'd2, rdata, 1, 32'h7654_3200, 1'b0);
    `CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h1010, 3'd0, 4'd2, rdata, 1, 32'h1111_1100, 1'b0);
    `CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h1014, 3'd0, 4'd2, rdata, 1, 32'h2222_2200, 1'b0);
    `CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h1018, 3'd0, 4'd2, rdata, 1, 32'h3333_3300, 1'b0);
    `CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h101c, 3'd0, 4'd2, rdata, 1, 32'h4444_4400, 1'b0);
    `CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h1020, 3'd0, 4'd2, rdata, 1, 32'h5555_5500, 1'b0);
    `CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h1024, 3'd0, 4'd2, rdata, 1, 32'h6666_6600, 1'b0);
    `CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h1028, 3'd0, 4'd2, rdata, 1, 32'h7777_7700, 1'b0);
    `CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h102c, 3'd0, 4'd2, rdata, 1, 32'h8888_8800, 1'b0);
    `CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h1030, 3'd0, 4'd2, rdata, 1, 32'h9999_9900, 1'b0);
    `CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h1034, 3'd0, 4'd2, rdata, 1, 32'haaaa_aa00, 1'b0);
    `CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h1038, 3'd0, 4'd2, rdata, 1, 32'hbbbb_bb00, 1'b0);
    `CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h103c, 3'd0, 4'd2, rdata, 1, 32'hcccc_cc00, 1'b0);
    `CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h1040, 3'd0, 4'd2, rdata, 1, 32'hdddd_dd00, 1'b0);
    `CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h1044, 3'd0, 4'd2, rdata, 1, 32'heeee_ee00, 1'b0);
    `CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h1048, 3'd0, 4'd2, rdata, 1, 32'hffff_ff00, 1'b0);
    `CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h104c, 3'd0, 4'd2, rdata, 1, 32'ha5a5_a500, 1'b0);
    end
    else if(mode == 2'b01) begin  // I2s format
    `CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h1000, 3'd0, 4'd2, rdata, 1, 32'hffff_ff00, 1'b0);
    `CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h1004, 3'd0, 4'd2, rdata, 1, 32'ha5a5_a500, 1'b0);
    `CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h1008, 3'd0, 4'd2, rdata, 1, 32'h123_4500, 1'b0);
    `CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h100c, 3'd0, 4'd2, rdata, 1, 32'h89ab_cd00, 1'b0);
    `CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h1010, 3'd0, 4'd2, rdata, 1, 32'hfedc_ba00, 1'b0);
    `CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h1014, 3'd0, 4'd2, rdata, 1, 32'h7654_3200, 1'b0);
    `CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h1018, 3'd0, 4'd2, rdata, 1, 32'h1111_1100, 1'b0);
    `CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h101c, 3'd0, 4'd2, rdata, 1, 32'h2222_2200, 1'b0);
    `CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h1020, 3'd0, 4'd2, rdata, 1, 32'h3333_3300, 1'b0);
    `CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h1024, 3'd0, 4'd2, rdata, 1, 32'h4444_4400, 1'b0);
    `CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h1028, 3'd0, 4'd2, rdata, 1, 32'h5555_5500, 1'b0);
    `CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h102c, 3'd0, 4'd2, rdata, 1, 32'h6666_6600, 1'b0);
    `CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h1030, 3'd0, 4'd2, rdata, 1, 32'h7777_7700, 1'b0);
    `CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h1034, 3'd0, 4'd2, rdata, 1, 32'h8888_8800, 1'b0);
    `CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h1038, 3'd0, 4'd2, rdata, 1, 32'h9999_9900, 1'b0);
    `CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h103c, 3'd0, 4'd2, rdata, 1, 32'haaaa_aa00, 1'b0);
    `CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h1040, 3'd0, 4'd2, rdata, 1, 32'hbbbb_bb00, 1'b0);
    `CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h1044, 3'd0, 4'd2, rdata, 1, 32'hcccc_cc00, 1'b0);
    `CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h1048, 3'd0, 4'd2, rdata, 1, 32'hdddd_dd00, 1'b0);
    `CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h104c, 3'd0, 4'd2, rdata, 1, 32'heeee_ee00, 1'b0);
    end
    else if(mode == 2'b11) begin
    `CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h1000, 3'd0, 4'd2, rdata, 1, 32'h123_4500, 1'b0);
    `CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h1004, 3'd0, 4'd2, rdata, 1, 32'h89ab_cd00, 1'b0);
    `CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h1008, 3'd0, 4'd2, rdata, 1, 32'hfedc_ba00, 1'b0);
    `CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h100c, 3'd0, 4'd2, rdata, 1, 32'h7654_3200, 1'b0);
    end

    repeat(5000)@(posedge `CHIP_CLK48M);    
    $finish;
`endif
end

