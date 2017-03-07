#include "stdint.h"
#include "addr_map.h"

// reuse-pragma startSub [ReplaceParameter -package ck_usartapb_geg0.inc -lib work -format verilog ALSB]
#define ALSB 2
//-----------------------------------------------------------------------------
// Test bench setup parameters
//-----------------------------------------------------------------------------
//`include "apb_slave_timing.inc"     // APB slave timing definitions
//`include "usart_slave_timing.inc"   // USART slave timing definitions
//`include "ck_usartapb_geg0_params.v"   // 

// module register address definitions

#define U1TBUF      (0x80+(0x00 << ALSB))   // xxx0*2
#define U1RBUF      (0x80+(0x01 << ALSB))   // xxx2
#define U1ICTRL     (0x80+(0x02 << ALSB))   // xxx4
#define U1STAT      (0x80+(0x03 << ALSB))   // xxx6
#define U1FRS       (0x80+(0x04 << ALSB))   // xxx8
#define U1MDSL      (0x80+(0x05 << ALSB))   // xxxA
#define U1BAUD      (0x80+(0x06 << ALSB))   // xxxC
#define U1PSR       (0x80+(0x07 << ALSB))   // xxxE
#define U1OVR       (0x80+(0x08 << ALSB))   // xx10
#define U1MDSL2     (0x80+(0x09 << ALSB))   // xx12
#define U1SPOS      (0x80+(0x0A << ALSB))   // xx14
#define U1WPSRH     (0x80+(0x0B << ALSB))   // xx16
#define U1WPSRL     (0x80+(0x0C << ALSB))   // xx18
#define U1WPCS      (0x80+(0x0D << ALSB))   // xx1A
#define U1WLMN      (0x80+(0x0E << ALSB))   // xx1C
#define U1WLMX      (0x80+(0x0F << ALSB))   // xx1E
#define U1WPMN      (0x80+(0x10 << ALSB))   // xx20
#define U1WPMX      (0x80+(0x11 << ALSB))   // xx22

//reg [1:0] 	int_en = 2'b11;
//reg		dual_bank = 1'b0;
//reg		redu_en=1'b0;
static uint32_t rdata;                      //32bit
//reg [31:0]	data=32'h0;
//real       period=666.88;
//reg pclk_f;
//reg [7:0] dword=32;

extern uint32_t UART0_RXDM_INTR;
extern uint32_t UART0_TXDM_INTR;
extern void set_port_uart0(void);
extern int rd_compare(uint32_t addr, uint32_t val, uint32_t compare_val);
extern int rd_mask_compare(uint32_t addr, uint32_t val, uint32_t compare_val, uint32_t mask);

#if 0
initial begin
   #1 pclk_f= 1'b0;

    forever begin
        #((period)/2) pclk_f=!pclk_f;
      end

end
#endif

