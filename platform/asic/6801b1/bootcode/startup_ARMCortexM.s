;/******************************************************************************
;*  Copyright 2018, CloudChip, Inc.
;*  ---------------------------------------------------------------------------
;*  Statement:
;*  ----------
;*  This software is protected by Copyright and the information contained
;*  herein is confidential. The software may not be copied and the information
;*  contained herein may not be used or disclosed except with the written
;*  permission of CloudChip, Inc. (C) 2018
;******************************************************************************/

#include "stackheapheader.h"

Heap_Size       EQU     SYS_HEAP_SIZE
                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit

Stack_Size      EQU     SYS_STACK_SIZE
                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp

SB_Size         EQU     0x50    ;don't change this value if you don't know sleep procedure
                AREA    SLEEP_BUFFER, NOINIT, READWRITE, ALIGN=3
__SB_base
SB_Mem          SPACE   SB_Size
    
                PRESERVE8
                THUMB

;                AREA    WDTBIN, CODE, READONLY
;                EXPORT  __WDTRST_entry
;__WDTRST_entry
;                INCBIN  ..\..\..\FPGA_M0p_wdtrst.bin    ;include WDT bin file


; Vector Table Mapped to Address 0 at Reset
                AREA    VECTOR_TABLE, DATA, READONLY

                EXPORT  __Vectors
                EXPORT  __Vectors_End
                EXPORT  __Vectors_Size

__Vectors       DCD     __initial_sp              ; Top of Stack
                DCD     Reset_Handler             ; Reset Handler
                DCD     NMI_Handler               ; NMI Handler
                DCD     HardFault_Handler         ; Hard Fault Handler
                DCD     MemManage_Handler         ; MPU Fault Handler		(valid only for M4)
                DCD     BusFault_Handler          ; Bus Fault Handler	 	(valid only for M4)
                DCD     UsageFault_Handler        ; Usage Fault Handler  	(valid only for M4)
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     SVC_Handler               ; SVCall Handler
                DCD     DebugMon_Handler          ; Debug Monitor Handler	(valid only for M4)
                DCD     0                         ; Reserved
                DCD     PendSV_Handler            ; PendSV Handler
                DCD     SysTick_Handler           ; SysTick Handler

                ; External Interrupts
                DCD     BLE_IRQHandler            ;
                DCD     WDT_IRQHandler            ;
                DCD     I2C0_M_IRQHandler         ;
                DCD     SPI3_M_IRQHandler         ;
                DCD     WKTM0_IRQHandler          ;
                DCD     WKTM1_IRQHandler          ;
                DCD     UART0_RXDMA_IRQHandler    ;
                DCD     UART1_RXDMA_IRQHandler    ;
                DCD     UART2_RXDMA_IRQHandler    ;
                DCD     SPI0_M_IRQHandler         ;
                DCD     SPI1_M_IRQHandler         ;
                DCD     SPI2_M_IRQHandler         ;
                DCD     I2C1_M_IRQHandler         ;
                DCD     GPIO_IRQHandler           ;
                DCD     UART0_TXDMA_IRQHandler    ;
                DCD     UART0_IP_IRQHandler       ;
                DCD     UART1_TXDMA_IRQHandler    ;
                DCD     UART1_IP_IRQHandler       ;
                DCD     UART2_TXDMA_IRQHandler    ;
                DCD     UART2_IP_IRQHandler       ;
                DCD     HS_IRQHandler             ;
                DCD     PWM0_IRQHandler           ;
                DCD     PWM1_IRQHandler           ;
                DCD     RTC_IRQHandler            ;
                DCD     VOICE_WKUP_IRQHandler     ;
                DCD     I2S_RXDMA_IRQHandler      ;
                DCD     I2S_TXDMA_IRQHandler      ;
                DCD     I2S_IP_IRQHandler         ;
                DCD     DMIC_IRQHandler           ;
                DCD     AHBBRG_IRQHandler         ;
                DCD     AES_IRQHandler            ;
                DCD     EFLASH_IRQHandler         ;
__Vectors_End

__Vectors_Size  EQU     __Vectors_End - __Vectors

