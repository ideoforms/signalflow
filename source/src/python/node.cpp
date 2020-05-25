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
        .def(py::init<>([](std::vector<NodeRef> value) { return new ChannelArray(value); }))
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
        .def("__pow__", [](NodeRef a, NodeRef b)
            { return new Pow(a, b); })
        .def("__pow__", [](NodeRef a, float b)
            { return new Pow(a, b); })
        .def("__rpow__", [](NodeRef a, float b)
            { return new Pow(b, a); })
        .def("__sub__", [](NodeRef a, float b)
            { return new Pow(b, a); })
        .def("__getitem__", [](NodeRef a, int index)
            { return new ChannelSelect(a, index); })
        .def("__getitem__", [](NodeRef a, py::slice slice)
            {
                ssize_t start, stop, step, slicelength;
                if (!slice.compute(a->num_output_channels, &start, &stop, &step, &slicelength))
                {
                    throw py::error_already_set();
                }
                return new ChannelSelect(a, start, stop, step);
            })
        .def_readonly("name", &Node::name)
        .def_property_readonly("patch", &Node::get_patch)

        .def_readonly("num_output_channels", &Node::num_output_channels)
        .def_readonly("min_output_channels", &Node::min_output_channels)
        .def_readonly("max_output_channels", &Node::max_output_channels)
        .def_readonly("num_input_channels", &Node::num_input_channels)
        .def_readonly("min_input_channels", &Node::min_input_channels)
        .def_readonly("max_input_channels", &Node::max_input_channels)

        .def("set_buffer", &Node::set_buffer)
        .def("poll", [](Node &node, float frequency = 1.0) { node.poll(frequency); })
        .def("poll", [](Node &node, float frequency = 1.0, std::string label = "") { node.poll(frequency, label); })

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
        .def("trigger", [](Node& node, std::string name) { node.trigger(name); })
        .def("trigger", [](Node& node, std::string name, float value) { node.trigger(name, value); })
        .def("process", [](Node& node, int num_frames) {
            node.process(num_frames);
            node.last_num_frames = num_frames;
        })
        .def("process", [](Node& node, Buffer &buffer)
        {
            if (node.num_output_channels != buffer.num_channels)
            {
                throw std::runtime_error("Buffer and Node output channels don't match");
            }
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

    // python native lists
    py::implicitly_convertible<py::list, Node>();

    // numpy arrays
    py::implicitly_convertible<py::array, Node>();
}
