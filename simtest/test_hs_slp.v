integer 	i;
reg [31:0]	rdata;

localparam      PWR_NOR = 2'd0,
                PWR_SLP = 2'd1,
                PWR_DSLP = 2'd2,
                PWR_HIB = 2'd3;

localparam      HS_RESET        = 0,
                HS_SCR_REQ	= 1,
                HS_DISABLE_PSO  = 2,
                HS_PWRUP_ACK    = 3,
                HS_SW_RESET     = 4,
                HS_NORMAL       = 5,
                HS_CPU_PWRDN    = 6,
                HS_DR_RD        = 7,
                HS_DEV_WR       = 8,
                HS_PWRDN_ACK    = 9,
                HS_SLEEP        = 10,
                HS_DSLEEP       = 11,
                HS_HIBERNATE    = 12,
                HS_DMA_INTR     = 13,
                HS_CPU_PWRUP    = 14;

localparam	RAM_ADDR_WIDTH = 11;


reg [1:0]	pwr_mode;
reg [RAM_ADDR_WIDTH-1:0]	dr_pdn_cnt;
reg [RAM_ADDR_WIDTH-1:0]	dr_pdn_st_addr;
reg [RAM_ADDR_WIDTH-1:0]	dr_pdn_cnt;
reg [RAM_ADDR_WIDTH-1:0]        dr0_pup_st_addr;
reg [RAM_ADDR_WIDTH-1:0]        dr0_pup_cnt;
reg [RAM_ADDR_WIDTH-1:0]        dr0_pup1_st_addr;
reg [RAM_ADDR_WIDTH-1:0]        dr0_pup1_cnt;
reg [4:0]			dr0_pup_id;
reg [4:0]			dr0_pup1_id;
reg [RAM_ADDR_WIDTH-1:0]        dr1_pup_st_addr;
reg [RAM_ADDR_WIDTH-1:0]        dr1_pup_cnt;
reg [RAM_ADDR_WIDTH-1:0]        dr1_pup1_st_addr;
reg [RAM_ADDR_WIDTH-1:0]        dr1_pup1_cnt;
reg [4:0]                       dr1_pup_id;
reg [4:0]                       dr1_pup1_id;
reg [RAM_ADDR_WIDTH-1:0]        dr2_pup_st_addr;
reg [RAM_ADDR_WIDTH-1:0]        dr2_pup_cnt;
reg [RAM_ADDR_WIDTH-1:0]        dr2_pup1_st_addr;
reg [RAM_ADDR_WIDTH-1:0]        dr2_pup1_cnt;
reg [4:0]                       dr2_pup_id;
reg [4:0]                       dr2_pup1_id;
/****
reg [RAM_ADDR_WIDTH*3-1:0]      dr_pup_st_addr;
reg [RAM_ADDR_WIDTH*3-1:0]      dr_pup_cnt;
reg [RAM_ADDR_WIDTH*3-1:0]      dr_pup1_st_addr;
reg [RAM_ADDR_WIDTH*3-1:0]      dr_pup1_cnt;
reg [3*4-1:0]                   dr_pup_id;
reg [3*4-1:0]                   dr_pup1_id;
****/
reg [3:0]			hs_pu_st_idx;
reg [3:0]			hs_pu_end_idx;
reg [3:0]			hs_pd_st_idx;
reg [3:0]			hs_pd_end_idx;

// i2c
reg [16:0]	dma_str_raddr;
reg [16:0]	dma_str_waddr;
reg [11:0]	wdata_byte_num;
reg [11:0]	rdata_byte_num;
reg [1:0]	port_op_mode;
reg [0:0]	dma_enable;
reg [0:0]	dbus_burst;
wire [6:0]      i2c_slv0_id = 7'h1f;
wire [1:0]      i2c_slv0_core_select = 2'b10;
wire [5:0]      i2c_slv0_mask = 6'd4;
wire            ms_addr_16bit = 1'b0;
wire            ms_address_en = 1'b0;
wire            ms_no_stop = 1'b0;
wire [6:0]      ms_slave_addr = i2c_slv0_id;
wire [15:0]     ms_word_addr=16'd0;
wire [11:0]     ms_prescaler=12'd492;
reg [5:0]  	ms_i2c_mask = 6'd3;
reg [11:0]      ofs;
reg [7:0]	ms_id=0; // 0:i2c0, 1: i2c1

