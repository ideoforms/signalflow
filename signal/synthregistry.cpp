#include "synthregistry.h"
#include "synth.h"

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

void SynthRegistry::add(std::string name, StructRef structure)
{
	this->structures[name] = structure;
}

StructRef SynthRegistry::get(std::string name)
{
	return this->structures[name];
}


Synth *SynthRegistry::create(std::string name)
{
	if (!this->structures[name])
	{
		fprintf(stderr, "Could not instantiate synth (unknown name: %s)\n", name.c_str());
		exit(1);
	}
	Synth *synth = new Synth(this->structures[name]);
	return synth;
}


}
