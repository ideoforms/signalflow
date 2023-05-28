#include "signalflow/python/python.h"

void init_python_config(py::module &m)
{
    /*--------------------------------------------------------------------------------
     * Config
     *-------------------------------------------------------------------------------*/
    py::class_<AudioGraphConfig>(m, "AudioGraphConfig")
        .def(py::init<>())
        .def_property("sample_rate", &AudioGraphConfig::get_sample_rate, &AudioGraphConfig::set_sample_rate)
        .def_property("input_buffer_size", &AudioGraphConfig::get_input_buffer_size, &AudioGraphConfig::set_input_buffer_size)
        .def_property("output_buffer_size", &AudioGraphConfig::get_output_buffer_size, &AudioGraphConfig::set_output_buffer_size)
        .def_property("input_device_name", &AudioGraphConfig::get_input_device_name, &AudioGraphConfig::set_input_device_name)
        .def_property("output_device_name", &AudioGraphConfig::get_output_device_name, &AudioGraphConfig::set_output_device_name)
        .def_property("output_backend_name", &AudioGraphConfig::get_output_backend_name, &AudioGraphConfig::set_output_backend_name)
        .def_property("cpu_usage_limit", &AudioGraphConfig::get_cpu_usage_limit, &AudioGraphConfig::set_cpu_usage_limit);
}
