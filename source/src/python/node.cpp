#include "signalflow/python/python.h"

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
        .def(py::init<>([]() { return new Node(); }))
        .def(py::init<>([](int value) { return new Constant(value); }))
        .def(py::init<>([](float value) { return new Constant(value); }))
        .def(py::init<>([](std::vector<NodeRef> value) { return new ChannelArray(value); }))

        /*--------------------------------------------------------------------------------
         * Operators
         *-------------------------------------------------------------------------------*/
        .def("__add__", [](NodeRef a, NodeRef b) { return a + b; })
        .def("__add__", [](NodeRef a, float b) { return a + NodeRef(b); })
        .def("__radd__", [](NodeRef a, float b) { return NodeRef(b) + a; })
        .def("__sub__", [](NodeRef a, NodeRef b) { return a - b; })
        .def("__sub__", [](NodeRef a, float b) { return a - NodeRef(b); })
        .def("__rsub__", [](NodeRef a, float b) { return NodeRef(b) - a; })
        .def("__mul__", [](NodeRef a, NodeRef b) { return a * b; })
        .def("__mul__", [](NodeRef a, float b) { return a * NodeRef(b); })
        .def("__rmul__", [](NodeRef a, float b) { return NodeRef(b) * a; })
        .def("__truediv__", [](NodeRef a, NodeRef b) { return a / b; })
        .def("__truediv__", [](NodeRef a, float b) { return a / NodeRef(b); })
        .def("__rtruediv__", [](NodeRef a, float b) { return NodeRef(b) / a; })
        .def("__pow__", [](NodeRef a, NodeRef b) { return new Pow(a, b); })
        .def("__pow__", [](NodeRef a, float b) { return new Pow(a, b); })
        .def("__rpow__", [](NodeRef a, float b) { return new Pow(b, a); })
        .def("__sub__", [](NodeRef a, float b) { return new Pow(b, a); })
        .def("__getitem__", [](NodeRef a, int index) { return new ChannelSelect(a, index); })
        .def("__getitem__", [](NodeRef a, py::slice slice) {
            ssize_t start, stop, step, slicelength;
            if (!slice.compute(a->get_num_output_channels(), &start, &stop, &step, &slicelength))
            {
                throw py::error_already_set();
            }
            return new ChannelSelect(a, start, stop, step);
        })
        .def("__setattr__", [](NodeRef a, std::string attr, NodeRef value) { a->set_input(attr, value); })
        .def("__getattr__", [](NodeRef a, std::string attr) { return a->get_input(attr); })

        /*--------------------------------------------------------------------------------
         * Properties
         *-------------------------------------------------------------------------------*/
        .def_readonly("name", &Node::name)
        .def_property_readonly("num_output_channels", &Node::get_num_output_channels)
        .def_property_readonly("num_input_channels", &Node::get_num_input_channels)
        .def_property_readonly("num_output_channels_allocated", &Node::get_num_output_channels_allocated)
        .def_property_readonly("patch", &Node::get_patch)
        .def_property_readonly("state", &Node::get_state)
        .def_property_readonly("inputs", [](Node &node) {
            std::unordered_map<std::string, NodeRef> inputs(node.inputs.size());
            for (auto input : node.inputs)
            {
                inputs[input.first] = *(input.second);
            }
            return inputs;
        })
        .def_property_readonly("output_buffer", [](Node &node) {
            /*--------------------------------------------------------------------------------
             * Assigning a data owner to the array ensures that it is returned as a
             * pointer to the original data, rather than a copy. This means that we can
             * modify the contents of the output buffer in-place from Python if we want to.
             * https://github.com/pybind/pybind11/issues/323
             *-------------------------------------------------------------------------------*/
            py::str dummy_data_owner;
            return py::array_t<float>(
                { node.get_num_output_channels_allocated(), node.last_num_frames },
                { sizeof(float) * node.get_output_buffer_length(), sizeof(float) },
                node.out[0],
                dummy_data_owner);
        })

        /*--------------------------------------------------------------------------------
         * Methods
         *-------------------------------------------------------------------------------*/
        .def("set_buffer", &Node::set_buffer)
        .def("poll", [](Node &node) { node.poll(); })
        .def("poll", [](Node &node, float frequency) { node.poll(frequency); })
        .def("poll", [](Node &node, float frequency, std::string label) { node.poll(frequency, label); })
        .def("set_input", [](Node &node, std::string name, float value) { node.set_input(name, value); })
        .def("set_input", [](Node &node, std::string name, NodeRef noderef) { node.set_input(name, noderef); })
        .def("get_input", &Node::get_input)
        .def("add_input", &Node::add_input)
        .def("trigger", [](Node &node) { node.trigger(); })
        .def("trigger", [](Node &node, std::string name) { node.trigger(name); })
        .def("trigger", [](Node &node, std::string name, float value) { node.trigger(name, value); })
        .def("process", [](Node &node, int num_frames) {
            node.process(num_frames);
            node.last_num_frames = num_frames;
        })
        .def("process", [](Node &node, Buffer &buffer) {
            if (node.get_num_output_channels() != buffer.get_num_channels())
            {
                throw std::runtime_error("Buffer and Node output channels don't match");
            }
            node.process(buffer, buffer.get_num_frames());
            node.last_num_frames = buffer.get_num_frames();
        });

    py::enum_<signalflow_filter_type_t>(m, "signalflow_filter_type_t", py::arithmetic(), "Filter type")
        .value("SIGNALFLOW_FILTER_TYPE_LOW_PASS", SIGNALFLOW_FILTER_TYPE_LOW_PASS, "Low-pass filter")
        .value("SIGNALFLOW_FILTER_TYPE_HIGH_PASS", SIGNALFLOW_FILTER_TYPE_HIGH_PASS, "High-pass filter")
        .value("SIGNALFLOW_FILTER_TYPE_BAND_PASS", SIGNALFLOW_FILTER_TYPE_BAND_PASS, "Band-pass filter")
        .value("SIGNALFLOW_FILTER_TYPE_NOTCH", SIGNALFLOW_FILTER_TYPE_NOTCH, "Notch filter")
        .value("SIGNALFLOW_FILTER_TYPE_PEAK", SIGNALFLOW_FILTER_TYPE_PEAK, "Peak filter")
        .value("SIGNALFLOW_FILTER_TYPE_LOW_SHELF", SIGNALFLOW_FILTER_TYPE_LOW_SHELF, "Low-shelf filter")
        .value("SIGNALFLOW_FILTER_TYPE_HIGH_SHELF", SIGNALFLOW_FILTER_TYPE_HIGH_SHELF, "High-shelf filter")
        .export_values();

    py::implicitly_convertible<int, Node>();
    py::implicitly_convertible<float, Node>();

    // python native lists
    py::implicitly_convertible<py::list, Node>();

    // numpy arrays
    py::implicitly_convertible<py::array, Node>();

    py::class_<NodeRegistry>(m, "NodeRegistry")
        .def(py::init<>())
        .def("create", &NodeRegistry::create);
}
