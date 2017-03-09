
////parameter       M_CLK_PERIOD_DIV2 = 166.66; // 3MHZ. 333.33/2
parameter       M_CLK_PERIOD_DIV2 = 83.33; // 6MHZ. 166.66/2
reg     m_clk=0;
always begin
    m_clk = #M_CLK_PERIOD_DIV2 ~m_clk;
end

integer 	i;
reg [1:0] 	int_en = 2'b11;
reg		dual_bank = 1'b0;
reg		redu_en=1'b0;
reg [31:0]	rdata;

reg [31:0] wr_din[7:0];

reg [16:0]	dma_str_raddr;
reg [16:0]	dma_str_waddr;
reg [11:0]	wdata_byte_num;
reg [11:0]	rdata_byte_num;
reg [1:0]	port_op_mode;
reg [0:0]	dma_enable;
reg [0:0]	dbus_burst;
reg [4:0]       bc_mode;
reg [1:0]       key_size;
reg             decrypt;
reg [255:0]	key_exp, key_swap;
reg [31:0]	sram_dat[31:0];

`define AES_INTR_REG            `AES_ADDR_BASE + 32'h0
`define AES_CTRL_REG            `AES_ADDR_BASE + 32'h4
`define AES_DMA_CTRL_REG        `AES_ADDR_BASE + 32'h8
`define AES_DMA_WADDR_REG       `AES_ADDR_BASE + 32'hc
`define AES_DMA_RADDR_REG       `AES_ADDR_BASE + 32'h10

initial begin
    $display("@%t, %m MSG: ****** test name: test_otp_efuse", $time);
   //// force `CHIP_TOP.test_mode=1'b1;
    ////force `TOPSYS.VDDQ=1'b1;

    wait (`TB_INIT_DONE);
    $display("@%d, %m MSG: TB_INIT_DONEROM", $time);
`ifdef M4_SELECT
    `CHIP_CPU_BFM.set_rd_file_limit("ahb_rdata1.hex",0,32768);
`else
    `CHIP_CPU_BFM.set_rd_file_limit("ahb_rdata.hex",0,32768);
`endif

    @(posedge `CHIP_CLK48M);

    // TEMPORARY   
    force `CHIP_TOP.OTP_TDO = `CHIP_TOP.jtag_tdi ^ `CHIP_TOP.jtag_tms;

`ifndef FPGA
    // -------------------------------------------------------
    // otp efuse write - hardware assit
   
    $display("@%0d, MSG: **** efuse write hardware assist", $time);
    force `CHIP_TOP.test_mode=1'b1;
    force `TOPSYS.VDDQ=1'b1;

    repeat(1) @(posedge `TOPSYS.jtag_tck);
    force `CHIP_OTP_AES.otp.otp_ctrl.rstn= 1'b0; 
    repeat(2) @(posedge `TOPSYS.jtag_tck);
    release `CHIP_OTP_AES.otp.otp_ctrl.rstn;

    repeat(1) @(posedge `TOPSYS.jtag_tck);
