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
        .def("__radd__", [](NodeRef a, float b)
            { return NodeRef(b) + a; })
        .def("__sub__", [](NodeRef a, NodeRef b)
            { return a - b; })
        .def("__sub__", [](NodeRef a, float b)
            { return a - NodeRef(b); })
        .def("__rsub__", [](NodeRef a, float b)
            { return NodeRef(b) - a; })
        .def("__mul__", [](NodeRef a, NodeRef b)
            { return a * b; })
        .def("__mul__", [](NodeRef a, float b)
            { return a * NodeRef(b); })
        .def("__rmul__", [](NodeRef a, float b)
            { return NodeRef(b) * a; })
        .def("__truediv__", [](NodeRef a, NodeRef b)
            { return a / b; })
        .def("__truediv__", [](NodeRef a, float b)
            { return a / NodeRef(b); })
        .def("__rtruediv__", [](NodeRef a, float b)
            { return NodeRef(b) / a; })
        .def_readonly("name", &Node::name)
        .def_property_readonly("inputs", [](Node& node) {
            std::unordered_map<std::string, NodeRef>inputs(node.inputs.size());
            for (auto input : node.inputs)
            {
                inputs[input.first] = *(input.second);
            }
            return inputs;
        })
        .def("process", [](Node& node, int num_frames) { node.process(num_frames); })
        .def_property_readonly("output_buffer", [](Node &node)
        {
            return py::array_t<float>({ 1024 }, { sizeof(float) }, node.out[0]);
        });

    py::implicitly_convertible<int, Node>();
    py::implicitly_convertible<float, Node>();
    py::implicitly_convertible<py::list, Node>();
}
