#include <pybind11/pybind11.h>

#include "signal/node.h"
#include "signal/signal.h"

using namespace libsignal;

namespace py = pybind11;

/*------------------------------------------------------------------------------
 * Exposes the _a literal which is equivalent to py::arg
 * https://pybind11.readthedocs.io/en/stable/basics.html
 *----------------------------------------------------------------------------*/
using namespace pybind11::literals;

PYBIND11_DECLARE_HOLDER_TYPE(T, NodeRefTemplate<T>);

PYBIND11_MODULE(libsignal, m)
{
    py::class_<Node, NodeRefTemplate<Node>>(m, "Node")
        .def("__add__", [](NodeRef a, NodeRef b)
            { return NodeRef(new Add(a, b)); })
        .def("__add__", [](NodeRef a, float b)
            { return NodeRef(new Add(a, NodeRef(new Constant(b)))); })
        .def("__radd__", [](NodeRefTemplate<Node>a, float b)
            { return NodeRef(new Add(a, NodeRef(new Constant(b)))); })
        .def("__sub__", [](NodeRef a, NodeRef b)
            { return NodeRef(new Subtract(a, b)); })
        .def("__sub__", [](NodeRef a, float b)
            { return NodeRef(new Subtract(a, NodeRef(new Constant(b)))); })
        .def("__rsub__", [](NodeRefTemplate<Node>a, float b)
            { return NodeRef(new Subtract(a, NodeRef(new Constant(b)))); })
        .def("__mul__", [](NodeRef a, NodeRef b)
            { return NodeRef(new Multiply(a, b)); })
        .def("__mul__", [](NodeRef a, float b)
            { return NodeRef(new Multiply(a, NodeRef(new Constant(b)))); })
        .def("__rmul__", [](NodeRefTemplate<Node>a, float b)
            { return NodeRef(new Multiply(a, NodeRef(new Constant(b)))); });

    py::class_<Constant, Node, NodeRefTemplate<Constant>>(m, "Constant")
        .def(py::init<double>());

    py::class_<Sine, Node, NodeRefTemplate<Sine>>(m, "Sine")
        .def(py::init<NodeRef>(), py::arg("frequency") = NodeRef(440.0))
        .def(py::init<float>(), py::arg("frequency") = NodeRef(440.0));

    py::class_<Square, Node, NodeRefTemplate<Square>>(m, "Square")
        .def(py::init<NodeRef, NodeRef>(), "frequency"_a = NodeRef(440.0), "width"_a = NodeRef(0.5))
        .def(py::init<float, float>(), "frequency"_a = 440.0, "width"_a = 0.5);

    py::class_<Pan, Node, NodeRefTemplate<Pan>>(m, "Pan")
        .def(py::init<int, NodeRef, NodeRef>(), py::arg("channels") = 2, py::arg("input") = NodeRef(440.0), py::arg("pan") = NodeRef(0))
        .def(py::init<int, NodeRef, float>(), py::arg("channels") = 2, py::arg("input") = NodeRef(440.0), py::arg("pan") = NodeRef(0));

    py::class_<ASR, Node, NodeRefTemplate<ASR>>(m, "ASR")
        .def(py::init<NodeRef, NodeRef, NodeRef, NodeRef>(), py::arg("attack") = NodeRef(0.1), py::arg("sustain") = NodeRef(0.1), py::arg("release") = NodeRef(0.1), py::arg("clock") = NodeRef(0.0))
        .def(py::init<float, float, float, float>(), "attack"_a = 0.1, "sustain"_a = 0.1, "release"_a = 0.1, "clock"_a=0);

    py::class_<AudioGraph>(m, "AudioGraph")
        .def(py::init<>())
        .def("start", &AudioGraph::start)
        .def("add_output", [](AudioGraph &graph, NodeRef a) { graph.add_output(a); });

}
