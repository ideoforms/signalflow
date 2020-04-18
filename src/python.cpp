#include <pybind11/pybind11.h>

#include "signal/node.h"
#include "signal/signal.h"

using namespace libsignal;

namespace py = pybind11;

PYBIND11_DECLARE_HOLDER_TYPE(T, NodeRefTemplate<T>);

PYBIND11_MODULE(libsignal, m)
{
    py::class_<Node, NodeRefTemplate<Node>>(m, "Node")
        .def("__add__", [](NodeRef a, NodeRef b)
            { return NodeRef(new Add(a, b)); })
        .def("__add__", [](NodeRef  a, float b)
            { return NodeRef(new Add(a, NodeRef(new Constant(b)))); })
        .def("__radd__", [](NodeRefTemplate<Node>a, float b)
            { return NodeRef(new Add(a, NodeRef(new Constant(b)))); });

    py::class_<Constant, Node, NodeRefTemplate<Constant>>(m, "Constant")
        .def(py::init<double>());

    py::class_<Sine, Node, NodeRefTemplate<Sine>>(m, "Sine")
        .def(py::init<NodeRef>(), py::arg("frequency") = NodeRef(440.0))
        .def(py::init<float>(), py::arg("frequency") = NodeRef(440.0));

    py::class_<AudioGraph>(m, "AudioGraph")
        .def(py::init<>())
        .def("start", &AudioGraph::start)
        .def("add_output", [](AudioGraph &graph, NodeRef a) { graph.add_output(a); });

    py::class_<Add, Node, NodeRefTemplate<Add>>(m, "Add")
        .def(py::init<NodeRef, NodeRef>(),
            py::arg("a") = NodeRef(0.0),
            py::arg("b") = NodeRef(0.0));
}
