#pragma once

#include "signal/synth/nodedef.h"
#include "signal/synth/synthspec.h"
#include "signal/synth/synthtemplate.h"

namespace libsignal
{

typedef enum
{
    SIGNAL_SYNTH_STATE_ACTIVE,
    SIGNAL_SYNTH_STATE_FINISHED
} signal_synth_state_t;


class Synth
{
public:
    Synth();
    Synth(SynthSpecRef synthspec);
    Synth(SynthTemplateRef synthtemplate);
    Synth(std::string name);
    virtual ~Synth();

    signal_synth_state_t get_state();
    void set_input(std::string name, NodeRef value);
    void disconnect();
    bool get_auto_free();
    void set_auto_free(bool value);
    void node_state_changed(Node *node);

    NodeRef output = nullptr;
    std::unordered_map<std::string, NodeRef> inputs;
    std::set<NodeRef> nodes;

    /*----------------------------------------------------------------------------------
     * Methods for creating a Synth template from live Node objects.
     *---------------------------------------------------------------------------------*/
    NodeRef add_input(std::string name, sample default_value = 0);
    NodeRef add_node(NodeRef node);
    void set_output(NodeRef out);

    /*----------------------------------------------------------------------------------
     * Parse a template from live Node objects to create a network of NodeDefs
     *---------------------------------------------------------------------------------*/
    SynthSpecRef parse();

    std::string name;

private:
    NodeRef instantiate(NodeDefinition *nodedef);
    void set_state(signal_synth_state_t state);
    bool auto_free;
    signal_synth_state_t state;
    AudioGraph *graph;

    /*----------------------------------------------------------------------------------
     * Template stuff
     *---------------------------------------------------------------------------------*/
    int last_id = 0;

    std::string _get_input_name(const NodeRef &node);
    NodeDefinition _parse_from_node(const NodeRef &node);
    std::unordered_map<int, NodeDefinition> nodedefs;
    std::set<NodeRef>parsed_nodes;
};


template <class T>
class SynthRefTemplate : public std::shared_ptr<T>
{
public:
    SynthRefTemplate()
        : std::shared_ptr<T>(nullptr) {}
    SynthRefTemplate(Synth *ptr)
        : std::shared_ptr<T>(ptr) {}
    SynthRefTemplate(SynthSpecRef synthspec)
        : std::shared_ptr<T>(new T(synthspec)) {}
};

typedef SynthRefTemplate<Synth> SynthRef;


}
