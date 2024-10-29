#include "signalflow/python/python.h"

void init_python_exceptions(py::module &m)
{
    /*--------------------------------------------------------------------------------
     * Exceptions
     *-------------------------------------------------------------------------------*/
    py::register_exception<signalflow::graph_not_created_exception>(m, "GraphNotCreatedException");

    // Note that this should never be thrown as Python's AudioGraph layer treats AudioGraph
    // as a singleton.
    py::register_exception<signalflow::graph_already_created_exception>(m, "GraphAlreadyCreatedException");

    py::register_exception<signalflow::invalid_channel_count_exception>(m, "InvalidChannelCountException");
    py::register_exception<signalflow::insufficient_buffer_size_exception>(m, "InsufficientBufferSizeException");
    py::register_exception<signalflow::patch_finished_playback_exception>(m, "PatchFinishedPlaybackException");
    py::register_exception<signalflow::device_not_found_exception>(m, "DeviceNotFoundException");
    py::register_exception<signalflow::audio_io_exception>(m, "AudioIOException");
    py::register_exception<signalflow::node_already_playing_exception>(m, "NodeAlreadyPlayingException");
    py::register_exception<signalflow::node_not_playing_exception>(m, "NodeNotPlayingException");
    py::register_exception<signalflow::cpu_usage_above_limit_exception>(m, "CPUUsageAboveLimitException");
    py::register_exception<signalflow::unknown_trigger_name_exception>(m, "UnknownTriggerNameException");
}
