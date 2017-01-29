#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h> /* for "program_invocation_short_name", not directly for "errno" */

#include "err.h"

/* static declarations */
static void verror(int, const char *, va_list);

extern char *program_invocation_short_name;


void errExit(int errnum, const char *msg, ...)
{
	CALL_VERROR(errnum, msg, msg);
	exit(EXIT_FAILURE);
}

void errWarn(int errnum, const char *msg, ...)
{
	CALL_VERROR(errnum, msg, msg);
}

static void verror(int errnum, const char *fmt, va_list argptr)
{
	fprintf(stderr, "%s: ", program_invocation_short_name);
	vfprintf(stderr, fmt, argptr);
	if ( errnum != NO_ERRNO )
		fprintf(stderr, ": %s", strerror(errnum));
	fprintf(stderr, "\n");
}
