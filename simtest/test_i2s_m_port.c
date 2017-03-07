#include "stdint.h"
#include "addr_map.h"

#define I2S_PORT_8

//reg [1:0] 	int_en = 2'b11;
//reg		dual_bank = 1'b0;
//reg		redu_en=1'b0;
static uint32_t rdata;              //32bit
//reg [31:0]	data;
//reg [31:0]	addr=32'h8000_6000; 
//integer		i;
//reg [15:0]      ws_cnt;
//reg [31:0]      i2s_tx_data;
//reg [5:0]       tx_cnt;
// I2s mode 
// 2'b01 : I2s format
// 2'b10 : Left-justified
// 2'b11 : Right-justified
const static uint32_t mode = 2;           //2bit

extern uint32_t I2S_TXDMA_INTR;
extern uint32_t I2S_RXDMA_INTR;
extern void set_port_i2s_m8(void);
extern void set_port_i2s_s4(void);
extern void set_port_i2s_m0(void);
extern void set_port_i2s_m4(void);
extern void set_port_i2s_s8(void);
extern void set_port_i2s_m12(void);
extern void set_port_i2s_m16(void);
extern void set_port_i2s_m20(void);
extern void set_port_i2s_m24(void);
extern void set_port_i2s_m28(void);
extern void set_port_i2s_m32(void);
extern void set_port_i2s_m36(void);
extern void set_port_i2s_m40(void);
extern void set_port_i2s_m44(void);
extern int rd_compare(uint32_t addr, uint32_t val, uint32_t compare_val);

