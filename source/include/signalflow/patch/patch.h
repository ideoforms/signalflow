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
    Patch(PatchRef patch);
    Patch(PatchRef patch, std::unordered_map<std::string, NodeRef> params);
    Patch(PatchSpecRef patchspec);
    Patch(PatchSpecRef patchspec, std::unordered_map<std::string, NodeRef> params);
    Patch(std::string name);
    virtual ~Patch();

    signalflow_patch_state_t get_state();
    void set_input(std::string name, float value);
    void set_input(std::string name, NodeRef value);
    void set_input(std::string name, BufferRef value);
    void disconnect();
    bool get_auto_free();
    void set_auto_free(bool value);
    NodeRef get_auto_free_node();
    void set_auto_free_node(NodeRef node);
    void node_state_changed(Node *node);

    NodeRef output = nullptr;
    std::unordered_map<std::string, NodeRef> inputs;
    std::unordered_map<std::string, BufferRef> buffer_inputs;
    std::set<NodeRef> nodes;

    /*----------------------------------------------------------------------------------
     * Methods for creating a Patch template from live Node objects.
     *---------------------------------------------------------------------------------*/
    NodeRef add_input(std::string name, sample default_value = 0);

    BufferRef add_buffer_input(std::string name);
    NodeRef add_node(NodeRef node);
    void set_output(NodeRef out);

    AudioGraph *get_graph();

    /**------------------------------------------------------------------------
     * Parse the nodes within a Patch, beginning at the output and iterating
     * to inputs. This is necessary for functions such as auto-free.
     * If the Patch has already been parsed, which is implicitly true if
     * it has been instantiated from a PatchSpec, returns without performing
     * any actions.
     *
     *-----------------------------------------------------------------------*/
    void parse();

    /*----------------------------------------------------------------------------------
     * Parse a template from live Node objects to create a network of NodeDefs
     *---------------------------------------------------------------------------------*/
    PatchSpecRef create_spec();

    std::string name;

private:
    NodeRef instantiate(PatchNodeSpec *nodespec);
    void set_state(signalflow_patch_state_t state);
    bool auto_free;
    NodeRef auto_free_node;
    signalflow_patch_state_t state;
    AudioGraph *graph;

    void _iterate_from_node(const NodeRef &node);

    /*----------------------------------------------------------------------------------
     * Template stuff
     *---------------------------------------------------------------------------------*/
    int last_id = 0;

    std::string _get_input_name(const NodeRef &node);
    std::string _get_input_name(const BufferRef &buf);
    PatchNodeSpec *_create_spec_from_node(const NodeRef &node);
    std::map<int, PatchNodeSpec *> nodespecs;
    std::set<NodeRef> parsed_nodes;
    bool parsed = false;
};

}
