#pragma once

#include "signal/patch/nodespec.h"
#include "signal/patch/patchspec.h"

namespace libsignal
{

typedef enum
{
    SIGNAL_SYNTH_STATE_ACTIVE,
    SIGNAL_SYNTH_STATE_FINISHED
} signal_patch_state_t;

class Patch
{
public:
    Patch();
    Patch(PatchSpecRef patchspec);
    Patch(std::string name);
    virtual ~Patch();

    signal_patch_state_t get_state();
    void set_input(std::string name, NodeRef value);
    void set_input(std::string name, BufferRef value);
    void disconnect();
    bool get_auto_free();
    void set_auto_free(bool value);
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

    /*----------------------------------------------------------------------------------
     * Parse a template from live Node objects to create a network of NodeDefs
     *---------------------------------------------------------------------------------*/
    PatchSpecRef create_spec();

    std::string name;

private:
    NodeRef instantiate(NodeSpec *nodespec);
    void set_state(signal_patch_state_t state);
    bool auto_free;
    signal_patch_state_t state;
    AudioGraph *graph;

    /*----------------------------------------------------------------------------------
     * Template stuff
     *---------------------------------------------------------------------------------*/
    int last_id = 0;

    std::string _get_input_name(const NodeRef &node);
    NodeSpec _parse_from_node(const NodeRef &node);
    std::unordered_map<int, NodeSpec> nodespecs;
    std::set<NodeRef> parsed_nodes;
};

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
};

typedef PatchRefTemplate<Patch> PatchRef;

}
