#include "stdint.h"
#include "addr_map.h"

static uint32_t wdata;
static uint32_t rdata;
static uint32_t i;
static uint32_t data_erase_mode;    //4bit
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
static uint32_t data_erase_enable;
static uint32_t rw_mode;
static uint32_t dump_event;

#define		PRELOAD_FLASH

int test_eflash_rd(void)
{
    dump_event=0;
    //$display("@%t, %m MSG: ****** test name: test_eflash_rd", $time);
    //wait (`TB_INIT_DONE);
    //$display("@%t, %m MSG: TB_INIT_DONEROM", $time);

    rd(EF_BASE, rdata);

#if 0
    //---- ef main
    //$display("@%d, MSG: EF read main", $time);
    for (i=0; i<4096; i=i+1)
    {
        wdata = i*16;
        `CHIP_CPU_BFM.ahbrd_incr( `EF_BASE+(i*16*4), 1, 16, 2, rdata, 1, wdata, 0);
    }

    //---- ef page erase
    //$display("@%d, MSG: EF page erase", $time);
    ef_setmode(4,1,0);

    //---- ef page wr
    //$display("@%d, MSG: EF write main", $time);
    for (i=0; i<512; i=i+1)
    {
        ef_wr(i*4, 32'hffff_0000+i, 0);
    }

    ic_flush();
    dump_event=1'b1;
    for (i=0; i<32; i=i+1)
    {
        wdata = 32'hffff_0000 + i*16;
        `CHIP_CPU_BFM.ahbrd_incr( `EF_BASE+(i*16*4), 1, 16, 2, rdata, 1, wdata, 0);
    }

    //---- ef information page rd
    ic_flush();
    //$display("@%d, MSG: EF read info", $time);

    ef_setmode(3, 0, 0); // mode, en, rw
    for (i=0; i<32; i=i+1)
    {
        wdata = 32'h0001_0000 + i*16;
        `CHIP_CPU_BFM.ahbrd_incr( `EF_BASE+(i*16*4), 1, 16, 2, rdata, 1, wdata, 0);
    }

#endif

    return 0;

    //repeat(100)@(posedge `CHIP_CLK48M);
    //$finish;
}


