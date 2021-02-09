#include "signalflow/python/python.h"

void init_python_constants(py::module &m)
{
    /*--------------------------------------------------------------------------------
     * Constants
     *-------------------------------------------------------------------------------*/
    py::enum_<signalflow_interpolation_mode_t>(m, "signalflow_interpolation_mode_t", py::arithmetic(), "signalflow_interpolation_mode_t")
        .value("SIGNALFLOW_INTERPOLATION_NONE", SIGNALFLOW_INTERPOLATION_NONE, "No interpolation")
        .value("SIGNALFLOW_INTERPOLATION_LINEAR", SIGNALFLOW_INTERPOLATION_LINEAR, "Linear interpolation")
        .value("SIGNALFLOW_INTERPOLATION_COSINE", SIGNALFLOW_INTERPOLATION_COSINE, "Cosine interpolation")
        .export_values();

    py::enum_<signalflow_event_distribution_t>(m, "signalflow_event_distribution_t", py::arithmetic(), "signalflow_event_distribution_t")
        .value("SIGNALFLOW_EVENT_DISTRIBUTION_UNIFORM", SIGNALFLOW_EVENT_DISTRIBUTION_UNIFORM, "Uniform distribution")
        .value("SIGNALFLOW_EVENT_DISTRIBUTION_POISSON", SIGNALFLOW_EVENT_DISTRIBUTION_POISSON, "Poisson distribution")
        .export_values();

    py::enum_<signalflow_node_state_t>(m, "signalflow_node_state_t", py::arithmetic(), "signalflow_node_state_t")
        .value("SIGNALFLOW_NODE_STATE_ACTIVE", SIGNALFLOW_NODE_STATE_ACTIVE, "Active")
        .value("SIGNALFLOW_NODE_STATE_STOPPED", SIGNALFLOW_NODE_STATE_STOPPED, "Stopped")
        .export_values();

    m.attr("SIGNALFLOW_MAX_CHANNELS") = SIGNALFLOW_MAX_CHANNELS;
    m.attr("SIGNALFLOW_DEFAULT_FFT_SIZE") = SIGNALFLOW_DEFAULT_FFT_SIZE;
    m.attr("SIGNALFLOW_MAX_FFT_SIZE") = SIGNALFLOW_MAX_FFT_SIZE;
    m.attr("SIGNALFLOW_DEFAULT_FFT_HOP_SIZE") = SIGNALFLOW_DEFAULT_FFT_HOP_SIZE;
    m.attr("SIGNALFLOW_DEFAULT_SAMPLE_RATE") = SIGNALFLOW_DEFAULT_SAMPLE_RATE;
    m.attr("SIGNALFLOW_DEFAULT_BLOCK_SIZE") = SIGNALFLOW_DEFAULT_BLOCK_SIZE;
    m.attr("SIGNALFLOW_NODE_BUFFER_SIZE") = SIGNALFLOW_NODE_BUFFER_SIZE;
    m.attr("SIGNALFLOW_DEFAULT_TRIGGER") = SIGNALFLOW_DEFAULT_TRIGGER;
}
