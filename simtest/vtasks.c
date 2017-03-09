#include "stdint.h"
#include "addr_map.h"

//reg		init_done = 1'b0;
//uint32_t	gpio_pin_en = 0;
//uint32_t	gpio_port_mode = 0;
uint32_t	pd, byp, vco_div;    //1bit
uint8_t 	divc0, divm, divn;

extern uint32_t EFLASH_INTR;

//0: not equal; 1: equal
int rd_compare(uint32_t addr, uint32_t val, uint32_t compare_val)
{
	rd(addr, val);
	if (val != compare_val)
    {
        uint32_t err_count = 0;
        rd(GPIO_INTR_TYPE_REG, err_count);
        wr(GPIO_INTR_TYPE_REG, err_count+1);
		return 0;
    }
	return 1;
}

//0: not equal; 1: equal
int rd_mask_compare(uint32_t addr, uint32_t val, uint32_t compare_val, uint32_t mask)
{
	rd(addr, val);
	if ( (val&mask) != compare_val)
    {
        uint32_t err_count = 0;
        rd(GPIO_INTR_TYPE_REG, err_count);
        wr(GPIO_INTR_TYPE_REG, err_count+1);
		return 0;
    }
	return 1;
}

void test_init(void)
{
   //$display("%t %m FORCE------", $time);
   #ifdef FPGA
   #else
    //force `CHIP_CORE_AO.POR11_ULP.VDDC = 1'b0;                //need HW team to force VDDC
   #endif

    #ifdef FORCE_PLLCLK
	//force `CHIP_PLLCLK = `TOPSYS.T_PLLCLK;
    #endif

    // PMU
    ////force `CHIP_CORE1.SYSPWRDOWNACK = `CHIP_CORE1.SYSPWRDOWN;
    ////force `CHIP_CORE1.M0P_DBGPWRDOWNACK = `CHIP_CORE1.M0P_DBGPWRDOWN;

    #ifdef FPGA
    #else
    //#5; //dealy 5ns
    //release `CHIP_CORE_AO.POR11_ULP.VDDC;
    #endif

    ////repeat(3) @(posedge `CHIP_CLK32K);
    //#100;
    //`CHIP_RSTN = 1'b1;

#ifdef CPU_BFM
    //repeat(10) @(posedge `CHIP_CLK32K);
    delayns(10);
    #ifdef SCU_PLLCLK
        byp=0;
        pd=1;
	    divc0=0;
        divn=8;
        divm=20;
        vco_div = 0;
        //`CHIP_CPU_BFM.ahbwr(`SCU_PLLCFG_REG, 3'd0, 4'd2, {6'd0, byp, pd, divc0, divm, divn} );
        wr(SCU_PLLCFG_REG, (0 | vco_div<<26 | byp<<25 | pd<<24 | divc0<<16 | divm<<8 | divn) );
        //`CHIP_CPU_BFM.ahbwr(`SCU_CLKCFG0_REG, 3'd0, 4'd2, 32'h0000_0500);
        wr(SCU_CLKCFG0_REG, 0x00000500);
        //`CHIP_CPU_BFM.ahbwr(`SCU_CLKCFG1_REG, 3'd0, 4'd2, 32'h0000_0101);
        wr(SCU_CLKCFG1_REG, 0x00000101);
    #else
     #ifdef SCU_CLK32K
             $display("@t %m MSG: setup SCUGEN REG. sysclk=32khz", $time);
             `CHIP_CPU_BFM.ahbwr(`SCU_CLKCFG1_REG, 3'd0, 4'd2, 32'h0000_0100);
             `CHIP_CPU_BFM.ahbwr(`SCU_CLKCFG0_REG, 3'd0, 4'd2, 32'h0000_0000);
	 #else
             //$display("@t %m MSG: setup SCUGEN REG. sysclk=48mhz", $time);
             //`CHIP_CPU_BFM.ahbwr(`SCU_CLKCFG1_REG, 3'd0, 4'd2, 32'h0000_0101);
             wr(SCU_CLKCFG1_REG, 0x00000301);
             //`CHIP_CPU_BFM.ahbwr(`SCU_CLKCFG0_REG, 3'd0, 4'd2, 32'h0000_0000);
             //*((volatile unsigned int *)(SCU_CLKCFG0_REG)) = 0x00000000;
             //wr(SCU_CLKCFG0_REG, 0x00000000);
	 #endif
    #endif

     //$display("@t %m MSG: setup CKGEN REG, all device on. device clk=48mhz", $time);

    //`CHIP_CPU_BFM.ahbwr(`CKGEN_CFG1_REG, 3'd0, 4'd2, 32'h0000_0000 | 1'b1<<29 | 1'b1<<21 | 1'b1<<13 | 1'b1<<5);
    wr(CKGEN_CFG1_REG, (0x00000000 | 1<<29 | 1<<21 | 1<<13 | 1<<5));
    //`CHIP_CPU_BFM.ahbwr(`CKGEN_CFG2_REG, 3'd0, 4'd2, 32'h0000_0000 | 1'b1<<29 | 1'b1<<21 | 1'b1<<13 | 1'b1<<5);
    wr(CKGEN_CFG2_REG, (0x00000000 | 1<<29 | 1<<21 | 1<<13 | 1<<5));
    //`CHIP_CPU_BFM.ahbwr(`CKGEN_CFG3_REG, 3'd0, 4'd2, 32'h0000_0000 | 1'b1<<29 | 1'b1<<21 | 1'b1<<13 | 1'b1<<5);
    wr(CKGEN_CFG3_REG, (0x00000000 | 1<<29 | 1<<21 | 1<<13 | 1<<5));
    //`CHIP_CPU_BFM.ahbwr(`CKGEN_CFG4_REG, 3'd0, 4'd2, 32'h0000_0000 | 1'b1<<5);
    wr(CKGEN_CFG4_REG, (0x00000000 | 1<<5));

    ////`CHIP_CPU_BFM.ahbwr(`CKGEN_CLKEN_REG, 3'd0, 4'd2, 32'hffff_ffff);
    ////`CHIP_CPU_BFM.ahbwr(`CKGEN_SWRST_REG, 3'd0, 4'd2, 32'hffe0_0000);
    ////`CHIP_CPU_BFM.ahbwr(`CKGEN_SWRST_REG, 3'd0, 4'd2, 32'hffff_ffff);

    //repeat(3) @(posedge `CHIP_CLK32K);
    delayns(3);

#endif


    //init_done = 1'b1;


}


#if 0
`ifdef CPU_BFM
/////////////////////////////////////////
task ic_flush;
reg [31:0] wdata, rdata;

begin
    $display("@%d, MSG: %m ic_flush", $time);
`ifdef M4_SELECT
    `CHIP_CPU_BFM.ahbrd( `SCU_ICACHE_REG, 0, 2, rdata, 1, 32'h0 | 1'b1<<9, 0);