// io
reg [4:0]     st_bit;
reg [3:0]     gpio_pin_en;
reg [3:0]     gpio_port_mode;
reg [3:0]     gpio_oe;

reg [31:0]	mask;


initial begin
    $display("@%t, %m MSG: ****** test name: test_hs_slp", $time);
    wait (`TB_INIT_DONE);
    $display("@%d, %m MSG: TB_INIT_DONEROM", $time);

    @(posedge `CHIP_CLK48M);

    // ----------------------------
    // wait for HS_NORMAL
    wait_hs_st(HS_NORMAL);

    // ----------------------------
    // hs clock same as sys_genclk
    //`CHIP_CPU_BFM.ahbwr(`SCU_HSCLK_REG, 3'd0, 4'd2, 32'h0000_0001);
    
    repeat(4) @(posedge `CHIP_CLK32K);
    // ----------------------------
    $display("@%d, MSG: Setup hs dr register", $time);
    pwr_mode=PWR_SLP;
    dr_pdn_cnt=2;
    dr_pdn_st_addr=512;
    dr0_pup_cnt=22;
    dr0_pup_st_addr=1024;
    dr0_pup1_cnt=6;
    dr0_pup1_st_addr=1536;
    dr0_pup_id = 5'd0;
    dr0_pup1_id = 5'd19;
    hs_pu_st_idx = 4'd1;
    hs_pu_end_idx = 4'd6;
    hs_pd_st_idx = 4'd7;
    hs_pd_end_idx = 4'd12;
 
    `CHIP_CPU_BFM.ahbwr(`HS_CONFIG_REG, 3'd0, 4'd2, {30'h0,pwr_mode});
    `CHIP_CPU_BFM.ahbrd(`HS_CONFIG_REG, 3'd0, 4'd2, rdata,1,{30'h0,pwr_mode},0);
  
    `CHIP_CPU_BFM.ahbwr(`HS_DR_PDN_REG, 3'd0, 4'd2, { {16-RAM_ADDR_WIDTH{1'b0}}, dr_pdn_cnt, {16-RAM_ADDR_WIDTH{1'b0}}, dr_pdn_st_addr});
    `CHIP_CPU_BFM.ahbrd(`HS_DR_PDN_REG, 3'd0, 4'd2, rdata,1,{ {16-RAM_ADDR_WIDTH{1'b0}}, dr_pdn_cnt, {16-RAM_ADDR_WIDTH{1'b0}}, dr_pdn_st_addr},0);

    `CHIP_CPU_BFM.ahbwr(`HS_DR0_PUP_REG, 3'd0, 4'd2, { {16-RAM_ADDR_WIDTH{1'b0}}, dr0_pup_cnt, {16-RAM_ADDR_WIDTH{1'b0}}, dr0_pup_st_addr});
    `CHIP_CPU_BFM.ahbrd(`HS_DR0_PUP_REG, 3'd0, 4'd2, rdata,1,{ {16-RAM_ADDR_WIDTH{1'b0}}, dr0_pup_cnt, {16-RAM_ADDR_WIDTH{1'b0}}, dr0_pup_st_addr},0);

    `CHIP_CPU_BFM.ahbwr(`HS_DR0_PUP1_REG, 3'd0, 4'd2, { {16-RAM_ADDR_WIDTH{1'b0}}, dr0_pup1_cnt, {16-RAM_ADDR_WIDTH{1'b0}}, dr0_pup1_st_addr});
    `CHIP_CPU_BFM.ahbrd(`HS_DR0_PUP1_REG, 3'd0, 4'd2, rdata,1,{ {16-RAM_ADDR_WIDTH{1'b0}}, dr0_pup1_cnt, {16-RAM_ADDR_WIDTH{1'b0}}, dr0_pup1_st_addr},0);

    `CHIP_CPU_BFM.ahbwr(`HS_DR0_INTR_REG, 3'd0, 4'd2, { 8'd0,3'd0,dr0_pup1_id,8'd0,3'd0,dr0_pup_id});
    `CHIP_CPU_BFM.ahbrd(`HS_DR0_INTR_REG, 3'd0, 4'd2, rdata,1,{8'd0,3'd0,dr0_pup1_id,8'd0,3'd0,dr0_pup_id},0);



    // ----------------------------
    $display("@%d, MSG: Setup hs scratch register", $time);
    `CHIP_CPU_BFM.ahbwr(`HS_SCRATCH_BASE_REG, 3'd0, 4'd2, {4'd0,hs_pd_end_idx,
                                                      4'd0,hs_pd_st_idx,
                                                      4'd0,hs_pu_end_idx,
                                                      4'd0,hs_pu_st_idx});

    // pwr up, ldo, iso, ret
    `CHIP_CPU_BFM.ahbwr(`HS_SCRATCH_BASE_REG+(hs_pu_st_idx*4), 3'd0, 4'd2, `SCU_PSOCTRL_REG);
    `CHIP_CPU_BFM.ahbwr(`HS_SCRATCH_BASE_REG+((hs_pu_st_idx+1)*4),3'd0, 4'd2, 32'h0);

    `CHIP_CPU_BFM.ahbwr(`HS_SCRATCH_BASE_REG+((hs_pu_st_idx+2)*4),3'd0, 4'd2, `SCU_ISOCTRL_REG);
    `CHIP_CPU_BFM.ahbwr(`HS_SCRATCH_BASE_REG+((hs_pu_st_idx+3)*4),3'd0, 4'd2, 32'h0000_0000);

    `CHIP_CPU_BFM.ahbwr(`HS_SCRATCH_BASE_REG+((hs_pu_st_idx+4)*4),3'd0, 4'd2, `SCU_RETCTRL_REG);
    `CHIP_CPU_BFM.ahbwr(`HS_SCRATCH_BASE_REG+((hs_pu_st_idx+5)*4),3'd0, 4'd2, 32'h0000_0000);

    //
    // pwr dn, ldo, iso, ret
    `CHIP_CPU_BFM.ahbwr(`HS_SCRATCH_BASE_REG+(hs_pd_st_idx*4),3'd0, 4'd2, `SCU_RETCTRL_REG);
    `CHIP_CPU_BFM.ahbwr(`HS_SCRATCH_BASE_REG+((hs_pd_st_idx+1)*4),3'd0, 4'd2, 32'h0000_0003);
    
    `CHIP_CPU_BFM.ahbwr(`HS_SCRATCH_BASE_REG+((hs_pd_st_idx+2)*4),3'd0, 4'd2, `SCU_ISOCTRL_REG);
    `CHIP_CPU_BFM.ahbwr(`HS_SCRATCH_BASE_REG+((hs_pd_st_idx+3)*4),3'd0, 4'd2, 32'hffff_ffff);
   
    `CHIP_CPU_BFM.ahbwr(`HS_SCRATCH_BASE_REG+((hs_pd_st_idx+4)*4),3'd0, 4'd2, `SCU_PSOCTRL_REG);
    `CHIP_CPU_BFM.ahbwr(`HS_SCRATCH_BASE_REG+((hs_pd_st_idx+5)*4),3'd0, 4'd2, 32'hffff_ffff);
    //
    //

    
    // ----------------------------
    // hs_clk=sys_genclk_ungated
    `CHIP_CPU_BFM.ahbwr( `SCU_HSCLK_REG, 3'd0, 4'd2, 32'h0000_0001);
     
    // ----------------------------
    // DATARAM
    $display("@%d, MSG: DATARAM SETUP for POWER DOWN", $time);

    // sys_genclk=pllclk, clk_en=0 
    ////`CHIP_CPU_BFM.ahbwr( `SCU_CLKCFG1_REG, 3'd0, 4'd2, 32'h0000_0001);
    `CHIP_CPU_BFM.ahbwr( `DATARAM_ADDR_BASE+dr_pdn_st_addr+32'h0, 3'd0, 4'd2, `SCU_CLKCFG1_REG); // cmd[0], wr
    `CHIP_CPU_BFM.ahbwr( `DATARAM_ADDR_BASE+dr_pdn_st_addr+32'h4, 3'd0, 4'd2, 32'h0000_0001);

    // ----------------------------
    $display("@%d, MSG: DATARAM SETUP for POWER UP", $time);

    // clock, hs can't change clock setting enable clock
    // enable sys_genclk to if sys_genclk is gated when going to SLEEP
    `CHIP_CPU_BFM.ahbwr( `DATARAM_ADDR_BASE+dr0_pup_st_addr+32'h0, 3'd0, 4'd2, `SCU_CLKCFG1_REG);
    `CHIP_CPU_BFM.ahbwr( `DATARAM_ADDR_BASE+dr0_pup_st_addr+32'h4, 3'd0, 4'd2, 32'h0000_0101);
 

    // io
    ////set_port_i2c0_12;
    //12, 4'h3, 4'd7, 4'd0
    st_bit = 12;
    gpio_pin_en = 3;
    gpio_port_mode = 7;
    gpio_oe = 0;

    mask = 32'b0 | (4'hf<<st_bit);

    `CHIP_CPU_BFM.ahbrd(`GPIO_PAD_PULLUP_REG, 3'd0, 4'd2, rdata, 0, rdata,1'b0);
    `CHIP_CPU_BFM.ahbwr( `DATARAM_ADDR_BASE+dr0_pup_st_addr+32'h8, 3'd0, 4'd2, `GPIO_PAD_PULLUP_REG);
    `CHIP_CPU_BFM.ahbwr( `DATARAM_ADDR_BASE+dr0_pup_st_addr+32'hc, 3'd0, 4'd2, (rdata & ~mask) | gpio_pin_en<<st_bit);

    `CHIP_CPU_BFM.ahbrd(`GPIO_AUX_REG, 3'd0, 4'd2, rdata, 0, rdata,1'b0);
    `CHIP_CPU_BFM.ahbwr( `DATARAM_ADDR_BASE+dr0_pup_st_addr+32'h10, 3'd0, 4'd2, `GPIO_AUX_REG);
    `CHIP_CPU_BFM.ahbwr( `DATARAM_ADDR_BASE+dr0_pup_st_addr+32'h14, 3'd0, 4'd2, (rdata & ~mask) | gpio_pin_en<<st_bit);

    `CHIP_CPU_BFM.ahbrd(`GPIO_AUX_PORT_MODE_REG, 3'd0, 4'd2, rdata, 0, rdata,1'b0);    // gpio port mode
    `CHIP_CPU_BFM.ahbwr(`DATARAM_ADDR_BASE+dr0_pup_st_addr+32'h18, 3'd0, 4'd2, `GPIO_AUX_PORT_MODE_REG);
    `CHIP_CPU_BFM.ahbwr(`DATARAM_ADDR_BASE+dr0_pup_st_addr+32'h1c, 3'd0, 4'd2, (rdata & ~mask) | gpio_port_mode<<st_bit);    // gpio port mode

    `CHIP_CPU_BFM.ahbrd(`GPIO_PAD_OE_REG, 3'd0,4'd2, rdata, 0, rdata, 1'b0);
    `CHIP_CPU_BFM.ahbwr(`DATARAM_ADDR_BASE+dr0_pup_st_addr+32'h20, 3'd0, 4'd2, `GPIO_PAD_OE_REG);
    `CHIP_CPU_BFM.ahbwr(`DATARAM_ADDR_BASE+dr0_pup_st_addr+32'h24, 3'd0,4'd2, (rdata & ~mask) | gpio_oe<<st_bit);


    // i2c
    dma_str_raddr = 17'd0;
    dma_str_waddr = 17'd512;
    wdata_byte_num = 12'd63;
    rdata_byte_num = 12'd63;
    port_op_mode = 2'd1;
    dma_enable = 1'b1;
    dbus_burst=1'b0;

    ofs = ms_id==0 ? 12'h0 : 12'h100;
    `CHIP_CPU_BFM.ahbwr(`DATARAM_ADDR_BASE+dr0_pup_st_addr+32'h28, 3'd0, 4'd2, `I2C0_ADDR_BASE+ofs+32'h0);
    `CHIP_CPU_BFM.ahbwr(`DATARAM_ADDR_BASE+dr0_pup_st_addr+32'h2c, 3'd0,4'd2, 32'h0000_0007);

    `CHIP_CPU_BFM.ahbwr(`DATARAM_ADDR_BASE+dr0_pup_st_addr+32'h30, 3'd0, 4'd2, `I2C0_ADDR_BASE+ofs+32'h14);
    `CHIP_CPU_BFM.ahbwr(`DATARAM_ADDR_BASE+dr0_pup_st_addr+32'h34, 3'd0, 4'd2, {4'd0,ms_prescaler[11:0],8'd0,ms_i2c_mask,2'd1});

    `CHIP_CPU_BFM.ahbwr(`DATARAM_ADDR_BASE+dr0_pup_st_addr+32'h38, 3'd0, 4'd2, `I2C0_ADDR_BASE+ofs+32'h18);
    `CHIP_CPU_BFM.ahbwr(`DATARAM_ADDR_BASE+dr0_pup_st_addr+32'h3c, 3'd0, 4'd2, {ms_word_addr,1'b0,i2c_slv0_id,5'd0,ms_no_stop,ms_address_en,ms_addr_16bit});

    `CHIP_CPU_BFM.ahbwr(`DATARAM_ADDR_BASE+dr0_pup_st_addr+32'h40, 3'd0, 4'd2, `I2C0_ADDR_BASE+ofs+32'hc);
    `CHIP_CPU_BFM.ahbwr(`DATARAM_ADDR_BASE+dr0_pup_st_addr+32'h44, 3'd0, 4'd2, {15'd0,dma_str_waddr[16:0]});

    `CHIP_CPU_BFM.ahbwr(`DATARAM_ADDR_BASE+dr0_pup_st_addr+32'h48, 3'd0, 4'd2, `I2C0_ADDR_BASE+ofs+32'h10);
    `CHIP_CPU_BFM.ahbwr(`DATARAM_ADDR_BASE+dr0_pup_st_addr+32'h4c, 3'd0, 4'd2, {15'd0,dma_str_raddr[16:0]});

    `CHIP_CPU_BFM.ahbwr(`DATARAM_ADDR_BASE+dr0_pup_st_addr+32'h50, 3'd0, 4'd2, `I2C0_ADDR_BASE+ofs+32'h8);
    `CHIP_CPU_BFM.ahbwr(`DATARAM_ADDR_BASE+dr0_pup_st_addr+32'h54, 3'd0, 4'd2, {dma_enable,1'd0,port_op_mode,rdata_byte_num[11:0],dbus_burst,3'd0,wdata_byte_num[11:0]});

    // ----------------------------
    $display("@%d, MSG: DATARAM SETUP for POWER UP1", $time);
    `CHIP_CPU_BFM.ahbwr( `DATARAM_ADDR_BASE+dr0_pup1_st_addr+32'h0, 3'd0, 4'd2, `SCU_CLKCFG1_REG); // cmd[0], wr
    `CHIP_CPU_BFM.ahbwr( `DATARAM_ADDR_BASE+dr0_pup1_st_addr+32'h4, 3'd0, 4'd2, 32'h0000_0101);
    
    `CHIP_CPU_BFM.ahbwr(`DATARAM_ADDR_BASE+dr0_pup1_st_addr+32'h8, 3'd0, 4'd2, `I2C0_ADDR_BASE+ofs+32'h0+1'b1); // read
    `CHIP_CPU_BFM.ahbwr(`DATARAM_ADDR_BASE+dr0_pup1_st_addr+32'hc, 3'd0, 4'd2, 32'h0001_0007);

    `CHIP_CPU_BFM.ahbwr(`DATARAM_ADDR_BASE+dr0_pup1_st_addr+32'h10, 3'd0, 4'd2, `I2C0_ADDR_BASE+ofs+32'h0); 
    `CHIP_CPU_BFM.ahbwr(`DATARAM_ADDR_BASE+dr0_pup1_st_addr+32'h14, 3'd0, 4'd2, 32'h0001_0007);

    // ----------------------------
    // CPU issue sleep
    set_cpu_wfi;
    repeat(2)@(posedge `CHIP_CLK32K);    

    // ----------------------------
    // wait for HS_SLEEP
    wait_hs_st(HS_SLEEP);
    // TODO block cpu see interrupt

    // ----------------------------
    // external interrupt
    set_wkup_intr_ext(14'd1);
    repeat(3)@(posedge `CHIP_CLK32K);    
    wait_hs_st(HS_DMA_INTR);
    repeat(3)@(posedge `CHIP_CLK32K);    
    wait_hs_st(HS_CPU_PWRUP);
    repeat(3)@(posedge `CHIP_CLK32K);
    unset_cpu_wfi; 
    force_trig_cpu_WAKEUP;
    wait_hs_st(HS_NORMAL);
    unset_wkup_intr_ext;

    // ----------------------------
    // internal interrupt
    ////wait(|`CHIP_CORE_AO.hs_top_wrap.intr_i);     
    repeat(3)@(posedge `CHIP_CLK32K);    

    // ----------------------------
    // wakeup interrupt
    repeat(100)@(posedge `CHIP_CLK48M);    
    $finish;
