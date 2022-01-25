#pragma once

/**-------------------------------------------------------------------------
 * @file patch.h
 * @brief Patch encapsulates a subgraph of nodes, with named inputs
 *        and a single output.
 *
 *-----------------------------------------------------------------------*/

#include "signalflow/patch/patch-node-spec.h"
#include "signalflow/patch/patch-spec.h"
#include <map>

namespace signalflow
{

typedef enum
{
    SIGNALFLOW_PATCH_STATE_ACTIVE,
    SIGNALFLOW_PATCH_STATE_STOPPED
} signalflow_patch_state_t;

class Patch;

template <class T>
class PatchRefTemplate : public std::shared_ptr<T>
{
public:
    PatchRefTemplate()
        : std::shared_ptr<T>(nullptr) {}
    PatchRefTemplate(Patch *ptr)
        : std::shared_ptr<T>(ptr) {}
    PatchRefTemplate(PatchSpecRef patchspec)
        : std::shared_ptr<T>(new T(patchspec)) {}

    NodeRef operator*(NodeRef other);
    NodeRef operator*(double constant);
    NodeRef operator+(NodeRef other);
    NodeRef operator+(double constant);
    NodeRef operator-(NodeRef other);
    NodeRef operator-(double constant);
    NodeRef operator/(NodeRef other);
    NodeRef operator/(double constant);
};

typedef PatchRefTemplate<Patch> PatchRef;

class Patch
{
public:
    Patch();
    Patch(PatchSpecRef patchspec);
    Patch(PatchSpecRef patchspec, std::unordered_map<std::string, NodeRef> params);
    Patch(std::string name);
    virtual ~Patch();

    /**--------------------------------------------------------------------------------
     * Query the current playback state of this patch.
     * @returns A signalflow_patch_state_t
     *--------------------------------------------------------------------------------*/
    signalflow_patch_state_t get_state();

    /**--------------------------------------------------------------------------------
     * Set a named patch input to a constant value.
     * @param name The name of the input
     * @param value The value to set
     *--------------------------------------------------------------------------------*/
    void set_input(std::string name, float value);
    void set_input(std::string name, NodeRef value);
    void set_input(std::string name, BufferRef value);

    /**--------------------------------------------------------------------------------
     * Stop the playback of this patch.
     *--------------------------------------------------------------------------------*/
    void stop();

    /**--------------------------------------------------------------------------------
     * Query the patch's auto_free property.
     * If true, the patch will disconnect its output when its `auto_free_node`
     * state becomes STOPPED.
     *
     * @returns The auto_free property
     *--------------------------------------------------------------------------------*/
    bool get_auto_free();
    void set_auto_free(bool value);

    /**--------------------------------------------------------------------------------
     * Query the node that is used to set the auto_free event.
     * When this node transitions to STOPPED, the patch stops.
     *
     * @returns The auto_free node.
     *--------------------------------------------------------------------------------*/
    NodeRef get_auto_free_node();
    void set_auto_free_node(NodeRef node);

    /**--------------------------------------------------------------------------------
     * Query the node that is used when triggering the patch.
     *
     * @returns The trigger node.
     *--------------------------------------------------------------------------------*/
    NodeRef get_trigger_node();
    void set_trigger_node(NodeRef node);

    /**--------------------------------------------------------------------------------
     * Enumerates the names and nodes corresponding to this patch's inputs.
     *
     * @returns The input map.
     *--------------------------------------------------------------------------------*/
    std::unordered_map<std::string, NodeRef> get_inputs();

    NodeRef add_input(std::string name, sample default_value = 0);
    BufferRef add_buffer_input(std::string name);

    std::set<NodeRef> get_nodes();
    NodeRef add_node(NodeRef node);

    NodeRef get_output();
    void set_output(NodeRef out);

    /**--------------------------------------------------------------------------------
     * Get the AudioGraph that this Patch is in.
     * @returns The AudioGraph
     *--------------------------------------------------------------------------------*/
    AudioGraph *get_graph();

    /**--------------------------------------------------------------------------------
     * Parse the nodes within a Patch, beginning at the output and iterating
     * to inputs. This is necessary for functions such as auto-free.
     * If the Patch has already been parsed, which is implicitly true if
     * it has been instantiated from a PatchSpec, returns without performing
     * any actions.
     *
     *--------------------------------------------------------------------------------*/
    void parse();

    /**--------------------------------------------------------------------------------
     * Trigger this Patch.
     *--------------------------------------------------------------------------------*/
    void trigger();

    /**--------------------------------------------------------------------------------
     * Parse a template from live Node objects to create a network of NodeDefs
     *--------------------------------------------------------------------------------*/
    PatchSpecRef create_spec();

protected:
    /**--------------------------------------------------------------------------------
     * Called by Node members when the node state changes.
     * Used to trigger auto_free.
     *--------------------------------------------------------------------------------*/
    void node_state_changed(Node *node);

private:
    std::string name;
    NodeRef output = nullptr;
    std::unordered_map<std::string, NodeRef> inputs;
    std::unordered_map<std::string, BufferRef> buffer_inputs;
    std::set<NodeRef> nodes;

    bool auto_free;
    NodeRef auto_free_node;
    NodeRef trigger_node;
    signalflow_patch_state_t state;
    AudioGraph *graph;

    NodeRef instantiate(PatchNodeSpec *nodespec);
    void set_state(signalflow_patch_state_t state);
    void iterate_from_node(const NodeRef &node);

    /**--------------------------------------------------------------------------------
     * Properties and methods used when creating a patch prototype,
     * used in create_spec()
     *--------------------------------------------------------------------------------*/
    bool parsed = false;
    int last_id = 0;

    std::set<NodeRef> parsed_nodes;
    std::map<int, PatchNodeSpec *> nodespecs;

    PatchNodeSpec *create_spec_from_node(const NodeRef &node);
    std::string get_input_name(const NodeRef &node);
    std::string get_input_name(const BufferRef &buf);

    friend class Node;
};

}
