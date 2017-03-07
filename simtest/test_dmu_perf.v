
`define SPIDATA   32'h12345678
`define SPIDATA_1 32'h09abcdef
`define SPIDATA_2 32'h55555555
`define SPIDATA_3 32'haaaaaaaa
`define SPIDATA_4 32'h5a5a5a5a
`define SPIDATA_5 32'ha5a5a5a5
`define SPIDATA_6 32'h11118888

reg [1:0] 	int_en = 2'b11;
reg		dual_bank = 1'b0;
reg		redu_en=1'b0;
reg [31:0]	rdata;
reg [31:0]	data;
reg [31:0]	addr=32'h8000_6000; 
integer		i;

wire spim_clk;// = topsys.chip.GPIO[0];
wire spim_cs ;// = topsys.chip.GPIO[1];
wire spim_d  ;// = topsys.chip.GPIO[2];
reg [31:0] spi_data = 0;

reg [5:0] spim_cnt = 0;

// TX byte swap
wire tx_byte_swap = 0;
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
////wire [11:0]     ms_prescaler=12'd492;
wire [11:0]     ms_prescaler=12'd52;
////wire [5:0]	ms_i2c_mask =6'd4;
wire [5:0]	ms_i2c_mask =6'd3;

initial begin
    $display("@%t, %m MSG: ****** test name: init_test_CPU_BFM", $time);
    wait (`TB_INIT_DONE);
    $display("@%d, %m MSG: TB_INIT_DONEROM", $time);

    // ROM
  //  $display("@%d, MSG: AHB READ ROM", $time);
  //  `CHIP_CPU_BFM.ahbrd( 32'h00, 0, 2, rdata, 1, 32'h0, 0);
  //  `CHIP_CPU_BFM.ahbrd_incr( 32'h00, 1, 16, 2, rdata, 1, 32'h0, 0);
  //  `CHIP_CPU_BFM.ahbrd( 32'h00, 2, 2, rdata, 1, 32'h0, 0);
  //  `CHIP_CPU_BFM.ahbrd( 32'h00, 3, 2, rdata, 1, 32'h0, 0);
  //  `CHIP_CPU_BFM.ahbrd( 32'h00, 4, 2, rdata, 1, 32'h0, 0);
  //  `CHIP_CPU_BFM.ahbrd( 32'h00, 5, 2, rdata, 1, 32'h0, 0);
  // SYSRAM
  data=0;
  for (integer j=0;j<16;j++)begin
    data++; 
    `CHIP_CPU_BFM.ahbwr( `SYSRAM_ADDR_BASE+j*4,0,2, data);
    `CHIP_CPU_BFM.ahbrd (`SYSRAM_ADDR_BASE+j*4,0,2,rdata,1,data,0); 
  end

    // EF init
    $display("@%d, MSG: AHB READ EF", $time);
    `CHIP_CPU_BFM.ahbwr(`EF_INTERRUPT_REG, 3'd0, 4'd2, {30'h0,int_en});
    `CHIP_CPU_BFM.ahbrd(`EF_INTERRUPT_REG, 3'd0, 4'd2, rdata,1,{30'h0,int_en},1'b0);
    `CHIP_CPU_BFM.ahbwr(`EF_CONFIG_REG, 3'd0, 4'd2, {16'h0,7'd0,redu_en,7'd0,dual_bank});
    `CHIP_CPU_BFM.ahbrd(`EF_CONFIG_REG, 3'd0, 4'd2, rdata,1,{16'h0,7'd0,redu_en,7'd0,dual_bank},1'b0);
    
  set_port_i2c0_12;
  set_port_i2c1_4;

  set_port_spim0_0();
  set_port_spim1_16();
  set_port_spim2_8();
  

  `CHIP_CPU_BFM.ahbwr(`SYSRAM_ADDR_BASE+32'h2000, 3'd0, 4'd2, `SPIDATA);
  `CHIP_CPU_BFM.ahbwr(`SYSRAM_ADDR_BASE+32'h2004, 3'd0, 4'd2, `SPIDATA_1);
  `CHIP_CPU_BFM.ahbwr(`SYSRAM_ADDR_BASE+32'h2008, 3'd0, 4'd2, `SPIDATA_2);
  `CHIP_CPU_BFM.ahbwr(`SYSRAM_ADDR_BASE+32'h200c, 3'd0, 4'd2, `SPIDATA_3);
  `CHIP_CPU_BFM.ahbwr(`SYSRAM_ADDR_BASE+32'h2010, 3'd0, 4'd2, `SPIDATA_4);
  `CHIP_CPU_BFM.ahbwr(`SYSRAM_ADDR_BASE+32'h2014, 3'd0, 4'd2, `SPIDATA_5);
  `CHIP_CPU_BFM.ahbwr(`SYSRAM_ADDR_BASE+32'h2018, 3'd0, 4'd2, `SPIDATA_6);
  `CHIP_CPU_BFM.ahbwr(`SYSRAM_ADDR_BASE+32'h201c, 3'd0, 4'd2, `SPIDATA_3);
  `CHIP_CPU_BFM.ahbwr(`SYSRAM_ADDR_BASE+32'h2020, 3'd0, 4'd2, `SPIDATA_4);
  `CHIP_CPU_BFM.ahbwr(`SYSRAM_ADDR_BASE+32'h2024, 3'd0, 4'd2, `SPIDATA_5);
  `CHIP_CPU_BFM.ahbwr(`SYSRAM_ADDR_BASE+32'h2028, 3'd0, 4'd2, `SPIDATA_6);
  `CHIP_CPU_BFM.ahbwr(`SYSRAM_ADDR_BASE+32'h202c, 3'd0, 4'd2, `SPIDATA_3);
  `CHIP_CPU_BFM.ahbwr(`SYSRAM_ADDR_BASE+32'h2030, 3'd0, 4'd2, `SPIDATA_4);
  `CHIP_CPU_BFM.ahbwr(`SYSRAM_ADDR_BASE+32'h2034, 3'd0, 4'd2, `SPIDATA_5);
  `CHIP_CPU_BFM.ahbwr(`SYSRAM_ADDR_BASE+32'h2038, 3'd0, 4'd2, `SPIDATA_6);
  `CHIP_CPU_BFM.ahbwr(`SYSRAM_ADDR_BASE+32'h203c, 3'd0, 4'd2, `SPIDATA_3);
  `CHIP_CPU_BFM.ahbwr(`SYSRAM_ADDR_BASE+32'h2040, 3'd0, 4'd2, `SPIDATA_4);
  `CHIP_CPU_BFM.ahbwr(`SYSRAM_ADDR_BASE+32'h2044, 3'd0, 4'd2, `SPIDATA_5);
  `CHIP_CPU_BFM.ahbwr(`SYSRAM_ADDR_BASE+32'h2048, 3'd0, 4'd2, `SPIDATA_6);
  `CHIP_CPU_BFM.ahbwr(`SYSRAM_ADDR_BASE+32'h204c, 3'd0, 4'd2, `SPIDATA_3);
  `CHIP_CPU_BFM.ahbwr(`SYSRAM_ADDR_BASE+32'h2050, 3'd0, 4'd2, `SPIDATA_4);
  `CHIP_CPU_BFM.ahbwr(`SYSRAM_ADDR_BASE+32'h2054, 3'd0, 4'd2, `SPIDATA_5);
  `CHIP_CPU_BFM.ahbwr(`SYSRAM_ADDR_BASE+32'h2058, 3'd0, 4'd2, `SPIDATA_6);
  `CHIP_CPU_BFM.ahbwr(`SYSRAM_ADDR_BASE+32'h205c, 3'd0, 4'd2, `SPIDATA_3);
  `CHIP_CPU_BFM.ahbwr(`SYSRAM_ADDR_BASE+32'h2060, 3'd0, 4'd2, `SPIDATA_4);
  `CHIP_CPU_BFM.ahbwr(`SYSRAM_ADDR_BASE+32'h2064, 3'd0, 4'd2, `SPIDATA_5);
  `CHIP_CPU_BFM.ahbwr(`SYSRAM_ADDR_BASE+32'h2068, 3'd0, 4'd2, `SPIDATA_6);
  `CHIP_CPU_BFM.ahbwr(`SYSRAM_ADDR_BASE+32'h206c, 3'd0, 4'd2, `SPIDATA_3);
  `CHIP_CPU_BFM.ahbwr(`SYSRAM_ADDR_BASE+32'h2070, 3'd0, 4'd2, `SPIDATA_4);
  `CHIP_CPU_BFM.ahbwr(`SYSRAM_ADDR_BASE+32'h2074, 3'd0, 4'd2, `SPIDATA_5);
  `CHIP_CPU_BFM.ahbwr(`SYSRAM_ADDR_BASE+32'h2078, 3'd0, 4'd2, `SPIDATA_6);
  `CHIP_CPU_BFM.ahbwr(`SYSRAM_ADDR_BASE+32'h207c, 3'd0, 4'd2, `SPIDATA_3);
  `CHIP_CPU_BFM.ahbwr(`SYSRAM_ADDR_BASE+32'h2080, 3'd0, 4'd2, `SPIDATA_4);
  `CHIP_CPU_BFM.ahbwr(`SYSRAM_ADDR_BASE+32'h2084, 3'd0, 4'd2, `SPIDATA_5);
  `CHIP_CPU_BFM.ahbwr(`SYSRAM_ADDR_BASE+32'h2088, 3'd0, 4'd2, `SPIDATA_6);
  `CHIP_CPU_BFM.ahbwr(`SYSRAM_ADDR_BASE+32'h208c, 3'd0, 4'd2, `SPIDATA_3);
  `CHIP_CPU_BFM.ahbwr(`SYSRAM_ADDR_BASE+32'h2090, 3'd0, 4'd2, `SPIDATA_4);
  `CHIP_CPU_BFM.ahbwr(`SYSRAM_ADDR_BASE+32'h2094, 3'd0, 4'd2, `SPIDATA_5);
  `CHIP_CPU_BFM.ahbwr(`SYSRAM_ADDR_BASE+32'h2098, 3'd0, 4'd2, `SPIDATA_6);
  `CHIP_CPU_BFM.ahbwr(`SYSRAM_ADDR_BASE+32'h209c, 3'd0, 4'd2, `SPIDATA_3);
  `CHIP_CPU_BFM.ahbwr(`SYSRAM_ADDR_BASE+32'h20a0, 3'd0, 4'd2, `SPIDATA_4);
  `CHIP_CPU_BFM.ahbwr(`SYSRAM_ADDR_BASE+32'h20a4, 3'd0, 4'd2, `SPIDATA_5);
  `CHIP_CPU_BFM.ahbwr(`SYSRAM_ADDR_BASE+32'h20a8, 3'd0, 4'd2, `SPIDATA_6);
  `CHIP_CPU_BFM.ahbwr(`SYSRAM_ADDR_BASE+32'h20ac, 3'd0, 4'd2, `SPIDATA_3);
  `CHIP_CPU_BFM.ahbwr(`SYSRAM_ADDR_BASE+32'h20b0, 3'd0, 4'd2, `SPIDATA_4);
  `CHIP_CPU_BFM.ahbwr(`SYSRAM_ADDR_BASE+32'h20b4, 3'd0, 4'd2, `SPIDATA_5);
  `CHIP_CPU_BFM.ahbwr(`SYSRAM_ADDR_BASE+32'h20b8, 3'd0, 4'd2, `SPIDATA_6);
  `CHIP_CPU_BFM.ahbwr(`SYSRAM_ADDR_BASE+32'h20bc, 3'd0, 4'd2, `SPIDATA_3);
  `CHIP_CPU_BFM.ahbwr(`SYSRAM_ADDR_BASE+32'h20c0, 3'd0, 4'd2, `SPIDATA_4);
  `CHIP_CPU_BFM.ahbwr(`SYSRAM_ADDR_BASE+32'h20c4, 3'd0, 4'd2, `SPIDATA_5);
  `CHIP_CPU_BFM.ahbwr(`SYSRAM_ADDR_BASE+32'h20c8, 3'd0, 4'd2, `SPIDATA_6);
  `CHIP_CPU_BFM.ahbwr(`SYSRAM_ADDR_BASE+32'h20cc, 3'd0, 4'd2, `SPIDATA_3);
  `CHIP_CPU_BFM.ahbwr(`SYSRAM_ADDR_BASE+32'h20d0, 3'd0, 4'd2, `SPIDATA_4);
  `CHIP_CPU_BFM.ahbwr(`SYSRAM_ADDR_BASE+32'h20d4, 3'd0, 4'd2, `SPIDATA_5);
  `CHIP_CPU_BFM.ahbwr(`SYSRAM_ADDR_BASE+32'h20d8, 3'd0, 4'd2, `SPIDATA_6);
  `CHIP_CPU_BFM.ahbwr(`SYSRAM_ADDR_BASE+32'h20dc, 3'd0, 4'd2, `SPIDATA_3);
  `CHIP_CPU_BFM.ahbwr(`SYSRAM_ADDR_BASE+32'h20e0, 3'd0, 4'd2, `SPIDATA_4);
  `CHIP_CPU_BFM.ahbwr(`SYSRAM_ADDR_BASE+32'h20e4, 3'd0, 4'd2, `SPIDATA_5);
  `CHIP_CPU_BFM.ahbwr(`SYSRAM_ADDR_BASE+32'h20e8, 3'd0, 4'd2, `SPIDATA_6);
  `CHIP_CPU_BFM.ahbwr(`SYSRAM_ADDR_BASE+32'h20ec, 3'd0, 4'd2, `SPIDATA_3);
  `CHIP_CPU_BFM.ahbwr(`SYSRAM_ADDR_BASE+32'h20f0, 3'd0, 4'd2, `SPIDATA_4);
  `CHIP_CPU_BFM.ahbwr(`SYSRAM_ADDR_BASE+32'h20f4, 3'd0, 4'd2, `SPIDATA_5);
  `CHIP_CPU_BFM.ahbwr(`SYSRAM_ADDR_BASE+32'h20f8, 3'd0, 4'd2, `SPIDATA_6);
  `CHIP_CPU_BFM.ahbwr(`SYSRAM_ADDR_BASE+32'h20fc, 3'd0, 4'd2, `SPIDATA_3);

  // SPI0 master setting 
  // program cpha/cpol == 3'b00 , cs = 2'b11
  `CHIP_CPU_BFM.ahbwr(`SPI0_ADDR_BASE+32'h04, 3'd0, 4'd2, 32'h0000_0400 | 1'b1 << 24); 
  // program write starting address
  `CHIP_CPU_BFM.ahbwr(`SPI0_ADDR_BASE+32'h0c, 3'd0, 4'd2, `SYSRAM_ADDR_BASE+32'h1000); 
  // program read starting address
  `CHIP_CPU_BFM.ahbwr(`SPI0_ADDR_BASE+32'h10, 3'd0, 4'd2, `SYSRAM_ADDR_BASE+32'h2000); 
  // program interrupt enable
  `CHIP_CPU_BFM.ahbwr(`SPI0_ADDR_BASE+32'h00, 3'd0, 4'd2, 32'h003); 

  //enable SPI write_only mode (op_mode =0,swap=1, w=1byte)
  `CHIP_CPU_BFM.ahbwr(`SPI0_ADDR_BASE+32'h08, 3'd0, 4'd2, 32'h0000_00ff | tx_byte_swap <<18 | 1'b0 <<24); 

  `CHIP_CPU_BFM.ahbwr(`DMU_ADDR_BASE+32'h08, 3'd0, 4'd2, 32'h0000_0010); 



   // SPI1 master setting 
  // program cpha/cpol == 3'b00 , cs = 2'b11
  `CHIP_CPU_BFM.ahbwr(`SPI1_ADDR_BASE+32'h04, 3'd0, 4'd2, 32'h0000_0400 | 1'b1 << 24); 
  // program write starting address
  `CHIP_CPU_BFM.ahbwr(`SPI1_ADDR_BASE+32'h0c, 3'd0, 4'd2, `SYSRAM_ADDR_BASE+32'h1000); 
  // program read starting address
  `CHIP_CPU_BFM.ahbwr(`SPI1_ADDR_BASE+32'h10, 3'd0, 4'd2, `SYSRAM_ADDR_BASE+32'h2000); 
  // program interrupt enable
  `CHIP_CPU_BFM.ahbwr(`SPI1_ADDR_BASE+32'h00, 3'd0, 4'd2, 32'h003); 

  //enable SPI write_only mode (op_mode =0,swap=1, w=1byte)
  `CHIP_CPU_BFM.ahbwr(`SPI1_ADDR_BASE+32'h08, 3'd0, 4'd2, 32'h0000_00ff | tx_byte_swap <<18 | 1'b0 <<24); 

  `CHIP_CPU_BFM.ahbwr(`DMU_ADDR_BASE+32'h28, 3'd0, 4'd2, 32'h0000_0010); 

  // SPI2 master setting 
  // program cpha/cpol == 3'b00 , cs = 2'b11
  `CHIP_CPU_BFM.ahbwr(`SPI2_ADDR_BASE+32'h04, 3'd0, 4'd2, 32'h0000_0400 | 1'b1 << 24); 
  // program write starting address
  `CHIP_CPU_BFM.ahbwr(`SPI2_ADDR_BASE+32'h0c, 3'd0, 4'd2, `SYSRAM_ADDR_BASE+32'h1000); 
  // program read starting address
  `CHIP_CPU_BFM.ahbwr(`SPI2_ADDR_BASE+32'h10, 3'd0, 4'd2, `SYSRAM_ADDR_BASE+32'h2000); 
  // program interrupt enable
  `CHIP_CPU_BFM.ahbwr(`SPI2_ADDR_BASE+32'h00, 3'd0, 4'd2, 32'h003); 

  //enable SPI write_only mode (op_mode =0,swap=1, w=1byte)
  `CHIP_CPU_BFM.ahbwr(`SPI2_ADDR_BASE+32'h08, 3'd0, 4'd2, 32'h0000_00ff | tx_byte_swap <<18 | 1'b0 <<24); 


  `CHIP_CPU_BFM.ahbwr(`DMU_ADDR_BASE+32'h48, 3'd0, 4'd2, 32'h0000_0010); 

  // DMU enable
  `CHIP_CPU_BFM.ahbwr(`DMU_ADDR_BASE+32'h00, 3'd0, 4'd2, 32'h0000_0001); 
  `CHIP_CPU_BFM.ahbwr(`DMU_ADDR_BASE+32'h0c, 3'd0, 4'd2, 32'h0000_0000 | 1'b1 <<10); 
  // topsys.s_spi_bfm.s_spi_bfm[0].genblk1.s_spi_bfm.spi_s_wr(4 , tx_byte_swap, `SPIDATA);
  // DMU enable
  `CHIP_CPU_BFM.ahbwr(`DMU_ADDR_BASE+32'h20, 3'd0, 4'd2, 32'h0000_0001); 
  `CHIP_CPU_BFM.ahbwr(`DMU_ADDR_BASE+32'h2c, 3'd0, 4'd2, 32'h0000_0000 | 1'b1 <<10); 
  // topsys.s_spi_bfm.s_spi_bfm[1].genblk1.s_spi_bfm.spi_s_wr(4 , tx_byte_swap, `SPIDATA_2);
  // DMU enable
  `CHIP_CPU_BFM.ahbwr(`DMU_ADDR_BASE+32'h40, 3'd0, 4'd2, 32'h0000_0001); 
  `CHIP_CPU_BFM.ahbwr(`DMU_ADDR_BASE+32'h4c, 3'd0, 4'd2, 32'h0000_0000 | 1'b1 <<10); 
  // wait interrupt


    // DMU enable
    `CHIP_CPU_BFM.ahbwr(`DMU_ADDR_BASE+32'h60, 3'd0, 4'd2, 32'h0000_0001); 
    `CHIP_CPU_BFM.ahbwr(`DMU_ADDR_BASE+32'h6c, 3'd0, 4'd2, 32'h0000_0000 | 1'b1 <<10); 



    dma_str_raddr = 17'd0;
    dma_str_waddr = 17'd512;
    wdata_byte_num = 12'd63;
    rdata_byte_num = 12'd63;
    port_op_mode = 2'd0;
    dma_enable = 1'b0;
    dbus_burst=1'b0;
    fork
    begin
      topsys.s_i2c_bfm.s_i2c_bfm[0].s_i2c_bfm.i2c_wr_address_check(7'h1f);
      data=0;
      for (integer j=0;j<64;j++)begin
        if (j%4==0)begin
          data++;         
          topsys.s_i2c_bfm.s_i2c_bfm[0].s_i2c_bfm.i2c_wr_data_check(data[7:0],0);
        end
        else if (j%4!=0)begin
          if (j==63) topsys.s_i2c_bfm.s_i2c_bfm[0].s_i2c_bfm.i2c_wr_data_check(0,1);
          else       topsys.s_i2c_bfm.s_i2c_bfm[0].s_i2c_bfm.i2c_wr_data_check(0,0);
        end
      end
    end
      i2c0_wr(dma_str_raddr,dma_str_waddr,wdata_byte_num,rdata_byte_num,port_op_mode,dma_enable,dbus_burst);
    join 


    // DMU enable
    `CHIP_CPU_BFM.ahbwr(`DMU_ADDR_BASE+32'h80, 3'd0, 4'd2, 32'h0000_0001); 
    `CHIP_CPU_BFM.ahbwr(`DMU_ADDR_BASE+32'h8c, 3'd0, 4'd2, 32'h0000_0000 | 1'b1 <<10); 

    dma_str_raddr = 17'd0;
    dma_str_waddr = 17'd512;
    wdata_byte_num = 12'd63;
    rdata_byte_num = 12'd63;
    port_op_mode = 2'd0;
    dma_enable = 1'b0;
    dbus_burst=1'b0;
    fork
    begin
      topsys.s_i2c_bfm.s_i2c_bfm[1].s_i2c_bfm.i2c_wr_address_check(7'h1f);
      data=0;
      for (integer j=0;j<64;j++)begin
        if (j%4==0)begin
          data++;         
          topsys.s_i2c_bfm.s_i2c_bfm[1].s_i2c_bfm.i2c_wr_data_check(data[7:0],0);
        end
        else if (j%4!=0)begin
          if (j==63) topsys.s_i2c_bfm.s_i2c_bfm[1].s_i2c_bfm.i2c_wr_data_check(0,1);
          else       topsys.s_i2c_bfm.s_i2c_bfm[1].s_i2c_bfm.i2c_wr_data_check(0,0);
        end
      end
    end
      i2c1_wr(dma_str_raddr,dma_str_waddr,wdata_byte_num,rdata_byte_num,port_op_mode,dma_enable,dbus_burst);
    join 

  wait(`SPI0_M_INTR);
  `CHIP_CPU_BFM.ahbwr(`SPI0_ADDR_BASE+32'h00, 3'd0, 4'd2, 32'h1_0001); 
  wait(~`SPI0_M_INTR);

  // wait interrupt
  wait(`SPI1_M_INTR);
  `CHIP_CPU_BFM.ahbwr(`SPI1_ADDR_BASE+32'h00, 3'd0, 4'd2, 32'h1_0001); 
  wait(~`SPI1_M_INTR);

  wait(`SPI2_M_INTR);
  `CHIP_CPU_BFM.ahbwr(`SPI2_ADDR_BASE+32'h00, 3'd0, 4'd2, 32'h1_0001); 
  wait(~`SPI2_M_INTR);

    repeat(1500)@(posedge `CHIP_CLK48M);    

  wait(topsys.chip.core_domain0.dmu_int);

  `CHIP_CPU_BFM.ahbwr(`DMU_ADDR_BASE+32'h00, 3'd0, 4'd2, 32'h0000_0001 | 1'b1 <<16); 
  `CHIP_CPU_BFM.ahbwr(`DMU_ADDR_BASE+32'h0c, 3'd0, 4'd2, 32'h0000_0000 | 1'b1 <<11 |1'b0 <<10); 
  `CHIP_CPU_BFM.ahbwr(`DMU_ADDR_BASE+32'h20, 3'd0, 4'd2, 32'h0000_0001 | 1'b1 <<16); 
  `CHIP_CPU_BFM.ahbwr(`DMU_ADDR_BASE+32'h2c, 3'd0, 4'd2, 32'h0000_0000 | 1'b1 <<11 |1'b0 <<10); 
  `CHIP_CPU_BFM.ahbwr(`DMU_ADDR_BASE+32'h40, 3'd0, 4'd2, 32'h0000_0001 | 1'b1 <<16); 
  `CHIP_CPU_BFM.ahbwr(`DMU_ADDR_BASE+32'h4c, 3'd0, 4'd2, 32'h0000_0000 | 1'b1 <<11 |1'b0 <<10); 
  `CHIP_CPU_BFM.ahbwr(`DMU_ADDR_BASE+32'h60, 3'd0, 4'd2, 32'h0000_0001 | 1'b1 <<16); 
  `CHIP_CPU_BFM.ahbwr(`DMU_ADDR_BASE+32'h6c, 3'd0, 4'd2, 32'h0000_0000 | 1'b1 <<11 |1'b0 <<10); 
  `CHIP_CPU_BFM.ahbwr(`DMU_ADDR_BASE+32'h80, 3'd0, 4'd2, 32'h0000_0001 | 1'b1 <<16); 
  `CHIP_CPU_BFM.ahbwr(`DMU_ADDR_BASE+32'h8c, 3'd0, 4'd2, 32'h0000_0000 | 1'b1 <<11 |1'b0 <<10); 

    repeat(50)@(posedge `CHIP_CLK48M);    
 // wait(~topsys.chip.core_domain0.dmu_int);


    repeat(15000)@(posedge `CHIP_CLK48M);    
    $finish;
end
task i2c0_wr;
input [16:0] dma_str_raddr_t;
input [16:0] dma_str_waddr_t;
input [11:0] wdata_byte_num_t;
input [11:0] rdata_byte_num_t;
input [1:0] port_op_mode_t;
input [0:0] dma_enable_t;
input [0:0] dbus_burst_t;
reg [6:0] ahb_burst;
begin
    $display("@%d, MSG: I2C0 READ -> DBUS_WRITE", $time);

    `CHIP_CPU_BFM.ahbwr( `I2C0_ADDR_BASE+32'h0, 3'd0, 4'd2, 32'h0000_0007); // intr enable
    `CHIP_CPU_BFM.ahbrd( `I2C0_ADDR_BASE+32'h0, 3'd0, 4'd2, rdata, 1, 32'h0000_0007, 1'b0); // intr enable
    `CHIP_CPU_BFM.ahbwr( `I2C0_ADDR_BASE+32'h14, 3'd0, 4'd2, {4'd0,ms_prescaler[11:0],8'd0,ms_i2c_mask,2'd1});
    `CHIP_CPU_BFM.ahbrd( `I2C0_ADDR_BASE+32'h14, 3'd0, 4'd2, rdata, 1, {4'd0,ms_prescaler[11:0],8'd0,ms_i2c_mask,2'd1}, 1'b0); // intr enable
    `CHIP_CPU_BFM.ahbwr( `I2C0_ADDR_BASE+32'h18, 3'd0, 4'd2, {ms_word_addr,1'b0,i2c_slv0_id,5'd0,ms_no_stop,ms_address_en,ms_addr_16bit});
    `CHIP_CPU_BFM.ahbrd( `I2C0_ADDR_BASE+32'h18, 3'd0, 4'd2, rdata, 1, {ms_word_addr,1'b0,i2c_slv0_id,5'd0,ms_no_stop,ms_address_en,ms_addr_16bit},1'b0);

    `CHIP_CPU_BFM.ahbwr(`I2C0_ADDR_BASE+32'hc, 3'd0, 4'd2, {15'd0,dma_str_waddr_t[16:0]});
    `CHIP_CPU_BFM.ahbwr(`I2C0_ADDR_BASE+32'h10, 3'd0, 4'd2, {15'd0,dma_str_raddr_t[16:0]});
    `CHIP_CPU_BFM.ahbwr(`I2C0_ADDR_BASE+32'h8, 3'd0, 4'd2, {dma_enable_t,1'd0,port_op_mode_t,rdata_byte_num_t[11:0],dbus_burst_t,3'd0,wdata_byte_num_t[11:0]}); // intr enable
    wait (`I2C0_INTR);
    `CHIP_CPU_BFM.ahbrd(`I2C0_ADDR_BASE+32'h0, 3'd0, 4'd2, rdata, 1, 32'h0001_0007, 1'b0);
    `CHIP_CPU_BFM.ahbwr(`I2C0_ADDR_BASE+32'h0, 3'd0, 4'd2, 32'h0001_0007);
//    wait (`I2C0_INTR==0);
//    `CHIP_CPU_BFM.ahbwr(`I2C0_ADDR_BASE+32'h8, 3'd0, 4'd2, {dma_enable_t,1'd0,port_op_mode_t,rdata_byte_num_t[11:0],dbus_burst_t,3'd0,wdata_byte_num_t[11:0]}); // intr enable
//

end
endtask
// -----------------------------------
task i2c1_wr;
input [16:0] dma_str_raddr_t;
input [16:0] dma_str_waddr_t;
input [11:0] wdata_byte_num_t;
input [11:0] rdata_byte_num_t;
input [1:0] port_op_mode_t;
input [0:0] dma_enable_t;
input [0:0] dbus_burst_t;
reg [6:0] ahb_burst;

begin
    $display("@%d, MSG: I2C1 READ -> DBUS_WRITE", $time);

    `CHIP_CPU_BFM.ahbwr( `I2C1_ADDR_BASE+32'h0, 3'd0, 4'd2, 32'h0000_0007); // intr enable
    `CHIP_CPU_BFM.ahbrd( `I2C1_ADDR_BASE+32'h0, 3'd0, 4'd2, rdata, 1, 32'h0000_0007, 1'b0); // intr enable
    `CHIP_CPU_BFM.ahbwr( `I2C1_ADDR_BASE+32'h14, 3'd0, 4'd2, {4'd0,ms_prescaler[11:0],8'd0,ms_i2c_mask,2'd1});
    `CHIP_CPU_BFM.ahbrd( `I2C1_ADDR_BASE+32'h14, 3'd0, 4'd2, rdata, 1, {4'd0,ms_prescaler[11:0],8'd0,ms_i2c_mask,2'd1}, 1'b0); // intr enable
    `CHIP_CPU_BFM.ahbwr( `I2C1_ADDR_BASE+32'h18, 3'd0, 4'd2, {ms_word_addr,1'b0,i2c_slv0_id,5'd0,ms_no_stop,ms_address_en,ms_addr_16bit});
    `CHIP_CPU_BFM.ahbrd( `I2C1_ADDR_BASE+32'h18, 3'd0, 4'd2, rdata, 1, {ms_word_addr,1'b0,i2c_slv0_id,5'd0,ms_no_stop,ms_address_en,ms_addr_16bit},1'b0);

    `CHIP_CPU_BFM.ahbwr(`I2C1_ADDR_BASE+32'hc, 3'd0, 4'd2, {15'd0,dma_str_waddr_t[16:0]});
    `CHIP_CPU_BFM.ahbwr(`I2C1_ADDR_BASE+32'h10, 3'd0, 4'd2, {15'd0,dma_str_raddr_t[16:0]});
    `CHIP_CPU_BFM.ahbwr(`I2C1_ADDR_BASE+32'h8, 3'd0, 4'd2, {dma_enable_t,1'd0,port_op_mode_t,rdata_byte_num_t[11:0],dbus_burst_t,3'd0,wdata_byte_num_t[11:0]}); // intr enable
    wait (`I2C1_INTR);
    `CHIP_CPU_BFM.ahbrd(`I2C1_ADDR_BASE+32'h0, 3'd0, 4'd2, rdata, 1, 32'h0001_0007, 1'b0);
    `CHIP_CPU_BFM.ahbwr(`I2C1_ADDR_BASE+32'h0, 3'd0, 4'd2, 32'h0001_0007);

end
endtask
// -----------------------------------

