
#ifndef LOGUTIL_H
#define LOGUTIL_H

#ifdef LOG_PRINTF
#undef LOG_PRINTF
#endif

#ifdef LOGSYS_FLAG

extern void rt_initialize (void);
extern void rt_log_printf (const char * pchFormat,...);

#define LOG_INIT(A, B, C)   rt_log_initialize()
#define LOG_PRINTF(P)		rt_log_printf P

#else

#define LOG_INIT(A, B, C)
#define LOG_PRINTF(P)

#endif

#endif
