#include "signal/core/core.h"
#include "signal/core/random.h"

#include <iostream>
#include <stdarg.h>
#include <stdlib.h>

using namespace libsignal;

void signal_init()
{
    random_init();
}

void signal_debug(char const *msg, ...)
{
#ifdef DEBUG

    va_list v;
    va_start(v, msg);
    vfprintf(stdout, msg, v);
    fprintf(stdout, "\n");
    va_end(v);

#endif
}

void signal_warn(char const *msg, ...)
{
    va_list v;
    va_start(v, msg);
    vfprintf(stderr, msg, v);
    fprintf(stderr, "\n");
    va_end(v);
}
