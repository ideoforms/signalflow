#include "signalflow/patch/patch-registry.h"
#include "signalflow/patch/patch.h"

namespace signalflow
{

PatchRegistry::PatchRegistry()
{
}

PatchRegistry *PatchRegistry::global()
{
    static PatchRegistry *registry;
    if (!registry)
        registry = new PatchRegistry();
    return registry;
}

void PatchRegistry::add(std::string name, PatchSpecRef patchspec)
{
    this->patchspecs[name] = patchspec;
}

PatchSpecRef PatchRegistry::get(std::string name)
{
    return this->patchspecs[name];
}

PatchRef PatchRegistry::create(std::string name)
{
    if (!this->patchspecs[name])
    {
        throw std::runtime_error("Could not instantiate patch (unknown name: " + name + ")");
    }
    PatchRef patch = new Patch(this->patchspecs[name]);
    return patch;
}

}
