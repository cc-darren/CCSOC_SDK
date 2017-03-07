// reuse-pragma startSub [ReplaceParameter -package ck_usartapb_geg0.inc -lib work -format verilog ALSB]
`define ALSB 2
//-----------------------------------------------------------------------------
// Test bench setup parameters
//-----------------------------------------------------------------------------
`include "apb_slave_timing.inc"     // APB slave timing definitions
`include "usart_slave_timing.inc"   // USART slave timing definitions
`include "ck_usartapb_geg0_params.v"   // 

// module register address definitions

parameter U1TBUF  =  32'h80+('h00 << `ALSB),   // xxx0*2
          U1RBUF  =  32'h80+('h01 << `ALSB),   // xxx2
          U1ICTRL =  32'h80+('h02 << `ALSB),   // xxx4
          U1STAT  =  32'h80+('h03 << `ALSB),   // xxx6
          U1FRS   =  32'h80+('h04 << `ALSB),   // xxx8
          U1MDSL  =  32'h80+('h05 << `ALSB),   // xxxA
          U1BAUD  =  32'h80+('h06 << `ALSB),   // xxxC
          U1PSR   =  32'h80+('h07 << `ALSB),   // xxxE
          U1OVR   =  32'h80+('h08 << `ALSB),   // xx10
          U1MDSL2 =  32'h80+('h09 << `ALSB),   // xx12
          U1SPOS  =  32'h80+('h0A << `ALSB),   // xx14
          U1WPSRH =  32'h80+('h0B << `ALSB),   // xx16
          U1WPSRL =  32'h80+('h0C << `ALSB),   // xx18
          U1WPCS  =  32'h80+('h0D << `ALSB),   // xx1A
          U1WLMN  =  32'h80+('h0E << `ALSB),   // xx1C
          U1WLMX  =  32'h80+('h0F << `ALSB),   // xx1E
          U1WPMN  =  32'h80+('h10 << `ALSB),   // xx20
          U1WPMX  =  32'h80+('h11 << `ALSB);   // xx22

reg [1:0] 	int_en = 2'b11;
reg		dual_bank = 1'b0;
reg		redu_en=1'b0;
reg [31:0]	rdata;

//topsys.chip.core_domain0.u0_uart_top_wrap.uart_top.uart_controller.xusart.xusartapb_core_1.xusartapb_bclk_1.baud_clk
initial begin
    $display("@%t, %m MSG: ****** test name: init_test_CPU_BFM", $time);
    wait (`TB_INIT_DONE);
    $display("@%d, %m MSG: TB_INIT_DONEROM", $time);


    @(posedge `CHIP_CLK48M);
   