`else
    `CHIP_CPU_BFM.ahbrd( `SCU_ICACHE_REG, 0, 2, rdata, 1, 32'h0 | 1'b0<<9, 0);
`endif
    wdata = rdata | 1'b1;
    `CHIP_CPU_BFM.ahbwr(`SCU_ICACHE_REG, 3'd0, 4'd2, wdata);

    wait (`CHIP_CORE1.ic_flush_dn);
    wdata = {wdata[31:1],1'b0};
    `CHIP_CPU_BFM.ahbwr(`SCU_ICACHE_REG, 3'd0, 4'd2, wdata);
end
endtask
#endif

/////////////////////////////////////////
void ef_setmode(uint32_t data_erase_mode, uint32_t data_erase_enable, uint32_t rw_mode) {
//input [3:0] data_erase_mode;
//input 	data_erase_enable;
//input 	rw_mode;
    uint32_t wdata, rdata;  //32bit
    uint32_t int_en;        //2bit

    int_en=3;
    wr(EF_INTERRUPT_REG, int_en);

    wdata = (data_erase_enable<<8 | rw_mode<<4 | data_erase_mode);
    //`CHIP_CPU_BFM.ahbwr(`EF_FLASHMODE_REG, 3'd0, 4'd2, wdata);
    wr(EF_FLASHMODE_REG, wdata);

    if (data_erase_enable)
    {
        while(!EFLASH_INTR);
        EFLASH_INTR = 0;
        
        if (data_erase_mode & 0x0C)
        {
            //rdata = {8'd0,6'd0,1'b1,1'b0,8'd0,6'd0,2'd3};
        }
        else
        {
            //rdata = (0 | 0 | 0 | 1<<16 | 0 | 0 | 33);
        }
        
        //`CHIP_CPU_BFM.ahbrd(`EF_INTERRUPT_REG, 3'd0, 4'd2, rdata,1,rdata,1'b0);
        rd(EF_INTERRUPT_REG, rdata);
        //`CHIP_CPU_BFM.ahbwr(`EF_INTERRUPT_REG, 3'd0, 4'd2, rdata);  // wr clear
        wr(EF_INTERRUPT_REG, rdata);  // wr clear
    }
}
/////////////////////////////////////////
void ef_setmode_dma(uint32_t data_erase_mode, uint32_t data_erase_enable, uint32_t rw_mode) {
//input [3:0] data_erase_mode;
//input   data_erase_enable;
//input   rw_mode;
    uint32_t wdata;      //32bit
    uint32_t int_en;            //3bit
    uint32_t dma_mode;          //1bit

    //$display("@%d, MSG: %m ef_setmode_dma. data_erase_mode=%x, data_erase_enable=%x, rw_mode=%x", $time, data_erase_mode, data_erase_enable, rw_mode);
    int_en=3;
    dma_mode=1;
    //`CHIP_CPU_BFM.ahbwr(`EF_INTERRUPT_REG, 3'd0, 4'd2, 32'd0 | int_en);
    wr(EF_INTERRUPT_REG, (0 | int_en));

    wdata = (0 | data_erase_enable<<8 | 0 | dma_mode<<5 | rw_mode<<4 | data_erase_mode);
    //`CHIP_CPU_BFM.ahbwr(`EF_FLASHMODE_REG, 2'd0, 4'd2, wdata);
    wr(EF_FLASHMODE_REG, wdata);
}
/////////////////////////////////////////
#if 0
task ef_wr;
input [31:0] addr;
input [31:0] wdata_i;
input	     redu;

reg [19:0]	start_addr;
reg [9:0]	dw_num;
reg		dual_bank;
reg [3:0]	data_erase_mode;
reg		data_erase_enable;
reg		rw_mode;
reg [31:0]	wdata, rdata;
reg [1:0]	int_en;

begin
    $display("@%0d, MSG: %m ef_wr addr=%0x wdata=%x redu=%0x", $time, addr,wdata,redu);

    int_en = 2'd3;
    `CHIP_CPU_BFM.ahbwr(`EF_INTERRUPT_REG, 3'd0, 4'd2, {30'h0,int_en});
    `CHIP_CPU_BFM.ahbrd(`EF_INTERRUPT_REG, 3'd0, 4'd2, rdata,1,{30'h0,int_en},1'b0);

    dw_num=0;
    start_addr=addr[19:0];
    data_erase_mode=redu ? 4'b0001 : 4'b0000;
    data_erase_enable=1'b1;
    rw_mode=1'b0;
    dual_bank=0;

    wdata = {6'd0,dw_num,7'd0,redu,7'd0,dual_bank};
    `CHIP_CPU_BFM.ahbwr(`EF_CONFIG_REG, 3'd0, 4'd2, wdata);
    `CHIP_CPU_BFM.ahbrd(`EF_CONFIG_REG, 3'd0, 4'd2, rdata,1,wdata,1'b0);

    wdata = {4'd0,start_addr,8'd0};
    `CHIP_CPU_BFM.ahbwr(`EF_ACCESS_REG, 3'd0, 4'd2, wdata);
    `CHIP_CPU_BFM.ahbrd(`EF_ACCESS_REG, 3'd0, 4'd2, rdata,1,wdata,1'b0);

    `CHIP_CPU_BFM.ahbwr(`EF_WR_DATA0_REG, 3'd0, 4'd2, wdata_i);

    ef_setmode( data_erase_mode, data_erase_enable, rw_mode);

    $display("@%0d, MSG: %m : receive data_interrupt", $time);

end
endtask

/////////////////////////////////////////
task ef_rd;
input [31:0] addr;
input        redu;
input [31:0] exp_data;

reg [19:0]      start_addr;
reg [9:0]       dw_num;
reg             dual_bank;
reg [3:0]       data_erase_mode;
reg             data_erase_enable;
reg             rw_mode;
reg [31:0]      wdata, rdata;
reg [1:0]       int_en;

begin
    $display("@%0d, MSG: %m ef_rd addr=%0x exp_data=%x redu=%0x", $time, addr,exp_data,redu);

    int_en = 2'd3;
    `CHIP_CPU_BFM.ahbwr(`EF_INTERRUPT_REG, 3'd0, 4'd2, {30'h0,int_en});
    `CHIP_CPU_BFM.ahbrd(`EF_INTERRUPT_REG, 3'd0, 4'd2, rdata,1,{30'h0,int_en},1'b0);

    dw_num=0;
    start_addr=addr[19:0];
    data_erase_mode=4'b0000;
    data_erase_enable=1'b1;
    rw_mode=1'b1;
    dual_bank=0;

    wdata = {6'd0,dw_num,7'd0,redu,7'd0,dual_bank};
    `CHIP_CPU_BFM.ahbwr(`EF_CONFIG_REG, 3'd0, 4'd2, wdata);
    `CHIP_CPU_BFM.ahbrd(`EF_CONFIG_REG, 3'd0, 4'd2, rdata,1,wdata,1'b0);

    wdata = {4'd0,start_addr,8'd0};
    `CHIP_CPU_BFM.ahbwr(`EF_ACCESS_REG, 3'd0, 4'd2, wdata);
    `CHIP_CPU_BFM.ahbrd(`EF_ACCESS_REG, 3'd0, 4'd2, rdata,1,wdata,1'b0);

    ef_setmode( data_erase_mode, data_erase_enable, rw_mode);

    `CHIP_CPU_BFM.ahbrd(`EF_RD_DATA0_REG, 3'd0, 4'd2, rdata,1,exp_data,1'b0);
    $display("@%0d, MSG: %m : receive data_interrupt", $time);

end
endtask

/////////////////////////////////////////
task ef_mas;
input [19:0] addr;
reg [19:0]      start_addr;
reg [9:0]       dw_num;
reg             dual_bank;
reg [3:0]       data_erase_mode;
reg             data_erase_enable;
reg             rw_mode;
reg [31:0]      wdata, rdata;
reg [1:0]       int_en;

begin
    $display("@%0d, MSG: %m ef_mas", $time);
    dw_num=10'd0;
    dual_bank=0;
    wdata = {6'd0,dw_num,15'd0,dual_bank};
    `CHIP_CPU_BFM.ahbwr(`EF_CONFIG_REG, 3'd0, 4'd2, wdata);
    `CHIP_CPU_BFM.ahbrd(`EF_CONFIG_REG, 3'd0, 4'd2, rdata,1,wdata,1'b0);

    start_addr=addr[19:0];
    wdata = {4'd0,start_addr,8'd0};
    `CHIP_CPU_BFM.ahbwr(`EF_ACCESS_REG, 3'd0, 4'd2, wdata);
    `CHIP_CPU_BFM.ahbrd(`EF_ACCESS_REG, 3'd0, 4'd2, rdata,1,wdata,1'b0);

    data_erase_mode = 4'b1100;
    data_erase_enable = 1'b1;
    rw_mode=0;
    wdata = {23'd0,data_erase_enable,3'd0,rw_mode,data_erase_mode};
    `CHIP_CPU_BFM.ahbwr(`EF_FLASHMODE_REG, 3'd0, 4'd2, wdata);
    wait(`EF_INTR);
    rdata = {8'd0,6'd0,1'b1,1'b0,8'd0,6'd0,2'd3};
    `CHIP_CPU_BFM.ahbrd(`EF_INTERRUPT_REG, 3'd0, 4'd2, rdata,1,rdata,1'b0);
    `CHIP_CPU_BFM.ahbwr(`EF_INTERRUPT_REG, 3'd0, 4'd2, rdata);  // wr clear
    $display("@%0d, MSG: set_mas: receive erase_interrupt", $time);
end
endtask
#endif
/////////////////////////////////////////
void set_ports_pu(uint32_t st_bit, uint32_t gpio_pin_en)
{
//input [5:0]     st_bit;
//input [3:0]     gpio_pin_en;
uint32_t	mask;
uint32_t	rdata;
uint32_t	st_bit_t;

    //$display("@%d, MSG: %m set_ports_pu. st_bit=%0x, gpio_pin_en=%0x", $time, st_bit,gpio_pin_en);
    if (st_bit<32) {
         //mask = 32'b0 | (4'hf<<st_bit);
         mask = (0 | (0xf<<st_bit));
         //`CHIP_CPU_BFM.ahbrd(`GPIO_PAD_PULLUP_REG, 3'd0, 4'd2, rdata, 0, rdata,1'b0);
         rd(GPIO_PAD_PULLUP_REG, rdata);
         //`CHIP_CPU_BFM.ahbwr(`GPIO_PAD_PULLUP_REG, 3'd0, 4'd2, (rdata & ~mask) | gpio_pin_en<<st_bit);      // gpio pin en 
         wr(GPIO_PAD_PULLUP_REG, (rdata & ~mask) | gpio_pin_en<<st_bit);
    }
    else {
         st_bit_t = st_bit-32;
         //mask = 32'b0 | (4'hf<<st_bit_t);
         mask = (0 | (0xf<<st_bit_t));
         //`CHIP_CPU_BFM.ahbrd(`GPIO1_PAD_PULLUP_REG, 3'd0, 4'd2, rdata, 0, rdata,1'b0);
         rd(GPIO1_PAD_PULLUP_REG, rdata);
         //`CHIP_CPU_BFM.ahbwr(`GPIO1_PAD_PULLUP_REG, 3'd0, 4'd2, (rdata & ~mask) | gpio_pin_en<<st_bit);      // gpio pin en 
         wr(GPIO1_PAD_PULLUP_REG, (rdata & ~mask) | gpio_pin_en<<st_bit);
    }
}
/////////////////////////////////////////
void set_ports(uint32_t st_bit, uint32_t gpio_pin_en, uint32_t gpio_port_mode, uint32_t gpio_oe)
{
//input [5:0]	st_bit;
//input [3:0]	gpio_pin_en;
//input [3:0]	gpio_port_mode;
//input [3:0]	gpio_oe;
uint32_t	mask;
uint32_t	rdata;
uint32_t	st_bit_t;

    //$display("@%d, MSG: %m set_ports. gpio_pin_en=%0x, gpio_port_mode=%0x", $time, gpio_pin_en, gpio_port_mode);
    if (st_bit<32) {
        //mask = 32'b0 | (4'hf<<st_bit);
        mask = (0 | (0xf << st_bit));
        //`CHIP_CPU_BFM.ahbrd(`GPIO_AUX_REG, 3'd0, 4'd2, rdata, 0, rdata,1'b0);
        rd(GPIO_AUX_REG, rdata);
        //`CHIP_CPU_BFM.ahbwr(`GPIO_AUX_REG, 3'd0, 4'd2, (rdata & ~mask) | gpio_pin_en<<st_bit);      // gpio pin en
        wr(GPIO_AUX_REG, ((rdata & ~mask) | gpio_pin_en<<st_bit));
        //`CHIP_CPU_BFM.ahbrd(`GPIO_AUX_PORT_MODE_REG, 3'd0, 4'd2, rdata, 0, rdata,1'b0);    // gpio port mode
        rd(GPIO_AUX_PORT_MODE_REG,rdata);
        //`CHIP_CPU_BFM.ahbwr(`GPIO_AUX_PORT_MODE_REG, 3'd0, 4'd2, (rdata & ~mask) | gpio_port_mode<<st_bit);    // gpio port mode
        wr(GPIO_AUX_PORT_MODE_REG, ((rdata & ~mask) | gpio_port_mode<<st_bit));
        //`CHIP_CPU_BFM.ahbrd(`GPIO_PAD_OE_REG, 3'd0,4'd2, rdata, 0, rdata, 1'b0);
        rd(GPIO_PAD_OE_REG, rdata);
        //`CHIP_CPU_BFM.ahbwr(`GPIO_PAD_OE_REG, 3'd0,4'd2, (rdata & ~mask) | gpio_oe<<st_bit);
        wr(GPIO_PAD_OE_REG, ((rdata & ~mask) | gpio_oe<<st_bit));
    }
    else {
        st_bit_t = st_bit-32;
        //mask = 32'b0 | (4'hf<<st_bit_t);
        mask = (0 | (0xf << st_bit_t)); 
        //`CHIP_CPU_BFM.ahbrd(`GPIO1_AUX_REG, 3'd0, 4'd2, rdata, 0, rdata,1'b0);
        rd(GPIO1_AUX_REG, rdata);
        //`CHIP_CPU_BFM.ahbrd(`GPIO1_AUX_REG, 3'd0, 4'd2, rdata, 0, rdata,1'b0);
        rd(GPIO1_AUX_REG, rdata);
        //`CHIP_CPU_BFM.ahbwr(`GPIO1_AUX_REG, 3'd0, 4'd2, (rdata & ~mask) | gpio_pin_en<<st_bit_t);      // gpio pin en 
        wr(GPIO1_AUX_REG, (rdata & ~mask) | gpio_pin_en<<st_bit_t);
        //`CHIP_CPU_BFM.ahbrd(`GPIO1_AUX_PORT_MODE_REG, 3'd0, 4'd2, rdata, 0, rdata,1'b0);    // gpio port mode
        rd(GPIO1_AUX_PORT_MODE_REG, rdata);
        //`CHIP_CPU_BFM.ahbwr(`GPIO1_AUX_PORT_MODE_REG, 3'd0, 4'd2, (rdata & ~mask) | gpio_port_mode<<st_bit_t);    // gpio port mode
        wr(GPIO1_AUX_PORT_MODE_REG, (rdata & ~mask) | gpio_port_mode<<st_bit_t);
        //`CHIP_CPU_BFM.ahbrd(`GPIO1_PAD_OE_REG, 3'd0,4'd2, rdata, 0, rdata, 1'b0);
        rd(GPIO1_PAD_OE_REG, rdata);
        //`CHIP_CPU_BFM.ahbwr(`GPIO1_PAD_OE_REG, 3'd0,4'd2, (rdata & ~mask) | gpio_oe<<st_bit_t);
        wr(GPIO1_PAD_OE_REG, (rdata & ~mask) | gpio_oe<<st_bit_t);
    }
}

void set_gpio1_ports(uint32_t st_bit, uint32_t gpio_pin_en, uint32_t gpio_port_mode, uint32_t gpio_oe)
//input [4:0]	st_bit;
//input [3:0]	gpio_pin_en;
//input [3:0]	gpio_port_mode;    
//input [3:0]	gpio_oe;    
{
    uint32_t mask;
    uint32_t rdata;

    //$display("@%d, MSG: %m set_ports. gpio_pin_en=%0x, gpio_port_mode=%0x", $time, gpio_pin_en, gpio_port_mode);
    //mask = 32'b0 | (4'hf<<st_bit); 
    mask = (0 | (0xf << st_bit));
    //`CHIP_CPU_BFM.ahbrd(`GPIO1_AUX_REG, 3'd0, 4'd2, rdata, 0, rdata,1'b0);
    rd(GPIO1_AUX_REG, rdata);
    //`CHIP_CPU_BFM.ahbwr(`GPIO1_AUX_REG, 3'd0, 4'd2, (rdata & ~mask) | gpio_pin_en<<st_bit);      // gpio pin en 
    wr(GPIO1_AUX_REG, ((rdata & ~mask) | gpio_pin_en<<st_bit));
    //`CHIP_CPU_BFM.ahbrd(`GPIO1_AUX_PORT_MODE_REG, 3'd0, 4'd2, rdata, 0, rdata,1'b0);    // gpio port mode
    rd(GPIO1_AUX_PORT_MODE_REG, rdata);
    //`CHIP_CPU_BFM.ahbwr(`GPIO1_AUX_PORT_MODE_REG, 3'd0, 4'd2, (rdata & ~mask) | gpio_port_mode<<st_bit);    // gpio port mode
    wr(GPIO1_AUX_PORT_MODE_REG, ((rdata & ~mask) | gpio_port_mode<<st_bit));
    //`CHIP_CPU_BFM.ahbrd(`GPIO1_PAD_OE_REG, 3'd0,4'd2, rdata, 0, rdata, 1'b0);
    rd(GPIO1_PAD_OE_REG, rdata);
    //`CHIP_CPU_BFM.ahbwr(`GPIO1_PAD_OE_REG, 3'd0,4'd2, (rdata & ~mask) | gpio_oe<<st_bit);
    wr(GPIO1_PAD_OE_REG, ((rdata & ~mask) | gpio_oe<<st_bit));
}

/////////////////////////////////////////
void set_port_uart0(void)
{
    //$display("@%d, MSG: %m set_uart0", $time);
    set_ports(0, 0xf, 4, 0x3);
}

#if 0
task set_port_uart0_12;
begin
    $display("@%d, MSG: %m set_uart0_12", $time);
    set_ports(12, 4'hf, 4'd4, 4'h3);
end
endtask
task set_port_uart0_24;
begin
    $display("@%d, MSG: %m set_uart0_24", $time);
    set_ports(24, 4'hf, 4'd4, 4'h3);
end
endtask
task set_port_uart0_32;
begin
    $display("@%d, MSG: %m set_uart0_32", $time);
    set_ports(32, 4'hf, 4'd4, 4'h3);
end
endtask
task set_port_uart0_44;
begin
    $display("@%d, MSG: %m set_uart0_44", $time);
    set_ports(44, 4'hf, 4'd4, 4'h3);
end
endtask
#endif

void set_port_uart1(void)
{
    //$display("@%d, MSG: %m set_uart1", $time);
    set_ports(4, 0xf, 4, 0x3);
}

#if 0
task set_port_uart1_16;
begin
    $display("@%d, MSG: %m set_uart1_16", $time);
    set_ports(16, 4'hf, 4'd4, 4'h3);
end
endtask
task set_port_uart1_28;
begin
    $display("@%d, MSG: %m set_uart1_28", $time);
    set_ports(28, 4'hf, 4'd4, 4'h3);
end
endtask
task set_port_uart1_36;
begin
    $display("@%d, MSG: %m set_uart1_36", $time);
    set_ports(36, 4'hf, 4'd4, 4'h3);
end
endtask
#endif

void set_port_uart2(void)
{
    //$display("@%d, MSG: %m set_uart2", $time);
    set_ports(8, 0xf, 4, 0x3);
}

#if 0
task set_port_uart2_20;
begin
    $display("@%d, MSG: %m set_uart2_20", $time);
    set_ports(20, 4'hf, 4'd4, 4'h3);
end
endtask
task set_port_uart2_40;
begin
    $display("@%d, MSG: %m set_uart2_40", $time);
    set_ports(40, 4'hf, 4'd4, 4'h3);
end
endtask

/////////////////////////////////////////
task set_port_i2s_m0;
begin
    $display("@%d, MSG: %m set_i2s_m0", $time);
    set_ports(0, 4'hf, 4'd2, 4'h7);
end
endtask
task set_port_i2s_m4;
begin
    $display("@%d, MSG: %m set_i2s_m4", $time);
    set_ports(4, 4'hf, 4'd2, 4'h7);
end
endtask
#endif

void set_port_i2s_m8(void)
{
    //$display("@%d, MSG: %m set_i2s_m8", $time);
    set_ports(8, 0xf, 2, 0x7);
}

#if 0
task set_port_i2s_m12;
begin
    $display("@%d, MSG: %m set_i2s_m12", $time);
    set_ports(12, 4'hf, 4'd2, 4'h7);
end
endtask
task set_port_i2s_m16;
begin
    $display("@%d, MSG: %m set_i2s_m16", $time);
    set_ports(16, 4'hf, 4'd2, 4'h7);
end
endtask

task set_port_i2s_m20;
begin
    $display("@%d, MSG: %m set_i2s_m20", $time);
    set_ports(20, 4'hf, 4'd2, 4'h7);
end
endtask
task set_port_i2s_m24;
begin
    $display("@%d, MSG: %m set_i2s_m24", $time);
    set_ports(24, 4'hf, 4'd2, 4'h7);
end
endtask
task set_port_i2s_m28;
begin
    $display("@%d, MSG: %m set_i2s_m28", $time);
    set_ports(28, 4'hf, 4'd2, 4'h7);
end
endtask
task set_port_i2s_m32;
begin
    $display("@%d, MSG: %m set_i2s_m32", $time);
    set_gpio1_ports(0, 4'hf, 4'd2, 4'h7);
end
endtask
task set_port_i2s_m36;
begin
    $display("@%d, MSG: %m set_i2s_m36", $time);
    set_gpio1_ports(4, 4'hf, 4'd2, 4'h7);
end
endtask
task set_port_i2s_m40;
begin
    $display("@%d, MSG: %m set_i2s_m40", $time);
    set_gpio1_ports(8, 4'hf, 4'd2, 4'h7);
end
endtask
task set_port_i2s_m44;
begin
    $display("@%d, MSG: %m set_i2s_m44", $time);
    set_gpio1_ports(12, 4'hf, 4'd2, 4'h7);
end
endtask


task set_port_i2s_s0;
begin
    $display("@%d, MSG: %m set_i2s_s0", $time);
    set_ports(0, 4'hf, 4'd3, 4'h4);
end
endtask
#endif

void set_port_i2s_s4(void)
{
    //$display("@%d, MSG: %m set_i2s_s4", $time);
    set_ports(4, 0xf, 3, 0x4);
}

#if 0
task set_port_i2s_s8;
begin
    $display("@%d, MSG: %m set_i2s_s8", $time);
    set_ports(8, 4'hf, 4'd3, 4'h4);
end
endtask
task set_port_i2s_s12;
begin
    $display("@%d, MSG: %m set_i2s_s12", $time);
    set_ports(12, 4'hf, 4'd3, 4'h4);
end
endtask
task set_port_i2s_s16;
begin
    $display("@%d, MSG: %m set_i2s_s16", $time);
    set_ports(16, 4'hf, 4'd3, 4'h4);
end
endtask
task set_port_i2s_s20;
begin
    $display("@%d, MSG: %m set_i2s_s20", $time);
    set_ports(20, 4'hf, 4'd3, 4'h4);
end
endtask
task set_port_i2s_s24;
begin
    $display("@%d, MSG: %m set_i2s_s24", $time);
    set_ports(24, 4'hf, 4'd3, 4'h4);
end
endtask
task set_port_i2s_s28;
begin
    $display("@%d, MSG: %m set_i2s_s28", $time);
    set_ports(28, 4'hf, 4'd3, 4'h4);
end
endtask
task set_port_i2s_s32;
begin
    $display("@%d, MSG: %m set_i2s_s32", $time);
    set_gpio1_ports(0, 4'hf, 4'd3, 4'h4);
end
endtask
task set_port_i2s_s36;
begin
    $display("@%d, MSG: %m set_i2s_s36", $time);
    set_gpio1_ports(4, 4'hf, 4'd3, 4'h4);
end
endtask
task set_port_i2s_s40;
begin
    $display("@%d, MSG: %m set_i2s_s40", $time);
    set_gpio1_ports(8, 4'hf, 4'd3, 4'h4);
end
endtask
task set_port_i2s_s44;
begin
    $display("@%d, MSG: %m set_i2s_s44", $time);
    set_gpio1_ports(12, 4'hf, 4'd3, 4'h4);
end
endtask
#endif

/////////////////////////////////////////
void set_port_spim0_0(void)
{
    //$display("@%d, MSG: %m set_spim0_0", $time);
    set_ports(0, 0xf, 1, 7);
}

#if 0
task set_port_spim0_tri_0;
begin
    $display("@%d, MSG: %m set_spim0_tri_0", $time);
    set_ports(0, 4'h7, 4'd11, 4'd3);
end
endtask
task set_port_spim0_tri_12;
begin
    $display("@%d, MSG: %m set_spim0_tri_12", $time);
    set_ports(12, 4'h7, 4'd11, 4'd3);
end
endtask
task set_port_spim0_tri_24;
begin
    $display("@%d, MSG: %m set_spim0_tri_24", $time);
    set_ports(24, 4'h7, 4'd11, 4'd3);
end
endtask
task set_port_spim0_tri_32;
begin
    $display("@%d, MSG: %m set_spim0_tri_32", $time);
    set_gpio1_ports(0, 4'h7, 4'd11, 4'd3);
end
endtask
task set_port_spim0_tri_44;
begin
    $display("@%d, MSG: %m set_spim0_tri_44", $time);
    set_gpio1_ports(12, 4'h7, 4'd11, 4'd3);
end
endtask
task set_port_spim1_tri_4;
begin
    $display("@%d, MSG: %m set_spim0_tri_4", $time);
    set_ports(4, 4'h7, 4'd11, 4'd3);
end
endtask
task set_port_spim1_tri_16;
begin
    $display("@%d, MSG: %m set_spim0_tri_16", $time);
    set_ports(16, 4'h7, 4'd11, 4'd3);
end
endtask
task set_port_spim1_tri_28;
begin
    $display("@%d, MSG: %m set_spim0_tri_28", $time);
    set_ports(28, 4'h7, 4'd11, 4'd3);
end
endtask
task set_port_spim1_tri_36;
begin
    $display("@%d, MSG: %m set_spim0_tri_36", $time);
    set_gpio1_ports(4, 4'h7, 4'd11, 4'd3);
end
endtask
task set_port_spim2_tri_8;
begin
    $display("@%d, MSG: %m set_spim0_tri_8", $time);
    set_ports(8, 4'h7, 4'd11, 4'd3);
end
endtask
task set_port_spim2_tri_20;
begin
    $display("@%d, MSG: %m set_spim0_tri_20", $time);
    set_ports(20, 4'h7, 4'd11, 4'd3);
end
endtask
task set_port_spim2_tri_40;
begin
    $display("@%d, MSG: %m set_spim0_tri_40", $time);
    set_gpio1_ports(8, 4'h7, 4'd11, 4'd3);
end
endtask
#endif

void set_port_spim0_12(void)
{
    //$display("@%d, MSG: %m set_spim0_12", $time);
    set_ports(12, 0xf, 1, 7);
}

void set_port_spim0_24(void)
{
    //$display("@%d, MSG: %m set_spim0_24", $time);
    set_ports(24, 0xf, 1, 7);
}

#if 0
task set_port_spim0_32;
begin
    $display("@%d, MSG: %m set_spim0_32", $time);
    set_gpio1_ports(0, 4'hf, 4'd1, 4'd7);
end
endtask
task set_port_spim0_44;
begin
    $display("@%d, MSG: %m set_spim0_44", $time);
    set_gpio1_ports(12, 4'hf, 4'd1, 4'd7);
end
endtask
#endif

void set_port_spim1_4(void)
{
    //$display("@%d, MSG: %m set_spim1_4", $time);
    set_ports(4, 0xf, 1, 7);
}

#if 0
task set_port_spim1_36;
begin
    $display("@%d, MSG: %m set_spim1_36", $time);
    set_gpio1_ports(4, 4'hf, 4'd1, 4'd7);
end
endtask
#endif

void set_port_spim1_16(void)
{
    //$display("@%d, MSG: %m set_spim1_16", $time);
    set_ports(16, 0xf, 1, 7);
}

void set_port_spim1_28(void)
{
    //$display("@%d, MSG: %m set_spim1_28", $time);
    set_ports(28, 0xf, 1, 7);
}

void set_port_spim2_8(void)
{
    //$display("@%d, MSG: %m set_spim2_8", $time);
    set_ports(8, 0xf, 1, 7);
}

void set_port_spim2_20(void)
{
    //$display("@%d, MSG: %m set_spim2_20", $time);
    set_ports(20, 0xf, 1, 7);
}

#if 0
task set_port_spim2_40;
begin
    $display("@%d, MSG: %m set_spim2_40", $time);
    set_gpio1_ports(8, 4'hf, 4'd1, 4'd7);
end
endtask
#endif

/////////////////////////////////////////
void set_port_dmic0(void)
{
    //$display("@%d, MSG: %m set_port_dmic0", $time);
    set_ports(0, 0x3, 5, 1);
}

#if 0
task set_port_dmic2;
begin
    $display("@%d, MSG: %m set_port_dmic2", $time);
    set_ports(0, 4'hc, 4'd6, 4'd4);
end
endtask
task set_port_dmic4;
begin
    $display("@%d, MSG: %m set_port_dmic4", $time);
    set_ports(4, 4'h3, 4'd5, 4'd1);
end
endtask
task set_port_dmic6;
begin
    $display("@%d, MSG: %m set_port_dmic6", $time);
    set_ports(4, 4'hc, 4'd6, 4'd4);
end
endtask
task set_port_dmic8;
begin
    $display("@%d, MSG: %m set_port_dmic8", $time);
    set_ports(8, 4'h3, 4'd5, 4'd1);
end
endtask
task set_port_dmic10;
begin
    $display("@%d, MSG: %m set_port_dmic10", $time);
    set_ports(8, 4'hc, 4'd6, 4'd4);
end
endtask
task set_port_dmic12;
begin
    $display("@%d, MSG: %m set_port_dmic12", $time);
    set_ports(12, 4'h3, 4'd5, 4'd1);
end
endtask
task set_port_dmic14;
begin
    $display("@%d, MSG: %m set_port_dmic14", $time);
    set_ports(12, 4'hc, 4'd6, 4'd4);
end
endtask
task set_port_dmic16;
begin
    $display("@%d, MSG: %m set_port_dmic16", $time);
    set_ports(16, 4'h3, 4'd5, 4'd1);
end
endtask
task set_port_dmic18;
begin
    $display("@%d, MSG: %m set_port_dmic18", $time);
    set_ports(16, 4'hc, 4'd6, 4'd4);
end
endtask
task set_port_dmic20;
begin
    $display("@%d, MSG: %m set_port_dmic20", $time);
    set_ports(20, 4'h3, 4'd5, 4'd1);
end
endtask
task set_port_dmic22;
begin
    $display("@%d, MSG: %m set_port_dmic22", $time);
    set_ports(20, 4'hc, 4'd6, 4'd4);
end
endtask
task set_port_dmic24;
begin
    $display("@%d, MSG: %m set_port_dmic24", $time);
    set_ports(24, 4'h3, 4'd5, 4'd1);
end
endtask
task set_port_dmic26;
begin
    $display("@%d, MSG: %m set_port_dmic26", $time);
    set_ports(24, 4'hc, 4'd6, 4'd4);
end
endtask
task set_port_dmic28;
begin
    $display("@%d, MSG: %m set_port_dmic28", $time);
    set_ports(28, 4'h3, 4'd5, 4'd1);
end
endtask
task set_port_dmic30;
begin
    $display("@%d, MSG: %m set_port_dmic30", $time);
    set_ports(28, 4'hc, 4'd6, 4'd4);
end
endtask
task set_port_dmic32;
begin
    $display("@%d, MSG: %m set_port_dmic32", $time);
    set_gpio1_ports(0, 4'h3, 4'd5, 4'd1);
end
endtask
task set_port_dmic34;
begin
    $display("@%d, MSG: %m set_port_dmic34", $time);
    set_gpio1_ports(0, 4'hc, 4'd6, 4'd4);
end
endtask
task set_port_dmic36;
begin
    $display("@%d, MSG: %m set_port_dmic36", $time);
    set_gpio1_ports(4, 4'h3, 4'd5, 4'd1);
end
endtask
task set_port_dmic38;
begin
    $display("@%d, MSG: %m set_port_dmic38", $time);
    set_gpio1_ports(4, 4'hc, 4'd6, 4'd4);
end
endtask
task set_port_dmic40;
begin
    $display("@%d, MSG: %m set_port_dmic40", $time);
    set_gpio1_ports(8, 4'h3, 4'd5, 4'd1);
end
endtask
task set_port_dmic42;
begin
    $display("@%d, MSG: %m set_port_dmic42", $time);
    set_gpio1_ports(8, 4'hc, 4'd6, 4'd4);
end
endtask
task set_port_dmic44;
begin
    $display("@%d, MSG: %m set_port_dmic44", $time);
    set_gpio1_ports(12, 4'h3, 4'd5, 4'd1);
end
endtask
task set_port_dmic46;
begin
    $display("@%d, MSG: %m set_port_dmic46", $time);
    set_gpio1_ports(12, 4'hc, 4'd6, 4'd4);
end
endtask
#endif

void set_port_pwm0_0(void)
{
    //$display("@%d, MSG: %m set_port_pwm0_0", $time);
    set_ports(0, 0x1, 9, 1);
}

void set_port_pwm0_2(void)
{
    //$display("@%d, MSG: %m set_port_pwm0_0", $time);
    set_ports(0, 0x4, 10, 4);
}

void set_port_pwm0_32(void)
{
    //$display("@%d, MSG: %m set_port_pwm0_32", $time);
    set_gpio1_ports(0, 0x1, 9, 1);
}

void set_port_pwm0_34(void)
{
    //$display("@%d, MSG: %m set_port_pwm0_34", $time);
    set_gpio1_ports(0, 0x4, 10, 4);
}

#if 0
task set_port_pwm0_44;
begin
    $display("@%d, MSG: %m set_port_pwm0_44", $time);
    set_gpio1_ports(12, 4'h1, 4'd9, 4'd1);
end
endtask
task set_port_pwm0_46;
begin
    $display("@%d, MSG: %m set_port_pwm0_46", $time);
    set_gpio1_ports(12, 4'h4, 4'd10, 4'd4);
end
endtask
#endif

void set_port_pwm1_1(void)
{
    //$display("@%d, MSG: %m set_port_pwm1_1", $time);
    set_ports(0, 0x2, 9, 2);
}

#if 0
task set_port_pwm1_2;
begin
    $display("@%d, MSG: %m set_port_pwm1_2", $time);
    set_ports(0, 4'h2, 4'd9, 4'd2);
end
endtask
#endif

void set_port_pwm1_3(void)
{
    //$display("@%d, MSG: %m set_port_pwm1_3", $time);
    set_ports(0, 0x8, 10, 8);
}

void set_port_pwm0_4(void)
{
    //$display("@%d, MSG: %m set_port_pwm0_4", $time);
    set_ports(4, 0x1, 9, 1);
}

void set_port_pwm0_36(void)
{
    //$display("@%d, MSG: %m set_port_pwm0_36", $time);
    set_gpio1_ports(4, 0x1, 9, 1);
}

void set_port_pwm0_38(void)
{
    //$display("@%d, MSG: %m set_port_pwm0_38", $time);
    set_gpio1_ports(4, 0x4, 10, 4);
}

void set_port_pwm1_5(void)
{
    //$display("@%d, MSG: %m set_port_pwm1_5", $time);
    set_ports(4, 0x2, 9, 2);
}

void set_port_pwm0_6(void)
{
    //$display("@%d, MSG: %m set_port_pwm0_6", $time);
    set_ports(4, 0x4, 10, 4);
}

void set_port_pwm1_7(void)
{
    //$display("@%d, MSG: %m set_port_pwm1_7", $time);
    set_ports(4, 0x8, 10, 8);
}

void set_port_pwm0_8(void)
{
    //$display("@%d, MSG: %m set_port_pwm0_8", $time);
    set_ports(8, 0x1, 9, 1);
}

void set_port_pwm0_40(void)
{
    //$display("@%d, MSG: %m set_port_pwm0_40", $time);
    set_gpio1_ports(8, 0x1, 9, 1);
}

void set_port_pwm0_42(void)
{
    //$display("@%d, MSG: %m set_port_pwm0_42", $time);
    set_gpio1_ports(8, 0x4, 10, 4);
}

void set_port_pwm1_9(void)
{
    //$display("@%d, MSG: %m set_port_pwm1_9", $time);
    set_ports(8, 0x2, 9, 2);
}

void set_port_pwm1_11(void)
{
    //$display("@%d, MSG: %m set_port_pwm1_11", $time);
    set_ports(8, 0x8, 10, 8);
}

void set_port_pwm0_12(void)
{
    //$display("@%d, MSG: %m set_port_pwm0_12", $time);
    set_ports(12, 0x1, 9, 1);
}

void set_port_pwm1_13(void)
{
    //$display("@%d, MSG: %m set_port_pwm1_13", $time);
    set_ports(12, 0x2, 9, 2);
}

void set_port_pwm0_16(void)
{
    //$display("@%d, MSG: %m set_port_pwm0_16", $time);
    set_ports(16, 0x1, 9, 1);
}

void set_port_pwm1_17(void)
{
    //$display("@%d, MSG: %m set_port_pwm1_17", $time);
    set_ports(16, 0x2, 9, 2);
}

void set_port_pwm0_20(void)
{
    //$display("@%d, MSG: %m set_port_pwm0_20", $time);
    set_ports(20, 0x1, 9, 1);
}

void set_port_pwm1_21(void)
{
    //$display("@%d, MSG: %m set_port_pwm1_21", $time);
    set_ports(20, 0x2, 9, 2);
}

void set_port_pwm0_24(void)
{
    //$display("@%d, MSG: %m set_port_pwm0_24", $time);
    set_ports(24, 0x1, 9, 1);
}

void set_port_pwm1_25(void)
{
    //$display("@%d, MSG: %m set_port_pwm1_25", $time);
    set_ports(24, 0x2, 9, 2);
}

void set_port_pwm0_28(void)
{
    //$display("@%d, MSG: %m set_port_pwm0_28", $time);
    set_ports(28, 0x1, 9, 1);
}

void set_port_pwm1_29(void)
{
    //$display("@%d, MSG: %m set_port_pwm1_29", $time);
    set_ports(28, 0x2, 9, 2);
}

void set_port_pwm1_31(void)
{
    //$display("@%d, MSG: %m set_port_pwm1_31", $time);
    set_ports(28, 0x8, 10, 8);
}

void set_port_pwm1_33(void)
{
    //$display("@%d, MSG: %m set_port_pwm1_33", $time);
    set_gpio1_ports(0, 0x2, 9, 2);
}

void set_port_pwm1_35(void)
{
    //$display("@%d, MSG: %m set_port_pwm1_35", $time);
    set_gpio1_ports(0, 0x8, 10, 8);
}

void set_port_pwm1_37(void)
{
    //$display("@%d, MSG: %m set_port_pwm1_37", $time);
    set_gpio1_ports(4, 0x2, 9, 2);
}

void set_port_pwm1_39(void)
{
    //$display("@%d, MSG: %m set_port_pwm1_39", $time);
    set_gpio1_ports(4, 0x8, 10, 8);
}

void set_port_pwm1_41(void)
{
    //$display("@%d, MSG: %m set_port_pwm1_41", $time);
    set_gpio1_ports(8, 0x2, 9, 2);
}

void set_port_pwm1_43(void)
{
    //$display("@%d, MSG: %m set_port_pwm1_43", $time);
    set_gpio1_ports(8, 0x8, 10, 8);
}

void set_port_pwm1_45(void)
{
    //$display("@%d, MSG: %m set_port_pwm1_45", $time);
    set_gpio1_ports(12, 0x2, 9, 2);
}

void set_port_pwm1_47(void)
{
    //$display("@%d, MSG: %m set_port_pwm1_47", $time);
    set_gpio1_ports(12, 0x8, 10, 8);
}

/////////////////////////////////////////
#if 0
task set_port_i2c0_0;
begin
    $display("@%d, MSG: %m set_port_i2c0_0", $time);
    set_ports_pu(0, 4'h3);
    set_ports(0, 4'h3, 4'd7, 4'd0);
end
endtask
task set_port_i2c0_2;
begin
    $display("@%d, MSG: %m set_port_i2c0_2", $time);
    set_ports_pu(0, 4'hc);
    set_ports(0, 4'hc, 4'd8, 4'd0);
end
endtask
#endif

void set_port_i2c0_12(void)
{
    //$display("@%d, MSG: %m set_port_i2c0_12", $time);
    set_ports_pu(12, 0x3);
    set_ports(12, 0x3, 7, 0);
}

#if 0
task set_port_i2c0_14;
begin
    $display("@%d, MSG: %m set_port_i2c0_14", $time);
    set_ports_pu(12, 4'hc);
    set_ports(12, 4'hc, 4'd8, 4'h0);
end
endtask
task set_port_i2c0_24;
begin
    $display("@%d, MSG: %m set_port_i2c0_24", $time);
    set_ports_pu(24, 4'h3);
    set_ports(24, 4'h3, 4'd7, 4'd0);
end
endtask
task set_port_i2c0_26;
begin
    $display("@%d, MSG: %m set_port_i2c0_26", $time);
    set_ports_pu(24, 4'hc);
    set_ports(24, 4'hc, 4'd8, 4'h0);
end
endtask
task set_port_i2c0_32;
begin
    $display("@%d, MSG: %m set_port_i2c0_32", $time);
    set_ports_pu(32, 4'h3);
    set_ports(32, 4'h3, 4'd7, 4'd0);
end
endtask
task set_port_i2c0_34;
begin
    $display("@%d, MSG: %m set_port_i2c0_34", $time);
    set_ports_pu(32, 4'hc);
    set_ports(32, 4'hc, 4'd8, 4'd0);
end
endtask
task set_port_i2c0_40;
begin
    $display("@%d, MSG: %m set_port_i2c0_40", $time);
    set_ports_pu(40, 4'h3);
    set_ports(40, 4'h3, 4'd7, 4'd0);
end
endtask
task set_port_i2c0_44;
begin
    $display("@%d, MSG: %m set_port_i2c0_44", $time);
    set_ports_pu(44, 4'h3);
    set_ports(44, 4'h3, 4'd7, 4'd0);
end
endtask
task set_port_i2c0_46;
begin
    $display("@%d, MSG: %m set_port_i2c0_46", $time);
    set_ports_pu(44, 4'hc);
    set_ports(44, 4'hc, 4'd8, 4'd0);
end
endtask
#endif

void set_port_i2c1_4(void)
{
    //$display("@%d, MSG: %m set_port_i2c1_4", $time);
    set_ports_pu(4, 0x3);
    set_ports(4, 0x3, 7, 0);
}

#if 0
task set_port_i2c1_6;
begin
    $display("@%d, MSG: %m set_port_i2c1_6", $time);
    set_ports_pu(4, 4'hc);
    set_ports(4, 4'hc, 4'd8, 4'h0);
end
endtask
task set_port_i2c1_16;
begin
    $display("@%d, MSG: %m set_port_i2c1_16", $time);
    set_ports_pu(16, 4'h3);
    set_ports(16, 4'h3, 4'd7, 4'd0);
end
endtask
task set_port_i2c1_18;
begin
    $display("@%d, MSG: %m set_port_i2c1_18", $time);
    set_ports_pu(16, 4'hc);
    set_ports(16, 4'hc, 4'd8, 4'h0);
end
endtask
task set_port_i2c1_28;
begin
    $display("@%d, MSG: %m set_port_i2c1_28", $time);
    set_ports_pu(28, 4'h3);
    set_ports(28, 4'h3, 4'd7, 4'd0);
end
endtask
task set_port_i2c1_30;
begin
    $display("@%d, MSG: %m set_port_i2c1_30", $time);
    set_ports_pu(28, 4'hc);
    set_ports(28, 4'hc, 4'd8, 4'h0);
end
endtask

task set_port_i2c0_8;
begin
    $display("@%d, MSG: %m set_port_i2c0_8", $time);
    set_ports_pu(8, 4'h3);
    set_ports(8, 4'h3, 4'd7, 4'd0);
end
endtask
task set_port_i2c0_20;
begin
    $display("@%d, MSG: %m set_port_i2c0_20", $time);
    set_ports_pu(20, 4'h3);
    set_ports(20, 4'h3, 4'd7, 4'd0);
end
endtask

task set_port_i2c1_10;
begin
    $display("@%d, MSG: %m set_port_i2c1_10", $time);
    set_ports_pu(8, 4'hc);
    set_ports(8, 4'hc, 4'd8, 4'h0);
end
endtask
task set_port_i2c1_22;
begin
    $display("@%d, MSG: %m set_port_i2c1_22", $time);
    set_ports_pu(20, 4'hc);
    set_ports(20, 4'hc, 4'd8, 4'h0);
end
endtask
task set_port_i2c1_36;
begin
    $display("@%d, MSG: %m set_port_i2c1_36", $time);
    set_ports_pu(36, 4'h3);
    set_ports(36, 4'h3, 4'd7, 4'd0);
end
endtask
task set_port_i2c1_38;
begin
    $display("@%d, MSG: %m set_port_i2c1_38", $time);
    set_ports_pu(36, 4'hc);
    set_ports(36, 4'hc, 4'd8, 4'h0);
end
endtask
task set_port_i2c1_42;
begin
    $display("@%d, MSG: %m set_port_i2c1_42", $time);
    set_ports_pu(40, 4'hc);
    set_ports(40, 4'hc, 4'd8, 4'h0);
end
endtask

//////////////////////////////////////////
task set_uart_baudrate;
input           id;
input [31:0]    pclk_freq;
input [31:0]   baud_rate;

reg [10:0]	N;
reg [4:0]	P;
reg [3:0]	O;
reg [1:0]	N_1;
begin
    // baud rate = pclk / (N * P * O), N=UNDIV[1:0:0] + 1
    get_baud_vars(pclk_freq, baud_rate, N, P, O);
    $display("@%0d, MSG: %m set_uart_baudrate, id=%d, pclk=%d, baud_rate=%d, N=%d, P=%d, O=%d", $time, id, pclk_freq, baud_rate, N, P, O);
    N_1= N-1;
    if (id==0) begin
       `CHIP_CPU_BFM.ahbwr (`UART0_UnBAUD_REG, 0,2, {24'd0,N_1[7:0]});
       `CHIP_CPU_BFM.ahbwr (`UART0_UnPSR_REG,  0,2, {24'd0, P[4:0], N_1[10:8]});
       `CHIP_CPU_BFM.ahbwr (`UART0_UnOVR_REG,  0,2, {24'd0, 4'd0, O[3:0]});
    end
    else if (id==1) begin
       `CHIP_CPU_BFM.ahbwr (`UART1_UnBAUD_REG, 0,2, {24'd0,N_1[7:0]});
       `CHIP_CPU_BFM.ahbwr (`UART1_UnPSR_REG,  0,2, {24'd0, P[4:0], N_1[10:8]});
       `CHIP_CPU_BFM.ahbwr (`UART1_UnOVR_REG,  0,2, {24'd0, 4'd0, O[3:0]});
    end
    else begin
       `CHIP_CPU_BFM.ahbwr (`UART2_UnBAUD_REG, 0,2, {24'd0,N_1[7:0]});
       `CHIP_CPU_BFM.ahbwr (`UART2_UnPSR_REG,  0,2, {24'd0, P[4:0], N_1[10:8]});
       `CHIP_CPU_BFM.ahbwr (`UART2_UnOVR_REG,  0,2, {24'd0, 4'd0, O[3:0]});
    end

end
endtask

task get_baud_vars;
input [31:0]    pclk;
input [31:0]    br;
output [10:0]      N;
output [4:0]       P;
output [3:0]       O;

begin
    $display("@%0d, MSG: %m get_baud_vars, pclk=%d, baud_rate=%d, N=%d, P=%d, O=%d", $time, pclk, br, N, P, O);
    case(pclk)
    48 : case(br)
	 300 : begin O=16; N=2000; P=9; end
	 9600 : begin O=16; N=125; P=4; end
         14400 : begin O=11; N=202; P=2; end
	 1536000 : begin O=9; N=1; P=6; end
         default: begin O=16; N=125; P=4; end // 9600
         endcase
    24 : case(br)
         300 : begin O=16; N=2000; P=4; end
         9600 : begin O=10; N=250; P=1; end
         14400 : begin O=11; N=101; P=2; end
         1536000 : begin O=8; N=2; P=1; end
         default: begin O=16; N=125; P=4; end // 9600
         endcase
    default : begin O=16; N=125; P=4; end
    endcase

end
endtask
/////////////////////////////////////////
task i2c_rd;
input [7:0] ms_id;
input [7:0] i2c_slv0_id;
input [11:0] ms_prescaler;
input [5:0]  ms_i2c_mask;
input [16:0] dma_str_raddr_t;
input [16:0] dma_str_waddr_t;
input [11:0] wdata_byte_num_t;
input [11:0] rdata_byte_num_t;
input [1:0] port_op_mode_t;
input [0:0] dma_enable_t;
input [0:0] dbus_burst_t;
reg [6:0] ahb_burst;
reg [31:0] rdata;
reg [1:0]      i2c_slv0_core_select = 2'b10;
reg [5:0]      i2c_slv0_mask = 6'd4;
reg            ms_addr_16bit = 1'b0;
reg            ms_address_en = 1'b0;
reg            ms_no_stop = 1'b0;
reg [6:0]      ms_slave_addr = i2c_slv0_id;
reg [15:0]     ms_word_addr=16'd0;
reg [11:0]	ofs;
reg		intr;
begin
    $display("@%d, MSG: I2C(%0d) READ -> DBUS_WRITE", $time, ms_id);
    ofs = ms_id==0 ? 12'h0 : 12'h100;
    `CHIP_CPU_BFM.ahbwr( `I2C0_ADDR_BASE+ofs+32'h0, 3'd0, 4'd2, 32'h0000_0007); // intr enable
    `CHIP_CPU_BFM.ahbrd( `I2C0_ADDR_BASE+ofs+32'h0, 3'd0, 4'd2, rdata, 1, 32'h0000_0007, 1'b0); // intr enable
    `CHIP_CPU_BFM.ahbwr( `I2C0_ADDR_BASE+ofs+32'h14, 3'd0, 4'd2, {4'd0,ms_prescaler[11:0],8'd0,ms_i2c_mask,2'd1});
    `CHIP_CPU_BFM.ahbrd( `I2C0_ADDR_BASE+ofs+32'h14, 3'd0, 4'd2, rdata, 1, {4'd0,ms_prescaler[11:0],8'd0,ms_i2c_mask,2'd1}, 1'b0); // intr enable
    `CHIP_CPU_BFM.ahbwr( `I2C0_ADDR_BASE+ofs+32'h18, 3'd0, 4'd2, {ms_word_addr,1'b0,i2c_slv0_id,5'd0,ms_no_stop,ms_address_en,ms_addr_16bit});
    `CHIP_CPU_BFM.ahbrd( `I2C0_ADDR_BASE+ofs+32'h18, 3'd0, 4'd2, rdata, 1, {ms_word_addr,1'b0,i2c_slv0_id,5'd0,ms_no_stop,ms_address_en,ms_addr_16bit},1'b0);

    `CHIP_CPU_BFM.ahbwr(`I2C0_ADDR_BASE+ofs+32'hc, 3'd0, 4'd2, {15'd0,dma_str_waddr_t[16:0]});
    `CHIP_CPU_BFM.ahbwr(`I2C0_ADDR_BASE+ofs+32'h10, 3'd0, 4'd2, {15'd0,dma_str_raddr_t[16:0]});
    `CHIP_CPU_BFM.ahbwr(`I2C0_ADDR_BASE+ofs+32'h8, 3'd0, 4'd2, {dma_enable_t,1'd0,port_op_mode_t,rdata_byte_num_t[11:0],dbus_burst_t,3'd0,wdata_byte_num_t[11:0]}); // intr enable
    wait ( (ms_id==0? `I2C0_INTR : `I2C1_INTR) );
    `CHIP_CPU_BFM.ahbrd(`I2C0_ADDR_BASE+ofs+32'h0, 3'd0, 4'd2, rdata, 1, 32'h0001_0007, 1'b0);
    `CHIP_CPU_BFM.ahbwr(`I2C0_ADDR_BASE+ofs+32'h0, 3'd0, 4'd2, 32'h0001_0007);
    `CHIP_CPU_BFM.set_rd_file("ahb_rdata.hex");
    ahb_burst = (wdata_byte_num_t+1)/4;
    `CHIP_CPU_BFM.ahbrd_incr(`SYSRAM_ADDR_BASE+dma_str_waddr_t, 1, ahb_burst, 2, rdata, 1, 32'h0, 0);
    `CHIP_CPU_BFM.unset_rd_file;
end
endtask
/////////////////////////////////////////
task chip_por;
begin
    $display("@%d, MSG: chip_por", $time);
    force `CHIP_RSTN=1'b1;
    repeat (3) @(posedge `CHIP_CLK32K);
    release `CHIP_RSTN;
    test_init;
    wait(`TB_INIT_DONE);

end
endtask
#endif
/////////////////////////////////////////

void ef_dma(uint32_t addr, uint32_t dma_str_raddr, uint32_t dma_str_waddr, uint32_t wdata_byte_num, uint32_t rdata_byte_num, uint32_t port_op_mode, uint32_t dma_enable, uint32_t dbus_burst) {
//input [19:0] addr;
//input [17:0] dma_str_raddr;
//input [17:0] dma_str_waddr;
//input [10:0] wdata_byte_num;
//input [10:0] rdata_byte_num;
//input [1:0] port_op_mode;
//input [0:0] dma_enable;
//input [0:0] dbus_burst;
uint32_t ahb_burst;            //7bi5
uint32_t rdata;               //32bit
uint32_t wdata;               //32bit
//reg [2:0]  int_en;
//reg 	   dma_mode;
uint32_t dw_num;              //10bit
uint32_t data_erase_mode;     //4bit
uint32_t data_erase_enable;   //1bit
uint32_t rw_mode;             //1bit
uint32_t dual_bank;           //1bit
uint32_t redu;                //1bit

    //$display("@%d, MSG: ef_dma. port_op_mode=%0d, dma_str_raddr=%0x, rdata_byte_num=%0d, dma_str_waddr=%0x, wdata_byte_num=%0d", $time, port_op_mode, dma_str_raddr, dma_str_waddr, wdata_byte_num, rdata_byte_num);

    // setup ef registers
    dw_num=((wdata_byte_num>>2)&0x1FF);
    data_erase_mode= 0;
    data_erase_enable=1;
    rw_mode=0;
    //dma_mode = 1;
    dual_bank=0;
    redu=0;

    wdata = (0 | dw_num<<16 | 0 | redu<<8 | 0 | dual_bank);
    //`CHIP_CPU_BFM.ahbwr(`EF_CONFIG_REG, 3'd0, 4'd2, wdata);
    wr(EF_CONFIG_REG, wdata);
    //`CHIP_CPU_BFM.ahbrd(`EF_CONFIG_REG, 3'd0, 4'd2, rdata,1,wdata,1'b0);
    rd_compare(EF_CONFIG_REG, rdata, wdata);

    wdata = (0 | addr <<8 | 0);
    //`CHIP_CPU_BFM.ahbwr(`EF_ACCESS_REG, 3'd0, 4'd2, wdata);
    wr(EF_ACCESS_REG, wdata);
    //`CHIP_CPU_BFM.ahbrd(`EF_ACCESS_REG, 3'd0, 4'd2, rdata,1,wdata,1'b0);
    rd_compare(EF_ACCESS_REG, rdata, wdata);

    ef_setmode_dma( data_erase_mode, data_erase_enable, rw_mode);

    // setup dma register
    //`CHIP_CPU_BFM.ahbwr(`EF_DMA_WADDR_REG, 3'd0, 4'd2, {14'd0,dma_str_waddr[17:0]});
    wr(EF_DMA_WADDR_REG, (0 | dma_str_waddr));
    //`CHIP_CPU_BFM.ahbwr(`EF_DMA_RADDR_REG, 3'd0, 4'd2, {14'd0,dma_str_raddr[17:0]});
    wr(EF_DMA_RADDR_REG, (0 | dma_str_raddr));
    //`CHIP_CPU_BFM.ahbwr(`EF_DMA_CTRL_REG, 3'd0, 4'd2, {dma_enable,1'd0,port_op_mode,1'b0,rdata_byte_num[10:0],dbus_burst,4'd0,wdata_byte_num[10:0]}); // intr enable
    wr(EF_DMA_CTRL_REG, (dma_enable<<31 | 0 | port_op_mode<<28 | 0 | rdata_byte_num<<16 | dbus_burst | 0 | wdata_byte_num));
    //wait (`EF_INTR);
    while(!EFLASH_INTR);
    EFLASH_INTR = 0;
    //`CHIP_CPU_BFM.ahbrd(`EF_INTERRUPT_REG, 3'd0, 4'd2, rdata, 1, 32'h0005_0003, 1'b0);
    //`CHIP_CPU_BFM.ahbwr(`EF_INTERRUPT_REG, 3'd0, 4'd2, 32'h0005_0003);
    if (port_op_mode==1) {
        int j = 0;
        //`CHIP_CPU_BFM.set_rd_file("ahb_rdata.hex");
        ahb_burst = (wdata_byte_num+1)/4;
        //`CHIP_CPU_BFM.ahbrd_incr(`SYSRAM_ADDR_BASE+dma_str_waddr, 1, ahb_burst, 2, rdata, 1, 32'h0, 0);
        for (j=0; j<ahb_burst; j++) {
            rd(SYSRAM_TEST_ADDR_BASE+dma_str_waddr+j*4, rdata);
        }
        //`CHIP_CPU_BFM.unset_rd_file;
    }
}
