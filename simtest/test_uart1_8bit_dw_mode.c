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

extern volatile uint32_t UART1_RXDM_INTR;
extern volatile uint32_t UART1_TXDM_INTR;
extern void set_port_uart1(void);
extern int rd_compare(uint32_t addr, uint32_t val, uint32_t compare_val);
extern int rd_mask_compare(uint32_t addr, uint32_t val, uint32_t compare_val, uint32_t mask);

//topsys.chip.core_domain0.u0_uart_top_wrap.uart_top.uart_controller.xusart.xusartapb_core_1.xusartapb_bclk_1.baud_clk
int test_uart1_8bit_dw_mode(void) {
    int ret = 0;

    //$display("@%t, %m MSG: ****** test name: init_test_CPU_BFM", $time);
    //wait (`TB_INIT_DONE);
    //$display("@%d, %m MSG: TB_INIT_DONEROM", $time);


    //@(posedge `CHIP_CLK48M);
    delayns(1);


wr(0x2000E000, 0x01);
//-----------------------------------------------------------------------------
//      1. Transmit and receive 8 data bits, 1 stop bit
//-----------------------------------------------------------------------------
    set_port_uart1();
    //program TRX interrupt enable  0: RX 1: TX
    //`CHIP_CPU_BFM.ahbwr( `UART1_ADDR_BASE+32'h00,0,2,32'h0000_0003);
    wr(UART1_ADDR_BASE+0x00, 0x00000003);
    //`CHIP_CPU_BFM.ahbrd (`UART1_ADDR_BASE+32'h00,0,2,rdata,1,32'h0000_0003,0);
    rd_compare(UART1_ADDR_BASE+0x00, rdata, 0x00000003); 
    //program TRX ByteNum=4 byte
    //`CHIP_CPU_BFM.ahbwr( `UART1_ADDR_BASE+32'h04,0,2,32'h0000_0303);
    wr(UART1_ADDR_BASE+0x04, 0x00000303);
    //`CHIP_CPU_BFM.ahbrd (`UART1_ADDR_BASE+32'h04,0,2,rdata,1,32'h0000_0303,0); 
    rd_compare(UART1_ADDR_BASE+0x04, rdata, 0x00000303); 
    //program dma_rx_start_addr
    //`CHIP_CPU_BFM.ahbwr( `UART1_ADDR_BASE+32'h08,0,2,`SYSRAM_ADDR_BASE+32'h1000);
    wr(UART1_ADDR_BASE+0x08, SYSRAM_ADDR_BASE+0x1000);
    //program dma_rx_end_addr
    //`CHIP_CPU_BFM.ahbwr( `UART1_ADDR_BASE+32'h0c,0,2,`SYSRAM_ADDR_BASE+32'h1004);
    wr(UART1_ADDR_BASE+0x0c, SYSRAM_ADDR_BASE+0x1004);
    //program dma_tx_start_addr
    //`CHIP_CPU_BFM.ahbwr( `UART1_ADDR_BASE+32'h14,0,2,`SYSRAM_ADDR_BASE+32'h0000);
    wr(UART1_ADDR_BASE+0x14, SYSRAM_TEST_ADDR_BASE+0x00);
    //program dma_tx_end_addr
    //`CHIP_CPU_BFM.ahbwr( `UART1_ADDR_BASE+32'h18,0,2,`SYSRAM_ADDR_BASE+32'h0004);
    wr(UART1_ADDR_BASE+0x18, SYSRAM_TEST_ADDR_BASE+0x0004);
    //read RX addr in RAM location
    //`CHIP_CPU_BFM.ahbwr( `UART1_ADDR_BASE+32'h20,0,2,32'h0);
    wr(UART1_ADDR_BASE+0x20, 0x0);
    //`CHIP_CPU_BFM.ahbrd( `UART1_ADDR_BASE+32'h20,0,2,rdata,1,32'h0,0);
    rd_compare(UART1_ADDR_BASE+0x20, rdata, 0x0);
    //repeat(5)@(posedge `CHIP_CLK48M);
    delayns(5);
    //read TX addr in RAM location
    //`CHIP_CPU_BFM.ahbwr( `UART1_ADDR_BASE+32'h24,0,2,32'h0);
    wr(UART1_ADDR_BASE+0x24, 0x0);
    //`CHIP_CPU_BFM.ahbrd( `UART1_ADDR_BASE+32'h24,0,2,rdata,1,32'h0,0);
    rd_compare(UART1_ADDR_BASE+0x24, rdata, 0x0);
    //repeat(5)@(posedge `CHIP_CLK48M); 
    delayns(5);
    // DMA TX data
    //`CHIP_CPU_BFM.ahbwr( `SYSRAM_ADDR_BASE,0,2, 32'hA5A5_A5A5);
    wr(SYSRAM_TEST_ADDR_BASE, 0xA5A5A5A5);
    //`CHIP_CPU_BFM.ahbrd (`SYSRAM_ADDR_BASE,0,2,rdata,1,32'hA5A5_A5A5,0);
    rd_compare(SYSRAM_TEST_ADDR_BASE, rdata, 0xA5A5A5A5); 
wr(0x2000E000, 0x02);

    //**********program uart_ip*************
    // pck 
    // UnBAUD
    //`CHIP_CPU_BFM.ahbwr (`UART1_ADDR_BASE+U1BAUD, 0,2, 32'h7c);		  // N=125,  U1BAUD = 0x00 
    wr(UART1_ADDR_BASE+U1BAUD, 0x7c);
    // UnPSR                                                                       
    //`CHIP_CPU_BFM.ahbwr (`UART1_ADDR_BASE+U1PSR,  0,2, 32'h0|8'h4<<3);      // P=2.5,  U1PSR = 0x08 
    wr(UART1_ADDR_BASE+U1PSR, (0x0 | 0x4<<3));
    // UnOVR                                                                       
    //`CHIP_CPU_BFM.ahbwr (`UART1_ADDR_BASE+U1OVR,  0,2, 32'h10);		  // O=16,   U1OVR = 0x10
    wr(UART1_ADDR_BASE+U1OVR, 0x10);
    //Default : UnMDSL1 
    //`CHIP_CPU_BFM.ahbwr (`UART1_ADDR_BASE+U1MDSL,0,2,(32'h0|1'b1<<7|1'b1<<5|1'b1<<4)); // rx+tx dma DMA enabled
    wr(UART1_ADDR_BASE+U1MDSL, (0x0 | 1<<7 | 1<<5 | 1<<4));
    //repeat(5)@(posedge `CHIP_CLK48M);
    delayns(5);
    // UnICTRL
    //`CHIP_CPU_BFM.ahbwr (`UART1_ADDR_BASE+U1ICTRL,0,2,(32'h0|1'b1<<7|1'b1<<6|1'b1<<5|1'b1<<4));//  
    wr(UART1_ADDR_BASE+U1ICTRL, (0x0 | 1<<7 | 1<<6 | 1<<5 | 1<<4));  
    // 8-bit, 1 stp, parity=disabled
    //`CHIP_CPU_BFM.ahbwr (`UART1_ADDR_BASE+U1FRS,  0,2, 32'h0000);
    wr(UART1_ADDR_BASE+U1FRS, 0x0000);
wr(0x2000E000, 0x03);

    //*********end uart_ip programming*************
    //
    //
    //start DBUS RX-DMA
    //program dma_rx_rd_en
    //`CHIP_CPU_BFM.ahbwr( `UART1_ADDR_BASE+32'h10,0,2,32'h0000_0001);
    wr(UART1_ADDR_BASE+0x10, 0x00000001);
    //`CHIP_CPU_BFM.ahbrd( `UART1_ADDR_BASE+32'h10,0,2,rdata,1,32'h1,0);
    rd_compare(UART1_ADDR_BASE+0x10, rdata, 0x1);
    //start DBUS TX-DMA
    //program dma_tx_rd_en
    //`CHIP_CPU_BFM.ahbwr( `UART1_ADDR_BASE+32'h1c,0,2,32'h0000_0001);
    wr(UART1_ADDR_BASE+0x1c, 0x00000001);
    //`CHIP_CPU_BFM.ahbrd( `UART1_ADDR_BASE+32'h1c,0,2,rdata,1,32'h1,0);
    rd_compare(UART1_ADDR_BASE+0x1c, rdata, 0x1);
wr(0x2000E000, 0x04);

    //***************enable DMA*******************
#if 0
  begin 
    for (integer i=0;i<4;i++)begin
        repeat(1)@(posedge `CHIP_CLK48M); 
        topsys.m_uart_bfm.u_uart_bfm[1].m_uart_bfm.Ucts_n_drive(0);
        topsys.m_uart_bfm.u_uart_bfm[1].m_uart_bfm.Async_check(0, 1, 13'h34A, 13'h34A, 31*(`TCLKL + `TCLKH)/2);
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
      wait ( topsys.chip.core_domain0.uart1_txdma_int); 
      `CHIP_CPU_BFM.ahbwr( `UART1_ADDR_BASE+32'h1c,0,2,32'h0000_0000);  
      `CHIP_CPU_BFM.ahbwr( `UART1_ADDR_BASE+32'h00,0,2,32'h0002_0003);//clear TX DMA interrupt
      disable tx_irt_timeout;
    end 
    begin:rx_irt
      //wait for RX interrupt
      wait ( topsys.chip.core_domain0.uart1_rxdma_int);
      #200ns;
      `CHIP_CPU_BFM.ahbwr( `UART1_ADDR_BASE+32'h10,0,2,32'h0000_0000);
      `CHIP_CPU_BFM.ahbwr( `UART1_ADDR_BASE+32'h00,0,2,32'h0001_0003);//clear RX DMA interrupt
      disable rx_irt_timeout;
    end 
  join
#endif

wr(0x2000E000, 0x05);
    while(!UART1_RXDM_INTR);
    UART1_RXDM_INTR = 0;
wr(0x2000E000, 0x06);

    //`CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h1000, 3'd0, 4'd2, rdata, 1, 32'hA5A5_A5A5, 1'b0);// check Sysram received data
    rd_compare(SYSRAM_ADDR_BASE+0x1000, rdata, 0xA5A5A5A5);
    //repeat(5)@(posedge `CHIP_CLK48M); 
    delayns(5);
    //`CHIP_CPU_BFM.ahbrd (`UART1_ADDR_BASE+U1RBUF, 0,2,rdata,1,32'h00A5, 0); // check received data
    rd_compare(UART1_ADDR_BASE+U1RBUF, rdata, 0x00A5);
    //repeat(5)@(posedge `CHIP_CLK48M); 
    delayns(5);
    //`CHIP_CPU_BFM.ahbrd (`UART1_ADDR_BASE+U1TBUF, 0,2,rdata,1,32'h00A5, 0); // check TBUF contents
    rd_compare(UART1_ADDR_BASE+U1TBUF, rdata, 0x00A5);
    //repeat(5)@(posedge `CHIP_CLK48M); 
    delayns(5);
    //`CHIP_CPU_BFM.ahbrd (`UART1_ADDR_BASE+U1STAT, 0,2,rdata,1,32'h0000, 0); // transmission ended
    rd_compare(UART1_ADDR_BASE+U1STAT, rdata, 0x0000);
    //repeat(5)@(posedge `CHIP_CLK48M); 
    delayns(5);
    //`CHIP_CPU_BFM.ahbrd (`UART1_ADDR_BASE+U1ICTRL,0,2,rdata,1,32'h00fd, 0); // check RBF reset
    rd_compare(UART1_ADDR_BASE+U1ICTRL, rdata, 0x00fd);
    //repeat(5)@(posedge `CHIP_CLK48M); 
    delayns(5);
wr(0x2000E000, 0x07);

    //end UART - RX
    //`CHIP_CPU_BFM.ahbwr( `UART1_ADDR_BASE+32'h04,0,2,32'h0007_00de);
    wr(UART1_ADDR_BASE+0x04, 0x000700de);
    //`CHIP_CPU_BFM.ahbwr( `UART1_ADDR_BASE+32'h10,0,2,32'h0);
    wr(UART1_ADDR_BASE+0x10, 0x0);
    //end UART - TX
    //`CHIP_CPU_BFM.ahbwr( `UART1_ADDR_BASE+32'h08,0,2,32'h0004_00de);
    wr(UART1_ADDR_BASE+0x08, 0x000400de);
    //`CHIP_CPU_BFM.ahbwr( `UART1_ADDR_BASE+32'h1C,0,2,32'h0);
    wr(UART1_ADDR_BASE+0x1C, 0x0);
wr(0x2000E000, 0x08);

    //repeat(5)@(posedge `CHIP_CLK48M);
    delayns(5);
    //`CHIP_CPU_BFM.ahbrd (`UART1_ADDR_BASE+32'h00,0,2,rdata,1,32'h0000_0003,0);//clear interrupt status 
    rd_compare(UART1_ADDR_BASE+0x00, rdata, 0x00000003);


    //`CHIP_CPU_BFM.ahbwr( `UART1_ADDR_BASE+32'h0,0,2,32'h0017_0101);
    wr(UART1_ADDR_BASE+0x0, 0x00170101);

    //repeat(100)@(posedge `CHIP_CLK48M);   
    delayns(10);
    //flush TX fifo
    //program flush dma_tx_rd_en
    //`CHIP_CPU_BFM.ahbwr( `I2S_ADDR_BASE+32'h1c, 3'd0, 4'd2, 32'h0000_0100);
    wr(UART1_ADDR_BASE+0x1c, 0x00000100);
    //repeat(5)@(posedge `CHIP_CLK48M); 
    delayns(5);
    //`CHIP_CPU_BFM.ahbwr( `I2S_ADDR_BASE+32'h1c, 3'd0, 4'd2, 32'h0000_0000);
    wr(UART1_ADDR_BASE+0x1c, 0x00000000);
wr(0x2000E000, 0x09);

    //repeat(500)@(posedge `CHIP_CLK48M);   


    //***************disable DMA*******************

    //$finish;

    return ret;
}

// reuse-pragma startSub DoUndef [IncludeIf 1 -comment // %subText]
#undef ALSB
