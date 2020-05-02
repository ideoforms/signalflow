#include "signal/python/python.h"

void init_python_node(py::module &m)
{
    /*--------------------------------------------------------------------------------
     * Node
     *-------------------------------------------------------------------------------*/
    py::class_<Node, NodeRefTemplate<Node>>(m, "Node")
        /*--------------------------------------------------------------------------------
         * Critical to enable pybind11 to automatically convert from floats
         * to Node objects.
         *-------------------------------------------------------------------------------*/
        .def(py::init<>([](int value) { return new Constant(value); }))
        .def(py::init<>([](float value) { return new Constant(value); }))
        .def(py::init<>([](std::vector<NodeRef> value) { return new Multiplex(value); }))
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

    py::implicitly_convertible<int, Node>();
    py::implicitly_convertible<float, Node>();
    py::implicitly_convertible<py::list, Node>();
}
