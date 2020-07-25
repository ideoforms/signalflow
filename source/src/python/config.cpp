#include "signalflow/python/python.h"

void init_python_config(py::module &m)
{
    /*--------------------------------------------------------------------------------
     * Config
     *-------------------------------------------------------------------------------*/
    py::class_<SignalFlowConfig>(m, "SignalFlowConfig")
        .def(py::init<>())
        .def_property_readonly("sample_rate", &SignalFlowConfig::get_sample_rate);
}
