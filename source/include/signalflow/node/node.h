#pragma once

#include "signalflow/buffer/buffer.h"
#include "signalflow/buffer/ringbuffer.h"
#include "signalflow/core/constants.h"
#include "signalflow/core/platform.h"
#include "signalflow/core/property.h"
#include "signalflow/node/registry.h"

#include <math.h>
#include <memory>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

namespace signalflow
{

/*------------------------------------------------------------------------
 * Forward-declare key classes to avoid interdependencies.
 *-----------------------------------------------------------------------*/
class Node;
class Patch;
class AudioGraph;
class NodeMonitor;

typedef enum
{
    SIGNAL_NODE_STATE_ACTIVE,
    SIGNAL_NODE_STATE_STOPPED
} signal_node_state_t;

/*------------------------------------------------------------------------
 * Allows us to use a float (or direct node ptr) in place of a NodeRef
 * by specifying conversion constructors.
 *-----------------------------------------------------------------------*/
template <class T>
class NodeRefTemplate : public std::shared_ptr<T>
{
public:
    NodeRefTemplate()
        : std::shared_ptr<T>(nullptr) {};
    NodeRefTemplate(T *ptr)
        : std::shared_ptr<T>(ptr) {};
    NodeRefTemplate(double x);
    NodeRefTemplate(int x);
    NodeRefTemplate(std::initializer_list<NodeRefTemplate> x);
    NodeRefTemplate(std::vector<NodeRefTemplate> x);
    NodeRefTemplate(std::vector<float> x);

    NodeRefTemplate operator*(NodeRefTemplate other);
    NodeRefTemplate operator*(double constant);
    NodeRefTemplate operator+(NodeRefTemplate other);
    NodeRefTemplate operator+(double constant);
    NodeRefTemplate operator-(NodeRefTemplate other);
    NodeRefTemplate operator-(double constant);
    NodeRefTemplate operator/(NodeRefTemplate other);
    NodeRefTemplate operator/(double constant);
    sample operator[](int index);
};

typedef NodeRefTemplate<Node> NodeRef;

/*------------------------------------------------------------------------
 * Mathematical operator where NodeRef is the RHS operand
 * TODO Test and restore
 *-----------------------------------------------------------------------*/
//    NodeRef operator*(double constant, const NodeRef other);
//    NodeRef operator+(double constant, const NodeRef other);
//    NodeRef operator-(double constant, const NodeRef other);
//    NodeRef operator/(double constant, const NodeRef other);

class Node
{

public:
    Node();
    virtual ~Node();

    /*------------------------------------------------------------------------
     * The main generation/processing method.
     *-----------------------------------------------------------------------*/
    virtual void process(sample **out, int num_frames);

    /*------------------------------------------------------------------------
     * If no buffer pointer given, output to the Node's default output buffer.
     *-----------------------------------------------------------------------*/
    virtual void process(int num_frames);

    /*------------------------------------------------------------------------
     * Get state.
     *-----------------------------------------------------------------------*/
    virtual signal_node_state_t get_state();

    /*------------------------------------------------------------------------
     * Get/set inputs.
     *-----------------------------------------------------------------------*/
    virtual NodeRef get_input(std::string name);
    virtual void set_input(std::string name, const NodeRef &input);
    virtual void set_input(std::string name, float value);

    /*------------------------------------------------------------------------
     * The `add_input` method is used only by variable-input nodes
     * which can take multiple unnamed inputs. The superclass implementation
     * throws an exception when called.
     *-----------------------------------------------------------------------*/
    virtual void add_input(NodeRef input);
    virtual void remove_input(NodeRef input);

    /*------------------------------------------------------------------------
     * Disconnect inputs and outputs.
     *-----------------------------------------------------------------------*/
    virtual void disconnect_inputs();
    virtual void disconnect_outputs();

    /*------------------------------------------------------------------------
     * Get/set properties.
     *-----------------------------------------------------------------------*/
    virtual void set_property(std::string name, const PropertyRef &value);
    virtual PropertyRef get_property(std::string name);

