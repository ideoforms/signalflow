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
	va_list v;
	va_start(v, msg);
	vfprintf(stderr, msg, v);
	fprintf(stderr, "\n");
	va_end(v);
}
