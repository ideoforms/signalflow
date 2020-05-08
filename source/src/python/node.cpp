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
        .def_property_readonly("synth", &Node::get_synth)

        .def_readonly("num_output_channels", &Node::num_output_channels)
        .def_readonly("min_output_channels", &Node::min_output_channels)
        .def_readonly("max_output_channels", &Node::max_output_channels)
        .def_readonly("num_input_channels", &Node::num_input_channels)
        .def_readonly("min_input_channels", &Node::min_input_channels)
        .def_readonly("max_input_channels", &Node::max_input_channels)

        .def("set_buffer", &Node::set_buffer)

        .def_property_readonly("inputs", [](Node& node) {
            std::unordered_map<std::string, NodeRef>inputs(node.inputs.size());
            for (auto input : node.inputs)
            {
                inputs[input.first] = *(input.second);
            }
            return inputs;
        })
        .def("set_input", &Node::set_input)
        .def("get_input", &Node::get_input)
        .def("trigger", [](Node& node) { node.trigger(); })
        .def("process", [](Node& node, int num_frames) {
            node.process(num_frames);
            node.last_num_frames = num_frames;
        })
        .def("process", [](Node& node, Buffer &buffer) {
            node.process(buffer.data, buffer.num_frames);
            node.last_num_frames = buffer.num_frames;
        })
        .def_property_readonly("output_buffer", [](Node &node)
        {
            return py::array_t<float>(
                { SIGNAL_MAX_CHANNELS, node.last_num_frames },
                { sizeof(float) * node.get_output_buffer_length(), sizeof(float) },
                node.out[0]);
        });

    py::implicitly_convertible<int, Node>();
    py::implicitly_convertible<float, Node>();
    py::implicitly_convertible<py::list, Node>();
}