;*********************
;   App image header *
;*********************
                AREA    IMAGE_HEADER, DATA, READONLY
                IMPORT  |Load$$LR$$SOC$$Length|

                DCB     IMG_ID                      ;4 bytes image name
                DCD     IMG_FW_VERSION              ;4 bytes version: 0xAABBBCCC, AA=major, BBB=minor, CCC=update
                DCD     |Load$$LR$$SOC$$Length|     ;4 bytes image size
                DCD     IMG_FLAG1                   ;4 bytes flag for future usage
                DCD     IMG_FLAG2                   ;4 bytes flag for future usage

;****************
;   OTA Block   *
;****************
                AREA    OTA_BLK_DATA, DATA, READONLY

                DCB     OTA_ID                      ;4 bytes OTA block ID
                DCD     OTA_FLAG                    ;4 bytes OTA flag
                DCD     OTA_START_ADDR              ;4 bytes start address
                DCD     OTA_FILE_SIZE               ;4 bytes Image size to be programmed

;****************
;   BLE Block   *
;****************
                AREA    BLE_BLK_DATA, DATA, READONLY

                DCB     BLE_ID                      ;4 bytes BLE block ID
                DCD     BLE_FLAG                    ;4 bytes flag for future usage
                DCD     BLE_RANDOM_KEY              ;4 bytes to store random key


;****************
;   Startup     *
;****************
                AREA    |.text|, CODE, READONLY
; Reset Handler
Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]
                IMPORT  sys_InitStartup
                IMPORT  __main

                LDR     R0, =__initial_sp
                MOV     SP, R0
                LDR     R0, =sys_InitStartup
                BLX     R0
                LDR     R0, =__main
                BX      R0

                ENDP
                    
;***************************************************************************************
; void Dslp_Handler(void)
;
; Purpose: We need CPU off in sleep mode to gain more power saving.
;          The penalty is FW must save Cortex all core registers before WFI instruction.
;          Also, FW must restore them from BootROM when CPU is back to ON.
;

