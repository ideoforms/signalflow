#pragma once

#include "signal/node/node.h"
#include "signal/synth/nodedef.h"
#include "signal/synth/synthspec.h"

namespace libsignal
{
class SynthTemplate
{
public:
    SynthTemplate(std::string name);

    /*----------------------------------------------------------------------------------
     * Methods for creating a SynthTemplate from live Node objects.
     *---------------------------------------------------------------------------------*/
    NodeRef add_input(std::string name, sample default_value = 0);
    NodeRef add_node(NodeRef node);
    void set_output(const NodeRef &out);

    /*----------------------------------------------------------------------------------
     * Parse a template from live Node objects to create a network of NodeDefs
     *---------------------------------------------------------------------------------*/
    SynthSpecRef parse();

    std::string name;

private:
    NodeRef output = nullptr;
    SynthSpecRef spec = nullptr;
    bool parsed = false;
    int last_id = 0;

    std::string get_input_name(const NodeRef &node);
    NodeDefinition parse_root(const NodeRef &node);
    std::unordered_map<int, NodeDefinition> nodedefs;

    std::unordered_map<std::string, Node *> inputs;
};

template <class T>
class SynthTemplateRefTemplate : public std::shared_ptr<T>
{
public:
    SynthTemplateRefTemplate()
        : std::shared_ptr<T>(nullptr) {}
    SynthTemplateRefTemplate(T *ptr)
        : std::shared_ptr<T>(ptr) {}
};

typedef SynthTemplateRefTemplate<SynthTemplate> SynthTemplateRef;

}
