#pragma once

#include "signal/node/node.h"
#include "signal/synth/nodedef.h"
#include "signal/synth/synthspec.h"

#include <unordered_map>
#include <set>

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
    int last_id = 0;

    std::string _get_input_name(const NodeRef &node);
    NodeDefinition _parse_from_node(const NodeRef &node);
    std::unordered_map<int, NodeDefinition> nodedefs;
    std::unordered_map<std::string, Node *> inputs;
    std::set<NodeRef>parsed_nodes;
    std::set<NodeRef>nodes;
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
