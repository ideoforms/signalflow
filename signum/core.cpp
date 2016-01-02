#include "core.h"

void signum_error(char * msg, ... )
{
	va_list v;
	va_start(v, msg);
	vfprintf(stderr, msg, v);
	va_end(v);

	signum_exit(1);
}

void signum_exit(int status_code)
{
	exit(status_code);
}