Dslp_Handler    PROC {}                 ;"{}" means no arguments for this funciton
                EXPORT  Dslp_Handler
                IMPORT  |Image$$SLEEP_FLAG$$Base|
                ;IMPORT  check_dslp

                ;make sure SLEEP_BUFFER is retained before store data to it
                PUSH    {r0-r2}
                LDR     r0, =0x40000028
                LDR     r1, [r0]
                LDR     r2, =0xFDFFFFFF     ;bit25 = 0 bottom 32K PSO disabled
                ANDS    r1, r1, r2
                STR     r1, [r0]
                LDR     r2, =0xFFFFFDFF     ;bit9 = 0 bottom 32K retention disabled
                ANDS    r1, r1, r2
                STR     r1, [r0]
                POP     {r0-r2}

                ;start to store data, use r7 as base register
                PUSH    {r7}
                
                ;set key and return address
                LDR     r7, =|Image$$SLEEP_FLAG$$Base|
                PUSH    {r0, r1}            ;r0=key, r1=Wakeup_Handler
                LDR     r0, =0xBEEFBEEF     ;a key to indicate system waking up from deep sleep
                LDR     r1, =Wakeup_Handler ;address of Wakeup_Handler
                STMIA   r7!, {r0-r1}        ;store key and wakeup address
                POP     {r0, r1}            ;restore r0, r1
                
                ;store r0-r14, r15 is not necessary because system return by r14
                LDR     r7, =__SB_base
                STMIA   r7!, {r0-r6}        ;store r0~r6
                POP     {r0}                ;pop r7 to r0
                MOV     r1, r8              ;move hi register to lo register
                MOV     r2, r9
                MOV     r3, r10
                MOV     r4, r11
                MOV     r5, r12
                MOV     r6, r13             ;r13 is back to original address
                STMIA   r7!, {r0-r6}        ;store r7~r13
                MOV     r0, r14
                STMIA   r7!, {r0}           ;store r14, no need to store r15 because FW return by r14
                
                ;all CPU registers are available to be used now
                ;store other CPU registers
                LDR     r0, =0xE000ED08     ;VTOR, 0xE000ED08
                LDR     r1, [r0]
                LDR     r2, [r0, #0x0C]     ;CCR, 0xE000ED14
                ;M0 not support --> LDR     r3, [r0, #0x80]     ;CPACR, 0xE000ED88
                LDR     r0, =0xE000ED88     ;CPACR, 0xE000ED88
                LDR     r3, [r0]
                STMIA   r7!, {r1-r3}        ;store them
                

;;;;;testing
;                LDR     r0, =0
;                LDR     r1, =0
;                LDR     r2, =0
;                LDR     r3, =0
;                LDR     r4, =0
;                LDR     r5, =0
;                LDR     r6, =0
;                LDR     r7, =0
;                MOV     r9, r0
;                MOV     r9, r0
;                MOV     r10, r0
;                MOV     r11, r0
;                MOV     r12, r0
;                MOV     r13, r0
;                MOV     r14, r0
;                LDR     r0, =0xE000ED08     ;VTOR
;                STR     r1, [r0]
;                STR     r2, [r0, #0x0C]     ;CCR
;                ;M0 not support --> STR     r3, [r0, #0x80]     ;CPACR
;                LDR     r0, =0xE000ED88     ;CPACR
;                STR     r3, [r0]
;                
;                LDR     r0, =Wakeup_Handler
;                BX      r0
 ;;;;;testing


                WFI

                ;never return because CPU will be turned OFF
                ;when system wakeup, directly use r14 as return address
                B       .
                ENDP

Wakeup_Handler  PROC {}                 ;"{}" means no arguments for this funciton
                IMPORT  |Image$$SLEEP_FLAG$$Base|

                ;This function is called by BootROM and SLEEP_BUFFER is already ON by BootROM
                ;Now, SP is point to the bottom of BootROM stack which is unknown to flash code
                ;So, don't do PUSH/POP before SP is restore.
                ;Otherwise, original stack content will be destroied

                ;clear key and function address
                LDR     r7, =|Image$$SLEEP_FLAG$$Base|
                LDR     r0, =0x00
                STR     r0, [r7]            ;clear key
                STR     r0, [r7, #4]        ;clear Wakeup_Handler address

                ;start to restore data, use r7 as base register
                LDR     r7, =__SB_base + 8*4    ;skip r0~r7
                LDMIA   r7!, {r0-r6}        ;restore r8~r14
                MOV     r8, r0
                MOV     r9, r1
                MOV     r10, r2
                MOV     r11, r3
                MOV     r12, r4
                MOV     r13, r5             ;SP is restored, stack is available
                MOV     r14, r6
                
                ;restore other CPU registers
                LDMIA   r7!, {r1-r3}        ;restore VTOR, CCR, CPACR
                LDR     r0, =0xE000ED08     ;VTOR
                STR     r1, [r0]
                STR     r2, [r0, #0x0C]     ;CCR
                ;M0 not support --> STR     r3, [r0, #0x80]     ;CPACR
                LDR     r0, =0xE000ED88     ;CPACR
                STR     r3, [r0]
                
                ;final step, restore r0~r6
                LDR     r7, =__SB_base
                LDMIA   r7!, {r0-r6}        ;restore r0~r6
                LDR     r7, [r7]            ;restore r7
                
                BX      lr
                ENDP


; Dummy Exception Handlers (infinite loops which can be modified)

NMI_Handler     PROC
                EXPORT  NMI_Handler               [WEAK]
                B       .
                ENDP
HardFault_Handler\
                PROC
                EXPORT  HardFault_Handler         [WEAK]
                B       .
                ENDP
MemManage_Handler\
                PROC
                EXPORT  MemManage_Handler         [WEAK]
                B       .
                ENDP
BusFault_Handler\
                PROC
                EXPORT  BusFault_Handler          [WEAK]
                B       .
                ENDP
UsageFault_Handler\
                PROC
                EXPORT  UsageFault_Handler        [WEAK]
                B       .
                ENDP
SVC_Handler     PROC
                EXPORT  SVC_Handler               [WEAK]
                B       .
                ENDP
DebugMon_Handler\
                PROC
                EXPORT  DebugMon_Handler          [WEAK]
                B       .
                ENDP
PendSV_Handler  PROC
                EXPORT  PendSV_Handler            [WEAK]
                B       .
                ENDP
SysTick_Handler PROC
                EXPORT  SysTick_Handler           [WEAK]
                B       .
                ENDP

Default_Handler PROC

                EXPORT  BLE_IRQHandler              [WEAK]
                EXPORT  WDT_IRQHandler              [WEAK]
                EXPORT  I2C0_M_IRQHandler           [WEAK]
                EXPORT  SPI3_M_IRQHandler           [WEAK]
                EXPORT  WKTM0_IRQHandler            [WEAK]
                EXPORT  WKTM1_IRQHandler            [WEAK]
                EXPORT  UART0_RXDMA_IRQHandler      [WEAK]
                EXPORT  UART1_RXDMA_IRQHandler      [WEAK]
                EXPORT  UART2_RXDMA_IRQHandler      [WEAK]
                EXPORT  SPI0_M_IRQHandler           [WEAK]
                EXPORT  SPI1_M_IRQHandler           [WEAK]
                EXPORT  SPI2_M_IRQHandler           [WEAK]
                EXPORT  I2C1_M_IRQHandler           [WEAK]
                EXPORT  GPIO_IRQHandler             [WEAK]
                EXPORT  UART0_TXDMA_IRQHandler      [WEAK]
                EXPORT  UART0_IP_IRQHandler         [WEAK]
                EXPORT  UART1_TXDMA_IRQHandler      [WEAK]
                EXPORT  UART1_IP_IRQHandler         [WEAK]
                EXPORT  UART2_TXDMA_IRQHandler      [WEAK]
                EXPORT  UART2_IP_IRQHandler         [WEAK]
                EXPORT  HS_IRQHandler               [WEAK]
                EXPORT  PWM0_IRQHandler             [WEAK]
                EXPORT  PWM1_IRQHandler             [WEAK]
                EXPORT  RTC_IRQHandler              [WEAK]
                EXPORT  VOICE_WKUP_IRQHandler       [WEAK]
                EXPORT  I2S_RXDMA_IRQHandler        [WEAK]
                EXPORT  I2S_TXDMA_IRQHandler        [WEAK]
                EXPORT  I2S_IP_IRQHandler           [WEAK]
                EXPORT  DMIC_IRQHandler             [WEAK]
                EXPORT  AHBBRG_IRQHandler           [WEAK]
                EXPORT  AES_IRQHandler              [WEAK]
                EXPORT  EFLASH_IRQHandler           [WEAK]

BLE_IRQHandler
WDT_IRQHandler
I2C0_M_IRQHandler
SPI3_M_IRQHandler
WKTM0_IRQHandler
WKTM1_IRQHandler
UART0_RXDMA_IRQHandler
UART1_RXDMA_IRQHandler
UART2_RXDMA_IRQHandler
SPI0_M_IRQHandler
SPI1_M_IRQHandler
SPI2_M_IRQHandler
I2C1_M_IRQHandler
GPIO_IRQHandler
UART0_TXDMA_IRQHandler
UART0_IP_IRQHandler
UART1_TXDMA_IRQHandler
UART1_IP_IRQHandler
UART2_TXDMA_IRQHandler
UART2_IP_IRQHandler
HS_IRQHandler
PWM0_IRQHandler
PWM1_IRQHandler
RTC_IRQHandler
VOICE_WKUP_IRQHandler
I2S_RXDMA_IRQHandler
I2S_TXDMA_IRQHandler
I2S_IP_IRQHandler
DMIC_IRQHandler
AHBBRG_IRQHandler
AES_IRQHandler
EFLASH_IRQHandler
                B       .

                ENDP

                ALIGN

; User Initial Stack & Heap

                IF      :DEF:__MICROLIB

                EXPORT  __initial_sp
                EXPORT  __heap_base
                EXPORT  __heap_limit

                ELSE

                IMPORT  __use_two_region_memory
                EXPORT  __user_initial_stackheap

__user_initial_stackheap PROC
                LDR     R0, =  Heap_Mem
                LDR     R1, =(Stack_Mem + Stack_Size)
                LDR     R2, = (Heap_Mem +  Heap_Size)
                LDR     R3, = Stack_Mem
                BX      LR
                ENDP

                ALIGN

                ENDIF


                END
