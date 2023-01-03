#include "pybind11/operators.h"
#include "signalflow/python/python.h"

/*--------------------------------------------------------------------------------
 * Required for hash() - see below.
 *-------------------------------------------------------------------------------*/
template <>
struct std::hash<Node>
{
    std::size_t operator()(Node const &a) const noexcept
    {
        return (std::size_t) &a;
    }
};

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
        .def(py::init<>([](PatchRef patch) { return patch->get_output(); }))

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
        .def("__mod__", [](NodeRef a, NodeRef b) { return new Modulo(a, b); })
        .def("__getitem__", [](NodeRef a, int index) { return new ChannelSelect(a, index); })
        .def("__getitem__", [](NodeRef a, py::slice slice) {
            ssize_t start, stop, step, slicelength;
            if (!slice.compute(a->get_num_output_channels(), &start, &stop, &step, &slicelength))
            {
                throw py::error_already_set();
            }
            return new ChannelSelect(a, start, stop, step);
        })
        /*--------------------------------------------------------------------------------
         * Explicitly handle the case in which an input is assigned a float value.
         * Under the bonnet, SignalFlow then checks whether the input is already
         * a `Constant` object; if so, it updates the value of this object, rather
         * than destroying the Constant and creating a new input.
         *-------------------------------------------------------------------------------*/
        .def("__setattr__", [](NodeRef a, std::string attr, float value) { a->set_input(attr, value); })
        .def("__setattr__", [](NodeRef a, std::string attr, NodeRef value) {
            if (value == nullptr)
            {
                throw std::runtime_error("Node: Cannot set an input to null");
            }
            else
            {
                a->set_input(attr, value);
            }
        })
        .def("__getattr__", [](NodeRef a, std::string attr) {
            /*--------------------------------------------------------------------------------
             * For double-underscored attributes, throw a correct attribute error.
             *-------------------------------------------------------------------------------*/
            if (attr.length() > 2 && attr.substr(0, 2) == "__")
            {
                throw py::attribute_error("No such attribute: " + attr);
            }
            else
            {
                return a->get_input(attr);
            }
        })

        /*--------------------------------------------------------------------------------
         * Need to define an explicit hashing function because when we overwrite __eq__,
         * pybind11 sets __hash__ to None, for compliance with standard Python behaviour,
         * with reasoning explained here:
         * https://github.com/pybind/pybind11/issues/2191
         *
         * In this case, we set Node's hash to be equal to its memory address.
         * See struct std::hash<Node> at top of file.
         *-------------------------------------------------------------------------------*/
        .def(hash(py::self))

        .def("__eq__", [](NodeRef a, NodeRef b) { return new Equal(a, b); })
        .def("__ne__", [](NodeRef a, NodeRef b) { return new NotEqual(a, b); })
        .def("__gt__", [](NodeRef a, NodeRef b) { return new GreaterThan(a, b); })
        .def("__ge__", [](NodeRef a, NodeRef b) { return new GreaterThanOrEqual(a, b); })
        .def("__lt__", [](NodeRef a, NodeRef b) { return new LessThan(a, b); })
        .def("__le__", [](NodeRef a, NodeRef b) { return new LessThanOrEqual(a, b); })

        /*--------------------------------------------------------------------------------
         * Properties
         *-------------------------------------------------------------------------------*/
        .def_property_readonly("name", &Node::get_name)
        .def_property_readonly("num_output_channels", &Node::get_num_output_channels)
        .def_property_readonly("num_input_channels", &Node::get_num_input_channels)
        .def_property_readonly("num_output_channels_allocated", &Node::get_num_output_channels_allocated)
        .def_property_readonly("matches_input_channels", &Node::get_matches_input_channels)
        .def_property_readonly("has_variable_inputs", &Node::get_has_variable_inputs)
        .def_property_readonly("patch", &Node::get_patch)
        .def_property_readonly("state", &Node::get_state)
        .def_property_readonly("value", &Node::get_value)
        .def_property_readonly("inputs", [](Node &node) {
            std::unordered_map<std::string, NodeRef> inputs(node.get_inputs().size());
            for (auto input : node.get_inputs())
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
        .def_property_readonly("output_buffer_length", &Node::get_output_buffer_length)

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
        .def("get_property", [](Node &node, std::string name) -> py::object {
            PropertyRef property = node.get_property(name);
            switch (property->get_property_type())
            {
                case SIGNALFLOW_PROPERTY_TYPE_INT:
                    return py::cast(property->int_value());
                case SIGNALFLOW_PROPERTY_TYPE_FLOAT:
                    return py::cast(property->float_value());
                case SIGNALFLOW_PROPERTY_TYPE_FLOAT_ARRAY:
                    return py::cast(property->float_array_value());
                case SIGNALFLOW_PROPERTY_TYPE_STRING:
                    return py::cast(property->string_value());
                case SIGNALFLOW_PROPERTY_TYPE_STRING_ARRAY:
                    return py::cast(property->string_array_value());
                default:
                    return py::none();
            }
        })
        .def("add_input", &Node::add_input)
        .def("remove_input", &Node::remove_input)
        .def("trigger", [](Node &node) { node.trigger(); })
        .def("trigger", [](Node &node, std::string name) { node.trigger(name); })
        .def("trigger", [](Node &node, std::string name, float value) { node.trigger(name, value); })
        .def("process", [](Node &node, int num_frames) {
            node.process(num_frames);
            node.last_num_frames = num_frames;
        })
        .def("process", [](Node &node, Buffer &buffer) {
            if ((unsigned int) node.get_num_output_channels() != buffer.get_num_channels())
            {
                throw std::runtime_error("Buffer and Node output channels don't match");
            }
            node.process(buffer, buffer.get_num_frames());
            node.last_num_frames = buffer.get_num_frames();
        })
        .def("scale", [](NodeRef node, float from, float to) { return node.scale(from, to); })
        .def("scale", [](NodeRef node, float from, float to, signalflow_scale_t scale) {
            return node.scale(from, to, scale);
        })

        /*--------------------------------------------------------------------------------
         * Convenience methods to play/stop a Node.
         * These are not available in the C++ interface because a Node does not
         * have access to its NodeRef container.
         *-------------------------------------------------------------------------------*/
        .def("play", [](NodeRef node) { node->get_graph()->play(node); })
        .def("stop", [](NodeRef node) { node->get_graph()->stop(node); });

    /*--------------------------------------------------------------------------------
     * StochasticNode
     *-------------------------------------------------------------------------------*/
    py::class_<StochasticNode, Node, NodeRefTemplate<StochasticNode>>(m, "StochasticNode")
        .def("set_seed", &StochasticNode::set_seed);

    /*--------------------------------------------------------------------------------
     * Constants and enums
     *-------------------------------------------------------------------------------*/
    py::enum_<signalflow_node_state_t>(m, "signalflow_node_state_t", py::arithmetic(), "signalflow_node_state_t")
        .value("SIGNALFLOW_NODE_STATE_ACTIVE", SIGNALFLOW_NODE_STATE_ACTIVE, "Active")
        .value("SIGNALFLOW_NODE_STATE_STOPPED", SIGNALFLOW_NODE_STATE_STOPPED, "Stopped")
        .export_values();

    py::enum_<signalflow_filter_type_t>(m, "signalflow_filter_type_t", py::arithmetic(), "Filter type")
        .value("SIGNALFLOW_FILTER_TYPE_LOW_PASS", SIGNALFLOW_FILTER_TYPE_LOW_PASS, "Low-pass filter")
        .value("SIGNALFLOW_FILTER_TYPE_HIGH_PASS", SIGNALFLOW_FILTER_TYPE_HIGH_PASS, "High-pass filter")
        .value("SIGNALFLOW_FILTER_TYPE_BAND_PASS", SIGNALFLOW_FILTER_TYPE_BAND_PASS, "Band-pass filter")
        .value("SIGNALFLOW_FILTER_TYPE_NOTCH", SIGNALFLOW_FILTER_TYPE_NOTCH, "Notch filter")
        .value("SIGNALFLOW_FILTER_TYPE_PEAK", SIGNALFLOW_FILTER_TYPE_PEAK, "Peak filter")
        .value("SIGNALFLOW_FILTER_TYPE_LOW_SHELF", SIGNALFLOW_FILTER_TYPE_LOW_SHELF, "Low-shelf filter")
        .value("SIGNALFLOW_FILTER_TYPE_HIGH_SHELF", SIGNALFLOW_FILTER_TYPE_HIGH_SHELF, "High-shelf filter")
        .export_values();

    /*--------------------------------------------------------------------------------
     * implicitly_convertible() allows dynamic run-time casting from numeric to
     * Node types.
     *-------------------------------------------------------------------------------*/
    py::implicitly_convertible<int, Node>();
    py::implicitly_convertible<float, Node>();

    // python native lists
    py::implicitly_convertible<py::list, Node>();

    // numpy arrays
    py::implicitly_convertible<py::array, Node>();

    // allows patches to be used in place of nodes as Node inputs, using the
    // conversion constructor above:
    // .def(py::init<>([](PatchRef patch) { return patch->output; }))
    py::implicitly_convertible<Patch, Node>();

    py::class_<NodeRegistry>(m, "NodeRegistry")
        .def(py::init<>())
        .def("create", &NodeRegistry::create);

    // object-based panning helper classes
    py::class_<SpatialEnvironment, std::shared_ptr<SpatialEnvironment>>(m, "SpatialEnvironment")
        .def(py::init<>())
        .def("add_speaker", [](SpatialEnvironment &env, int channel, float x, float y) { env.add_speaker(channel, x, y); })
        .def("add_speaker", [](SpatialEnvironment &env, int channel, float x, float y, float z) { env.add_speaker(channel, x, y, z); });
    py::class_<SpatialSpeaker, std::shared_ptr<SpatialSpeaker>>(m, "SpatialSpeaker");

    // WORK IN PROGRESS
    /*
    py::class_<Property, PropertyRefTemplate<Property>>(m, "Property")
        .def(py::init<>([](float value) { return new FloatProperty(value); }))
        .def(py::init<>([](std::vector<float> value) { return new FloatArrayProperty(value); }));

    // TODO: These implicitly_convertibles may not be needed - may be safer just to
    //  force specific types at the C++ signature level. TBD.
    py::implicitly_convertible<float, Property>();
    py::implicitly_convertible<std::vector<float>, Property>();
     */
}
