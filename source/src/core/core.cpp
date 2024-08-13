#include "signalflow/core/core.h"
#include "signalflow/core/graph.h"
#include "signalflow/core/random.h"

#include <iostream>
#include <stdarg.h>
#include <stdlib.h>

using namespace signalflow;

namespace signalflow
{

extern AudioGraph *shared_graph;

void signalflow_init()
{
    random_init();
}

void signalflow_debug(char const *msg, ...)
{
#ifdef DEBUG

    va_list v;
    va_start(v, msg);
    vfprintf(stdout, msg, v);
    fprintf(stdout, "\n");
    va_end(v);

#endif
}

void signalflow_audio_thread_error(std::string message)
{
    /*------------------------------------------------------------------------
     * Set an error flag to ensure this error handling only happens once, and
     * stop the graph.
     *
     * TODO: Ideally, we want to tear down the audio I/O. However, because
     *       this function is inherently called from within the audio I/O
     *       process, this can't be done here. We could set a flag that
     *       is acted on within the Python implementation of the
     *       AudioGraph::wait() loop, but this won't be used in all
     *       instances.
     *-----------------------------------------------------------------------*/
    if (!shared_graph->has_raised_audio_thread_error())
    {
        std::cerr << "Exception in audio thread: " + message << std::endl;
        std::cerr << "AudioGraph now terminating..." << std::endl;
        shared_graph->raise_audio_thread_error();
        shared_graph->stop();
    }
}

}