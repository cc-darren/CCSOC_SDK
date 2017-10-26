/******************************************************************************
*  Copyright 2017, CloudChip, Inc.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of CloudChip, Inc. (C) 2017
******************************************************************************/

#ifndef _EFLASH_H_
#define _EFLASH_H_

#include "global.h"
#include "cc6801_reg.h"

typedef void (*fpEflash_Callback)(uint32_t sys_evt);

//#define SCU_ICACHE_REG              (SCU_ADDR_BASE + 0x00000034)
    #define FLUSH_EN    0x00000001

// EF regiser definition
//#define EF_INTERRUPT_REG            (EF_ADDR_BASE + 0x00000000)
    #define EF_INTERRUPT_REG_DMAModeStatus      0x00040000  //18
    #define EF_INTERRUPT_REG_EraPrgModeStatus   0x00020000  //17
    #define EF_INTERRUPT_REG_DataModeStatus     0x00010000  //16
    #define EF_INTERRUPT_REG_EraPrgModeEn       0x00000002  //1
    #define EF_INTERRUPT_REG_DataModeEn         0x00000001  //0
//#define EF_CONFIG_REG               (EF_ADDR_BASE + 0x00000004)
//#define EF_ACCESS_REG               (EF_ADDR_BASE + 0x00000008)
//#define EF_WR_DATA0_REG             (EF_ADDR_BASE + 0x0000000c)
//#define EF_WR_DATA1_REG             (EF_ADDR_BASE + 0x00000010)
//#define EF_WR_DATA2_REG             (EF_ADDR_BASE + 0x00000014)
//#define EF_WR_DATA3_REG             (EF_ADDR_BASE + 0x00000018)
//#define EF_FLASHMODE_REG            (EF_ADDR_BASE + 0x0000001c)
    #define EF_FLASHMODE_REG_AHBEnable          0x00000100  //8
    #define EF_FLASHMODE_REG_DMAChannel         0x00000020  //5
    #define EF_FLASHMODE_REG_AccessRead         0x00000010  //4
    #define EF_FLASHMODE_REG_ModeMassErase      0x0000000C  //3~0
    #define EF_FLASHMODE_REG_ModeMainErase      0x00000004  //3~0
    #define EF_FLASHMODE_REG_ModeMain           0x00000000  //3~0
//#define EF_RD_DATA0_REG             (EF_ADDR_BASE + 0x00000020)
//#define EF_RD_DATA1_REG             (EF_ADDR_BASE + 0x00000024)
//#define EF_REDUN_DATA_REG           (EF_ADDR_BASE + 0x00000028)

//#define EF_TIMING0_REG              (EF_ADDR_BASE + 0x00000044)
//#define EF_TIMING1_REG              (EF_ADDR_BASE + 0x00000048)
//#define EF_TIMING2_REG              (EF_ADDR_BASE + 0x0000004c)
//#define EF_TIMING3_REG              (EF_ADDR_BASE + 0x00000050)
//#define EF_TIMING4_REG              (EF_ADDR_BASE + 0x00000054)
//#define EF_TIMING5_REG              (EF_ADDR_BASE + 0x00000058)
//#define EF_TIMING6_REG              (EF_ADDR_BASE + 0x0000005c)

//#define EF_DMA_CTRL_REG             (EF_ADDR_BASE + 0x00000060)
    #define EF_DMA_CTRL_REG_DMAEnable       0x80000000  //31
    #define EF_DMA_CTRL_REG_OPModeWrite     0x00000000  //29~28
    #define EF_DMA_CTRL_REG_DMABurst        0x00008000  //15
//#define EF_DMA_WADDR_REG            (EF_ADDR_BASE + 0x00000064) //RAM addr to write flash
//#define EF_DMA_RADDR_REG            (EF_ADDR_BASE + 0x00000068) //RAM addr to read flash


void cc6801_EflashInit(void);
void cc6801_EflashFlush(void);
BOOL cc6801_EflashEraseALL(void);
BOOL cc6801_EflashErasePage(uint32_t adr);
void cc6801_EflashProgram(uint32_t dwEflashAdr,unsigned char * pBufAdr,uint32_t dwBufSize);
void cc6801_EflashRegisterCallback(fpEflash_Callback fpCB);

#define EFLASH_DMAMODE 1



#endif
