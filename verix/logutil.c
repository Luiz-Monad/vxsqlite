

#include <stdio.h>
#include <stdarg.h>
#include <string.h>

#ifdef LOGSYS_FLAG

#include <logsys.h> 

extern int dbprintf (const char *fmt, ...);

static int g_console_handle = 0;

const char* APPNAME = "SQLITE";

void rt_log_initialize()
{
	//LOG_INIT((char*)APPNAME, LOGSYS_COMM, LOG_FILTER);
	g_console_handle = open(DEV_CONSOLE, 0);
	//vLogMessage("%s\n", "Started");
}

void rt_log_printf (const char * pchFormat,...)
{
	char buf[4096];
	va_list lst;
	va_start(lst, pchFormat);
	vsnprintf(buf, sizeof(buf), pchFormat, lst);
    write(g_console_handle, buf, strlen(buf));
	dbprintf("%s\n", buf);
    //vLogMessage("%s\n", buf);
	va_end(lst);
}

#endif
