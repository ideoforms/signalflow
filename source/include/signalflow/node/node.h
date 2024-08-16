#pragma once

#include "signalflow/buffer/buffer.h"
#include "signalflow/buffer/ringbuffer.h"
#include "signalflow/core/constants.h"
#include "signalflow/core/platform.h"
#include "signalflow/core/property.h"
#include "signalflow/node/registry.h"

#include <list>
#include <memory>
#include <set>
#include <string>
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
    SIGNALFLOW_NODE_STATE_ACTIVE,
    SIGNALFLOW_NODE_STATE_STOPPED
} signalflow_node_state_t;

/*------------------------------------------------------------------------
 * Allows us to use a float (or direct node ptr) in place of a NodeRef
 * by specifying conversion constructors.
 *-----------------------------------------------------------------------*/
template <class T>
class NodeRefTemplate : public std::shared_ptr<T>
{
public:
    NodeRefTemplate()
        : std::shared_ptr<T>(nullptr) {}
    NodeRefTemplate(T *ptr)
        : std::shared_ptr<T>(ptr) {}
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

    NodeRefTemplate scale(float from, float to, signalflow_scale_t scale = SIGNALFLOW_SCALE_LIN_LIN);
};

typedef NodeRefTemplate<Node> NodeRef;

class Node
{

    /**------------------------------------------------------------------------
     * A Node is a single signal-processing unit in the DSP graph.
     *
     *-----------------------------------------------------------------------*/

public:
    Node();
    virtual ~Node();

    /**------------------------------------------------------------------------
     * Node::process is the main generation/processing method.
     * The Node must generate `num_frames` frames of audio, for exactly
     * `node->get_num_output_channels()` channels, and populate the contents
     * of `out`.
     *
     * @param out The buffer to populate
     * @param num_frames The number of frames needed
     *-----------------------------------------------------------------------*/
    virtual void process(Buffer &out, int num_frames);

    /**------------------------------------------------------------------------
     * If no buffer pointer given, output to the Node's default output buffer.
     *
     * @param num_frames The number of frames to generate.
     *-----------------------------------------------------------------------*/
    virtual void process(int num_frames);

    /**------------------------------------------------------------------------
     * If no buffer pointer given and no num_frames specified, fill the
     * Node's default output buffer.
     *
     *-----------------------------------------------------------------------*/
    // virtual void process();

    /**------------------------------------------------------------------------
     * Get this node's human-readable name.
     * @returns The name
     *-----------------------------------------------------------------------*/
    std::string get_name();

    /*------------------------------------------------------------------------
     * Get state. Most nodes begin in state ACTIVE. Nodes which have a
     * finite lifetime (e.g. envelopes) then transition to STOPPED.
     * If the node is part of a Patch, this may then be used to stop the patch.
     *-----------------------------------------------------------------------*/
    virtual signalflow_node_state_t get_state();

    /*------------------------------------------------------------------------
     * Get/set inputs. An input is a {string, Node} pair that can be used
     * to modulate the destination node.
     *-----------------------------------------------------------------------*/

    virtual NodeRef get_input(std::string name);
    virtual void set_input(std::string name, const NodeRef &input);
    virtual void set_input(std::string name, float value);

    /*------------------------------------------------------------------------
     * The `add_input` method is used only by variable-input nodes
     * which can accept arbitrary numbers of unnamed inputs. Examples include
     * AudioOut, Sum, and ChannelMixer. This method must be subclassed by
     * variable-input nodes. (See get_has_variable_inputs())
     *
     * The superclass implementation throws an exception when called.
     *-----------------------------------------------------------------------*/
    virtual void add_input(NodeRef input);
    virtual void remove_input(NodeRef input);

    /*------------------------------------------------------------------------
     * Register an output. Should be called in the Node's constructor.
     * Note that this must be mirrored with a call to `set_input` on the
     * output node.
     *-----------------------------------------------------------------------*/
    virtual void add_output(Node *target, std::string name);
    virtual void remove_output(Node *target, std::string name);

    /**------------------------------------------------------------------------
     * Get the number of input channels.
     * @returns The number of input channels expected by the node.
     *
     *-----------------------------------------------------------------------*/
    virtual int get_num_input_channels();

    /**------------------------------------------------------------------------
     * Get the number of output channels.
     * @returns The number of output channels generated by the node.
     *
     *-----------------------------------------------------------------------*/
    virtual int get_num_output_channels();

    /**------------------------------------------------------------------------
     * Get the number of output channels allocated in memory.
     * Initially, a node allocates SIGNALFLOW_NODE_INITIAL_OUTPUT_CHANNELS
     * channels. During the course of graph construction, if more channels
     * than this are required, it re-allocates the memory.
     *
     * @returns The number of output channels allocated in memory, which
     *          is at least as large as num_output_channels.
     *
     *-----------------------------------------------------------------------*/
    virtual int get_num_output_channels_allocated();