//-----------------------------------------------------------------------------
//      1. Transmit and receive 8 data bits, 1 stop bit
//-----------------------------------------------------------------------------
  set_port_uart2;
  //program TRX interrupt enable  0: RX 1: TX
  `CHIP_CPU_BFM.ahbwr( `UART2_ADDR_BASE+32'h00,0,2,32'h0000_0003);
  `CHIP_CPU_BFM.ahbrd (`UART2_ADDR_BASE+32'h00,0,2,rdata,1,32'h0000_0003,0); 
  //program TRX ByteNum=4 byte
  `CHIP_CPU_BFM.ahbwr( `UART2_ADDR_BASE+32'h04,0,2,32'h0000_0303);
  `CHIP_CPU_BFM.ahbrd (`UART2_ADDR_BASE+32'h04,0,2,rdata,1,32'h0000_0303,0); 
  //program dma_rx_start_addr
  `CHIP_CPU_BFM.ahbwr( `UART2_ADDR_BASE+32'h08,0,2,`SYSRAM_ADDR_BASE+32'h1000);
  //program dma_rx_end_addr
  `CHIP_CPU_BFM.ahbwr( `UART2_ADDR_BASE+32'h0c,0,2,`SYSRAM_ADDR_BASE+32'h1004);
  //program dma_tx_start_addr
  `CHIP_CPU_BFM.ahbwr( `UART2_ADDR_BASE+32'h14,0,2,`SYSRAM_ADDR_BASE+32'h0000);
  //program dma_tx_end_addr
  `CHIP_CPU_BFM.ahbwr( `UART2_ADDR_BASE+32'h18,0,2,`SYSRAM_ADDR_BASE+32'h0004);
  //read RX addr in RAM location
  `CHIP_CPU_BFM.ahbwr( `UART2_ADDR_BASE+32'h20,0,2,32'h0);
  `CHIP_CPU_BFM.ahbrd( `UART2_ADDR_BASE+32'h20,0,2,rdata,1,32'h0,0);
  repeat(5)@(posedge `CHIP_CLK48M); 
  //read TX addr in RAM location
  `CHIP_CPU_BFM.ahbwr( `UART2_ADDR_BASE+32'h24,0,2,32'h0);
  `CHIP_CPU_BFM.ahbrd( `UART2_ADDR_BASE+32'h24,0,2,rdata,1,32'h0,0);
  repeat(5)@(posedge `CHIP_CLK48M); 
  // DMA TX data
  `CHIP_CPU_BFM.ahbwr( `SYSRAM_ADDR_BASE,0,2, 32'hA5A5_A5A5);
  `CHIP_CPU_BFM.ahbrd (`SYSRAM_ADDR_BASE,0,2,rdata,1,32'hA5A5_A5A5,0); 

    //**********program uart_ip*************
    // pck 
    // UnBAUD
  `CHIP_CPU_BFM.ahbwr (`UART2_ADDR_BASE+U1BAUD, 0,2, 32'h7c);		  // N=125,U1BAUD = 0x00 
    // UnPSR                                                                       
  `CHIP_CPU_BFM.ahbwr (`UART2_ADDR_BASE+U1PSR,  0,2, 32'h0|8'h4<<3);      // P=2.5,U1PSR = 0x08 
    // UnOVR                                                                       
  `CHIP_CPU_BFM.ahbwr (`UART2_ADDR_BASE+U1OVR,  0,2, 32'h10);		  // O=16, U1OVR = 0x10 
    //Default : UnMDSL1 
  `CHIP_CPU_BFM.ahbwr (`UART2_ADDR_BASE+U1MDSL,0,2,(32'h0|1'b1<<7|1'b1<<5|1'b1<<4)); // rx+tx dma DMA enabled
  repeat(5)@(posedge `CHIP_CLK48M); 
    // UnICTRL
  `CHIP_CPU_BFM.ahbwr (`UART2_ADDR_BASE+U1ICTRL,0,2,(32'h0|1'b1<<7|1'b1<<6|1'b1<<5|1'b1<<4));//  
    // 8-bit, 1 stp, parity=disabled
  `CHIP_CPU_BFM.ahbwr (`UART2_ADDR_BASE+U1FRS,  0,2, 32'h0000);		
    //*********end uart_ip programming*************
    //
    //
    //start DBUS RX-DMA
  //program dma_rx_rd_en
  `CHIP_CPU_BFM.ahbwr( `UART2_ADDR_BASE+32'h10,0,2,32'h0000_0001);
  `CHIP_CPU_BFM.ahbrd( `UART2_ADDR_BASE+32'h10,0,2,rdata,1,32'h1,0);
    //start DBUS TX-DMA
  //program dma_tx_rd_en
  `CHIP_CPU_BFM.ahbwr( `UART2_ADDR_BASE+32'h1c,0,2,32'h0000_0001);
  `CHIP_CPU_BFM.ahbrd( `UART2_ADDR_BASE+32'h1c,0,2,rdata,1,32'h1,0);

    //***************enable DMA*******************
 

  begin 
    for (integer i=0;i<4;i++)begin
        repeat(1)@(posedge `CHIP_CLK48M); 
        topsys.m_uart_bfm.u_uart_bfm[2].m_uart_bfm.Ucts_n_drive(0);
        topsys.m_uart_bfm.u_uart_bfm[2].m_uart_bfm.Async_check(0, 1, 13'h34A, 13'h34A, 31*(`TCLKL + `TCLKH)/2);
    end
  end
  fork
  //Check TX data and send RX data to DUT
    begin:tx_irt_timeout
      #3000ns;
//      repeat(500)@(posedge `CHIP_CLK48M); 
      disable tx_irt;
      $display("@%t, %m MSG: ERROR:TX_Interrupt timeout", $time);
    end
    begin:rx_irt_timeout
      #3000us;
//      repeat(1000)@(posedge `CHIP_CLK48M); 
      disable rx_irt;
      $display("@%t, %m MSG: ERROR:RX_Interrupt timeout", $time);
    end
    begin:tx_irt
      //wait for TX interrupt
      wait ( topsys.chip.core_domain0.uart2_txdma_int);  
      `CHIP_CPU_BFM.ahbwr( `UART2_ADDR_BASE+32'h1c,0,2,32'h0000_0000); 
      `CHIP_CPU_BFM.ahbwr( `UART2_ADDR_BASE+32'h00,0,2,32'h0002_0003);//clear TX DMA interrupt
      disable tx_irt_timeout;
    end 
    begin:rx_irt
      //wait for RX interrupt
      wait ( topsys.chip.core_domain0.uart2_rxdma_int);
      #200ns;
      `CHIP_CPU_BFM.ahbwr( `UART2_ADDR_BASE+32'h10,0,2,32'h0000_0000);
      `CHIP_CPU_BFM.ahbwr( `UART2_ADDR_BASE+32'h00,0,2,32'h0001_0003);//clear RX DMA interrupt
      disable rx_irt_timeout;
    end 
  join

      `CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h1000, 3'd0, 4'd2, rdata, 1, 32'hA5A5_A5A5, 1'b0);// check Sysram received data

      repeat(5)@(posedge `CHIP_CLK48M); 
      `CHIP_CPU_BFM.ahbrd (`UART2_ADDR_BASE+U1RBUF, 0,2,rdata,1,32'h00A5, 0); // check received data
      repeat(5)@(posedge `CHIP_CLK48M); 
      `CHIP_CPU_BFM.ahbrd (`UART2_ADDR_BASE+U1TBUF, 0,2,rdata,1,32'h00A5, 0); // check TBUF contents
      repeat(5)@(posedge `CHIP_CLK48M); 
      `CHIP_CPU_BFM.ahbrd (`UART2_ADDR_BASE+U1STAT, 0,2,rdata,1,32'h0000, 0); // transmission ended
      repeat(5)@(posedge `CHIP_CLK48M); 
      `CHIP_CPU_BFM.ahbrd (`UART2_ADDR_BASE+U1ICTRL,0,2,rdata,1,32'h00fd, 0); // check RBF reset
      repeat(5)@(posedge `CHIP_CLK48M); 


    //end UART - RX
  `CHIP_CPU_BFM.ahbwr( `UART2_ADDR_BASE+32'h04,0,2,32'h0007_00de);
  `CHIP_CPU_BFM.ahbwr( `UART2_ADDR_BASE+32'h10,0,2,32'h0);
    //end UART - TX
  `CHIP_CPU_BFM.ahbwr( `UART2_ADDR_BASE+32'h08,0,2,32'h0004_00de);
  `CHIP_CPU_BFM.ahbwr( `UART2_ADDR_BASE+32'h1C,0,2,32'h0);
  

      repeat(5)@(posedge `CHIP_CLK48M); 
      `CHIP_CPU_BFM.ahbrd (`UART2_ADDR_BASE+32'h00,0,2,rdata,1,32'h0000_0003,0);//clear interrupt status 


  `CHIP_CPU_BFM.ahbwr( `UART2_ADDR_BASE+32'h0,0,2,32'h0017_0101);

    repeat(100)@(posedge `CHIP_CLK48M);   
    //flush TX fifo
    //program flush dma_tx_rd_en
  `CHIP_CPU_BFM.ahbwr( `I2S_ADDR_BASE+32'h1c, 3'd0, 4'd2, 32'h0000_0100);
    repeat(5)@(posedge `CHIP_CLK48M); 
  `CHIP_CPU_BFM.ahbwr( `I2S_ADDR_BASE+32'h1c, 3'd0, 4'd2, 32'h0000_0000);



    //***************disable DMA*******************









 
    $finish;
end

// reuse-pragma startSub DoUndef [IncludeIf 1 -comment // %subText]
`undef ALSB
