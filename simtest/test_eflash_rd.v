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

`define		PRELOAD_FLASH

initial begin 
    dump_event=0;
    $display("@%t, %m MSG: ****** test name: test_eflash_rd", $time);
    wait (`TB_INIT_DONE);
    $display("@%t, %m MSG: TB_INIT_DONEROM", $time);

    //---- ef main
    $display("@%d, MSG: EF read main", $time);
    for (i=0; i<4096; i=i+1) begin
        wdata = i*16;
        `CHIP_CPU_BFM.ahbrd_incr( `EF_BASE+(i*16*4), 1, 16, 2, rdata, 1, wdata, 0);
    end

    //---- ef page erase
    $display("@%d, MSG: EF page erase", $time);
    ef_setmode(4,1,0);

    //---- ef page wr
    $display("@%d, MSG: EF write main", $time);
    for (i=0; i<512; i=i+1) begin
        ef_wr(i*4, 32'hffff_0000+i, 0); 
    end

    ic_flush;
    dump_event=1'b1;
    for (i=0; i<32; i=i+1) begin
        wdata = 32'hffff_0000 + i*16;
        `CHIP_CPU_BFM.ahbrd_incr( `EF_BASE+(i*16*4), 1, 16, 2, rdata, 1, wdata, 0);
    end

    //---- ef information page rd
    ic_flush;
    $display("@%d, MSG: EF read info", $time);

    ef_setmode(3, 0, 0); // mode, en, rw
    for (i=0; i<32; i=i+1) begin
        wdata = 32'h0001_0000 + i*16;
        `CHIP_CPU_BFM.ahbrd_incr( `EF_BASE+(i*16*4), 1, 16, 2, rdata, 1, wdata, 0);
    end

/**** NO redundancy in new eflash_ulp
    //---- ef repair page/status rd
    ic_flush;
    $display("@%d, MSG: EF read repair page/status", $time);

    ef_setmode(2,0,0);
    for (i=0; i<32; i=i+1) begin
        wdata = 32'h0002_0000 + i*16;
        `CHIP_CPU_BFM.ahbrd_incr( `EF_BASE+(i*16*4), 1, 16, 2, rdata, 1, wdata, 0);
    end

    //---- ef redundancy erase
    $display("@%d, MSG: EF redundancy page erase", $time);
    ef_setmode(5,1,0);

    //---- ef redundancy write
    $display("@%d, MSG: EF write redundancy", $time);

    for (i=0; i<512; i=i+1) begin
        ef_wr(i*4, i, 1); 
    end

    //---- ef redundancy read
    ic_flush;
    $display("@%d, MSG: EF read redundancy", $time);

    // ef redundancy enable
    wdata = 32'h0000_0010;
    `CHIP_CPU_BFM.ahbwr(`EF_CONFIG_REG, 3'd0, 4'd2, wdata);

    // ef read redundancy ram
    ef_setmode(1,0,0);

    for (i=0; i<32; i=i+1) begin
        wdata = i*16;
        `CHIP_CPU_BFM.ahbrd_incr( `EF_BASE+(i*16*4), 1, 16, 2, rdata, 1, wdata, 0);
    end
****/
    repeat(100)@(posedge `CHIP_CLK48M);    
    $finish;
end


