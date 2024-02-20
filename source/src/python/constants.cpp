#include "signalflow/python/python.h"

void init_python_constants(py::module &m)
{
    /*--------------------------------------------------------------------------------
     * Constants
     *-------------------------------------------------------------------------------*/
    py::enum_<signalflow_interpolation_mode_t>(m, "signalflow_interpolation_mode_t", py::arithmetic(), "signalflow_interpolation_mode_t")
        .value("SIGNALFLOW_INTERPOLATION_MODE_NONE", SIGNALFLOW_INTERPOLATION_MODE_NONE, "No interpolation")
        .value("SIGNALFLOW_INTERPOLATION_MODE_LINEAR", SIGNALFLOW_INTERPOLATION_MODE_LINEAR, "Linear interpolation")
        .value("SIGNALFLOW_INTERPOLATION_MODE_COSINE", SIGNALFLOW_INTERPOLATION_MODE_COSINE, "Cosine interpolation")
        .export_values();

    py::enum_<signalflow_event_distribution_t>(m, "signalflow_event_distribution_t", py::arithmetic(), "signalflow_event_distribution_t")
        .value("SIGNALFLOW_EVENT_DISTRIBUTION_UNIFORM", SIGNALFLOW_EVENT_DISTRIBUTION_UNIFORM, "Uniform distribution")
        .value("SIGNALFLOW_EVENT_DISTRIBUTION_POISSON", SIGNALFLOW_EVENT_DISTRIBUTION_POISSON, "Poisson distribution")
        .export_values();

    m.attr("SIGNALFLOW_MAX_CHANNELS") = SIGNALFLOW_MAX_CHANNELS;
    m.attr("SIGNALFLOW_NODE_INITIAL_OUTPUT_BUFFERS") = SIGNALFLOW_NODE_INITIAL_OUTPUT_BUFFERS;
    m.attr("SIGNALFLOW_DEFAULT_FFT_SIZE") = SIGNALFLOW_DEFAULT_FFT_SIZE;
    m.attr("SIGNALFLOW_MAX_FFT_SIZE") = SIGNALFLOW_MAX_FFT_SIZE;
    m.attr("SIGNALFLOW_DEFAULT_FFT_HOP_SIZE") = SIGNALFLOW_DEFAULT_FFT_HOP_SIZE;
    m.attr("SIGNALFLOW_DEFAULT_SAMPLE_RATE") = SIGNALFLOW_DEFAULT_SAMPLE_RATE;
    m.attr("SIGNALFLOW_DEFAULT_BLOCK_SIZE") = SIGNALFLOW_DEFAULT_BLOCK_SIZE;
    m.attr("SIGNALFLOW_NODE_BUFFER_SIZE") = SIGNALFLOW_NODE_BUFFER_SIZE;
    m.attr("SIGNALFLOW_DEFAULT_TRIGGER") = SIGNALFLOW_DEFAULT_TRIGGER;
}
