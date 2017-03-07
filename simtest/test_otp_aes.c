#include "stdint.h"
#include "addr_map.h"

#pragma diag_suppress 550

////parameter       M_CLK_PERIOD_DIV2 = 166.66; // 3MHZ. 333.33/2
//parameter       M_CLK_PERIOD_DIV2 = 83.33; // 6MHZ. 166.66/2
//reg     m_clk=0;
//always begin
//    m_clk = #M_CLK_PERIOD_DIV2 ~m_clk;
//end

static int i;
//reg [1:0] 	int_en = 2'b11;
//reg		dual_bank = 1'b0;
//reg		redu_en=1'b0;
static uint32_t rdata;              //32bit

//reg [31:0] wr_din[7:0];

static uint32_t dma_str_raddr;      //17bit
static uint32_t dma_str_waddr;      //17bit
static uint32_t wdata_byte_num;     //12bit
static uint32_t rdata_byte_num;     //12bit
//reg [1:0]	port_op_mode;
static uint32_t dma_enable;         //1bit
static uint32_t dbus_burst;         //1bit
static uint32_t bc_mode;            //5bit
static uint32_t key_size;           //2bit
static uint32_t decrypt;            //1bit
//reg [255:0]	key_exp;

//`define AES_INTR_REG            `AES_ADDR_BASE + 32'h0
//`define AES_CTRL_REG            `AES_ADDR_BASE + 32'h4
//`define AES_DMA_CTRL_REG        `AES_ADDR_BASE + 32'h8
//`define AES_DMA_WADDR_REG       `AES_ADDR_BASE + 32'hc
//`define AES_DMA_RADDR_REG       `AES_ADDR_BASE + 32'h10
//
extern uint32_t AES_INTR;

