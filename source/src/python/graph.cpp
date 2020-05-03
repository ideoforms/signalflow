#include "signal/python/python.h"

void init_python_graph(py::module &m)
{
    /*--------------------------------------------------------------------------------
     * Graph
     *-------------------------------------------------------------------------------*/
    py::class_<AudioGraph>(m, "AudioGraph")
        .def(py::init<>())
        .def("start", &AudioGraph::start)
        .def("print", [](AudioGraph &graph) { graph.print(); })
        .def("pull_input", [](AudioGraph &graph, int num_frames) { graph.pull_input(num_frames); })
        .def_readonly("sample_rate", &AudioGraph::sample_rate)
        .def_readonly("node_count", &AudioGraph::node_count)
        .def("wait", [](AudioGraph &graph)
        {
            // Interruptible wait
            // https://pybind11.readthedocs.io/en/stable/faq.html#how-can-i-properly-handle-ctrl-c-in-long-running-functions
            for (;;) {
                if (PyErr_CheckSignals() != 0)
                    throw py::error_already_set();
            }
        })
        .def("add_output", [](AudioGraph &graph, NodeRef a) { graph.add_output(a); })
        .def("remove_output", [](AudioGraph &graph, NodeRef a) { graph.remove_output(a); });
}
