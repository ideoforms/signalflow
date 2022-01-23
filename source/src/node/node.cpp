#include "signalflow/node/node.h"

#include "signalflow/node/operators/add.h"
#include "signalflow/node/operators/divide.h"
#include "signalflow/node/operators/multiply.h"
#include "signalflow/node/operators/scale.h"
#include "signalflow/node/operators/subtract.h"

#include "signalflow/node/operators/channel-array.h"
#include "signalflow/node/oscillators/constant.h"

#include "signalflow/core/core.h"
#include "signalflow/core/exceptions.h"
#include "signalflow/core/graph.h"
#include "signalflow/node/node-monitor.h"

namespace signalflow
{

extern AudioGraph *shared_graph;

Node::Node()
{
    this->name = "(unknown node)";
    this->graph = shared_graph;
    this->state = SIGNALFLOW_NODE_STATE_ACTIVE;

    this->matches_input_channels = true;
    this->num_input_channels = 1;
    this->num_output_channels = 1;

    this->no_input_upmix = false;

    this->monitor = nullptr;
    this->patch = nullptr;

    this->has_rendered = false;
    this->num_output_channels_allocated = 0;

    /*------------------------------------------------------------------------
     * In most cases, shared_graph->get_output_buffer_size() should always
     * be non-zero. However, there are edge cases when it is zero
     * (e.g. when creating an AudioOut before the graph has been instantiated).
     *
     *-----------------------------------------------------------------------*/
    if (shared_graph && shared_graph->get_config().get_output_buffer_size())
    {
        this->output_buffer_length = shared_graph->get_config().get_output_buffer_size();
    }
    else
    {
        this->output_buffer_length = SIGNALFLOW_NODE_BUFFER_SIZE;
    }

    this->resize_output_buffers(SIGNALFLOW_NODE_INITIAL_OUTPUT_BUFFERS);

    /*------------------------------------------------------------------------
     * last_num_frames caches the number of frames generated in the last
     * audio I/O cycle. Initialise it to be equal to the full buffer size,
     * so that if it is read before an audio I/O cycle has completed,
     * we avoid errors when trying to access elements of a buffer that is
     * 0 samples long (particularly in the Python bindings).
     *-----------------------------------------------------------------------*/
    this->last_num_frames = this->output_buffer_length;
}

Node::~Node()
{
}

////////////////////////////////////////////////////////////////////////////////
// Processing
////////////////////////////////////////////////////////////////////////////////

/*------------------------------------------------------------------------
 * Called directly by AudioGraph, this function wraps around the
 * subclassed process() method and handles extra bits of magic,
 * including populating the buffer history so that the value of the
 * previous frame can be checked.
 *-----------------------------------------------------------------------*/
void Node::_process(Buffer &out, int num_frames)
{
    if (&out == &this->out && num_frames > this->output_buffer_length)
    {
        throw std::runtime_error("Node " + this->name + " cannot render because output buffer size is insufficient (" + std::to_string(num_frames) + " samples requested, buffer size = " + std::to_string(this->output_buffer_length) + "). Increase the buffer size.");
    }

    /*--------------------------------------------------------------------------------
     * Must use `num_output_channels_allocated`, rather than `num_output_channels`,
     * as the additional allocated channels may be used for upmixing, and otherwise
     * the last-sample-magic fails for upmixed channels.
     *-------------------------------------------------------------------------------*/
    for (int i = 0; i < this->num_output_channels_allocated; i++)
    {
        this->last_sample[i] = out[i][last_num_frames - 1];
    }
    this->process(out, num_frames);
    this->last_num_frames = num_frames;
}

void Node::process(int num_frames)
{
    this->process(this->out, num_frames);
}

void Node::process(Buffer &out, int num_frames)
{
    /*------------------------------------------------------------------------
     * This should always be overridden by Node subclasses.
     * The Node class process() is a no-op, used rarely (for example,
     * when constructing unit tests when the Node's output samples need to
     * be specified directly by the Python code).
     *-----------------------------------------------------------------------*/
}

////////////////////////////////////////////////////////////////////////////////
// Channels
////////////////////////////////////////////////////////////////////////////////

void Node::set_channels(int num_input_channels, int num_output_channels)
{
    this->num_input_channels = num_input_channels;
    this->num_output_channels = num_output_channels;
    this->matches_input_channels = false;
}

void Node::update_channels()
{
    if (this->matches_input_channels)
    {
        int max_channels = 1;
        for (auto input : this->inputs)
        {
            NodeRef *ptr = input.second;
            // A param may be registered but not yet set
            if (!ptr || !*ptr)
                continue;
            std::string param_name = input.first;

            NodeRef input_node = *ptr;
            if (input_node->get_num_output_channels() > max_channels)
                max_channels = input_node->get_num_output_channels();
        }

        int previous_num_output_channels = this->num_output_channels;
        this->num_input_channels = max_channels;
        this->num_output_channels = max_channels;

        for (auto input : this->inputs)
        {
            NodeRef node = *input.second;
            if (node && node->get_num_output_channels_allocated() < this->num_output_channels)
            {
                node->resize_output_buffers(this->num_output_channels);
            }
        }

        if (previous_num_output_channels != this->num_output_channels)
        {
            for (auto output : this->outputs)
            {
                Node *node = output.first;
                node->update_channels();
            }
        }

        this->resize_output_buffers(this->num_output_channels);

        signalflow_debug("Node %s set num_out_channels to %d", this->name.c_str(), this->num_output_channels);
    }
    else
    {
        for (auto input : this->inputs)
        {
            NodeRef *ptr = input.second;
            // A param may be registered but not yet set
            if (!ptr || !*ptr)
                continue;
            std::string param_name = input.first;

            NodeRef input_node = *ptr;
            if (input_node->get_num_output_channels() > this->num_input_channels)
            {
                throw invalid_channel_count_exception("Node " + input_node->name + " has more output channels than " + this->name + " supports (" + std::to_string(input_node->get_num_output_channels()) + " > " + std::to_string(this->num_input_channels) + "). Either downmix with ChannelMixer, or select the intended channels with ChannelSelect.");
            }
        }
    }
}

int Node::get_num_input_channels()
{
    return this->num_input_channels;
}

int Node::get_num_output_channels()
{
    return this->num_output_channels;
}

int Node::get_num_output_channels_allocated()
{
    return this->num_output_channels_allocated;
}

////////////////////////////////////////////////////////////////////////////////
// Memory lifecycle
////////////////////////////////////////////////////////////////////////////////

void Node::alloc()
{
}

void Node::free()
{
}

void Node::resize_output_buffers(int output_buffer_count)
{
    if ((unsigned) output_buffer_count <= this->out.get_num_channels() && (unsigned) this->output_buffer_length <= this->out.get_num_frames())
    {
        /*------------------------------------------------------------------------
         * If enough channels and frames already allocated, don't do anything.
         *-----------------------------------------------------------------------*/
        return;
    }
    else
    {
        /*------------------------------------------------------------------------
         * If not enough channels/frames  are allocated, dealloc the current
         * allocations and resize
         *-----------------------------------------------------------------------*/
        this->free();
        this->out.resize(output_buffer_count, this->output_buffer_length);
        this->last_sample.resize(output_buffer_count);
        this->num_output_channels_allocated = output_buffer_count;
        this->alloc();
    }
}

int Node::get_output_buffer_length()
{
    return this->output_buffer_length;
}

////////////////////////////////////////////////////////////////////////////////
// States
////////////////////////////////////////////////////////////////////////////////

signalflow_node_state_t Node::get_state()
{
    return this->state;
}

void Node::set_state(signalflow_node_state_t state)
{
    if (state != this->state)
    {
        this->state = state;
        if (this->patch)
        {
            this->patch->node_state_changed(this);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
// Inputs and outputs
////////////////////////////////////////////////////////////////////////////////

void Node::create_input(std::string name, NodeRef &input)
{
    this->inputs[name] = &input;

    /*------------------------------------------------------------------------
     * Create a new named input.
     *-----------------------------------------------------------------------*/
    if (input)
    {
        input->add_output(this, name);

        /*------------------------------------------------------------------------
         * In case the input has more output channels than this Node is
         * currently able to accept, update our channel routing.
         *-----------------------------------------------------------------------*/
        this->update_channels();
    }
}

void Node::destroy_input(std::string name)
{
    /*------------------------------------------------------------------------
     * Only done by special classes (ChannelArray, AudioOut)
     *-----------------------------------------------------------------------*/
    this->inputs.erase(name);
    this->update_channels();
}

NodeRef Node::get_input(std::string name)
{
    if (this->inputs.find(name) == this->inputs.end())
    {
        throw std::runtime_error("Node " + this->name + " has no such input: " + name);
    }

    return *(this->inputs[name]);
}

void Node::set_input(std::string name, const NodeRef &node)
{
    if (this->inputs.find(name) == this->inputs.end())
    {
        throw std::runtime_error("Node " + this->name + " has no such input: " + name);
    }

    NodeRef current_input = *(this->inputs[name]);

    if (current_input)
    {
        current_input->remove_output(this, name);
    }

    *(this->inputs[name]) = node;
    this->update_channels();

    node->add_output(this, name);
}

void Node::set_input(std::string name, float value)
{
    if (this->inputs.find(name) == this->inputs.end())
    {
        throw std::runtime_error("Node " + this->name + " has no such input: " + name);
    }

    NodeRef current_input = *(this->inputs[name]);

    if (current_input && current_input->name == "constant")
    {
        Constant *constant = (Constant *) current_input.get();
        constant->value = value;
    }
    else
    {
        this->set_input(name, new Constant(value));
    }
}

void Node::add_input(NodeRef input)
{
    throw std::runtime_error("This Node class does not support unnamed inputs");
}

void Node::remove_input(NodeRef input)
{
    throw std::runtime_error("This Node class does not support unnamed inputs");
}

void Node::add_output(Node *target, std::string name)
{
    this->outputs.insert(std::make_pair(target, name));
}

void Node::remove_output(Node *target, std::string name)
{
    this->outputs.erase(std::make_pair(target, name));
}

void Node::disconnect_inputs()
{
    for (auto param : this->inputs)
    {
        this->set_input(param.first, 0);
    }
}

void Node::disconnect_outputs()
{
    // TODO: Can this method be removed?
    /*------------------------------------------------------------------------
     * Don't iterate over outputs as the output set will change during
     * iteration (as calling set_input on each output of the node will 
     * change our own `outputs` array). Instead, keep trying until all
     * outputs are removed.
     *-----------------------------------------------------------------------*/
    while (this->outputs.size() > 0)
    {
        auto output = *(this->outputs.begin());
        Node *target = output.first;
        std::string name = output.second;
        //        target->set_input(name, new Constant(0.0));

        // This is causing crashes
        if (target->has_variable_inputs)
        {
            target->remove_input(this);
        }
        else
        {
            target->set_input(name, new Constant(0.0));
        }
    }
}

bool Node::get_has_variable_inputs()
{
    return this->has_variable_inputs;
}

////////////////////////////////////////////////////////////////////////////////
// Properties
////////////////////////////////////////////////////////////////////////////////

void Node::create_property(std::string name, PropertyRef &value)
{
    this->properties[name] = &value;
}

void Node::set_property(std::string name, const PropertyRef &value)
{
    //    if (this->properties.find(name) == this->properties.end())
    //        throw std::runtime_error("Node " + this->name + " has no such property: " + name);

    *this->properties[name] = value;
}

PropertyRef Node::get_property(std::string name)
{
    if (this->properties.find(name) == this->properties.end())
        throw std::runtime_error("Node " + this->name + " has no such property: " + name);

    return *(this->properties[name]);
}

////////////////////////////////////////////////////////////////////////////////
// Buffers
////////////////////////////////////////////////////////////////////////////////

void Node::create_buffer(std::string name, BufferRef &buffer)
{
    this->buffers[name] = &buffer;
}

void Node::set_buffer(std::string name, BufferRef buffer)
{
    if (this->buffers.find(name) == this->buffers.end())
        throw std::runtime_error("Node " + this->name + " has no such buffer: " + name);

    *(this->buffers[name]) = buffer;
}

////////////////////////////////////////////////////////////////////////////////
// Patches
////////////////////////////////////////////////////////////////////////////////

Patch *Node::get_patch()
{
    return this->patch;
}

AudioGraph *Node::get_graph()
{
    return this->graph;
}

void Node::set_patch(Patch *patch)
{
    if (this->patch)
    {
        throw std::runtime_error("This Node is already part of a Patch");
    }
    this->patch = patch;
}

////////////////////////////////////////////////////////////////////////////////
// Miscellaneous
////////////////////////////////////////////////////////////////////////////////

void Node::trigger(std::string name, float value)
{
    throw std::runtime_error("Trigger " + name + " is not implemented in node class " + this->name);
}

void Node::poll(float frequency, std::string label)
{
    if (frequency > 0)
    {
        this->monitor = new NodeMonitor(this, label, frequency);
        this->monitor->start();
    }
    else
    {
        if (this->monitor)
        {
            this->monitor->stop();
            // TODO: Properly free monitor
        }
    }
}

float Node::get_value()
{
    throw std::runtime_error("get_value() is only applicable to Constant nodes");
}

///////////////////////////////////////////////////////////////////////////////

BinaryOpNode::BinaryOpNode(NodeRef a, NodeRef b)
    : Node(), input0(a), input1(b)
{
    this->create_input("input0", this->input0);
    this->create_input("input1", this->input1);
}

UnaryOpNode::UnaryOpNode(NodeRef a)
    : Node(), input(a)
{
    this->create_input("input", this->input);
}

////////////////////////////////////////////////////////////////////////////////
// NodeRef
////////////////////////////////////////////////////////////////////////////////

template <class T>
NodeRefTemplate<T>::NodeRefTemplate(double x)
    : std::shared_ptr<T>(new Constant(x)) {}

template <class T>
NodeRefTemplate<T>::NodeRefTemplate(int x)
    : std::shared_ptr<T>(new Constant((float) x)) {}

template <class T>
NodeRefTemplate<T>::NodeRefTemplate(std::initializer_list<NodeRefTemplate> x)
    : std::shared_ptr<T>(new ChannelArray(x)) {}

template <class T>
NodeRefTemplate<T>::NodeRefTemplate(std::vector<NodeRefTemplate> x)
    : std::shared_ptr<T>(new ChannelArray(x)) {}

template <class T>
NodeRefTemplate<T>::NodeRefTemplate(std::vector<float> x)
    : std::shared_ptr<T>(new ChannelArray(x)) {}

template <class T>
NodeRefTemplate<T> NodeRefTemplate<T>::operator*(NodeRefTemplate<T> other)
{
    return new Multiply(*this, other);
}

template <class T>
NodeRefTemplate<T> NodeRefTemplate<T>::operator*(double constant)
{
    return new Multiply(*this, constant);
}

template <class T>
NodeRefTemplate<T> operator*(double constant, const NodeRefTemplate<T> node)
{
    return new Multiply(node, constant);
}

template <class T>
NodeRefTemplate<T> NodeRefTemplate<T>::operator+(NodeRefTemplate<T> other)
{
    return new Add(*this, other);
}

template <class T>
NodeRefTemplate<T> NodeRefTemplate<T>::operator+(double constant)
{
    return new Add(*this, constant);
}

template <class T>
NodeRefTemplate<T> operator+(double constant, const NodeRefTemplate<T> node)
{
    return new Add(node, constant);
}

template <class T>
NodeRefTemplate<T> NodeRefTemplate<T>::operator-(NodeRefTemplate<T> other)
{
    return new Subtract(*this, other);
}

template <class T>
NodeRefTemplate<T> NodeRefTemplate<T>::operator-(double constant)
{
    return new Subtract(*this, constant);
}

template <class T>
NodeRefTemplate<T> operator-(double constant, const NodeRefTemplate<T> node)
{
    return new Subtract(node, constant);
}

template <class T>
NodeRefTemplate<T> NodeRefTemplate<T>::operator/(NodeRefTemplate<T> other)
{
    return new Divide(*this, other);
}

template <class T>
NodeRefTemplate<T> NodeRefTemplate<T>::operator/(double constant)
{
    return new Divide(*this, constant);
}

template <class T>
NodeRefTemplate<T> operator/(double constant, const NodeRefTemplate<T> node)
{
    return new Divide(node, constant);
}

template <class T>
sample NodeRefTemplate<T>::operator[](int index)
{
    // unused?
    return (*this)->out[0][index];
}

template <class T>
NodeRefTemplate<T> NodeRefTemplate<T>::scale(float from, float to, signalflow_scale_t scale)
{
    switch (scale)
    {
        case SIGNALFLOW_SCALE_LIN_LIN:
            return new ScaleLinLin(*this, -1, 1, from, to);
        case SIGNALFLOW_SCALE_LIN_EXP:
            return new ScaleLinExp(*this, -1, 1, from, to);
        default:
            return nullptr;
    }
}

// Explicitly instantiate the class
template class NodeRefTemplate<Node>;

} /* namespace signalflow */
