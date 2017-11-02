;/**************************************************************************//**
; * @file     startup_ARMCM4.s
; * @brief    CMSIS Core Device Startup File for
; *           ARMCM4 Device Series
; * @version  V5.00
; * @date     02. March 2016
; ******************************************************************************/
;/*
; * Copyright (c) 2009-2016 ARM Limited. All rights reserved.
; *
; * SPDX-License-Identifier: Apache-2.0
; *
; * Licensed under the Apache License, Version 2.0 (the License); you may
; * not use this file except in compliance with the License.
; * You may obtain a copy of the License at
; *
; * http://www.apache.org/licenses/LICENSE-2.0
; *
; * Unless required by applicable law or agreed to in writing, software
; * distributed under the License is distributed on an AS IS BASIS, WITHOUT
; * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
; * See the License for the specific language governing permissions and
; * limitations under the License.
; */

;/*
;//-------- <<< Use Configuration Wizard in Context Menu >>> ------------------
;*/


; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Stack_Size      EQU     0x00000800

                AREA    STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem       SPACE   Stack_Size
__initial_sp


; <h> Heap Configuration
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Heap_Size       EQU     0x000004

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base
Heap_Mem        SPACE   Heap_Size
__heap_limit

                PRESERVE8
                THUMB

;                AREA    WDTBIN, CODE, READONLY
;                EXPORT  __WDTRST_entry
;__WDTRST_entry
;                INCBIN  ..\..\..\FPGA_M0p_wdtrst.bin    ;include WDT bin file


; Vector Table Mapped to Address 0 at Reset
                AREA    VECTOR_IN_EFLASH, DATA, READONLY
                DCD     __initial_sp              ; Top of Stack
                DCD     Reset_Handler             ; Reset Handler

; Vector Table Mapped to retention RAM for system wakeup
                AREA    VECTOR_IN_RET_RAM, DATA, READONLY
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
                DCD     SWI0_IRQHandler           ;
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
                DCD     SWI1_IRQHandler           ;
                DCD     PWM0_IRQHandler           ;
                DCD     PWM1_IRQHandler           ;
                DCD     RTC_IRQHandler            ;
                DCD     DMU_IRQHandler            ;
                DCD     I2S_RXDMA_IRQHandler      ;
                DCD     I2S_TXDMA_IRQHandler      ;
                DCD     I2S_IP_IRQHandler         ;
                DCD     DMIC_IRQHandler           ;
                DCD     CCU_IRQHandler            ;
                DCD     AES_IRQHandler            ;
                DCD     EFLASH_IRQHandler         ;
__Vectors_End

__Vectors_Size  EQU     __Vectors_End - __Vectors

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
                EXPORT  SWI0_IRQHandler             [WEAK]
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
                EXPORT  SWI1_IRQHandler             [WEAK]
                EXPORT  PWM0_IRQHandler             [WEAK]
                EXPORT  PWM1_IRQHandler             [WEAK]
                EXPORT  RTC_IRQHandler              [WEAK]
                EXPORT  DMU_IRQHandler              [WEAK]
                EXPORT  I2S_RXDMA_IRQHandler        [WEAK]
                EXPORT  I2S_TXDMA_IRQHandler        [WEAK]
                EXPORT  I2S_IP_IRQHandler           [WEAK]
                EXPORT  DMIC_IRQHandler             [WEAK]
                EXPORT  CCU_IRQHandler              [WEAK]
                EXPORT  AES_IRQHandler              [WEAK]
                EXPORT  EFLASH_IRQHandler           [WEAK]

BLE_IRQHandler
WDT_IRQHandler
I2C0_M_IRQHandler
SWI0_IRQHandler
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
SWI1_IRQHandler
PWM0_IRQHandler
PWM1_IRQHandler
RTC_IRQHandler
DMU_IRQHandler
I2S_RXDMA_IRQHandler
I2S_TXDMA_IRQHandler
I2S_IP_IRQHandler
DMIC_IRQHandler
CCU_IRQHandler
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
