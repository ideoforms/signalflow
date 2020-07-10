#include "signalflow/core/core.h"
#include "signalflow/core/random.h"

#include <iostream>
#include <stdarg.h>
#include <stdlib.h>

using namespace signalflow;

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
