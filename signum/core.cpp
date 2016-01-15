#include "core.h"

#include <iostream>
#include <stdlib.h>
#include <stdarg.h>

#include "util.h"

using namespace signum;

void signum_init()
{
	rng_init();
}

void signum_debug(char const * msg, ... )
{
	#ifdef DEBUG

		va_list v;
		va_start(v, msg);
		vfprintf(stdout, msg, v);
		fprintf(stdout, "\n");
		va_end(v);

	#endif
}

void signum_warn(char const * msg, ... )
{
	va_list v;
	va_start(v, msg);
	vfprintf(stderr, msg, v);
	fprintf(stderr, "\n");
	va_end(v);
}
