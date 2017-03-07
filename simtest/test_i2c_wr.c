#include "stdint.h"
#include "addr_map.h"

static uint32_t int_en = 3;                 //2bit
static uint32_t dual_bank = 0;              //1bit
static uint32_t redu_en = 0;                //1bit
static uint32_t rdata;                      //32bit
static uint32_t data;                       //32bit

static uint32_t dma_str_raddr;              //17bit
static uint32_t dma_str_waddr;              //17bit
static uint32_t wdata_byte_num;             //12bit
static uint32_t rdata_byte_num;             //12bit
static uint32_t port_op_mode;               //2bit
static uint32_t dma_enable;                 //1bit
static uint32_t dbus_burst;					//1bit
static const uint32_t i2c_slv0_id = 0x1f;	//7bit
//wire [1:0]      i2c_slv0_core_select = 2'b10;
//wire [5:0]      i2c_slv0_mask = 6'd4;
static uint32_t ms_addr_16bit = 0;					//1bit
static uint32_t ms_address_en = 0;					//1bit
static uint32_t ms_no_stop = 0;					//1bit
//wire [6:0]      ms_slave_addr = i2c_slv0_id;
static uint32_t ms_word_addr=0;					//16bit
//wire [11:0]     ms_prescaler=12'd492;
static uint32_t ms_prescaler=52;					//12bit
//wire [5:0]	ms_i2c_mask =6'd4;
static uint32_t ms_i2c_mask =3;					//6bit

extern uint32_t I2C0_M_INTR;
extern uint32_t I2C1_M_INTR;
extern void set_port_i2c0_12(void);
extern void set_port_i2c1_4(void);
extern int rd_compare(uint32_t addr, uint32_t val, uint32_t compare_val);

static int i2c0_wr(uint32_t dma_str_raddr_t,
            uint32_t dma_str_waddr_t,
            uint32_t wdata_byte_num_t,
            uint32_t rdata_byte_num_t,
            uint32_t port_op_mode_t,
            uint32_t dma_enable_t,
            uint32_t dbus_burst_t);
static int i2c1_wr(uint32_t dma_str_raddr_t,
            uint32_t dma_str_waddr_t,
            uint32_t wdata_byte_num_t,
            uint32_t rdata_byte_num_t,
            uint32_t port_op_mode_t,
            uint32_t dma_enable_t,
            uint32_t dbus_burst_t);

