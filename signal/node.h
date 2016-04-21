#pragma once

#include "constants.h"
#include "ringbuffer.h"
#include "registry.h"
#include "platform.h"
#include "buffer.h"
#include "property.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <set>
#include <memory>


namespace libsignal
{

	/*------------------------------------------------------------------------
	 * Forward-declare our operator classes to avoid interdependencies.
	 *-----------------------------------------------------------------------*/
	class Multiply;
	class Add;
	class Subtract;
	class Divide;

	class Node;
	class Graph;
	class NodeMonitor;

	/*------------------------------------------------------------------------
	 * Allows us to use a float (or direct node ptr) in place of a NodeRef
	 * by specifying conversion constructors.
	 *-----------------------------------------------------------------------*/
	template<class T>
	class NodeRefT : public std::shared_ptr<T>
	{
		public:
			using std::shared_ptr<T>::shared_ptr;

			NodeRefT();
			NodeRefT(Node *ptr);
			NodeRefT(double x);
			NodeRefT(int x);
			NodeRefT(std::initializer_list<NodeRefT> x);

			NodeRefT operator* (NodeRefT other);
			NodeRefT operator* (double constant);
			NodeRefT operator+ (NodeRefT other);
			NodeRefT operator+ (double constant);
			NodeRefT operator- (NodeRefT other);
			NodeRefT operator- (double constant);
			NodeRefT operator/ (NodeRefT other);
			NodeRefT operator/ (double constant);
			sample operator[] (int index);

			// NodeRefT operator= (const NodeRefT &other);

	};

	typedef NodeRefT <Node> NodeRef;

	/*------------------------------------------------------------------------
	 * Mathematical operators where NodeRef is the RHS operand
	 *-----------------------------------------------------------------------*/
	NodeRef operator*(double constant, const NodeRef other);
	NodeRef operator+(double constant, const NodeRef other);
	NodeRef operator-(double constant, const NodeRef other);
	NodeRef operator/(double constant, const NodeRef other);

	class Node
	{

		public:

			Node();
			Node(double x);

			virtual void next(sample **out, int num_frames);

			/*------------------------------------------------------------------------
			 * Connect a new signal input to this node. These connections form
			 * the overall signal graph.
			 *-----------------------------------------------------------------------*/
			virtual void add_input(NodeRef input) {}

			/*------------------------------------------------------------------------
			 * Called after add_input/route to update our routing ins/outs.
			 *-----------------------------------------------------------------------*/
			virtual void update_channels();

			/*------------------------------------------------------------------------
			 * Register parameters.
			 *-----------------------------------------------------------------------*/
			virtual void add_param(std::string name, NodeRef &param);
			virtual void set_param(std::string name, const NodeRef &param);

			/*------------------------------------------------------------------------
			 * Register properties.
			 *-----------------------------------------------------------------------*/
			virtual void add_property(std::string name);
			virtual void set_property(std::string name, PropertyRef value);
			virtual PropertyRef get_property(std::string name);

			/*------------------------------------------------------------------------
			 * Register buffer params.
			 *-----------------------------------------------------------------------*/
			virtual void add_buffer(std::string name, Buffer **buffer);
			virtual void set_buffer(std::string name, Buffer *buffer);

			/*------------------------------------------------------------------------
			 * TODO: Generic trigger method. Take named param for trigger ID?
			 *-----------------------------------------------------------------------*/
			virtual void trigger();
			virtual void trigger(std::string name);

			/*------------------------------------------------------------------------
			 * Check whether a trigger has been triggered.
			 *-----------------------------------------------------------------------*/
			virtual bool triggered();
			virtual bool triggered(std::string name);

			/*------------------------------------------------------------------------
			 * Sets our output buffer to zero.
			 *-----------------------------------------------------------------------*/
			virtual void zero_output();

			/*------------------------------------------------------------------------
			 * Outputs the node's value at a user-specified frequency.
			 *-----------------------------------------------------------------------*/
			virtual void poll(float frequency = 1.0, std::string label = "");
			NodeMonitor *monitor;

			/*------------------------------------------------------------------------
			 * Returns a new Node that scales the output of this node from
			 * `from` to `to`.
			 *-----------------------------------------------------------------------*/
			virtual Node *scale(float from, float to, signal_scale_t scale = SIGNAL_SCALE_LIN_LIN);

			/*------------------------------------------------------------------------
			 * Human-readable name identifier [a-z0-9-]
			 *----------------------------------------------------------------------*/
			std::string name;

			/*------------------------------------------------------------------------
			 * Hash table of parameters: (name, pointer to NodeRef)
			 * Must be a pointer rather than the NodeRef itself as these
			 * params are actually pointers to struct fields (this->frequency).
			 *-----------------------------------------------------------------------*/
			std::unordered_map <std::string, NodeRef *> params;

			/*------------------------------------------------------------------------
			 * Hash table of properties: (name, PropertyRef)
			 * A property is a static, non-streaming value assigned to this node.
			 * Properties may be ints, floats, strings or arrays.
			 *-----------------------------------------------------------------------*/
			std::unordered_map <std::string, PropertyRef> properties;

			/*------------------------------------------------------------------------
			 * Buffers are distinct from parameters, pointing to a fixed
			 * area of sample storage that must be non-null.
			 *-----------------------------------------------------------------------*/
			std::unordered_map <std::string, Buffer **> buffers;

			/*------------------------------------------------------------------------
			 * Pointer to the Graph that this node is a part of.
			 * Set automatically in constructor.
			 *-----------------------------------------------------------------------*/
			Graph *graph;

			/*------------------------------------------------------------------------
			 * Number of actual in/out channels. This should always reflect
			 * the number of audio channels allocated in our `out` buffer,
			 *-----------------------------------------------------------------------*/
			int num_input_channels;
			int num_output_channels;

			/*------------------------------------------------------------------------
			 * Number of preferred in/out channels. This is used to determine
			 * how audio should be up-mixed or down-mixed when passing signals
			 * between nodes.
			 *
			 * N-to-N nodes should use N_CHANNELS / N_CHANNELS
			 * 1-to-N nodes should use 1 / N_CHANNELS
			 *-----------------------------------------------------------------------*/
			int  min_input_channels,
			     max_input_channels,
			     min_output_channels,
			     max_output_channels;
			bool no_input_automix;

			/*------------------------------------------------------------------------
			 * Buffer containing this node's output.
			 * TODO: Point this partway through a bigger frame buffer so that
			 * its history can be read for delay lines etc.
			 *-----------------------------------------------------------------------*/
			sample **out;

			/*------------------------------------------------------------------------
			 * A reference to the NodeRef shared_ptr pointing to this Node.
			 * Necessary so that a node can make outgoing/incoming connections to
			 * other NodeRefs, increasing its own shared_ptr's reference count.
			 *-----------------------------------------------------------------------*/
			NodeRef *ref;

			/*------------------------------------------------------------------------
			 * Trigger states.
			 *-----------------------------------------------------------------------*/
			std::set <std::string> triggers;
	};

	class GeneratorNode : public Node
	{
		public:
			GeneratorNode() : Node()
			{
				this->num_input_channels = 0;
				this->num_output_channels = 1;
			}
	};

	class UnaryOpNode : public Node
	{
		public:
			UnaryOpNode(NodeRef input = 0);

			NodeRef input;
	};

	class BinaryOpNode : public Node
	{
		public:
			BinaryOpNode(NodeRef a = 0, NodeRef b = 0);

			NodeRef input0;
			NodeRef input1;
	};
}