int test_uart_rx_irt_byte0(void) {
    int ret = 0;

    //$display("@%t, %m MSG: ****** test name: init_test_CPU_BFM", $time);
    //wait (`TB_INIT_DONE);
    //$display("@%d, %m MSG: TB_INIT_DONEROM", $time);


    //@(posedge `CHIP_CLK48M);
    delayns(1);

    wr(SYSRAM_TEST_ADDR_BASE+0x1000, 0x0);
    wr(SYSRAM_TEST_ADDR_BASE+0x1004, 0x0);
    wr(SYSRAM_TEST_ADDR_BASE+0x1008, 0x0);
    wr(SYSRAM_TEST_ADDR_BASE+0x100c, 0x0);

//-----------------------------------------------------------------------------
//      1. Transmit and receive 8 data bits, 1 stop bit
//-----------------------------------------------------------------------------
    set_port_uart0();
    //program TRX interrupt enable  0: RX 1: TX
    //`CHIP_CPU_BFM.ahbwr( `UART0_ADDR_BASE+32'h00,0,2,32'h0000_0003);
    wr(UART0_ADDR_BASE+0x00, 0x00000003);
    //`CHIP_CPU_BFM.ahbrd (`UART0_ADDR_BASE+32'h00,0,2,rdata,1,32'h0000_0003,0); 
    ret = rd_compare(UART0_ADDR_BASE+0x00, rdata, 0x00000003);
    if (ret == 0) return -1; 
    //program TRX ByteNum=1 byte
    //`CHIP_CPU_BFM.ahbwr( `UART0_ADDR_BASE+32'h04,0,2,32'h0000_0000);
    wr(UART0_ADDR_BASE+0x04, 0x00000000);
    //`CHIP_CPU_BFM.ahbrd (`UART0_ADDR_BASE+32'h04,0,2,rdata,1,32'h0000_0000,0);
    ret = rd_compare(UART0_ADDR_BASE+0x04, rdata, 0x00000000);
    if (ret == 0) return -1; 
//  //program TRX ByteNum=2 byte 
//  `CHIP_CPU_BFM.ahbwr( `UART0_ADDR_BASE+32'h04,0,2,32'h0000_0101);
//  `CHIP_CPU_BFM.ahbrd (`UART0_ADDR_BASE+32'h04,0,2,rdata,1,32'h0000_0101,0); 
//  //program TRX ByteNum=3 byte 
//  `CHIP_CPU_BFM.ahbwr( `UART0_ADDR_BASE+32'h04,0,2,32'h0000_0202);
//  `CHIP_CPU_BFM.ahbrd (`UART0_ADDR_BASE+32'h04,0,2,rdata,1,32'h0000_0202,0); 
//  //program TRX ByteNum=17 byte 
//  `CHIP_CPU_BFM.ahbwr( `UART0_ADDR_BASE+32'h04,0,2,32'h0000_1010);
//  `CHIP_CPU_BFM.ahbrd (`UART0_ADDR_BASE+32'h04,0,2,rdata,1,32'h0000_1010,0); 
//  //program TRX ByteNum=18 byte 
//  `CHIP_CPU_BFM.ahbwr( `UART0_ADDR_BASE+32'h04,0,2,32'h0000_1111);
//  `CHIP_CPU_BFM.ahbrd (`UART0_ADDR_BASE+32'h04,0,2,rdata,1,32'h0000_1111,0); 
//  //program TRX ByteNum=19 byte 
//  `CHIP_CPU_BFM.ahbwr( `UART0_ADDR_BASE+32'h04,0,2,32'h0000_1212);
//  `CHIP_CPU_BFM.ahbrd (`UART0_ADDR_BASE+32'h04,0,2,rdata,1,32'h0000_1212,0); 

    //program dma_rx_start_addr
    //`CHIP_CPU_BFM.ahbwr( `UART0_ADDR_BASE+32'h08,0,2,`SYSRAM_ADDR_BASE+32'h1000);
    wr(UART0_ADDR_BASE+0x08, SYSRAM_TEST_ADDR_BASE+0x1000);
    //program dma_rx_end_addr
    //`CHIP_CPU_BFM.ahbwr( `UART0_ADDR_BASE+32'h0c,0,2,`SYSRAM_ADDR_BASE+32'h1000+4*4);
    wr(UART0_ADDR_BASE+0x0c, SYSRAM_TEST_ADDR_BASE+0x1000+4*4);
    //program dma_tx_start_addr
    //`CHIP_CPU_BFM.ahbwr( `UART0_ADDR_BASE+32'h14,0,2,`SYSRAM_ADDR_BASE+32'h0000);
    wr(UART0_ADDR_BASE+0x14, SYSRAM_TEST_ADDR_BASE+0x00);
    //program dma_tx_end_addr
    //CHIP_CPU_BFM.ahbwr( `UART0_ADDR_BASE+32'h18,0,2,`SYSRAM_ADDR_BASE+32'h0000+4*4);
    wr(UART0_ADDR_BASE+0x18, SYSRAM_TEST_ADDR_BASE+0x0+4*4);
    //read RX addr in RAM location
    //`CHIP_CPU_BFM.ahbrd( `UART0_ADDR_BASE+32'h20,0,2,rdata,1,32'h0,0);
    rd(UART0_ADDR_BASE+0x20, rdata);
    //repeat(5)@(posedge `CHIP_CLK48M);
    delayns(5);
    //read TX addr in RAM location
    //`CHIP_CPU_BFM.ahbrd( `UART0_ADDR_BASE+32'h24,0,2,rdata,1,32'h0,0);
    rd(UART0_ADDR_BASE+0x24, rdata);
    //repeat(5)@(posedge `CHIP_CLK48M); 
    delayns(5);
    // DMA TX data
    //for (integer j=0;j<4;j++)begin
    //  data++; 
    //  `CHIP_CPU_BFM.ahbwr( `SYSRAM_ADDR_BASE+j*4,0,2, data);
    //  `CHIP_CPU_BFM.ahbrd (`SYSRAM_ADDR_BASE+j*4,0,2,rdata,1,data,0); 
    //end
    //`CHIP_CPU_BFM.ahbwr( `SYSRAM_ADDR_BASE,0,2, 32'h87654321);
    wr(SYSRAM_TEST_ADDR_BASE, 0x87654321);
    //`CHIP_CPU_BFM.ahbrd (`SYSRAM_ADDR_BASE,0,2,rdata,1,32'h87654321,0);
    rd(SYSRAM_TEST_ADDR_BASE, rdata); 

    //**********program uart_ip*************
    // pck 
    // UnBAUD
    //`CHIP_CPU_BFM.ahbwr (`UART0_ADDR_BASE+U1BAUD, 0,2, 32'h0);		  // N=125, U1BAUD = 0x00 
    wr(UART0_ADDR_BASE+U1BAUD, 0x0);
    // UnPSR                                                                       
    //`CHIP_CPU_BFM.ahbwr (`UART0_ADDR_BASE+U1PSR,  0,2, 32'h18);      // P=2.5, U1PSR = 0x08 
    wr(UART0_ADDR_BASE+U1PSR, 0x18);
    // UnOVR                                                                       
    //`CHIP_CPU_BFM.ahbwr (`UART0_ADDR_BASE+U1OVR,  0,2, 32'h0);		  // O=16,  U1OVR = 0x10     
    wr(UART0_ADDR_BASE+U1OVR, 0x0); 
    //Default : UnMDSL1 
    //`CHIP_CPU_BFM.ahbwr (`UART0_ADDR_BASE+U1MDSL,0,2,(32'h0|1'b1<<7|1'b1<<5|1'b1<<4)); // rx+tx dma DMA enabled
    wr(UART0_ADDR_BASE+U1MDSL, (0x0 | 1<<7 | 1<<5 | 1<<4));
    //repeat(5)@(posedge `CHIP_CLK48M); 
    delayns(5);
    // UnICTRL
    //`CHIP_CPU_BFM.ahbwr (`UART0_ADDR_BASE+U1ICTRL,0,2,(32'h0|1'b1<<7|1'b1<<6|1'b1<<5|1'b1<<4));//  
    wr(UART0_ADDR_BASE+U1ICTRL, (0x0 | 1<<7 | 1<<6 | 1<<5 | 1<<4));
    // 8-bit, 1 stp, parity=disabled
    //`CHIP_CPU_BFM.ahbwr (`UART0_ADDR_BASE+U1FRS,  0,2, 32'h0000);
    wr(UART0_ADDR_BASE+U1FRS, 0x00);
    //*********end uart_ip programming*************
    //
    //
    //start DBUS RX-DMA
    //program dma_rx_rd_en
    //`CHIP_CPU_BFM.ahbwr( `UART0_ADDR_BASE+32'h10,0,2,32'h0000_0001);
    wr(UART0_ADDR_BASE+0x10, 0x00000001);
    //`CHIP_CPU_BFM.ahbrd( `UART0_ADDR_BASE+32'h10,0,2,rdata,1,32'h1,0);
    rd(UART0_ADDR_BASE+0x10, rdata);
    //start DBUS TX-DMA
    //program dma_tx_rd_en
    //`CHIP_CPU_BFM.ahbwr( `UART0_ADDR_BASE+32'h1c,0,2,32'h0000_0001);
    wr(UART0_ADDR_BASE+0x1c, 0x00000001);
    //`CHIP_CPU_BFM.ahbrd( `UART0_ADDR_BASE+32'h1c,0,2,rdata,1,32'h1,0);
    rd(UART0_ADDR_BASE+0x1c, rdata);

    //***************enable DMA*******************
#if 0
  data=0;
    force topsys.m_uart_bfm.u_uart_bfm[0].m_uart_bfm.clk_phi1_c=pclk_f; 

  begin 
    for (integer j=0;j<1;j++)begin
      data++;
      for (integer i=0;i+j*4<j*4+4;i++)begin
        topsys.m_uart_bfm.u_uart_bfm[0].m_uart_bfm.Ucts_n_drive(0);
        if (j==0)begin
          if (i==0) data=8'h21;
          else if (i==1) data=8'h43;
          else if (i==2) data=8'h65;
          else if (i==3) data=8'h87;
            topsys.m_uart_bfm.u_uart_bfm[0].m_uart_bfm.Async_receive(0,1,0,0,0,0,0,0,{4'b1/*stop*/,data,1'b0/*start*/},31*(`TCLKL + `TCLKH)/2);
        end
        else if (i%4!=0)begin
 	    topsys.m_uart_bfm.u_uart_bfm[0].m_uart_bfm.Async_receive(0,1,0,0,0,0,0,0,{4'b1/*stop*/,8'b0,1'b0/*start*/},31*(`TCLKL + `TCLKH)/2);
        end
	    else begin
 	    topsys.m_uart_bfm.u_uart_bfm[0].m_uart_bfm.Async_receive(0,1,0,0,0,0,0,0,{4'b1/*stop*/,data,1'b0/*start*/},31*(`TCLKL + `TCLKH)/2);
        end
 //     if (i+j*4==1)begin
        fork
        //Check TX data and send RX data to DUT
          begin:tx_irt_timeout
            repeat(15000)@(posedge `CHIP_CLK48M); 
            disable tx_irt;
            $display("@%t, %m MSG: ERROR:TX_Interrupt timeout", $time);
          end
          begin:tx_irt
            //wait for TX interrupt
            wait ( topsys.chip.core_domain0.uart0_txdma_int);  
            `CHIP_CPU_BFM.ahbwr( `UART0_ADDR_BASE+32'h00,0,2,32'h0002_0003);//clear TX DMA interrupt
            disable tx_irt_timeout;
          end 
        
        //Check TX data and send RX data to DUT
          begin:rx_irt_timeout
            repeat(15000)@(posedge `CHIP_CLK48M); 
            disable rx_irt;
            $display("@%t, %m MSG: ERROR:RX_Interrupt timeout", $time);
          end
          begin:rx_irt
            //wait for RX interrupt
            wait ( topsys.chip.core_domain0.uart0_rxdma_int);
            repeat(20)@(posedge `CHIP_CLK48M); 
            `CHIP_CPU_BFM.ahbwr( `UART0_ADDR_BASE+32'h00,0,2,32'h0001_0003);//clear RX DMA interrupt
            disable rx_irt_timeout;
          end 
        join_none
//      end
    end
    end
  end
#endif

    while(!UART0_RXDM_INTR);
    UART0_RXDM_INTR = 0;

    // check Sysram received data
    //data=0;
    //repeat(20)@(posedge `CHIP_CLK48M); 
    delayns(20);
    //repeat(500)@(posedge `CHIP_CLK48M); 
    delayns(500);
    //`CHIP_CPU_BFM.ahbrd (`SYSRAM_ADDR_BASE+32'h1000,0,2,rdata,1,32'hxxxxxx21,0); 
    rd_mask_compare(SYSRAM_TEST_ADDR_BASE+0x1000, rdata, 0x21, 0xff);
    //`CHIP_CPU_BFM.ahbrd (`SYSRAM_ADDR_BASE+32'h1004,0,2,rdata,1,32'hxxxxxx43,0); 
    rd_mask_compare(SYSRAM_TEST_ADDR_BASE+0x1004, rdata, 0x43, 0xff);
    //`CHIP_CPU_BFM.ahbrd (`SYSRAM_ADDR_BASE+32'h1008,0,2,rdata,1,32'hxxxxxx65,0);
    rd_mask_compare(SYSRAM_TEST_ADDR_BASE+0x1008, rdata, 0x65, 0xff);
    //`CHIP_CPU_BFM.ahbrd (`SYSRAM_ADDR_BASE+32'h100c,0,2,rdata,1,32'hxxxxxx87,0);
    rd_mask_compare(SYSRAM_TEST_ADDR_BASE+0x100c, rdata, 0x87, 0xff);


    //repeat(5)@(posedge `CHIP_CLK48M);
    delayns(5);
    //`CHIP_CPU_BFM.ahbrd( `UART0_ADDR_BASE+32'h20,0,2,rdata,0,rdata,0);
    rd(UART0_ADDR_BASE+0x20, rdata);
    if ( ((rdata>>24) && 0x03) == 1) {
        //`CHIP_CPU_BFM.ahbrd( `UART0_ADDR_BASE+32'h28,0,2,rdata,0,rdata,0);
        rd(UART0_ADDR_BASE+0x28, rdata);
        //if (rdata[7:0]!=8'h87)$display("@%t, %m MSG: ERROR:DMA rx rdata[7:0]=%h is wrong", $time, rdata[7:0]);
        if ( (rdata&0xff) != 0x87) return -1;
    }
    else if ( ((rdata>>24) && 0x03) == 2 ){
        //`CHIP_CPU_BFM.ahbrd( `UART0_ADDR_BASE+32'h28,0,2,rdata,0,rdata,0);
        rd(UART0_ADDR_BASE+0x28, rdata);
        //if (rdata[15:0]!=16'h8765)$display("@%t, %m MSG: ERROR:DMA rx rdata[15:0]=%h is wrong", $time, rdata[15:0]);
        if ( (rdata&0xffff) != 0x8765) return -1;
    }
    else if ( ((rdata>>24) && 0x03) == 3){
        //`CHIP_CPU_BFM.ahbrd( `UART0_ADDR_BASE+32'h28,0,2,rdata,0,rdata,0);
        rd(UART0_ADDR_BASE+0x28, rdata);
        //if(rdata[23:0]!=24'h876543)$display("@%t, %m MSG: ERROR:DMA rx rdata[23:0]=%h is wrong", $time, rdata[23:0]);
        if ( (rdata&0xffffff) != 0x876543) return -1;
    }
    else if ( ((rdata>>24) && 0x03) == 0){
        //`CHIP_CPU_BFM.ahbrd( `UART0_ADDR_BASE+32'h28,0,2,rdata,0,rdata,0);
        //if(rdata[31:0]!=31'h87)$display("@%t, %m MSG: ERROR:DMA rx rdata[31:0]=%h is wrong", $time, rdata[31:0]);
    	return -1;
    }
    else {
    	//$display("@%t, %m MSG: ERROR:DMA pbcnt is wrong", $time);
    	return -1;
    }


    //repeat(5)@(posedge `CHIP_CLK48M);
    delayns(500);
    //`CHIP_CPU_BFM.ahbrd (`UART0_ADDR_BASE+U1RBUF, 0,2,rdata,1,32'h0087, 0); // check received data
    rd_compare(UART0_ADDR_BASE+U1RBUF, rdata, 0x87);
    //repeat(500)@(posedge `CHIP_CLK48M); 
    delayns(50);
    //`CHIP_CPU_BFM.ahbrd (`UART0_ADDR_BASE+U1STAT, 0,2,rdata,1,32'h0000, 0); // transmission ended
    rd_compare(UART0_ADDR_BASE+U1STAT, rdata, 0x00);
    //repeat(5)@(posedge `CHIP_CLK48M); 
    delayns(5);


    //end UART - RX
    //`CHIP_CPU_BFM.ahbwr( `UART0_ADDR_BASE+32'h04,0,2,32'h0007_00de);
    wr(UART0_ADDR_BASE+0x04, 0x000700de);
    //`CHIP_CPU_BFM.ahbwr( `UART0_ADDR_BASE+32'h10,0,2,32'h0);
    wr(UART0_ADDR_BASE+0x10, 0x0);
    //end UART - TX
    //`CHIP_CPU_BFM.ahbwr( `UART0_ADDR_BASE+32'h08,0,2,32'h0004_00de);
    wr(UART0_ADDR_BASE+0x08, 0x000400de);
    //`CHIP_CPU_BFM.ahbwr( `UART0_ADDR_BASE+32'h1C,0,2,32'h0);
    wr(UART0_ADDR_BASE+0x1C, 0x00);
  

    //`CHIP_CPU_BFM.ahbwr( `UART0_ADDR_BASE+32'h0,0,2,32'h0017_0101);
    wr(UART0_ADDR_BASE+0x0, 0x00170101);

    //repeat(100)@(posedge `CHIP_CLK48M);
    delayns(10);
    //flush TX fifo
    //program flush dma_tx_rd_en
    //`CHIP_CPU_BFM.ahbwr( `UART0_ADDR_BASE+32'h1c, 3'd0, 4'd2, 32'h0000_0100);
    wr(UART0_ADDR_BASE+0x1c, 0x00000100);
    //repeat(5)@(posedge `CHIP_CLK48M); 
    delayns(5);
    //`CHIP_CPU_BFM.ahbwr( `UART0_ADDR_BASE+32'h1c, 3'd0, 4'd2, 32'h0000_0000);
    wr(UART0_ADDR_BASE+0x1c, 0x00000000);

    //repeat(500)@(posedge `CHIP_CLK48M);   


    //***************disable DMA*******************
 
    //$finish;

    return ret;
}

// reuse-pragma startSub DoUndef [IncludeIf 1 -comment // %subText]
#undef ALSB
