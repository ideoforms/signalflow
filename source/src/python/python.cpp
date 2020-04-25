// clang-format off

#include "signal/python/python.h"

void init_python_nodes(py::module &m);
void init_python_buffer(py::module &m);
void init_python_graph(py::module &m);
void init_python_synth(py::module &m);

PYBIND11_MODULE(libsignal, m)
{
    /*--------------------------------------------------------------------------------
     * Node
     *-------------------------------------------------------------------------------*/
    py::class_<Node, NodeRefTemplate<Node>>(m, "Node")
        .def("__add__", [](NodeRef a, NodeRef b)
            { return a + b; })
        .def("__add__", [](NodeRef a, float b)
            { return a + NodeRef(b); })
        .def("__radd__", [](NodeRefTemplate<Node>a, float b)
            { return a + NodeRef(b); })
        .def("__sub__", [](NodeRef a, NodeRef b)
            { return a - b; })
        .def("__sub__", [](NodeRef a, float b)
            { return a - NodeRef(b); })
        .def("__rsub__", [](NodeRefTemplate<Node>a, float b)
            { return a - NodeRef(b); })
        .def("__mul__", [](NodeRef a, NodeRef b)
            { return a * b; })
        .def("__mul__", [](NodeRef a, float b)
            { return a * NodeRef(b); })
        .def("__rmul__", [](NodeRefTemplate<Node>a, float b)
            { return a * NodeRef(b); })
        .def("__div__", [](NodeRef a, NodeRef b)
            { return a / b; })
        .def("__div__", [](NodeRef a, float b)
            { return a / NodeRef(b); })
        .def("__rdiv__", [](NodeRefTemplate<Node>a, float b)
            { return a / NodeRef(b); })
        .def_readonly("name", &Node::name);

    init_python_nodes(m);
    init_python_graph(m);
    init_python_buffer(m);
    init_python_synth(m);
}

// clang-format on
