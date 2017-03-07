reg [31:0]	wdata;
reg [31:0]	rdata;
integer		i;
reg [3:0]	data_erase_mode;
////   data_erase_mode :
////   *    0000 : main access (R/W)
////   *    0001 : redundancy access (R/W)
////   *    0010 : twimcode access (R), repair page/status information
////   *    0011 : information access (R/W)
////   *    0100 : main page erase
////   *    0101 : redundancy page erase
////   *    0110 : NA
////   *    0111 : information page erase
////   *    1100 : mass erase
////
reg		data_erase_enable;
reg		rw_mode;
reg		dump_event;

reg [19:0] ef_addr;
reg [16:0] dma_str_raddr;
reg [16:0] dma_str_waddr;
reg [11:0] wdata_byte_num;
reg [11:0] rdata_byte_num;
reg [1:0] port_op_mode;
reg [0:0] dma_enable;
reg [0:0] dbus_burst;

reg [31:0] wdata_base;
`define		PRELOAD_FLASH

initial begin 
    dump_event=0;
    $display("@%t, %m MSG: ****** test name: test_eflash_dma_wr", $time);
    wait (`TB_INIT_DONE);
    $display("@%t, %m MSG: TB_INIT_DONEROM", $time);

`ifndef FPGA
    //---- ef page erase
    $display("@%d, MSG: EF page erase", $time);
    ef_setmode(4,1,0);

    dump_event=1'b1;
`endif

    //---- system ram setup
    wdata_base = 32'hffff_0000;
    for (i=0; i<24576; i=i+1) begin
        `CHIP_CPU_BFM.ahbwr( `SYSRAM_ADDR_BASE+i*4, 3'd0, 4'd2, wdata_base + i);
    end

    //---- ef page dma wr
    dump_event=1'b1;
    $display("@%d, MSG: EF dma 1 page write main", $time);
    // port_write
    dma_str_raddr = 17'h0;
    wdata_byte_num = 11'h7ff;

    // port read
    dma_str_waddr = 17'h100;	 
    rdata_byte_num = 11'h7ff;

    port_op_mode = 2'h0; // write to port
    dma_enable=1'b1;
    dbus_burst = 1'h1;
    ef_addr=20'd0;
    ef_dma(ef_addr,dma_str_raddr,dma_str_waddr,wdata_byte_num, rdata_byte_num,port_op_mode, dma_enable, dbus_burst);

    //----- ef page read
    $display("@%d, MSG: EF dma 1 page read main", $time);
    ic_flush;
    for (i=0; i<32; i=i+1) begin
        wdata = wdata_base + i*16;
        `CHIP_CPU_BFM.ahbrd_incr( `EF_BASE+(i*16*4), 1, 16, 2, rdata, 1, wdata, 0);
    end

    repeat(100)@(posedge `CHIP_CLK48M);    
    $finish;
end


