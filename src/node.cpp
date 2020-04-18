#include "signal/node.h"

#include "signal/operators/multiply.h"
#include "signal/operators/add.h"
#include "signal/operators/subtract.h"
#include "signal/operators/divide.h"
#include "signal/operators/scale.h"

#include "signal/oscillators/constant.h"
#include "signal/operators/multiplex.h"

#include "signal/core.h"
#include "signal/graph.h"
#include "signal/util.h"
#include "signal/monitor.h"

#include <stdio.h>
#include <stdlib.h>
#include <cassert>


namespace libsignal
{
    
extern AudioGraph *shared_graph;

Node::Node()
{
    this->graph = shared_graph;
    this->out = (sample **) malloc(SIGNAL_MAX_CHANNELS * sizeof(sample *));
    for (int i = 0; i < SIGNAL_MAX_CHANNELS; i++)
    {
        this->out[i] = (sample *) calloc(SIGNAL_NODE_BUFFER_SIZE, sizeof(sample));

        /*------------------------------------------------------------------------
         * Memory allocation magic: incrementing the `out` pointer means that
         * we can query out[-1] which holds the last frame of the previous
         * block. 
         *-----------------------------------------------------------------------*/
        this->out[i] = this->out[i] + 1;
    }

    this->min_input_channels = N_CHANNELS;
    this->max_input_channels = N_CHANNELS;
    this->min_output_channels = N_CHANNELS;
    this->max_output_channels = N_CHANNELS;

    this->num_input_channels = 1;
    this->num_output_channels = 1;
    this->last_num_frames = 0;

    this->no_input_automix = false;

    this->monitor = NULL;
}

Node::~Node()
{
    for (int i = 0; i < SIGNAL_MAX_CHANNELS; i++)
    {
        /*------------------------------------------------------------------------
         * Memory allocation magic: Pointer to out[] is actually 1 byte off
         * the original allocated segment (see Node constructor above).
         *-----------------------------------------------------------------------*/
        free(this->out[i] - 1);
    }
    free(this->out);
}

/*------------------------------------------------------------------------
 * Called directly by AudioGraph, this function wraps around the
 * subclassed process() method and handles extra bits of magic,
 * including populating the buffer history so that the value of the
 * previous frame can be checked.
 *-----------------------------------------------------------------------*/
void Node::_process(sample **out, int num_frames)
{
    if (this->last_num_frames > 0)
    {
        for (int i = 0; i < SIGNAL_MAX_CHANNELS; i++)
        {
            out[i][-1] = out[i][last_num_frames - 1];
        }
    }
    this->process(out, num_frames);
    this->last_num_frames = num_frames;
}

void Node::process(sample **out, int num_frames)
{
    throw std::runtime_error("Node::process (should never be called)");
}

void Node::update_channels()
{
    if (this->min_input_channels == N_CHANNELS)
    {
        int max_channels = 1;
        for (auto param : this->params)
        {
            NodeRef *ptr = param.second;
            // A param may be registered but not yet set
            if (!ptr || !*ptr)
                continue;
            std::string param_name = param.first;
            // signal_debug("%s: update_channels (%s)", this->name.c_str(), param_name.c_str());

            NodeRef input = *ptr;
            if (input->num_output_channels > max_channels)
                max_channels = input->num_output_channels;
        }

        this->num_input_channels = max_channels;
        this->num_output_channels = max_channels;

        if (this->min_output_channels > this->num_output_channels)
        {
            this->num_output_channels = this->min_output_channels;
        }

        signal_debug("Node %s set num_out_channels to %d", this->name.c_str(), this->num_output_channels);
    }
}

void Node::add_input(std::string name, NodeRef &node)
{
    /*------------------------------------------------------------------------
     * Update each input's channel count first, allowing up-mix to
     * perculate to the root of the graph.
     *-----------------------------------------------------------------------*/
    if (node)
    {
        node->update_channels();
        node->add_output(this, name);
    }

    this->params[name] = &node;
    this->update_channels();

}

void Node::remove_input(std::string name)
{
    /*------------------------------------------------------------------------
     * Only done by special classes (Multiplex, AudioOut)
     *-----------------------------------------------------------------------*/
    this->params.erase(name);
    this->update_channels();
}

void Node::set_input(std::string name, const NodeRef &node)
{
    if (this->params.find(name) == this->params.end())
        throw std::runtime_error("Node " + this->name + " has no such param: " + name);

    NodeRef current_input = *(this->params[name]);
    if (current_input)
    {
        current_input->remove_output(this, name);
    }

    *(this->params[name]) = node;
    this->update_channels();
    node->update_channels();

    node->add_output(this, name);
}

void Node::add_output(Node *target, std::string name)
{
    this->outputs.insert(std::make_pair(target, name));
}

void Node::remove_output(Node *target, std::string name)
{
    this->outputs.erase(std::make_pair(target, name));
}

void Node::disconnect_outputs()
{
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
        target->set_input(name, 0);
    }
}

