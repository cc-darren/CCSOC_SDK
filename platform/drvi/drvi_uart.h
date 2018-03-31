/* Copyright (c) 2018 Cloudchip, Inc. All Rights Reserved.
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

typedef struct S_UartPort
{
    uint8_t bPortNum;
    uint32_t dwConfig;
    T_UartEvent tUevent;

    void (*fpDrviUartCb)(T_UartEvent *p_event);
} T_UartPort;

typedef void (*fpDrviUartCallback)(T_UartEvent *pEvent);

int drvi_UartInit(void);

__forceinline void drvi_UartConfigSet(T_UartPort *pUartPort)
{
    cc6801_UartConfigSet(pUartPort);
}

#ifdef CC6801A1
void drvi_UartRxDoneRegister(uint8_t bIdx, fpDrviUartCallback fpCb);
#else
__forceinline void drvi_UartRxDoneRegister(uint8_t bIdx, fpUartRxDone RxCallBack)
{
    cc6801_UartRxDoneRegister(bIdx, RxCallBack);
}
#endif

#if (UART_USE_DMA) && UART_USE_DMA
__forceinline int drvi_UartTx(uint8_t bPortNum,
                              uint8_t const * const pData,
                              uint8_t bLen)
{
    return cc6801_UartTxDMA(bPortNum, pData, bLen);
}
#ifdef CC6801A1
int drvi_UartReceive(uint8_t bPort, uint8_t *buf, int size);
#else
__forceinline int drvi_UartRx(uint8_t bPortNum, uint8_t *pData, uint8_t bLen)
{
    return cc6801_UartRxDMA(bPortNum, pData, bLen);
}
#endif
#else
__forceinline int drvi_UartTx(uint8_t bPortNum,
                              uint8_t const * const pData,
                              uint8_t bLen)
{
    return cc6801_UartTx(bPortNum, pData, bLen);
}

__forceinline int drvi_UartRx(uint8_t bPortNum, uint8_t *pData, uint8_t bLen)
{
    return cc6801_UartRx(bPortNum, pData, bLen);
}
#endif

#endif //_DRVI_UART_H_

