#include <pybind11/pybind11.h>

#include "signal/node.h"
#include "signal/signal.h"

using namespace libsignal;

namespace py = pybind11;

PYBIND11_DECLARE_HOLDER_TYPE(T, NodeRefTemplate<T>);

PYBIND11_MODULE(thing, m)
{
    py::class_<Node, NodeRefTemplate<Node>>(m, "Node")
        .def("__add__", [](NodeRefTemplate<Node>a, NodeRefTemplate<Node>b)
            { return NodeRefTemplate<Node>(new Add(a, b)); })
        .def("__add__", [](NodeRefTemplate<Node>a, float b)
            { return NodeRefTemplate<Node>(new Add(a, NodeRefTemplate<Node>(new Constant(b)))); })
        .def("__radd__", [](NodeRefTemplate<Node>a, float b)
            { return NodeRefTemplate<Node>(new Add(a, NodeRefTemplate<Node>(new Constant(b)))); });
    py::class_<Constant, Node, NodeRefTemplate<Constant>>(m, "Constant")
        .def(py::init<double>());

    py::class_<Sine, Node, NodeRefTemplate<Sine>>(m, "Sine")
        .def(py::init<NodeRefTemplate<Node>>(),
            py::arg("frequency") = NodeRefTemplate<Node>(440.0));

    py::class_<Add, Node, NodeRefTemplate<Add>>(m, "Add")
        .def(py::init<NodeRefTemplate<Node>, NodeRefTemplate<Node>>(),
            py::arg("a") = NodeRefTemplate<Node>(0.0),
            py::arg("b") = NodeRefTemplate<Node>(0.0));
}
