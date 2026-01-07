#include <algorithm>

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

    this->resize_output_buffers(SIGNALFLOW_NODE_INITIAL_OUTPUT_CHANNELS);

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
    if (this->monitor)
    {
        this->monitor->stop();
    }
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
        throw insufficient_buffer_size_exception("Node " + this->name + " cannot render because output buffer size is insufficient (" + std::to_string(num_frames) + " samples requested, buffer size = " + std::to_string(this->output_buffer_length) + "). Increase the buffer size. More information: https://signalflow.dev/troubleshooting/insufficient_buffer_size_exception/");
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

void Node::set_channels(int num_input_channels, int num_output_channels, bool disable_input_channel_matching)
{
    int previous_num_output_channels = this->num_output_channels;
    this->num_input_channels = num_input_channels;
    this->num_output_channels = num_output_channels;

    if (disable_input_channel_matching)
    {
        this->matches_input_channels = false;
    }

    /*--------------------------------------------------------------------------------
     * Added 2024-08-05 to address crash in which a multichannel ChannelPanner
     * connected directly to AudioOut does not correctly allocate its buffers, as
     * resize_output_buffers() currently only gets called in the
     * `this->matches_input_channels` block in `update_channels()` (below).
     *
     * This means that resize_output_buffers() is called on every call to
     * set_channels(). However, resize_output_buffers() is a no-op if no resize
     * operation is needed, so this should have minimal impact on CPU.
     *--------------------------------------------------------------------------------*/
    if (num_output_channels > previous_num_output_channels)
    {
        signalflow_debug("Node %s increased num_out_channels to %d", this->name.c_str(), this->num_output_channels);

        this->resize_output_buffers(num_output_channels);

        for (auto output : this->outputs)
        {
            Node *node = output.first;
            node->update_channels();
        }
    }
}

