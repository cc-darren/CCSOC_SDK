/* Copyright (c) 2019 Cloudchip, Inc. All Rights Reserved.
 *
 * The information contained herein is property of Cloudchip, Inc.
 * Terms and conditions of usage are described in detail in CLOUDCHIP
 * STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information.
 * NO WARRANTY of ANY KIND is provided. This heading must NOT be removed
 * from the file.
 */

#ifndef _DRVI_UART_H_
#define _DRVI_UART_H_

#include "global.h"
#include "project.h"

#include "uart.h"

typedef enum
{
    DRVI_UART_EVENT_TX_DONE,
    DRVI_UART_EVENT_RX_DONE,
    DRVI_UART_EVENT_ERROR,
} E_DrviUartEvent;

typedef struct S_UartEvent
{
    E_DrviUartEvent type;
    uint8_t *pRxBuf;
    uint8_t bRxBytes;
} T_UartEvent;

#if (defined(CC6801B0) || defined(CC6801B1))
typedef struct S_UartRing
{
    uint32_t dwHead;
    uint32_t dwTail;
    uint32_t dwWrLastPos;
    int32_t iRxBufSz;
    uint16_t wCurrRxByteNum;
    uint8_t *pbRxBuffer;
} T_UartRing;
#endif

typedef struct S_UartPort
{
    uint8_t bPortNum;
    uint32_t dwConfig;
    T_UartEvent tUevent;

#if (defined(CC6801B0) || defined(CC6801B1))
    T_UartRing tUring;

    int32_t (*fpDrviUartReceive)(uint8_t *pbBuf, int32_t iSize);
#endif

    void (*fpDrviUartCb)(T_UartEvent *p_event);
} T_UartPort;

typedef void (*fpDrviUartCallback)(T_UartEvent *pEvent);

int32_t drvi_UartInit(void);

__forceinline void drvi_UartConfigSet(T_UartPort *pUartPort)
{
    cc6801_UartConfigSet(pUartPort);
}

#if ((UART_USE_DMA) && (UART_USE_DMA))
__forceinline int32_t drvi_UartTx(uint8_t bPortNum,
                              uint8_t const * const pData,
                              uint8_t bLen)
{
    return cc6801_UartTxDMA(bPortNum, pData, bLen);
}

#if (defined(CC6801B0) || defined(CC6801B1))
void drvi_UartRxDoneRegister(uint8_t bIdx, fpDrviUartCallback fpCb);
int32_t drvi_UartReceive(uint8_t bPort, uint8_t *pbBuf, int32_t iSize);
#else
__forceinline void drvi_UartRxDoneRegister(uint8_t bIdx, fpUartRxDone RxCallBack)
{
    cc6801_UartRxDoneRegister(bIdx, RxCallBack);
}

__forceinline int32_t drvi_UartReceive(uint8_t bPort, uint8_t *pbBuf, uint8_t bLen)
{
    return cc6801_UartRxDMA(bPort, pbBuf, bLen);
}
#endif

#else
__forceinline int32_t drvi_UartTx(uint8_t bPortNum,
                              uint8_t const * const pData,
                              uint8_t bLen)
{
    return cc6801_UartTx(bPortNum, pData, bLen);
}

__forceinline int32_t drvi_UartRx(uint8_t bPortNum, uint8_t *pData, uint8_t bLen)
{
    return cc6801_UartRx(bPortNum, pData, bLen);
}
#endif

#endif //_DRVI_UART_H_