//0: success; others: fail
int test_i2c_wr(void) {
    int ret = 0;

    //$display("@%t, %m MSG: ****** test name: init_i2c", $time);
    //wait (`TB_INIT_DONE);
    //$display("@%d, %m MSG: TB_INIT_DONEROM", $time);

    //@(posedge `CHIP_CLK48M);
    delayns(1);
    #if 0 // do not check rom, because this area is not 0
    // ROM
    //$display("@%d, MSG: AHB READ ROM", $time);
    //`CHIP_CPU_BFM.ahbrd( 32'h00, 0, 2, rdata, 1, 32'h0, 0);
    rd(0, rdata);
    //`CHIP_CPU_BFM.ahbrd_incr( 32'h00, 1, 16, 2, rdata, 1, 32'h0, 0);
    {
        int i;
        int addr=0;
        for (i=0; i<16; i++)
            rd(addr+4*i, rdata);
    }
    //`CHIP_CPU_BFM.ahbrd( 32'h00, 2, 2, rdata, 1, 32'h0, 0);
    rd(0, rdata);
    //`CHIP_CPU_BFM.ahbrd( 32'h00, 3, 2, rdata, 1, 32'h0, 0);
    rd(0, rdata);
    //`CHIP_CPU_BFM.ahbrd( 32'h00, 4, 2, rdata, 1, 32'h0, 0);
    rd(0, rdata);
    //`CHIP_CPU_BFM.ahbrd( 32'h00, 5, 2, rdata, 1, 32'h0, 0);
    rd(0, rdata);
    #endif
    // SYSRAM
    data=0;
    {
        int j;
        for (j=0;j<16;j++){
            data++; 
            //`CHIP_CPU_BFM.ahbwr( `SYSRAM_TEST_ADDR_BASE+j*4,0,2, data);
            wr(SYSRAM_TEST_ADDR_BASE+j*4, data);
            //`CHIP_CPU_BFM.ahbrd (`SYSRAM_TEST_ADDR_BASE+j*4,0,2,rdata,1,data,0);
            ret = rd_compare(SYSRAM_TEST_ADDR_BASE+j*4, rdata, data);
            if (ret == 0) return -1;
        }
    }





    // EF init
    //$display("@%d, MSG: AHB READ EF", $time);
    //`CHIP_CPU_BFM.ahbwr(`EF_INTERRUPT_REG, 3'd0, 4'd2, {30'h0,int_en});
    wr(EF_INTERRUPT_REG, (0 | int_en));
    //`CHIP_CPU_BFM.ahbrd(`EF_INTERRUPT_REG, 3'd0, 4'd2, rdata,1,{30'h0,int_en},1'b0);
    ret = rd_compare(EF_INTERRUPT_REG, rdata, (0 | int_en));
    //if (ret == 0) return -1;
    //`CHIP_CPU_BFM.ahbwr(`EF_CONFIG_REG, 3'd0, 4'd2, {16'h0,7'd0,redu_en,7'd0,dual_bank});
    wr(EF_CONFIG_REG, (0 | 0 | redu_en<<8 | 0 | dual_bank));
    //`CHIP_CPU_BFM.ahbrd(`EF_CONFIG_REG, 3'd0, 4'd2, rdata,1,{16'h0,7'd0,redu_en,7'd0,dual_bank},1'b0);
    ret = rd_compare(EF_CONFIG_REG, rdata, (0 | 0 | redu_en<<8 | 0 | dual_bank));
    //if (ret == 0) return -1;

    set_port_i2c0_12();
    set_port_i2c1_4();

    dma_str_raddr = (SYSRAM_TEST_ADDR_BASE-SYSRAM_ADDR_BASE);
    dma_str_waddr = 512;
    wdata_byte_num = 63;
    rdata_byte_num = 63;
    port_op_mode = 0;
    dma_enable = 1;
    dbus_burst=0;
    //fork
    #if 0
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
    #endif
    ret = i2c0_wr(dma_str_raddr,dma_str_waddr,wdata_byte_num,rdata_byte_num,port_op_mode,dma_enable,dbus_burst);
    //if (ret != 0) return -1;
    //join 
    dma_str_raddr = (SYSRAM_TEST_ADDR_BASE-SYSRAM_ADDR_BASE);
    dma_str_waddr = 512;
    wdata_byte_num = 63;
    rdata_byte_num = 63;
    port_op_mode = 0;
    dma_enable = 1;
    dbus_burst=0;
    //fork
    #if 0
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
    #endif
    ret = i2c1_wr(dma_str_raddr,dma_str_waddr,wdata_byte_num,rdata_byte_num,port_op_mode,dma_enable,dbus_burst);
    //if (ret != 0) return -1;
    //join 

    //repeat(100)@(posedge `CHIP_CLK48M);
    //$finish;

    return 0;
}

