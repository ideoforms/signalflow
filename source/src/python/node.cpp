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
    py::class_<Node, NodeRefTemplate<Node>>(m, "Node", "A single signal-processing unit in the DSP graph, a node generates audio samples, optionally acting on zero or more input nodes.")
        /*--------------------------------------------------------------------------------
         * Critical to enable pybind11 to automatically convert from floats
         * to Node objects.
         *-------------------------------------------------------------------------------*/
        .def(py::init<>([]() { return new Node(); }))
        .def(py::init<>([](int value) { return new Constant(value); }), "value"_a,
             R"pbdoc(Creates a new Constant node which outputs a value of `value`.)pbdoc")
        .def(py::init<>([](float value) { return new Constant(value); }), "value"_a,
             R"pbdoc(Creates a new Constant node which outputs a value of `value`.)pbdoc")
        .def(py::init<>([](std::vector<NodeRef> value) { return new ChannelArray(value); }))
        .def(py::init<>([](PatchRef patch) { return patch->get_output(); }))

        /*--------------------------------------------------------------------------------
         * Operators
         *-------------------------------------------------------------------------------*/
        .def(
            "__add__", [](NodeRef a, NodeRef b) { return a + b; }, "other"_a,
            R"pbdoc(Returns a new Node whose output is equal to `self` + `other`)pbdoc")
        .def(
            "__add__", [](NodeRef a, float b) { return a + NodeRef(b); }, "value"_a,
            R"pbdoc(Returns a new Node whose output is equal to `self` + `value`.)pbdoc")
        .def(
            "__radd__", [](NodeRef a, float b) { return NodeRef(b) + a; }, "value"_a,
            R"pbdoc(Returns a new Node whose output is equal to `self` + `value`)pbdoc")

        .def(
            "__sub__", [](NodeRef a, NodeRef b) { return a - b; }, "other"_a,
            R"pbdoc(Returns a new Node whose output is equal to `self` - `other`)pbdoc")
        .def(
            "__sub__", [](NodeRef a, float b) { return a - NodeRef(b); }, "value"_a,
            R"pbdoc(Returns a new Node whose output is equal to `self` - `value`)pbdoc")
        .def(
            "__rsub__", [](NodeRef a, float b) { return NodeRef(b) - a; }, "value"_a,
            R"pbdoc(Returns a new Node whose output is equal to `self` - `value`)pbdoc")

        .def(
            "__mul__", [](NodeRef a, NodeRef b) { return a * b; }, "other"_a,
            R"pbdoc(Returns a new Node whose output is equal to `self` * `other`)pbdoc")
        .def(
            "__mul__", [](NodeRef a, float b) { return a * NodeRef(b); }, "value"_a,
            R"pbdoc(Returns a new Node whose output is equal to `self` * `value`)pbdoc")
        .def(
            "__rmul__", [](NodeRef a, float b) { return NodeRef(b) * a; }, "value"_a,
            R"pbdoc(Returns a new Node whose output is equal to `self` * `value`)pbdoc")

        .def(
            "__truediv__", [](NodeRef a, NodeRef b) { return a / b; }, "other"_a,
            R"pbdoc(Returns a new Node whose output is equal to `self` * `other`)pbdoc")
        .def(
            "__truediv__", [](NodeRef a, float b) { return a / NodeRef(b); }, "value"_a,
            R"pbdoc(Returns a new Node whose output is equal to `self` * `other`)pbdoc")
        .def(
            "__rtruediv__", [](NodeRef a, float b) { return NodeRef(b) / a; }, "other"_a,
            R"pbdoc(Returns a new Node whose output is equal to `self` * `other`)pbdoc")

        .def(
            "__pow__", [](NodeRef a, NodeRef b) { return new Pow(a, b); }, "other"_a,
            R"pbdoc(Returns a new Node whose output is equal to `self` ** `other`)pbdoc")
        .def(
            "__pow__", [](NodeRef a, float b) { return new Pow(a, b); }, "value"_a,
            R"pbdoc(Returns a new Node whose output is equal to `self` ** `value`)pbdoc")
        .def(
            "__rpow__", [](NodeRef a, float b) { return new Pow(b, a); }, "value"_a,
            R"pbdoc(Returns a new Node whose output is equal to `self` ** `value`)pbdoc")

        .def(
            "__mod__", [](NodeRef a, NodeRef b) { return new Modulo(a, b); }, "other"_a,
            R"pbdoc(Returns a new Node whose output is equal to `self` % `other`)pbdoc")

        .def(
            "__getitem__", [](NodeRef a, int index) { return new ChannelSelect(a, index); }, "index"_a,
            R"pbdoc(Returns a new Node whose output is equal to the `index`'th channel of `self`)pbdoc")
        .def(
            "__getitem__", [](NodeRef a, py::slice slice) {
                py::ssize_t start, stop, step, slicelength;
                if (!slice.compute(a->get_num_output_channels(), &start, &stop, &step, &slicelength))
                {
                    throw py::error_already_set();
                }
                return new ChannelSelect(a, start, stop, step);
            },
            "slice"_a, R"pbdoc(Returns a new Node whose output is equal to the channels of `self` denoted by `slice`)pbdoc")
        /*--------------------------------------------------------------------------------
         * Explicitly handle the case in which an input is assigned a float value.
         * Under the bonnet, SignalFlow then checks whether the input is already
         * a `Constant` object; if so, it updates the value of this object, rather
         * than destroying the Constant and creating a new input.
         *-------------------------------------------------------------------------------*/
        .def(
            "__setattr__", [](NodeRef a, std::string attr, float value) { a->set_input(attr, value); }, "input_name"_a, "value"_a, R"pbdoc(Set the Node's input `input_name` to value `value`)pbdoc")
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
        .def(
            "__getattr__", [](NodeRef a, std::string attr) {
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
            },
            "input_name"_a, R"pbdoc(Get the value of the input named `input_name`)pbdoc")

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

        .def(
            "__eq__", [](NodeRef a, NodeRef b) { return new Equal(a, b); }, "other"_a, R"pbdoc(Returns a new Node whose output is equal to `self` == `other`)pbdoc")
        .def(
            "__ne__", [](NodeRef a, NodeRef b) { return new NotEqual(a, b); }, "other"_a, R"pbdoc(Returns a new Node whose output is equal to `self` != `other`)pbdoc")
        .def(
            "__gt__", [](NodeRef a, NodeRef b) { return new GreaterThan(a, b); }, "other"_a, R"pbdoc(Returns a new Node whose output is equal to `self` > `other`)pbdoc")
        .def(
            "__ge__", [](NodeRef a, NodeRef b) { return new GreaterThanOrEqual(a, b); }, "other"_a, R"pbdoc(Returns a new Node whose output is equal to `self` >= `other`)pbdoc")
        .def(
            "__lt__", [](NodeRef a, NodeRef b) { return new LessThan(a, b); }, "other"_a, R"pbdoc(Returns a new Node whose output is equal to `self` < `other`)pbdoc")
        .def(
            "__le__", [](NodeRef a, NodeRef b) { return new LessThanOrEqual(a, b); }, "other"_a, R"pbdoc(Returns a new Node whose output is equal to `self` <= `other`)pbdoc")

        /*--------------------------------------------------------------------------------
         * Properties
         *-------------------------------------------------------------------------------*/
        .def_property_readonly("name", &Node::get_name, R"pbdoc(Short human-readable identifier for the node type (e.g. 'comb-delay'))pbdoc")
        .def_property_readonly("num_output_channels", &Node::get_num_output_channels, R"pbdoc(The number of output channels that the node will currently generate)pbdoc")
        .def_property_readonly("num_input_channels", &Node::get_num_input_channels, R"pbdoc(The number of input channels expected by the node)pbdoc")
        .def_property_readonly("num_output_channels_allocated", &Node::get_num_output_channels_allocated, R"pbdoc(The number of channels allocated in the output buffer)pbdoc")
        .def_property_readonly("matches_input_channels", &Node::get_matches_input_channels, R"pbdoc(If true, indicates that the node will output the same number of channels that it receives)pbdoc")
        .def_property_readonly("has_variable_inputs", &Node::get_has_variable_inputs, R"pbdoc(If true, indicates that the node can support an arbitrary number of inputs (e.g. 'Sum'))pbdoc")
        .def_property_readonly("patch", &Node::get_patch, R"pbdoc(Returns the patch that a node is part of, or None if the node is not part of a patch)pbdoc")
        .def_property_readonly("state", &Node::get_state, R"pbdoc(Returns the node's current playback state)pbdoc")
        .def_property("value", &Node::get_value, &Node::set_value, R"pbdoc(Get/set the node's current value. Only applicable to Constant nodes.)pbdoc")
        .def("get_value", &Node::get_value, R"pbdoc(Get the node's current value. Only applicable to Constant nodes.)pbdoc")
        .def("set_value", &Node::set_value, "value"_a, R"pbdoc(Set the node's current value. Only applicable to Constant nodes.)pbdoc")
        .def_property_readonly(
            "inputs", [](Node &node) {
                std::map<std::string, NodeRef> inputs;
                for (auto input : node.get_inputs())
                {
                    inputs[input.first] = *(input.second);
                }
                return inputs;
            },
            R"pbdoc(Returns a dict of the node's input names and values)pbdoc")
        .def_property_readonly(
            "outputs", [](Node &node) {
                std::vector<Node *> outputs(node.get_outputs().size());
                int index = 0;
                for (auto output : node.get_outputs())
                {
                    outputs[index++] = output.first;
                }
                return outputs;
            },
            R"pbdoc(Returns a list of the node's output connections)pbdoc")
        .def_property_readonly(
            "output_buffer", [](Node &node) {
                /*--------------------------------------------------------------------------------
                 * Assigning a data owner to the array ensures that it is returned as a
                 * pointer to the original data, rather than a copy. This means that we can
                 * modify the contents of the output buffer in-place from Python if we want to.
                 * https://github.com/pybind/pybind11/issues/323
                 *
                 * TODO: Why not return the actual Buffer object?
                 *-------------------------------------------------------------------------------*/
                py::str dummy_data_owner;
                return py::array_t<float>(
                    { node.get_num_output_channels_allocated(), node.last_num_frames },
                    { sizeof(float) * node.get_output_buffer_length(), sizeof(float) },
                    node.out[0],
                    dummy_data_owner);
            },
            R"pbdoc(Returns the node's current output buffer, as a numpy array)pbdoc")
        .def_property_readonly("output_buffer_length", &Node::get_output_buffer_length, R"pbdoc(The length of the node's output buffer, in frames)pbdoc")

        /*--------------------------------------------------------------------------------
         * Methods
         *-------------------------------------------------------------------------------*/
        .def("set_buffer", &Node::set_buffer, "string"_a, "buffer"_a, R"pbdoc(Set the value of a node's buffer input)pbdoc")
        .def(
            "poll", [](Node &node) { node.poll(); }, R"pbdoc(Print the node's last output value, once per second)pbdoc")
        .def(
            "poll", [](Node &node, float frequency) { node.poll(frequency); }, R"pbdoc(Print the node's last output value, at a frequency of `frequency`)pbdoc")
        .def(
            "poll", [](Node &node, float frequency, std::string label) { node.poll(frequency, label); }, R"pbdoc(Print the node's last output value, at a frequency of `frequency`, labelled with `label`)pbdoc")
        .def(
            "set_input", [](Node &node, std::string name, float value) { node.set_input(name, value); }, "name"_a, "value"_a, R"pbdoc(Set the input named `name` to `value`)pbdoc")
        .def(
            "set_input", [](Node &node, std::string name, NodeRef noderef) { node.set_input(name, noderef); }, "name"_a, "node"_a, R"pbdoc(Set the input named `name` to node `node`)pbdoc")
        .def("get_input", &Node::get_input, "name"_a, R"pbdoc(Get the value of the input named `name`)pbdoc")
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
        .def("add_input", &Node::add_input, "name"_a, R"pbdoc(When instantiating a new Node object, specifies the name of a new audio-rate input)pbdoc")
        .def("remove_input", &Node::remove_input, "name"_a, R"pbdoc(Removes a named input from a Node object. Generally only applies to variable-input nodes.)pbdoc")
        .def(
            "trigger", [](Node &node) { node.trigger(); }, R"pbdoc(Triggers the default trigger of a node)pbdoc")
        .def(
            "trigger", [](Node &node, std::string name) { node.trigger(name); }, "name"_a, R"pbdoc(Triggers the trigger named `name`)pbdoc")
        .def(
            "trigger", [](Node &node, std::string name, float value) { node.trigger(name, value); }, "name"_a, "value"_a, R"pbdoc(Triggers the trigger named `name` with value `value`)pbdoc")
        .def(
            "process", [](Node &node, int num_frames) {
                node.process(num_frames);
                node.last_num_frames = num_frames;
            },
            "num_frames"_a, R"pbdoc(Generates `num_frames` of output)pbdoc")
        .def(
            "process", [](Node &node, Buffer &buffer) {
                if ((unsigned int) node.get_num_output_channels() != buffer.get_num_channels())
                {
                    throw std::runtime_error("Buffer and Node output channels don't match (buffer num_channels = " + std::to_string(buffer.get_num_channels()) + ", node num_output_channels = " + std::to_string(node.get_num_output_channels()) + ")");
                }
                node.process(buffer, buffer.get_num_frames());
                node.last_num_frames = buffer.get_num_frames();
            },
            "buffer"_a, R"pbdoc(Generates output to fill buffer `buffer`)pbdoc")
        .def(
            "scale", [](NodeRef node, float from, float to) { return node.scale(from, to); }, "from"_a, "to"_a, R"pbdoc(Linearly scales the output of the node to the range `from` ... `to`)pbdoc")
        .def(
            "scale", [](NodeRef node, float from, float to, signalflow_scale_t scale) {
                return node.scale(from, to, scale);
            },
            "from"_a, "to"_a, "scale"_a, R"pbdoc(Linearly scales the output of the node to the range `from` ... `to`, with scale `scale`)pbdoc")

        /*--------------------------------------------------------------------------------
         * Convenience methods to play/stop a Node.
         * These are not available in the C++ interface because a Node does not
         * have access to its NodeRef container.
         *-------------------------------------------------------------------------------*/
        .def(
            "play", [](NodeRef node) { node->get_graph()->play(node); }, R"pbdoc(Begin playing the node by connecting it to the graph's output)pbdoc")
        .def_property_readonly(
            "is_playing", [](NodeRef node) { return node->get_graph()->is_playing(node); }, R"pbdoc(Query whether the node is currently playing)pbdoc")
        .def(
            "stop", [](NodeRef node) { node->get_graph()->stop(node); }, R"pbdoc(Stop playing the node by disconnecting it from the graph's output)pbdoc")
        .def_property_readonly("graph", &Node::get_graph, R"pbdoc(The AudioGraph that the node is part of.)pbdoc");

    /*--------------------------------------------------------------------------------
     * StochasticNode
     *-------------------------------------------------------------------------------*/
    py::class_<StochasticNode, Node, NodeRefTemplate<StochasticNode>>(m, "StochasticNode")
        .def("set_seed", &StochasticNode::set_seed);

    /*--------------------------------------------------------------------------------
     * FFTNode
     *-------------------------------------------------------------------------------*/
    py::class_<FFTNode, Node, NodeRefTemplate<FFTNode>>(m, "FFTNode")
        .def("get_magnitudes", &FFTNode::get_magnitudes)
        .def("get_phases", &FFTNode::get_phases);
    py::class_<FFTOpNode, FFTNode, NodeRefTemplate<FFTOpNode>>(m, "FFTOpNode");

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
