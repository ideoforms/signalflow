#include "signal/synth/synthregistry.h"
#include "signal/synth/synth.h"

namespace libsignal
{

SynthRegistry::SynthRegistry()
{
}

SynthRegistry *SynthRegistry::global()
{
    static SynthRegistry *registry;
    if (!registry)
        registry = new SynthRegistry();
    return registry;
}

void SynthRegistry::add(std::string name, SynthSpecRef synthspec)
{
    this->synthspecs[name] = synthspec;
}

SynthSpecRef SynthRegistry::get(std::string name)
{
    return this->synthspecs[name];
}

Synth *SynthRegistry::create(std::string name)
{
    if (!this->synthspecs[name])
    {
        throw std::runtime_error("Could not instantiate synth (unknown name: " + name + ")");
    }
    Synth *synth = new Synth(this->synthspecs[name]);
    return synth;
}

}
