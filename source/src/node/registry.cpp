#include "signal/node/registry.h"
#include "signal/patch/patch.h"
#include "signal/patch/patchspec.h"

#include <stdlib.h>

namespace libsignal
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
