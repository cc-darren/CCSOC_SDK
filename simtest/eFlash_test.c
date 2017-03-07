#include "stdint.h"
#include "addr_map.h"

uint32_t    int_en = 3;     //2bit
uint32_t    dual_bank = 0;  //1bit
uint32_t    redu_en = 0;    //1bit

uint32_t	rdata;
uint32_t	wdata;

uint32_t    dma_str_raddr;  //17bit
uint32_t    dma_str_waddr;  //17bit
uint32_t    wdata_byte_num; //12bit
uint32_t    rdata_byte_num; //12bit
uint32_t    port_op_mode;   //2bit
uint32_t    dma_enable;     //1bit
uint32_t    dbus_burst;     //1bit

uint32_t    spi_cs_pol;     //1bit
uint32_t    spi_cs;         //2bit
uint32_t    spi_cpol;       //1bit
uint32_t    spi_cpha;       //1bit
uint32_t    spi_en;         //1bit
uint32_t    spi_opmode;     //2bit
uint32_t    spi_wrswap;     //1bit
uint32_t    spi_rdswap;     //1bit
uint32_t    spi_wbytes;     //8bit
uint32_t    spi_rbytes;     //8bit
uint32_t    spi_waddr;      //24bit
uint32_t    spi_raddr;      //24bit
uint32_t    flash_op;       //8bit
uint32_t    flash_addr;     //24bit

uint8_t     hexBuffer[512];

//uint32_t    gpio_pin_en;
//uint32_t    gpio_port_mode;
//uint32_t    gpio_oen;

extern uint32_t SPI0_M_INTR;
extern void set_port_spim0_0(void);

