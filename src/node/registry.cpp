#include "signal/node/registry.h"
#include "signal/synth/synth.h"
#include "signal/synth/synthspec.h"

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
        fprintf(stderr, "Could not instantiate object (unknown type: %s)\n", name.c_str());
        exit(1);
    }
    Node *object = this->classes[name]();
    return object;
}

}