    /*------------------------------------------------------------------------
     * Queries whether the node's num_output_channels is automatically
     * increased to match the number of channels of its inputs
     *
     * @returns true or false
     *-----------------------------------------------------------------------*/
    bool get_matches_input_channels();

    /**------------------------------------------------------------------------
     * Queries whether the node has a variable input count
     * (for example, ChannelArray, Sum, AudioOut)
     *
     * @returns true or false
     *
     *-----------------------------------------------------------------------*/
    bool get_has_variable_inputs();

    /*------------------------------------------------------------------------
     * Get/set properties.
     *-----------------------------------------------------------------------*/
    virtual void set_property(std::string name, const PropertyRef &value);
    virtual PropertyRef get_property(std::string name);

    /*------------------------------------------------------------------------
     * Get/set buffer properties, as used by BufferPlayer, Granulator, etc.
     *-----------------------------------------------------------------------*/
    virtual void set_buffer(std::string name, BufferRef buffer);

    /*------------------------------------------------------------------------
     * Generic trigger method. 
     *-----------------------------------------------------------------------*/
    virtual void trigger(std::string name = SIGNALFLOW_DEFAULT_TRIGGER,
                         float value = SIGNALFLOW_NULL_FLOAT);

    /*------------------------------------------------------------------------
     * Print the node's output value to stdout at a specified frequency.
     *-----------------------------------------------------------------------*/
    virtual void poll(float frequency = 1.0, std::string label = "");

    /*------------------------------------------------------------------------
     * Get the number of samples in the node's output buffer, per channel.
     *-----------------------------------------------------------------------*/
    int get_output_buffer_length();

    /*------------------------------------------------------------------------
     * Get the Patch that this node is part of.
     *-----------------------------------------------------------------------*/
    Patch *get_patch();

    /*------------------------------------------------------------------------
     * Get the AudioGraph that this node is part of.
     *-----------------------------------------------------------------------*/
    AudioGraph *get_graph();

    /*------------------------------------------------------------------------
     * Buffer containing this node's output.
     *-----------------------------------------------------------------------*/
    Buffer out;

    /*------------------------------------------------------------------------
     * Used to cache
     *-----------------------------------------------------------------------*/
    std::vector<float> last_sample;

    /*------------------------------------------------------------------------
     * Stores the number of frames in the previous processing block. Used
     * to populate frame history in out[-1].
     *
     * TODO: Add a block buffer so that we always have a fixed block size.
     *-----------------------------------------------------------------------*/
    int last_num_frames;

    /*------------------------------------------------------------------------
     * For Constant nodes, returns the value of the constant; for all other
     * nodes, returns the first sample of the first channel of the output
     * buffer (node.output_buffer[0][0])
     *-----------------------------------------------------------------------*/
    virtual float get_value();

    /*------------------------------------------------------------------------
     * Only applicable to Constant nodes.
     *-----------------------------------------------------------------------*/
    virtual void set_value(float value);

    std::map<std::string, NodeRef *> get_inputs();
    std::vector<std::pair<Node *, std::string>> get_outputs();
    std::map<std::string, PropertyRef *> get_properties();
    std::map<std::string, BufferRef *> get_buffers();

protected:
    /*------------------------------------------------------------------------
      * Called by Node subclasses to specify a fixed number of in/out
      * channels. Also unsets matches_input_channels.
      *-----------------------------------------------------------------------*/
    virtual void set_channels(int num_input_channels, int num_output_channels);

    /*------------------------------------------------------------------------
      * Called whenever a Node's inputs are modified, so that it can
      * update its channels.
      *
      * For nodes which have `matches_input_channels`, this updates the
      * Node's number of input/output channels.
      *  - Find the input with the maximum number of output channels
      *  - Set this Node's number of input/output channels to this value
      *  - If the # outputs has changed, propagate the change to outputs
      *    (recursively).
      *
      * For nodes with fixed input/output channels, this checks that none
      * of the inputs exceeds these limits. If an invalid input is found,
      * invalid_channel_count_exception is thrown.
      *-----------------------------------------------------------------------*/
    virtual void update_channels();

    /*------------------------------------------------------------------------
     * Allocate memory for output buffers.
     *  - resize_output_buffers() allocates at least as many as
     *    output_buffer_count buffers. If enough buffers have already been
     *    allocated, don't do anything.
     *-----------------------------------------------------------------------*/
    virtual void resize_output_buffers(int output_buffer_count);