void eFlash_test(void)
{
    //$display("@%t, %m MSG: ****** test name: test_exflash", $time);
    //wait (`TB_INIT_DONE);
    //$display("@%t, %m MSG: TB_INIT_DONEROM", $time);


    // GPIO
    //$display("@%d, MSG: setup GPIO REG, GPIO_AUX", $time);
    set_port_spim0_0();
    ////gpio_pin_en = 32'h0000_000f;
    ////gpio_port_mode = 32'h0000_0001;
    ////gpio_oen = 32'h0000_0007;
    ////`CHIP_CPU_BFM.ahbwr(`GPIO_AUX_REG, 3'd0, 4'd2,gpio_pin_en);
    ////`CHIP_CPU_BFM.ahbwr(`GPIO_AUX_PORT_MODE_REG, 3'd0,4'd2, gpio_port_mode);
    ////`CHIP_CPU_BFM.ahbwr(`GPIO_PAD_OE_REG, 3'd0,4'd2, gpio_oen);

    // SPI0
    //$display("@%d, MSG: SPI0_INTR_REG", $time);
    //`CHIP_CPU_BFM.ahbwr(`SPI0_INTR_REG, 3'd0, 4'd2, {30'h0,int_en});
    wr(SPI0_INTR_REG, int_en);
    //`CHIP_CPU_BFM.ahbrd(`SPI0_INTR_REG, 3'd0, 4'd2, rdata,1,{30'h0,int_en},1'b0);
    rd(SPI0_INTR_REG, rdata);

    //$display("@%d, MSG: SPI0_CTRL_REG", $time);
    spi_cs_pol = 1; // active low
    spi_cs = 0;
    spi_cpol = 0; // clock standby = 0
    spi_cpha = 0; // di sample@rise clk, do drive@fall clk
    wdata = (0x00 | spi_cs_pol<<10 | spi_cs<<8 | spi_cpol<<1 | spi_cpha);
    //`CHIP_CPU_BFM.ahbwr(`SPI0_CTRL_REG, 3'd0, 4'd2, wdata);
    wr(SPI0_CTRL_REG, wdata);
    //`CHIP_CPU_BFM.ahbrd(`SPI0_CTRL_REG, 3'd0, 4'd2, rdata,1,wdata,1'b0);
    rd(SPI0_CTRL_REG, rdata);

    //$display("@%d, MSG: SPI0 READ BYTE", $time);
    //spi_en = 1'b1;
    //spi_wrswap = 1'b1;
    //spi_rdswap = 1'b0;
    //spi_rbytes = 8'd3;
    //spi_wbytes = 8'd255;
    //spi_raddr = 24'd0;
    //spi_waddr = 24'd256;
    //spi_opmode = 2'd2;
    //flash_op = 8'h3;
    //flash_addr = 24'd0;
    spi_en = 1;
    spi_wrswap = 1;
    spi_rdswap = 0;
    spi_rbytes = 3;
    spi_wbytes = 255;
    spi_raddr = (uint32_t)hexBuffer;
    spi_waddr = 256;
    spi_opmode = 2;
    flash_op = 3;
    flash_addr = 0;

    //wdata = {flash_op, flash_addr};
    wdata = (flash_op<<24 | flash_addr);
    //`CHIP_CPU_BFM.ahbwr(`SYSRAM_ADDR_BASE+spi_raddr, 3'd0, 4'd2, wdata);
    wr((SYSRAM_ADDR_BASE+spi_raddr), wdata);

    //wdata = 32'd0 | spi_waddr;
    wdata = spi_waddr;
    //`CHIP_CPU_BFM.ahbwr(`SPI0_DMA_WR_REG, 3'd0, 4'd2, wdata);
    wr(SPI0_DMA_WR_REG, wdata);
    //`CHIP_CPU_BFM.ahbrd(`SPI0_DMA_WR_REG, 3'd0, 4'd2, rdata, 1, wdata, 1'b0);
    rd(SPI0_DMA_WR_REG, rdata);
    //wdata = 32'd0 | spi_raddr;
    wdata = spi_raddr;
    //`CHIP_CPU_BFM.ahbwr(`SPI0_DMA_RD_REG, 3'd0, 4'd2, wdata);
    wr(SPI0_DMA_RD_REG, wdata);
    //`CHIP_CPU_BFM.ahbrd(`SPI0_DMA_RD_REG, 3'd0, 4'd2, rdata, 1, wdata, 1'b0);
    rd(SPI0_DMA_RD_REG, rdata);

    //wdata = 32'd0 | spi_en<<24 | spi_rdswap<<19 | spi_wrswap<<18 | spi_opmode<<16 | spi_wbytes<<8 | spi_rbytes;
    wdata = (0x00 | spi_en<<24 | spi_rdswap<<19 | spi_wrswap<<18 | spi_opmode<<16 | spi_wbytes<<8 | spi_rbytes);
    //`CHIP_CPU_BFM.ahbwr(`SPI0_DMA_CTRL_REG, 3'd0, 4'd2, wdata);
    wr(SPI0_DMA_CTRL_REG, wdata);
    //`CHIP_CPU_BFM.ahbrd(`SPI0_DMA_CTRL_REG, 3'd0, 4'd2, rdata, 1, wdata,1'b0);
    rd(SPI0_DMA_CTRL_REG, rdata);

    //wait (`SPI0_M_INTR);
    while(!SPI0_M_INTR);
    SPI0_M_INTR = 0;

    //$display("@%d, %m MSG: detecting SPI0_M INTERRUPT", $time);
    //wdata = 32'd0 | 1'b1<<16 | int_en;
    wdata = (0x00 | 1<<16 | int_en);
    //`CHIP_CPU_BFM.ahbrd(`SPI0_INTR_REG, 3'd0, 4'd2, rdata,1,wdata,1'b0);
    rd(SPI0_INTR_REG, rdata);
    //`CHIP_CPU_BFM.ahbwr(`SPI0_INTR_REG, 3'd0, 4'd2, wdata);
    wr(SPI0_INTR_REG, wdata);

    //$display("@%d, %m MSG: READ SYSMEM CHECK FLASH DATA", $time);
    //`CHIP_CPU_BFM.set_rd_file("ahb_rdata.hex");
    //`CHIP_CPU_BFM.ahbrd_incr(`SYSRAM_ADDR_BASE+spi_waddr, 1, 16, 2, rdata, 1, 32'h0, 0);
    //`CHIP_CPU_BFM.unset_rd_file;


    //repeat(100)@(posedge `CHIP_CLK48M);
    delayns(100);

}

