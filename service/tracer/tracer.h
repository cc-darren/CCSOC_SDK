#ifndef _TRACER_H_
#define _TRACER_H_

#include "drvi_uart.h"

#define TRACE_EMERG      "<0>"   /* system is unusable           */
#define TRACE_ALERT      "<1>"   /* action must be taken immediately */
#define TRACE_CRIT       "<2>"   /* critical conditions          */
#define TRACE_ERR        "<3>"   /* error conditions         */
#define TRACE_WARNING    "<4>"   /* warning conditions           */
#define TRACE_NOTICE     "<5>"   /* normal but significant condition */
#define TRACE_INFO       "<6>"   /* informational            */
#define TRACE_DEBUG      "<7>"   /* debug-level messages         */

#ifndef TracerFormat
#define TracerFormat(fmt) fmt
#endif

/*
 * These can be used to print at the various log levels.
 */
#define TracerEmerg(fmt, ...) \
    Tracer_Printf(TRACE_EMERG TracerFormat(fmt), ##__VA_ARGS__)
#define TracerAlert(fmt, ...) \
    Tracer_Printf(TRACE_ALERT TracerFormat(fmt), ##__VA_ARGS__)
#define TracerCrit(fmt, ...) \
    Tracer_Printf(TRACE_CRIT TracerFormat(fmt), ##__VA_ARGS__)
#define TracerErr(fmt, ...) \
    Tracer_Printf(TRACE_ERR TracerFormat(fmt), ##__VA_ARGS__)
#define TracerWarning(fmt, ...) \
    Tracer_Printf(TRACE_WARNING TracerFormat(fmt), ##__VA_ARGS__)
#define TracerWarn TracerWarning
#define TracerNotice(fmt, ...) \
    Tracer_Printf(TRACE_NOTICE TracerFormat(fmt), ##__VA_ARGS__)
#define TracerInfo(fmt, ...) \
    Tracer_Printf(TRACE_INFO TracerFormat(fmt), ##__VA_ARGS__)
#define TracerDebug(fmt, ...) \
    Tracer_Printf(TRACE_DEBUG TracerFormat(fmt), ##__VA_ARGS__)

void Tracer_Init(void);
int Tracer_Printf(const char *fmt, ...);

#endif //_TRACER_H_
