#include "node.h"

#include "op/multiply.h"
#include "op/add.h"
#include "op/subtract.h"
#include "op/divide.h"

#include "gen/constant.h"
#include "op/multiplex.h"

#include "core.h"
#include "graph.h"
#include "util.h"
#include "monitor.h"

#include <stdio.h>
#include <stdlib.h>
#include <cassert>


namespace libsignal
{
    
extern Graph *shared_graph;

Node::Node()
{
	this->graph = shared_graph;
	this->out = (sample **) malloc(SIGNAL_MAX_CHANNELS * sizeof(sample *));
	for (int i = 0; i < SIGNAL_MAX_CHANNELS; i++)
		this->out[i] = (sample *) malloc(SIGNAL_NODE_BUFFER_SIZE * sizeof(sample));

	this->min_input_channels = N_CHANNELS;
	this->max_input_channels = N_CHANNELS;
	this->min_output_channels = N_CHANNELS;
	this->max_output_channels = N_CHANNELS;

	this->num_input_channels = 1;
	this->num_output_channels = 1;

	this->no_input_automix = false;

	this->ref = NULL;
	this->monitor = NULL;
}

void Node::next(sample **out, int num_frames)
{
	// Basic next() loop assumes we are N-in, N-out.
	// TODO: Assert channel config makes sense? (> 0)

	throw std::runtime_error("Node::next (should never be called)");
}

void Node::update_channels()
{
	if (this->min_input_channels == N_CHANNELS)
	{
		int max_channels = 0;
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

		// signal_debug("Node %s set num_out_channels to %d", this->name.c_str(), max_channels);
		this->num_input_channels = max_channels;
		this->num_output_channels = max_channels;
	}
}

void Node::add_param(std::string name, NodeRef &node)
{
	this->params[name] = &node;
	this->update_channels();

	if (node)
		node->update_channels();
}

void Node::set_param(std::string name, const NodeRef &node)
{
	if (this->params.find(name) == this->params.end())
		throw std::runtime_error("Node " + this->name + " has no such param: " + name);

	*(this->params[name]) = node;
	this->update_channels();
	node->update_channels();
}



void Node::add_buffer(std::string name, Buffer **buffer)
{
	this->buffers[name] = buffer;
}

void Node::set_buffer(std::string name, Buffer *buffer)
{
	if (!this->buffers[name])
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
	//	(*this)->ref = other->ref;
	return *this;
}
*/

void Node::zero_output()
{
	for (int i = 0; i < this->num_output_channels; i++)
		memset(this->out[i], 0, SIGNAL_NODE_BUFFER_SIZE * sizeof(sample));
}

void Node::trigger(std::string name)
{
	triggers.insert(name);
}

void Node::trigger()
{
	this->trigger("default");
}

bool Node::triggered(std::string name)
{
	bool rv = false;
	auto location = triggers.find(name);
	if (location != triggers.end())
	{
		triggers.erase(location);
		rv = true;
	}
	return rv;
}

bool Node::triggered()
{
	return this->triggered("default");
}

void Node::poll(float frequency, std::string label)
{
	this->monitor = new NodeMonitor(this, label, frequency); 
	this->monitor->start();
}

/*------------------------------------------------------------------------
 * Default constructors. 
 *-----------------------------------------------------------------------*/

template<>
NodeRef::NodeRefT() : std::shared_ptr<Node>(nullptr) { }

template<>
NodeRef::NodeRefT(Node *ptr) : std::shared_ptr<Node>(ptr)
	{ ptr->ref = this; }

template<>
NodeRef::NodeRefT(double x) : std::shared_ptr<Node>(new Constant(x))
	{ (*this)->ref = this; }

template<>
NodeRef::NodeRefT(int x) : std::shared_ptr<Node>(new Constant((float) x))
	{ (*this)->ref = this; }

template<>
NodeRef::NodeRefT(std::initializer_list<NodeRefT> x) : std::shared_ptr<Node>(new Multiplex(x))
	{ (*this)->ref = this; }


/*------------------------------------------------------------------------
 * Don't explicitly cast to NodeRef here or bad things happen
 * (shared_ptrs freed too early -- causing SIGSEGV when doing
 * sine * 0.25)
 *-----------------------------------------------------------------------*/
template<>
NodeRef NodeRef::operator* (NodeRef other)
	{ return new Multiply(*this, other); }

template<>
NodeRef NodeRef::operator* (double constant)
	{ return new Multiply(*this, constant); }

NodeRef operator*(double constant, const NodeRef node)
	{ return new Multiply(node, constant); }

template<>
NodeRef NodeRef::operator+ (NodeRef other)
	{ return new Add(*this, other); }

template<>
NodeRef NodeRef::operator+ (double constant)
	{ return new Add(*this, constant); }

NodeRef operator+(double constant, const NodeRef node)
	{ return new Add(node, constant); }

template<>
NodeRef NodeRef::operator- (NodeRef other)
	{ return new Subtract(*this, other); }

template<>
NodeRef NodeRef::operator- (double constant)
	{ return new Subtract(*this, constant); }

NodeRef operator-(double constant, const NodeRef node)
	{ return new Subtract(node, constant); }

template<>
NodeRef NodeRef::operator/ (NodeRef other)
	{ return new Divide(*this, other); }

template<>
NodeRef NodeRef::operator/ (double constant)
	{ return new Divide(*this, constant); }

NodeRef operator/(double constant, const NodeRef node)
	{ return new Divide(node, constant); }

template<>
sample NodeRef::operator[] (int index)
{
	// unused?
	return (*this)->out[0][index];
}

BinaryOpNode::BinaryOpNode(NodeRef a, NodeRef b) : Node(), input0(a), input1(b)
{
	this->add_param("input0", this->input0);
	this->add_param("input1", this->input1);
}

UnaryOpNode::UnaryOpNode(NodeRef a) : Node(), input(a)
{
	this->add_param("input", this->input);
}

}

