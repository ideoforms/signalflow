#include "signal/patch/patchregistry.h"
#include "signal/patch/patch.h"

namespace libsignal
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

Patch *PatchRegistry::create(std::string name)
{
    if (!this->patchspecs[name])
    {
        throw std::runtime_error("Could not instantiate patch (unknown name: " + name + ")");
    }
    Patch *patch = new Patch(this->patchspecs[name]);
    return patch;
}

}