void Node::disconnect_inputs()
{
    for (auto param : this->params)
    {
        this->set_input(param.first, 0);
    }
}


void Node::add_property(std::string name, PropertyRef &value)
{
    this->properties[name] = &value;
}

void Node::set_property(std::string name, const PropertyRef &value)
{
    if (this->properties.find(name) == this->properties.end())
        throw std::runtime_error("Node " + this->name + " has no such property: " + name);

    *this->properties[name] = value;
}

PropertyRef Node::get_property(std::string name)
{
    if (this->properties.find(name) == this->properties.end())
        throw std::runtime_error("Node " + this->name + " has no such property: " + name);
    
    return *(this->properties[name]);
}

void Node::add_buffer(std::string name, BufferRef &buffer)
{
    this->buffers[name] = &buffer;
}

void Node::set_buffer(std::string name, BufferRef buffer)
{
    if (this->buffers.find(name) == this->buffers.end())
        throw std::runtime_error("Node " + this->name + " has no such buffer: " + name);

    *(this->buffers[name]) = buffer;
}


// TODO: Assignment operator breaks our paradigm as (I think) we need 
// to update the new object's 'ref' pointer to its shared_ptr container...
// This might be bad practice. 
/*
template<>
NodeRef NodeRef::operator= (const NodeRef &other)
{
    printf("UNITREF ASSIGN, HERE BE DRAGONS\n");
    // if (this != other)
    //    (*this)->ref = other->ref;
    return *this;
}
*/

void Node::zero_output()
{
    for (int i = 0; i < this->num_output_channels; i++)
        memset(this->out[i], 0, SIGNAL_NODE_BUFFER_SIZE * sizeof(sample));
}

void Node::trigger(std::string name, float value)
{
}


void Node::poll(float frequency, std::string label)
{
    this->monitor = new NodeMonitor(this, label, frequency); 
    this->monitor->start();
}

/*------------------------------------------------------------------------
 * Default constructors.
 *-----------------------------------------------------------------------*/

NodeRef::NodeRef() : std::shared_ptr<Node>(nullptr) {}
NodeRef::NodeRef(Node *ptr) : std::shared_ptr<Node>(ptr) {}
NodeRef::NodeRef(double x) : std::shared_ptr<Node>(new Constant(x)) {}
NodeRef::NodeRef(int x) : std::shared_ptr<Node>(new Constant((float) x)) {}
NodeRef::NodeRef(std::initializer_list<NodeRef> x) : std::shared_ptr<Node>(new Multiplex(x)) {}
NodeRef::NodeRef(std::vector<NodeRef> x) : std::shared_ptr<Node>(new Multiplex(x)) {}

/*------------------------------------------------------------------------
 * Don't explicitly cast to NodeRef here or bad things happen
 * (shared_ptrs freed too early -- causing SIGSEGV when doing
 * sine * 0.25)
 *-----------------------------------------------------------------------*/
NodeRef NodeRef::operator* (NodeRef other)
    { return new Multiply(*this, other); }

NodeRef NodeRef::operator* (double constant)
    { return new Multiply(*this, constant); }

NodeRef operator*(double constant, const NodeRef node)
    { return new Multiply(node, constant); }

NodeRef NodeRef::operator+ (NodeRef other)
    { return new Add(*this, other); }

NodeRef NodeRef::operator+ (double constant)
    { return new Add(*this, constant); }

NodeRef operator+(double constant, const NodeRef node)
    { return new Add(node, constant); }

NodeRef NodeRef::operator- (NodeRef other)
    { return new Subtract(*this, other); }

NodeRef NodeRef::operator- (double constant)
    { return new Subtract(*this, constant); }

NodeRef operator-(double constant, const NodeRef node)
    { return new Subtract(node, constant); }

NodeRef NodeRef::operator/ (NodeRef other)
    { return new Divide(*this, other); }

NodeRef NodeRef::operator/ (double constant)
    { return new Divide(*this, constant); }

NodeRef operator/(double constant, const NodeRef node)
    { return new Divide(node, constant); }

sample NodeRef::operator[] (int index)
{
    // unused?
    return (*this)->out[0][index];
}

BinaryOpNode::BinaryOpNode(NodeRef a, NodeRef b) : Node(), input0(a), input1(b)
{
    this->add_input("input0", this->input0);
    this->add_input("input1", this->input1);
}

UnaryOpNode::UnaryOpNode(NodeRef a) : Node(), input(a)
{
    this->add_input("input", this->input);
}

NodeRef Node::scale(float from, float to, signal_scale_t scale)
{
    switch (scale)
    {
        case SIGNAL_SCALE_LIN_LIN:
            return new Scale(this, -1, 1, from, to);
        case SIGNAL_SCALE_LIN_EXP:
            return new LinExp(this, -1, 1, from, to);
        default:
            return nullptr;
    }
}

// Node Node::operator+ (Node &other)
//    { return Add(this, &other); }

} /* namespace libsignal */

