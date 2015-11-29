#pragma once

#include "../constants.h"
#include "../unit.h"
#include "../registry.h"

namespace signum::op
{

class Multiply : public BinaryOpUnit
{

public:

	Multiply(UnitRef a = 1, UnitRef b = 1) : BinaryOpUnit(a, b)
	{
		this->name = "multiply";
	}

	static bool init()
	{
		NodeRegistry *reg = NodeRegistry::global();
		reg->add<Multiply>("multiply");
		return false;
	}

	virtual void next(sample **out, int num_frames)
	{
		UnitRef a = this->inputs[0];
		UnitRef b = this->inputs[1];
		for (int frame = 0; frame < num_frames; frame++)
		{
			for (int channel = 0; channel < this->channels_out; channel++)
			{
				out[channel][frame] = a->out[channel][frame] * b->out[channel][frame];
			}
		}
	}
};


REGISTER(Multiply, "multiply");
// NodeRegistry::global()->add<class>(name);
// static bool Multiply_ok = NodeRegistry::global()->add<Multiply>("multiply");


}