// -----------------------------------
static int i2c0_wr(uint32_t dma_str_raddr_t,
            uint32_t dma_str_waddr_t,
            uint32_t wdata_byte_num_t,
            uint32_t rdata_byte_num_t,
            uint32_t port_op_mode_t,
            uint32_t dma_enable_t,
            uint32_t dbus_burst_t) {
//input [16:0] dma_str_raddr_t;
//input [16:0] dma_str_waddr_t;
//input [11:0] wdata_byte_num_t;
//input [11:0] rdata_byte_num_t;
//input [1:0] port_op_mode_t;
//input [0:0] dma_enable_t;
//input [0:0] dbus_burst_t;
//reg [6:0] ahb_burst;

    //$display("@%d, MSG: I2C0 READ -> DBUS_WRITE", $time);

    //`CHIP_CPU_BFM.ahbwr( `I2C0_ADDR_BASE+32'h0, 3'd0, 4'd2, 32'h0000_0007); // intr enable
    wr(I2C0_ADDR_BASE+0, 0x00000007);
    //`CHIP_CPU_BFM.ahbrd( `I2C0_ADDR_BASE+32'h0, 3'd0, 4'd2, rdata, 1, 32'h0000_0007, 1'b0); // intr enable
    rd_compare(I2C0_ADDR_BASE+0, rdata, 0x00000007);
    //`CHIP_CPU_BFM.ahbwr( `I2C0_ADDR_BASE+32'h14, 3'd0, 4'd2, {4'd0,ms_prescaler[11:0],8'd0,ms_i2c_mask,2'd1});
	wr(I2C0_ADDR_BASE+0x14, (0 | ms_prescaler<<16 | 0 | ms_i2c_mask<<2 | 1));
	//`CHIP_CPU_BFM.ahbrd( `I2C0_ADDR_BASE+32'h14, 3'd0, 4'd2, rdata, 1, {4'd0,ms_prescaler[11:0],8'd0,ms_i2c_mask,2'd1}, 1'b0); // intr enable
    rd_compare(I2C0_ADDR_BASE+0x14, rdata, (0 | ms_prescaler<<16 | 0 | ms_i2c_mask<<2 | 1));
    //`CHIP_CPU_BFM.ahbwr( `I2C0_ADDR_BASE+32'h18, 3'd0, 4'd2, {ms_word_addr,1'b0,i2c_slv0_id,5'd0,ms_no_stop,ms_address_en,ms_addr_16bit});
    wr(I2C0_ADDR_BASE+0x18, (ms_word_addr<<16 | 0 | i2c_slv0_id<<8 | 0 | ms_no_stop<<2 | ms_address_en<<1 | ms_addr_16bit));
    //`CHIP_CPU_BFM.ahbrd( `I2C0_ADDR_BASE+32'h18, 3'd0, 4'd2, rdata, 1, {ms_word_addr,1'b0,i2c_slv0_id,5'd0,ms_no_stop,ms_address_en,ms_addr_16bit},1'b0);
    rd_compare(I2C0_ADDR_BASE+0x18, rdata, (ms_word_addr<<16 | 0 | i2c_slv0_id<<8 | 0 | ms_no_stop<<2 | ms_address_en<<1 | ms_addr_16bit));

    //`CHIP_CPU_BFM.ahbwr(`I2C0_ADDR_BASE+32'hc, 3'd0, 4'd2, {15'd0,dma_str_waddr_t[16:0]});
    wr(I2C0_ADDR_BASE+0xc, (0 | dma_str_waddr_t));
    //`CHIP_CPU_BFM.ahbwr(`I2C0_ADDR_BASE+32'h10, 3'd0, 4'd2, {15'd0,dma_str_raddr_t[16:0]});
    wr(I2C0_ADDR_BASE+0x10, (0 | dma_str_raddr_t));
    //`CHIP_CPU_BFM.ahbwr(`I2C0_ADDR_BASE+32'h8, 3'd0, 4'd2, {dma_enable_t,1'd0,port_op_mode_t,rdata_byte_num_t[11:0],dbus_burst_t,3'd0,wdata_byte_num_t[11:0]}); // intr enable
    wr(I2C0_ADDR_BASE+0x8, (dma_enable_t<<31 | 0 | port_op_mode_t<<28 | rdata_byte_num_t<<16 | dbus_burst_t<<15 | 0 | wdata_byte_num_t));
    //wait (`I2C0_INTR);
    while(!I2C0_M_INTR);
    I2C0_M_INTR = 0;
    //`CHIP_CPU_BFM.ahbrd(`I2C0_ADDR_BASE+32'h0, 3'd0, 4'd2, rdata, 1, 32'h0001_0007, 1'b0);
    //`CHIP_CPU_BFM.ahbwr(`I2C0_ADDR_BASE+32'h0, 3'd0, 4'd2, 32'h0001_0007);
    //wr(I2C0_ADDR_BASE+0, 0x00010007);
//    wait (`I2C0_INTR==0);
//    `CHIP_CPU_BFM.ahbwr(`I2C0_ADDR_BASE+32'h8, 3'd0, 4'd2, {dma_enable_t,1'd0,port_op_mode_t,rdata_byte_num_t[11:0],dbus_burst_t,3'd0,wdata_byte_num_t[11:0]}); // intr enable
//

    return 0;
}
            
