#include "signalflow/python/python.h"

void init_python_constants(py::module &m)
{
    /*--------------------------------------------------------------------------------
     * Constants
     *-------------------------------------------------------------------------------*/
    py::enum_<signal_interpolation_mode_t>(m, "signal_interpolation_mode_t", py::arithmetic(), "signal_interpolation_mode_t")
        .value("SIGNAL_INTERPOLATION_NONE", SIGNAL_INTERPOLATION_NONE, "No interpolation")
        .value("SIGNAL_INTERPOLATION_LINEAR", SIGNAL_INTERPOLATION_LINEAR, "Linear interpolation")
        .export_values();

    py::enum_<signal_event_distribution_t>(m, "signal_event_distribution_t", py::arithmetic(), "signal_event_distribution_t")
        .value("SIGNAL_EVENT_DISTRIBUTION_UNIFORM", SIGNAL_EVENT_DISTRIBUTION_UNIFORM, "Uniform distribution")
        .value("SIGNAL_EVENT_DISTRIBUTION_POISSON", SIGNAL_EVENT_DISTRIBUTION_POISSON, "Poisson distribution")
        .export_values();

    py::enum_<signal_node_state_t>(m, "signal_node_state_t", py::arithmetic(), "signal_node_state_t")
        .value("SIGNAL_NODE_STATE_ACTIVE", SIGNAL_NODE_STATE_ACTIVE, "Active")
        .value("SIGNAL_NODE_STATE_FINISHED", SIGNAL_NODE_STATE_FINISHED, "Finished")
        .export_values();

    m.attr("SIGNAL_MAX_CHANNELS") = SIGNAL_MAX_CHANNELS;
    m.attr("SIGNAL_DEFAULT_FFT_SIZE") = SIGNAL_DEFAULT_FFT_SIZE;
    m.attr("SIGNAL_MAX_FFT_SIZE") = SIGNAL_MAX_FFT_SIZE;
    m.attr("SIGNAL_DEFAULT_FFT_HOP_SIZE") = SIGNAL_DEFAULT_FFT_HOP_SIZE;
    m.attr("SIGNAL_DEFAULT_SAMPLE_RATE") = SIGNAL_DEFAULT_SAMPLE_RATE;
    m.attr("SIGNAL_DEFAULT_BLOCK_SIZE") = SIGNAL_DEFAULT_BLOCK_SIZE;
    m.attr("SIGNAL_NODE_BUFFER_SIZE") = SIGNAL_NODE_BUFFER_SIZE;
    m.attr("SIGNAL_DEFAULT_TRIGGER") = SIGNAL_DEFAULT_TRIGGER;
}
