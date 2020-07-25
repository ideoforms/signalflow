#include "signalflow/python/python.h"

void init_python_config(py::module &m)
{
    /*--------------------------------------------------------------------------------
     * Config
     *-------------------------------------------------------------------------------*/
    py::class_<SignalFlowConfig>(m, "SignalFlowConfig")
        .def(py::init<>())
        .def_property("sample_rate", &SignalFlowConfig::get_sample_rate, &SignalFlowConfig::set_sample_rate)
        .def_property("input_buffer_size", &SignalFlowConfig::get_input_buffer_size, &SignalFlowConfig::set_input_buffer_size)
        .def_property("output_buffer_size", &SignalFlowConfig::get_output_buffer_size, &SignalFlowConfig::set_output_buffer_size)
        .def_property("input_device_name", &SignalFlowConfig::get_input_device_name, &SignalFlowConfig::set_input_device_name)
        .def_property("output_device_name", &SignalFlowConfig::get_output_device_name, &SignalFlowConfig::set_output_device_name);
}
