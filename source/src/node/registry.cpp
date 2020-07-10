#include "signalflow/node/registry.h"
#include "signalflow/patch/patch.h"
#include "signalflow/patch/patchspec.h"

#include <stdlib.h>

namespace signalflow
{

NodeRegistry::NodeRegistry()
{
}

NodeRegistry *NodeRegistry::global()
{
    static NodeRegistry *registry;
    if (!registry)
        registry = new NodeRegistry();
    return registry;
}

Node *NodeRegistry::create(std::string name)
{
    if (!this->classes[name])
    {
        throw std::runtime_error("Could not instantiate Node (unknown type: " + name + ")");
    }
    Node *node = this->classes[name]();
    return node;
}

}
