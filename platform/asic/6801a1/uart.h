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

#ifndef _UART_H
#define _UART_H

#include "global.h"
#include "cc6801_reg.h"

#define UART_CTRL_INT_EFCI_DISABLE_BIT (0)    /* Flow control interrupt Disable */
#define UART_CTRL_INT_EFCI_DISABLE_MASK (UART_CTRL_INT_EFCI_DISABLE_BIT << 4)
#define UART_CTRL_INT_EFCI_ENABLE_BIT (1)     /* Flow control interrupt Enable */
#define UART_CTRL_INT_EFCI_ENABLE_MASK (UART_CTRL_INT_EFCI_ENABLE_BIT << 4)

#define UART_CTRL_INT_ETI_DISABLE_BIT (0)    /* Transmitter interrupt Disable */
#define UART_CTRL_INT_ETI_DISABLE_MASK (UART_CTRL_INT_ETI_DISABLE_BIT << 5)
#define UART_CTRL_INT_ETI_ENABLE_BIT (1)     /* Transmitter interrupt Enable */
#define UART_CTRL_INT_ETI_ENABLE_MASK (UART_CTRL_INT_ETI_ENABLE_BIT << 5)

#define UART_CTRL_INT_ERI_DISABLE_BIT (0)    /* Receiver interrupt Disable */
#define UART_CTRL_INT_ERI_DISABLE_MASK (UART_CTRL_INT_ERI_DISABLE_BIT << 6)
#define UART_CTRL_INT_ERI_ENABLE_BIT (1)     /* Receiver interrupt Enable */
#define UART_CTRL_INT_ERI_ENABLE_MASK (UART_CTRL_INT_ERI_ENABLE_BIT << 6)

#define UART_CTRL_INT_EEI_DISABLE_BIT (0)    /* Receive error interrupt Disable */
#define UART_CTRL_INT_EEI_DISABLE_MASK (UART_CTRL_INT_EEI_DISABLE_BIT << 7)
#define UART_CTRL_INT_EEI_ENABLE_BIT (1)     /* Receive error interrupt Enable */
#define UART_CTRL_INT_EEI_ENABLE_MASK (UART_CTRL_INT_EEI_ENABLE_BIT << 7)

#define UART_FRS_CHAR_WLEN8_BIT (0)     /* Wordlength: 8 bits */
#define UART_FRS_CHAR_WLEN8_MASK (UART_FRS_CHAR_WLEN8_BIT << 0)
#define UART_FRS_CHAR_WLEN7_BIT (1)     /* Wordlength: 7 bits */
#define UART_FRS_CHAR_WLEN7_MASK (UART_FRS_CHAR_WLEN7_BIT << 0)
#define UART_FRS_CHAR_WLEN9_BIT (2)     /* Wordlength: 9 bits */
#define UART_FRS_CHAR_WLEN9_MASK (UART_FRS_CHAR_WLEN9_BIT << 0)
#define UART_FRS_CHAR_LOOPBACK_BIT (3)
#define UART_FRS_CHAR_LOOPBACK_MASK (UART_FRS_CHAR_LOOPBACK_BIT << 0)

#define UART_FRS_STP_1BIT_BIT (0)       /* Stop bits: 0=1 bit, 1=2 bits */
#define UART_FRS_STP_1BIT_MASK (UART_FRS_STP_1BIT_BIT << 2)
#define UART_FRS_STP_2BIT_BIT (1)
#define UART_FRS_STP_2BIT_MASK (UART_FRS_STP_2BIT_BIT << 2)

#define UART_FRS_XB9_0_BIT (0)          /* Transmit 9th bit: 9th bit=0, 9th bit=1  */
#define UART_FRS_XB9_0_MASK (UART_FRS_XB9_0_BIT << 3)
#define UART_FRS_XB9_1_BIT (1)
#define UART_FRS_XB9_1_MASK (UART_FRS_XB9_1_BIT << 3)

#define UART_FRS_PSEL_OPAR_BIT (0)      /* Odd parity select */
#define UART_FRS_PSEL_OPAR_MASK (UART_FRS_PSEL_OPAR_BIT << 4)
#define UART_FRS_PSEL_EPAR_BIT (1)      /* Even parity */
#define UART_FRS_PSEL_EPAR_MASK (UART_FRS_PSEL_EPAR_BIT << 4)
#define UART_FRS_PSEL_MPAR_BIT (2)      /* Mark parity */
#define UART_FRS_PSEL_MPAR_MASK (UART_FRS_PSEL_MPAR_BIT << 4)
#define UART_FRS_PSEL_SPAR_BIT (3)      /* Space parity */
#define UART_FRS_PSEL_SPAR_MASK (UART_FRS_PSEL_SPAR_BIT << 4)