    /*------------------------------------------------------------------------
     * Get/set buffers.
     *-----------------------------------------------------------------------*/
    virtual void set_buffer(std::string name, BufferRef buffer);

    /*------------------------------------------------------------------------
     * Generic trigger method. 
     *-----------------------------------------------------------------------*/
    virtual void trigger(std::string name = SIGNAL_DEFAULT_TRIGGER, float value = 1);

    /*------------------------------------------------------------------------
     * Outputs the node's value at a user-specified frequency.
     *-----------------------------------------------------------------------*/
    virtual void poll(float frequency = 1.0, std::string label = "");

    /*------------------------------------------------------------------------
     * Returns a new Node that scales the output of this node from
     * `from` to `to`.
     *-----------------------------------------------------------------------*/
    virtual NodeRef scale(float from, float to, signal_scale_t scale = SIGNAL_SCALE_LIN_LIN);

    /*------------------------------------------------------------------------
     * Get the number of samples in the node's output buffer, per channel.
     *-----------------------------------------------------------------------*/
    int get_output_buffer_length();

    /*------------------------------------------------------------------------
     * Get the Patch that this node is part of.
     *-----------------------------------------------------------------------*/
    Patch *get_patch();

    /*------------------------------------------------------------------------
     * Human-readable name identifier [a-z0-9-]
     *----------------------------------------------------------------------*/
    std::string name;

    /*------------------------------------------------------------------------
     * Hash table of parameters: (name, pointer to NodeRef)
     * Must be a pointer, rather than the NodeRef itself, as the actual
     * storage for the pointer is held directly in the node's named
     * parameter field (e.g. node->frequency, node->pan). The inputs
     * dict is a lookup table which stores pointers to the fields, so that
     * they can be populated when the node is instantiated, such as from
     * a graph of NodeDefs.
     *-----------------------------------------------------------------------*/
    std::unordered_map<std::string, NodeRef *> inputs;

    /*------------------------------------------------------------------------
     * Set of outputs.
     * Each output is a std::pair containing 
     *  - a reference to the Node connected outwards to
     *  - a string containing the name of the parameter that this node
     *    modulates.
     * Note that a node may modulate two different parameters of the same
     * node.
     *-----------------------------------------------------------------------*/
    std::set<std::pair<Node *, std::string>> outputs;

    /*------------------------------------------------------------------------
     * Hash table of properties: (name, PropertyRef *)
     * A property is a static, non-streaming value assigned to this node.
     * Properties may be ints, floats, strings or arrays.
     *
     * Similar to `inputs`, each property actually points to a local 
     * PropertyRef field which must be separately allocated on the object.
     *-----------------------------------------------------------------------*/
    std::unordered_map<std::string, PropertyRef *> properties;

    /*------------------------------------------------------------------------
     * Buffers are distinct from parameters, pointing to a fixed
     * area of sample storage that must be non-null.
     *-----------------------------------------------------------------------*/
    std::unordered_map<std::string, BufferRef *> buffers;

    /*------------------------------------------------------------------------
     * Pointer to the Graph that this node is a part of.
     * Set automatically in constructor.
     *
     * TODO: Should be an AudioGraphRef.
     *       This is harder to accomplish than it looks.
     *-----------------------------------------------------------------------*/
    AudioGraph *graph = nullptr;

    /*------------------------------------------------------------------------
     * Number of actual in/out channels. This should always reflect
     * the number of audio channels in use.
     *-----------------------------------------------------------------------*/
    int num_input_channels;
    int num_output_channels;

    /*------------------------------------------------------------------------
     * Buffer containing this node's output.
     *-----------------------------------------------------------------------*/
    sample **out;

    /*------------------------------------------------------------------------
     * Node state
     *-----------------------------------------------------------------------*/
    signal_node_state_t state;

