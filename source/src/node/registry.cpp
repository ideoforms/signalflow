#include "signalflow/node/registry.h"
#include "signalflow/patch/patch-spec.h"
#include "signalflow/patch/patch.h"

#include <stdlib.h>

namespace signalflow
{

static NodeRegistry *registry;

NodeRegistry::NodeRegistry()
{
}

NodeRegistry::~NodeRegistry()
{
    if (this == registry)
    {
        registry = 0;
    }
}

NodeRegistry *NodeRegistry::global()
{
    if (!registry)
        registry = new NodeRegistry();
    return registry;
}

Node *NodeRegistry::create(std::string name)
{
    if (!registry->classes[name])
    {
        throw std::runtime_error("Could not instantiate Node (unknown type: " + name + ")");
    }
    Node *node = registry->classes[name]();
    return node;
}

}
