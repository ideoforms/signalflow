#include "signal/core.h"
#include "signal/util.h"

#include <iostream>
#include <stdlib.h>
#include <stdarg.h>


using namespace libsignal;

void signal_init()
{
    random_init();
}

void signal_debug(char const * msg, ... )
{
    #ifdef DEBUG

        va_list v;
        va_start(v, msg);
        vfprintf(stdout, msg, v);
        fprintf(stdout, "\n");
        va_end(v);

    #endif
}

void signal_assert(bool equality, char const *msg, ...)
{
    if (!equality)
    {
        va_list v;
        va_start(v, msg);
        fprintf(stdout, "SIGNAL FATAL ERROR: ");
        vfprintf(stdout, msg, v);
        fprintf(stdout, "\n");
        va_end(v);
        exit(1);
    }
}

void signal_warn(char const * msg, ... )
{
    va_list v;
    va_start(v, msg);
    vfprintf(stderr, msg, v);
    fprintf(stderr, "\n");
    va_end(v);
}