#define UART_FRS_PEN_DISABLE_BIT (0)    /* Parity Disable */
#define UART_FRS_PEN_DISABLE_MASK (UART_FRS_PEN_DISABLE_BIT << 6)
#define UART_FRS_PEN_ENABLE_BIT (1)     /* Parity Enable */
#define UART_FRS_PEN_ENABLE_MASK (UART_FRS_PEN_ENABLE_BIT << 6)

#define UART_MDSL_BRK_DISABLE_BIT (0)    /* Transmission break Disable */
#define UART_MDSL_BRK_DISABLE_MASK (UART_MDSL_BRK_DISABLE_BIT << 1)
#define UART_MDSL_BRK_ENABLE_BIT (1)     /* Transmission break Enable */
#define UART_MDSL_BRK_ENABLE_MASK (UART_MDSL_BRK_ENABLE_BIT << 1)

#define UART_MDSL_ATN_DISABLE_BIT (0)    /* Attention mode Disable */
#define UART_MDSL_ATN_DISABLE_MASK (UART_MDSL_ATN_DISABLE_BIT << 2)
#define UART_MDSL_ATN_ENABLE_BIT (1)     /* Attention mode Enable */
#define UART_MDSL_ATN_ENABLE_MASK (UART_MDSL_ATN_ENABLE_BIT << 2)

#define UART_MDSL_ETD_DISABLE_BIT (0)    /* Transmit DMA Disable */
#define UART_MDSL_ETD_DISABLE_MASK (UART_MDSL_ETD_DISABLE_BIT << 4)
#define UART_MDSL_ETD_ENABLE_BIT (1)     /* Transmit DMA Enable */
#define UART_MDSL_ETD_ENABLE_MASK (UART_MDSL_ETD_ENABLE_BIT << 4)

#define UART_MDSL_ERD_DISABLE_BIT (0)    /* Receive DMA Disable */
#define UART_MDSL_ERD_DISABLE_MASK (UART_MDSL_ERD_DISABLE_BIT << 5)
#define UART_MDSL_ERD_ENABLE_BIT (1)     /* Receive DMA Enable */
#define UART_MDSL_ERD_ENABLE_MASK (UART_MDSL_ERD_ENABLE_BIT << 5)

#define UART_MDSL_FCE_DISABLE_BIT (0)    /* Flow Control Disable */
#define UART_MDSL_FCE_DISABLE_MASK (UART_MDSL_FCE_DISABLE_BIT << 6)
#define UART_MDSL_FCE_ENABLE_BIT (1)     /* Flow Control Enable */
#define UART_MDSL_FCE_ENABLE_MASK (UART_MDSL_FCE_ENABLE_BIT << 6)

#define UART_MDSL_RTS_DISABLE_BIT (0)    /* RTS: 0=High, 1=Low */
#define UART_MDSL_RTS_DISABLE_MASK (UART_MDSL_RTS_DISABLE_BIT << 7)
#define UART_MDSL_RTS_ENABLE_BIT (1)
#define UART_MDSL_RTS_ENABLE_MASK (UART_MDSL_RTS_ENABLE_BIT << 7)

typedef struct S_UartPort T_UartPort;
typedef struct S_UartEvent T_UartEvent;
typedef void (*fpUartRxDone)(T_UartEvent *p_event);

typedef enum
{
    UART_0 = 0,
    UART_1,
    UART_2,
    UART_TOTAL_SUPPORTED
}E_UartSupported;

typedef struct
{
    void *pDmaReg;
    void *pCtrlReg;
    int (*fpUartXfer)(void const * const pReg);
    int (*fpUartRcvr)(void const * const pReg);

    uint8_t *pRxBuffer;
    uint32_t dwRxCount;
    uint32_t dwRxBufferLen;

    void (*fbRxDoneHandler)(T_UartEvent *p_event);
} T_cc6801UartPort;

int cc6801_UartInit(T_UartPort *pUartPort);

void cc6801_UartConfigSet(T_UartPort *pUartPort);

void cc6801_UartRxDoneRegister(uint8_t bIdx, fpUartRxDone RxCallBack);

int cc6801_UartTxDMA(uint8_t bPortNum,
                   uint8_t const * const pData, uint8_t bLen);

int cc6801_UartRxDMA(uint8_t bPortNum, uint8_t *pData, uint8_t bLen);

int cc6801_UartTx(uint8_t bPortNum,
                   uint8_t const * const pData, uint8_t bLen);

int cc6801_UartRx(uint8_t bPortNum, uint8_t *pData, uint8_t bLen);

#endif //_UART_H