/****
    // key = {128'h0ca57533_16367518_f11032c6_5b58a600};
    // iv = {128'hc1e086aa_99729985_8b984aa2_aa671c83};
    key_swap = {128'h5b58a600_f11032c6_16367518_0ca57533,
		128'haa671c83_8b984aa2_99729985_c1e086aa};
    wr_din = key_swap;
****/
/****
    // key = {128'h89abcdef_01234567_89abcdef_01234567};
    // iv = {128'h00000003_00000002_00000001_00000000};
****/
    key_swap = {128'h01234567_89abcdef_01234567_89abcdef,
    	           128'h00000000_00000001_00000002_00000003};		
    wr_din[0] = key_swap[32*1-1:32*0];
    wr_din[1] = key_swap[32*2-1:32*1];
    wr_din[2] = key_swap[32*3-1:32*2];
    wr_din[3] = key_swap[32*4-1:32*3];
    wr_din[4] = key_swap[32*5-1:32*4];
    wr_din[5] = key_swap[32*6-1:32*5];
    wr_din[6] = key_swap[32*7-1:32*6];
    wr_din[7] = key_swap[32*8-1:32*7];
/****
    wr_din[0] = 32'h00000003; 
    wr_din[1] = 32'h00000002; 
    wr_din[2] = 32'h00000001; 
    wr_din[3] = 32'h00000000; 
    wr_din[4] = 32'h89abcdef; 
    wr_din[5] = 32'h01234567; 
    wr_din[6] = 32'h89abcdef; 
    wr_din[7] = 32'h01234567; 
****/

    key_exp = { wr_din[7],wr_din[6],wr_din[5],wr_din[4],
		wr_din[3],wr_din[2],wr_din[1],wr_din[0]};

    for (i=0; i<8; i=i+1) begin
        force `CHIP_OTP_AES.otp.otp_ctrl.prog_en = 1'b1;
        force `CHIP_OTP_AES.otp.otp_ctrl.prog_wr = 1'b1;
        force `CHIP_OTP_AES.otp.otp_ctrl.prog_manual = 1'b0;
        force `CHIP_OTP_AES.otp.otp_ctrl.prog_din[31:0] = wr_din[i];
        repeat(2) @(posedge `TOPSYS.jtag_tck);
        wait (~`CHIP_OTP_AES.otp.otp_ctrl.prog_dn);
        wait (`CHIP_OTP_AES.otp.otp_ctrl.prog_dn);
        $display("@%0d, MSG: prog_dn(%0d)", $time,i);
        repeat(1) @(posedge `TOPSYS.jtag_tck);
        force `CHIP_OTP_AES.otp.otp_ctrl.prog_en = 1'b0;
        repeat(1) @(posedge `TOPSYS.jtag_tck);
    end


    release `CHIP_OTP_AES.otp.otp_ctrl.prog_en;
    release `CHIP_OTP_AES.otp.otp_ctrl.prog_wr;
    release `CHIP_OTP_AES.otp.otp_ctrl.prog_manual;
    release `CHIP_OTP_AES.otp.otp_ctrl.prog_din;

    // --------------------------------------------------
    // otp efuse read - parallel bus
    $display("@%0d, MSG: **** read parallel mode", $time);
    repeat(1) @(posedge `TOPSYS.jtag_tck);
    release `CHIP_TOP.test_mode;
    release `CHIP_CORE1.VDDQ;

    force `TOPSYS.RESETN = 1'b0;;
    repeat(1) @(posedge `TOPSYS.jtag_tck);
    force `TOPSYS.RESETN = 1'b1;

    ////force `CHIP_CORE1.op_mode=1'd1;
    repeat(10) @(posedge `CHIP_SYSCLK);
    repeat(1) @(posedge `CHIP_SYSCLK);

    wait(`CHIP_CORE1.key_en);
    if (`CHIP_CORE1.key!==key_exp) begin
        $display("@%0d, %m otp key mismathc. exp=%x, got=%x", $time, key_exp, `CHIP_CORE1.key);
    end

    ////release `CHIP_CORE1.otp.clk;
`endif

`ifdef M4_SELECT
    `CHIP_CPU_BFM.set_rd_file_limit("ahb_rdata1.hex", 128/4,(128/4)+32);
    $readmemh("ahb_rdata1.hex", sram_dat); 
`else
    `CHIP_CPU_BFM.set_rd_file_limit("ahb_rdata.hex", 128/4,(128/4)+32);
    $readmemh("ahb_rdata.hex", sram_dat); 
`endif


    //---------------------- dma read rom -> ase(encrypt) -> dbus write
    $display("@%d, MSG: DMA READ SRAM -> AES(ENCRYPT) -> DBUS_WRITE", $time);
    `CHIP_CPU_BFM.ahbwr(`AES_INTR_REG, 3'd0, 4'd2, {31'h0,1'b1});
    bc_mode = 5'b00010;
    key_size = 2'd0;
    decrypt = 1'b0;
    `CHIP_CPU_BFM.ahbwr( `AES_CTRL_REG, 3'd0, 4'd2, {15'd0,decrypt,6'd0,key_size,3'd0,bc_mode});
    wdata_byte_num = 12'd63;
    dma_str_raddr = `SYSRAM_ADDR_BASE;
    dma_str_waddr = 17'd128;
    wdata_byte_num = 12'd63;
    rdata_byte_num = 12'd63;
    dma_enable = 1'b1;
    dbus_burst=1'b0;
    port_op_mode=2'd2;

    for (i=0; i<16; i=i+1) begin
	`CHIP_CPU_BFM.ahbwr(`SYSRAM_ADDR_BASE+(i*4), 3'd0, 4'd2, sram_dat[i+16]);
    end

    `CHIP_CPU_BFM.ahbwr( `AES_DMA_WADDR_REG, 3'd0, 4'd2, {15'd0,dma_str_waddr[16:0]});
    `CHIP_CPU_BFM.ahbwr( `AES_DMA_RADDR_REG, 3'd0, 4'd2, {15'd0,dma_str_raddr[16:0]});
    `CHIP_CPU_BFM.ahbwr( `AES_DMA_CTRL_REG, 3'd0, 4'd2, {dma_enable,1'd0,port_op_mode,rdata_byte_num[11:0],dbus_burst,3'd0,wdata_byte_num[11:0]}); // intr enable
    wait (`AES_INTR); 
    `CHIP_CPU_BFM.ahbwr( `AES_INTR_REG, 3'd0, 4'd2, 32'h0001_0001); 
    `CHIP_CPU_BFM.ahbwr( `AES_CTRL_REG, 3'd0, 4'd2, 32'h8000_0000);  // aes_clear


    `CHIP_CPU_BFM.ahbrd_incr(`SYSRAM_ADDR_BASE+dma_str_waddr, 1, 16, 2, rdata, 1, 32'h0, 0);
  
    // ---------------------- dma read rom -> ase(decrypt) -> dbus write
    $display("@%d, MSG: DMA READ SRAM -> AES(DECRYPT) -> DBUS_WRITE", $time);
    `CHIP_CPU_BFM.ahbwr( `CKGEN_SWRST_REG, 3'd0, 4'd2, 32'hffff_ffff & ~(32'h0020_0000)); // intr enable
    `CHIP_CPU_BFM.ahbwr( `CKGEN_SWRST_REG, 3'd0, 4'd2, 32'hffff_ffff | (32'h0020_0000)); // intr enable
  
    `CHIP_CPU_BFM.ahbwr( `AES_INTR_REG, 3'd0, 4'd2, 32'h0000_0001); // intr enable
    bc_mode = 5'b00010;
    key_size = 2'd0;
    decrypt = 1'b1;
    `CHIP_CPU_BFM.ahbwr( `AES_CTRL_REG, 3'd0, 4'd2, {15'd0,decrypt,6'd0,key_size,3'd0,bc_mode});
    dma_str_raddr = `SYSRAM_ADDR_BASE+`SYSRAM_SIZE-rdata_byte_num;
    dma_str_waddr = 17'd128+64;
    wdata_byte_num = 12'd63;
    rdata_byte_num = 12'd63;
    dma_enable = 1'b1;

    for (i=0; i<16; i=i+1) begin
        `CHIP_CPU_BFM.ahbwr(`SYSRAM_ADDR_BASE+`SYSRAM_SIZE-rdata_byte_num+(i*4), 3'd0, 4'd2, sram_dat[i]);
    end

    `CHIP_CPU_BFM.ahbwr( `AES_DMA_WADDR_REG, 3'd0, 4'd2, {15'd0,dma_str_waddr[16:0]});
    `CHIP_CPU_BFM.ahbwr( `AES_DMA_RADDR_REG, 3'd0, 4'd2, {15'd0,dma_str_raddr[16:0]});
    `CHIP_CPU_BFM.ahbwr( `AES_DMA_CTRL_REG, 3'd0, 4'd2, {dma_enable,1'd0,port_op_mode,rdata_byte_num[11:0],dbus_burst,3'd0,wdata_byte_num[11:0]}); // intr enable
    wait (`AES_INTR);
    `CHIP_CPU_BFM.ahbwr( `AES_INTR_REG, 3'd0, 4'd2, 32'h0001_0001);
    `CHIP_CPU_BFM.ahbwr( `AES_CTRL_REG, 3'd0, 4'd2, 32'h8000_0000);  // aec_clear

    `CHIP_CPU_BFM.ahbrd_incr(`SYSRAM_ADDR_BASE+dma_str_waddr, 1, 16, 2, rdata, 1, 32'h0, 0);
    

    repeat(100)@(posedge `CHIP_CLK48M);    
    $finish;
end