void Node::update_channels()
{
    if (this->matches_input_channels)
    {
        /*--------------------------------------------------------------------------------
         * Initialise max_channels to be num_output_channels, rather than 1.
         * This is done because we only ever want to _increase_ the channel count when
         * matching input_channels. Otherwise, pathological behaviour can occur, such
         * as when stereo inputs are repeatedly added and removed from a Sum() node,
         * in which case num_input_channels will repeatedly drop down to 1 every time
         * the last node is removed.
         *
         * Is this the correct design decision? It could cause confusing behaviour in
         * other situations (in which the user explicitly wants the num_output_channels
         * to drop back down). The alternative would be to switch to using a Bus
         * metaphor when mixing varying numbers of inputs, and connected a fixed
         * 2-channel Bus to the Sum node. That approach would also be more CPU-efficient,
         * as it wouldn't require constantly calling update_channels on the Sum node.
         *-------------------------------------------------------------------------------*/
        int max_channels = this->num_output_channels;
        for (auto input : this->inputs)
        {
            NodeRef *ptr = input.second;
            // A param may be registered but not yet set
            if (!ptr || !*ptr)
                continue;

            NodeRef input_node = *ptr;
            if (input_node->get_num_output_channels() > max_channels)
                max_channels = input_node->get_num_output_channels();
        }

        /*--------------------------------------------------------------------------------
         * If the number of output channels has increased due to a change in an input's
         * channel count:
         *  - allocate more output buffers accordingly
         *  - iterate over inputs, ensuring each input has enough working buffers
         *    for inpux upmixing. This does not need to happen recursively, as inputs
         *    to inputs are unaffected by the change.
         *  - apply the change to each output node, propagating up the entire outbound
         *    tree recursively.
         *-------------------------------------------------------------------------------*/
        this->set_channels(max_channels, max_channels, false);

        for (auto input : this->inputs)
        {
            NodeRef node = *input.second;
            if (node && node->get_num_output_channels_allocated() < this->num_output_channels)
            {
                node->resize_output_buffers(this->num_output_channels);
            }
        }
    }
    else
    {
        for (auto input : this->inputs)
        {
            NodeRef *ptr = input.second;
            // A param may be registered but not yet set
            if (!ptr || !*ptr)
                continue;

            NodeRef input_node = *ptr;
            if (input_node->get_num_output_channels() > this->num_input_channels)
            {
                std::string message = "Node " + input_node->get_name() + " has more output channels than " + this->name + " supports (" + std::to_string(input_node->get_num_output_channels()) + " > " + std::to_string(this->num_input_channels) + "). Either downmix with ChannelMixer, or select the intended channels with ChannelSelect.";
                signalflow_audio_thread_error(message);
                return;
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
         * If not enough channels/frames are allocated, dealloc the current
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

std::string Node::get_name()
{
    return this->name;
}

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

const std::map<std::string, NodeRef *> &Node::get_inputs() const
{
    return this->inputs;
}

const std::vector<std::pair<Node *, std::string>> &Node::get_outputs() const
{
    return this->outputs;
}

const std::map<std::string, PropertyRef *> &Node::get_properties() const
{
    return this->properties;
}

const std::map<std::string, BufferRef *> &Node::get_buffers() const
{
    return this->buffers;
}

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
    this->outputs.push_back(std::make_pair(target, name));
}

void Node::remove_output(Node *target, std::string name)
{
    std::vector<std::pair<Node *, std::string>>::iterator iter;

    for (iter = this->outputs.begin(); iter != this->outputs.end();)
    {
        if (iter->second == name)
        {
            this->outputs.erase(iter);
            break;
        }
    }
}

bool Node::get_matches_input_channels()
{
    return this->matches_input_channels;
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
    throw unknown_trigger_name_exception("Trigger " + name + " is not implemented in node class " + this->name);
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
    return this->out.data[0][0];
}

void Node::set_value(float value)
{
    throw std::runtime_error("set_value() is only applicable to Constant nodes");
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
// VariableInputNode
////////////////////////////////////////////////////////////////////////////////

VariableInputNode::VariableInputNode()
{
    this->has_variable_inputs = true;
    this->last_input_index = 0;
}

VariableInputNode::VariableInputNode(std::initializer_list<NodeRef> inputs)
    : VariableInputNode()
{
    for (NodeRef input : inputs)
    {
        this->add_input(input);
    }
}

VariableInputNode::VariableInputNode(std::vector<NodeRef> inputs)
    : VariableInputNode()
{
    for (NodeRef input : inputs)
    {
        this->add_input(input);
    }
}

VariableInputNode::VariableInputNode(std::vector<float> inputs)
    : VariableInputNode()
{
    for (float input : inputs)
    {
        this->add_input(new Constant(input));
    }
}

VariableInputNode::VariableInputNode(std::vector<int> inputs)
    : VariableInputNode()
{
    for (int input : inputs)
    {
        this->add_input(new Constant(input));
    }
}

void VariableInputNode::add_input(NodeRef input)
{
    /*------------------------------------------------------------------------
     * Use last_input_index rather than input_list.size() because this
     * index must monotonically increase. If inputs are removed from the
     * list, input_list.size() will drop meaning that an index will be
     * erroneously reused.
     *-----------------------------------------------------------------------*/
    std::string input_name = "input" + std::to_string(last_input_index++);
    this->input_list.push_back(input);
    this->Node::create_input(input_name, input_list.back());
}

void VariableInputNode::set_input(std::string name, const NodeRef &node)
{
    if (this->inputs.find(name) == this->inputs.end())
    {
        this->input_list.push_back(node);
        this->Node::create_input(name, input_list.back());
    }

    this->Node::set_input(name, node);
}

void VariableInputNode::remove_input(NodeRef node)
{
    bool removed = false;
    for (auto param : this->inputs)
    {
        if (*(param.second) == node)
        {
            /*--------------------------------------------------------------------------------
             * Remove Node outgoing reference first, to avoid node being garbage collected.
             *--------------------------------------------------------------------------------*/
            node->remove_output(this, param.first);

            this->destroy_input(param.first);
            this->input_list.remove(node);
            removed = true;
            break;
        }
    }
    if (!removed)
    {
        throw std::runtime_error("VariableInputNode: Couldn't find node to remove");
    }
}

void VariableInputNode::clear_inputs()
{
    while (!this->input_list.empty())
    {
        NodeRef node = this->input_list.front();
        this->remove_input(node);
    }
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