// -----------------------------------
static int i2c1_wr(uint32_t dma_str_raddr_t,
            uint32_t dma_str_waddr_t,
            uint32_t wdata_byte_num_t,
            uint32_t rdata_byte_num_t,
            uint32_t port_op_mode_t,
            uint32_t dma_enable_t,
            uint32_t dbus_burst_t) {
//input [16:0] dma_str_raddr_t;
//input [16:0] dma_str_waddr_t;
//input [11:0] wdata_byte_num_t;
//input [11:0] rdata_byte_num_t;
//input [1:0] port_op_mode_t;
//input [0:0] dma_enable_t;
//input [0:0] dbus_burst_t;
//reg [6:0] ahb_burst;

    //$display("@%d, MSG: I2C1 READ -> DBUS_WRITE", $time);

    //`CHIP_CPU_BFM.ahbwr( `I2C1_ADDR_BASE+32'h0, 3'd0, 4'd2, 32'h0000_0007); // intr enable
    wr(I2C1_ADDR_BASE+0, 0x00000007);
    //`CHIP_CPU_BFM.ahbrd( `I2C1_ADDR_BASE+32'h0, 3'd0, 4'd2, rdata, 1, 32'h0000_0007, 1'b0); // intr enable
    rd_compare(I2C1_ADDR_BASE+0, rdata, 0x00000007);
    //`CHIP_CPU_BFM.ahbwr( `I2C1_ADDR_BASE+32'h14, 3'd0, 4'd2, {4'd0,ms_prescaler[11:0],8'd0,ms_i2c_mask,2'd1});
    wr(I2C1_ADDR_BASE+0x14, (0 | ms_prescaler<<16 | 0 | ms_i2c_mask<<2 | 1));
    //`CHIP_CPU_BFM.ahbrd( `I2C1_ADDR_BASE+32'h14, 3'd0, 4'd2, rdata, 1, {4'd0,ms_prescaler[11:0],8'd0,ms_i2c_mask,2'd1}, 1'b0); // intr enable
    rd_compare(I2C1_ADDR_BASE+0x14, rdata, (0 | ms_prescaler<<16 | 0 | ms_i2c_mask<<2 | 1));
    //`CHIP_CPU_BFM.ahbwr( `I2C1_ADDR_BASE+32'h18, 3'd0, 4'd2, {ms_word_addr,1'b0,i2c_slv0_id,5'd0,ms_no_stop,ms_address_en,ms_addr_16bit});
    wr(I2C1_ADDR_BASE+0x18, (ms_word_addr<<16 | 0 | i2c_slv0_id<<8 | 0 | ms_no_stop<<2 | ms_address_en<<1 | ms_addr_16bit));
    //`CHIP_CPU_BFM.ahbrd( `I2C1_ADDR_BASE+32'h18, 3'd0, 4'd2, rdata, 1, {ms_word_addr,1'b0,i2c_slv0_id,5'd0,ms_no_stop,ms_address_en,ms_addr_16bit},1'b0);
    rd_compare(I2C1_ADDR_BASE+0x18, rdata, (ms_word_addr<<16 | 0 | i2c_slv0_id<<8 | 0 | ms_no_stop<<2 | ms_address_en<<1 | ms_addr_16bit));

    //`CHIP_CPU_BFM.ahbwr(`I2C1_ADDR_BASE+32'hc, 3'd0, 4'd2, {15'd0,dma_str_waddr_t[16:0]});
    wr(I2C1_ADDR_BASE+0xc, (0 | dma_str_waddr_t));
    //`CHIP_CPU_BFM.ahbwr(`I2C1_ADDR_BASE+32'h10, 3'd0, 4'd2, {15'd0,dma_str_raddr_t[16:0]});
    wr(I2C1_ADDR_BASE+0x10, (0 | dma_str_raddr_t));
    //`CHIP_CPU_BFM.ahbwr(`I2C1_ADDR_BASE+32'h8, 3'd0, 4'd2, {dma_enable_t,1'd0,port_op_mode_t,rdata_byte_num_t[11:0],dbus_burst_t,3'd0,wdata_byte_num_t[11:0]}); // intr enable
    wr(I2C1_ADDR_BASE+0x8, (dma_enable_t<<31 | 0 | port_op_mode_t<<28 | rdata_byte_num_t<<16 | dbus_burst_t<<15 | 0 | wdata_byte_num_t));
    //wait (`I2C1_INTR);
    while(!I2C1_M_INTR);
    I2C1_M_INTR = 0;
    //`CHIP_CPU_BFM.ahbrd(`I2C1_ADDR_BASE+32'h0, 3'd0, 4'd2, rdata, 1, 32'h0001_0007, 1'b0);
    //`CHIP_CPU_BFM.ahbwr(`I2C1_ADDR_BASE+32'h0, 3'd0, 4'd2, 32'h0001_0007);
    //wr(I2C1_ADDR_BASE+0, 0x00010007);

    return 0;
}
// -----------------------------------

