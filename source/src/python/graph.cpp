#include "signal/python/python.h"

void init_python_graph(py::module &m)
{
    /*--------------------------------------------------------------------------------
     * Graph
     *-------------------------------------------------------------------------------*/
    py::class_<AudioGraph>(m, "AudioGraph")
        .def(py::init<>())
        .def("start", &AudioGraph::start)
        .def_readonly("sample_rate", &AudioGraph::sample_rate)
        .def("wait", [](AudioGraph &graph)
        {
            // Interruptible wait
            // https://pybind11.readthedocs.io/en/stable/faq.html#how-can-i-properly-handle-ctrl-c-in-long-running-functions
            for (;;) {
                if (PyErr_CheckSignals() != 0)
                    throw py::error_already_set();
            }
        })
        .def("add_output", [](AudioGraph &graph, NodeRef a) { graph.add_output(a); });
}