    /*------------------------------------------------------------------------
     * Allocate memory for other dynamic node storage.
     *-----------------------------------------------------------------------*/
    virtual void alloc();
    virtual void free();

    /*------------------------------------------------------------------------
     * Set node run state.
     *-----------------------------------------------------------------------*/
    virtual void set_state(signalflow_node_state_t state);

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
      * Register properties.
      *-----------------------------------------------------------------------*/
    virtual void create_property(std::string name, PropertyRef &property);

    /*------------------------------------------------------------------------
     * Register buffer inputs.
     *-----------------------------------------------------------------------*/
    virtual void create_buffer(std::string name, BufferRef &buffer);

    /*------------------------------------------------------------------------
     * Set the Patch that this node is part of.
     *-----------------------------------------------------------------------*/
    virtual void set_patch(Patch *patch);

    /*------------------------------------------------------------------------
     * Human-readable name identifier [a-z0-9-]
     *----------------------------------------------------------------------*/
    std::string name;

    /*------------------------------------------------------------------------
     * Hash table of parameters: (name, pointer to NodeRef)
     *
     * Must be a pointer, rather than the NodeRef itself, as the actual
     * storage for the pointer is held directly in the node's named
     * parameter field (e.g. node->frequency, node->pan). The inputs
     * dict is a lookup table which stores pointers to the fields, so that
     * they can be populated when the node is instantiated, such as from
     * a graph of NodeDefs.
     *-----------------------------------------------------------------------*/
    std::map<std::string, NodeRef *> inputs;

    /*------------------------------------------------------------------------
     * Set of outputs.
     *
     * Each output is a std::pair containing
     *  - a reference to the Node connected outwards to
     *  - a string containing the name of the parameter that this node
     *    modulates.
     *
     * Note that a node may modulate two different parameters of the same
     * node.
     *-----------------------------------------------------------------------*/
    std::vector<std::pair<Node *, std::string>> outputs;

    /*------------------------------------------------------------------------
     * Hash table of properties: (name, PropertyRef *)
     * A property is a static, non-streaming value assigned to this node.
     * Properties may be ints, floats, strings or arrays.
     *
     * Similar to `inputs`, each property actually points to a local
     * PropertyRef field which must be separately allocated on the object.
     *-----------------------------------------------------------------------*/
    std::map<std::string, PropertyRef *> properties;

    /*------------------------------------------------------------------------
     * Buffers are distinct from parameters, pointing to a fixed
     * area of sample storage that must be non-null.
     *-----------------------------------------------------------------------*/
    std::map<std::string, BufferRef *> buffers;

    /*------------------------------------------------------------------------
     * Pointer to the Graph that this node is a part of.
     * Set automatically in constructor.
     *
     * TODO: Should be an AudioGraphRef.
     *       This is harder to accomplish than it looks.
     *-----------------------------------------------------------------------*/
    AudioGraph *graph = nullptr;

    /*------------------------------------------------------------------------
     * Pointer to the Patch that this node is a part of, if any.
     *-----------------------------------------------------------------------*/
    Patch *patch = nullptr;

    /*------------------------------------------------------------------------
     * Used for polling this output of this node.
     *-----------------------------------------------------------------------*/
    NodeMonitor *monitor = nullptr;

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

    /*------------------------------------------------------------------------
     * Flag indicating whether the node has a variable input count
     * (e.g. ChannelArray, Sum, AudioOut_Abstract, etc)
     *-----------------------------------------------------------------------*/
    bool has_variable_inputs;

    /*------------------------------------------------------------------------
     * Number of actual in/out channels. This should always reflect
     * the number of audio channels in use.
     *-----------------------------------------------------------------------*/
    int num_input_channels;
    int num_output_channels;

    /*------------------------------------------------------------------------
     * Node state
     *-----------------------------------------------------------------------*/
    signalflow_node_state_t state;

private:
    /*------------------------------------------------------------------------
     * Wrapper around process(), called by AudioGraph.pull_input,
     * which handles caching of earlier frames etc.
     *-----------------------------------------------------------------------*/
    virtual void _process(Buffer &out, int num_frames);

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

class VariableInputNode : public Node
{

public:
    VariableInputNode();
    VariableInputNode(std::initializer_list<NodeRef> inputs);
    VariableInputNode(std::vector<NodeRef> inputs);
    VariableInputNode(std::vector<int> inputs);
    VariableInputNode(std::vector<float> inputs);

    virtual void add_input(NodeRef input);
    virtual void set_input(std::string name, const NodeRef &node);
    virtual void remove_input(NodeRef input);

protected:
    std::list<NodeRef> input_list;
    unsigned int last_input_index;
};

}
