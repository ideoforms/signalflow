#pragma once

#include <string>

namespace signalflow
{

/*------------------------------------------------------------------------
 * Perform one-time global initialisation.
 * Called from AudioGraph::init().
 *-----------------------------------------------------------------------*/
void signalflow_init();

/*------------------------------------------------------------------------
 * Output debugging messages that are only included in DEBUG builds.
 *-----------------------------------------------------------------------*/
void signalflow_debug(char const *msg, ...);

/*------------------------------------------------------------------------
 * Throw an exception from the audio thread. It is important not to
 * throw a C++ exception as normal, as these will not percolate up
 * to the main Python thread, so will manifest as a crash in many
 * environments (e.g. Jupyter).
 *
 * Instead, output the error message to stderr and halt the graph.
 *-----------------------------------------------------------------------*/
void signalflow_audio_thread_error(std::string message);

}
