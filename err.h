#ifndef ERR_H
#define ERR_H

	#include <stdio.h>
	#include <stdlib.h>
	#include <stdarg.h>
	#include <string.h>
	#include <errno.h> /* for "program_invocation_short_name", not directly for "errno" */

	#define MAGENTA_COL_CODE "\x1B[35m"
	#define RESET_COL_CODE "\x1B[0m"

	#define NO_ERRNO 0

	#define CALL_VERROR(errnum, msg, lastArg) { \
		va_list argptr; \
		va_start(argptr, lastArg); \
		verror(errnum, msg, argptr); \
		va_end(argptr); \
	}

	void errWarn(int, const char *, ...);
	void errExit(int, const char *, ...);

#endif
