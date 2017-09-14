/******************************************************************************
*  Copyright 2017, Cloud Chip, Inc.
*  ---------------------------------------------------------------------------
*  Statement:
*  ----------
*  This software is protected by Copyright and the information contained
*  herein is confidential. The software may not be copied and the information
*  contained herein may not be used or disclosed except with the written
*  permission of Cloud Chip, Inc. (C) 2017
******************************************************************************/

#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>

#include "tracer.h"

#define CONFIG_LOG_BUF_SHIFT 10

#define CONFIG_CONSOLE_LOGLEVEL 8
#define CONFIG_DEFAULT_LOGLEVEL 4

#define CONSOLE_LOGLEVEL         CONFIG_CONSOLE_LOGLEVEL
#define DEFAULT_LOGLEVEL         CONFIG_DEFAULT_LOGLEVEL

#define LOG_BUF_LEN   (1 << CONFIG_LOG_BUF_SHIFT)
#define LOG_BUF_MASK (LOG_BUF_LEN-1)
#define LOG_BUF(idx) (g_pLogBuf[(idx) & LOG_BUF_MASK])

static int g_bNewTextLine;
static char g_baPrintfBuf[1024];

static char g_baLogBuf[LOG_BUF_LEN];
static char *g_pLogBuf = g_baLogBuf;

static uint16_t g_wLogStart;
static uint16_t g_wConStart;
static uint16_t g_wLogEnd;

static inline int
vscnprintf(char *buf, size_t size, const char *fmt, va_list args)
{
    int i;
    size_t ssize = size;

    i = vsnprintf(buf, size, fmt, args);

    return (i >= ssize) ? (ssize - 1) : i;
}

static uint8_t Tracer_LogPrefix(const char *p, uint8_t *pbLevel)
{
    uint8_t bLvl = 0;
    uint8_t bPrefixLen;

    if (p[0] != '<' || !p[1])
        return 0;
    if (p[2] == '>') {
        /* usual single digit level number or special char */
        bLvl = p[1] - '0';
        bPrefixLen = 3;
    } else
        return 0;

    if (pbLevel)
        *pbLevel = bLvl;

    return bPrefixLen;
}

static void Tracer_EmitLogChar(char c)
{
    LOG_BUF(g_wLogEnd) = c;
    g_wLogEnd++;
    if (g_wLogEnd - g_wLogStart > LOG_BUF_LEN)
        g_wLogStart = g_wLogEnd - LOG_BUF_LEN;
	if (g_wLogEnd - g_wConStart > LOG_BUF_LEN)
		g_wConStart = g_wLogEnd - LOG_BUF_LEN;
}

int Tracer_PrintfEmit(const char *fmt, va_list args)
{
    int16_t wPrintedLen = 0;
    uint8_t bCurrentLogLevel = DEFAULT_LOGLEVEL;
    uint8_t bPrefixLen;
    char *pPrintfBuf;

    /* Emit the output into the temporary buffer */
    wPrintedLen += vscnprintf(g_baPrintfBuf + wPrintedLen,
                  sizeof(g_baPrintfBuf) - wPrintedLen, fmt, args);

    pPrintfBuf = g_baPrintfBuf;

    /* Read log level and handle special printk prefix */
    bPrefixLen = Tracer_LogPrefix(pPrintfBuf, &bCurrentLogLevel);
    if (bPrefixLen)
    {
        pPrintfBuf += bPrefixLen;
        if (!g_bNewTextLine)
        {
            Tracer_EmitLogChar('\n');
            g_bNewTextLine = 1;
        }
    }
    if ((bCurrentLogLevel > CONSOLE_LOGLEVEL))
    {
        wPrintedLen = 0;
        return wPrintedLen;
    }

    /*
     * Copy the output into g_pLogBuf. If the caller didn't provide
     * the appropriate log prefix, we insert them here
     */
    for (; *pPrintfBuf; pPrintfBuf++)
    {
        if (g_bNewTextLine)
        {
            g_bNewTextLine = 0;
            if (bPrefixLen)
            {
                /* Copy original log prefix */
                int i;

                for (i = 0; i < bPrefixLen; i++)
                    Tracer_EmitLogChar(g_baPrintfBuf[i]);
                wPrintedLen += bPrefixLen;
            }
            else
            {
                /* Add log prefix */
                Tracer_EmitLogChar('<');
                Tracer_EmitLogChar(bCurrentLogLevel + '0');
                Tracer_EmitLogChar('>');
                wPrintedLen += 3;
            }

//          if (printk_time)
//          {
//              /* Add the current time stamp */
//              char tbuf[50], *tp;
//              unsigned tlen;
//              unsigned long long t;
//              unsigned long nanosec_rem;

//              t = cpu_clock(printk_cpu);
//              nanosec_rem = do_div(t, 1000000000);
//              tlen = sprintf(tbuf, "[%5lu.%06lu] ",
//                      (unsigned long) t,
//                      nanosec_rem / 1000);

//              for (tp = tbuf; tp < tbuf + tlen; tp++)
//                  Tracer_EmitLogChar(*tp);
//              printed_len += tlen;
//          }

            if (!*pPrintfBuf)
                break;
        }

        Tracer_EmitLogChar(*pPrintfBuf);
        if (*pPrintfBuf == '\n')
            g_bNewTextLine = 1;
    }

    drvi_UartTx(TRACER_IF_ID, (const uint8_t*)&LOG_BUF(g_wConStart), g_wLogEnd - g_wConStart);
    if (g_wLogEnd & 0x3UL)
    {
        g_wLogEnd = ((g_wLogEnd >> 2) +1) << 2;
        g_wConStart = g_wLogEnd;		/* Flush */
    }


    return wPrintedLen;
}

int Tracer_Printf(const char *fmt, ...)
{
    va_list args;
    int r;

    va_start(args, fmt);
    r = Tracer_PrintfEmit(fmt, args);
    va_end(args);

    return r;
}