    /*------------------------------------------------------------------------
     * Stores the number of frames in the previous processing block. Used
     * to populate frame history in out[-1].
     *-----------------------------------------------------------------------*/
    int last_num_frames;

protected:
    /*------------------------------------------------------------------------
      * Called by Node subclasses to specify a fixed number of in/out
      * channels. Also unsets matches_input_channels.
      *-----------------------------------------------------------------------*/
    virtual void set_channels(int num_input_channels, int num_output_channels);

    /*------------------------------------------------------------------------
      * Called after create_input/route to update our routing ins/outs,
      * called by AudioGraph
      *-----------------------------------------------------------------------*/
    virtual void update_channels();

    /*------------------------------------------------------------------------
     * Allocate memory for output buffers.
     *-----------------------------------------------------------------------*/
    virtual void allocate_output_buffer();
    virtual void free_output_buffer();

    /*------------------------------------------------------------------------
     * Set node run state.
     *-----------------------------------------------------------------------*/
    virtual void set_state(signal_node_state_t state);

    /*------------------------------------------------------------------------
     * Creates a new named input.
     * Should only ever be used in the class constructor.
     *-----------------------------------------------------------------------*/
    virtual void create_input(std::string name, NodeRef &input);

    /*------------------------------------------------------------------------
     * Removing an input is only done by special classes
     * (ChannelArray, AudioOut)
     *-----------------------------------------------------------------------*/
    virtual void destroy_input(std::string name);

    /*------------------------------------------------------------------------
     * Register an output.
     * Note that this must be mirrored with a call to `set_input` on the
     * output node.
     *-----------------------------------------------------------------------*/
    virtual void add_output(Node *target, std::string name);
    virtual void remove_output(Node *target, std::string name);

    /*------------------------------------------------------------------------
      * Register properties.
      *-----------------------------------------------------------------------*/
    virtual void add_property(std::string name, PropertyRef &property);

    /*------------------------------------------------------------------------
     * Register buffer inputs.
     *-----------------------------------------------------------------------*/
    virtual void create_buffer(std::string name, BufferRef &buffer);

    /*------------------------------------------------------------------------
     * Set the Patch that this node is part of.
     *-----------------------------------------------------------------------*/
    virtual void set_patch(Patch *patch);

    /*------------------------------------------------------------------------
     * Output buffer length, in samples.
     *-----------------------------------------------------------------------*/
    int output_buffer_length;

    /*------------------------------------------------------------------------
     * Number of output channels currently allocated.
     *-----------------------------------------------------------------------*/
    int num_output_channels_allocated;

    /*------------------------------------------------------------------------
     * If a node currently has N input channels but M>N channels are
     * requested, its input will be automatically upmixed by the containing
     * AudioGraph by duplicating the existing channels until M is reached.
     *
     * If this is disabled, no upmixing takes place.
     *-----------------------------------------------------------------------*/
    bool no_input_upmix;

    /*------------------------------------------------------------------------
     * If matches_input_channels is set, a node automatically increases its
     * input/output channel count to match those of its inputs.
     *-----------------------------------------------------------------------*/
    bool matches_input_channels;

    /*------------------------------------------------------------------------
     * Flag indicating whether the node has been processed within this
     * graph traversal.
     *-----------------------------------------------------------------------*/
    bool has_rendered;

private:
    /*------------------------------------------------------------------------
     * Used for polling this output of this node.
     *-----------------------------------------------------------------------*/
    NodeMonitor *monitor;

    /*------------------------------------------------------------------------
     * Wrapper around process(), called by AudioGraph.pull_input,
     * which handles caching of earlier frames etc.
     *-----------------------------------------------------------------------*/
    virtual void _process(sample **out, int num_frames);

    /*------------------------------------------------------------------------
     * Pointer to the Patch that this node is a part of, if any.
     *-----------------------------------------------------------------------*/
    Patch *patch = nullptr;

    /*------------------------------------------------------------------------
     * Allow friends to access private methods
     *-----------------------------------------------------------------------*/
    friend class AudioGraph;
    friend class Patch;
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
