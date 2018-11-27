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

#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>

#include "tracer.h"

#define CONFIG_DEFAULT_LOGLEVEL 4

#define CONSOLE_LOGLEVEL         TRACER_LOGLEVEL
#define DEFAULT_LOGLEVEL         CONFIG_DEFAULT_LOGLEVEL

#define LOG_BUF_LEN   TRACER_LOGBUF_SIZE
#define LOG_BUF_MASK (LOG_BUF_LEN-1)
#define LOG_BUF(idx) (g_pLogBuf[(idx) & LOG_BUF_MASK])

static int g_bNewTextLine;
static char g_baPrintfBuf[1024];

__align(4) static char g_baLogBuf[LOG_BUF_LEN];
static char *g_pLogBuf = g_baLogBuf;

static uint16_t g_wLogStart = 0;
static uint16_t g_wLogEnd = 0;

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
}

int Tracer_PrintfEmit(const char *fmt, va_list args)
{
    int16_t wPrintedLen = 0;
    uint8_t bCurrentLogLevel = DEFAULT_LOGLEVEL;
    uint8_t bPrefixLen;
    char *pPrintfBuf;

    /* Emit the output into the temporary buffer */
    wPrintedLen = vscnprintf(g_baPrintfBuf,
                   sizeof(g_baPrintfBuf), fmt, args);

    pPrintfBuf = g_baPrintfBuf;

    /* Read log level and handle special printk prefix */
    bPrefixLen = Tracer_LogPrefix(pPrintfBuf, &bCurrentLogLevel);
    if (bPrefixLen)
    {
        pPrintfBuf += bPrefixLen;
//        if (!g_bNewTextLine)
//        {
//            Tracer_EmitLogChar('\n');
//            g_bNewTextLine = 1;
//        }
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
//            if (bPrefixLen)
//            {
//                /* Copy original log prefix */
//                int i;

//                for (i = 0; i < bPrefixLen; i++)
//                    Tracer_EmitLogChar(g_baPrintfBuf[i]);
//                wPrintedLen += bPrefixLen;
//            }
//            else
//            {
//                /* Add log prefix */
//                Tracer_EmitLogChar('<');
//                Tracer_EmitLogChar(bCurrentLogLevel + '0');
//                Tracer_EmitLogChar('>');
//                wPrintedLen += 3;
//            }

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

    drvi_UartTx(TRACER_IF_ID, (const uint8_t*)g_pLogBuf, g_wLogEnd - g_wLogStart);
    g_wLogEnd = g_wLogStart;

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
