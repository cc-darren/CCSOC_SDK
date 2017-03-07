#include "stdint.h"
#include "string.h"
#include "addr_map.h"

static uint32_t wdata;              //32bit
static uint32_t rdata;              //32bit
static uint32_t i;
//reg [3:0]	data_erase_mode;
////   data_erase_mode :
////   *    0000 : main access (R/W)
////   *    0001 : redundancy access (R/W)
////   *    0010 : twimcode access (R), repair page/status information
////   *    0011 : information access (R/W)
////   *    0100 : main page erase
////   *    0101 : redundancy page erase
////   *    0110 : NA
////   *    0111 : information page erase
////   *    1100 : mass erase
////
//reg		data_erase_enable;
//reg		rw_mode;
//reg		dump_event;

static uint32_t ef_addr;            //20bit
static uint32_t dma_str_raddr;      //17bit
static uint32_t dma_str_waddr;      //17bit
static uint32_t wdata_byte_num;
static uint32_t rdata_byte_num;     //12bit
static uint32_t port_op_mode;       //2bit
static uint32_t dma_enable;         //1bit
static uint32_t dbus_burst;         //1bit

//static uint32_t wdata_base;     //32bit
#define		PRELOAD_FLASH

extern uint32_t __WDTRST_entry;
extern void set_port_pwm0_0(void);
extern void ef_setmode(uint32_t data_erase_mode, uint32_t data_erase_enable, uint32_t rw_mode);
extern void ef_dma(uint32_t addr, uint32_t dma_str_raddr, uint32_t dma_str_waddr, uint32_t wdata_byte_num, uint32_t rdata_byte_num, uint32_t port_op_mode, uint32_t dma_enable, uint32_t dbus_burst);
extern int rd_compare(uint32_t addr, uint32_t val, uint32_t compare_val);

//0: success; others: fail
int test_eflash_dma_wr(void)
{
    void (*eFlashEntry)(void);
    
#ifndef FPGA
    //---- ef page erase
    //$display("@%d, MSG: EF page erase", $time);
    ef_setmode(4,1,0);

    //dump_event=1;
#endif

    //---- system ram setup
    //wdata_base = 0xffff0000;
    //for (i=0; i<24576; i=i+1)
    //{
    //    wr(SYSRAM_TEST_ADDR_BASE+i*4, wdata_base + i);
    //}

    //---- ef page dma wr
    dma_str_raddr = ((uint32_t)&__WDTRST_entry & ~0x01);
    //dma_str_raddr = 0x00001000;
    wdata_byte_num = 0x7ff;

    dma_str_waddr = 0x20001000;
    rdata_byte_num = 0x7ff;

    //move ROM to RAM first because eFlash DMA can only read data from RAM
    memcpy((void*)dma_str_waddr, (const void*)dma_str_raddr, wdata_byte_num);

    dma_str_raddr = 0x20001000;
    dma_str_waddr = 0x10000000;

    port_op_mode = 0x0; // write to port
    dma_enable=1;
    dbus_burst = 0x1;
    ef_addr=0;
    ef_dma(ef_addr,dma_str_raddr,dma_str_waddr, wdata_byte_num, rdata_byte_num, port_op_mode, dma_enable, dbus_burst);
    //memcpy((void*)dma_str_waddr, (const void*)dma_str_raddr, wdata_byte_num);

    //----- ef page read
    //0x800>>2 => 0x200 dword
    wr(GPIO_INTR_TYPE_REG, 0);
    wr(EF_FLASHMODE_REG, 0x10);
    for (i=0; i<0x800; i+=4)
    {
        rd(dma_str_raddr + i, rdata);
        rd_compare(dma_str_waddr+i, wdata, rdata);
    }

    //check error count
    rd(GPIO_INTR_TYPE_REG, rdata);
    while(rdata);   //if any errors, stop
    
    eFlashEntry = (void(*)(void))(0x100000D5);
    
    eFlashEntry();
    
    //repeat(100)@(posedge `CHIP_CLK48M);
    //$finish;

    return 0;
}
