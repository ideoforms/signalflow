#include "core.h"

#include <iostream>
#include <stdlib.h>

void signum_debug(char const * msg, ... )
{
	va_list v;
	va_start(v, msg);
	vfprintf(stderr, msg, v);
	va_end(v);
}