int test_otp_aes(void) {
    int ret = 0;

    //$display("@%t, %m MSG: ****** test name: test_otp_efuse", $time);
    //// force `CHIP_TOP.test_mode=1'b1;
    ////force `TOPSYS.VDDQ=1'b1;

    //wait (`TB_INIT_DONE);
    //$display("@%d, %m MSG: TB_INIT_DONEROM", $time);
//`ifdef M4_SELECT
//    `CHIP_CPU_BFM.set_rd_file_limit("ahb_rdata1.hex",0,32768);
//`else
//    `CHIP_CPU_BFM.set_rd_file_limit("ahb_rdata.hex",0,32768);
//`endif

    //@(posedge `CHIP_CLK48M);
    delayns(1);

    // TEMPORARY   
    //force `CHIP_TOP.OTP_TDO = `CHIP_TOP.jtag_tdi ^ `CHIP_TOP.jtag_tms;

#if 0 //ndef FPGA
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
    wr_din[0] = 32'h00000003; 
    wr_din[1] = 32'h00000002; 
    wr_din[2] = 32'h00000001; 
    wr_din[3] = 32'h00000000; 
    wr_din[4] = 32'h89abcdef; 
    wr_din[5] = 32'h01234567; 
    wr_din[6] = 32'h89abcdef; 
    wr_din[7] = 32'h01234567; 
    key_exp = { wr_din[7],wr_din[6],wr_din[5],wr_din[4],
		wr_din[3],wr_din[2],wr_din[1],wr_din[0]};

    for (i=0; i<8; i=i+1) begin
        force `CHIP_OTP_AES.otp.otp_ctrl.prog_en = 1'b1;
        force `CHIP_OTP_AES.otp.otp_ctrl.prog_manual = 1'b0;
        force `CHIP_OTP_AES.otp.otp_ctrl.prog_din[31:0] = wr_din[i];
        repeat(2) @(posedge `TOPSYS.jtag_tck);
        wait (`CHIP_OTP_AES.otp.otp_ctrl.prog_dn);
        $display("@%0d, MSG: prog_dn(%0d)", $time,i);
        repeat(1) @(posedge `TOPSYS.jtag_tck);
        force `CHIP_OTP_AES.otp.otp_ctrl.prog_en = 1'b0;
        repeat(1) @(posedge `TOPSYS.jtag_tck);
    end


    release `CHIP_OTP_AES.otp.otp_ctrl.prog_en;
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
#endif

//`ifdef M4_SELECT
//    `CHIP_CPU_BFM.set_rd_file_limit("ahb_rdata1.hex", 128/4,(128/4)+32);
//`else
//    `CHIP_CPU_BFM.set_rd_file_limit("ahb_rdata.hex", 128/4,(128/4)+32);
//`endif


    //---------------------- dma read rom -> ase(encrypt) -> dbus write
    //$display("@%d, MSG: DMA READ ROM -> AES(ENCRYPT) -> DBUS_WRITE", $time);
    //`CHIP_CPU_BFM.ahbwr(`AES_INTR_REG, 3'd0, 4'd2, {31'h0,1'b1});
    wr(AES_INTR_REG, (0 | 1));
    bc_mode = 3;
    key_size = 0;
    decrypt = 0;
    //`CHIP_CPU_BFM.ahbwr( `AES_CTRL_REG, 3'd0, 4'd2, {15'd0,decrypt,6'd0,key_size,3'd0,bc_mode});
    wr(AES_CTRL_REG, (0 | decrypt<<16 | 0 | key_size<<8 | 0 | bc_mode));
    dma_str_raddr = 0;
    dma_str_waddr = 128;
    wdata_byte_num = 63;
    rdata_byte_num = 63;
    dma_enable = 1;
    dbus_burst=1;
    //`CHIP_CPU_BFM.ahbwr( `AES_DMA_WADDR_REG, 3'd0, 4'd2, {15'd0,dma_str_waddr[16:0]});
    wr(AES_DMA_WADDR_REG, (0 | dma_str_waddr));
    //`CHIP_CPU_BFM.ahbwr( `AES_DMA_RADDR_REG, 3'd0, 4'd2, {15'd0,dma_str_raddr[16:0]});
    wr(AES_DMA_RADDR_REG, (0 | dma_str_raddr));
    //`CHIP_CPU_BFM.ahbwr( `AES_DMA_CTRL_REG, 3'd0, 4'd2, {dma_enable,3'd0,rdata_byte_num[11:0],dbus_burst,3'd0,wdata_byte_num[11:0]}); // intr enable
    wr(AES_DMA_CTRL_REG, (dma_enable<<31 | 0 | rdata_byte_num<<16 | dbus_burst | 0 | wdata_byte_num));
    //wait (`AES_INTR); 
    while(!AES_INTR);
    AES_INTR = 0;
    //`CHIP_CPU_BFM.ahbwr( `AES_INTR_REG, 3'd0, 4'd2, 32'h0001_0001); 
    //`CHIP_CPU_BFM.ahbwr( `AES_CTRL_REG, 3'd0, 4'd2, 32'h8000_0000);  // aes_clear
    wr(AES_CTRL_REG, 0x80000000);


    //`CHIP_CPU_BFM.ahbrd_incr(`SYSRAM_ADDR_BASE+dma_str_waddr, 1, 16, 2, rdata, 1, 32'h0, 0);
    for (i=0; i<16; i++) {
        rd(SYSRAM_ADDR_BASE+dma_str_waddr+i*4, rdata);
    }
  
    // ---------------------- dma read rom -> ase(decrypt) -> dbus write
    //$display("@%d, MSG: DMA READ ROM -> AES(DECRYPT) -> DBUS_WRITE", $time);
    //`CHIP_CPU_BFM.ahbwr( `AES_INTR_REG, 3'd0, 4'd2, 32'h0000_0001); // intr enable
    wr(AES_INTR_REG, 0x00000001); // intr enable
    bc_mode = 2;
    key_size = 0;
    decrypt = 1;
    //`CHIP_CPU_BFM.ahbwr( `AES_CTRL_REG, 3'd0, 4'd2, {15'd0,decrypt,6'd0,key_size,3'd0,bc_mode});
    wr(AES_CTRL_REG, (0 | decrypt<<16 | 0 | key_size<<8 | 3 | bc_mode));
    dma_str_raddr = 128;
    dma_str_waddr = 128+64;
    wdata_byte_num = 63;
    rdata_byte_num = 63;
    dma_enable = 1;
    //`CHIP_CPU_BFM.ahbwr( `AES_DMA_WADDR_REG, 3'd0, 4'd2, {15'd0,dma_str_waddr[16:0]});
    wr(AES_DMA_WADDR_REG, (0 | dma_str_waddr));
    //`CHIP_CPU_BFM.ahbwr( `AES_DMA_RADDR_REG, 3'd0, 4'd2, {15'd0,dma_str_raddr[16:0]});
    wr(AES_DMA_RADDR_REG, (0 | dma_str_raddr));
    //`CHIP_CPU_BFM.ahbwr( `AES_DMA_CTRL_REG, 3'd0, 4'd2, {dma_enable,3'd0,rdata_byte_num[11:0],dbus_burst,3'd0,wdata_byte_num[11:0]}); // intr enable
    wr(AES_DMA_CTRL_REG, (dma_enable<<31 | 0 | rdata_byte_num<<16 | dbus_burst | 0 | wdata_byte_num));
    //wait (`AES_INTR);
    while(!AES_INTR);
    AES_INTR = 0;
    //`CHIP_CPU_BFM.ahbwr( `AES_INTR_REG, 3'd0, 4'd2, 32'h0001_0001);
    //`CHIP_CPU_BFM.ahbwr( `AES_CTRL_REG, 3'd0, 4'd2, 32'h8000_0000);  // aec_clear
    wr(AES_CTRL_REG, 0x80000000);

    //`CHIP_CPU_BFM.ahbrd_incr(`SYSRAM_ADDR_BASE+dma_str_waddr, 1, 16, 2, rdata, 1, 32'h0, 0);
    for (i=0; i<16; i++) {
        rd(SYSRAM_ADDR_BASE+dma_str_waddr+i*4, rdata);
    }

    //repeat(100)@(posedge `CHIP_CLK48M);    
    //$finish;

    return ret;
}