end

//// ----------------------------------------
task set_cpu_wfi;
begin
    $display("@%d, %m MSG: set_cpu_wfi", $time); 
`ifdef M4_SELECT
    force `CHIP_CORE1.m4_top.SLEEPDEEP=1'b1;
    wait(~`CHIP_CORE1.m4_top.SLEEPHOLDREQn);
    force `CHIP_CORE1.m4_top.SLEEPHOLDACKn = 1'b0;
`else
    force `CHIP_CORE1.m0p_top.SLEEPDEEP=1'b1;
    wait(~`CHIP_CORE1.m0p_top.SLEEPHOLDREQn);
    force `CHIP_CORE1.m0p_top.SLEEPHOLDACKn = 1'b0;
`endif

end
endtask
//// ----------------------------------------
task unset_cpu_wfi;
begin
    $display("@%d, %m MSG: unset_cpu_wfi", $time);
`ifdef M4_SELECT
    release `CHIP_CORE1.m4_top.SLEEPDEEP;
    release `CHIP_CORE1.m4_top.SLEEPHOLDACKn;
`else
    release `CHIP_CORE1.m0p_top.SLEEPDEEP;
    release `CHIP_CORE1.m0p_top.SLEEPHOLDACKn;
`endif
end
endtask

//// ----------------------------------------
task set_wkup_intr_ext;
input [13:0] intr_i;
begin
    // TODO: setup chip's ext0/ext_int
    $display("@%d, %m MSG: set_wkup_intr_ext", $time); 
    force `CHIP_CORE_AO.hs_top_wrap.intr_e = intr_i;
end
endtask
//// ----------------------------------------
task unset_wkup_intr_ext;
begin
    $display("@%d, %m MSG: unset_wkup_intr_ext", $time);
    release `CHIP_CORE_AO.hs_top_wrap.intr_e;
end
endtask
//// ----------------------------------------
task force_trig_cpu_WAKEUP;
begin
    $display("@%d, %m MSG: force_trig_cpu_WAKEUP", $time);
    `ifdef M4_SELECT
        force `CHIP_CORE1.M4_WAKEUP = 1'b1;
    `else
        force `CHIP_CORE1.M0P_WAKEUP = 1'b1;
    `endif
    wait_hs_st(HS_NORMAL);
    `ifdef M4_SELECT
        release `CHIP_CORE1.M4_WAKEUP;
    `else
        release `CHIP_CORE1.M0P_WAKEUP;
    `endif
end
endtask
//// ----------------------------------------
task wait_hs_st;
input [3:0]	mode;
begin
    $display("@%d, %m MSG: chk_pwr_sleep=%0d", $time, mode);
    wait(`CHIP_CORE_AO.hs_top_wrap.hs_top.hs_ctrl.hs_st==mode);
end
endtask
