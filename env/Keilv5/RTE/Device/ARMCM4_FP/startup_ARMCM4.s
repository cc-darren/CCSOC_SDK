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

                AREA    RESET, DATA, READONLY
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
                DCD     SWI0_IRQHandler           ;  0:  Real Time Clock
                DCD     WDT_IRQHandler            ;  1:  Watchdog Timer
                DCD     RTC_IRQHandler            ;  2:  Timer0 / Timer1
                DCD     PWM0_IRQHandler           ;  3:  PWM0 Interrup
                DCD     PWM1_IRQHandler           ;  4:  PWM1 Interrup
                DCD     WKTM0_IRQHandler          ;  5:  MCIb
                DCD     WKTM1_IRQHandler          ;  6:  UART0 - DUT FPGA
                DCD     SPI0_M_IRQHandler         ;  7:  SPI0 Master
                DCD     SPI1_M_IRQHandler         ;  8:  SPI1 Master
                DCD     SPI2_M_IRQHandler         ;  9:  SPI2 Master
                DCD     UART0_RXDMA_IRQHandler    ; 10: UART0 Rx DMA interrupt
                DCD     UART0_TXDMA_IRQHandler    ; 11: UART0 Tx DMA interrupt
                DCD     UART0_IP_IRQHandler       ; 12: UART0 Core interrupt
                DCD     UART1_RXDMA_IRQHandler    ; 13: UART1 Rx DMA interrupt
                DCD     UART1_TXDMA_IRQHandler    ; 14: UART1 Tx DMA interrupt
                DCD     UART1_IP_IRQHandler       ; 15: UART1 Core interrupt
                DCD     UART2_RXDMA_IRQHandler    ; 16: UART2 Rx DMA interrupt
                DCD     UART2_TXDMA_IRQHandler    ; 17: UART2 Tx DMA interrupt
                DCD     UART2_IP_IRQHandler       ; 18: UART2 Core interrupt
                DCD     I2C0_M_IRQHandler         ; 19: I2C0 ADC/DAC
                DCD     I2C1_M_IRQHandler         ; 20: I2C1 ADC/DAC
                DCD     I2S_RXDMA_IRQHandler      ; 21: I2S Rx DMA interrupt
                DCD     I2S_TXDMA_IRQHandler      ; 22: I2S Tx DMA interrupt
                DCD     I2S_IP_IRQHandler         ; 23: I2S Core interrupt
                DCD     DMIC_IRQHandler           ; 24: DMIC interrupt
                DCD     BLE_IRQHandler            ; 25: Reserved
                DCD     GPIO_IRQHandler           ; 26: GPIO interrupt
                DCD     CCU_IRQHandler            ; 27: Reserved
                DCD     AES_IRQHandler            ; 28: AES interrupt
                DCD     DMU_IRQHandler            ; 29: DMU interrupt
                DCD     SWI1_IRQHandler           ; 30: UART3    - CPU FPGA
                DCD     EFLASH_IRQHandler         ; 31: SPI Touchscreen - CPU FPGA
__Vectors_End

__Vectors_Size  EQU     __Vectors_End - __Vectors

                AREA    |.text|, CODE, READONLY


; Reset Handler

Reset_Handler   PROC
                EXPORT  Reset_Handler             [WEAK]
                IMPORT  SystemInit
                IMPORT  __main
                
                LDR     R0, =SystemInit
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

                EXPORT  SWI0_IRQHandler             [WEAK]
                EXPORT  WDT_IRQHandler              [WEAK]
                EXPORT  RTC_IRQHandler              [WEAK]
                EXPORT  PWM0_IRQHandler             [WEAK]
                EXPORT  PWM1_IRQHandler             [WEAK]
                EXPORT  WKTM0_IRQHandler            [WEAK]
                EXPORT  WKTM1_IRQHandler            [WEAK]
                EXPORT  SPI0_M_IRQHandler           [WEAK]
                EXPORT  SPI1_M_IRQHandler           [WEAK]
                EXPORT  SPI2_M_IRQHandler           [WEAK]
                EXPORT  UART0_RXDMA_IRQHandler      [WEAK]
                EXPORT  UART0_TXDMA_IRQHandler      [WEAK]
                EXPORT  UART0_IP_IRQHandler         [WEAK]
                EXPORT  UART1_RXDMA_IRQHandler      [WEAK]
                EXPORT  UART1_TXDMA_IRQHandler      [WEAK]
                EXPORT  UART1_IP_IRQHandler         [WEAK]
                EXPORT  UART2_RXDMA_IRQHandler      [WEAK]
                EXPORT  UART2_TXDMA_IRQHandler      [WEAK]
                EXPORT  UART2_IP_IRQHandler         [WEAK]
                EXPORT  I2C0_M_IRQHandler           [WEAK]
                EXPORT  I2C1_M_IRQHandler           [WEAK]
                EXPORT  I2S_RXDMA_IRQHandler        [WEAK]
                EXPORT  I2S_TXDMA_IRQHandler        [WEAK]
                EXPORT  I2S_IP_IRQHandler           [WEAK]
                EXPORT  DMIC_IRQHandler             [WEAK]
                EXPORT  BLE_IRQHandler              [WEAK]
                EXPORT  GPIO_IRQHandler             [WEAK]
                EXPORT  CCU_IRQHandler              [WEAK]
                EXPORT  AES_IRQHandler              [WEAK]
                EXPORT  DMU_IRQHandler              [WEAK]
                EXPORT  SWI1_IRQHandler             [WEAK]
                EXPORT  EFLASH_IRQHandler           [WEAK]

SWI0_IRQHandler
WDT_IRQHandler
RTC_IRQHandler
PWM0_IRQHandler
PWM1_IRQHandler
WKTM0_IRQHandler
WKTM1_IRQHandler
SPI0_M_IRQHandler
SPI1_M_IRQHandler
SPI2_M_IRQHandler
UART0_RXDMA_IRQHandler
UART0_TXDMA_IRQHandler
UART0_IP_IRQHandler
UART1_RXDMA_IRQHandler
UART1_TXDMA_IRQHandler
UART1_IP_IRQHandler
UART2_RXDMA_IRQHandler
UART2_TXDMA_IRQHandler
UART2_IP_IRQHandler
I2C0_M_IRQHandler
I2C1_M_IRQHandler
I2S_RXDMA_IRQHandler
I2S_TXDMA_IRQHandler
I2S_IP_IRQHandler
DMIC_IRQHandler
BLE_IRQHandler
GPIO_IRQHandler
CCU_IRQHandler
AES_IRQHandler
DMU_IRQHandler
SWI1_IRQHandler
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