//0: success; others: fail
int test_i2s_m_port(void) {
    int ret = 0;

    //$display("@%t, %m MSG: ****** test name: init_test_CPU_BFM", $time);
    //wait (`TB_INIT_DONE);
    //$display("@%d, %m MSG: TB_INIT_DONEROM", $time);

    #ifdef I2S_PORT_8
        // set_port_i2s_m8
        //force topsys.chip.GPIO[4] = topsys.chip.GPIO[8];
        //force topsys.chip.GPIO[5] = topsys.chip.GPIO[9];
        //force topsys.chip.GPIO[7] = topsys.chip.GPIO[10];
        set_port_i2s_m8();
        set_port_i2s_s4();
        //$display(" ===== I2S master PORT 8 test =====");
    #elif defined(I2S_PORT_0)
        //force topsys.chip.GPIO[4] = topsys.chip.GPIO[0];
        //force topsys.chip.GPIO[5] = topsys.chip.GPIO[1];
        //force topsys.chip.GPIO[3] = topsys.chip.GPIO[2];
        set_port_i2s_m0();
        set_port_i2s_s4();
        //$display(" ===== I2S master PORT 0 test =====");
    #elif defined(I2S_PORT_4)
        //force topsys.chip.GPIO[8] = topsys.chip.GPIO[4];
        //force topsys.chip.GPIO[9] = topsys.chip.GPIO[5];
        //force topsys.chip.GPIO[7] = topsys.chip.GPIO[6];
        set_port_i2s_m4();
        set_port_i2s_s8();
        //$display(" ===== I2S master PORT 4 test =====");
    #elif defined(I2S_PORT_12)
        //force topsys.chip.GPIO[4] = topsys.chip.GPIO[12];
        //force topsys.chip.GPIO[5] = topsys.chip.GPIO[13];
        //force topsys.chip.GPIO[7] = topsys.chip.GPIO[14];
        set_port_i2s_m12();
        set_port_i2s_s4();
        //$display(" ===== I2S master PORT 12 test =====");
    #elif defined(I2S_PORT_16)
        //force topsys.chip.GPIO[8] = topsys.chip.GPIO[16];
        //force topsys.chip.GPIO[9] = topsys.chip.GPIO[17];
        //force topsys.chip.GPIO[11] = topsys.chip.GPIO[18];
        set_port_i2s_m16();
        set_port_i2s_s8();
        //$display(" ===== I2S master PORT 16 test =====");
    #elif defined(I2S_PORT_20)
        //force topsys.chip.GPIO[4] = topsys.chip.GPIO[20];
        //force topsys.chip.GPIO[5] = topsys.chip.GPIO[21];
        //force topsys.chip.GPIO[7] = topsys.chip.GPIO[22];
        set_port_i2s_m20();
        set_port_i2s_s4();
        //$display(" ===== I2S master PORT 20 test =====");
    #elif defined(I2S_PORT_24)
        //force topsys.chip.GPIO[4] = topsys.chip.GPIO[24];
        //force topsys.chip.GPIO[5] = topsys.chip.GPIO[25];
        //force topsys.chip.GPIO[7] = topsys.chip.GPIO[26];
        set_port_i2s_m24();
        set_port_i2s_s4();
        //$display(" ===== I2S master PORT 24 test =====");
    #elif defined(I2S_PORT_28)
        //force topsys.chip.GPIO[8] = topsys.chip.GPIO[28];
        //force topsys.chip.GPIO[9] = topsys.chip.GPIO[29];
        //force topsys.chip.GPIO[11] = topsys.chip.GPIO[30];
        set_port_i2s_m28();
        set_port_i2s_s8();
        //$display(" ===== I2S master PORT 28 test =====");
    #elif defined(I2S_PORT_32)
        //force topsys.chip.GPIO[8] = topsys.chip.GPIO[32];
        //force topsys.chip.GPIO[9] = topsys.chip.GPIO[33];
        //force topsys.chip.GPIO[11] = topsys.chip.GPIO[34];
        set_port_i2s_m32();
        set_port_i2s_s8();
        //$display(" ===== I2S master PORT 32 test =====");
    #elif defined(I2S_PORT_36)
        //force topsys.chip.GPIO[8] = topsys.chip.GPIO[36];
        //force topsys.chip.GPIO[9] = topsys.chip.GPIO[37];
        //force topsys.chip.GPIO[11] = topsys.chip.GPIO[38];
        set_port_i2s_m36();
        set_port_i2s_s8();
        //$display(" ===== I2S master PORT 36 test =====");
    #elif defined(I2S_PORT_40)
        //force topsys.chip.GPIO[8] = topsys.chip.GPIO[40];
        //force topsys.chip.GPIO[9] = topsys.chip.GPIO[41];
        //force topsys.chip.GPIO[11] = topsys.chip.GPIO[42];
        set_port_i2s_m40();
        set_port_i2s_s8();
        //$display(" ===== I2S master PORT 40 test =====");
    #else
        //force topsys.chip.GPIO[8] = topsys.chip.GPIO[44];
        //force topsys.chip.GPIO[9] = topsys.chip.GPIO[45];
        //force topsys.chip.GPIO[11] = topsys.chip.GPIO[46];
        set_port_i2s_m44();
        set_port_i2s_s8();
        //$display(" ===== I2S master PORT 44 test =====");
    #endif

    // I2S TX data
    //`CHIP_CPU_BFM.ahbwr( `SYSRAM_ADDR_BASE+32'h00, 3'd0, 4'd2, 32'h0123_4567);
    wr(SYSRAM_TEST_ADDR_BASE+0x00, 0x01234567);
    //`CHIP_CPU_BFM.ahbwr( `SYSRAM_ADDR_BASE+32'h04, 3'd0, 4'd2, 32'h89ab_cdef);
    wr(SYSRAM_TEST_ADDR_BASE+0x04, 0x89abcdef);
    //`CHIP_CPU_BFM.ahbwr( `SYSRAM_ADDR_BASE+32'h08, 3'd0, 4'd2, 32'hfedc_ba98);
    wr(SYSRAM_TEST_ADDR_BASE+0x08, 0xfedcba98);
    //`CHIP_CPU_BFM.ahbwr( `SYSRAM_ADDR_BASE+32'h0c, 3'd0, 4'd2, 32'h7654_3210);
    wr(SYSRAM_TEST_ADDR_BASE+0x0c, 0x76543210);
    //`CHIP_CPU_BFM.ahbwr( `SYSRAM_ADDR_BASE+32'h10, 3'd0, 4'd2, 32'h1111_1111);
    wr(SYSRAM_TEST_ADDR_BASE+0x10, 0x11111111);
    //`CHIP_CPU_BFM.ahbwr( `SYSRAM_ADDR_BASE+32'h14, 3'd0, 4'd2, 32'h2222_2222);
    wr(SYSRAM_TEST_ADDR_BASE+0x14, 0x22222222);
    //`CHIP_CPU_BFM.ahbwr( `SYSRAM_ADDR_BASE+32'h18, 3'd0, 4'd2, 32'h3333_3333);
    wr(SYSRAM_TEST_ADDR_BASE+0x18, 0x33333333);
    //`CHIP_CPU_BFM.ahbwr( `SYSRAM_ADDR_BASE+32'h1c, 3'd0, 4'd2, 32'h4444_4444);
    wr(SYSRAM_TEST_ADDR_BASE+0x1c, 0x44444444);
    //`CHIP_CPU_BFM.ahbwr( `SYSRAM_ADDR_BASE+32'h20, 3'd0, 4'd2, 32'h5555_5555);
    wr(SYSRAM_TEST_ADDR_BASE+0x20, 0x55555555);
    //`CHIP_CPU_BFM.ahbwr( `SYSRAM_ADDR_BASE+32'h24, 3'd0, 4'd2, 32'h6666_6666);
    wr(SYSRAM_TEST_ADDR_BASE+0x24, 0x66666666);
    //`CHIP_CPU_BFM.ahbwr( `SYSRAM_ADDR_BASE+32'h28, 3'd0, 4'd2, 32'h7777_7777);
    wr(SYSRAM_TEST_ADDR_BASE+0x28, 0x77777777);
    //`CHIP_CPU_BFM.ahbwr( `SYSRAM_ADDR_BASE+32'h2c, 3'd0, 4'd2, 32'h8888_8888);
    wr(SYSRAM_TEST_ADDR_BASE+0x2c, 0x88888888);
    //`CHIP_CPU_BFM.ahbwr( `SYSRAM_ADDR_BASE+32'h30, 3'd0, 4'd2, 32'h9999_9999);
    wr(SYSRAM_TEST_ADDR_BASE+0x30, 0x99999999);
    //`CHIP_CPU_BFM.ahbwr( `SYSRAM_ADDR_BASE+32'h34, 3'd0, 4'd2, 32'haaaa_aaaa);
    wr(SYSRAM_TEST_ADDR_BASE+0x34, 0xaaaaaaaa);
    //`CHIP_CPU_BFM.ahbwr( `SYSRAM_ADDR_BASE+32'h38, 3'd0, 4'd2, 32'hbbbb_bbbb);
    wr(SYSRAM_TEST_ADDR_BASE+0x38, 0xbbbbbbbb);
    //`CHIP_CPU_BFM.ahbwr( `SYSRAM_ADDR_BASE+32'h3c, 3'd0, 4'd2, 32'hcccc_cccc);
    wr(SYSRAM_TEST_ADDR_BASE+0x3c, 0xcccccccc);
    //`CHIP_CPU_BFM.ahbwr( `SYSRAM_ADDR_BASE+32'h40, 3'd0, 4'd2, 32'hdddd_dddd);
    wr(SYSRAM_TEST_ADDR_BASE+0x40, 0xdddddddd);
    //`CHIP_CPU_BFM.ahbwr( `SYSRAM_ADDR_BASE+32'h44, 3'd0, 4'd2, 32'heeee_eeee);
    wr(SYSRAM_TEST_ADDR_BASE+0x44, 0xeeeeeeee);
    //`CHIP_CPU_BFM.ahbwr( `SYSRAM_ADDR_BASE+32'h48, 3'd0, 4'd2, 32'hffff_ffff);
    wr(SYSRAM_TEST_ADDR_BASE+0x48, 0xffffffff);
    //`CHIP_CPU_BFM.ahbwr( `SYSRAM_ADDR_BASE+32'h4c, 3'd0, 4'd2, 32'ha5a5_a5a5);
    wr(SYSRAM_TEST_ADDR_BASE+0x4c, 0xa5a5a5a5);
    //  `CHIP_CPU_BFM.ahbwr( 32'h0006_0080, 3'd0, 4'd2, 32'h0123_4567);
    //  `CHIP_CPU_BFM.ahbwr( 32'h0006_0084, 3'd0, 4'd2, 32'h89ab_cdef);
    //  `CHIP_CPU_BFM.ahbwr( 32'h0006_0088, 3'd0, 4'd2, 32'hfedc_ba98);
    //  `CHIP_CPU_BFM.ahbwr( 32'h0006_008c, 3'd0, 4'd2, 32'h7654_3210);
    //repeat(4) @(posedge `CHIP_CLK48M);
    delayns(4);

    //program interrupt enable  0: RX 1: TX
    //`CHIP_CPU_BFM.ahbwr( `I2S_ADDR_BASE+32'h20, 3'd0, 4'd2, 32'h0000_0003);
    wr(I2S_ADDR_BASE+0x20, 0x00000003);
    //repeat(4) @(posedge `CHIP_CLK48M);
    delayns(4);

    //program dam_byte_num [15:8] TX byte [7:0] R byte : 0 > 1byte ~ FF > 256byte          
    // TX 16byte RX 12byte   
    //`CHIP_CPU_BFM.ahbwr( `I2S_ADDR_BASE+32'h24, 3'd0, 4'd2, 32'h0000_0c0c);
    wr(I2S_ADDR_BASE+0x24, 0x00000c0c);
    //`CHIP_CPU_BFM.ahbrd( `I2S_ADDR_BASE+32'h24, 3'd0, 4'd2, rdata, 1, 32'h0000_0c0c, 1'b0);
    rd(I2S_ADDR_BASE+0x24, rdata);
    //repeat(4) @(posedge `CHIP_CLK48M);
    delayns(4);

    //program dma_rx_start_addr
    //`CHIP_CPU_BFM.ahbwr( `I2S_ADDR_BASE+32'h28, 3'd0, 4'd2, `SYSRAM_ADDR_BASE+32'h1000);
    wr(I2S_ADDR_BASE+0x28, SYSRAM_ADDR_BASE+0x1000);
  //  `CHIP_CPU_BFM.ahbrd( `I2S_ADDR_BASE+32'h28, 3'd0, 4'd2, rdata, 1, 32'h6_1000, 1'b0);
    //repeat(4) @(posedge `CHIP_CLK48M);
    delayns(4);

    //program dma_rx_end_addr
    //`CHIP_CPU_BFM.ahbwr( `I2S_ADDR_BASE+32'h2c, 3'd0, 4'd2, `SYSRAM_ADDR_BASE+32'h104c);
    wr(I2S_ADDR_BASE+0x2c, SYSRAM_ADDR_BASE+0x104c);
  //  `CHIP_CPU_BFM.ahbrd( `I2S_ADDR_BASE+32'h2c, 3'd0, 4'd2, rdata, 1, 32'h6_100c, 1'b0);
    //repeat(4) @(posedge `CHIP_CLK48M);
    delayns(4);

    //program dma_tx_start_addr
    //`CHIP_CPU_BFM.ahbwr( `I2S_ADDR_BASE+32'h34, 3'd0, 4'd2, `SYSRAM_ADDR_BASE);
    wr(I2S_ADDR_BASE+0x34, SYSRAM_TEST_ADDR_BASE);
  //  `CHIP_CPU_BFM.ahbrd( `I2S_ADDR_BASE+32'h34, 3'd0, 4'd2, rdata, 1, 32'h6000, 1'b0);
    //repeat(4) @(posedge `CHIP_CLK48M);
    delayns(4);

    //program dma_tx_end_addr
    //`CHIP_CPU_BFM.ahbwr( `I2S_ADDR_BASE+32'h38, 3'd0, 4'd2, `SYSRAM_ADDR_BASE+32'h004c);
    wr(I2S_ADDR_BASE+0x38, SYSRAM_TEST_ADDR_BASE+0x004c);
  //  `CHIP_CPU_BFM.ahbrd( `I2S_ADDR_BASE+32'h38, 3'd0, 4'd2, rdata, 1, 32'h6_000c, 1'b0);
    //repeat(4) @(posedge `CHIP_CLK48M);
    delayns(4);

    //read RX addr in RAM location
    //`CHIP_CPU_BFM.ahbwr( `I2S_ADDR_BASE+32'h40, 3'd0, 4'd2, 32'h0);
    wr(I2S_ADDR_BASE+0x40, 0x0);
 //   `CHIP_CPU_BFM.ahbrd( `I2S_ADDR_BASE+32'h40, 3'd0, 4'd2, rdata, 1, 32'h0, 1'b0);
    //repeat(4) @(posedge `CHIP_CLK48M);
    delayns(4);

    //read TX addr in RAM location
    //`CHIP_CPU_BFM.ahbwr( `I2S_ADDR_BASE+32'h44, 3'd0, 4'd2, 32'h0);
    wr(I2S_ADDR_BASE+0x44, 0x0);
  //  `CHIP_CPU_BFM.ahbrd( `I2S_ADDR_BASE+32'h44, 3'd0, 4'd2, rdata, 1, 32'h0, 1'b0);
    //repeat(4) @(posedge `CHIP_CLK48M);
    delayns(4);

    //set Master Mode
    //`CHIP_CPU_BFM.ahbrd( `I2S_ADDR_BASE+32'h00, 3'd0, 4'd2, rdata, 1, 32'h17_0109, 1'b0);
    rd(I2S_ADDR_BASE+0x00, rdata);
    //repeat(4) @(posedge `CHIP_CLK48M);
    delayns(4);

    //`CHIP_CPU_BFM.ahbwr( `I2S_ADDR_BASE+32'h0c, 3'd0, 4'd2, 32'hffff_ff00);
    wr(I2S_ADDR_BASE+0x0c, 0xffffff00);
    //repeat(4) @(posedge `CHIP_CLK48M);
    delayns(4);

//    `CHIP_CPU_BFM.ahbwr( `I2S_ADDR_BASE+32'h10, 3'd0, 4'd2, 32'h0);
//    repeat(4) @(posedge `CHIP_CLK48M);

//    `CHIP_CPU_BFM.ahbwr( `I2S_ADDR_BASE+32'h0c, 3'd0, 4'd2, 32'h0);
//    repeat(4) @(posedge `CHIP_CLK48M);

    //`CHIP_CPU_BFM.ahbwr( `I2S_ADDR_BASE+32'h10, 3'd0, 4'd2, 32'hffff_ff00);
    wr(I2S_ADDR_BASE+0x10, 0xffffff00);
    //repeat(4) @(posedge `CHIP_CLK48M);
    delayns(4);

    //*********end i2s_ip programming*************
    if(mode == 1) {
        //$display(" ===== I2s foramt =====");
    }
    else if(mode == 2) {
        //$display(" ===== Left justified =====");
    }
    else if(mode == 3) {
        //$display(" ===== Right justified =====");
    }
    else {
        //$display(" ===== TX/RX disabled =====");
    }

    //***************enable DMA*******************
    //program I2S-IP to start TX
    // threshold [18:16] = 4
    // [13] TXFIFOENL = 1 [12] TXFIFIENR = 1
    // [11] TXLD = 1 DMA enable
    // [10] TXRD = 1 DMA enable
    // [7] TXER =1 int enable
    // [6:2] = 00000
    // [1:0] = 10 Left-J   >  01:I2S format 11 Right-J 
    //`CHIP_CPU_BFM.ahbwr( `I2S_ADDR_BASE+32'h08, 3'd0, 4'd2, {32'h4_3cdc + mode});
    wr(I2S_ADDR_BASE+0x08, (0x00043cdc + mode));
    //`CHIP_CPU_BFM.ahbrd( `I2S_ADDR_BASE+32'h08, 3'd0, 4'd2, rdata, 1, {32'h4_3cdc + mode}, 1'b0);
    rd(I2S_ADDR_BASE+0x08, rdata);
    //repeat(4) @(posedge `CHIP_CLK48M);
    delayns(4);

    //program I2S-IP to start RX
    //`CHIP_CPU_BFM.ahbwr( `I2S_ADDR_BASE+32'h04, 3'd0, 4'd2, {32'h4_3cdc + mode});
    wr(I2S_ADDR_BASE+0x04, (0x00043cdc + mode));
    //`CHIP_CPU_BFM.ahbrd( `I2S_ADDR_BASE+32'h04, 3'd0, 4'd2, rdata, 1, {32'h4_3cdc + mode}, 1'b0);
    rd(I2S_ADDR_BASE+0x04, rdata);

    //start DBUS RX-DMA
    //`CHIP_CPU_BFM.ahbwr( `I2S_ADDR_BASE+32'h30, 3'd0, 4'd2, 32'h1);
    wr(I2S_ADDR_BASE+0x30, 0x1);
    //`CHIP_CPU_BFM.ahbrd( `I2S_ADDR_BASE+32'h30, 3'd0, 4'd2, rdata, 1, 32'h1, 1'b0);
    rd(I2S_ADDR_BASE+0x30, rdata);

    //start DBUS TX-DMA
    //`CHIP_CPU_BFM.ahbwr( `I2S_ADDR_BASE+32'h3c, 3'd0, 4'd2, 32'h1);
    wr(I2S_ADDR_BASE+0x3c, 0x1);
    //`CHIP_CPU_BFM.ahbrd( `I2S_ADDR_BASE+32'h3c, 3'd0, 4'd2, rdata, 1, 32'h1, 1'b0);
    rd(I2S_ADDR_BASE+0x3c, rdata);

    //`CHIP_CPU_BFM.ahbwr( `I2S_ADDR_BASE+32'h00, 3'd0, 4'd2, 32'h17_0109);
    wr(I2S_ADDR_BASE+0x00, 0x00170109);

    //wait(`I2S_TXDMA_INTR);
    while(!I2S_TXDMA_INTR);
    I2S_TXDMA_INTR = 0;
    //repeat(20)@(posedge `CHIP_CLK48M);
    delayns(20);
    //repeat(1)@(posedge `CHIP_CLK48M);
    delayns(1);

    //repeat(10)@(posedge `CHIP_CLK48M);
    delayns(10);
    //repeat(2000)@(posedge `CHIP_CLK48M);
    delayns(2000);

    //wait(`I2S_RXDMA_INTR);
    while(!I2S_RXDMA_INTR);
    I2S_RXDMA_INTR = 0;
    // end I2S -RX 
    //`CHIP_CPU_BFM.ahbwr( `I2S_ADDR_BASE+32'h04, 3'd0, 4'd2, 32'h7_00de);
    wr(I2S_ADDR_BASE+0x04, 0x000700de);
    //`CHIP_CPU_BFM.ahbwr( `I2S_ADDR_BASE+32'h30, 3'd0, 4'd2, 32'h0);
    wr(I2S_ADDR_BASE+0x30, 0x0);

    // end I2S -TX 
    //`CHIP_CPU_BFM.ahbwr( `I2S_ADDR_BASE+32'h08, 3'd0, 4'd2, 32'h4_00de);
    wr(I2S_ADDR_BASE+0x08, 0x000400de);
    //`CHIP_CPU_BFM.ahbwr( `I2S_ADDR_BASE+32'h20, 3'd0, 4'd2, 32'h2_0003);
    //`CHIP_CPU_BFM.ahbwr( `I2S_ADDR_BASE+32'h3c, 3'd0, 4'd2, 32'h0);
    wr(I2S_ADDR_BASE+0x3c, 0x0);

    //`CHIP_CPU_BFM.ahbwr( `I2S_ADDR_BASE+32'h0, 3'd0, 4'd2, 32'h17_0101);
    wr(I2S_ADDR_BASE+0x0, 0x00170101);

    //repeat(100)@(posedge `CHIP_CLK48M);
    delayns(100);
    // Flush TX fifo   
    //`CHIP_CPU_BFM.ahbwr( `I2S_ADDR_BASE+32'h3c, 3'd0, 4'd2, 32'h0100);
    wr(I2S_ADDR_BASE+0x3c, 0x0100);
    //repeat(20)@(posedge `CHIP_CLK48M);
    delayns(20);
    //`CHIP_CPU_BFM.ahbwr( `I2S_ADDR_BASE+32'h3c, 3'd0, 4'd2, 32'h0);
    wr(I2S_ADDR_BASE+0x3c, 0x0);

    // read RX fifo 32'h6_1000
    if(mode == 2) {  // Left-justified
        //`CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h1000, 3'd0, 4'd2, rdata, 1, 32'h123_4500, 1'b0);
        rd_compare(SYSRAM_ADDR_BASE+0x1000, rdata, 0x01234500);
        //`CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h1004, 3'd0, 4'd2, rdata, 1, 32'h89ab_cd00, 1'b0);
        rd_compare(SYSRAM_ADDR_BASE+0x1004, rdata, 0x89abcd00);
        //`CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h1008, 3'd0, 4'd2, rdata, 1, 32'hfedc_ba00, 1'b0);
        rd_compare(SYSRAM_ADDR_BASE+0x1008, rdata, 0xfedcba00);
        //`CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h100c, 3'd0, 4'd2, rdata, 1, 32'h7654_3200, 1'b0);
        rd_compare(SYSRAM_ADDR_BASE+0x100c, rdata, 0x76543200);
        //`CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h1010, 3'd0, 4'd2, rdata, 1, 32'h1111_1100, 1'b0);
        rd_compare(SYSRAM_ADDR_BASE+0x1010, rdata, 0x11111100);
        //`CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h1014, 3'd0, 4'd2, rdata, 1, 32'h2222_2200, 1'b0);
        rd_compare(SYSRAM_ADDR_BASE+0x1014, rdata, 0x22222200);
        //`CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h1018, 3'd0, 4'd2, rdata, 1, 32'h3333_3300, 1'b0);
        rd_compare(SYSRAM_ADDR_BASE+0x1018, rdata, 0x33333300);
        //`CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h101c, 3'd0, 4'd2, rdata, 1, 32'h4444_4400, 1'b0);
        rd_compare(SYSRAM_ADDR_BASE+0x101c, rdata, 0x44444400);
        //`CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h1020, 3'd0, 4'd2, rdata, 1, 32'h5555_5500, 1'b0);
        rd_compare(SYSRAM_ADDR_BASE+0x1020, rdata, 0x55555500);
        //`CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h1024, 3'd0, 4'd2, rdata, 1, 32'h6666_6600, 1'b0);
        rd_compare(SYSRAM_ADDR_BASE+0x1024, rdata, 0x66666600);
        //`CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h1028, 3'd0, 4'd2, rdata, 1, 32'h7777_7700, 1'b0);
        rd_compare(SYSRAM_ADDR_BASE+0x1028, rdata, 0x77777700);
        //`CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h102c, 3'd0, 4'd2, rdata, 1, 32'h8888_8800, 1'b0);
        rd_compare(SYSRAM_ADDR_BASE+0x102c, rdata, 0x88888800);
        //`CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h1030, 3'd0, 4'd2, rdata, 1, 32'h9999_9900, 1'b0);
        rd_compare(SYSRAM_ADDR_BASE+0x1030, rdata, 0x99999900);
        //`CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h1034, 3'd0, 4'd2, rdata, 1, 32'haaaa_aa00, 1'b0);
        rd_compare(SYSRAM_ADDR_BASE+0x1034, rdata, 0xaaaaaa00);
        //`CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h1038, 3'd0, 4'd2, rdata, 1, 32'hbbbb_bb00, 1'b0);
        rd_compare(SYSRAM_ADDR_BASE+0x1038, rdata, 0xbbbbbb00);
        //`CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h103c, 3'd0, 4'd2, rdata, 1, 32'hcccc_cc00, 1'b0);
        rd_compare(SYSRAM_ADDR_BASE+0x103c, rdata, 0xcccccc00);
        //`CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h1040, 3'd0, 4'd2, rdata, 1, 32'hdddd_dd00, 1'b0);
        rd_compare(SYSRAM_ADDR_BASE+0x1040, rdata, 0xdddddd00);
        //`CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h1044, 3'd0, 4'd2, rdata, 1, 32'heeee_ee00, 1'b0);
        rd_compare(SYSRAM_ADDR_BASE+0x1044, rdata, 0xeeeeee00);
        //`CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h1048, 3'd0, 4'd2, rdata, 1, 32'hffff_ff00, 1'b0);
        rd_compare(SYSRAM_ADDR_BASE+0x1048, rdata, 0xffffff00);
        //`CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h104c, 3'd0, 4'd2, rdata, 1, 32'ha5a5_a500, 1'b0);
        rd_compare(SYSRAM_ADDR_BASE+0x104c, rdata, 0xa5a5a500);
    }
    else if(mode == 1) {  // I2s format
        //`CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h1000, 3'd0, 4'd2, rdata, 1, 32'hffff_ff00, 1'b0);
        rd_compare(SYSRAM_ADDR_BASE+0x1000, rdata, 0xffffff00);
        //`CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h1004, 3'd0, 4'd2, rdata, 1, 32'ha5a5_a500, 1'b0);
        rd_compare(SYSRAM_ADDR_BASE+0x1004, rdata, 0xa5a5a500);
        //`CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h1008, 3'd0, 4'd2, rdata, 1, 32'h123_4500, 1'b0);
        rd_compare(SYSRAM_ADDR_BASE+0x1008, rdata, 0x1234500);
        //`CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h100c, 3'd0, 4'd2, rdata, 1, 32'h89ab_cd00, 1'b0);
        rd_compare(SYSRAM_ADDR_BASE+0x100c, rdata, 0x89abcd00);
        //`CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h1010, 3'd0, 4'd2, rdata, 1, 32'hfedc_ba00, 1'b0);
        rd_compare(SYSRAM_ADDR_BASE+0x1010, rdata, 0xfedcba00);
        //`CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h1014, 3'd0, 4'd2, rdata, 1, 32'h7654_3200, 1'b0);
        rd_compare(SYSRAM_ADDR_BASE+0x1014, rdata, 0x76543200);
        //`CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h1018, 3'd0, 4'd2, rdata, 1, 32'h1111_1100, 1'b0);
        rd_compare(SYSRAM_ADDR_BASE+0x1018, rdata, 0x11111100);
        //`CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h101c, 3'd0, 4'd2, rdata, 1, 32'h2222_2200, 1'b0);
        rd_compare(SYSRAM_ADDR_BASE+0x101c, rdata, 0x22222200);
        //`CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h1020, 3'd0, 4'd2, rdata, 1, 32'h3333_3300, 1'b0);
        rd_compare(SYSRAM_ADDR_BASE+0x1020, rdata, 0x33333300);
        //`CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h1024, 3'd0, 4'd2, rdata, 1, 32'h4444_4400, 1'b0);
        rd_compare(SYSRAM_ADDR_BASE+0x1024, rdata, 0x44444400);
        //`CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h1028, 3'd0, 4'd2, rdata, 1, 32'h5555_5500, 1'b0);
        rd_compare(SYSRAM_ADDR_BASE+0x1028, rdata, 0x55555500);
        //`CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h102c, 3'd0, 4'd2, rdata, 1, 32'h6666_6600, 1'b0);
        rd_compare(SYSRAM_ADDR_BASE+0x102c, rdata, 0x66666600);
        //`CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h1030, 3'd0, 4'd2, rdata, 1, 32'h7777_7700, 1'b0);
        rd_compare(SYSRAM_ADDR_BASE+0x1030, rdata, 0x77777700);
        //`CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h1034, 3'd0, 4'd2, rdata, 1, 32'h8888_8800, 1'b0);
        rd_compare(SYSRAM_ADDR_BASE+0x1034, rdata, 0x88888800);
        //`CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h1038, 3'd0, 4'd2, rdata, 1, 32'h9999_9900, 1'b0);
        rd_compare(SYSRAM_ADDR_BASE+0x1038, rdata, 0x99999900);
        //`CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h103c, 3'd0, 4'd2, rdata, 1, 32'haaaa_aa00, 1'b0);
        rd_compare(SYSRAM_ADDR_BASE+0x103c, rdata, 0xaaaaaa00);
        //`CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h1040, 3'd0, 4'd2, rdata, 1, 32'hbbbb_bb00, 1'b0);
        rd_compare(SYSRAM_ADDR_BASE+0x1040, rdata, 0xbbbbbb00);
        //`CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h1044, 3'd0, 4'd2, rdata, 1, 32'hcccc_cc00, 1'b0);
        rd_compare(SYSRAM_ADDR_BASE+0x1044, rdata, 0xcccccc00);
        //`CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h1048, 3'd0, 4'd2, rdata, 1, 32'hdddd_dd00, 1'b0);
        rd_compare(SYSRAM_ADDR_BASE+0x1048, rdata, 0xdddddd00);
        //`CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h104c, 3'd0, 4'd2, rdata, 1, 32'heeee_ee00, 1'b0);
        rd_compare(SYSRAM_ADDR_BASE+0x104c, rdata, 0xeeeeee00);
    }
    else if(mode == 3) {
        //`CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h1000, 3'd0, 4'd2, rdata, 1, 32'h123_4500, 1'b0);
        rd_compare(SYSRAM_ADDR_BASE+0x1000, rdata, 0x1234500);
        //`CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h1004, 3'd0, 4'd2, rdata, 1, 32'h89ab_cd00, 1'b0);
        rd_compare(SYSRAM_ADDR_BASE+0x1004, rdata, 0x89abcd00);
        //`CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h1008, 3'd0, 4'd2, rdata, 1, 32'hfedc_ba00, 1'b0);
        rd_compare(SYSRAM_ADDR_BASE+0x1008, rdata, 0xfedcba00);
        //`CHIP_CPU_BFM.ahbrd( `SYSRAM_ADDR_BASE+32'h100c, 3'd0, 4'd2, rdata, 1, 32'h7654_3200, 1'b0);
        rd_compare(SYSRAM_ADDR_BASE+0x100c, rdata, 0x76543200);
    }

    //repeat(5000)@(posedge `CHIP_CLK48M);    
    //$finish;

    return ret;
}
