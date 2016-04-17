#include "registry.h"
#include "structure.h"
#include "synth.h"

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
