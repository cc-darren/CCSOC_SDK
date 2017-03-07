#include "stdint.h"
#include "addr_map.h"

#define SPIDATA   0x12345678
#define SPIDATA_1 0x09abcdef
#define SPIDATA_2 0x55555555
#define SPIDATA_3 0xaaaaaaaa
#define SPIDATA_4 0x5a5a5a5a
#define SPIDATA_5 0xa5a5a5a5
#define SPIDATA_6 0x11118888

static uint32_t int_en = 3;                 //2bit
static uint32_t dual_bank = 0;              //1bit
static uint32_t redu_en = 0;                //1bit
static uint32_t rdata;                      //32bit
static uint32_t data;                       //32bit
//reg [31:0]	addr=32'h8000_6000; 
//integer		i;

//wire spim_clk;// = topsys.chip.GPIO[0];
//wire spim_cs ;// = topsys.chip.GPIO[1];
//wire spim_d  ;// = topsys.chip.GPIO[2];
//reg [31:0] spi_data = 0;

//reg [5:0] spim_cnt = 0;

// TX byte swap
static uint32_t tx_byte_swap = 0;           //32bit
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

extern volatile uint32_t I2C0_M_INTR;
extern volatile uint32_t I2C1_M_INTR;
extern volatile uint32_t SPI0_M_INTR;
extern volatile uint32_t SPI1_M_INTR;
extern volatile uint32_t SPI2_M_INTR;
extern volatile uint32_t DMU_INTR;
extern void set_port_i2c0_12(void);
extern void set_port_i2c1_4(void);
extern void set_port_spim0_0(void);
extern void set_port_spim1_16(void);
extern void set_port_spim2_8(void);
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
int test_dmu_perf(void) {
    int j;

    //$display("@%t, %m MSG: ****** test name: init_test_CPU_BFM", $time);
    //wait (`TB_INIT_DONE);
    //$display("@%d, %m MSG: TB_INIT_DONEROM", $time);

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
    for (j=0;j<16;j++) {
        data++; 
        //`CHIP_CPU_BFM.ahbwr( `SYSRAM_ADDR_BASE+j*4,0,2, data);
        wr(SYSRAM_TEST_ADDR_BASE+j*4, data);
        //`CHIP_CPU_BFM.ahbrd (`SYSRAM_ADDR_BASE+j*4,0,2,rdata,1,data,0); 
        rd_compare(SYSRAM_TEST_ADDR_BASE+j*4, rdata, data); 
    }

    // EF init
    //$display("@%d, MSG: AHB READ EF", $time);
    //`CHIP_CPU_BFM.ahbwr(`EF_INTERRUPT_REG, 3'd0, 4'd2, {30'h0,int_en});
    wr(EF_INTERRUPT_REG, (0x0 | int_en));
    //`CHIP_CPU_BFM.ahbrd(`EF_INTERRUPT_REG, 3'd0, 4'd2, rdata,1,{30'h0,int_en},1'b0);
    rd_compare(EF_INTERRUPT_REG, rdata, (0x0 | int_en));
    //`CHIP_CPU_BFM.ahbwr(`EF_CONFIG_REG, 3'd0, 4'd2, {16'h0,7'd0,redu_en,7'd0,dual_bank});
    wr(EF_CONFIG_REG, (0x0 | 0 | redu_en<<8 | 0 | dual_bank));
    //`CHIP_CPU_BFM.ahbrd(`EF_CONFIG_REG, 3'd0, 4'd2, rdata,1,{16'h0,7'd0,redu_en,7'd0,dual_bank},1'b0);
    rd_compare(EF_CONFIG_REG, rdata, (0x0 | 0 | redu_en<<8 | 0 | dual_bank));
    
    set_port_i2c0_12();
    set_port_i2c1_4();
    
    set_port_spim0_0();
    set_port_spim1_16();
    set_port_spim2_8();
  

    wr(SYSRAM_ADDR_BASE+0x2000, SPIDATA);
    wr(SYSRAM_ADDR_BASE+0x2004, SPIDATA_1);
    wr(SYSRAM_ADDR_BASE+0x2008, SPIDATA_2);
    wr(SYSRAM_ADDR_BASE+0x200c, SPIDATA_3);
    wr(SYSRAM_ADDR_BASE+0x2010, SPIDATA_4);
    wr(SYSRAM_ADDR_BASE+0x2014, SPIDATA_5);
    wr(SYSRAM_ADDR_BASE+0x2018, SPIDATA_6);
    wr(SYSRAM_ADDR_BASE+0x201c, SPIDATA_3);
    wr(SYSRAM_ADDR_BASE+0x2020, SPIDATA_4);
    wr(SYSRAM_ADDR_BASE+0x2024, SPIDATA_5);
    wr(SYSRAM_ADDR_BASE+0x2028, SPIDATA_6);
    wr(SYSRAM_ADDR_BASE+0x202c, SPIDATA_3);
    wr(SYSRAM_ADDR_BASE+0x2030, SPIDATA_4);
    wr(SYSRAM_ADDR_BASE+0x2034, SPIDATA_5);
    wr(SYSRAM_ADDR_BASE+0x2038, SPIDATA_6);
    wr(SYSRAM_ADDR_BASE+0x203c, SPIDATA_3);
    wr(SYSRAM_ADDR_BASE+0x2040, SPIDATA_4);
    wr(SYSRAM_ADDR_BASE+0x2044, SPIDATA_5);
    wr(SYSRAM_ADDR_BASE+0x2048, SPIDATA_6);
    wr(SYSRAM_ADDR_BASE+0x204c, SPIDATA_3);
    wr(SYSRAM_ADDR_BASE+0x2050, SPIDATA_4);
    wr(SYSRAM_ADDR_BASE+0x2054, SPIDATA_5);
    wr(SYSRAM_ADDR_BASE+0x2058, SPIDATA_6);
    wr(SYSRAM_ADDR_BASE+0x205c, SPIDATA_3);
    wr(SYSRAM_ADDR_BASE+0x2060, SPIDATA_4);
    wr(SYSRAM_ADDR_BASE+0x2064, SPIDATA_5);
    wr(SYSRAM_ADDR_BASE+0x2068, SPIDATA_6);
    wr(SYSRAM_ADDR_BASE+0x206c, SPIDATA_3);
    wr(SYSRAM_ADDR_BASE+0x2070, SPIDATA_4);
    wr(SYSRAM_ADDR_BASE+0x2074, SPIDATA_5);
    wr(SYSRAM_ADDR_BASE+0x2078, SPIDATA_6);
    wr(SYSRAM_ADDR_BASE+0x207c, SPIDATA_3);
    wr(SYSRAM_ADDR_BASE+0x2080, SPIDATA_4);
    wr(SYSRAM_ADDR_BASE+0x2084, SPIDATA_5);
    wr(SYSRAM_ADDR_BASE+0x2088, SPIDATA_6);
    wr(SYSRAM_ADDR_BASE+0x208c, SPIDATA_3);
    wr(SYSRAM_ADDR_BASE+0x2090, SPIDATA_4);
    wr(SYSRAM_ADDR_BASE+0x2094, SPIDATA_5);
    wr(SYSRAM_ADDR_BASE+0x2098, SPIDATA_6);
    wr(SYSRAM_ADDR_BASE+0x209c, SPIDATA_3);
    wr(SYSRAM_ADDR_BASE+0x20a0, SPIDATA_4);
    wr(SYSRAM_ADDR_BASE+0x20a4, SPIDATA_5);
    wr(SYSRAM_ADDR_BASE+0x20a8, SPIDATA_6);
    wr(SYSRAM_ADDR_BASE+0x20ac, SPIDATA_3);
    wr(SYSRAM_ADDR_BASE+0x20b0, SPIDATA_4);
    wr(SYSRAM_ADDR_BASE+0x20b4, SPIDATA_5);
    wr(SYSRAM_ADDR_BASE+0x20b8, SPIDATA_6);
    wr(SYSRAM_ADDR_BASE+0x20bc, SPIDATA_3);
    wr(SYSRAM_ADDR_BASE+0x20c0, SPIDATA_4);
    wr(SYSRAM_ADDR_BASE+0x20c4, SPIDATA_5);
    wr(SYSRAM_ADDR_BASE+0x20c8, SPIDATA_6);
    wr(SYSRAM_ADDR_BASE+0x20cc, SPIDATA_3);
    wr(SYSRAM_ADDR_BASE+0x20d0, SPIDATA_4);
    wr(SYSRAM_ADDR_BASE+0x20d4, SPIDATA_5);
    wr(SYSRAM_ADDR_BASE+0x20d8, SPIDATA_6);
    wr(SYSRAM_ADDR_BASE+0x20dc, SPIDATA_3);
    wr(SYSRAM_ADDR_BASE+0x20e0, SPIDATA_4);
    wr(SYSRAM_ADDR_BASE+0x20e4, SPIDATA_5);
    wr(SYSRAM_ADDR_BASE+0x20e8, SPIDATA_6);
    wr(SYSRAM_ADDR_BASE+0x20ec, SPIDATA_3);
    wr(SYSRAM_ADDR_BASE+0x20f0, SPIDATA_4);
    wr(SYSRAM_ADDR_BASE+0x20f4, SPIDATA_5);
    wr(SYSRAM_ADDR_BASE+0x20f8, SPIDATA_6);
    wr(SYSRAM_ADDR_BASE+0x20fc, SPIDATA_3);

    // SPI0 master setting 
    // program cpha/cpol == 3'b00 , cs = 2'b11
    //`CHIP_CPU_BFM.ahbwr(`SPI0_ADDR_BASE+32'h04, 3'd0, 4'd2, 32'h0000_0400 | 1'b1 << 24); 
    wr(SPI0_ADDR_BASE+0x04, (0x00000400 | 1 << 24)); 
    // program write starting address
    //`CHIP_CPU_BFM.ahbwr(`SPI0_ADDR_BASE+32'h0c, 3'd0, 4'd2, `SYSRAM_ADDR_BASE+32'h1000); 
    wr(SPI0_ADDR_BASE+0x0c, SYSRAM_ADDR_BASE+0x1000); 
    // program read starting address
    //`CHIP_CPU_BFM.ahbwr(`SPI0_ADDR_BASE+32'h10, 3'd0, 4'd2, `SYSRAM_ADDR_BASE+32'h2000); 
    wr(SPI0_ADDR_BASE+0x10, SYSRAM_ADDR_BASE+0x2000); 
    // program interrupt enable
    //`CHIP_CPU_BFM.ahbwr(`SPI0_ADDR_BASE+32'h00, 3'd0, 4'd2, 32'h003); 
    wr(SPI0_ADDR_BASE+0x00, 0x003); 
    
    //enable SPI write_only mode (op_mode =0,swap=1, w=1byte)
    //`CHIP_CPU_BFM.ahbwr(`SPI0_ADDR_BASE+32'h08, 3'd0, 4'd2, 32'h0000_00ff | tx_byte_swap <<18 | 1'b0 <<24); 
    wr(SPI0_ADDR_BASE+0x08, (0x000000ff | tx_byte_swap <<18 | 0 <<24)); 

    //`CHIP_CPU_BFM.ahbwr(`DMU_ADDR_BASE+32'h08, 3'd0, 4'd2, 32'h0000_0010); 
    wr(DMU_ADDR_BASE+0x08, 0x00000010); 



    // SPI1 master setting 
    // program cpha/cpol == 3'b00 , cs = 2'b11
    //`CHIP_CPU_BFM.ahbwr(`SPI1_ADDR_BASE+32'h04, 3'd0, 4'd2, 32'h0000_0400 | 1'b1 << 24); 
    wr(SPI1_ADDR_BASE+0x04, (0x00000400 | 1 << 24)); 
    // program write starting address
    //`CHIP_CPU_BFM.ahbwr(`SPI1_ADDR_BASE+32'h0c, 3'd0, 4'd2, `SYSRAM_ADDR_BASE+32'h1000); 
    wr(SPI1_ADDR_BASE+0x0c, SYSRAM_ADDR_BASE+0x1000); 
    // program read starting address
    //`CHIP_CPU_BFM.ahbwr(`SPI1_ADDR_BASE+32'h10, 3'd0, 4'd2, `SYSRAM_ADDR_BASE+32'h2000); 
    wr(SPI1_ADDR_BASE+0x10, SYSRAM_ADDR_BASE+0x2000); 
    // program interrupt enable
    //`CHIP_CPU_BFM.ahbwr(`SPI1_ADDR_BASE+32'h00, 3'd0, 4'd2, 32'h003); 
    wr(SPI1_ADDR_BASE+0x00, 0x003); 

    //enable SPI write_only mode (op_mode =0,swap=1, w=1byte)
    //`CHIP_CPU_BFM.ahbwr(`SPI1_ADDR_BASE+32'h08, 3'd0, 4'd2, 32'h0000_00ff | tx_byte_swap <<18 | 1'b0 <<24); 
    wr(SPI1_ADDR_BASE+0x08, (0x000000ff | tx_byte_swap <<18 | 0 <<24)); 

    //`CHIP_CPU_BFM.ahbwr(`DMU_ADDR_BASE+32'h28, 3'd0, 4'd2, 32'h0000_0010); 
    wr(DMU_ADDR_BASE+0x28, 0x00000010); 

    // SPI2 master setting 
    // program cpha/cpol == 3'b00 , cs = 2'b11
    //`CHIP_CPU_BFM.ahbwr(`SPI2_ADDR_BASE+32'h04, 3'd0, 4'd2, 32'h0000_0400 | 1'b1 << 24); 
    wr(SPI2_ADDR_BASE+0x04, (0x00000400 | 1 << 24)); 
    // program write starting address
    //`CHIP_CPU_BFM.ahbwr(`SPI2_ADDR_BASE+32'h0c, 3'd0, 4'd2, `SYSRAM_ADDR_BASE+32'h1000); 
    wr(SPI2_ADDR_BASE+0x0c, SYSRAM_ADDR_BASE+0x1000); 
    // program read starting address
    //`CHIP_CPU_BFM.ahbwr(`SPI2_ADDR_BASE+32'h10, 3'd0, 4'd2, `SYSRAM_ADDR_BASE+32'h2000); 
    wr(SPI2_ADDR_BASE+0x10, SYSRAM_ADDR_BASE+0x2000); 
    // program interrupt enable
    //`CHIP_CPU_BFM.ahbwr(`SPI2_ADDR_BASE+32'h00, 3'd0, 4'd2, 32'h003); 
    wr(SPI2_ADDR_BASE+0x00, 0x003); 

    //enable SPI write_only mode (op_mode =0,swap=1, w=1byte)
    //`CHIP_CPU_BFM.ahbwr(`SPI2_ADDR_BASE+32'h08, 3'd0, 4'd2, 32'h0000_00ff | tx_byte_swap <<18 | 1'b0 <<24); 
    wr(SPI2_ADDR_BASE+0x08, (0x000000ff | tx_byte_swap <<18 | 0 <<24)); 


    //`CHIP_CPU_BFM.ahbwr(`DMU_ADDR_BASE+32'h48, 3'd0, 4'd2, 32'h0000_0010); 
    wr(DMU_ADDR_BASE+0x48, 0x00000010); 
    
    // DMU enable
    //`CHIP_CPU_BFM.ahbwr(`DMU_ADDR_BASE+32'h00, 3'd0, 4'd2, 32'h0000_0001); 
    wr(DMU_ADDR_BASE+0x00, 0x00000001); 
    //`CHIP_CPU_BFM.ahbwr(`DMU_ADDR_BASE+32'h0c, 3'd0, 4'd2, 32'h0000_0000 | 1'b1 <<10); 
    wr(DMU_ADDR_BASE+0x0c, (0x00000000 | 1 <<10)); 
    // topsys.s_spi_bfm.s_spi_bfm[0].genblk1.s_spi_bfm.spi_s_wr(4 , tx_byte_swap, `SPIDATA);
    // DMU enable
    //`CHIP_CPU_BFM.ahbwr(`DMU_ADDR_BASE+32'h20, 3'd0, 4'd2, 32'h0000_0001); 
    wr(DMU_ADDR_BASE+0x20, 0x00000001); 
    //`CHIP_CPU_BFM.ahbwr(`DMU_ADDR_BASE+32'h2c, 3'd0, 4'd2, 32'h0000_0000 | 1'b1 <<10); 
    wr(DMU_ADDR_BASE+0x2c, (0x00000000 | 1 <<10)); 
    // topsys.s_spi_bfm.s_spi_bfm[1].genblk1.s_spi_bfm.spi_s_wr(4 , tx_byte_swap, `SPIDATA_2);
    // DMU enable
    //`CHIP_CPU_BFM.ahbwr(`DMU_ADDR_BASE+32'h40, 3'd0, 4'd2, 32'h0000_0001); 
    wr(DMU_ADDR_BASE+0x40, 0x00000001); 
    //`CHIP_CPU_BFM.ahbwr(`DMU_ADDR_BASE+32'h4c, 3'd0, 4'd2, 32'h0000_0000 | 1'b1 <<10); 
    wr(DMU_ADDR_BASE+0x4c, (0x00000000 | 1 <<10)); 
    // wait interrupt


    // DMU enable
    //`CHIP_CPU_BFM.ahbwr(`DMU_ADDR_BASE+32'h60, 3'd0, 4'd2, 32'h0000_0001); 
    wr(DMU_ADDR_BASE+0x60, 0x00000001); 
    //`CHIP_CPU_BFM.ahbwr(`DMU_ADDR_BASE+32'h6c, 3'd0, 4'd2, 32'h0000_0000 | 1'b1 <<10); 
    wr(DMU_ADDR_BASE+0x6c, (0x00000000 | 1 <<10)); 



    dma_str_raddr = 0;
    dma_str_waddr = 512;
    wdata_byte_num = 63;
    rdata_byte_num = 63;
    port_op_mode = 0;
    dma_enable = 0;
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
      i2c0_wr(dma_str_raddr,dma_str_waddr,wdata_byte_num,rdata_byte_num,port_op_mode,dma_enable,dbus_burst);
    //join 


    // DMU enable
    //`CHIP_CPU_BFM.ahbwr(`DMU_ADDR_BASE+32'h80, 3'd0, 4'd2, 32'h0000_0001); 
    wr(DMU_ADDR_BASE+0x80, 0x00000001); 
    //`CHIP_CPU_BFM.ahbwr(`DMU_ADDR_BASE+32'h8c, 3'd0, 4'd2, 32'h0000_0000 | 1'b1 <<10); 
    wr(DMU_ADDR_BASE+0x8c, (0x00000000 | 1 <<10)); 

    dma_str_raddr = 0;
    dma_str_waddr = 512;
    wdata_byte_num = 63;
    rdata_byte_num = 63;
    port_op_mode = 0;
    dma_enable = 0;
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
      i2c1_wr(dma_str_raddr,dma_str_waddr,wdata_byte_num,rdata_byte_num,port_op_mode,dma_enable,dbus_burst);
    //join 

    //wait(`SPI0_M_INTR);
    while(!SPI0_M_INTR);
    SPI0_M_INTR = 0;
    //`CHIP_CPU_BFM.ahbwr(`SPI0_ADDR_BASE+32'h00, 3'd0, 4'd2, 32'h1_0001); 
    //wait(~`SPI0_M_INTR);
    
    // wait interrupt
    //wait(`SPI1_M_INTR);
    while(!SPI1_M_INTR);
    SPI1_M_INTR = 0;
    //`CHIP_CPU_BFM.ahbwr(`SPI1_ADDR_BASE+32'h00, 3'd0, 4'd2, 32'h1_0001); 
    //wait(~`SPI1_M_INTR);
    
    //wait(`SPI2_M_INTR);
    while(!SPI2_M_INTR);
    SPI2_M_INTR = 0;
    //`CHIP_CPU_BFM.ahbwr(`SPI2_ADDR_BASE+32'h00, 3'd0, 4'd2, 32'h1_0001); 
    //wait(~`SPI2_M_INTR);

    //repeat(1500)@(posedge `CHIP_CLK48M);    

    //wait(topsys.chip.core_domain0.dmu_int);
    while(!DMU_INTR);
    DMU_INTR = 0;

    wr(DMU_ADDR_BASE+0x00, (0x00000001 | 1 <<16)); 
    wr(DMU_ADDR_BASE+0x0c, (0x00000000 | 1 <<11 | 0 <<10)); 
    wr(DMU_ADDR_BASE+0x20, (0x00000001 | 1 <<16)); 
    wr(DMU_ADDR_BASE+0x2c, (0x00000000 | 1 <<11 | 0 <<10)); 
    wr(DMU_ADDR_BASE+0x40, (0x00000001 | 1 <<16)); 
    wr(DMU_ADDR_BASE+0x4c, (0x00000000 | 1 <<11 | 0 <<10)); 
    wr(DMU_ADDR_BASE+0x60, (0x00000001 | 1 <<16)); 
    wr(DMU_ADDR_BASE+0x6c, (0x00000000 | 1 <<11 | 0 <<10)); 
    wr(DMU_ADDR_BASE+0x80, (0x00000001 | 1 <<16)); 
    wr(DMU_ADDR_BASE+0x8c, (0x00000000 | 1 <<11 | 0 <<10)); 

    //repeat(50)@(posedge `CHIP_CLK48M);    
 // wait(~topsys.chip.core_domain0.dmu_int);


    //repeat(15000)@(posedge `CHIP_CLK48M);    
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

    return 0;
}
// -----------------------------------
